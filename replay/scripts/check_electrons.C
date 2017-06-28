void check_electrons(int run){

  Int_t Run;
  Int_t spec_mom;
  Double_t epcut = 0;
  TString arm;
  
  const Double_t Wlow = 0.8;
  const Double_t Whigh = 1.1;

  TChain* t = new TChain("T");
  if (run < 20000) {
    t->Add(Form("~/gmp/replay/Rootfiles/left_gmp_%d*.root",run));
    epcut = .5;
    arm = "L";
  }
  else {
	t->Add(Form("~/gmp/replay/Rootfiles/right_gmp_%d*.root",run));
	epcut = .8;
	arm = "R";
  }

  Int_t entries = t->GetEntries();  

  //cout << "This chain contains " << t->GetNtrees() << " trees and " << entries << " Entries." << endl;
  
  //Variables
  Double_t numtrk_d;
  Int_t numtrk;
  Double_t dp[50];
  Double_t phi[50];
  Double_t vertexz[50];
  //Int_t numdp;
  Double_t tr_mom[50];
  Int_t num_p;
  Double_t theta[50];
  
  Double_t prl1;
  Double_t prl2;
  Double_t prltot;
  Double_t e_p;
  
  Double_t gcsum;  
  Double_t Nphe;
  Double_t w2;

  //Tree Branches
  t->SetBranchStatus("*",0);
  t->SetBranchStatus(Form("%s.tr.*",arm.Data()),1);
  t->SetBranchAddress(Form("%s.tr.n",arm.Data()),&numtrk_d);
  t->SetBranchAddress(Form("%s.tr.tg_dp",arm.Data()),dp);
  t->SetBranchAddress(Form("%s.tr.tg_ph",arm.Data()),phi);
  t->SetBranchAddress(Form("%s.tr.tg_th",arm.Data()),theta);
  t->SetBranchAddress(Form("%s.tr.vz",arm.Data()),vertexz);
  //t->SetBranchAddress("Ndata.L.tr.tg_dp",&numdp);
  t->SetBranchAddress(Form("%s.tr.p",arm.Data()),tr_mom);
  t->SetBranchStatus(Form("Ndata.%s.tr.p",arm.Data()),1);
  t->SetBranchAddress(Form("Ndata.%s.tr.p",arm.Data()),&num_p);
  t->SetBranchStatus(Form("%s.cer.asum_c",arm.Data()),1);
  t->SetBranchAddress(Form("%s.cer.asum_c",arm.Data()),&gcsum);
  t->SetBranchStatus(Form("%s.*.e",arm.Data()),1);
  if (arm=="L") {
      t->SetBranchAddress("L.prl1.e",&prl1);
      t->SetBranchAddress("L.prl2.e",&prl2);
      t->SetBranchStatus("EKL.W2",1);
      t->SetBranchAddress("EKL.W2",&w2);
  }
  if (arm=="R") {
      t->SetBranchAddress("R.ps.e",&prl1);
      t->SetBranchAddress("R.sh.e",&prl2);
      t->SetBranchStatus("EKR.W2",1);
      t->SetBranchAddress("EKR.W2",&w2);
  }


  TH2F *h1 = new TH2F("h1","Gas Cherenkov N_{phe} vs. Calorimeter E/p:One Track Events",1000,0,2,3000,0,30);
  h1->GetYaxis()->SetTitle("N_{phe}");
  h1->GetYaxis()->CenterTitle();
  h1->GetXaxis()->SetTitle("E/p");
  h1->GetXaxis()->CenterTitle();
 
  TH2F *h2 = new TH2F("h2","Gas Cherenkov N_{phe} vs. Calorimeter E/p: After Cuts",1000,0,2,3000,0,30);
  h2->GetYaxis()->SetTitle("N_{phe}");
  h2->GetYaxis()->CenterTitle();
  h2->GetXaxis()->SetTitle("E/p");
  h2->GetXaxis()->CenterTitle();

  TH1F* hW = new TH1F("hW","Invariant mass of undetected particles",160,0.4,2.0);
  hW->GetYaxis()->SetTitle("Yield");
  hW->GetYaxis()->CenterTitle();
  hW->GetYaxis()->SetTitleSize(0.04);
  hW->GetYaxis()->SetTitleOffset(0.96);
  //hW->GetYaxis()->SetLabelSize(0.05);
  hW->GetXaxis()->SetTitle("Invariant mass (GeV/c)");
  hW->GetXaxis()->CenterTitle();
  hW->GetXaxis()->SetTitleSize(0.04);
  hW->GetXaxis()->SetTitleOffset(0.96);
  //hW->GetXaxis()->SetLabelSize(0.05);
  
  /*
  TH2F* hdp = new TH2F("hdp","Tg dp vs. Tg phi",100,-0.06,0.06,100,-0.06,0.06);
  hdp->GetYaxis()->SetTitle("Target dp");
  hdp->GetYaxis()->CenterTitle();
  hdp->GetXaxis()->SetTitle("Target phi");
  hdp->GetXaxis()->CenterTitle();
  */
  
  Int_t nElastic = 0;

  for(Int_t i=0; i<entries; i++){
  t->GetEntry(i);
  if(i%10000==0)cout <<"Events Analyzed = " <<i<<"/"<<entries<<endl; 
  
  numtrk = (Int_t)numtrk_d;
  
  prltot=prl1+prl2;
  if (num_p>0 && numtrk>0) e_p = prltot/tr_mom[0]/1000.;
  else e_p = 0.;
  
  Nphe = gcsum/100.;
  
  if(numtrk==1)h1->Fill(e_p,Nphe);
  if(numtrk==1 && Nphe>5 && e_p>epcut && TMath::Abs(dp[0])<0.05 && TMath::Abs(vertexz[0])<0.08) {
    hW->Fill(TMath::Sqrt(w2));
    //hdp->Fill(phi[0],dp[0]);
    h2->Fill(e_p,Nphe);
    if (TMath::Sqrt(w2)>Wlow && TMath::Sqrt(w2)<Whigh)	nElastic++;
  }
  //  if(numtrk==1 && TMath::Abs(dp[0])<0.05 && TMath::Abs(vertexz[0])<0.1 && TMath::Abs(theta[0])<0.1 && TMath::Abs(phi[0])<0.1) {
  //	hW->Fill(TMath::Sqrt(w2));
  //  }
  
  }

  cout<<"-----------------------------------------------------"<<endl;
  cout<<"Number of Electrons = "<<h2->GetEntries()<<endl;
  cout<<"Number of Elastic Electrons = "<<nElastic<<endl;
  //cout<<"Number of Good Events = "<<hW->GetEntries()<<endl;
  cout<<"-----------------------------------------------------"<<endl;

  Int_t nelectron=0;
  nelectron=h2->GetEntries();

  TCanvas *c1 = new TCanvas("c1","Electron Selection Histogram");
  gPad->SetLogz();
  h1->Draw("colz");

  TLine *line_ep = new TLine(epcut,0,epcut,30);
  line_ep->SetLineColor(kRed);
  line_ep->SetLineWidth(2);
  line_ep->Draw();

  TLine *line_nphe = new TLine(0,5,2,5);
  line_nphe->SetLineColor(kRed);
  line_nphe->SetLineWidth(2);
  line_nphe->Draw();

  TLatex * tex = new TLatex(1.1,12,Form("Number of Electrons = %d",nelectron));
  tex->SetTextColor(kRed);
  tex->SetTextSize(0.035);
  tex->Draw();
  
  c1->Update();

  TCanvas *c2 = new TCanvas("c2","Invariant mass of selected electron events");
  hW->Draw();

  c2->Update();

  TLine* cut1 = new TLine(Wlow,gPad->GetUymin(),Wlow,gPad->GetUymax());
  cut1->SetLineColor(kRed);
  cut1->SetLineWidth(2);
  cut1->Draw();

  TLine* cut2 = new TLine(Whigh,gPad->GetUymin(),Whigh,gPad->GetUymax());
  cut2->SetLineColor(kRed);
  cut2->SetLineWidth(2);
  cut2->Draw();

  TLatex * tex1 = new TLatex(0.5,0.6,Form("Number of Elastic Electrons = %d",nElastic));
  tex1->SetNDC();
  tex1->SetTextColor(kRed);
  tex1->SetTextSize(0.035);
  tex1->Draw();

  /*
  TLine* cutlow = new TLine(0.8,0,0.8,hW->GetMaximum());
  cutlow->SetLineColor(kRed);
  cutlow->SetLineWidth(2);
  cutlow->Draw();
  TLine* cuthigh = new TLine(1.1,0,1.1,hW->GetMaximum());
  cuthigh->SetLineColor(kRed);
  cuthigh->SetLineWidth(2);
  cuthigh->Draw();

  TCanvas* c3 = new TCanvas("c3","Tg dp vs. Tg phi");
  hdp->Draw();
*/
}
