#include <iostream>
#include <iomanip>
using namespace std;
void s1_timeL(){
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("../Rootfiles/e07006_HRS_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  T->Add( Form("../Rootfiles/e07006_HRS_%d_1.root",run_number) );
  T->Add( Form("../Rootfiles/e07006_HRS_%d_2.root",run_number) );
  T->Add( Form("../Rootfiles/e07006_HRS_%d_3.root",run_number) );

  cout << T->GetEntries() << endl;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, pmin,pmax,temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t time_need = 0;
  //Double_t timeL[6] = {386.58, 195.12, 37.16, -453.06, 249.23, 290.26};
  //Double_t timeR[6] = {276.20, 298.86, 462.14, 459.74, 485.07, 537.38};
  TLatex *tex;
  
  TCut time_cut = "DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0";
  
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[12], val[12];
  TLatex *tex;
  ofstream myfile;
  myfile.open("s1_time_L.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(8) << "Offset"  << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(2,3); 
  for(i=0;i<6;i++){
    c1->cd(i+1);
    TH1F *h1 = new TH1F("h1",Form("Left_S1L_0%d",i+1),220,-200,200);
    T->Draw(Form("L.s1.lt[%d]-(L.s2.lt[7]+L.s2.rt[7])/2.>>h1",i),Form("abs(L.s1.lt[%d]-L.s1.rt[%d])<200.&&L.s1.lt[%d]>0.&&L.s1.rt[%d]>0.&&L.s2.lt[7]>0.&&L.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));

    gPad->SetLogy();
    h1->SetNdivisions(-5);
    max_bin = h1->GetMaximumBin();
    pmin = h1->GetBinCenter(max_bin) - 50;
    pmax = h1->GetBinCenter(max_bin) + 50;
    min = h1->GetBinCenter(max_bin) - 15;
    max = h1->GetBinCenter(max_bin) + 15;
    h1->Delete();
    TH1F *p1 = new TH1F("p1",Form("Left_S1L_0%d",i+1),220,pmin,pmax);
    T->Draw(Form("L.s1.lt[%d]-(L.s2.lt[7]+L.s2.rt[7])/2.>>p1",i),Form("abs(L.s1.lt[%d]-L.s1.rt[%d])<200.&&L.s1.lt[%d]>0.&&L.s1.rt[%d]>0.&&L.s2.lt[7]>0.&&L.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
    p1->Fit("gaus","","",min,max);
    val[i] = gaus->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Time Offset - %3.2lf",val[i]));
    tex->SetNDC();
    tex->Draw();
    num[i] = i+1;
    cout << "Left " << i << "   " << val[i] << endl;
    myfile << "L" << i << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << val[i] <<  "   " << endl;
    
  } 

  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(2,3); 
  for(i=0;i<6;i++){
    c2->cd(i+1);
    TH1F *h2 = new TH1F("h2",Form("Left_S1R_0%d",i+1),220,-200,200);
    T->Draw(Form("L.s1.rt[%d]-(L.s2.lt[7]+L.s2.rt[7])/2.>>h2",i),Form("abs(L.s1.lt[%d]-L.s1.rt[%d])<200.&&L.s1.lt[%d]>0.&&L.s1.rt[%d]>0.&&L.s2.lt[7]>0.&&L.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
    gPad->SetLogy();
    h2->SetNdivisions(-5);
    max_bin = h2->GetMaximumBin();
    pmin = h2->GetBinCenter(max_bin) - 100;
    pmax = h2->GetBinCenter(max_bin) + 100;
    min = h2->GetBinCenter(max_bin) - 50;
    max = h2->GetBinCenter(max_bin) + 50;
    h2->Delete();
    TH1F *p2 = new TH1F("p2",Form("Left_S1R_0%d",i+1),220,pmin,pmax);
    T->Draw(Form("L.s1.rt[%d]-(L.s2.lt[7]+L.s2.rt[7])/2.>>p2",i),Form("abs(L.s1.lt[%d]-L.s1.rt[%d])<200.&&L.s1.lt[%d]>0.&&L.s1.rt[%d]>0.&&L.s2.lt[7]>0.&&L.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
    
    p2->Fit("gaus","","",min,max);
    
    val[i+6] = gaus->GetParameter(1);

    tex = new TLatex(0.3,0.9,Form("Time Offset - %3.2lf",val[i+6]));
    tex->SetNDC();
    tex->Draw();

    num[i+6] = i+1;
    cout << "Right " << i << "   " << val[i+6] << endl;
    myfile << "R" << i << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << val[i+6] <<  "   " << endl;
    
  } 
  myfile << endl;
  myfile.close();
  

  for(int j=0;j<6;j++){
    cerr <val[j]<<" ";
  }
  cerr <<endl;
  
  for(int j=0;j<6;j++){
    cerr <<val[j+6]<<" ";
  }
  cerr <<endl;


//   TCanvas *c3 = new TCanvas("c3","c3");
//   TH2F *hh = new TH2F("hh","Left HRS S2m time",17,0,17,50,80,130);
//   hh->GetXaxis()->SetTitle("PMT number");
//   hh->GetXaxis()->CenterTitle();
//   hh->GetYaxis()->SetTitle("Time (ch)");
//   hh->GetYaxis()->CenterTitle();
//   hh->GetYaxis()->SetTitleOffset(1.2);
//   hh->Draw();
//   TGraph *gr = new TGraph(12,num,val);
//   gr->SetMarkerStyle(20);
//   gr->Draw("P");
}



