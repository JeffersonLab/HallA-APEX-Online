#include <iostream>
#include <iomanip>
using namespace std;
void s2m_adcL()
{
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  cout << T->GetEntries() << endl;
  TCut time_cut = "DBB.evtypebits&(1<<3)&&DBB.edtpr[0]==0";
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio[32];
  Double_t peak_need = 20;
  Int_t max_bin;
  Int_t i, ii;
  Double_t num[32], val[32];
  TLatex *tex;
  ofstream myfile;
  myfile.open("L.s2m_adj.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Peak_obs" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Peak_need" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "HV_ratio"  << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);
    TH1F *tt1 = new TH1F("tt1",Form("S2mL_0%d",i+1),200,10,1000);
    T->Draw(Form("L.s2.la[%d]>>tt1",i),time_cut);
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 25;
    max = tt1->GetBinCenter(max_bin) + 25;
    tt1->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt1;
    TH1F *h1 = new TH1F("h1",Form("S2mL_0%d",i+1),100,min-20,min+150);
    T->Draw(Form("L.s2.la[%d]>>h1",i),time_cut);
    h1->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    max = ped_val + 2.5*gaus->GetParameter(2);   
     
    if(i==0){
      max = ped_val + 3.5*gaus->GetParameter(2);
    }
    if(i==6){
      max = ped_val + 3.5*gaus->GetParameter(2);
    }
    if(i==12 || i==13 || i==14 || i==15){
      max = ped_val + 5*gaus->GetParameter(2);
    }
    
    h1->Fit("landau","+Q","",max,800);
    peak_val = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
    hv_ratio[i] = TMath::Power(peak_ratio,0.1);
    cout << "Left " << i << endl;
    myfile << "L" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(3) << hv_ratio[i] << endl;
  } 
  c1->Print(Form("s2m_adcl_L_%d.ps",run_number));
  
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4);
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *tt2 = new TH1F("tt2",Form("S2mR_0%d",i+1),200,10,1000);
    T->Draw(Form("L.s2.ra[%d]>>tt2",i),time_cut);
    max_bin = tt2->GetMaximumBin();
    min = tt2->GetBinCenter(max_bin) - 25;
    max = tt2->GetBinCenter(max_bin) + 25;
    tt2->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt2;
    TH1F *h2 = new TH1F("h2",Form("S2mR_0%d",i+1),100,min-20,min+150);
    T->Draw(Form("L.s2.ra[%d]>>h2",i),time_cut);
    h2->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    max = ped_val + 3*gaus->GetParameter(2);
    
    if(i==0){
      max = ped_val + 5*gaus->GetParameter(2);
    }
    
    if(i==1 || i==7){
      max = ped_val + 4*gaus->GetParameter(2);
    }
    if(i==12 || i==14 || i==15){
      max = ped_val + 5*gaus->GetParameter(2);
    }    
    h2->Fit("landau","+Q","",max,800);
    peak_val = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    val[i+16] = peak_val-ped_val;
    num[i+16] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
    hv_ratio[i+16] = TMath::Power(peak_ratio,0.1);
    cout << "Right " << i << endl;
    myfile << "R" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(3) << hv_ratio[i+16] << endl;
  }
   myfile.close(); c2->Print(Form("s2m_adcr_L_%d.ps",run_number));
  cout << T->GetEntries() << endl;
  
  for(int j=0;j<16;j++){
    cerr << hv_ratio[j];
  }

  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",33,0,33,100,0,100);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraph *gr = new TGraph(32,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}

