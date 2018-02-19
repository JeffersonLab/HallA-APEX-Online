//plot s0 and s2 TDC/ADC hits

void scin_hit_L(){

  gStyle->SetOptStat(0);
  TTree *T = (TTree*)gDirectory->Get("T");

  TString arm;
  Int_t maintrig;
  Double_t thres=0;// adc pedestal level
  
  arm = "L";maintrig=1;thres=100;


  //   plot s0 and s2 adc and tdc hits 
   

  Double_t s2la[16],s2ra[16],s2lt[16],s2rt[16];
  Double_t s0la,s0ra,s0lt,s0rt,prl1,prl2;

  T->SetBranchAddress(arm+".s0.la_p",&s0la);
  T->SetBranchAddress(arm+".s0.ra_p",&s0ra);
  T->SetBranchAddress(arm+".s0.lt",&s0lt);
  T->SetBranchAddress(arm+".s0.rt",&s0rt);

  T->SetBranchAddress(arm+".s2.la_p",s2la);
  T->SetBranchAddress(arm+".s2.ra_p",s2ra);
  T->SetBranchAddress(arm+".s2.lt",s2lt);
  T->SetBranchAddress(arm+".s2.rt",s2rt);

  T->SetBranchAddress(arm+".prl1.asum_p",&prl1);
  T->SetBranchAddress(arm+".prl2.asum_p",&prl2);


  Double_t scale2=1;//0.937; // tdc fired but adc<50
  Double_t scale0=1;
  Double_t max;
    

   
  TH1F *ns0lt= new TH1F("ns0lt","",3,0,2);
  TH1F *ns0rt= new TH1F("ns0rt","",3,0,2);
  TH1F *ns0la= new TH1F("ns0la","",3,0,2);
  TH1F *ns0ra= new TH1F("ns0ra","",3,0,2);

  Int_t bin,b1,b2;
  bin=18;
  b1=-1;
  b2=17;
  TH1F *ns2rt = new TH1F("ns2rt"," Right PMT TDC hits",bin,b1,b2);
  TH1F *ns2ra = new TH1F("ns2ra"," Right PMT ADC hits",bin,b1,b2);
  TH1F *ns2la = new TH1F("ns2la"," Left PMT ADC hits",bin,b1,b2);
  TH1F *ns2lt = new TH1F("ns2lt"," Left PMT TDC hits",bin,b1,b2);


  //----------------Get Entry---------------------------
  Int_t nentries=T->GetEntries();
  cout<<"Total Number of Events = "<<nentries<<endl;
  for(Int_t i=0;i<nentries;i++){

    if(i%100000==0) cout << " events processed = " << i << endl;
    T->GetEntry(i);
    if (prl1>400 && prl2>400){
      if (s0la>thres) ns0la->Fill(1);
      if (s0ra>thres) ns0ra->Fill(1);
      if (s0lt>100) ns0lt->Fill(1,scale0);
      if (s0rt>100) ns0rt->Fill(1,scale0);
      
      for(Int_t j=0;j<16;j++){
	if (s2la[j]>50) ns2la->Fill(j);
	if (s2ra[j]>50) ns2ra->Fill(j);
	if (s2lt[j]>100) ns2lt->Fill(j,scale2);
	if (s2rt[j]>100) ns2rt->Fill(j,scale2);
      }
    }
  }

  //  TCanvas *c1 = new TCanvas("c1","",200,10,600,800);
  TVirtualPad *current_pad = gPad;
  TCanvas *c1=current_pad->GetCanvas();
  c1->cd();
  TPad *pad1 = new TPad("pad1", "",0.0,0.94,1.0,1.0,31);
  TPad *pad2 = new TPad("pad2", "",0.0,0.47,1.0,0.94,32);
  TPad *pad3 = new TPad("pad3", "",0.0,0.0,1.0,0.47,33);

  gPad->SetFillStyle(4000);
  gPad->SetFillColor(0);
  //gPad->SetTopMargin(0.2);
    // c1->Divide(1,2);
  pad1->SetFillStyle(4000);
  pad1->SetFillColor(0);
  pad2->SetFillStyle(4000);
  pad2->SetFillColor(0);
  pad3->SetFillStyle(4000);
  pad3->SetFillColor(0);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();

  pad2->cd();
  ns0ra->SetLineColor(kBlack);
  ns0la->SetLineColor(kGreen);
  ns0rt->SetLineColor(kRed);
  ns0lt->SetLineColor(kBlue);
    
  ns0la->Draw();
  ns0ra->Draw("same");
  ns0lt->Draw("same");
  ns0rt->Draw("same");
  max=ns0lt->GetMaximum();
  ns0la->SetMaximum(max*1.5);
  ns0la->SetXTitle("s0 Paddle");
  ns0la->SetYTitle("number of hits");
  gPad->SetGrid(1,1);
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->SetTextSize(0.03);
  leg->SetTextFont(60);
   //LHRS s0A=s0.r=Top
    leg->AddEntry(ns0lt,"Bottom PMT TDC Hits");
    leg->AddEntry(ns0rt,"Top PMT TDC Hits");
    leg->AddEntry(ns0la,"Bottom PMT ADC Hits");
    leg->AddEntry(ns0ra,"Top PMT ADC Hits");
  

  leg->Draw();
  ns0la->SetTitle("S0: Number of Hits");


  //---------------s2----------------------
 
  pad3->cd();

  
  ns2ra->SetLineColor(kBlack);
  ns2la->SetLineColor(kGreen);
  ns2rt->SetLineColor(kRed);
  ns2lt->SetLineColor(kBlue);
    
  ns2la->Draw();
  ns2ra->Draw("same");
  ns2lt->Draw("same");
  ns2rt->Draw("same");
  max=ns2lt->GetMaximum();
  ns2la->SetMaximum(max*1.5);
  ns2la->SetXTitle("paddle number");
  ns2la->SetYTitle("number of hits");
  gPad->SetGrid(1,1);
  TLegend *leg2 = new TLegend(0.6,0.7,0.9,0.9);
  leg2->SetTextSize(0.03);
  leg2->SetTextFont(60);
  leg2->AddEntry(ns2rt);
  leg2->AddEntry(ns2lt);
  leg2->AddEntry(ns2ra);
  leg2->AddEntry(ns2la);
  leg2->Draw();

  ns2la->SetTitle("S2: Number of Hits per Paddle");

 

  return 0;
}
