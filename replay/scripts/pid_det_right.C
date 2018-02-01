//Display info from the pid detectors
//Barak Schmookler, Sep 2015
void pid_det_right(Int_t flag, TString drawoption){

  gStyle->SetOptStat(0);
  
  TTree *tree = (TTree*)gDirectory->Get("T");

  if(flag ==1){
    
    TH2F *h = new TH2F("h","Shower adc sum vs. Pre-Shower adc sum",1000,0,3000,2000,0,4000);
    h->GetXaxis()->SetTitle("Layer 1 adc_sum");h->GetYaxis()->CenterTitle();
    h->GetYaxis()->SetTitle("Layer 2 adc_sum");h->GetXaxis()->CenterTitle();
    
    tree->Draw("R.sh.asum_c:R.ps.asum_c>>h","R.tr.n==1",drawoption);
  }

  if(flag ==2){

    TH2F *h1 = new TH2F("h1","Gas Cherenkov N_{phe} vs. Calorimeter E/p",200,0,2,500,0,100);
    h1->GetYaxis()->SetTitle("N_{phe}");h1->GetYaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("E/p");h1->GetXaxis()->CenterTitle();
    
    tree->Draw("R.cer.asum_c/300.:(R.ps.e+R.sh.e)/(1000.*R.tr.p[0])>>h1","R.tr.n==1",drawoption);  

  }

}
