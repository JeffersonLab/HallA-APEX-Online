#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

using namespace std;

TChain* Load_more_rootfile(Int_t runnum_1=3000,Int_t runnum_2= -1, Bool_t Scifi_flag=false){

  //  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(Form("apex_%d.root",runnum));


   TString ROOTFILE_DIR =   "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_%d.root";

  

  if (Scifi_flag){
    // for SciFi replays
    ROOTFILE_DIR =   "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_SciFi_%d.root";
  }


  TChain *T = new TChain("T");


  TString filenamebase;
  TString filename;

  Long_t split = 0;


  Int_t No_of_runs = 0;

  if( runnum_2 == -1){
    No_of_runs = 0;
  }
  else{
    No_of_runs = runnum_2-runnum_1;
  }
  


  for(Int_t i = 0; i< No_of_runs+1; i++){
    


    filenamebase = Form(ROOTFILE_DIR,runnum_1 + i);
    filename = filenamebase;
    
    cout << "Current runnumber = " << runnum_1+i << endl;
    
    filenamebase.Remove(filenamebase.Last('.'),5);
    
    
    split = 0;
    while ( !gSystem->AccessPathName(filename.Data()) ) {
      cout << "Added root file: " << filename << " to Tree" << endl;
      T->Add(filename);
      split++;
      filename = filenamebase + "_" + split + ".root";
    }
    
  }
  
  //  cout << "Opened Run " << runnum << " with "  << T->GetEntries() << " events and " << split  << " file splits" <<  endl;
   



  // if (!f || !f->IsOpen()) {
  //   f = TFile::Open(Form("apex_%d.root",runnum));
  //   cout << "open line" << endl;
  // }


  return T;

}
