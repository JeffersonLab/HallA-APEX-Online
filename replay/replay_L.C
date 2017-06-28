//HRS-L only replay script

#include "/adaqfs/home/a-onl/tritium/replay/def_coinc.h"
using namespace std;

void replay_L(Int_t runnumber=0,Int_t all=50000,Int_t fstEvt=0,Bool_t QuietRun = kFALSE){

  //Enable modules
  Bool_t bBeam=kTRUE;
  Bool_t bScaler=kTRUE;
  Bool_t bHelicity=kFALSE;
  Bool_t bPhysics=kTRUE;
  Bool_t bPlots=kFALSE;
  Bool_t bEloss=kTRUE;
  
  char* RNAME="%s/coinc_test_%d.root"; 
  char* ODEF="/adaqfs/home/a-onl/tritium/replay/LHRS.odef";
  char* CUTS="/adaqfs/home/a-onl/tritium/replay/LHRS.cuts";
  
  //==================================
  //  Detectors
  //==================================
  //THaApparatus* HRSL = new THaHRS("L","Left arm HRS"); //Adds vdc,s1,s2
  THaApparatus* HRSL = new Gmp_HRS("L","Left arm HRS"); //Add vdc,s2...uses s0 for track beta
  gHaApps->Add( HRSL );
  HRSL->AddDetector( new Gmp_Xscin("s0","s0 scintillator",kFALSE) );
  HRSL->AddDetector( new THaFpp("fpp","Focal Plane Polarimeters"));
  HRSL->AddDetector( new THaCherenkov("cer", "Gas Cherenkov counter" ));
  HRSL->AddDetector( new THaShower("prl1", "Pre-shower pion rej." ));
  HRSL->AddDetector( new THaShower("prl2", "Show pion rej." ));
  // For using new GmpShower class, comment the previous two lines and uncomment
  // the following two lines
  //HRSL->AddDetector( new GmpShower("prl1", "Pre-shower pion rej." ));
  //HRSL->AddDetector( new GmpShower("prl2", "Show pion rej." ));
  
  //==================================
  //  Scaler
  //==================================
  if(bScaler){
    THaScalerGroup* LeftScalers = new THaScalerGroup("Left");
    THaScalerGroup* EvLeftScalers = new THaScalerGroup("evleft");
    LeftScalers->GetScalerObj()->SetClockRate(103700);
    EvLeftScalers->GetScalerObj()->SetClockRate(103700);
    gHaScalers->Add(LeftScalers);
    gHaScalers->Add(EvLeftScalers);

  }

  //==================================
  //  NormAna Class
  //==================================
   //THaNormAna* norm = new THaNormAna("N","Normalization Analysis");
   //GmpNormAna* norm = new GmpNormAna("N","Normalization Analysis","Left");
   GmpNormAna* norm = new GmpNormAna("N","Left");
   gHaPhysics->Add(norm);
   GmpNormAna* evnorm = new GmpNormAna("N","evLeft");
   gHaPhysics->Add(evnorm);

  //==================================
  //  Decoder Data
  //==================================
   THaApparatus* dec = new THaDecData("DL","Misc. Decoder Data");
   gHaApps->Add(dec);
  
  //==================================
  //  Beam
  //==================================

  if(bBeam){
    THaIdealBeam *ib = new THaIdealBeam("ib", "Ideal beam");
    gHaApps->Add(ib);
    
    THaUnRasteredBeam *Lurb = new THaUnRasteredBeam("Lurb","Unrastered beam to L-HRS");
    gHaApps->Add(Lurb);
    
    if(runnumber<10000){
      THaRasteredBeam *Lrb = new THaRasteredBeam("Lrb","Rastered beam to L-HRS");
      gHaApps->Add(Lrb);
    }
    else{
      LukeBeam *Lrbluke = new LukeBeam("Lrbluke","Rastered beam to L-HRS"); //Does not use Raster information when determining beam parameters
      gHaApps->Add(Lrbluke);
      
      Gmp_Rastered_Beam *LrbGmp = new Gmp_Rastered_Beam("LrbGmp","Rastered beam to L-HRS");
      gHaApps->Add(LrbGmp);      
    }
  }

  TString beamchoice;
  
  if(runnumber<10000) beamchoice = "Lrb";
  else beamchoice = "LrbGmp";
  
  //==================================
  //  Physics
  //==================================
  if(bPhysics){
     Double_t mass_el  = 0.000511; // electron mass
     Double_t amu = 931.494*1.e-3; // amu to GeV
     Double_t mass_C12 = 12.0*amu;
     Double_t mass_hydr = 0.93827;
     Double_t mass_tg = mass_hydr; //default target 
  
     THaPhysicsModule *Lgold = new THaGoldenTrack( "L.gold", "HRS-L Golden Track", "L" );
     gHaPhysics->Add(Lgold);
     
     THaPhysicsModule *EKL = new THaElectronKine("EKL","Electron kinematics in HRS-L","L","ib",mass_tg); //Should be same if no beam included in constructor
     gHaPhysics->Add(EKL);
     
     THaPhysicsModule *EKLc = new THaElectronKine("EKLc","Corrected Electron kinematics in HRS-L","L",beamchoice,mass_tg);
     gHaPhysics->Add(EKLc);

     THaReactionPoint *rpl = new THaReactionPoint("rpl","Reaction vertex for HRS-L","L",beamchoice);
     gHaPhysics->Add(rpl);
     
     THaExtTarCor *exL =  new THaExtTarCor("exL","Corrected for extended target, HRS-L","L","rpl");
     gHaPhysics->Add(exL);

     THaPhysicsModule *EKLx = new THaElectronKine("EKLx","Better Corrected Electron kinematics in HRS-L","exL",beamchoice,mass_tg);
     gHaPhysics->Add(EKLx);
     
     if(bEloss){
       // Beam Energy Loss
       Double_t zbeam_off = -0.075 ; //For a target centered at z=0, this should equal to the targetlength/2. (in m)
       
       Gmp_Beam_Eloss *ElbL = new Gmp_Beam_Eloss("ElbL","Beam Corrected for Energy Loss",beamchoice,"rpl",zbeam_off);
       ElbL->SetDebug(1);
       ElbL->SetMedium(1.,1.00727,0.0723); // Set medium assuming LH2 Target. According to the Cryotarget Training Slides,
                                           // the density should be 0.0723 g/cc (agrees more or less w/ NIST table).
       gHaPhysics->Add(ElbL);
       
       //Track Energy Loss
       Double_t targ_length = 0.15 ; // In meters. Set to 15 cm for GMp LH2 target
       Double_t ztrack_off = 0. ; //For a target centered at z=0, this should equal to 0. (in m)
       Double_t air_length = 0.3868; // In meters. Set to 0.3543 m for RHRS and 0.2697 m for LHRS for Spring 16.
                                     //            Set to 0.3757 m for RHRS and 0.3868 m for LHRS for Fall 16.
        
       Gmp_Track_Eloss *EltL = new Gmp_Track_Eloss("EltL","Track Corrected for Energy Loss","exL","rpl",targ_length,ztrack_off,air_length);
       EltL->SetDebug(1);
       EltL->SetMedium(1.,1.00727,0.0723); // See above for explanation.
       gHaPhysics->Add(EltL);

       THaPhysicsModule *EKLxe = new THaElectronKine("EKLxe","Best Corrected Electron kinematics in HRS-L","EltL","ElbL",mass_tg);
       gHaPhysics->Add(EKLxe);

     }
     
     //THaPrimaryKine *PKL = new THaPrimaryKine("PKL","kinematics of scattering of electron to HRS-L","L",mass_el,mass_tg); //Same as EKL
     //THaPrimaryKine *PKL = new THaPrimaryKine("PKL","kinematics of scattering of electron to HRS-L","L",beamchoice,mass_tg); //Same as EKLc
     //THaPrimaryKine *PKL = new THaPrimaryKine("PKL","kinematics of scattering of electron to HRS-L","exL",beamchoice,mass_tg); //Same as EKLx
     //gHaPhysics->Add(PKL);
   }
  
  //=====================================
  //  Set up Analyzer and replay data
  //=====================================
  ReplayCore(
	     runnumber,         //run #
	     0,                 //-1=replay all;0=ask for a number
	     all,               //default replay event num
	     RNAME,             //output file format
	     ODEF,	        //out define
	     CUTS, 	        //empty cut define
	     bScaler,           //replay scalar?
	     bHelicity,         //repaly helicity
	     fstEvt,		//First Event To Replay
	     QuietRun		//whether ask user for inputs
	     );

  //=====================================
  //Generate online plots
  //=====================================

  if(bPlots){
    char* CONFIGFILE="/adaqfs/home/a-onl/tritium/replay/onlineGUI64/LHRS.cfg";
    char* CONFIGFILEPHYS="/adaqfs/home/a-onl/tritium/replay/onlineGUI64/LHRS_phy.cfg";
    
    //gSystem->Exec(Form("~/tritium/replay/printonl %s %d", CONFIGFILE, runnumber));
    gSystem->Exec(Form("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/online -P -f %s -r %d", CONFIGFILE,runnumber));
    gSystem->Exec(Form("mv /adaqfs/home/a-onl/tritium/replay/summaryfiles/temp_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/detectors_%d.pdf",runnumber,runnumber));
    gSystem->Exec("unlink /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_detectors_latest.pdf");
    gSystem->Exec(Form("ln -s detectors_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_detectors_latest.pdf",runnumber));
    
    //gSystem->Exec(Form("~/tritium/replay/printonl %s %d", CONFIGFILEPHYS, runnumber));
    gSystem->Exec(Form("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/online -P -f %s -r %d", CONFIGFILEPHYS,runnumber));
    gSystem->Exec(Form("mv /adaqfs/home/a-onl/tritium/replay/summaryfiles/temp_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/physics_%d.pdf",runnumber,runnumber));
    gSystem->Exec("unlink /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_physics_latest.pdf");
    gSystem->Exec(Form("ln -s physics_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_physics_latest.pdf",runnumber));
  }
}
 




