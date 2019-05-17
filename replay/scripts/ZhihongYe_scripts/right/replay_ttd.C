/* VDC TDC calibration replay 
   Jens-Ole Hansen  ole@jlab.org  24-Jun-2010
   Requires analyzer 1.5.15 or higher
*/

#ifndef __CINT__
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <iostream>
#include "THaAnalyzer.h"
#include "THaHRS.h"
#include "THaVDC.h"
#include "THaRun.h"
#include "THaGlobals.h"
#include "THaDecData.h"

using namespace std;

TString getrun( Int_t irun );
Int_t getnev();

#endif

//-------------------------------------------------------------------------
void replay_ttd( const char* arm="L", Int_t runnumber=-1, Int_t nev=-1 ) {

  TString spec(arm);
  spec.ToUpper();
  if( spec != "L" && spec != "R" ) {
    cout << "usage: replay_ttd(\"L\"/\"R\", runno)" << endl;
    return;
  }

  TString codafile = getrun( runnumber );
  if( codafile.IsNull() ) return;

  THaAnalyzer* analyzer = THaAnalyzer::GetInstance();
  if( analyzer ) {
    analyzer->Close();
  } else {
    analyzer = new THaAnalyzer;
  }

  // Set up the HRS spectrometer
  TString dscrpt = (spec == "L" ? "Left" : "Right");
  dscrpt.Append(" HRS");
  THaHRS* hrs = new THaHRS(spec, dscrpt);
  THaDetector* vdc = hrs->GetDetector("vdc");
  if( !vdc ) { cout << "Error: HRS has no VDC?" << endl; return; }
  vdc->SetDebug(1);
  gHaApps->Add(hrs);

  // Decoder data
  THaApparatus* dec = new THaDecData("D","Misc. Decoder Data");
  gHaApps->Add( dec );

  TString rootfile(Form("rootfiles/ttdcalib_%s_%d.root",
			spec.Data(),runnumber));
  analyzer->SetOutFile(rootfile);
  analyzer->SetOdefFile(Form("replay_ttd_%s.odef",spec.Data()));
  analyzer->SetCutFile(Form("replay_ttd_%s.cdef",spec.Data()));
  analyzer->SetMarkInterval(5000);
  analyzer->SetCompressionLevel(1);
  analyzer->EnableBenchmarks();

  THaRun* run = new THaRun(codafile);
  run->SetLastEvent(nev);

  // Action
  analyzer->Process(run);

  analyzer->Close();
  delete run; run = 0;
  delete analyzer; analyzer = 0;
  gHaApps->Delete();
  gHaPhysics->Delete();

  new TFile(rootfile,"r");
}

//-------------------------------------------------------------------------
TString getrun( Int_t irun )
{
  // Get the run to analyze
  const char* paths[] = {
    ".",
    "/adaql2/data1",
    "/adaql2/data2",
    "/adaql2/data3",
    "/adaql2/data4",
    //    "/cache/mss/halla/apex/raw",
    0
  };
    // Define the analysis parameters
  
  if(irun<0){
    cout << "Run number?" << endl;
    cin >> irun;
  }
  if( irun<0 )
    return TString();

  TString filename;
  bool found = false;
  const char** path = paths;
  while ( path && *path ) {         
    filename = Form("%s/apex_%d.dat.0",*path,irun);
    gSystem->ExpandPathName(filename);
    if( !gSystem->AccessPathName( filename, kReadPermission )) {
      found = true;
      cout << "Will analyze file " << filename << endl;
      break;
    }
    path++;
  }
  if ( (!path || !*path) && !found ) {
    cout << "File not found.  Try again, or -1 to quit" << endl;
    filename = "";
  }
  return filename;
}

//-------------------------------------------------------------------------
Int_t getnev() {
  int nev;
  cout << "Number of events to analyze? (-1 for all)?" << endl;
  cin >> nev;
  return nev;
}
