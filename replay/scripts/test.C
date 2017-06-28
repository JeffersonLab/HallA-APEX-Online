void test(Int_t run=0){

  Int_t Run;
  Int_t spec_mom;
  Double_t epcut = 0;
  TString arm;

  const Double_t Wlow = 0.8;
  const Double_t Whigh = 1.25;

  TChain* t = new TChain("T");
  if (run==0) { // Left HRS
    cout<<"Doing Left HRS..."<<endl;
    
    epcut = .5;
    arm = "L";
    //Kinematic K4-11
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_13920*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1392[1,3-9]*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1393[0-7]*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1394[0-9]*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1395[^48]*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1396[0-1,3-9]*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1397[0-9]*");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_1398[0-1]*");
    
    //Kinematic K4-10
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1398[2-4,6-9]*");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1399[0-2,5-9]*");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1400[1,2,4,5,8,9]*");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1400[8-9]*");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1401[0-9]*");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1402[0-1]*");

    //Kinematic K4-9
    //  t->Add("~/gmp/replay/Rootfiles/left_gmp_1402[2,3,5-9]*");
    //  t->Add("~/gmp/replay/Rootfiles/left_gmp_1403[0,4,5,7-9]*");
    //  t->Add("~/gmp/replay/Rootfiles/left_gmp_1404[3,4]*");
    
    //Kinematic K3-6
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_14055*.root");
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_14056*.root");
    
    //Kinematic K3-4
    //t->Add("~/gmp/replay/Rootfiles/left_gmp_14090*.root");

    //Kinematic K3-7
    t->Add("~/gmp/replay/Rootfiles/left_gmp_1409[3-5]*.root");
     t->Add("~/gmp/replay/Rootfiles/left_gmp_1410[2-5]*.root");
    
    
    //Kinematic K3-8
    //  t->Add("~/gmp/replay/Rootfiles/left_gmp_14059*.root");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1406[0,1,3-9]*.root");
    // t->Add("~/gmp/replay/Rootfiles/left_gmp_1407[0-2,4]*.root");
    
  }
  else { // Right HRS
    cout<<"Doing Right HRS..."<<endl;

    epcut = .8;
    arm = "R";
    //Kinematic K4-12
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2285[1-9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2286[^48]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2287[1-9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2288[0-9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2289[1-9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2290[0,3-7,9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2291[0,2-3,6-9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2292[0-9]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2293[^24]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2294[3-8]*");
//    t->Add("~/gmp/replay/Rootfiles/right_gmp_2295[1,2]*");
    
    //Kinematic K3-9
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2295[5,6,9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2296[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2297[0-1,3,4]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2298[4-5,7-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2299[0,3-6]*.root");

    // GMp parasitic runs (4 pass)
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2300[5-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2301[^67]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2302[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2303[^9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2304[^02]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2305[^9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2306[^129]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2307[2-5,7]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2308[2-5,7-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2309[^78]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2310[8,9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2311[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2312[0,2]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2313[4-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2314[^7]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2315[^7]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2316[^6]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2316[^6]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2317[^89]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2318[0-4]*.root");
    
    // GMp parasitic runs (5 pass)
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2322[5,6,8,9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2323[^47]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2324[^4]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2325[4-7,9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2326[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2327[^9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2328[^0]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2329[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2330[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2331[^3]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2332[0]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2333[^12]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2334[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2335[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2336[^34]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2337[^37]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2338[^089]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2339[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2340[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2341[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2342[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2343[0-9]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2344[^023]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2345[^1]*.root");
    //t->Add("~/gmp/replay/Rootfiles/right_gmp_2346[0-4]*.root");
    //
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2219[67]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2220[2-7,9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2221[0-2]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2222[5-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2223[568]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2224[01]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2232[69]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2233[0,3-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2234[0-2,6-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2235[2-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2236[01,6-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2237[09]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2238[0-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2239[0-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2240[0-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2241[0-3,5,9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2242[^7]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2243[^19]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2244[0-9]*.root");
    t->Add("~/gmp/replay/Rootfiles/right_gmp_2245[^345]*.root");
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
  
  
  //TH2F* hdp = new TH2F("hdp","Tg dp vs. Tg phi",100,-0.06,0.06,100,-0.06,0.06);
  //hdp->GetYaxis()->SetTitle("Target dp");
  //hdp->GetYaxis()->CenterTitle();
  //hdp->GetXaxis()->SetTitle("Target phi");
  //hdp->GetXaxis()->CenterTitle();
  
  
  Int_t nElastic = 0;

  for(Int_t i=0; i<entries; i++){
  t->GetEntry(i);
  if(i%10000==0)cout <<"Events Analyzed = " << i << "/" <<  entries << endl; 
  
  numtrk = (Int_t)numtrk_d;
  
  prltot=prl1+prl2;
  if (num_p>0 && numtrk>0) e_p = prltot/tr_mom[0]/1000.;
  else e_p = 0.;
  
  Nphe = gcsum/100.;
  
  if(numtrk==1)h1->Fill(e_p,Nphe);
  if(numtrk==1 && Nphe>5 && e_p>epcut && TMath::Abs(dp[0])<0.05 && TMath::Abs(vertexz[0])<0.08) {
  //if(numtrk==1 && Nphe>5 && e_p>epcut && TMath::Abs(dp[0])<0.05) {
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
