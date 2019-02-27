using namespace std;
#include <iostream>
void target_stability_macro(Int_t flag, TString drawoption, Int_t runnumber, Int_t n_runs = 0, Int_t online = 0){
// Ratio of T2 single arm trigger rate to current scaler rate - stability over time (should be constant) and as a function of current (should be linear)

// I want to know the average singles (T2) rate/current scaler rate vs. run number and vs. time (vs. time in a plot, avg vs. run number as an average to show long time scales)
// - Take SUM: (T2_scaler_rate/current scaler rate) and DIVIDE by number of entries (should be = number of times scaler rates got summed and == number of prescaled triggers)
// BONUS: 
// I want to know the production resultant good T6 total counts vs. time and vs. integrated charge (I already can count good T6s with the fit)
// I want to know the integrated current in coulombs per run (extra)

// by hand
//int runnumber = 4261;

// Old
/*TString rootfile = Form("/adaqfs/home/a-onl/apex_work/ranit/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_online_%d.root",runnumber);
TFile *f = TFile::Open(rootfile,"READ");
TTree *tree2 = (TTree*)f->Get("evRight");
*/
  TChain *tree2 = new TChain("evRight");
  TString filenamebase;
  TString filename;
  Long_t split = 0;

  TString ROOTFILE_DIR;

  for(Int_t i = 0; i <= (n_runs); i++){

    //TString ROOTFILE_DIR = "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_%d.root";
    if (online == 1) {
        ROOTFILE_DIR = "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_online_%d.root";
    }
    else {
        ROOTFILE_DIR = "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_%d.root";

    }
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

//onlineGUI64
//  gStyle->SetOptStat(0);
//  gPad->SetFillStyle(4000);
//  TTree *tree2 = (TTree*)gDirectory->Get("evRight");

if (flag==1){
	// histogram current integrating scaler vs. Entry$, grab 5th entry, grab GetEntries()-5, take difference and scale, assign to total integrated charge
	// histogram current vs. entry number
	// add total integrated charge to legend
	// add avg current for run to legend
	ofstream file_out1;
	ofstream file_out1_avg;
	file_out1.open("Per_run_integrated_charge.csv",std::ofstream::app);
	file_out1_avg.open("Per_run_avg_current.csv",std::ofstream::app);

	tree2->Draw("0.00038*evRightdnew_r:Entry$","evRightdnew_r>0","*"); // eyeballed 0.00038 uA/rightDnew scaler rate
	double rdnew         = 0;
	double rdnew_initial = 0;
	double rdnew_final   = 0;
	double clock         = 0;
	double clock_initial = 0;
	double clock_final   = 0;
	int n_entries = tree2->GetEntries();
	tree2->SetBranchAddress("evRightdnew",&rdnew);
	tree2->SetBranchAddress("evRightLclock",&clock);
	tree2->GetEntry(2);
	rdnew_initial = rdnew;
	clock_initial = clock;
	cout<<"initial clock = "<<clock_initial<<endl;
	cout<<"initial dnew = "<<rdnew_initial<<endl;
	cout<<"n entries = "<<n_entries<<endl;
	tree2->GetEntry(n_entries-1);
	rdnew_final   = rdnew;
	clock_final   = clock;
	double clock_f = 104000.0; // eyeballed 1.04E5 Hz clock rate on scaler gui screen
	double time_elapsed = (clock_final - clock_initial)/clock_f;
	double avg_current = (rdnew_final - rdnew_initial)/(clock_final - clock_initial) * 40.0 ; // eyeballed 40 uA per 1 dnew/clock
	double integrated_charge = avg_current*time_elapsed;

	TLegend* legStable1 = new TLegend(0.15,0.15,0.50,0.35);
	legStable1->AddEntry((TObject*)0,Form("Time elapsed = %0.1f seconds",time_elapsed),"");
	legStable1->AddEntry((TObject*)0,Form("Average current = %0.2f uA",avg_current),"");
	legStable1->AddEntry((TObject*)0,Form("Integrated charge = %0.4f Coulombs",1e-6*integrated_charge),"");
	legStable1->Draw();

	file_out1<<runnumber<<","<<1e-6*integrated_charge<<",0.0"<<std::endl;
	file_out1.close();
	file_out1_avg<<runnumber<<","<<avg_current<<",0.0"<<std::endl;
	file_out1_avg.close();

	//cout<<"final clock = "<< clock_final<<endl;
	//cout<<"final dnew = "<< rdnew_final<<endl;
	//cout<<"final - initial clock = "<< clock_final - clock_initial <<endl;
	//cout<<"final - initial dnew = "<< rdnew_final - rdnew_initial <<endl;
//cout<<"time elapsed in calc (s) = "<< (clock_final - clock_initial)/clock_f <<endl;
	//cout<<"dnew rate = "<< (rdnew_final - rdnew_initial)/(n_entries) <<endl;
//cout<<"avg current for run (uA)  = "<< (rdnew_final - rdnew_initial)/(clock_final - clock_initial) * 40.0 <<endl; // eyeballed 40 uA per 1 dnew/clock
//cout<<"integrated charge on target (C) = "<< 1e-6*avg_current*time_elapsed <<endl;
	//29.2 uA = slope of 0.72 for scalerunits/clockunits
/*	0.72 scalers/clocks = 29.2 uA
	0.97 scalers/clocks = 38.5 uA
	therefore 38.5 uA/(0.97 scalers/clocks) ==? 29.2 uA/(0.72 scalers/clocks) 
	so 1 scaler/clock = 40.55 or 39.68 uA -> 40 uA
	integrated charge in uCoulombs = scalers/clock * 40 uCoulombs/seconds * N pulses/(frequency 104000pulses/second) seconds
*/

}

else if (flag==2){
	ofstream file_out2;
	file_out2.open("Per_run_target_stability_T6_per_uA.csv",std::ofstream::app);
	TH1F *ht2 = new TH1F("ht2","T6 coincidences rate/current",1000,0,500); // should fall at 50 kHz / uA, but give large bins for safety
	ht2->GetXaxis()->SetTitle("T6 coincidences/uAmp current on target");ht2->GetXaxis()->CenterTitle();
	tree2->Draw("(evRightT6_r/(0.00038*evRightdnew_r))>>ht2","evRightT6_r>0","");
	double T6s_per_uA_mean = 0.0;
	double T6s_per_uA_stdv = 0.0;
	T6s_per_uA_mean = ht2->GetMean();
	T6s_per_uA_stdv = ht2->GetRMS();
	ht2->GetXaxis()->SetRange(ht2->FindFirstBinAbove(0,1)-10,ht2->FindLastBinAbove(0,1)+10);
	ht2->Draw();
	TH1* ht2_rebin = ht2->Rebin(2,"ht2_rebin");
	ht2_rebin->GetXaxis()->SetRange(ht2_rebin->FindFirstBinAbove(0,1)-10,ht2_rebin->FindLastBinAbove(0,1)+10);
	ht2_rebin->GetXaxis()->SetTitle("T6 singles/uAmp current on target");ht2_rebin->GetXaxis()->CenterTitle();
	ht2_rebin->Draw();
	//cout<<"Average (T6 rate/uA beam) = "<<T6s_per_uA_mean<<endl;
	//cout<<"Std dev (T6 rate/uA beam) = "<<T6s_per_uA_stdv<<endl;
	TLegend* legStable2 = new TLegend(0.10,0.95,0.90,0.90);
	legStable2->AddEntry((TObject*)0,Form("Avg T6 rate/uA beam = %0.1f +- %0.1f",T6s_per_uA_mean,T6s_per_uA_stdv),"");
	legStable2->Draw();

	file_out2<<runnumber<<","<<T6s_per_uA_mean<<","<<T6s_per_uA_stdv<<std::endl;
	file_out2.close();
}

else if (flag==3){ 
	ofstream file_out3;
	file_out3.open("Per_run_target_stability_T2_per_uA.csv",std::ofstream::app);
	//	file_out3.open("target_stability.csv",std::ofstream::out | std::ofstream::app);

	TH1F *ht3 = new TH1F("ht3","T2 singles rate/current",100000,38200,58700); // should fall at 50 kHz / uA, but give large bins for safety
	ht3->GetXaxis()->SetTitle("T2 singles/uAmp current on target");ht3->GetXaxis()->CenterTitle();
	tree2->Draw("(evRightT2_r/(0.00038*evRightdnew_r))>>ht3","evRightT2_r>0","");
	double T2s_per_uA_mean = 0.0;
	double T2s_per_uA_stdv = 0.0;
	T2s_per_uA_mean = ht3->GetMean();
	T2s_per_uA_stdv = ht3->GetRMS();
	ht3->GetXaxis()->SetRange(ht3->FindFirstBinAbove(0,1)-10,ht3->FindLastBinAbove(0,1)+10);
	ht3->Draw();
	TH1* ht3_rebin = ht3->Rebin(50,"ht3_rebin");
	ht3_rebin->GetXaxis()->SetRange(ht3_rebin->FindFirstBinAbove(0,1)-10,ht3_rebin->FindLastBinAbove(0,1)+10);
	ht3_rebin->GetXaxis()->SetTitle("T2 singles/uAmp current on target");ht3_rebin->GetXaxis()->CenterTitle();
	ht3_rebin->Draw();
	TLegend* legStable3 = new TLegend(0.10,0.95,0.90,0.90);
	legStable3->AddEntry((TObject*)0,Form("Avg T2 rate/uA beam = %0.1f +- %0.1f",T2s_per_uA_mean,T2s_per_uA_stdv),"");
	legStable3->Draw();

	file_out3<<runnumber<<","<<T2s_per_uA_mean<<","<<T2s_per_uA_stdv<<std::endl;
	file_out3.close();

	//cout<<"Average (T2 rate/uA beam) = "<<T2s_per_uA_mean<<endl;
	//cout<<"Std dev (T2 rate/uA beam) = "<<T2s_per_uA_stdv<<endl;
}
else if (flag==4){ 
	ofstream file_out4;
	file_out4.open("Per_run_target_stability_avg_T2.csv",std::ofstream::app);
	//	file_out4.open("target_stability.csv",std::ofstream::out | std::ofstream::app);

	TH1F *ht4 = new TH1F("ht4","T2 singles rate",100000.0,0.0,1e7); // should fall at 50 kHz / uA, but give large bins for safety
	ht4->GetXaxis()->SetTitle("T2 singles");ht4->GetXaxis()->CenterTitle();
	tree2->Draw("evRightT2_r>>ht4","evRightT2_r>0","*");
	double T2s_mean = 0.0;
	double T2s_stdv = 0.0;
	T2s_mean = ht4->GetMean();
	T2s_stdv = ht4->GetRMS();
	ht4->GetXaxis()->SetRange(ht4->FindFirstBinAbove(0,1)-10,ht4->FindLastBinAbove(0,1)+10);
	ht4->Draw();
	TH1* ht4_rebin = ht4->Rebin(50,"ht4_rebin");
	ht4_rebin->GetXaxis()->SetRange(ht4_rebin->FindFirstBinAbove(0,1)-10,ht4_rebin->FindLastBinAbove(0,1)+10);
	ht4_rebin->GetXaxis()->SetTitle("T2 Rate");ht4_rebin->GetXaxis()->CenterTitle();
	ht4_rebin->Draw();
	TLegend* legStable4 = new TLegend(0.10,0.95,0.90,0.90);
	legStable4->AddEntry((TObject*)0,Form("Avg T2s in epics update = %0.1f +- %0.1f",T2s_mean,T2s_stdv),"");
	legStable4->Draw();

	file_out4<<runnumber<<","<<T2s_mean<<","<<T2s_stdv<<std::endl;
	file_out4.close();

	//cout<<"Average (T2 rate/uA beam) = "<<T2s_per_uA_mean<<endl;
	//cout<<"Std dev (T2 rate/uA beam) = "<<T2s_per_uA_stdv<<endl;
}
else if (flag==5){ 
	ofstream file_out5;
	file_out5.open("Per_run_target_stability_avg_T5.csv",std::ofstream::app);
	//	file_out5.open("target_stability.csv",std::ofstream::out | std::ofstream::app);

	TH1F *ht5 = new TH1F("ht5","T5 singles rate",100000.0,0.0,1e5); // should fall at 50 kHz / uA, but give large bins for safety
	ht5->GetXaxis()->SetTitle("T5 singles");ht5->GetXaxis()->CenterTitle();
	tree2->Draw("evRightT5_r>>ht5","evRightT5_r>0","*");
	double T5s_mean = 0.0;
	double T5s_stdv = 0.0;
	T5s_mean = ht5->GetMean();
	T5s_stdv = ht5->GetRMS();
	ht5->GetXaxis()->SetRange(ht5->FindFirstBinAbove(0,1)-10,ht5->FindLastBinAbove(0,1)+10);
	ht5->Draw();
	TH1* ht5_rebin = ht5->Rebin(20,"ht5_rebin");
	ht5_rebin->GetXaxis()->SetRange(ht5_rebin->FindFirstBinAbove(0,1)-10,ht5_rebin->FindLastBinAbove(0,1)+10);
	ht5_rebin->GetXaxis()->SetTitle("T5 Rate");ht5_rebin->GetXaxis()->CenterTitle();
	ht5_rebin->Draw();
	TLegend* legStable5 = new TLegend(0.10,0.95,0.90,0.90);
	legStable5->AddEntry((TObject*)0,Form("Avg T5s in epics update = %0.1f +- %0.1f",T5s_mean,T5s_stdv),"");
	legStable5->Draw();

	file_out5<<runnumber<<","<<T5s_mean<<","<<T5s_stdv<<std::endl;
	file_out5.close();

	//cout<<"Average (T5 rate) = "<<T5s_mean<<endl;
	//cout<<"Std dev (T5 rate) = "<<T5s_stdv<<endl;
}
else if (flag==6){ 
	ofstream file_out6;
	file_out6.open("Per_run_target_stability_avg_T6.csv",std::ofstream::app);
	//	file_out6.open("target_stability.csv",std::ofstream::out | std::ofstream::app);

	TH1F *ht6 = new TH1F("ht6","T6 singles rate",100000.0,0.0,1e4); // should fall at 60 kHz / uA, but give large bins for safety
	ht6->GetXaxis()->SetTitle("T6 singles");ht6->GetXaxis()->CenterTitle();
	tree2->Draw("evRightT6_r>>ht6","evRightT6_r>0","*");
	double T6s_mean = 0.0;
	double T6s_stdv = 0.0;
	T6s_mean = ht6->GetMean();
	T6s_stdv = ht6->GetRMS();
	ht6->GetXaxis()->SetRange(ht6->FindFirstBinAbove(0,1)-10,ht6->FindLastBinAbove(0,1)+10);
	ht6->Draw();
	TH1* ht6_rebin = ht6->Rebin(50,"ht6_rebin");
	ht6_rebin->GetXaxis()->SetRange(ht6_rebin->FindFirstBinAbove(0,1)-10,ht6_rebin->FindLastBinAbove(0,1)+10);
	ht6_rebin->GetXaxis()->SetTitle("T6 Rate");ht6_rebin->GetXaxis()->CenterTitle();
	ht6_rebin->Draw();
	TLegend* legStable6 = new TLegend(0.10,0.95,0.90,0.90);
	legStable6->AddEntry((TObject*)0,Form("Avg T6s in epics update = %0.1f +- %0.1f",T6s_mean,T6s_stdv),"");
	legStable6->Draw();

	file_out6<<runnumber<<","<<T6s_mean<<","<<T6s_stdv<<std::endl;
	file_out6.close();

	//cout<<"Average (T5 rate) = "<<T5s_mean<<endl;
	//cout<<"Std dev (T5 rate) = "<<T5s_stdv<<endl;
}

else if (flag==7){ 
	ofstream file_out7;
	file_out7.open("Per_run_target_stability_avg_T5toT2.csv",std::ofstream::app);
	//	file_out7.open("target_stability.csv",std::ofstream::out | std::ofstream::app);

	TH1F *ht7 = new TH1F("ht7","T5/T2 singles rate",50000.0,0.0,.05); // should fall at 70 kHz / uA, but give large bins for safety
	ht7->GetXaxis()->SetTitle("T5/T2 singles");ht7->GetXaxis()->CenterTitle();
	tree2->Draw("(evRightT5_r/evRightT2_r)>>ht7","evRightT5_r>0 && evRightT2>0","*");
	double T5to2s_mean = 0.0;
	double T5to2s_stdv = 0.0;
	T5to2s_mean = ht7->GetMean();
	T5to2s_stdv = ht7->GetRMS();
	ht7->GetXaxis()->SetRange(ht7->FindFirstBinAbove(0,1)-10,ht7->FindLastBinAbove(0,1)+10);
	ht7->Draw();
	TH1* ht7_rebin = ht7->Rebin(10,"ht7_rebin");
	ht7_rebin->GetXaxis()->SetRange(ht7_rebin->FindFirstBinAbove(0,1)-10,ht7_rebin->FindLastBinAbove(0,1)+10);
	ht7_rebin->GetXaxis()->SetTitle("T5/T2 Rate");ht7_rebin->GetXaxis()->CenterTitle();
	ht7_rebin->Draw();
	TLegend* legStable7 = new TLegend(0.10,.95,0.90,0.90);
	legStable7->AddEntry((TObject*)0,Form("Avg T5/T2 in epics update = %0.4f +- %0.4f",T5to2s_mean,T5to2s_stdv),"");
	legStable7->Draw();

	file_out7<<runnumber<<","<<T5to2s_mean<<","<<T5to2s_stdv<<std::endl;
	file_out7.close();

	//cout<<"Average (T5 rate) = "<<T5s_mean<<endl;
	//cout<<"Std dev (T5 rate) = "<<T5s_stdv<<endl;
}
}
