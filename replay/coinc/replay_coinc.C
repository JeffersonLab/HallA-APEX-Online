//HRS-R replay script

#include "/adaqfs/home/a-onl/tritium/replay/def_coinc.h"
using namespace std;

void replay_coinc(Int_t runnumber=0,Int_t all=50000,Int_t fstEvt=0,Bool_t QuietRun = kFALSE){

  char buf[300];
  Int_t nrun=0;
  if (runnumber<=0)
  {
	    cout << "\nreplay: Please enter a Run Number (-1 to exit):";
	    cin >> nrun;
	    fgets(buf,300,stdin);//get the extra '\n' from stdin
	    if( nrun<=0 ) break;
	    runnumber = nrun;
  }

  
  //Enable modules
  Bool_t bBeam=kTRUE;
  Bool_t bScaler=kTRUE;
  Bool_t bHelicity=kFALSE;
  Bool_t bPhysics=kTRUE;
  Bool_t bPlots=kTRUE; //not open GUI automatically
  Bool_t bEloss=kFALSE;
 

  char* RNAME="%s/coinc_test_%d.root";
  char* ODEF="/adaqfs/home/a-onl/tritium/replay/coinc.odef";
  char* CUTS="/adaqfs/home/a-onl/tritium/replay/coinc.cuts";

  // if (all > 50000 || all < -1) RNAME="%s/coinc_fullreplay_%d.root";
  //==================================
  //  Detectors
  //==================================
  //THaApparatus* HRSR = new THaHRS("R","Right arm HRS"); //Adds vdc,s1,s2
  THaApparatus* HRSR = new Gmp_HRS("R","Right arm HRS"); //Add vdc,s2...uses s0 for track beta
  //HRSR->SetDebug(1);
  gHaApps->Add( HRSR );
  HRSR->AddDetector( new Gmp_Xscin("s0","s0 scintillator",kTRUE) );
  HRSR->AddDetector( new THaFpp("fpp","Focal Plane Polarimeters"));
  HRSR->AddDetector( new THaCherenkov("cer", "Gas Cherenkov counter" ));
  HRSR->AddDetector( new THaShower("ps", "Pre-shower pion rej." ));
  HRSR->AddDetector( new THaShower("sh", "Show pion rej." ));

  //==================================
  //  Detectors
  //==================================
  //THaApparatus* HRSL = new THaHRS("L","Left arm HRS"); //Adds vdc,s1,s2
  THaApparatus *HRSL = new Gmp_HRS("L","Left arm HRS"); //Add vdc,s2...uses s0 for track beta
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
    THaScalerGroup* RightScalers = new THaScalerGroup("Right");

    	RightScalers->GetScalerObj()->SetClockRate(103700);
    gHaScalers->Add(RightScalers); 
	 
	 THaScalerGroup* EvRightScalers = new THaScalerGroup("evright");

    EvRightScalers->GetScalerObj()->SetClockRate(103700);
    gHaScalers->Add(EvRightScalers);


    //has to be checked if it works F.H 07.02.17
    THaScalerGroup* LeftScalers = new THaScalerGroup("Left");
 //  THaScalerGroup* EvLeftScalers = new THaScalerGroup("evleft");
    LeftScalers->GetScalerObj()->SetClockRate(103700);
//     EvLeftScalers->GetScalerObj()->SetClockRate(103700);
    gHaScalers->Add(LeftScalers);
//     gHaScalers->Add(EvLeftScalers);
  }

  
  //==================================
  //  NormAna Class
  //==================================
 // THaNormAna* norm = new THaNormAna("N","Normalization Analysis");
//   GmpNormAna* norm = new GmpNormAna("N","Normalization Analysis","Right");
//   GmpNormAna* norm = new GmpNormAna("N","NormRight");
//   gHaPhysics->Add(norm);
//   GmpNormAna* evnorm = new GmpNormAna("N","evRight");
//   gHaPhysics->Add(evnorm);
    //unclear if both norms are working
//   GmpNormAna* normL = new GmpNormAna("N","Left");
//   gHaPhysics->Add(norm);
//   GmpNormAna* evnormL = new GmpNormAna("N","evLeft");
//   gHaPhysics->Add(evnorm);

  
  //==================================
  //  Decoder Data
  //==================================
  THaApparatus* decR = new THaDecData("DR","Misc. Decoder Data");
  gHaApps->Add( decR );
//   THaApparatus* decL = new THaDecData("DL","Misc. Decoder Data");
//   gHaApps->Add( decL );
  
  //==================================
  //  Beam
  //==================================

  if(bBeam){
    THaIdealBeam* ib = new THaIdealBeam("ib","Ideal beam");
    gHaApps->Add(ib);

    THaUnRasteredBeam* Rurb = new THaUnRasteredBeam("Rurb","Unrastered beam to R-HRS");
    gHaApps->Add(Rurb);


   //   LukeBeam *Rrbluke = new LukeBeam("Rrbluke","Rastered beam to R-HRS"); //Does not use Raster information when determining beam parameters
   //   gHaApps->Add(Rrbluke);

      Gmp_Rastered_Beam *RrbGmp = new Gmp_Rastered_Beam("RrbGmp","Rastered beam to R-HRS");
      gHaApps->Add(RrbGmp);      


      THaUnRasteredBeam *Lurb = new THaUnRasteredBeam("Lurb","Unrastered beam to L-HRS");
      gHaApps->Add(Lurb);

   //   LukeBeam *Lrbluke = new LukeBeam("Lrbluke","Rastered beam to L-HRS"); //Does not use Raster information when determining beam parameters
   //   gHaApps->Add(Lrbluke);

      Gmp_Rastered_Beam *LrbGmp = new Gmp_Rastered_Beam("LrbGmp","Rastered beam to L-HRS");
      gHaApps->Add(LrbGmp);   

  }

  TString beamchoice;
  /* 
  if(runnumber<20678) beamchoice = "Rrb";
  else beamchoice = "RrbGmp";
  */
 beamchoice = "Rurb";
 // beamchoice = "Lurb";
  
  //==================================
  //  Physics
  //==================================
  if(bPhysics){
     Double_t mass_el  = 0.000511; // electron mass
     Double_t amu = 931.494*1.e-3; // amu to GeV
     Double_t mass_C12 = 12.0*amu;
     Double_t mass_hydr = 0.93827;
     Double_t mass_argon = 40*amu;
     Double_t mass_tg = mass_argon; //default target 
  
     THaPhysicsModule *Lgold = new THaGoldenTrack( "L.gold", "HRS-L Golden Track", "L" );
     gHaPhysics->Add(Lgold);

     THaPhysicsModule *EKL = new THaPrimaryKine("EKL","Electron kinematics in HRS-L","L","ib",mass_tg); //Should be same if no beam included in constructor
     gHaPhysics->Add(EKL);

     THaPhysicsModule *EKLc = new THaPrimaryKine("EKLc","Corrected Electron kinematics in HRS-L","L",beamchoice,mass_tg);
     gHaPhysics->Add(EKLc);

     THaReactionPoint *rpl = new THaReactionPoint("rpl","Reaction vertex for HRS-L","L",beamchoice);
     gHaPhysics->Add(rpl);

     THaExtTarCor *exL =  new THaExtTarCor("exL","Corrected for extended target, HRS-L","L","rpl");
     gHaPhysics->Add(exL);

     THaPhysicsModule *EKLx = new THaPrimaryKine("EKLx","Better Corrected Electron kinematics in HRS-L","exL",beamchoice,mass_tg);
     gHaPhysics->Add(EKLx);

     THaPhysicsModule *Rgold = new THaGoldenTrack( "R.gold", "HRS-R Golden Track", "R" );
     gHaPhysics->Add(Rgold);

     THaPhysicsModule *PKR = new THaSecondaryKine("PKR","Proton kinematics in HRS-R","R","EKL",mass_hydr); //Should be same if no beam included in constructor
     gHaPhysics->Add(PKR);

     THaPhysicsModule *PKRc = new THaSecondaryKine("PKRc","Corrected Proton kinematics in HRS-R","R","EKLc",mass_hydr);
     gHaPhysics->Add(PKRc);

     THaReactionPoint *rpr = new THaReactionPoint("rpr","Reaction vertex for HRS-R","R",beamchoice);
     gHaPhysics->Add(rpr);

     THaExtTarCor *exR =  new THaExtTarCor("exR","Corrected for extended target, HRS-R","R","rpr");
     gHaPhysics->Add(exR);

     THaPhysicsModule *PKRx = new THaSecondaryKine("PKRx","Better Corrected Proton kinematics in HRS-R","exR","EKLx",mass_hydr);
     gHaPhysics->Add(PKRx);
     

     if(bEloss){

       // Beam Energy Loss
       Double_t zbeam_off = -0.075 ; //For a target centered at z=0, this should equal to the targetlength/2. (in m)
       
       Gmp_Beam_Eloss *ElbR = new Gmp_Beam_Eloss("ElbR","Beam Corrected for Energy Loss",beamchoice,"rpr",zbeam_off);
       ElbR->SetDebug(1);
       ElbR->SetMedium(1.,1.00727,0.0723); // Set medium assuming LH2 Target. According to the Cryotarget Training Slides,
                                           // the density should be 0.0723 g/cc (agrees more or less w/ NIST table).
       gHaPhysics->Add(ElbR);
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
       
       Gmp_Track_Eloss *EltR = new Gmp_Track_Eloss("EltR","Track Corrected for Energy Loss","exR","rpr",targ_length,ztrack_off,air_length);
       EltR->SetDebug(1);
       EltR->SetMedium(1.,1.00727,0.0723); // See above for explanation.
       gHaPhysics->Add(EltR);

       THaPhysicsModule *EKRxe = new THaElectronKine("EKRxe","Best Corrected Electron kinematics in HRS-R","EltR","ElbR",mass_tg);
       gHaPhysics->Add(EKRxe);

       Gmp_Track_Eloss *EltL = new Gmp_Track_Eloss("EltL","Track Corrected for Energy Loss","exL","rpl",targ_length,ztrack_off,air_length);
       EltL->SetDebug(1);
       EltL->SetMedium(1.,1.00727,0.0723); // See above for explanation.
       gHaPhysics->Add(EltL);

       THaPhysicsModule *EKLxe = new THaElectronKine("EKLxe","Best Corrected Electron kinematics in HRS-L","EltL","ElbL",mass_tg);
       gHaPhysics->Add(EKLxe);

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
	     ODEF,	       //out define
	     CUTS, 	       //empty cut define
	     bScaler,          //replay scalar?
	     bHelicity,        //repaly helicity
	     fstEvt,	       //First Event To Replay
	     QuietRun	       //whether ask user for inputs
	     );

  //=====================================
  //Generate online plots
  //=====================================
  if(bPlots){
    char* CONFIGFILE="/adaqfs/home/a-onl/tritium/replay/onlineGUI64/RHRS.cfg";
    char* CONFIGFILEPHYS="/adaqfs/home/a-onl/tritium/replay/onlineGUI64/RHRS_phy.cfg";
    

    gSystem->Exec(Form("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/online -P -f %s -r %d", CONFIGFILE,runnumber));
    gSystem->Exec(Form("mv /adaqfs/home/a-onl/tritium/replay/summaryfiles/temp_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/right_detectors_%d.pdf",runnumber,runnumber));
    gSystem->Exec("unlink /adaqfs/home/a-onl/tritium/replay/summaryfiles/right_detectors_latest.pdf");
    gSystem->Exec(Form("ln -s right_detectors_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/right_detectors_latest.pdf",runnumber));
    

    gSystem->Exec(Form("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/online -P -f %s -r %d", CONFIGFILEPHYS,runnumber));
    gSystem->Exec(Form("mv /adaqfs/home/a-onl/tritium/replay/summaryfiles/temp_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/right_physics_%d.pdf",runnumber,runnumber));
    gSystem->Exec("unlink /adaqfs/home/a-onl/tritium/replay/summaryfiles/right_physics_latest.pdf");
    gSystem->Exec(Form("ln -s right_physics_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/right_physics_latest.pdf",runnumber));

    char* CONFIGFILE_L="/adaqfs/home/a-onl/tritium/replay/onlineGUI64/LHRS.cfg";
    char* CONFIGFILEPHYS_L="/adaqfs/home/a-onl/tritium/replay/onlineGUI64/LHRS_phy.cfg";

    gSystem->Exec(Form("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/online -P -f %s -r %d", CONFIGFILE_L,runnumber));
    gSystem->Exec(Form("mv /adaqfs/home/a-onl/tritium/replay/summaryfiles/temp_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_detectors_%d.pdf",runnumber,runnumber));
    gSystem->Exec("unlink /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_detectors_latest.pdf");
    gSystem->Exec(Form("ln -s left_detectors_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_detectors_latest.pdf",runnumber));

    gSystem->Exec(Form("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/online -P -f %s -r %d", CONFIGFILEPHYS_L,runnumber));
    gSystem->Exec(Form("mv /adaqfs/home/a-onl/tritium/replay/summaryfiles/temp_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_physics_%d.pdf",runnumber,runnumber));
    gSystem->Exec("unlink /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_physics_latest.pdf");
    gSystem->Exec(Form("ln -s left_physics_%d.pdf /adaqfs/home/a-onl/tritium/replay/summaryfiles/left_physics_latest.pdf",runnumber));
  }
  exit();
}
 

