void  fpp_eff(TString vHist,Int_t npl,Int_t maxbin) {
  TH1D *name = (TH1D*)gDirectory->Get(vHist);
   TLatex l;
  gStyle->SetOptStat(0);
  name->Draw();
  Int_t max_content,max_bin;
  max_bin = name->GetMaximumBin();
  max_content = name->GetBinContent(max_bin);
  Double_t sum,cnt0,cnt2,cnt3,eff,rat;
  Int_t n;
  sum = 0.;
  for (n=1;n<maxbin+1;n++) sum = sum + name->GetBinContent(n);
   cnt0 = name->GetBinContent(1);
  rat=cnt0/sum;
   TString rattext;
   rattext=Form("Zero/tot = %4.3f",rat);  
   l.DrawLatex(maxbin*.55,max_content*.95,rattext);
   cnt2 = name->GetBinContent(npl);
   cnt3 = name->GetBinContent(npl+1);
     rat = -1000;
   if ( cnt3 !=0) rat=cnt2/cnt3;
   eff=1./(1.+rat/(npl-1));
   TString efftext;
   efftext=Form(" Eff = %3.2f",eff);
   l.DrawLatex(maxbin*.7,max_content*.85,efftext);
}
