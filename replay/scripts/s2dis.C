//Check if Signal is inside Gate
//Ranit Das, Feb 2019
void s2dis(Int_t flag, TString drawoption){


  
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
  //  gPad->SetMarkerStyle(21);


  TTree *tree = (TTree*)gDirectory->Get("T");
    
  
    
  
  	if(flag==1){
    
    TH1F *hay1 = new TH1F("hay1","L-arm S2-X Distribution",150,-1.5,1.5);
    hay1->GetXaxis()->SetTitle("S2-X");hay1->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("L.s2.trx>>hay1");
    
    TLatex *t1 = new TLatex();
    t1->SetTextColor(kRed);
    t1->SetTextSize(0.04);
    t1->SetTextAlign(21);
    int i1=hay1->GetEntries();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t1->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i1));
    
  }

	if(flag==2){
    
   TH1F *hay2 = new TH1F("hay2","L-arm S2-Y Distribution",150,-0.4,0.4);
    hay2->GetXaxis()->SetTitle("S2-Y");hay2->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("L.s2.try>>hay2");
    
    TLatex *t2 = new TLatex();
    t2->SetTextColor(kRed);
    t2->SetTextSize(0.04);
    t2->SetTextAlign(21);
    int i2=hay2->GetEntries();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t2->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i2));
  
    //TLatex *t = new TLatex();
    //t->SetTextColor(kRed);
    //t->SetTextSize(0.05);
    //t->SetTextAlign(12);
    //int i1=ht1ac->GetEntries();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    //t->DrawLatexNDC(0.2,0.65,Form("e+/e- and good coinc. timing: %d",i1));

  }
  
  if(flag==3){
  
  TH2F *hay3 = new TH2F("hay3","L-arm S2 y vs. x",150,-1.5,1.5,150,-0.4,0.4);
  hay3->GetXaxis()->SetTitle("S2-X");hay3->GetXaxis()->CenterTitle();
  hay3->GetYaxis()->SetTitle("S2-Y");hay3->GetYaxis()->CenterTitle();
  
  tree->Draw("L.s2.try:L.s2.trx>>hay3","","colz");
}

if(flag==4){
    
    TH1F *hay4 = new TH1F("hay4","R-arm S2-X Distribution",150,-1.5,1.5);
    hay4->GetXaxis()->SetTitle("S2-X");hay4->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("R.s2.trx>>hay4");
    
    TLatex *t4 = new TLatex();
    t4->SetTextColor(kRed);
    t4->SetTextSize(0.04);
    t4->SetTextAlign(21);
    int i4=hay4->GetEntries();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t4->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i4));
    
  }

	if(flag==5){
    
   TH1F *hay5 = new TH1F("hay5","R-arm S2-Y Distribution",150,-0.4,0.4);
    hay5->GetXaxis()->SetTitle("S0-Y");hay5->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("R.s2.try>>hay5");
    
    TLatex *t5 = new TLatex();
    t5->SetTextColor(kRed);
    t5->SetTextSize(0.05);
    t5->SetTextAlign(12);
    int i5=hay5->GetEntries();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t5->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i5));
  
    //TLatex *t = new TLatex();
    //t->SetTextColor(kRed);
    //t->SetTextSize(0.05);
    //t->SetTextAlign(12);
    //int i1=ht1ac->GetEntries();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    //t->DrawLatexNDC(0.2,0.65,Form("e+/e- and good coinc. timing: %d",i1));

  }
  
  if(flag==6){
  
  TH2F *hay6 = new TH2F("hay6","R-arm S2 y vs. x",150,-1.5,1.5,150,-0.4,0.4);
  hay6->GetXaxis()->SetTitle("S0-X");hay6->GetXaxis()->CenterTitle();
  hay6->GetYaxis()->SetTitle("S0-Y");hay6->GetYaxis()->CenterTitle();
  
  tree->Draw("R.s2.try:R.s2.trx>>hay6","","colz");
}

}
