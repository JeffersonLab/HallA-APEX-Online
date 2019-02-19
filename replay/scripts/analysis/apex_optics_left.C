void apex_optics_left(Int_t run_no = 3663){

  // ----------------------------------------------------------------------

  Int_t    prl2_min  = 750;                   // the limit for (L.prl1.e+prl2_e*L.prl2.e)
  Double_t prl2_e    = 0.9;                   // for (L.prl1.e+prl2_e*L.prl2.e)
  Int_t    prl1_min  = 250;                   // for L.prl1.e
  Int_t    cer_min = 500;                   // the limit for Cherenkov
  Double_t phmin   = -0.04,  phmax = 0.05;  // the limits for tg_ph
  Double_t thmin   = -0.06,  thmax = 0.07;  // the limits for tg_th
  Double_t dpmin   = -0.035, dpmax = 0.06;  // the limits for tg_dp
  Double_t ytmin   = -0.2,   ytmax = 0.2;   // the limits for tg_y

  // ----------------------------------------------------------------------

  //  TCut TriggerCut = "(DR.evtypebits&1<<1)==1<<1 && (DR.evtypebits&1<<7)!=1<<7";  
  TCut cut_nclust = "L.vdc.u1.nclust==1&&L.vdc.v1.nclust==1&&L.vdc.u2.nclust==1&&L.vdc.u1.nclust==1";
  TCut cut_ntrack = "L.tr.n==1";
  TCut GeneralCut = cut_nclust + cut_ntrack; // + TriggerCut
  TCut ct1        = Form("(L.prl1.asum_c+%.2f*L.prl2.asum_c)>%d&&L.prl1.asum_c>%d", prl2_e, prl2_min, prl1_min);
  TCut ct2        = Form("L.cer.asum_c>%d", cer_min);
  TCut ct3        = Form("L.tr.tg_ph<%.2f&&L.tr.tg_ph>%.2f", phmax, phmin);
  TCut ct4        = Form("L.tr.tg_th<%.2f&&L.tr.tg_th>%.2f", thmax, thmin);
  TCut ct5        = Form("L.tr.tg_dp<%.2f&&L.tr.tg_dp>%.2f", dpmax, dpmin);
  TCut VertexCut  = Form("L.tr.tg_y<%.2f&&L.tr.tg_y>%.2f", ytmax, ytmin);

  GeneralCut += VertexCut;

  // ----------------------------------------------------------------------

  TChain *T = new TChain("T");
  T->Add(Form("/chafs1/work1/apex/Rootfiles/apex_online_%d.root", run_no));
  //T->Add(Form("/scratch/david/apex/Rootfiles/apex_online_%d.root", run_no));

  // ----------------------------------------------------------------------
  // 1D LHRS Focal plane variables 
  // ----------------------------------------------------------------------

  TCanvas *c1 = new TCanvas("c1","LHRS Focal Plane Variables",1000,800); 
  c1->Divide(3,2); 

  TH1F* h71 = new TH1F("h71","h71",200,-1.5,1.5); 
  TH1F* h72 = new TH1F("h72","h72",200,-0.2,0.2); 
  TH1F* h73 = new TH1F("h73","h73",200,-0.1,0.1); 
  TH1F* h74 = new TH1F("h74","h74",200,-0.2,0.2); 
  Int_t hmax, hmin;

  c1->cd(1);
  TH2F *h1 = new TH2F("h1","h1",100,0,2000,100,0,2000);
  T->Draw("L.prl1.asum_c:L.prl2.asum_c>>h1",GeneralCut,"colz");
  assert(h1);
  h1->SetTitle("LHRS Shower sum vs Preshower sum");
  h1->GetYaxis()->SetTitleOffset(1.0);
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetTitle("L.prl2.asum_c");
  h1->GetYaxis()->SetTitle("L.prl1.asum_c");
  TLine *line1 = new TLine(0,prl2_min,prl2_min/prl2_e,0);
  line1->SetLineColor(2);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine *line2 = new TLine(0,prl1_min,2000,prl1_min);
  line2->SetLineColor(2);
  line2->SetLineWidth(2);
  line2->Draw();

  c1->cd(2)->SetLogy(1);
  TH1F *h2 = new TH1F("h2","h2",500,0,2500);
  T->Draw("L.cer.asum_c>>h2",GeneralCut&&ct1);
  h2->SetTitle("LHRS Cherenkov sum");
  hmin = h2->GetMinimum();
  hmax = h2->GetMaximum();
  TLine *line3 = new TLine(cer_min,hmin,cer_min,hmax);
  line3->SetLineColor(2);
  line3->SetLineWidth(2);
  line3->Draw();

  c1->cd(3); 
  T->Draw("L.tr.y>>h72","","colz"); 
  h72->SetTitle("LHRS FP y"); 
  hmin = h72->GetMinimum();
  hmax = h72->GetMaximum();
  TLine *line18 = new TLine(-0.15,hmin,-0.15,hmax);
  line18->SetLineColor(2);
  line18->SetLineWidth(2);
  line18->Draw();
  TLine *line19 = new TLine(0.15,hmin,0.15,hmax);
  line19->SetLineColor(2);
  line19->SetLineWidth(2);
  line19->Draw();

  c1->cd(4); 
  T->Draw("L.tr.x>>h71","","colz"); 
  h71->SetTitle("LHRS FP x"); 
  hmin = h71->GetMinimum();
  hmax = h71->GetMaximum();
  TLine *line118 = new TLine(-0.7,hmin,-0.7,hmax);
  line118->SetLineColor(2);
  line118->SetLineWidth(2);
  line118->Draw();
  TLine *line119 = new TLine(0.7,hmin,0.7,hmax);
  line119->SetLineColor(2);
  line119->SetLineWidth(2);
  line119->Draw();

  c1->cd(5); 
  T->Draw("L.tr.ph>>h73","","colz"); 
  h73->SetTitle("LHRS FP ph"); 
  hmin = h73->GetMinimum();
  hmax = h73->GetMaximum();
  TLine *line218 = new TLine(-0.06,hmin,-0.06,hmax);
  line218->SetLineColor(2);
  line218->SetLineWidth(2);
  line218->Draw();
  TLine *line219 = new TLine(0.06,hmin,0.06,hmax);
  line219->SetLineColor(2);
  line219->SetLineWidth(2);
  line219->Draw();

  c1->cd(6); 
  T->Draw("(L.tr.th)>>h74","","colz"); 
  h74->SetTitle("LHRS FP th"); 
  hmin = h74->GetMinimum();
  hmax = h74->GetMaximum();
  TLine *line318 = new TLine(-0.15,hmin,-0.15,hmax);
  line318->SetLineColor(2);
  line318->SetLineWidth(2);
  line318->Draw();
  TLine *line319 = new TLine(0.15,hmin,0.15,hmax);
  line319->SetLineColor(2);
  line319->SetLineWidth(2);
  line319->Draw();

  // ----------------------------------------------------------------------
  // 2D LHRS Focal plane variables 
  // ----------------------------------------------------------------------

  TCanvas *c2 = new TCanvas("c2","LHRS Focal Plane Variables",1000,800); 
  c2->Divide(2,2); 

  TH2F* h701 = new TH2F("h701","h701",200,-15,15,200,-100,100); 
  TH2F* h702 = new TH2F("h702","h702",200,-100,100,200,-150,150); 
  TH2F* h703 = new TH2F("h703","h703",200,-150,150,200,-80,80); 
  TH2F* h704 = new TH2F("h704","h704",200,-50,50,200,-15,15);

  c2->cd(1); 
  T->Draw("L.tr.x*100:L.tr.y*100>>h701",GeneralCut,"colz"); 
  h701->SetTitle("LHRS FP x vs y"); 

  c2->cd(2); 
  T->Draw("(L.tr.th*1000):(L.tr.ph*1000)>>h702",GeneralCut,"colz"); 
  h702->SetTitle("LHRS FP th vs ph"); 

  c2->cd(3); 
  T->Draw("(L.tr.x*100):(L.tr.th*1000)>>h703",GeneralCut,"colz"); 
  h703->SetTitle("LHRS FP x vs th"); 

  c2->cd(4); 
  T->Draw("L.tr.y*100:L.tr.ph*1000>>h704",GeneralCut,"colz"); 
  h704->SetTitle("LHRS FP y vs ph"); 


  // ----------------------------------------------------------------------
  // LHRS Target variables 
  // ----------------------------------------------------------------------

  GeneralCut += ct1+ct2;
  TCanvas *c3 = new TCanvas("c3","LHRS Target Variables",1000,800);
  c3->Divide(2,2);

  c3->cd(1);
  TH1F *h3 = new TH1F("h3","h3",200, -0.1, 0.1);
  T->Draw("L.tr.tg_ph>>h3",GeneralCut);
  h3->SetTitle("LHRS target ph");
  hmax = h3->GetMaximum()*1.05;
  hmin = h3->GetMinimum();
  TLine *line4 = new TLine(phmin,hmin,phmin,hmax);
  line4->SetLineColor(2);
  line4->SetLineWidth(2);
  line4->Draw();
  TLine *line5 = new TLine(phmax,hmin,phmax,hmax);
  line5->SetLineColor(2);
  line5->SetLineWidth(2);
  line5->Draw();

  c3->cd(2);
  TH1F *h4 = new TH1F("h4","h4",200, -0.2, 0.2);
  T->Draw("L.tr.tg_th>>h4",GeneralCut);
  h4->SetTitle("LHRS target th");
  hmax = h4->GetMaximum()*1.05;
  hmin = h4->GetMinimum();
  TLine *line6 = new TLine(thmin,hmin,thmin,hmax);
  line6->SetLineColor(2);
  line6->SetLineWidth(2);
  line6->Draw();
  TLine *line7 = new TLine(thmax,hmin,thmax,hmax);
  line7->SetLineColor(2);
  line7->SetLineWidth(2);
  line7->Draw();

  c3->cd(3);
  TH1F *h5 = new TH1F("h5","h5",200, -0.1, 0.1);
  T->Draw("L.tr.tg_dp>>h5",GeneralCut);
  h5->SetTitle("LHRS target delta");
  hmax = h5->GetMaximum()*1.05;
  hmin = h5->GetMinimum();
  TLine *line8 = new TLine(dpmin,hmin,dpmin,hmax);
  line8->SetLineColor(2);
  line8->SetLineWidth(2);
  line8->Draw();
  TLine *line9 = new TLine(dpmax,hmin,dpmax,hmax);
  line9->SetLineColor(2);
  line9->SetLineWidth(2);
  line9->Draw();

  c3->cd(4);
  TH1F *h63 = new TH1F("h63","h63",200, -0.1, 0.1);
  T->Draw("L.tr.tg_y>>h63",GeneralCut);
  h63->SetTitle("LHRS target y");
  h63->GetXaxis()->SetTitleSize(0.05);
  h63->SetLineColor(4);
  hmax = h63->GetMaximum()*1.05;
  hmin = h63->GetMinimum();
  TLine *line808 = new TLine(ytmin,hmin,ytmin,hmax);
  line808->SetLineColor(2);
  line808->SetLineWidth(2);
  line808->Draw();
  TLine *line909 = new TLine(ytmax,hmin,ytmax,hmax);
  line909->SetLineColor(2);
  line909->SetLineWidth(2);
  line909->Draw();

  // ----------------------------------------------------------------------
  // LHRS Target variables 
  // ----------------------------------------------------------------------

  GeneralCut += ct3+ct4+ct5;
  TCanvas *c4 = new TCanvas("c4","c4",1000,800);
  c4->Divide(2,1);

  TH2F *h21 = new TH2F("h21","h21", 100,-0.04,0.05,100,-0.1,0.1);
  TH2F *h33 = new TH2F("h33","h33", 100,-0.05,0.06,100,-0.07,0.07);

  c4->cd(1);
  T->Draw("L.tr.tg_y:L.tr.tg_ph>>h21",GeneralCut,"colz");
  h21->SetTitle("LHRS y_tgt vs ph_tgt");

  GeneralCut += VertexCut;

  c4->cd(2);
  T->Draw("L.tr.tg_th:L.tr.tg_ph>>h33",GeneralCut,"colz");
  h33->SetTitle("LHRS th_tgt vs ph_tgt");
  
  c1->Print("apex_optics_left1.pdf");
  c2->Print("apex_optics_left2.pdf");
  c3->Print("apex_optics_left3.pdf");
  c4->Print("apex_optics_left4.pdf");

  gSystem->Exec(Form("pdfunite apex_optics_left*.pdf LHRS%d_apex_optics.pdf", run_no)); 
  gSystem->Exec("rm apex_optics_left*.pdf"); 

}



