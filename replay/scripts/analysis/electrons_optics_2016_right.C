void electrons_optics_2016_right(){
  //code to check optics run with new sieve,
  //e_beam =2.301, p0 = 0.75, HRS angle = 48.74

  //*********************************************//
  Int_t sh_min=700; // the limit for (R.ps.e+sh_e*R.sh.e)
  Double_t sh_e = 0.9; // for (R.ps.e+sh_e*R.sh.e)
  Int_t ps_min=200;
  Int_t cer_min=500; // the limit for Cherenkov
  Double_t phmin = -0.04, phmax=0.05; // the limits for tg_ph
  Double_t thmin= -0.06,thmax=0.07;  // the limits for tg_th
  Double_t dpmin = -0.05, dpmax=0.06; // the limits for tg_ph
  Int_t hmax,hmin;

  //  Double_t zmin= -0.06,zmax=0.06;  // the limits for R.tr.vz

  //choose trigger = 1(s0&s2m) and get rid of EDTM events
  //  TCut cut_trigger="(DR.evtypebits&1<<1)==1<<1 && (DR.evtypebits&1<<7)!=1<<7";  
  TCut cut_nclust = "R.vdc.u1.nclust==1&&R.vdc.v1.nclust==1&&R.vdc.u2.nclust==1&&R.vdc.u1.nclust==1";
  TCut cut_ntrack = "R.tr.n==1";
  TCut GeneralCut = cut_nclust + cut_ntrack;
  TCut ct1 = Form("(R.ps.e+%.2f*R.sh.e)>%d&&R.ps.e>%d",sh_e,sh_min,ps_min);
  //  TCut ct1 = Form("(R.ps.e+%.2f*R.sh.e)>%d",sh_e,sh_min);
  TCut ct2 = Form("R.cer.asum_c>%d",cer_min);
  TCut ct3 = Form("R.tr.tg_ph<%.2f&&R.tr.tg_ph>%.2f",phmax,phmin);
  TCut ct4 = Form("R.tr.tg_th<%.2f&&R.tr.tg_th>%.2f",thmax,thmin);
  TCut ct5 = Form("R.tr.tg_dp<%.2f&&R.tr.tg_dp>%.2f",dpmax,dpmin);

  TCut foi1 = "R.tr.vz<-0.1&R.tr.vz>-0.12";
  TCut foi2 = "R.tr.vz<-0.075&R.tr.vz>-0.1";
  TCut foi3 = "R.tr.vz<-0.055&&R.tr.vz>-0.075";
  TCut foi4 = "R.tr.vz<-0.02&&R.tr.vz>-0.055";
  TCut foi5 = "R.tr.vz<0&&R.tr.vz>-0.02";
  TCut foi6 = "R.tr.vz<0.025&&R.tr.vz>0";
  TCut foi7 = "R.tr.vz<0.055&&R.tr.vz>0.025";
  TCut foi8 = "R.tr.vz<0.08&&R.tr.vz>0.055";
  TCut foi9 = "R.tr.vz<0.1&&R.tr.vz>0.08";

//*********************************************//


  TChain *t = new TChain("T");

  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22828*.root");
  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22829*.root");
  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22830*.root");
  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22831*.root");
  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22832*.root");
  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22833*.root");
  t->Add("/adaqfs/home/a-onl/gmp/replay/Rootfiles/right_gmp_22834*.root");

  //general cut
  TCanvas *c1 = new TCanvas("c1","c1",1000,800);
  c1->Divide(3,2);

  c1->cd(1);
  TH2F *h1 = new TH2F("h1","h1",500,0,1000,500,0,1000);
  T->Draw("R.ps.e:R.sh.e>>h1",GeneralCut,"colz");
  assert(h1);
  h1->SetTitle(Form("cut1: (R.ps.e+%.2f*R.sh.e)>%d",sh_e,sh_min));
  h1->GetYaxis()->SetTitleOffset(1.0);
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetTitle("R.sh.e");
  h1->GetYaxis()->SetTitle("R.ps.e");
  TLine *line1 = new TLine(0,sh_min,sh_min/sh_e,0);
  line1->SetLineColor(2);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine *line2 = new TLine(0,ps_min,1000,ps_min);
  line2->SetLineColor(2);
  line2->SetLineWidth(2);
  line2->Draw();

  c1->cd(2)->SetLogy(1);
  TH1F *h2 = new TH1F("h2","h2",500,0,2500);
  T->Draw("R.cer.asum_c>>h2",GeneralCut&&ct1);
  h2->SetTitle(Form("cut2: R.cer.asum_c>%d",cer_min));
  h2->GetXaxis()->SetTitle("R.cer.asum_c");
  h2->SetTitleSize(0.05,"X");
  hmin = h2->GetMinimum();
  hmax = h2->GetMaximum();
  TLine *line3 = new TLine(cer_min,hmin,cer_min,hmax);
  line3->SetLineColor(2);
  line3->SetLineWidth(2);
  line3->Draw();

  c1->cd(3);
  TH1F *h3 = new TH1F("h3","h3",100, -0.1, 0.1);
  T->Draw("R.tr.tg_ph>>h3",GeneralCut&&ct1&&ct2);
  h3->SetTitle("R.tr.tg_ph");
  h3->SetTitle(Form("cut3: %.2f<tg_ph<%.2f",phmin,phmax));
  h3->GetXaxis()->SetTitle("R.tr.tg_ph(radian)");
  h3->GetXaxis()->SetTitleSize(0.05);
  hmax = h3->GetMaximum()*1.05;
  hmin = h3->GetMinimum();
  TLine *line1 = new TLine(phmin,hmin,phmin,hmax);
  line1->SetLineColor(2);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine *line2 = new TLine(phmax,hmin,phmax,hmax);
  line2->SetLineColor(2);
  line2->SetLineWidth(2);
  line2->Draw();

  c1->cd(4);
  TH1F *h4 = new TH1F("h4","h4",100, -0.2, 0.2);
  T->Draw("R.tr.tg_th>>h4",GeneralCut&&ct1&&ct2);
  h4->SetTitle(Form("cut4: %.2f<tg_th<%.2f",thmin,thmax));
  h4->GetXaxis()->SetTitle("R.tr.tg_th(radian)");
  h4->GetXaxis()->SetTitleSize(0.05);
  hmax = h4->GetMaximum()*1.05;
  hmin = h4->GetMinimum();
  TLine *line1 = new TLine(thmin,hmin,thmin,hmax);
  line1->SetLineColor(2);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine *line2 = new TLine(thmax,hmin,thmax,hmax);
  line2->SetLineColor(2);
  line2->SetLineWidth(2);
  line2->Draw();

  c1->cd(5);
  TH1F *h5 = new TH1F("h5","h5",100, -0.1, 0.1);
  T->Draw("R.tr.tg_dp>>h5",GeneralCut&&ct1&&ct2);
  h5->SetTitle("R.tr.tg_dp");
  h5->SetTitle(Form("cut5: %.2f<tg_dp<%.2f",dpmin,dpmax));
  h5->GetXaxis()->SetTitle("R.tr.tg_dp(radian)");
  h5->GetXaxis()->SetTitleSize(0.05);
  hmax = h5->GetMaximum()*1.05;
  hmin = h5->GetMinimum();
  TLine *line1 = new TLine(dpmin,hmin,dpmin,hmax);
  line1->SetLineColor(2);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine *line2 = new TLine(dpmax,hmin,dpmax,hmax);
  line2->SetLineColor(2);
  line2->SetLineWidth(2);
  line2->Draw();


  c1->cd(6);
  GeneralCut += ct1 +ct2 +ct3+ct4 +ct5; 
  TH1F *h61 = new TH1F("h61","h61",120, -0.12, 0.12);
  TH1F *h62 = new TH1F("h62","h62",120, -0.12, 0.12);
  TH1F *h63 = new TH1F("h63","h63",120, -0.12, 0.12);
  TH1F *h64 = new TH1F("h64","h64",120, -0.12, 0.12);
  TH1F *h65 = new TH1F("h65","h65",120, -0.12, 0.12);
  TH1F *h66 = new TH1F("h66","h66",120, -0.12, 0.12);
  TH1F *h67 = new TH1F("h67","h67",120, -0.12, 0.12);
  TH1F *h68 = new TH1F("h68","h68",120, -0.12, 0.12);
  TH1F *h69 = new TH1F("h69","h69",120, -0.12, 0.12);
  T->Draw("R.tr.vz>>h63",GeneralCut&&foi3);
  h63->SetTitle("R.tr.vz");
  h63->GetXaxis()->SetTitle("R.tr.vz(m)");
  h63->GetXaxis()->SetTitleSize(0.05);
  h63->SetLineColor(4);

  T->Draw("R.tr.vz>>h64",GeneralCut&&foi4,"");
  h64->SetLineColor(6);
  T->Draw("R.tr.vz>>h61",GeneralCut&&foi1,"same");
  h61->SetLineColor(8);
  T->Draw("R.tr.vz>>h62",GeneralCut&&foi2,"same");
  h62->SetLineColor(8);
  T->Draw("R.tr.vz>>h63",GeneralCut&&foi3,"same");
  h63->SetLineColor(15);
  T->Draw("R.tr.vz>>h65",GeneralCut&&foi5,"same");
  h65->SetLineColor(13);
  T->Draw("R.tr.vz>>h66",GeneralCut&&foi6,"same");
  h66->SetLineColor(2);
  T->Draw("R.tr.vz>>h67",GeneralCut&&foi7,"same");
  h67->SetLineColor(1);
  h67->SetLineStyle(2);
  T->Draw("R.tr.vz>>h68",GeneralCut&&foi8,"same");
  h68->SetLineColor(2);
  h68->SetLineStyle(2);
  T->Draw("R.tr.vz>>h69",GeneralCut&&foi9,"same");
  h69->SetLineColor(4);
  h69->SetLineStyle(2);




  TCanvas *c2 = new TCanvas("c2","c2",1000,800);
  T->Draw("R.tr.vz:R.tr.tg_ph>>h21(500,-0.04,0.05,600,-0.12,0.12)",GeneralCut,"colz");
  h21->SetTitle("R.tr.vz:R.tr.ph");

  TCanvas *c3 = new TCanvas("c3","c3",1000,800);
  c3->Divide(3,3);

  c3->cd(1);
  Int_t n1=0,n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0,n9=0,nn=0;

  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h31(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi1,"colz");
  h31->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n1=h31->GetEntries();

  c3->cd(2);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h32(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi2,"colz");
  h32->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n2=h32->GetEntries();

  c3->cd(3);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h33(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi3,"colz");
  h33->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n3=h33->GetEntries();

  c3->cd(4);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h34(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi4,"colz");
  h34->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n4=h34->GetEntries();

  c3->cd(5);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h35(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi5,"colz");
  h35->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n5=h35->GetEntries();

  c3->cd(6);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h36(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi6,"colz");
  h36->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n6=h36->GetEntries();


  c3->cd(7);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h37(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi7,"colz");
  h37->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n7=h37->GetEntries();

  c3->cd(8);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h38(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi8,"colz");
  h38->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n8=h38->GetEntries();

  c3->cd(9);
  T->Draw("R.tr.tg_th:R.tr.tg_ph>>h39(500,-0.05,0.06,500,-0.07,0.07)",GeneralCut&&foi9,"colz");
  h39->SetTitle("R.tr.tg_th : R.tr.tg_ph");
  n9=h39->GetEntries();

  TCanvas *cnumbers = new TCanvas("Numbers", "Numbers");
  cnumbers->cd();
  TPaveText *t0 = new TPaveText(0.2,0.1,0.8,0.8,"NDC");
  t0->AddText("Electron numbers:");
  t0->AddText(Form("Total = %d",n1+n2+n3+n4+n5+n6+n7+n8+n9));
  t0->AddText(Form("Foil#1 = %d",n1));
  t0->AddText(Form("Foil#2 = %d",n2));
  t0->AddText(Form("Foil#3 = %d",n3));
  t0->AddText(Form("Foil#4 = %d",n4));
  t0->AddText(Form("Foil#5 = %d",n5));
  t0->AddText(Form("Foil#6 = %d",n6));
  t0->AddText(Form("Foil#7 = %d",n7));
  t0->AddText(Form("Foil#8 = %d",n8));
  t0->AddText(Form("Foil#9 = %d",n9));
  t0->Draw(); 




  c1->Print("optics_electrons.pdf(");
  c2->Print("optics_electrons.pdf");
  c3->Print("optics_electrons.pdf)");
}



