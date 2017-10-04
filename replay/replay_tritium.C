/* 
 * Tritium Replay Script
 * Works for non-coincidence experiments using 2 daqs
 */

///////////////////////////////////// To Do ////////////////////////////////////
/*
 * - Correctly set run number ranges for each arm
 * - Automatically open Online Plots GUI for shift takers?
 * - Make work for coincidence as well? Or use separate script?
 * - Do we use GMP S0 Class or stock? GMP class needs to be updated to be 1.6 compatible
 * - Are we using energy loss classes? Need to be made 1.6 compatible
 */

#include "/adaqfs/home/a-onl/tritium/replay/def_tritium.h"
using namespace std;

#define RIGHT_ARM_CONDITION nrun>=20000
#define LEFT_ARM_CONDITION nrun<20000

void replay_tritium(Int_t runnumber=0,Int_t all=50000,Int_t fstEvt=0,Bool_t QuietRun = kFALSE){

  char buf[300];
  Int_t nrun=0;
  if (runnumber<=0)
  {
	    cout << "\nreplay: Please enter a Run Number (-1 to exit):";
	    cin >> nrun;
	    fgets(buf,300,stdin);//get the extra '\n' from stdin
	    if( nrun<=0 ) return;
	    runnumber = nrun;
  }

  
  //Enable modules
  Bool_t bScaler=kTRUE;
  Bool_t bHelicity=kFALSE;
  Bool_t bBeam=kFALSE;
  Bool_t bPhysics=kFALSE;
  Bool_t bPlots=kFALSE; //not open GUI automatically
  Bool_t bEloss=kFALSE;
 

  const char* RNAME="%s/tritium_%d.root";
  TString ODEF;
  TString CUTS;

  //==================================
  //  Right Arm Detectors
  //==================================
  
  if(RIGHT_ARM_CONDITION){
    ODEF=Form(REPLAY_DIR_PREFIX,"RHRS.odef");
    CUTS=Form(REPLAY_DIR_PREFIX,"RHRS.cuts");
    //==================================
    //  Detectors
    //==================================
    THaHRS* HRSR = new THaHRS("R","Right arm HRS");
    HRSR->AutoStandardDetectors(kFALSE);
    gHaApps->Add( HRSR );
    HRSR->AddDetector( new TriFadcXscin("s0","s0 scintillator",kTRUE) );
    HRSR->AddDetector( new THaVDC("vdc", "Vertical Drift Chamber" ));
    HRSR->AddDetector( new TriFadcCherenkov("cer", "Gas Cherenkov counter - FADC" ));
    HRSR->AddDetector( new TriFadcScin("s2", "S2 Scintillator - FADC" ));
    HRSR->AddDetector( new THaShower("ps", "Pre-shower pion rej." ));
    HRSR->AddDetector( new THaShower("sh", "Show pion rej." ));
    
    THaHRS* FbusHRSR = new THaHRS("FbusR", "Fastbus RHRS Readout");
    FbusHRSR->AutoStandardDetectors(kFALSE);
    gHaApps->Add(FbusHRSR);
    FbusHRSR->AddDetector( new THaCherenkov("cer", "Gas Cherenkov counter - Fastbus"));
    FbusHRSR->AddDetector( new THaScintillator("s2", "S2 Scintillator - Fastbus"));
    FbusHRSR->AddDetector( new Tritium_Xscin("s0", "S0 Scintillator - Fastbus", kTRUE));

    //==================================
    //  Scalers
    //==================================
    if(bScaler){
      THaScalerEvtHandler* rscaler = new THaScalerEvtHandler("Right","HA scaler event type 140 on R-HRS");
      gHaEvtHandlers->Add(rscaler);
    }

    //==================================
    //  Decoder Data
    //==================================
    THaApparatus* decR = new THaDecData("DR","Misc. Decoder Data");
    gHaApps->Add( decR );

    //==================================
    //  Beam
    //==================================
    if(bBeam){
      THaIdealBeam* ib = new THaIdealBeam("ib","Ideal beam");
      gHaApps->Add(ib);

      THaRasteredBeam* Rrb = new THaRasteredBeam("Rrb", "Rastered beam to R-HRS");
      Rrb->AddDetector(new THaRaster("Raster2", "Downstream Raster"));
      Rrb->AddDetector(new THaBPM("BPMA", "First BPM"));
      Rrb->AddDetector(new THaBPM("BPMB", "Second BPM"));
      gHaApps->Add(Rrb);
    }
    
    //==================================
    //  Physics
    //==================================
    if(bPhysics){
      Double_t mass_el  = 0.000511; // electron mass
      Double_t amu = 931.494*1.e-3; // amu to GeV
      Double_t mass_He3 = 3.0160293*amu;
      Double_t mass_H2 = 2.01410178*amu;
      Double_t mass_H3 = 3.0160492*amu;
      Double_t mass_tg = mass_H3; //default target 
  
      THaPhysicsModule *Rgold = new THaGoldenTrack( "R.gold", "HRS-R Golden Track", "R" );
      gHaPhysics->Add(Rgold);

      THaPhysicsModule *EKR = new THaPrimaryKine("EKR","Electron kinematics in HRS-R","R","ib",mass_tg); //Should be same if no beam included in constructor
      gHaPhysics->Add(EKR);

      THaPhysicsModule *EKRc = new THaPrimaryKine("EKRc","Corrected Electron kinematics in HRS-R","R","Rrb",mass_tg);
      gHaPhysics->Add(EKRc);

      THaReactionPoint *rpr = new THaReactionPoint("rpr","Reaction vertex for HRS-R","R","Rrb");
      gHaPhysics->Add(rpr);

      THaExtTarCor *exR =  new THaExtTarCor("exR","Corrected for extended target, HRS-R","R","rpr");
      gHaPhysics->Add(exR);

      THaPhysicsModule *EKRx = new THaPrimaryKine("EKRx","Better Corrected Electron kinematics in HRS-R","exR","Rrb",mass_tg);
      gHaPhysics->Add(EKRx);

      /*if(bEloss){
        // Beam Energy Loss
        Double_t zbeam_off = -0.075 ; //For a target centered at z=0, this should equal to the targetlength/2. (in m)
       
        Gmp_Beam_Eloss *ElbR = new Gmp_Beam_Eloss("ElbR","Beam Corrected for Energy Loss",beamchoice,"rpr",zbeam_off);
        ElbR->SetDebug(1);
        ElbR->SetMedium(1.,1.00727,0.0723); // Set medium assuming LH2 Target. According to the Cryotarget Training Slides,
                                           // the density should be 0.0723 g/cc (agrees more or less w/ NIST table).
        gHaPhysics->Add(ElbR);
        
        //Track Energy Loss
        Double_t targ_length = 0.15 ; // In meters. Set to 15 cm for GMp LH2 target
        Double_t ztrack_off = 0. ; //For a target centered at z=0, this should equal to 0. (in m)
        Double_t air_length = 0.3757; // In meters. Set to 0.3543 m for RHRS and 0.2697 m for LHRS for Spring 16.
                                     //            Set to 0.3757 m for RHRS and 0.3868 m for LHRS for Fall 16.
       
        Gmp_Track_Eloss *EltR = new Gmp_Track_Eloss("EltR","Track Corrected for Energy Loss","exR","rpr",targ_length,ztrack_off,air_length);
        EltR->SetDebug(1);
        EltR->SetMedium(1.,1.00727,0.0723); // See above for explanation.
        gHaPhysics->Add(EltR);

        THaPhysicsModule *EKRxe = new THaElectronKine("EKRxe","Best Corrected Electron kinematics in HRS-R","EltR","ElbR",mass_tg);
        gHaPhysics->Add(EKRxe);
      }*/
    }
  }

  //==================================
  //  Left Arm
  //==================================
  
  else if(LEFT_ARM_CONDITION){
    ODEF=Form(REPLAY_DIR_PREFIX,"LHRS.odef");
    CUTS=Form(REPLAY_DIR_PREFIX,"LHRS.cuts");
    //==================================
    //  Detectors
    //==================================
    THaHRS *HRSL = new THaHRS("L","Left arm HRS"); //Add vdc,s2...uses s0 for track beta
    HRSL->AutoStandardDetectors(kFALSE);
    gHaApps->Add( HRSL );
    HRSL->AddDetector( new TriFadcXscin("s0","s0 scintillator",kFALSE) );
    HRSL->AddDetector( new THaVDC("vdc", "Vertical Drift Chamber"));
    HRSL->AddDetector( new TriFadcCherenkov("cer", "Gas Cherenkov counter" ));
    HRSL->AddDetector( new TriFadcScin("s2", "S2 Scintillator" ));
    HRSL->AddDetector( new THaShower("prl1", "Pre-shower pion rej." ));
    HRSL->AddDetector( new THaShower("prl2", "Show pion rej." ));
    
    THaHRS* FbusHRSL = new THaHRS("FbusL", "Fastbus LHRS Readout");
    FbusHRSL->AutoStandardDetectors(kFALSE);
    gHaApps->Add(FbusHRSL);
    FbusHRSL->AddDetector( new THaCherenkov("cer", "Gas Cherenkov counter - Fastbus"));
    FbusHRSL->AddDetector( new THaScintillator("s2", "S2 Scintillator - Fastbus"));
    FbusHRSL->AddDetector( new Tritium_Xscin("s0", "S0 Scintillator - Fastbus", kFALSE));

    //==================================
    //  Scaler
    //==================================
    if(bScaler){
    THaScalerEvtHandler* lscaler = new THaScalerEvtHandler("Left","HA scaler event type 140 on R-HRS");
    gHaEvtHandlers->Add(lscaler);
    }

    //==================================
    //  Decoder Data
    //==================================
    THaApparatus* decL = new THaDecData("DL","Misc. Decoder Data");
    gHaApps->Add( decL );

    //==================================
    //  Beam
    //==================================
    if(bBeam){
      THaIdealBeam* ib = new THaIdealBeam("ib","Ideal beam");
      gHaApps->Add(ib);

      THaRasteredBeam* Lrb = new THaRasteredBeam("Lrb", "Rastered beam to L-HRS");
      Lrb->AddDetector(new THaRaster("Raster2", "Downstream Raster"));
      Lrb->AddDetector(new THaBPM("BPMA", "First BPM"));
      Lrb->AddDetector(new THaBPM("BPMB", "Second BPM"));
      gHaApps->Add(Lrb);
    }
    
    //==================================
    //  Physics 
    //==================================
    
    if(bPhysics){
      Double_t mass_el  = 0.000511; // electron mass
      Double_t amu = 931.494*1.e-3; // amu to GeV
      Double_t mass_He3 = 3.0160293*amu;
      Double_t mass_H2 = 2.01410178*amu;
      Double_t mass_H3 = 3.0160492*amu;
      Double_t mass_tg = mass_H3; //default target 
  
      THaPhysicsModule *Lgold = new THaGoldenTrack( "L.gold", "HRS-L Golden Track", "L" );
      gHaPhysics->Add(Lgold);

      THaPhysicsModule *EKL = new THaPrimaryKine("EKL","Electron kinematics in HRS-L","L","ib",mass_tg); //Should be same if no beam included in constructor
      gHaPhysics->Add(EKL);

      THaPhysicsModule *EKLc = new THaPrimaryKine("EKLc","Corrected Electron kinematics in HRS-L","L","Lrb",mass_tg);
      gHaPhysics->Add(EKLc);

      THaReactionPoint *rpl = new THaReactionPoint("rpl","Reaction vertex for HRS-L","L","Lrb");
      gHaPhysics->Add(rpl);

      THaExtTarCor *exL =  new THaExtTarCor("exL","Corrected for extended target, HRS-L","L","rpl");
      gHaPhysics->Add(exL);

      THaPhysicsModule *EKLx = new THaPrimaryKine("EKLx","Better Corrected Electron kinematics in HRS-L","exL","Lrb",mass_tg);
      gHaPhysics->Add(EKLx);
      
      /*if(bEloss){
        // Beam Energy Loss
        Double_t zbeam_off = -0.075 ; //For a target centered at z=0, this should equal to the targetlength/2. (in m)
        
        Gmp_Beam_Eloss *ElbL = new Gmp_Beam_Eloss("ElbL","Beam Corrected for Energy Loss",beamchoice,"rpr",zbeam_off);
        ElbL->SetDebug(1);
        ElbL->SetMedium(1.,1.00727,0.0723); // Set medium assuming LH2 Target. According to the Cryotarget Training Slides,
                                           // the density should be 0.0723 g/cc (agrees more or less w/ NIST table).
        gHaPhysics->Add(ElbL);
        
        //Track Energy Loss
        Double_t targ_length = 0.15 ; // In meters. Set to 15 cm for GMp LH2 target
        Double_t ztrack_off = 0. ; //For a target centered at z=0, this should equal to 0. (in m)
        Double_t air_length = 0.3757; // In meters. Set to 0.3543 m for RHRS and 0.2697 m for LHRS for Spring 16.
                                      //            Set to 0.3757 m for RHRS and 0.3868 m for LHRS for Fall 16.
        
        Gmp_Track_Eloss *EltL = new Gmp_Track_Eloss("EltL","Track Corrected for Energy Loss","exL","rpl",targ_length,ztrack_off,air_length);
        EltL->SetDebug(1);
        EltL->SetMedium(1.,1.00727,0.0723); // See above for explanation.
        gHaPhysics->Add(EltL);

        THaPhysicsModule *EKLxe = new THaElectronKine("EKLxe","Best Corrected Electron kinematics in HRS-L","EltL","ElbL",mass_tg);
        gHaPhysics->Add(EKLxe);
      }*/
    }
  }
  
  //=====================================
  //  Set up Analyzer and replay data
  //=====================================
  ReplayCore(
	     runnumber,        //run #
	     0,                //-1=replay all;0=ask for a number
	     all,              //default replay event num
	     RNAME,            //output file format
	     ODEF.Data(),	       //out define
	     CUTS.Data(), 	       //empty cut define
	     bScaler,          //replay scalar?
	     bHelicity,        //repaly helicity
	     fstEvt,	       //First Event To Replay
	     QuietRun	       //whether ask user for inputs
	     );

  //=====================================
  //Generate online plots
  //=====================================
  if(bPlots){
  const char* GUI_DIR = Form(REPLAY_DIR_PREFIX,"onlineGUI64/");
  const char* SUM_DIR = Form(REPLAY_DIR_PREFIX,"summaryfiles/");
    if(RIGHT_ARM_CONDITION){
      const char* CONFIGFILE=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS.cfg");
      const char* CONFIGFILEPHYS=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS_phy.cfg");
      

      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILE,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf %sright_detectors_%d.pdf",SUM_DIR,runnumber,SUM_DIR,runnumber));
      gSystem->Exec(Form("unlink %sright_detectors_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s right_detectors_%d.pdf %sright_detectors_latest.pdf",runnumber,SUM_DIR));
      

      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf %sright_physics_%d.pdf",SUM_DIR,runnumber,SUM_DIR,runnumber));
      gSystem->Exec(Form("unlink %sright_physics_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s right_physics_%d.pdf %sright_physics_latest.pdf",runnumber,SUM_DIR));
    }
    else if(LEFT_ARM_CONDITION){ 
      const char* CONFIGFILE_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS.cfg");
      const char* CONFIGFILEPHYS_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_phy.cfg");

      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILE_L,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf %sleft_detectors_%d.pdf",SUM_DIR,runnumber,SUM_DIR,runnumber));
      gSystem->Exec(Form("unlink %sleft_detectors_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s left_detectors_%d.pdf %sleft_detectors_latest.pdf",runnumber,SUM_DIR));

      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS_L,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf %sleft_physics_%d.pdf",SUM_DIR,runnumber,SUM_DIR,runnumber));
      gSystem->Exec(Form("unlink %sleft_physics_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s left_physics_%d.pdf %sleft_physics_latest.pdf",runnumber,SUM_DIR));
    }
  }
  exit(0);
}
 

