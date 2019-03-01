using namespace std;
#include <iostream>
void delta_correlation(Int_t flag, TString drawoption){
// Ratio of T2 single arm trigger rate to current scaler rate - stability over time (should be constant) and as a function of current (should be linear)

// I want to know the average singles (T2) rate/current scaler rate vs. run number and vs. time (vs. time in a plot, avg vs. run number as an average to show long time scales)
// - Take SUM: (T2_scaler_rate/current scaler rate) and DIVIDE by number of entries (should be = number of times scaler rates got summed and == number of prescaled triggers)
// BONUS: 
// I want to know the production resultant good T6 total counts vs. time and vs. integrated charge (I already can count good T6s with the fit)
// I want to know the integrated current in coulombs per run (extra)

// by hand
//int runnumber = 4261;
  int runnumber = 0;
  TString run = gSystem->Getenv("RUNNUM");
  runnumber = run.Atoi();

// Old
/*TString rootfile = Form("/adaqfs/home/a-onl/apex_work/ranit/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_online_%d.root",runnumber);
TFile *f = TFile::Open(rootfile,"READ");
TTree *tree2 = (TTree*)f->Get("evRight");
*/


/*  TChain *tree2 = new TChain("T");
  TString filenamebase;
  TString filename;
  Long_t split = 0;

  for(Int_t i = 0; i <= (n_runs); i++){

    TString ROOTFILE_DIR = "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_online_%d.root";
    //TString ROOTFILE_DIR = "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_%d.root";
    filenamebase = Form(ROOTFILE_DIR,runnumber+i);
    
    filename = filenamebase;
    
    filenamebase.Remove(filenamebase.Last('.'),5);
    
    cout << filename << endl;
    
    split = 0;
    while ( !gSystem->AccessPathName(filename.Data()) ) {
      cout << "Added root file: " << filename << " to Tree" << endl;
      tree2->Add(filename);
    split++;
    filename = filenamebase + "_" + split + ".root";
    }
}

//Manual
TCanvas* c1 = new TCanvas();
c1->Divide(1,2);
    
*/
//onlineGUI64
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
  TTree *tree2 = (TTree*)gDirectory->Get("T");



if (flag==1){ 
	ofstream file_out1;
	file_out1.open("Per_run_delta_correlation_online.csv",std::ofstream::app);
	TH2F *ht1 = new TH2F("ht1","R-arm tgt delta p vs L-arm tgt delta p with Cherenkov Sum >2000 cut",120,-0.06,0.06,60,-0.06,0.06); // should fall at 50 kHz / uA, but give large bins for safety
	ht1->GetXaxis()->SetTitle("L-arm tgt delta p");ht1->GetXaxis()->CenterTitle();
	ht1->GetYaxis()->SetTitle("R-arm tgt delta p");ht1->GetXaxis()->CenterTitle();
	tree2->Draw("R.tr.tg_dp:L.tr.tg_dp>>ht1","R.cer.asum_c>2000 && L.cer.asum_c>2000 && L.tr.n==1 && R.tr.n==1","colz");
	//cout<<"Entries: "<<ht1->GetEntries()<<endl;
	//ht1->Draw();
	//ht2->GetXaxis()->SetRange(ht3->FindFirstBinAbove(0,1)-10,ht3->FindLastBinAbove(0,1)+10);
	TLegend* leg1 = new TLegend(0.10,0.90,0.50,0.82);
	leg1->AddEntry((TObject*)0,Form("Number of entries: %0.1f",ht1->GetEntries()));
	leg1->Draw();
	file_out1<<runnumber<<","<<ht1->GetEntries()<<",0.0"<<std::endl;
	file_out1.close();
}
else if (flag==2){
	ofstream file_out2;
	file_out2.open("Per_run_delta_correlation_cerenkov_cuts_online.csv",std::ofstream::app);
	TH2F *ht2 = new TH2F("ht2","R-arm tgt delta p vs L-arm tgt delta p, with Cherenkov sum > 2000 cut, timing cuts 1695 to 1705",120,-0.06,0.06,60,-0.06,0.06); // should fall at 50 kHz / uA, but give large bins for safety
	ht2->GetXaxis()->SetTitle("L-arm tgt delta p, with timing cuts");ht2->GetXaxis()->CenterTitle();
	ht2->GetYaxis()->SetTitle("R-arm tgt delta p, with timing cuts");ht2->GetYaxis()->CenterTitle();
	tree2->Draw("R.tr.tg_dp:L.tr.tg_dp>>ht2","R.cer.asum_c>2000 && L.cer.asum_c>2000 && L.tr.n==1 && R.tr.n==1 && DR.rrawt2>1695 && DR.rrawt2<1705 && (DR.evtypebits>>6)&1","colz");
	//cout<<"Cut Entries: "<<ht2->GetEntries()<<endl;
	//ht2->GetXaxis()->SetRange(ht3->FindFirstBinAbove(0,1)-10,ht3->FindLastBinAbove(0,1)+10);
	//ht2->Draw();
	//TH1* ht1_rebin = ht3->Rebin(50,"ht3_rebin");
	//ht1_rebin->GetXaxis()->SetRange(ht3_rebin->FindFirstBinAbove(0,1)-10,ht3_rebin->FindLastBinAbove(0,1)+10);
	//ht1_rebin->GetXaxis()->SetTitle("T2 singles/uAmp current on target");ht3_rebin->GetXaxis()->CenterTitle();
	//ht1_rebin->Draw();
	TLegend* leg2 = new TLegend(0.10,0.90,0.50,0.82);
	leg2->AddEntry((TObject*)0,Form("Number of entries after cuts: %0.1f",ht2->GetEntries()));
	leg2->Draw();
	file_out2<<runnumber<<","<<ht2->GetEntries()<<",0.0"<<std::endl;
	file_out2.close();
}
else if (flag==3){
	ofstream file_out3;
	file_out3.open("Per_run_delta_correlation_cerenkov_weaker_cuts_online.csv",std::ofstream::app);
        TH2F *ht3 = new TH2F("ht3","L-arm tgt delta p vs R-arm tgt delta p, with Cherenkov > 1000 cut, timing cuts 1685 to 1710",120,-0.06,0.06,60,-0.06,0.06);
        ht3->GetXaxis()->SetTitle("L-arm tgt delta p, with timing cuts");ht3->GetXaxis()->CenterTitle();
        ht3->GetYaxis()->SetTitle("R-arm tgt delta p, with timing cuts");ht3->GetYaxis()->CenterTitle();
        // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
        // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
        // &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
        tree2->Draw("R.tr.tg_dp:L.tr.tg_dp>>ht3","R.cer.asum_c>1000&&R.tr.n==1&&L.cer.asum_c>1000&&L.tr.n==1&&((DR.rrawt2>1685)&&(DR.rrawt2<1710))&&fEvtHdr.fEvtType==6","COLZ");
	TLegend* leg3 = new TLegend(0.10,0.90,0.45,0.80);
	leg3->AddEntry((TObject*)0,Form("Number of entries after cuts: %0.1f",ht3->GetEntries()));
	leg3->Draw();
	file_out3<<runnumber<<","<<ht3->GetEntries()<<",0.0"<<std::endl;
	file_out3.close();
}
else if(flag==4){
	TH1F *ht4 = new TH1F("ht4","Sum of L-arm tgt delta p and R-arm tgt delta p, Cherenkov Sum > 2000",240,-0.12,0.12);
	ht4->GetXaxis()->SetTitle("L-arm tgt delta p + R-arm tgt delta p");ht4->GetXaxis()->CenterTitle();
	ht4->GetYaxis()->CenterTitle();
	// ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
	// ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
	// &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
	tree2->Draw("R.tr.tg_dp+L.tr.tg_dp>>ht4","R.cer.asum_c>2000&&R.tr.n==1&&L.cer.asum_c>2000&&L.tr.n==1","COLZ");
	TLegend* leg4 = new TLegend(0.10,0.90,0.45,0.80);
	leg4->AddEntry((TObject*)0,Form("Number of entries after cuts: %0.1f",ht4->GetEntries()));
	leg4->Draw();
}

else if(flag==5){
	TH1F *ht5 = new TH1F("ht5","Sum of L-arm tgt delta p and R-arm tgt delta p, Cherenkov Sum> 2000, timing cuts 1695 to 1705",240,-0.12,0.12);
	ht5->GetXaxis()->SetTitle("L-arm tgt delta p + R-arm tgt delta p, with timing cuts");ht5->GetXaxis()->CenterTitle();
	ht5->GetYaxis()->CenterTitle();
	// ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
	// ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
	// &&(R.ps.e+R.sh.e)/(R.gold.p*1000)>0.7 &&(L.prl1.e+L.prl2.e)/(L.gold.p*1000)>0.7&
	tree2->Draw("R.tr.tg_dp+L.tr.tg_dp>>ht5","R.cer.asum_c>2000&&R.tr.n==1&&L.cer.asum_c>2000&&L.tr.n==1&&((DR.rrawt2>1690)&&(DR.rrawt2<1705))&&fEvtHdr.fEvtType==6","COLZ");
	TLegend* leg5 = new TLegend(0.10,0.90,0.45,0.80);
	leg5->AddEntry((TObject*)0,Form("Number of entries after cuts: %0.1f",ht5->GetEntries()));
	leg5->Draw();
}
}
