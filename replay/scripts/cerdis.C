//Check if Signal is inside Gate
//Ranit Das, Feb 2019
void cerdis(Int_t flag, TString drawoption){


  
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
  //  gPad->SetMarkerStyle(21);


  TTree *tree = (TTree*)gDirectory->Get("T");
    
  
    
  
  	if(flag==1){
    
    TH1F *hay1 = new TH1F("hay1","L-arm Cerenkov-X Distribution",150,-1.5,1.5);
    hay1->GetXaxis()->SetTitle("Cerenkov-X");hay1->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("L.cer.trx>>hay1");
    
    TLatex *t1 = new TLatex();
    t1->SetTextColor(kRed);
    t1->SetTextSize(0.04);
    t1->SetTextAlign(21);
    int i1=hay1->Integral();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t1->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i1));
    
  }

	if(flag==2){
    
   TH1F *hay2 = new TH1F("hay2","L-arm Cerenkov-Y Distribution",150,-0.4,0.4);
    hay2->GetXaxis()->SetTitle("Cerenkov-Y");hay2->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("L.cer.try>>hay2");
    
    TLatex *t2 = new TLatex();
    t2->SetTextColor(kRed);
    t2->SetTextSize(0.05);
    t2->SetTextAlign(12);
    int i2=hay2->Integral();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t2->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i2));
  
    //TLatex *t = new TLatex();
    //t->SetTextColor(kRed);
    //t->SetTextSize(0.05);
    //t->SetTextAlign(12);
    //int i1=ht1ac->Integral();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    //t->DrawLatexNDC(0.2,0.65,Form("e+/e- and good coinc. timing: %d",i1));

  }
  
  if(flag==3){
  
  TH2F *hay3 = new TH2F("hay3","L-arm Cerenkov y vs. x",150,-1.5,1.5,150,-0.4,0.4);
  hay3->GetXaxis()->SetTitle("Cerenkov-X");hay3->GetXaxis()->CenterTitle();
  hay3->GetYaxis()->SetTitle("Cerenkov-Y");hay3->GetYaxis()->CenterTitle();
  
  tree->Draw("L.cer.try:L.cer.trx>>hay3","","colz");
}

if(flag==4){
    
    TH1F *hay4 = new TH1F("hay4","R-arm Cerenkov-X Distribution",150,-1.5,1.5);
    hay4->GetXaxis()->SetTitle("Cerenkov-X");hay4->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("R.cer.trx>>hay4");
    
    TLatex *t4 = new TLatex();
    t4->SetTextColor(kRed);
    t4->SetTextSize(0.04);
    t4->SetTextAlign(21);
    int i4=hay4->Integral();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t4->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i4));
    
  }

	if(flag==5){
    
   TH1F *hay5 = new TH1F("hay5","R-arm Cerenkov-Y Distribution",150,-0.4,0.4);
    hay5->GetXaxis()->SetTitle("Cerenkov-Y");hay5->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("R.cer.try>>hay5");
    
    TLatex *t5 = new TLatex();
    t5->SetTextColor(kRed);
    t5->SetTextSize(0.05);
    t5->SetTextAlign(12);
    int i5=hay5->Integral();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    t5->DrawLatexNDC(0.2,0.65,Form("Number of Entries: %d",i5));
  
    //TLatex *t = new TLatex();
    //t->SetTextColor(kRed);
    //t->SetTextSize(0.05);
    //t->SetTextAlign(12);
    //int i1=ht1ac->Integral();
    //t->DrawLatexNDC(0.2,0.85,Form("good coincidence counts"));
    //t->DrawLatexNDC(0.2,0.65,Form("e+/e- and good coinc. timing: %d",i1));

  }
  
  if(flag==6){
  
  TH2F *hay6 = new TH2F("hay6","R-arm Cerenkov y vs. x",150,-1.5,1.5,150,-0.4,0.4);
  hay6->GetXaxis()->SetTitle("Cerenkov-X");hay6->GetXaxis()->CenterTitle();
  hay6->GetYaxis()->SetTitle("Cerenkov-Y");hay6->GetYaxis()->CenterTitle();
  
  tree->Draw("R.cer.try:R.cer.trx>>hay6","","colz");
}

}
