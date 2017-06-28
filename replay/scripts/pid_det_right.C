//Display info from the pid detectors
//Barak Schmookler, Sep 2015
void pid_det_right(Int_t flag, Char_t *drawoption){

  gStyle->SetOptStat(0);
  
  TTree *tree = (TTree*)gDirectory->Get("T");

  if(flag ==1){
    
    TH2F *h = new TH2F("h","Shower E/p vs. Pre-Shower E/p",60,0,4,60,0,4);
    h->GetXaxis()->SetTitle("Pre-Shower E/p");
    h->GetYaxis()->SetTitle("Shower E/p");
    
    tree->Draw("R.sh.e/(1000.*R.tr.p[0]):R.ps.e/(1000.*R.tr.p[0])>>h","R.tr.n==1",drawoption);
  }

  if(flag ==2){

    TH2F *h1 = new TH2F("h1","Gas Cherenkov N_{phe} vs. Calorimeter E/p",100,0,2,200,0,40);
    h1->GetYaxis()->SetTitle("N_{phe}");h1->GetYaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("E/p");h1->GetXaxis()->CenterTitle();
    
    tree->Draw("R.cer.asum_c/100.:(R.ps.e+R.sh.e)/(1000.*R.tr.p[0])>>h1","R.tr.n==1",drawoption);  

  }

}
