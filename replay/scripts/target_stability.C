void target_stability(Int_t flag, TString drawoption){
// Ratio of T2 single arm trigger rate to current scaler rate - stability over time (should be constant) and as a function of current (should be linear)

// I want to know the average singles (T2) rate/current scaler rate vs. run number and vs. time (vs. time in a plot, avg vs. run number as an average to show long time scales)
// - Take SUM: (T2_scaler_rate/current scaler rate) and DIVIDE by number of entries (should be = number of times scaler rates got summed and == number of prescaled triggers)
// BONUS: 
// I want to know the production resultant good T6 total counts vs. time and vs. integrated charge (I already can count good T6s with the fit)
// I want to know the integrated current in coulombs per run (extra)

// by hand
/*int runnumber = 4261;
TString rootfile = Form("/adaqfs/home/a-onl/apex_work/ranit/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_online_%d.root",runnumber);
TFile *f = TFile::Open(rootfile,"READ");
TTree *tree2 = (TTree*)f->Get("evRight");*/


//onlineGUI64
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
  TTree *tree2 = (TTree*)gDirectory->Get("evRight");


if (flag==1){
	// histogram current integrating scaler vs. Entry$, grab 5th entry, grab GetEntries()-5, take difference and scale, assign to total integrated charge
	// histogram current vs. entry number
	// add total integrated charge to legend
	// add avg current for run to legend
	tree2->Draw("0.00038*evRightdnew_r:Entry$","","*"); // eyeballed 0.00038 uA/rightDnew scaler rate
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

	TLegend* legStable1 = new TLegend(0.15,0.15,0.65,0.35);
	legStable1->AddEntry((TObject*)0,Form("Time elapsed = %0.1f seconds",time_elapsed),"");
	legStable1->AddEntry((TObject*)0,Form("Average current = %0.1f uA",avg_current),"");
	legStable1->AddEntry((TObject*)0,Form("Integrated charge = %0.4f Coulombs",1e-6*integrated_charge),"");
	legStable1->Draw();


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
	TH1F *ht2 = new TH1F("ht2","T6 singles rate/current",1000,0,500); // should fall at 50 kHz / uA, but give large bins for safety
	ht2->GetXaxis()->SetTitle("T6 singles/uAmp current on target");ht2->GetXaxis()->CenterTitle();
	tree2->Draw("(evRightT6_r/(0.00038*evRightdnew_r))>>ht2","","");
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
	TLegend* legStable2 = new TLegend(0.12,0.75,0.55,0.65);
	legStable2->AddEntry((TObject*)0,Form("Avg T6 rate/uA beam = %0.1f +- %0.1f",T6s_per_uA_mean,T6s_per_uA_stdv),"");
	legStable2->Draw();
}

else if (flag==3){ 
	TH1F *ht3 = new TH1F("ht3","T2 singles rate/current",100000,38200,58700); // should fall at 50 kHz / uA, but give large bins for safety
	ht3->GetXaxis()->SetTitle("T2 singles/uAmp current on target");ht3->GetXaxis()->CenterTitle();
	tree2->Draw("(evRightT2_r/(0.00038*evRightdnew_r))>>ht3","","");
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
	TLegend* legStable3 = new TLegend(0.12,0.75,0.55,0.65);
	legStable3->AddEntry((TObject*)0,Form("Avg T2 rate/uA beam = %0.1f +- %0.1f",T2s_per_uA_mean,T2s_per_uA_stdv),"");
	legStable3->Draw();
	//cout<<"Average (T2 rate/uA beam) = "<<T2s_per_uA_mean<<endl;
	//cout<<"Std dev (T2 rate/uA beam) = "<<T2s_per_uA_stdv<<endl;
}

}
