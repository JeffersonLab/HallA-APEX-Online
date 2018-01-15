#include <iostream>
#include <iomanip>
using namespace std;
void s2_timeL(){
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("../Rootfiles/e07006_det_L_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  cout << T->GetEntries() << endl;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, pmin,pmax,temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t time_need = 0;
  Double_t timeL[16] = {708.32, 647.29, 592.85, 593.04, 517.08, 515.84, 624.01, 606.75, 
		       611.64, 590.49, 513.47, 419.92, 660.55, 590.72, 546.92, 547.46};
  Double_t timeL[16] = {614.68, 434.49, 475.95, 502.44, 687.52, 713.80, 739.55, 691.01, 
		       733.62, 613.11, 707.49, 830.48, 695.09, 621.60, 732.00, 723.42};
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[32], val[32];
  TLatex *tex;
  ofstream myfile;
  myfile.open("L.s2_time.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Time" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Time need" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Offset"  << endl;
  
   TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);
    TH1F *h1 = new TH1F("h1",Form("Left_SsL_0%d",i+1),220,-200,200);
    T->Draw(Form("L.s2.lt[%d]-(L.s1.lt[3]+L.s1.rt[3])/2.>>h1",i),Form("abs(L.s2.lt[%d]-L.s2.rt[%d])<200.&&L.s2.lt[%d]>0.&&L.s2.rt[%d]>0.&&L.s1.lt[3]>0.&&L.s1.rt[3]>0.",i));

    gPad->SetLogy();
    h1->SetNdivisions(-5);
    max_bin = h1->GetMaximumBin();
    pmin = h1->GetBinCenter(max_bin) - 50;
    pmax = h1->GetBinCenter(max_bin) + 50;
    min = h1->GetBinCenter(max_bin) - 15;
    max = h1->GetBinCenter(max_bin) + 15;
    h1->Delete();
    TH1F *p1 = new TH1F("p1",Form("Left_S2L_0%d",i+1),220,pmin,pmax);
    T->Draw(Form("L.s2.lt[%d]-(L.s1.lt[3]+L.s1.rt[3])/2.>>p1",i),Form("abs(L.s2.lt[%d]-L.s2.rt[%d])<200.&&L.s2.lt[%d]>0.&&L.s2.rt[%d]>0.&&L.s1.lt[3]>0.&&L.s1.rt[3]>0.",i));
    
    p1->Fit("gaus","","",min,max);
    val[i] = gaus->GetParameter(1);
    num[i] = i+1;
    cout << "Left " << i << "   " << val[i] << endl;
    myfile << "L" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << val[i] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << timeL[i] << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << timeL[i] - val[i] <<  "   " << endl;
    
  } 

  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4); 
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *h2 = new TH1F("h2",Form("Left_S2R_0%d",i+1),220,-200,200);
    T->Draw(Form("L.s2.rt[%d]-(L.s1.lt[3]+L.s1.rt[3])/2.>>h2",i),Form("abs(L.s2.lt[%d]-L.s2.rt[%d])<200.&&L.s2.lt[%d]>0.&&L.s2.rt[%d]>0.&&L.s1.lt[3]>0.&&L.s1.rt[3]>0.",i));

    //    T->Draw(Form("L.s2.rt[%d]-(L.s2.lt[7]+L.s2.rt[7])/2.>>h2",i),Form("abs(L.s2.lt[%d]-L.s2.rt[%d])<200.&&L.s2.lt[7]>0.&&L.s2.rt[7]>0.&&(L.s2.lt[%d]>0.&&L.s2.rt[%d]>0.)&&L.s2.nlthit[7]==1&&L.s2.nrthit[7]==1",i));
    T->Draw(Form("L.s2.rt[%d]-(L.s2.lt[7]+L.s2.rt[7])/2.>>h2",i),Form("abs(L.s2.lt[0]-L.s2.rt[0])<200.&&L.s2.lt[0]>0.&&L.s2.rt[%d]>0.&&L.s2.lt[7]>0.&&L.s2.rt[7]>0.",i));
    gPad->SetLogy();
    h2->SetNdivisions(-5);
    max_bin = h2->GetMaximumBin();
    pmin = h2->GetBinCenter(max_bin) - 100;
    pmax = h2->GetBinCenter(max_bin) + 100;
    min = h2->GetBinCenter(max_bin) - 50;
    max = h2->GetBinCenter(max_bin) + 50;
    h2->Delete();
    TH1F *p2 = new TH1F("p2",Form("Left_S2R_0%d",i+1),220,pmin,pmax);
    T->Draw(Form("L.s2.rt[%d]-(L.s1.lt[3]+L.s1.rt[3])/2.>>p2",i),Form("abs(L.s2.lt[%d]-L.s2.rt[%d])<200.&&L.s2.lt[%d]>0.&&L.s2.rt[%d]>0.&&L.s1.lt[3]>0.&&L.s1.rt[3]>0.",i));    
    
    p2->Fit("gaus","","",min,max);
    
    val[i+6] = gaus->GetParameter(1);
    num[i+6] = i+1;
    cout << "Right " << i << "   " << val[i+6] << endl;
    myfile << "R" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << val[i+6] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << timeR[i] << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << timeR[i] - val[i+6] <<  "   " << endl;
    
  } 
  myfile << endl;
  myfile.close();
  

  for(int j=0;j<16;j++){
    cerr <<timeL[j] - val[j]<<" ";
  }
  cerr <<endl;
  
  for(int j=0;j<16;j++){
    cerr <<timeR[j] - val[j+16]<<" ";
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

