#include <iostream>
#include <iomanip>
using namespace std;
void prl2_adc()
{
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  cout << T->GetEntries() << endl;
  
  TCut _cut = "1";
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t peak_need = 100;
  Int_t max_bin;
  Int_t i, ii;
  int kk=0;
  Double_t num[34], val[34],ped[34];
  TLatex *tex;
  ofstream myfile;
  myfile.open("L.prl2_adj.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Peak_obs" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Peak_need" << "   ";
   
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);
    TH1F *tt1 = new TH1F("tt1",Form("PRL2_%d",i+1),200,250,800);
    T->Draw(Form("L.prl2.a[%d]>>tt1",i),Form("L.prl2.a[%d]>100.0",i));
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 25;
    max = tt1->GetBinCenter(max_bin) + 25;
    tt1->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt1;
    TH1F *h1 = new TH1F("h1",Form("PRL2_%d",i+1),100,min-40,min+400);
    T->Draw(Form("L.prl2.a[%d]>>h1",i),Form("L.prl2.a[%d]>100.0",i));
    h1->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    max = ped_val + 2.5*gaus->GetParameter(2);   
    if(i==0||i==1||i==2)
      h1->Fit("landau","+Q","",max+60,max+300);
    else if(i==3)
      h1->Fit("landau","+Q","",max+80,max+300);
    else
      h1->Fit("landau","+Q","",max+20,max+300);
    peak_val = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    ped[kk]=ped_val; kk++;
    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
    hv_ratio = TMath::Power(peak_ratio,0.1);
    cout << "PRL2_ " << i << endl;
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<endl;
  } 
  c1->Print(Form("L_prl2_adj1_%d.ps",run_number));
 
  
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4); 
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *tt2 = new TH1F("tt2",Form("PRL2_%d",i+17),200,250,800);
    T->Draw(Form("L.prl2.a[%d]>>tt2",i+16),Form("L.prl2.a[%d]>100.0",i+16));
    max_bin = tt2->GetMaximumBin();
    min = tt2->GetBinCenter(max_bin) - 25;
    max = tt2->GetBinCenter(max_bin) + 25;
    tt2->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt2;
    TH1F *h2 = new TH1F("h2",Form("PRL2_%d",i+17),100,min-40,min+400);
    T->Draw(Form("L.prl2.a[%d]>>h2",i+16),Form("L.prl2.a[%d]>100.0",i+16));
    h2->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    max = ped_val + 2.5*gaus->GetParameter(2);   
    if(i==0||i==1||i==2)
      h2->Fit("landau","+Q","",max+60,max+300);
    else if(i==3)
      h2->Fit("landau","+Q","",max+80,max+300);
    else
      h2->Fit("landau","+Q","",max+20,max+300);
    peak_val = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    ped[kk]=ped_val; kk++;
    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
    hv_ratio = TMath::Power(peak_ratio,0.1);
    cout << "PRL2_ " << i << endl;
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<  "   ";
  } 
  c2->Print(Form("L_prl2_adj2_%d.ps",run_number));
   
  
  TCanvas *c3 = new TCanvas("c3","c3",1200,800);
  c3->Divide(2,1); 
  for(i=0;i<2;i++){
    c3->cd(i+1);
    TH1F *tt3 = new TH1F("tt3",Form("PRL2_%d",i+33),200,250,800);
    T->Draw(Form("L.prl2.a[%d]>>tt3",i+32),Form("L.prl2.a[%d]>100.0",i+32));
    max_bin = tt3->GetMaximumBin();
    min = tt3->GetBinCenter(max_bin) - 25;
    max = tt3->GetBinCenter(max_bin) + 25;
    tt3->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt3;
    
    TH1F *h3 = new TH1F("h3",Form("PRL2_%d",i+33),100,min-40,min+400);
    T->Draw(Form("L.prl2.a[%d]>>h3",i+32),Form("L.prl2.a[%d]>100.0",i+32));
    h3->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    max = ped_val + 2.5*gaus->GetParameter(2);   
    if(i==0||i==1||i==2)
      h3->Fit("landau","+Q","",max+60,max+300);
    else if(i==3)
      h3->Fit("landau","+Q","",max+80,max+300);
    else
      h3->Fit("landau","+Q","",max+20,max+300);
    peak_val = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    ped[kk]=ped_val; kk++;
    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
    hv_ratio = TMath::Power(peak_ratio,0.1);
    cout << "PRL2_ " << i << endl;
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) << endl;;
  } 
  c3->Print(Form("L_prl2_adj3_%d.ps",run_number));

  myfile<<endl;
  for(int kb=0;kb<kk+1;kb++){
    
    myfile<<"   "<<ped[kb];
  }
  myfile<<endl;

  cout << T->GetEntries() << endl;
  myfile.close();

}

