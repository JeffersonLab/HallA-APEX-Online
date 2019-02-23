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
  tree->Draw("DR.rrawt2>>ht1","fEvtHdr.fEvtType==6","kRED");
  } else if (flag==2){
  TH1F *ht2 = new TH1F("ht2","DR.rrawt2 Cut",200,1600,1800);
  ht2->GetXaxis()->SetTitle("DR.rrawt2");ht2->GetXaxis()->CenterTitle();
  tree->Draw("DR.rrawt2>>ht2","fEvtHdr.fEvtType==6&&DR.rrawt2>1650","kRED");
  }
  else if (flag==3){
//TCut cut1 = "L.s0.lt>0";
//TCut cut2 = "L.s0.rt>0";

//TCanvas*  c1 = new TCanvas("c1","");
TH1F *ht1 = new TH1F("ht1","DR.rrawt2 && T6 triggered",130,1630,1760);
//TH1F *ht1 = new TH1F("ht1","DR.rrawt2 && T6 triggered",200,1600,1800);
//TH1F *ht1 = new TH1F("ht1","DR.rrawt2 && T6 triggered",300,-1000,50000);
ht1->GetXaxis()->SetTitle("DR.rrawt2 (ns)");ht1->GetXaxis()->CenterTitle();
//TH1F *hr0 = new TH1F("hr0","",1000,-200,5000);


tree->Draw("DR.rrawt2>>ht1","fEvtHdr.fEvtType==6","kRED");

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
//c1->Divide(2,1);

double hl0m, hl0s, hl0m1, hl0s1, hl0m2, hl0s2;
double hr0m, hr0s, hr0m1, hr0s1, hr0m2, hr0s2;
double er_l, er_r;
gPad->SetLogy();

int bin_min = 1655;
int bin_max = 1735;
int bin_gaus_min = 1680;
int bin_gaus_max = 1700;

TF1* f2a=new TF1("f2a","gaus",bin_gaus_min,bin_gaus_max);
f2a->SetLineColor(0);
TF1* f31=new TF1("f31","pol1",bin_min,bin_max);
f31->SetLineColor(0);

TF1* total=new TF1("total","gaus(0)+pol1(3)",1655,1735);

total->SetLineColor(1);
double par[5];
//f2a = ht1->GetFunction("gaus");
ht1->Fit(f2a,"R");
ht1->Fit(f31,"R+");
f2a->GetParameters(&par[0]);
f31->GetParameters(&par[3]);
total->SetParameters(par);

ht1->Fit(total,"R+");



double par2[5];

total->GetParameters(&par2[0]);
cout<<"par2 3: "<<par2[3]<<" par2 4: "<<par2[4]<<endl;
TF1* f32=new TF1("f32","[3] + [4]*x",1655,1735);
f32->SetParameters(par2);
f32->SetLineColor(2);
f32->Draw("SAME");


//f2a = ht1->GetFunction("gaus");
//hr0m1 = f2a->GetParameter(1);
//hr0s1 = f2a->GetParameter(2);
//er_r = f2a->GetParError(1);

/*
for each bin_i (nbins between bin_number_at_value(1655) to bin_number_at_value(1735) do sum{bin_i content - line value at x=getBinCenter(bin_i)};
*/

int n_bins = ht1->GetNbinsX();
double n_good_coins=0.0;
double n_total_coins=0.0;
double n_subtracted_coins=0.0;
double func = 0.0;
double ratio_good_to_all=0.0;
for ( int i = 0; i < n_bins+1 ; i++ ){
	if (i >ht1->FindBin(bin_min) && i < ht1->FindBin(bin_max)){
		func = par2[3] + par2[4]*(ht1->GetXaxis()->GetBinCenter(i));
		n_good_coins+=(ht1->GetBinContent(i)-func);
		n_subtracted_coins+=func;
		//cout<<"accidentals = "<<func<<endl;

	}
	//cout<<"time (ns) = "<<ht1->GetXaxis()->GetBinCenter(i)<<endl;
	n_total_coins+=ht1->GetBinContent(i);
}
ratio_good_to_all=n_good_coins/n_total_coins;
//cout<<"number of good coins = "<<n_good_coins<<endl;
//cout<<"number of subtracted coins = "<<n_subtracted_coins<<endl;
//cout<<"total number of coin triggers = "<<n_total_coins<<endl;
//cout<<"integral = "<<ht1->Integral()<<endl;
//cout<<"percent of good coins from total number of coincidence triggers = "<<100.0*ratio_good_to_all<<"%"<<endl;
TLegend* leg = new TLegend(0.15,0.15,0.65,0.35);
leg->AddEntry((TObject*)0,Form("Number of good coincidences = %0.1f",n_good_coins),"");
//leg->AddEntry((TObject*)0,Form("Number of subtracted accidentals = %0.1f",n_subtracted_coins),"");
leg->AddEntry((TObject*)0,Form("Total number of coincidence triggers = %0.1f",n_total_coins),"");
leg->AddEntry((TObject*)0,Form("Percent of good coincidences = %0.1f percent",100.0*ratio_good_to_all),"");
leg->Draw();
}
  //if(flag==1){
    
    //TH1F *ht1 = new TH1F("ht1","DR.rrawt2",200,1600,1800);
    //ht1->GetXaxis()->SetTitle("DR.rrawt2");ht1->GetXaxis()->CenterTitle();
    // ht1->GetYaxis()->SetTitle("Amplitude [ADC Channel]");ht1->GetYaxis()->CenterTitle();
    // ht1->SetMarkerStyle(3);ht1->SetMarkerSize(0.75);
    
    //tree->Draw("DR.rrawt2>>ht1","fEvtHdr.fEvtType==6","kRED");
  //}
  
  

}
