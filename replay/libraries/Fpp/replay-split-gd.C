#include "TString.h"
#include "TObject.h"

// this is a class to hold standard CODA split file names.
// It can sort the filenames by run number, and then split order.
class TFNames : public TObject {
private:
  TString fString;
public:
  // just override the Compare method
  TFNames( const char* s="") : fString(s) { }
  TFNames( const TFNames &s ) : TObject(), fString(s.fString) { }
  virtual Int_t Compare(const TObject *obj) const {
    if (this==obj) return 0;
    if (!dynamic_cast<TFNames*>(obj)) return -1;
    TString tail1 = fString;
    TString tail2 = ((TFNames*)obj)->fString;
    Int_t p = tail1.Last('.');
    if (p>=0) tail1.Remove(0,p+1);
    p = tail2.Last('.');
    if (p>=0) tail2.Remove(0,p+1);
    int v1 = atoi(tail1.Data());
    int v2 = atoi(tail2.Data());
    if (v1>v2) return 1;
    if (v2>v1) return -1;
    return 0;
  }
  virtual Bool_t IsSortable() const { return kTRUE; }
  virtual Bool_t IsEqual(const TObject *obj) {
    return Compare(obj)==0;
  }

  void Print(Option_t *opt="") {
    cout << "TFNames = " << fString << endl;
  }
  
  TString& String() { return fString; }
};
void insertsorted(TList *l, TFNames *fn) {
  // put fn into the list l according to its sorted value
  // Needed because for an "interpretted" class, the pointers
  // do not cast properly for their virtual methods.
  TObject *o=0;
  TFNames *lf;
  TIter next(l);
  next.Reset();
  int cnt=0;
  while ( (o=next()) && (lf=(TFNames*)o) && (fn->Compare(lf)>0) ) { };
  if (o) {
    if (fn->Compare(lf)==0) {
      cout << "Warning! Attempted to insert file twice: "
           << lf->String() << " and " << fn->String() << endl;
    } else {
      l->AddBefore(o,fn);
    }
  }
  else l->Add(fn);
}

TList *filelist(const char* prefix, int runno,
		const char* rawdir[]) {
  // USE THIS METHOD to build a list of raw CODA files
  //  for run 'runno', where the files are to be found in
  //  the list of directories given by "rawdir".
  // Prefix is used to build the name of the file, so the
  //   name is   "prefix_runno.dat.n"

  TString basename = Form("%s_%d.dat.*",prefix,runno);

  TList *l = new TList;
  const char* *ptr = rawdir;
  char line[200];
  TString cmd;
  for (ptr = rawdir; ptr && *ptr; ptr++) {
    cmd = Form("ls -1 %s/",*ptr);
    cmd += basename;
    FILE *cmdo = gSystem->OpenPipe(cmd.Data(),"r");
    while (fscanf(cmdo,"%s",line)==1) {
      if (strncmp("ls: ",line,3)==0) continue; // error message
      insertsorted(l, new TFNames(line) );
    }
    gSystem->ClosePipe(cmdo);
  }

  TIter next(l);
  TObject *o;
  TFNames *os;
  cout << "Files in the list are: " << endl;
  while ( o = next() ) {
    os = (TFNames*)o;
    cout << os->String() << endl;
  }
  return l;
}


void replay_split_gd(int runno=-1) {
  //
  //  Steering script for Hall A analyzer demo
  //
  gSystem->Load("/usr/lib/gcc-lib/i386-redhat-linux/3.2.3/libg2c.so");
  gSystem->Load("/work/halla/ledex/disk1/adaq/analyzer/libFpp");
  gSystem->Load("/work/halla/ledex/disk1/adaq/analyzer/libHaHelicity");
  gSystem->Load("/work/halla/ledex/disk1/analyzer/libLEDEX");

  THaApparatus* HRSL = new THaHRS("L","Left arm HRS");
  gHaApps->Add( HRSL );
  HRSL->AddDetector(new THaFpp("fpp","Focal Plane Polarimeters"));
  HRSL->AddDetector( new THaCherenkov("cer", "Gas Cherenkov counter" ));
  HRSL->AddDetector( new THaScintillator("s0","Scintillator S0"));

  // Add an unrastered, ideally positioned and directed electron beam
  THaApparatus* BEAM = new THaIdealBeam("Beam","Simple ideal beamline");
  gHaApps->Add( BEAM );
  
  // Collect information about a easily modified random set of channels
  // (see DB_DIR/*/db_D.dat)
  THaApparatus* DECDAT = new THaDecData("D","Misc. Decoder Data");
  gHaApps->Add( DECDAT );
  
  // add scalers
  THaScalerGroup* LeftScalers = new THaScalerGroup("Left");
  LeftScalers->Init();
  gHaScalers->Add(LeftScalers);

  THaHelicityDet *hleft = new THaHelicityDet("HL","Beam helicity L-arm");
  hleft->SetState (1, 8, -1, 0, 0);  // G0 mode; 8 window delay; sign -1;
                                     // left arm; no redund
  hleft->SetROC (0, 11, 0, 3, 0, 4); // "Left arm" is ROC 11
  BEAM->AddDetector(hleft);

  // Prepare the physics modules to use

  // Watch the electron kinematics
  Double_t amu = 931.494*1.e-3;  // amu to GeV
  Double_t mass_he3 = 2.809413; // in GeV
  Double_t mass_C12 = 12.0*amu; // AMU to GeV
  Double_t mass_tg  = mass_C12;  // chosen for Carbon elastic calibration runs

  // Plain, uncorrected Electron kinematics
  THaPhysicsModule* EK_l = new THaElectronKine("EK_L",
					       "Electron kinematics in HRS-L",
					       "L",mass_tg);
  
  gHaPhysics->Add( EK_l );
   
//To get number of T3 triggers
//   THaNormAna* norm = new THaNormAna("N","Normalization Analysis");
//   gHaPhysics->Add(norm);

  // Use the reaction vertex to correct for an extended target
  // Cheating for now, and assuming a perfect, unrastered electron beam
   THaPhysicsModule *Rpt_l = new THaReactionPoint("ReactPt_L","Reaction vertex for Left",
						"L","Beam");
  gHaPhysics->Add( Rpt_l );

//Physics modules: golden track & proton single arm kinematics
  gHaPhysics->Add( new THaGoldenTrack( "L.gold", "HRS-L Golden Track", "L" ));
  gHaPhysics->Add( new GammaModule("GM","Gamma Reconstruction from HRSL","L"));
  gHaPhysics->Add( new SAProtonModule("SAP","Proton in HRSL","L"));

  // Correct for using an Extended target
  // This needs information about the Point of interaction (hence a THaVertexModule)
  THaPhysicsModule* TgC_l = new THaExtTarCor("ExTgtCor_L",
					     "Corrected for extended target, HRS-L",
					     "L","ReactPt_L");
  
  gHaPhysics->Add( TgC_l );

  // Finally, the CORRECTED Electron kinematics (note change of "spectrometer")
  THaPhysicsModule* EKxc_l = new THaElectronKine("EKxc_L","Electron kinematics in HRS-L",
  						"ExTgtCor_L",mass_tg);
  
  gHaPhysics->Add( EKxc_l );

  // Set up the analyzer - we use the standard one,
  // but this could be an experiment-specific one as well.
  // The Analyzer controls the reading of the data, executes
  // tests/cuts, loops over Apparatus's and PhysicsModules,
  // and executes the output routines.
  THaAnalyzer* analyzer = new THaAnalyzer;
  analyzer->EnableScalers();
  
  // A simple event class to be output to the resulting tree.
  // Creating your own descendant of THaEvent is one way of
  // defining and controlling the output.
  THaEvent* event = new THaEvent;
  
  // Define the analysis parameters
  analyzer->SetEvent( event );
  analyzer->SetOutFile( "Afile.root" );
  analyzer->SetOdefFile("replay_output_gd.def");
  analyzer->SetCutFile("cuts.def");        // don't want T9 triggers

  analyzer->EnableBenchmarks();
  
  // File to record cuts accounting information
  analyzer->SetSummaryFile("summary_example.log"); // optional

  cout << "runno = " << runno << endl;
  while (runno<=0) {
    cout << "What run number to analyze? " << endl;
    cin >> runno;
  }
  
  const char* rawpaths[] = {
    // list of directories that the raw CODA files can be found
    "/adaql2/data1",
    "/adaql2/data2",
    "/adaql2/data3",
    "/adaql2/data4",
    "/cache/mss/halla/ndelta/raw",
    ".",   // useful for processing on the farm computers.
    0
  };

  TList *flist = filelist("ledex",runno,rawpaths);
  // Process each file in the list
  TIter next(flist);
  TFNames *fn;
  THaRun *run, *oldrun=0;
  while ( fn = (TFNames*)next() ) {
    cout << "  Trying for file " << fn->String() << endl;
    run = new THaRun(fn->String());
    
    if (oldrun) {
      run->SetDate(oldrun->GetDate());
      run->SetNumber(oldrun->GetNumber());
    }
//    if (nevt>=0) {
//      run->SetLastEvent(1000);
//    }
    analyzer->Process(run); // analyze this run
    run->Close();
    if (!oldrun) oldrun = run;
  }

}


