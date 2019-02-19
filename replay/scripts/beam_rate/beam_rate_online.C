#include "../def_apex.h"


void beam_rate_online(){


  TTree *tree = (TTree*)gDirectory->Get("T");


  gStyle->SetOptStat(0);

  //  T->SetMarkerStyle(2);
  //  T->Draw("Rightunew_r:RightT2_r");

  // TCanvas* c1 = new TCanvas("c1","c1");

  TH2F *rt1 = new TH2F("rt1","Target stability",50000,0,50000,10,10,20);
  rt1->GetXaxis()->SetTitle("Entry");rt1->GetXaxis()->CenterTitle();
  rt1->GetYaxis()->SetTitle("T2 rate / beam current");rt1->GetXaxis()->CenterTitle();

  tree->Draw("(double(RightT2_r/Rightunew_r):Entry$>>rt1","Rightunew_r>0");


  // TCanvas* c2 = new TCanvas("c2","c2");

  // T->Draw("(RightT2_r/Rightunew_r):Entry$","abs(Rightunew_r-48300)<500");


  // TCanvas* c3 = new TCanvas("c3","c3");

  // T->Draw("(RightT2_r/Rightunew_r):RightRF_Time","Rightunew_r>0");
  
  
  // TCanvas* c4 = new TCanvas("c4","c4");

  // T->Draw("(RightT2_r/Rightunew_r):Rightunew_r","Rightunew_r>0");

 
  // TCanvas* c5 = new TCanvas("c5","c5");

  // T->Draw("RightT2_r:Rightunew_r>>h1(50000,-10000,60000,10000,80000","");


  // Rclock_r
  
}
