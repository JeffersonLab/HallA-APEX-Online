#include <iostream>
#include <iomanip>
using namespace std;
void cer_adcL()
{ 
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");

  TString filename;
  filename = Form("/w/halla-2/e08014/Rootfiles_0721/e08014_L_%d.root",run_number);

  // run_number = 3957;
  // filename = Form("/w/halla-2/e08014/Rootfiles_0714/e08014_less_%d.root",run_number);
  // T->Add(filename);
  // run_number = 3958;
  // filename = Form("/w/halla-2/e08014/Rootfiles_0714/e08014_less_%d.root",run_number);
  // T->Add(filename);
  // run_number = 3959;
  // filename = Form("/w/halla-2/e08014/Rootfiles_0714/e08014_less_%d.root",run_number);
  // T->Add(filename);
  // run_number = 3960;
  // filename = Form("/w/halla-2/e08014/Rootfiles_0714/e08014_less_%d.root",run_number);
  // T->Add(filename);
  
  run_number = 3957; filename = Form("/w/halla-2/e08014/Rootfiles/e08014_L_%d.root",run_number);
  T->Add(filename);
  run_number = 3958; filename = Form("/w/halla-2/e08014/Rootfiles/e08014_L_%d.root",run_number);
  T->Add(filename);
  run_number = 3959; filename = Form("/w/halla-2/e08014/Rootfiles/e08014_L_%d.root",run_number);
  T->Add(filename);
  run_number = 3960; filename = Form("/w/halla-2/e08014/Rootfiles/e08014_L_%d.root",run_number);
  T->Add(filename);


  float peds[10] ={567,568,595,525,617,673,678,685,536,564};
  //  float peds[10] ={586,585,615,543,633,673,678,685,536,564};

  cout << T->GetEntries() << endl;

  TString tcut;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  Double_t min, max, temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t peak_need = 100;
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[10], val[10];
  TLatex *tex;
  ofstream myfile;
  myfile.open("L.cer_adj_new.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#   " << "     "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Peak_obs" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Peak_need" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "SPE" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Gain" << endl;

  TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
  c1->Divide(2,5); 
  for(i=0;i<10;i++){
    
    c1->cd(i+1);
      tcut = Form("L.cer.t[%d]>1000.&&(L.prl1.asum_c+L.prl2.asum_c<600.)",i,i);
    //  tcut = Form("L.cer.t[%d]>1000.&&DBB.evtypebits>>1&1",i,i);
    //   tcut = Form("DBB.evtypebits>>1&1",i,i);
    
     /*    if(i==0)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.9&&L.cer.trx<-0.75",i,i);
    if(i==1)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.9&&L.cer.trx<-0.7",i,i);
    if(i==2)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.75&&L.cer.trx<-0.35",i,i);
    if(i==3)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.7&&L.cer.trx<-0.3",i,i);
    if(i==4)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.4&&L.cer.trx<0.05",i,i);
    if(i==5)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.35&&L.cer.trx<0.1",i,i);
    if(i==6)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>0.&&L.cer.trx<0.45",i,i);
    if(i==7)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>0.05&&L.cer.trx<0.5",i,i);
    if(i==8)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>0.4&&L.cer.trx<0.8",i,i);
    if(i==9)
      tcut = Form("L.cer.t[%d]>1000.&&L.cer.a[%d]>100.&&L.cer.trx>-0.45&&L.cer.trx<0.9",i,i);
    */
    TH1F *tt1 = new TH1F("tt1",Form("L-HRS Cer_0%d",i+1),250,200,1500);
    T->Draw(Form("L.cer.a[%d]>>tt1",i),tcut);
    max_bin = tt1->GetMaximumBin();
   
    min = tt1->GetBinCenter(max_bin) - 45;
    max = tt1->GetBinCenter(max_bin) + 50;
    
    delete tt1;

    TH1F *h1 = new TH1F("h1",Form("L-HRS Cer_0%d",i+1),250,0,3000);
    h1->SetLineColor(1);
    T->Draw(Form("L.cer.a_c[%d]>>h1",i), tcut);  
    
    h1->Fit("gaus","","",50,150);

    // if(i==0)
    //   h1->Fit("gaus","","",650,830);
    // if(i==1)
    //   h1->Fit("gaus","","",680,850);
    // if(i==2)
    //   h1->Fit("gaus","","",650,930);
    // if(i==3)
    //   h1->Fit("gaus","","",600,810);
    // if(i==4)
    //   h1->Fit("gaus","","",690,830);
    // if(i==5)
    //   h1->Fit("gaus","","",720,900);
    // if(i==6)
    //   h1->Fit("gaus","","",750,920);
    // if(i==7)
    //   h1->Fit("gaus","","",750,900);
    // if(i==8)
    //   h1->Fit("gaus","","",600,780);
    // if(i==9)
    //   h1->Fit("gaus","","",650,820);
    

    // ped_val = peds[i];
    ped_val = 0;
    peak_val = gaus->GetParameter(1);
  
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    //   gPad->SetLogy();

    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
     
    cout << "Cer " << i << endl;
    myfile << "Cer#" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(2) << val[i]<< "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(2) << peak_ratio<<endl;
  } 
  c1->Print(Form("cer_adcl_L_new_%d.png",run_number));
  myfile.close();


  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",10,0,10,100,50,300);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraph *gr = new TGraph(10,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}

