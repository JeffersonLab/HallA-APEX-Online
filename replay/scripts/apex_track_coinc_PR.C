// -------------------
// plot the rawtime vs wire number per VDC plane per event for LHRS
// shujie, 2019.3.7
// -------------------

void apex_track_coinc_PR(Int_t runnum,  Int_t fcoinc=1, Int_t entry=1){


  gStyle->SetTitleYSize(0.06);
  gStyle->SetTitleXSize(0.06);
  gStyle->SetLabelSize(0.06,"X");
  gStyle->SetLabelSize(0.06,"Y");
  gStyle->SetPaperSize(18,22);
  gStyle->SetOptStat(0);

  // only load the first split of rootfiles
  TChain *t = new TChain("T");
  TString rootfile = Form("../rootfiles/apex_online_%d.root",runnum);
  t->Add(rootfile.Data());
  if(!t){
   cout<< "can't find rootfile for run "<<runnum<<endl;
   exit(0);
   }
  Int_t nn=t->GetEntries();
  cout<<"=="<<rootfile<<" loaded, "<<nn<<" events in total==\n" <<endl;


  TCanvas *c1  = new TCanvas("c1","rawtime : wire number(cut on coinc peak)",1200,900);
  TH2F    *h1  = new TH2F("h1","u1 rawtime : wire number",330,10,350,200,2000,3200);
  TH2F    *h2  = new TH2F("h2","v1 rawtime : wire number",330,10,350,200,2000,3200);
  TH2F    *h3  = new TH2F("h3","u2 rawtime : wire number",330,10,350,200,2000,3200);
  TH2F    *h4  = new TH2F("h4","v2 rawtime : wire number",330,10,350,200,2000,3200);
  TH1F    *hl0  = new TH1F("hl0","PR amplitude : PR x location",17,1,17);
  TH1F    *hl1  = new TH1F("hl1","S2 amplitude : S2 x location",16,1,16);

  c1->Divide(1,6);
  //--------------------------------
  t->SetBranchStatus("*"       ,0);
  t->SetBranchStatus("L.vdc.*" ,1);
  t->SetBranchStatus("L.prl1.*",1);
  t->SetBranchStatus("L.prl2.*",1);
  t->SetBranchStatus("L.tr.n"  ,1);
  t->SetBranchStatus("L.s2.*"  ,1);
  t->SetBranchStatus("DR.rrawt2"  ,1);
 // t->SetBranchStatus("DR."  ,1);
  
  Double_t    nclust1,nclust2,nclust3,nclust4, ntrk;
  Double_t    clbeg1[40], clend1[40];
  Double_t    clbeg2[40], clend2[40];
  Double_t    clbeg3[40], clend3[40];
  Double_t    clbeg4[40], clend4[40];
  
  Double_t    L_prl1_a_p[34];
  Double_t    L_prl2_a_p[34];
  Double_t    L_fLA_p[16];
  Double_t    L_fRA_p[16];

  t->SetBranchAddress("L.tr.n",&ntrk);
  t->SetBranchAddress("L.vdc.u1.nclust",&nclust1);
  t->SetBranchAddress("L.vdc.v1.nclust",&nclust2);
  t->SetBranchAddress("L.vdc.u2.nclust",&nclust3);
  t->SetBranchAddress("L.vdc.v2.nclust",&nclust4);

  t->SetBranchAddress("L.vdc.u1.clbeg", clbeg1);
  t->SetBranchAddress("L.vdc.v1.clbeg", clbeg2);
  t->SetBranchAddress("L.vdc.u2.clbeg", clbeg3);
  t->SetBranchAddress("L.vdc.v2.clbeg", clbeg4);

  t->SetBranchAddress("L.vdc.u1.clend", clend1);
  t->SetBranchAddress("L.vdc.v1.clend", clend2);
  t->SetBranchAddress("L.vdc.u2.clend", clend3);
  t->SetBranchAddress("L.vdc.v2.clend", clend4);
  t->SetBranchAddress("L.prl1.a_p", L_prl1_a_p);
  t->SetBranchAddress("L.prl2.a_p", L_prl2_a_p);

  t->SetBranchAddress("L.s2.la_p", L_fLA_p);
  t->SetBranchAddress("L.s2.ra_p", L_fRA_p);

  // select list of events at coinc peak
  
 TCut coinc_cut = "DR.rrawt2>1000&&DR.rrawt2<2000";
 //if(fconic){
    t->Draw(">>coinc_list",coinc_cut);
    TEventList *coinc_list = (TEventList*)gDirectory->Get("coinc_list");
    t->SetEventList(0);
    t->SetEventList(coinc_list);
 
  Int_t nelist = coinc_list->GetN();

  TLatex tex;
  tex.SetTextSize(0.052);
  tex.SetTextAlign(11);

  while(entry>0){

    if(!coinc_list->Contains(entry)){
	entry++;
	continue;

    }

    t->GetEntry(entry);    
    TCut cut  = Form("Entry$==%d",entry);

    cout<<"\n--now plot VDC rawtime vs wire number from event "<<entry<<endl;

    c1->cd(1);t->Draw("L.vdc.u1.rawtime:L.vdc.u1.wire>>h1",cut,"box");
   
    tex.SetTextColor(kRed);
    tex.DrawLatexNDC(0.12,0.85,Form("Run %d, event #%d",runnum,entry));
    // cout<<Form("%g track, %g clusters found:",ntrk, nclust1)<<endl;
    tex.DrawLatexNDC(0.12,0.80,Form("%g track, %g clusters found:",ntrk, nclust1));
    tex.SetTextColor(kBlack);

    for(int ii=0;ii<nclust1;ii++){
      tex.DrawLatexNDC(0.12,0.75-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg1[ii],clend1[ii]));
    }
      TLine *l1 = new TLine(20, 2900, 350, 2850);
      l1->SetLineColor(2);
      l1->Draw("Same");
//---------------------
    c1->cd(2);
    t->Draw("L.vdc.v1.rawtime:L.vdc.v1.wire>>h2",cut,"box");
    for(int ii=0;ii<nclust2;ii++){
      tex.DrawLatexNDC(0.12,0.85-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg2[ii],clend2[ii]));
    }
      TLine *l2 = new TLine(20, 2900, 350, 2900);
      l2->SetLineColor(2);
      l2->Draw("Same");
//---------------------
    c1->cd(3);
    t->Draw("L.vdc.u2.rawtime:L.vdc.u2.wire>>h3",cut,"box");
    for(int ii=0;ii<nclust3;ii++){
      tex.DrawLatexNDC(0.12,0.85-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg3[ii],clend3[ii]));
    }
      TLine *l3 = new TLine(20, 2900, 350, 2900);
      l3->SetLineColor(2);
      l3->Draw("Same");
//--------------------
    c1->cd(4);
    t->Draw("L.vdc.v2.rawtime:L.vdc.v2.wire>>h4",cut,"box");
    for(int ii=0;ii<nclust4;ii++){
      tex.DrawLatexNDC(0.12,0.85-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg4[ii],clend4[ii]));
    }
      TLine *l4 = new TLine(20, 2900, 350, 2900);
      l4->SetLineColor(2);
      l4->Draw("Same");
 //------------------
 // PR-Amplitude plot 

   c1->cd(5);
int end=16;
double val[17];
double value=0;
for(int i=0;i<end+1;i=i+1){
value= L_prl2_a_p[i]+L_prl1_a_p[i]+ L_prl2_a_p[i+17]+L_prl1_a_p[i+17];
if (value>0) {val[i]= value;}
else {val[i]=0;}
hl0->SetBinContent(i+1,val[i]);

}
 hl0->SetMaximum(10000);
 hl0->SetMinimum(0);
 hl0->Draw();


 //------------------
 // PR-Amplitude plot 

   c1->cd(6);
int endS=15;
double valS[16];
double valueS=0;
for(int i=0;i<endS+1;i=i+1){
valueS= L_fLA_p[i] + L_fRA_p[i];
if (valueS>0) {valS[i]= valueS;}
else {valS[i]=0;}
hl1->SetBinContent(16-i+1,valS[i]);

}
 hl1->SetMaximum(1500);
 hl1->SetMinimum(0);
 hl1->Draw();


 //------------------
 
   // c1->cd(1);h1->Draw("box");

    // c1->cd(2);h2->Draw("box");
    // c1->cd(3);h3->Draw("box");
    // c1->cd(4);h4->Draw("box");
    // break;
    int index;
    c1->Update();
    cout << "\n--Which event you want to check next ?\n(Please enter an event number. ENTER = next event, -1 to exit) ";
    if(cin.peek() == '\n'){
      entry++;
      cin.ignore();
     }
    else{
      cin >> index;
      cin.ignore();
      
      if (index==-1) break;
      else if (index>0 && index<nn){
        entry = index;
      }
      else{
        cout<<"!! Please enter a valid event number !!";
        continue;
      }
    }

}   
  
}
