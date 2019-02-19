#include <iostream>
#include <iomanip>
using namespace std;
void s1_timeR(){
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("../Rootfiles/e07006_HRS_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  T->Add( Form("../Rootfiles/e07006_HRS_%d_1.root",run_number));
  T->Add( Form("../Rootfiles/e07006_HRS_%d_2.root",run_number));
  T->Add( Form("../Rootfiles/e07006_HRS_%d_3.root",run_number));
  T->Add( Form("../Rootfiles/e07006_HRS_%d_4.root",run_number));
  T->Add( Form("../Rootfiles/e07006_HRS_%d_5.root",run_number));
  T->Add( Form("../Rootfiles/e07006_HRS_%d_6.root",run_number));

  cout << T->GetEntries() << endl;
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);
  Double_t min, max, pmin,pmax,temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t time_need = 0;
  Double_t timeL[6] = { -115.12,  8.61, -154.65, -146.52,  54.11, -222.65};
  Double_t timeR[6] = {   81.15, 91.87,  167.13,  146.52, 142.44,  149.75};
 
  TCut time_cut = "DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0";
  
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[12], val[12];
  TLatex *tex;
  ofstream myfile;
  myfile.open("s1_time_R.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Time" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Time need" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Offset"  << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(2,3); 
  for(i=0;i<6;i++){
    c1->cd(i+1);
    TH1F *h1 = new TH1F("h1",Form("Right_S1L_0%d",i+1),220,-200,200);
    //    T->Draw(Form("R.s1.lt[%d]-(R.s2.lt[7]+R.s2.rt[7])/2.>>h1",i),Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>0.&&R.s1.rt[%d]>0.&&R.s2.lt[7]>0.&&R.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
  T->Draw(Form("R.s1.lt[%d]-(R.s0.lt+R.s0.rt)/2.>>h1",i),
	  Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>20.&&R.s1.rt[%d]>20.&&R.s0.lt>0.&&R.s0.rt>0.&&DBB.evtypebits&(1<<2)&&DBB.edtpr[0]==0",
	       i,i,i,i));

    gPad->SetLogy();
    h1->SetNdivisions(-5);
    max_bin = h1->GetMaximumBin();
    pmin = h1->GetBinCenter(max_bin) - 50;
    pmax = h1->GetBinCenter(max_bin) + 50;
    min = h1->GetBinCenter(max_bin) - 15;
    max = h1->GetBinCenter(max_bin) + 15;
    h1->Delete();
    TH1F *p1 = new TH1F("p1",Form("Right_S1L_0%d",i+1),100,pmin,pmax);
    //    T->Draw(Form("R.s1.lt[%d]-(R.s2.lt[7]+R.s2.rt[7])/2.>>p1",i),Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>0.&&R.s1.rt[%d]>0.&&R.s2.lt[7]>0.&&R.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
  T->Draw(Form("R.s1.lt[%d]-(R.s0.lt+R.s0.rt)/2.>>p1",i),
	  Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>20.&&R.s1.rt[%d]>20.&&R.s0.lt>0.&&R.s0.rt>0.&&DBB.evtypebits&(1<<2)&&DBB.edtpr[0]==0",
	       i,i,i,i));
  if(i==0)
    p1->Fit("gaus","","",min+5,max-5);
  else
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
  c2->Divide(2,3); 
  for(i=0;i<6;i++){
    c2->cd(i+1);
    TH1F *h2 = new TH1F("h2",Form("Right_S1R_0%d",i+1),220,-200,200);
    //    T->Draw(Form("R.s1.rt[%d]-(R.s2.lt[7]+R.s2.rt[7])/2.>>h2",i),Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>0.&&R.s1.rt[%d]>0.&&R.s2.lt[7]>0.&&R.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
    T->Draw(Form("R.s1.rt[%d]-(R.s0.lt+R.s0.rt)/2.>>h2",i),
	    Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>20.&&R.s1.rt[%d]>20.&&R.s0.lt>0.&&R.s0.rt>0.&&DBB.evtypebits&(1<<2)&&DBB.edtpr[0]==0",
		 i,i,i,i));
    gPad->SetLogy();
    h2->SetNdivisions(-5);
    max_bin = h2->GetMaximumBin();
    pmin = h2->GetBinCenter(max_bin) - 100;
    pmax = h2->GetBinCenter(max_bin) + 100;
    min = h2->GetBinCenter(max_bin) - 50;
    max = h2->GetBinCenter(max_bin) + 50;
    h2->Delete();
    TH1F *p2 = new TH1F("p2",Form("Right_S1R_0%d",i+1),100,pmin,pmax);
    //    T->Draw(Form("R.s1.rt[%d]-(R.s2.lt[7]+R.s2.rt[7])/2.>>p2",i),Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>0.&&R.s1.rt[%d]>0.&&R.s2.lt[7]>0.&&R.s2.rt[7]>0.&&DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0",i,i,i,i));
    T->Draw(Form("R.s1.rt[%d]-(R.s0.lt+R.s0.rt)/2.>>p2",i),
	    Form("abs(R.s1.lt[%d]-R.s1.rt[%d])<200.&&R.s1.lt[%d]>20.&&R.s1.rt[%d]>20.&&R.s0.lt>0.&&R.s0.rt>0.&&DBB.evtypebits&(1<<2)&&DBB.edtpr[0]==0",
		 i,i,i,i));

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
  

  for(int j=0;j<6;j++){
    cerr <<timeL[j] - val[j]<<" ";
  }
  cerr <<"      - Left Paddles"<<endl;
  
  for(int j=0;j<6;j++){
    cerr <<timeR[j] - val[j+6]<<" ";
  }
  cerr <<"      - Right Paddles" <<endl;


//   TCanvas *c3 = new TCanvas("c3","c3");
//   TH2F *hh = new TH2F("hh","Right HRS S2m time",17,0,17,50,80,130);
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



