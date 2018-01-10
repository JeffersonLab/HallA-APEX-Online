// Plot triggers' raw TDC spectrum
// now setup for single arm tritium experiment:
// LHRS trigger 1-3, RHRS trigger 4-6



void ts_all(Int_t run){

  gStyle->SetPalette(kRainBow);

  gStyle->SetOptFit(0); 
  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.06);
  gStyle->SetNdivisions(505);
  // gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadTopMargin(0.11);

  // gStyle->SetPadLeftMargin(0.14);
  // gStyle->SetPadRightMargin(0.08);
  // gStyle->SetPadBottomMargin(.14);
  // gStyle->SetTitleYOffset(0.95);
  // gStyle->SetTitleXOffset(0.75);
  gStyle->SetTitleYSize(0.1);
  gStyle->SetTitleXSize(0.1);
  gStyle->SetLabelFont(62,"X");
  gStyle->SetLabelFont(62,"Y");
  gStyle->SetTitleFont(62,"X");
  gStyle->SetTitleFont(62,"Y");
  gStyle->SetLabelSize(0.055,"X");
  gStyle->SetLabelSize(0.055,"Y");
  gStyle->SetPaperSize(18,22);

  //Int_t run = 361;
  TString dir = "/chafs1/work1/tritium/Rootfiles/";
  TString name =Form("tritium_%d.root",run);
  TString rootfile = dir+name;

  TFile *f = TFile::Open(rootfile);
  TTree * T = (TTree*)f->Get("T");

  Int_t flag=0;
  TString dc = "DL";
  TString arm = "l";
  TString ARM = "L";

  Int_t i = 1;
  Double_t tmin=1350,tmax=1550;
  if (run>90000){
    dc="DR";
    arm = "r";
    ARM="R";
    i = 4;
    tmin=1550;
    tmax=1750;
  }
  TCanvas *c1 = new TCanvas("c1","",400,100,800,1200);
    Int_t bin=200;
 
  cout<<"---Loading file "<<rootfile<<endl;
  cout<<"---Will check triggers' raw TDC values"<<endl;
 
  //if(run<90000)
  //  cout<<"Cut on which trigger?\n0 = no cuts \n1 = Left S0&&s2\n2 = Left (S0&&S2)&&CER\n3 = Left (S0||S2)&&CER\n----Enter here :";
  //else
  // cout<<"Cut on which trigger?\n0 = no cuts \n4 = Right S0&&s2\n5 = Right (S0&&S2)&&CER\n6 = Right (S0||S2)&&CER\n----Enter here: ";
    
  //cin>>flag;
  TCut cut ="";

  c1->Divide(1,4);
 for (int cnt=0;cnt<4;cnt++){

   c1->cd(cnt+1);
   gPad->SetLogy();
   flag=cnt;
   if (run>90000) flag=flag+3;

   if (cnt>0) cut=Form("(%s.evtypebits>>%d)&1",dc.Data(),flag);
    //else if (flag<0 ||flag>8){
    //cout<<"!! invalid trigger cut !!"<<endl;
    //return;
    // }
  TH1F *h1 = new TH1F("h1","S0&&S2",bin,tmin,tmax);
  TH1F *h2 = new TH1F("h2","(S0&&S2)&&CER",bin,tmin,tmax);
  TH1F *h3 = new TH1F("h3","(S0||S2)&&CER",bin,tmin,tmax);
  TCut pr;
  
  TCut cut1 = cut;
  if (run<90000) cut1=cut+Form("%s.prl1.asum_p>400",ARM.Data())+Form("%s.prl2.asum_p>400",ARM.Data());;

  T->Draw(Form("%s.%srawt%d>>h1",dc.Data(),arm.Data(),i),cut1,"");
T->Draw(Form("%s.%srawt%d>>h2",dc.Data(),arm.Data(),i+1),cut1,"");
T->Draw(Form("%s.%srawt%d>>h3",dc.Data(),arm.Data(),i+2),cut1,"");
    
  
   // T->Draw("rhrs.t1>>h1",cut,"");
   // T->Draw("rhrs.t2>>h2",cut,"");
   // T->Draw("rhrs.t3>>h3",cut,"");

  //  T->Project("h1","th.l_raw_T1",cut);
  //T->Project("h2","th.l_raw_T2",cut);
  //T->Project("h3","th.l_raw_T3",cut);

  h3->SetLineColor(kRed);
  h3->Draw("");
  
  h2->SetLineColor(kBlack);
  h2->Draw("same");
  h1->SetLineColor(kBlue);
  h1->Draw("same");

  h3->GetYaxis()->SetRangeUser(0.1,200000);
  

  TLegend *leg = new TLegend(0.7,0.6,0.9,0.9);
  leg->SetTextSize(0.08);
  leg->SetTextFont(60);
  leg->AddEntry(h1,"","l");
  leg->AddEntry(h2,"","l");
  leg->AddEntry(h3,"","l");

  leg->Draw();
  h3->SetTitle(Form("T%d",flag));
  if(cnt==0){
    h3->SetTitle("no cut");
    if(run<90000)     h3->SetTitle("no trigger cuts, only pion rejector cuts");
  }
  TLatex latex;
  latex.SetTextSize(0.1);
  latex.SetTextAlign(13);  //align at top
  if(cnt==0) latex.DrawLatexNDC(0.15,0.8,Form("RUN %d",run));

  latex.DrawLatexNDC(0.15,0.8,cut);
  
 }
 return;

   /*
 TCanvas *c2 = new TCanvas("c2","",200,100,800,600);
 TH2F *h2d = new TH2F("h2d","",bin,tmin,tmax,bin,tmin,tmax);
 
 T->Draw("th.l_raw_T3:th.l_raw_T2>>h2d",cut,"colz");
 h2d->GetXaxis()->SetTitle("T2");
 h2d->GetYaxis()->SetTitle("T3");
 h2d->SetTitle("th.l_raw_T3:th.l_raw_T2"+cut);

 TCut cut2 = cut;// + "th.l_raw_T2<390";
TCanvas *c3 = new TCanvas("c3","",200,100,800,600);
 TH2F *hc3 = new TH2F("hc3","",bin,tmin,tmax,bin,tmin,tmax);
  T->Draw("th.l_raw_T3:th.l_raw_T1>>hc3",cut2,"colz");
 hc3->GetXaxis()->SetTitle("T1");
 hc3->GetYaxis()->SetTitle("T3");
 hc3->SetTitle("th.l_raw_T3:th.l_raw_T1"+cut2);
   */
}
