#include <iostream>
#include <iomanip>
using namespace std;
void cer_adcR()
{
  TChain *T = new TChain("T");
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  // filename = Form("/w/halla-2/e08014/yez/HRS_Cali/Right/replay/Rootfiles/e08014_R1_%d.root",run_number);
  // T->Add(filename);
  // T->Add(Form("/w/halla-2/e08014//yez/HRS_Cali/Right/replay/Rootfiles/e08014_R1_%d_1.root",run_number));
  // // T->Add(Form("/w/halla-2/e08014/yez/Rootfiles/yez/HRS_Cali/Right/replay/Rootfiles/e08014_R_%d_2.root",run_number));

  run_number = 3865;
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d.root",run_number);
  T->Add(filename);
  run_number = 3864;
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d.root",run_number);
   T->Add(filename);
  run_number = 3863;
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d.root",run_number);
   T->Add(filename);
  run_number = 3862;
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d.root",run_number);
   T->Add(filename);
  
  float peds[10] ={502,353,300,345,453,582,473,550,303,283};
  float oldgain[10] = { 1.96,   2.13,   1.44,   2.16,   1.21,   1.82,   1.54,   3.80,   1.42,   1.21};
  cout << T->GetEntries() << endl;

  TCut time_cut = "((DBB.evtypebits>>2)&1)&&DBB.edtpr[0]==0"; //Only T1 trigger, no EDTM pulse
  TCut sh_cut = "R.sh.e+R.ps.e<800.";
  
  //  TCut all_cut = "DBB.evtypebits&(1<<1)&&DBB.edtpr[0]==0&&R.sh.asum_c>1200.&&R.ps.asum_c>1400.";
  TCut all_cut="1";
  TString tcut;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t peak_need = 100;
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[10], val[10];
  TLatex *tex;
  ofstream myfile;
  myfile.open("R.cer_adj.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#   " << "     "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Peak_obs" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Peak_need" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "SPE" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Gain" << endl;
//   myfile << setiosflags(ios::left) << setw(8) << "HV_old"<< "   ";
//   myfile << setiosflags(ios::left) << setw(8) << "HV_new" << "   ";
//   myfile << setiosflags(ios::left) << setw(8) << "hv_ratio"  << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
  c1->Divide(2,5); 
  for(i=0;i<10;i++){
    
    c1->cd(i+1);

    //    tcut = Form("R.cer.t[%d]>1000.&&R.cer.a[%d]>100.",i,i);
    //    tcut = time_cut+"&&"+sh_cut;
    
    tcut = "((DBB.evtypebits>>2)&1)&&DBB.edtpr[0]==0 &&R.sh.e+R.ps.e<800.";
    
    TH1F *tt1 = new TH1F("tt1",Form("R-HRS Cer_0%d",i+1),250,300,1500);
    T->Draw(Form("R.cer.a[%d]>>tt1",i));
    max_bin = tt1->GetMaximumBin();
   
    min = tt1->GetBinCenter(max_bin) - 100;
    max = tt1->GetBinCenter(max_bin) + 100;
    float mean = tt1->GetBinCenter(max_bin);

    delete tt1;

    // TH1F *h1 = new TH1F("h1",Form("R-HRS Cer_0%d",i+1),250,min-200,max+400);
    // h1->SetLineColor(1);
    // T->Draw(Form("R.cer.a[%d]>>h1",i), tcut);  
    // h1->Fit("gaus","","",min,max);
    // ped_val = peds[i];

    TH1F *h1 = new TH1F("h1",Form("R-HRS Cer_0%d",i+1),250,20,3000);
    T->Draw(Form("R.cer.a[%d]>>h1",i), TCut(tcut));  
    if(i==4)
      h1->Fit("gaus","","",500,600);
    if(i==6)
      h1->Fit("gaus","","",500,600);
    if(i==7)
      h1->Fit("gaus","","",500,600);
    if(i==8)
      h1->Fit("gaus","","",300,400);
    if(i==9)
      h1->Fit("gaus","","",400,460);
    else
      h1->Fit("gaus","","",mean-50,mean+60);
    ped_val = peds[i] ;
    c1->Update();
    peak_val = gaus->GetParameter(1);
  
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    //   gPad->SetLogy();

    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);//*oldgain[i];
     
    cout << "Cer " << i << endl;
    myfile << "Cer#" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(2) << val[i]<< "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(2) << peak_ratio<<endl;
  } 
  c1->Print(Form("cer_adcl_R_%d.png",run_number));
  myfile.close();


  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",10,0,10,100,0,150);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraph *gr = new TGraph(10,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}

