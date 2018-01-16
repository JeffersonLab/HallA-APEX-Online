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

#include "def_tritium.h"
using namespace std;

#define RIGHT_ARM_CONDITION runnumber>=20000
#define LEFT_ARM_CONDITION runnumber<20000

void replay_tritium(Int_t runnumber=0,Int_t numevents=0,Int_t fstEvt=0,Bool_t QuietRun = kFALSE, Bool_t OnlineReplay = kFALSE, Bool_t bPlots = kFALSE,Bool_t autoreplay = kFALSE){

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
  Bool_t bBeam=kTRUE;
  Bool_t bPhysics=kTRUE;
  Bool_t bEloss=kFALSE;
  Bool_t bOldTrack=kFALSE;
  
  TString rootname;
  if(OnlineReplay){
    rootname = "%s/tritium_online_%d.root";
  }else{
    rootname = "%s/tritium_%d.root";
  }


  const char* RNAME=rootname.Data();
  TString ODEF;
  TString CUTS;

  //==================================
  //  Right Arm Detectors
  //==================================

  if(RIGHT_ARM_CONDITION){
    ODEF=Form(REPLAY_DIR_PREFIX,"RHRS.odef");
    if(autoreplay)  ODEF=Form(REPLAY_DIR_PREFIX,"RHRS_auto.odef");
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
    //FbusHRSR->AddDetector( new THaScintillator("s2", "S2 Scintillator - Fastbus"));
    FbusHRSR->AddDetector( new Tritium_Xscin("s0", "S0 Scintillator - Fastbus", kTRUE));

    if(bOldTrack){
       THaApparatus* OldTrackR = new TriHRS("OldTrackR","old analyzer track");
       OldTrackR->AddDetector( new TriXscin("s0","s0 sintillator",kTRUE) );
       gHaApps->Add( OldTrackR );
     }

    //==================================
    //  Scalers
    //==================================
    if(bScaler){
      TriScalerEvtHandler* rscaler = new TriScalerEvtHandler("Right","HA scaler event type 140 on R-HRS");
      gHaEvtHandlers->Add(rscaler);

      Tritium_TSScaler* revscaler = new Tritium_TSScaler("evRight","HA scaler event type 1-14 on R-HRS");
      gHaEvtHandlers->Add(revscaler);

      Tritium_THaScaler100EvtHandler* rEndscaler = new Tritium_THaScaler100EvtHandler("EndRight","HA scaler event type 100");
      gHaEvtHandlers->Add(rEndscaler);

      // Marco - F1 and VETROC tdcs:
      gHaEvtHandlers->Add (new TdcDataEvtHandler("RTDC","F1 and VETROC TDCs rHRS")); // do not change the "RTDC" word
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

      TriFadcRasteredBeam* Rrb = new TriFadcRasteredBeam("Rrb", "Rastered beam to the R-HRS");
      gHaApps->Add(Rrb);

      THaRasteredBeam* FbusRrb = new THaRasteredBeam("FbusRrb", "Fastbus Rastered beam to R-HRS");
      FbusRrb->AddDetector(new THaRaster("Raster2", "Downstream Raster"));
      FbusRrb->AddDetector(new THaBPM("BPMA", "First BPM"));
      FbusRrb->AddDetector(new THaBPM("BPMB", "Second BPM"));
      gHaApps->Add(FbusRrb);
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
      Double_t mass_tg = mass_H3/3.0; //default target 
  
      THaPhysicsModule *Rgold = new THaGoldenTrack( "R.gold", "HRS-R Golden Track", "R" );
      gHaPhysics->Add(Rgold);

      THaPhysicsModule *Rvdceff = new TriVDCeff( "R.vdceff", "Reft vdc efficiency");
      gHaPhysics->Add(Rvdceff);

      THaPhysicsModule *EKR = new THaPrimaryKine("EKR","Electron kinematics in HRS-R","R","ib",mass_tg); //Should be same if no beam included in constructor
      gHaPhysics->Add(EKR);

      THaPhysicsModule *EKRc = new THaPrimaryKine("EKRc","Corrected Electron kinematics in HRS-R","R","Rrb",mass_tg);
      gHaPhysics->Add(EKRc);

      THaReactionPoint *rpr = new THaReactionPoint("rpr","Reaction vertex for HRS-R","R","FbusRrb");
      gHaPhysics->Add(rpr);

      THaExtTarCor *exR =  new THaExtTarCor("exR","Corrected for extended target, HRS-R","R","rpr");
      gHaPhysics->Add(exR);

      THaPhysicsModule *EKRx = new THaPrimaryKine("EKRx","Better Corrected Electron kinematics in HRS-R","exR","Rrb",mass_tg);
      gHaPhysics->Add(EKRx);
      
      THaPhysicsModule* BCM = new TriBCM("RightBCM","Beam Current Monitors","Left","ev",0);
	  gHaPhysics->Add(BCM);

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
    if(autoreplay)  ODEF=Form(REPLAY_DIR_PREFIX,"LHRS_auto.odef");
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

    if(bOldTrack){
       THaApparatus* OldTrackL = new TriHRS("OldTrackL","old analyzer track");
       OldTrackL->AddDetector( new TriXscin("s0","s0 sintillator",kFALSE) );
       gHaApps->Add( OldTrackL );
     }


    //==================================
    //  Scaler
    //==================================
    if(bScaler){
    TriScalerEvtHandler* lscaler = new TriScalerEvtHandler("Left","HA scaler event type 140 on L-HRS");
    gHaEvtHandlers->Add(lscaler);

    Tritium_TSScaler* levscaler = new Tritium_TSScaler("evLeft","HA scaler event type 1-14 on L-HRS");
    gHaEvtHandlers->Add(levscaler);

    Tritium_THaScaler100EvtHandler* lEndscaler = new Tritium_THaScaler100EvtHandler("EndLeft","HA scaler event type 100");
    gHaEvtHandlers->Add(lEndscaler);

    // Marco - for F1 tdc:
    gHaEvtHandlers->Add (new TdcDataEvtHandler("LTDC","F1 TDCs lHRS")); // do not change the "LTDC" word
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

      TriFadcRasteredBeam* Lrb = new TriFadcRasteredBeam("Lrb", "Rastered beam to L-HRS");
      gHaApps->Add(Lrb);

      THaRasteredBeam* FbusLrb = new THaRasteredBeam("FbusLrb", "Fastbus Rastered beam to L-HRS");
      FbusLrb->AddDetector(new THaRaster("Raster2", "Downstream Raster"));
      FbusLrb->AddDetector(new THaBPM("BPMA", "First BPM"));
      FbusLrb->AddDetector(new THaBPM("BPMB", "Second BPM"));
      gHaApps->Add(FbusLrb);  
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
      Double_t mass_tg = mass_H3/3.0; //default target 
  
      THaPhysicsModule *Lgold = new THaGoldenTrack( "L.gold", "HRS-L Golden Track", "L" );
      gHaPhysics->Add(Lgold);
      
      THaPhysicsModule *Lvdceff = new TriVDCeff( "L.vdceff", "Left vdc efficiency");
      gHaPhysics->Add(Lvdceff);

      THaPhysicsModule *EKL = new THaPrimaryKine("EKL","Electron kinematics in HRS-L","L","ib",mass_tg); //Should be same if no beam included in constructor
      gHaPhysics->Add(EKL);

      THaPhysicsModule *EKLc = new THaPrimaryKine("EKLc","Corrected Electron kinematics in HRS-L","L","Lrb",mass_tg);
      gHaPhysics->Add(EKLc);

      THaReactionPoint *rpl = new THaReactionPoint("rpl","Reaction vertex for HRS-L","L","FbusLrb");
      gHaPhysics->Add(rpl);

      THaExtTarCor *exL =  new THaExtTarCor("exL","Corrected for extended target, HRS-L","L","rpl");
      gHaPhysics->Add(exL);

      THaPhysicsModule *EKLx = new THaPrimaryKine("EKLx","Better Corrected Electron kinematics in HRS-L","exL","Lrb",mass_tg);
      gHaPhysics->Add(EKLx);
      
      THaPhysicsModule* BCM = new TriBCM("LeftBCM","Beam Current Monitors","Left","ev",0);
	  gHaPhysics->Add(BCM);

      
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
	     numevents,        //-1=replay all;0=ask for a number
	     50000,            //default replay event num
	     RNAME,            //output file format
	     ODEF.Data(),	   //out define
	     CUTS.Data(), 	   //empty cut define
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
      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs1/work1/tritium/Run_pdfs/right_detectors_%d.pdf",SUM_DIR,runnumber,runnumber));
      gSystem->Exec(Form("unlink %sright_detectors_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s /chafs1/work1/tritium/Run_pdfs/right_detectors_%d.pdf %sright_detectors_latest.pdf",runnumber,SUM_DIR));
      gSystem->Exec(Form("ln -sf /chafs1/work1/tritium/Run_pdfs/right_detectors_%d.pdf /chafs1/work1/tritium/Run_pdfs/right_detectors_latest.pdf",runnumber));
              
      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs1/work1/tritium/Run_pdfs/right_physics_%d.pdf",SUM_DIR,runnumber,runnumber));
      gSystem->Exec(Form("unlink %sright_physics_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s /chafs1/work1/tritium/Run_pdfs/right_physics_%d.pdf %sright_physics_latest.pdf",runnumber,SUM_DIR));    
      gSystem->Exec(Form("ln -sf /chafs1/work1/tritium/Run_pdfs/right_physics_%d.pdf /chafs1/work1/tritium/Run_pdfs/right_physics_latest.pdf",runnumber));
                
      const char* config_online=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS_online.cfg");
	  gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, config_online,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs1/work1/tritium/Run_pdfs/right_online_%d.pdf",SUM_DIR,runnumber,runnumber));
      gSystem->Exec(Form("unlink %sright_online_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s /chafs1/work1/tritium/Run_pdfs/right_online_%d.pdf %sright_online_latest.pdf",runnumber,SUM_DIR)); 
    }
    
    else if(LEFT_ARM_CONDITION){ 
      const char* CONFIGFILE_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS.cfg");
      const char* CONFIGFILEPHYS_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_phy.cfg");
	    
      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILE_L,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs1/work1/tritium/Run_pdfs/left_detectors_%d.pdf",SUM_DIR,runnumber,runnumber));
      gSystem->Exec(Form("unlink %sleft_detectors_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s /chafs1/work1/tritium/Run_pdfs/left_detectors_%d.pdf %sleft_detectors_latest.pdf",runnumber,SUM_DIR));
      gSystem->Exec(Form("ln -sf /chafs1/work1/tritium/Run_pdfs/left_detectors_%d.pdf /chafs1/work1/tritium/Run_pdfs/left_detectors_latest.pdf",runnumber));

      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS_L,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs1/work1/tritium/Run_pdfs/left_physics_%d.pdf",SUM_DIR,runnumber,runnumber));
      gSystem->Exec(Form("unlink %sleft_physics_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s /chafs1/work1/tritium/Run_pdfs/left_physics_%d.pdf %sleft_physics_latest.pdf",runnumber,SUM_DIR));
      gSystem->Exec(Form("ln -sf /chafs1/work1/tritium/Run_pdfs/left_physics_%d.pdf /chafs1/work1/tritium/Run_pdfs/left_physics_latest.pdf",runnumber));
      
      const char* config_online=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_online.cfg");
	  gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, config_online,runnumber));
      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs1/work1/tritium/Run_pdfs/left_online_%d.pdf",SUM_DIR,runnumber,runnumber));
      gSystem->Exec(Form("unlink %sleft_online_latest.pdf",SUM_DIR));
      gSystem->Exec(Form("ln -s /chafs1/work1/tritium/Run_pdfs/left_online_%d.pdf %sleft_online_latest.pdf",runnumber,SUM_DIR));
    }
  }

  exit(0);
}
 

