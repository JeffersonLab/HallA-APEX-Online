#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>


void Load_rootfile(Int_t runnum=3000, Bool_t Scifi_flag=false){

  //  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(Form("apex_%d.root",runnum));


  TString ROOTFILE_DIR =   "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_%d.root";

  if (Scifi_flag){
    // for SciFi replays
    ROOTFILE_DIR =   "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_SciFi_%d.root";
  }


  TChain *T = new TChain("T");


  TString filenamebase = Form(ROOTFILE_DIR,runnum);
  //  T->Add(Form(ROOTFILE_DIR,run_number));
  //  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));

  TString filename = filenamebase;

  filenamebase.Remove(filenamebase.Last('.'),5);

  Long_t split = 0;
  while ( !gSystem->AccessPathName(filename.Data()) ) {
    cout << "Added root file: " << filename << " to Tree" << endl;
    T->Add(filename);
    split++;
    filename = filenamebase + "_" + split + ".root";
  }



  cout << "Opened Run " << runnum << " with "  << T->GetEntries() << " events and " << split  << " file splits" <<  endl;
   



  // if (!f || !f->IsOpen()) {
  //   f = TFile::Open(Form("apex_%d.root",runnum));
  //   cout << "open line" << endl;
  // }


}
