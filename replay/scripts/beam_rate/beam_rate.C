#include "../def_apex.h"


void beam_rate(Int_t run_1=0, Int_t run_2=1){


  TChain *T = new TChain("T");


  TString filenamebase;
  TString filename;
  Long_t split = 0;

  for(Int_t i = 0; i <= (run_2-run_1); i++){



    
    filenamebase = Form(ROOTFILE_DIR,run_1+i);
    
    filename = filenamebase;
    
    filenamebase.Remove(filenamebase.Last('.'),5);
    
    cout << filename << endl;
    
    split = 0;
    while ( !gSystem->AccessPathName(filename.Data()) ) {
      cout << "Added root file: " << filename << " to Tree" << endl;
      T->Add(filename);
    split++;
    filename = filenamebase + "_" + split + ".root";
    }
    
  }

  cout << T->GetEntries() << endl;

  T->SetMarkerStyle(2);
  //  T->Draw("Rightunew_r:RightT2_r");

  TCanvas* c1 = new TCanvas("c1","c1");

  T->Draw("(RightT2_r/Rightunew_r):Entry$","Rightunew_r>0");


  TCanvas* c2 = new TCanvas("c2","c2");

  T->Draw("(RightT2_r/Rightunew_r):Entry$","abs(Rightunew_r-48300)<500");


  TCanvas* c3 = new TCanvas("c3","c3");

  T->Draw("(RightT2_r/Rightunew_r):RightRF_Time","Rightunew_r>0");
  
  
  TCanvas* c4 = new TCanvas("c4","c4");

  T->Draw("(RightT2_r/Rightunew_r):Rightunew_r","Rightunew_r>0");

 
  TCanvas* c5 = new TCanvas("c5","c5");

  T->Draw("RightT2_r:Rightunew_r>>h1(50000,-10000,60000,10000,80000","");


  // Rclock_r
  
}
