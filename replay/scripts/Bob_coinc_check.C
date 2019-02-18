//Check if Signal is inside Gate
//Barak Schmookler, Feb 2016
void Bob_coinc_check(Int_t flag, TString drawoption){


  
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
  //  gPad->SetMarkerStyle(21);


  TTree *tree = (TTree*)gDirectory->Get("T");
    
  
    if(flag==1){
    
    TH1F *ht1 = new TH1F("ht1","DR.rrawt2",200,1600,1800);
    ht1->GetXaxis()->SetTitle("DR.rrawt2");ht1->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    
    tree->Draw("DR.rrawt2>>ht1","fEvtHdr.fEvtType==6","kRED");
  }

}
