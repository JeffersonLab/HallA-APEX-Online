//////////////////////////////////////////////////////////////////////////
//                                                                          
// rootlogon.C                                                            
//                                                                          
// Load Lib, paths and possible scripts to the analyzer upon start
//                                                                          
//////////////////////////////////////////////////////////////////////////
//	
// Author : Jin Huang <mailto:jinhuang@jlab.org>    Mar 2008
//          Chao Gu         Update for g2p use      Mar 2012
//          Kalyan Allada   Update for GMp expt.    Dec 2013
//	    Florian Hauenstein   Update for Tritum expt.    Feb 2017
//
//////////////////////////////////////////////////////////////////////////

{
  TString Arch(gSystem->GetBuildArch());
  TString Arch32("linux");
  TString Arch64("linuxx8664gcc");
  if(Arch==Arch32){
    printf("\nrootlogon.C: Loading Replay Core Library..."); 
  //  gSystem->Load("/adaqfs/home/a-onl/tritium/replay/old/ReplayCore_C.so");
  }

  else if(Arch==Arch64){
    printf("\nrootlogon.C: Loading Replay Core Library..."); 
    gSystem->Load("/adaqfs/home/a-onl/tritium/replay/ReplayCore64_C.so");
    //gSystem->Load("libraries/Gmp_Beam_Eloss/libGmp_Beam_Eloss.so");
    //gSystem->Load("libraries/Gmp_Track_Eloss/libGmp_Track_Eloss.so");
    //gSystem->Load("libraries/Gmp_Xscin/libGmp_Xscin.so");
  }

    //Load more libs here, if necessary. 
    //Make sure it's in path of $LD_LIBRARY_PATH

    printf("\nrootlogon.C: Adding include directories...");

    gSystem->AddIncludePath("-I$ANALYZER");
    gInterpreter->AddIncludePath("$ANALYZER/");

    gSystem->AddIncludePath("-I$ANALYZER/src");
    gInterpreter->AddIncludePath("$ANALYZER/src/");

    gSystem->AddIncludePath("-I$ANALYZER/hana_decode");
    gInterpreter->AddIncludePath("$ANALYZER/hana_decode/");

    gSystem->AddIncludePath("-I$ANALYZER/hana_scaler");
    gInterpreter->AddIncludePath("$ANALYZER/hana_scaler/");

    printf("\nrootlogon.C: Done!\n\n");
}
