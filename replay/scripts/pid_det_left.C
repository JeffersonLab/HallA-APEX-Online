//Display info from the pid detectors
//Barak Schmookler, Sep 2015
void pid_det_left(Int_t flag, Char_t *drawoption){
  
  gStyle->SetOptStat(0);
  
  TTree *tree = (TTree*)gDirectory->Get("T");

  if(flag ==1){
    
    TH2F *h = new TH2F("h","Pion-Rej 2 E/p vs. Pion-Rej 1 E/p",60,0,10,60,0,10);
    h->GetXaxis()->SetTitle("Layer 1 E/p");h->GetYaxis()->CenterTitle();
    h->GetYaxis()->SetTitle("Layer 2 E/p");h->GetXaxis()->CenterTitle();
    
    tree->Draw("L.prl2.e/(1000.*L.tr.p[0]):L.prl1.e/(1000.*L.tr.p[0])>>h","L.tr.n==1",drawoption);
  }

  if(flag ==2){
    
    TH2F *h1 = new TH2F("h1","Gas Cherenkov N_{phe} vs. Calorimeter E/p",200,0,2,500,0,50);
    h1->GetYaxis()->SetTitle("N_{phe}");h1->GetYaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("E/p");h1->GetXaxis()->CenterTitle();

    tree->Draw("L.cer.asum_c/100.:(L.prl1.e+L.prl2.e)/(1000.*L.tr.p[0])>>h1","L.tr.n==1",drawoption);  

  }
}
