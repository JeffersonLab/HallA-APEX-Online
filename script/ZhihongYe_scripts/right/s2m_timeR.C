#include <iostream>
#include <iomanip>
using namespace std;
void s2m_timeL(){
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("../ROOTfiles/dvcs_lhrs_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  cout << T->GetEntries() << endl;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, pmin,pmax,temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t time_need = 0;
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[32], val[32];
  TLatex *tex;
  ofstream myfile;
  myfile.open("s2m_time_L.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Time" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Time need" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Offset"  << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);
    TH1F *h1 = new TH1F("h1",Form("Left_S2mL_0%d",i+1),220,-20,2000);
    //    T->Draw(Form("L.s2.lt[%d]-(L.s0.lt+L.s0.rt)/2>>h1",i),Form("L.s2.lt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1&&(L.s0.lt+L.s0.rt)/2>890",i));
    T->Draw(Form("L.s2.lt[%d]-(L.s0.lt+L.s0.rt)/2>>h1",i),Form("L.s2.lt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1",i));
    gPad->SetLogy();
    h1->SetNdivisions(-5);
    max_bin = h1->GetMaximumBin();

    pmin = h1->GetBinCenter(max_bin) - 20;
    pmax = h1->GetBinCenter(max_bin) + 20;
    min = h1->GetBinCenter(max_bin) - 5;
    max = h1->GetBinCenter(max_bin) + 5;
    h1->Delete();
    TH1F *p1 = new TH1F("p1",Form("Left_S2mL_0%d",i+1),220,pmin,pmax);
    // T->Draw(Form("L.s2.lt[%d]-(L.s0.lt+L.s0.rt)/2>>p1",i),Form("L.s2.lt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1&&(L.s0.lt+L.s0.rt)/2>890",i));
    T->Draw(Form("L.s2.lt[%d]-(L.s0.lt+L.s0.rt)/2>>p1",i),Form("L.s2.lt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1",i));
    p1->Fit("gaus","","",min,max);
    val[i] = gaus->GetParameter(1);
    num[i] = i+1;
    cout << "Left " << i << "   " << val[i] << endl;
    myfile << "L" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << val[i] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << time_need << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << time_need - val[i] <<  "   " << endl;
    
  } 

  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4); 
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *h2 = new TH1F("h2",Form("Left_S2mR_0%d",i+1),220,-20,2000);
    //   T->Draw(Form("L.s2.rt[%d]-(L.s0.lt+L.s0.rt)/2>>h2",i),Form("L.s2.rt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1&&(L.s0.lt+L.s0.rt)/2>890",i));
    T->Draw(Form("L.s2.rt[%d]-(L.s0.lt+L.s0.rt)/2>>h2",i),Form("L.s2.rt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1",i));
    gPad->SetLogy();
    h2->SetNdivisions(-5);
    max_bin = h2->GetMaximumBin();

    pmin = h2->GetBinCenter(max_bin) - 20;
    pmax = h2->GetBinCenter(max_bin) + 20;
    min = h2->GetBinCenter(max_bin) - 5;
    max = h2->GetBinCenter(max_bin) + 5;
    
    h2->Delete();
    TH1F *p2 = new TH1F("p2",Form("Left_S2mL_0%d",i+1),220,pmin,pmax);
    //   T->Draw(Form("L.s2.rt[%d]-(L.s0.lt+L.s0.rt)/2>>p2",i),Form("L.s2.rt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1&&(L.s0.lt+L.s0.rt)/2>890",i));  
    T->Draw(Form("L.s2.rt[%d]-(L.s0.lt+L.s0.rt)/2>>p2",i),Form("L.s2.rt[%d]&&L.s0.nlthit==1&&L.s0.nrthit==1",i));
    p2->Fit("gaus","","",min,max);
    
    val[i+16] = gaus->GetParameter(1);
    num[i+16] = i+1;
    cout << "Right " << i << "   " << val[i] << endl;
    myfile << "R" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << val[i+16] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << time_need << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << time_need - val[i+16] <<  "   " << endl;
    
  } 
  myfile << endl;
  myfile.close();
  
  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","Left HRS S2m time",17,0,17,50,80,130);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Time (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->GetYaxis()->SetTitleOffset(1.2);
  hh->Draw();
  TGraph *gr = new TGraph(32,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}

