//by Eric Jensen
//Locates the first photo-electron peak of each PMT
//Calculates amplitude adjustment coefficient

#include <iostream>
#include <fstream>
void cer_peakL(){

  Int_t run_number;
  TString filename;
  cout << "enter run_number: ";
  cin >> run_number;
  filename = Form("../ROOTfiles/dvcs_lhrs_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);

  gStyle->SetPalette(1,0);
  gStyle->SetFillColor(10);
  gStyle->SetOptStat(0);


  ofstream myFile;
  myFile.open("cer_peak_L.txt");
  myFile << "Left arm gas Cherenkov" << endl;
  myFile << "First photo-electron peak" << endl;
  myFile << endl;
  myFile << "GC#" << "  " << "Peak_obs" << "   " << "Peak_need" << "   " << "Ratio" << endl;

  TCanvas *c1 = new TCanvas("c1","With position cut",1000,800);
  c1->SetFillColor(0);
  Int_t i,j;
  TCut cut;
  TLatex *tex;
  Double_t peak_need = 200;
  Double_t ratio[10];
  c1->Divide(5,2);
  TString name, tit, dr;
  Int_t max_bin;
  Double_t temp,min,max,peak[10];
  TH1F *gc[10];
  TF1 *gaus = new TF1("gaus","gaus",10,300);
  

  for(i=0;i<10;i++){

    c1->cd(i+1);
    name = Form("gc%d",i);
    tit = Form("gc%d",i);
    if(i==2||i==4){
      gc[i] = new TH1F(name,tit,100,70,750);
    }
    else{
      gc[i] = new TH1F(name,tit,100,50,1000);
    }
    dr = Form("L.cer.a_p[%d]",i);
    dr = dr + ">>";
    dr = dr + name;
    
    //cut = new TCut(Form("L.cer.trx>%lf&&L.cer.trx<%lf&&L.cer.try>%lf&&L.cer.try<%lf",a[i],b[i],c[i],d[i]));
    T->Draw(dr,cut);
    
    max_bin = gc[i]->GetMaximumBin();

    if(i==2||i==3){
 
 
      cout << "max bin - " << max_bin << endl;
      if(i==2){

	min = gc[i]->GetBinCenter(max_bin) - 100;
	max = gc[i]->GetBinCenter(max_bin) + 100;
      }
      else{
	min = gc[i]->GetBinCenter(max_bin) - 150;
	max = gc[i]->GetBinCenter(max_bin) + 100;
      }
;
      gc[i]->Fit("gaus","WWQ","",min, max);
      peak[i] = gaus->GetParameter(1);
      ratio[i] = peak_need/peak[i];
      cout << "peak - " << peak[i] << endl;
      tex = new TLatex(0.3,0.85,"Cut on x and y position");
      tex->SetNDC();
      tex->SetTextSize(0.05);
      tex->SetTextColor(2);
      tex->Draw();
      tex = new TLatex(0.34,0.75,Form("peak - %lf",peak[i]));
      tex->SetNDC();
      tex->SetTextSize(0.05);
      tex->Draw();

      myFile << i << "   "  << peak[i] << "   " << peak_need << "   " << ratio[i] << endl;
    } 
    else{ 

    cout << "max bin - " << max_bin << endl;
    if(i==0){
      min = gc[i]->GetBinCenter(max_bin) - 100;
      max = gc[i]->GetBinCenter(max_bin) + 100;
    }
    else if(i==1){
      min = gc[i]->GetBinCenter(max_bin) - 110;
      max = gc[i]->GetBinCenter(max_bin) + 100;
    }
    else if(i==4){

      min = gc[i]->GetBinCenter(max_bin) - 100;
      max = gc[i]->GetBinCenter(max_bin) + 70;
    }
    else if(i==6){
      min = gc[i]->GetBinCenter(max_bin) - 120;
      max = gc[i]->GetBinCenter(max_bin) + 100;
    }
    else if(i==7){
      min = gc[i]->GetBinCenter(max_bin) - 100;
      max = gc[i]->GetBinCenter(max_bin) + 70;
    }
    else if(i==9){
      min = gc[i]->GetBinCenter(max_bin) - 100;
      max = gc[i]->GetBinCenter(max_bin) + 100;
    }
    else{
      min = gc[i]->GetBinCenter(max_bin) - 100;
      max = gc[i]->GetBinCenter(max_bin) + 100;
    }

    gc[i]->Fit("gaus","WWQ","",min,max);

    peak[i] = gaus->GetParameter(1);
    ratio[i] = peak_need/peak[i];
    cout << "peak - " << peak[i] << endl;
    tex = new TLatex(0.34,0.75,Form("peak - %lf",peak[i]));
    tex->SetNDC();
    tex->SetTextSize(0.05);
    tex->Draw();

    myFile << i << "   "  << peak[i] << "   " << peak_need << "   " << ratio[i] << endl;
    }

  }


}
