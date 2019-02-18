//Check if Signal is inside Gate
//Ranit Das, Feb 2019
void deltaR_deltaL_sum(Int_t flag, TString drawoption){


  
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
  //  gPad->SetMarkerStyle(21);


  TTree *tree = (TTree*)gDirectory->Get("T");
    
  
    
  
  	if(flag==2){
    
    TH1F *ht1ab = new TH1F("ht1ab","Sum of L-arm tgt delta and R-arm tgt delta",240,-0.12,0.12);
    ht1ab->GetXaxis()->SetTitle("R-arm");ht1ab->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("R.tr.tg_dp+L.tr.tg_dp>>ht1ab","R.cer.asum_c>2000&&R.tr.n==1&&L.cer.asum_c>2000&&L.tr.n==1","COLZ");
  }

	if(flag==3){
    
    TH1F *ht1ac = new TH1F("ht1ac","Sum of L-arm tgt delta and R-arm tgt delta with time cut(1680 to 1700)",240,-0.12,0.12);
    ht1ac->GetXaxis()->SetTitle("R-arm");ht1ac->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
    tree->Draw("R.tr.tg_dp+L.tr.tg_dp>>ht1ac","R.cer.asum_c>2000&&R.tr.n==1&&L.cer.asum_c>2000&&L.tr.n==1&&((DR.rrawt2>1680)&&(DR.rrawt2<1700))&&fEvtHdr.fEvtType==6","COLZ");
  }
}
