//Check if Signal is inside Gate
//Barak Schmookler, Feb 2016
void gate_check_right(Int_t flag, Char_t *drawoption){

  gStyle->SetOptStat(0);
  
  TTree *tree = (TTree*)gDirectory->Get("T");

  if(flag==1){
    
    TH2F *ht1 = new TH2F("ht1","All Cherenkov PMTs: Amplitude vs. Time",1000,1000,2000,1000,0,1000);
    ht1->GetXaxis()->SetTitle("Time [TDC Channel]");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    
    tree->Draw("R.cer.a_p:R.cer.t>>ht1","",drawoption);
  }

  if(flag==2){
    
    TH2F *ht2 = new TH2F("ht2","All Left S2 PMTs: Amplitude vs. Time",1000,1000,2000,200,0,200);
    ht2->GetXaxis()->SetTitle("Time [TDC Channel]");ht2->GetXaxis()->CenterTitle();
    ht2->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht2->GetYaxis()->CenterTitle();
    //ht2->SetMarkerStyle(3);ht2->SetMarkerSize(0.75);

    tree->Draw("R.s2.la_p:R.s2.lt>>ht2","",drawoption);
    
  }

  if(flag==3){
    
    TH2F *ht3 = new TH2F("ht3","All Right S2 PMTs: Amplitude vs. Time",1000,1000,2000,200,0,200);
    ht3->GetXaxis()->SetTitle("Time [TDC Channel]");ht3->GetXaxis()->CenterTitle();
    ht3->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht3->GetYaxis()->CenterTitle();
    //ht3->SetMarkerStyle(3);ht3->SetMarkerSize(0.75);

    tree->Draw("R.s2.ra_p:R.s2.rt>>ht3","",drawoption);
    
  }

  if(flag==4){
    
    TH2F *ht4 = new TH2F("ht4","S0-Top(B): Amplitude vs. Time",1000,1000,2000,2000,0,2000);
    ht4->GetXaxis()->SetTitle("Time [TDC Channel]");ht4->GetXaxis()->CenterTitle();
    ht4->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht4->GetYaxis()->CenterTitle();
    //ht4->SetMarkerStyle(3);ht4->SetMarkerSize(0.75);

    tree->Draw("R.s0.la_p:R.s0.lt>>ht4","",drawoption);
    
  }

  if(flag==5){
    
    TH2F *ht5 = new TH2F("ht5","S0-Bottom(A): Amplitude vs. Time",1000,1000,2000,2000,0,2000);
    ht5->GetXaxis()->SetTitle("Time [TDC Channel]");ht5->GetXaxis()->CenterTitle();
    ht5->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht5->GetYaxis()->CenterTitle();
    //ht5->SetMarkerStyle(3);ht5->SetMarkerSize(0.75);
    
    tree->Draw("R.s0.ra_p:R.s0.rt>>ht5","",drawoption);
    
  }

  if(flag==6){
    
    TH2F *ht6 = new TH2F("ht6","Calorimeter Hardware Sum: Amplitude vs. Time",1000,800,1800,1500,0,3000);
    ht6->GetXaxis()->SetTitle("Time [TDC Channel]");ht6->GetXaxis()->CenterTitle();
    ht6->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht6->GetYaxis()->CenterTitle();
    //ht6->SetMarkerStyle(3);ht6->SetMarkerSize(0.75);

    tree->Draw("DR.ShSum_a:DR.ShSum_t[Ndata.DR.ShSum_t-1]>>ht6","Ndata.DR.ShSum_t>0",drawoption);
    
  }

  if(flag==7){
    
    TH2F *ht7 = new TH2F("ht7","Gas Cherenkov Hardware Sum: Amplitude vs. Time (All Hits)",1000,800,1800,500,200,1200);
    ht7->GetXaxis()->SetTitle("Time [TDC Channel]");ht7->GetXaxis()->CenterTitle();
    ht7->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht7->GetYaxis()->CenterTitle();
    ht7->SetMarkerStyle(3);//ht7->SetMarkerSize(0.75);

    //tree->Draw("DR.GCsum_a:DR.GCsum_t[Ndata.DR.GCsum_t-1]>>ht7","Ndata.DR.GCsum_t>0",drawoption);
    tree->Draw("DR.GCsum_a:DR.GCsum_t>>ht7","",drawoption);

  }
  

}
