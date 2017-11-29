{
  TChain *T = new TChain("T");
  T->Add("/cache/mss/home/yez/root_0721/e08014_R_3862.root");
  
  TCanvas *c1 = new TCanvas("c1","c1",1000,800);
  // c1->Divide(1,2);
  c1->cd();
  TH2F *h1 = new TH2F("h1","HRS-R Cerenkov ADC Sum vs X}",200,-1.6,1.6,200,-400.0,4000);
  T->Draw("R.cer.asum_c:R.cer.trx>>h1","DBB.evtypebits>>1&1","colz");
  h1->SetXTitle("R.cer.trx");  h1->SetYTitle("R.cer.asum_c");

//   c1->cd(2) ;
//   TH2F *h1 = new TH2F("h2","HRS-L Cerenkov ADC Sum vs Cerenkov PMT",200,-1.6,1.6,200,-100.0,4000);
//   T->Draw("R.cer.asum_c:R.cer.trx>>h1","DBB.evtypebits>>3&1","colz");
  


  TCanvas *c2 = new TCanvas("c2","c2",600,1000);
  c2->Divide(2,5);
  
  TH2F *h3[10];
  for(int i=0;i<10;i++){
    c2->cd(i+1);
    TString hist = Form("h3%d",i);
    h3[i] = new TH2F(hist,Form("HRS-L Cerenkov ADC #%d vs X",i+1),200,-1.6,1.6,200,-400.0,4000);
    T->Draw(Form("R.cer.a_c[%d]:R.cer.trx>>",i)+hist,Form("DBB.evtypebits>>1&1&&R.cer.t[%d]>100",i),"colz");
    h3[i]->SetXTitle("R.cer.trx");  h3[i]->SetYTitle(Form("R.cer.a_c[%d]",i));

  }
  
}
