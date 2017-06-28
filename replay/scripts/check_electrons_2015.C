void check_electrons(void){

  Int_t Run;
  Int_t spec_mom;

  cout<<"Enter Run Number (e.g. 10414):"<<endl;
  cin>>Run;

  TChain* t = new TChain("T");
  if (Run<20000) { 
      t->Add(Form("/adaqfs/home/a-onl/gmp/replay/Rootfiles/left_gmp_%d*.root",Run));
  } else {
      t->Add(Form("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_%d*.root",Run));
  }
  Int_t entries = t->GetEntries();  

  cout<<"RootFile contains " <<entries<< " Entries."<<endl;
  
  //Variables
  Double_t numtrk_d;
  Int_t numtrk;
  //Double_t dp[50];
  //Int_t numdp;
  Double_t tr_mom[50];
  Int_t num_p;
  
  Double_t prl1;
  Double_t prl2;
  Double_t prltot;
  Double_t e_p;
  
  Double_t gcsum;  
  Double_t Nphe;

  //Tree Branches
  if (Run<20000) {
      t->SetBranchAddress("L.tr.n",&numtrk_d);
      //t->SetBranchAddress("L.tr.tg_dp",dp);
      //t->SetBranchAddress("Ndata.L.tr.tg_dp",&numdp);
      t->SetBranchAddress("L.tr.p",tr_mom);
      t->SetBranchAddress("Ndata.L.tr.p",&num_p);
      t->SetBranchAddress("L.cer.asum_c",&gcsum);
      t->SetBranchAddress("L.prl1.e",&prl1);
      t->SetBranchAddress("L.prl2.e",&prl2);
  } else {
      t->SetBranchAddress("R.tr.n",&numtrk_d);
      //t->SetBranchAddress("R.tr.tg_dp",dp);
      //t->SetBranchAddress("Ndata.L.tr.tg_dp",&numdp);
      t->SetBranchAddress("R.tr.p",tr_mom);
      t->SetBranchAddress("Ndata.R.tr.p",&num_p);
      t->SetBranchAddress("R.cer.asum_c",&gcsum);
      t->SetBranchAddress("R.ps.e",&prl1);
      t->SetBranchAddress("R.sh.e",&prl2);
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
  
  for(Int_t i=0; i<entries; i++){
  t->GetEntry(i);
  if(i%10000==0)cout <<"Events Analyzed = " <<i<<endl; 
  
  numtrk = (Int_t)numtrk_d;
  
  prltot=prl1+prl2;
  if (num_p>0 && numtrk>0) e_p = prltot/tr_mom[0]/1000.;
  else e_p = 0.;
  
  Nphe = gcsum/100.;
  
  if(numtrk==1)h1->Fill(e_p,Nphe);
  if(numtrk==1 && Nphe>5 && e_p>.8)h2->Fill(e_p,Nphe);

  }

  cout<<"-----------------------------------------------------"<<endl;
  cout<<"Number of Good Electrons = "<<h2->GetEntries()<<endl;
  cout<<"-----------------------------------------------------"<<endl;

  Int_t nelectron=0;
  nelectron=h2->GetEntries();

  TCanvas *c1 = new TCanvas("c1","Electron Selection Histogram");
  gPad->SetLogz();
  h1->Draw("colz");

  TLine *line_ep = new TLine(0.8,0,0.8,30);
  line_ep->SetLineColor(kRed);
  line_ep->SetLineWidth(2);
  line_ep->Draw();

  TLine *line_nphe = new TLine(0,5,2,5);
  line_nphe->SetLineColor(kRed);
  line_nphe->SetLineWidth(2);
  line_nphe->Draw();

  TLatex * tex = new TLatex(1.2,15,Form("Number of Electrons = %d",nelectron));
  tex->SetTextColor(kRed);
  tex->SetTextSize(0.035);
  tex->Draw();
  
  c1->Update();
  
  // TCanvas *c2 = new TCanvas("c2","Selected Electrons");
  //h2->Draw("colz");

}
