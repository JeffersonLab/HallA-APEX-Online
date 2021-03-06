//Check if Signal is inside Gate
//Barak Schmookler, Feb 2016
void coin_counter(Int_t flag, TString drawoption){

// by hand
/*int runnumber = 4261;
TString rootfile = Form("/adaqfs/home/a-onl/apex_work/ranit/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_online_%d.root",runnumber);
TFile *f = TFile::Open(rootfile,"READ");
TTree *tree = (TTree*)f->Get("T");
*/
  
  //  gPad->SetMarkerStyle(21);

// onlineGUI64
  gStyle->SetOptStat(0);
  gPad->SetFillStyle(4000);
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
TH1F *ht2 = new TH1F("ht2","DR.rrawt6",10,1660,1670);
//TH1F *ht1 = new TH1F("ht1","DR.rrawt2 && T6 triggered",200,1600,1800);
//TH1F *ht1 = new TH1F("ht1","DR.rrawt2 && T6 triggered",300,-1000,50000);
ht1->GetXaxis()->SetTitle("DR.rrawt2 (0.5 ns)");ht1->GetXaxis()->CenterTitle();
ht2->GetXaxis()->SetTitle("DR.rrawt6 (0.5 ns)");ht2->GetXaxis()->CenterTitle();
//TH1F *hr0 = new TH1F("hr0","",1000,-200,5000);


tree->Draw("DR.rrawt2>>ht1","fEvtHdr.fEvtType==6","kRED");
tree->Draw("DR.rrawt6>>ht2","","kRED");
double initial_T2  =0.0;
double initial_T6  =0.0;
double initial_dnew=0.0;

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
f2a->SetLineColorAlpha(3,1.0);
TF1* f31=new TF1("f31","pol1",bin_min,bin_max);
f31->SetLineColorAlpha(2,1.0);

TF1* total=new TF1("total","gaus(0)+pol1(3)",1655,1735);

total->SetLineColor(1);
double par[5];
//f2a = ht1->GetFunction("gaus");
ht1->Fit(f2a,"R");
ht1->Fit(f31,"R+");
f2a->GetParameters(&par[0]);
f31->GetParameters(&par[3]);
total->SetParameters(par);

ht1->Fit(total,"R"); // redraw and erase previous fit lines

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
int n_binsT6 = ht2->GetNbinsX();
double n_good_coins=0.0;
double n_total_T2=0.0;
double n_subtracted_coins=0.0;
double func = 0.0;
double ratio_good_to_T6=0.0;
double n_total_T6=0.0;
for ( int j = 0; j < n_binsT6+1;j++ ){
	n_total_T6+=ht2->GetBinContent(j);
}
//cout<<" "<<endl;
//cout<<"Total number of T6 = "<<n_total_T6<<endl;
for ( int i = 0; i < n_bins+1 ; i++ ){
	if (i >ht1->FindBin(bin_min) && i < ht1->FindBin(bin_max)){
		func = par2[3] + par2[4]*(ht1->GetXaxis()->GetBinCenter(i));
		n_good_coins+=(ht1->GetBinContent(i)-func);
		n_subtracted_coins+=func;
		//cout<<"accidentals = "<<func<<endl;

	}
	//cout<<"time (ns) = "<<ht1->GetXaxis()->GetBinCenter(i)<<endl;
	n_total_T2+=ht1->GetBinContent(i);
}
ratio_good_to_T6=n_good_coins/n_total_T6;
//cout<<"number of good coins = "<<n_good_coins<<endl;
//cout<<"number of subtracted coins = "<<n_subtracted_coins<<endl;
//cout<<"total number of coin triggers = "<<n_total_T2<<endl;
//cout<<"integral = "<<ht1->Integral()<<endl;
//cout<<"percent of good coins from total number of coincidence triggers = "<<100.0*ratio_good_to_all<<"%"<<endl;
TLegend* leg = new TLegend(0.15,0.15,0.65,0.35);
leg->AddEntry((TObject*)0,Form("Number of good T2s = %0.1f",n_good_coins),"");
//leg->AddEntry((TObject*)0,Form("Number of subtracted accidentals = %0.1f",n_subtracted_coins),"");
leg->AddEntry((TObject*)0,Form("Total number of T2 triggers = %0.1f",n_total_T2),"");
leg->AddEntry((TObject*)0,Form("Total number of T6 triggers = %0.1f",n_total_T6),"");
leg->AddEntry((TObject*)0,Form("Percent of good coincidences = %0.1f percent",100.0*ratio_good_to_T6),"");
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
