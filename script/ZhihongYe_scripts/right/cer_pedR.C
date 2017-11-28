//by Eric Jensen
//Locates pedestal value for each PMT

#include <iostream>
#include <iomanip>
void cer_pedR(){
  Int_t run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("/adaqfs/home/adaq/SRCs/replay/ROOTfiles/e07006_det_R_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);

  ofstream myfile;
  myfile.open("cer_ped_R.txt");
  cout << T->GetEntries() << endl;
  TCanvas *c1 = new TCanvas("c1","c1",1200,700);
  c1->Divide(5,2);
  TString name, tit, dr;
  Double_t min, max;
  Int_t max_bin;
  Int_t i;
  Double_t ped[10];
  TH1F *h1[10], *hh[10], *hh1[10];
  for(i=0;i<10;i++){
    h1[i] = new TH1F(Form("h%d",i),Form("h%d",i),300,0,1500);
  }
  for(i=0;i<10;i++){
    c1->cd(i+1);
    T->Draw(Form("R.cer.a[%d]>>h%d",i,i));
    max_bin = h1[i]->GetMaximumBin();
    ped[i] = h1[i]->GetBinCenter(max_bin);
    min = ped[i] - 100;
    max = min + 400;
    name = Form("gc_%d",i);
    tit = Form("gc_%d",i);
    hh[i] = new TH1F(name,tit,250,min,max);
    dr = Form("R.cer.a[%d]",i);
    dr = dr + ">>";
    dr = dr + name;
    T->Draw(dr);
    hh[i]->Fit("gaus","","",ped[i]-25,ped[i]+25);
    ped[i] = gaus->GetParameter(1);
    TLine *l1 = new TLine(ped[i],0,ped[i],hh[i]->GetMaximum());
    l1->SetLineColor(2);
    l1->Draw();
    gPad->SetLogy();
  }
  for(i=0;i<10;i++){
    myfile<< setprecision(3) << ped[i] << "   "; 
    cout<< setprecision(3) << ped[i] << "   ";
  }
  myfile <<endl;
  cout << endl;
}
