#include <iostream>
#include <iomanip>
using namespace std;
void plot()
{
  ifstream file("prl1.dat");  
  ofstream efile;
  efile.open("L.prl1_adj_new.txt");
  Double_t ped[34],peak[34],val[34],gain[34],num[34];
  int i=0;
  
  efile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  efile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  efile << setiosflags(ios::left) << setw(5) << "Peak" << "   ";
  efile << setiosflags(ios::left) << setw(5) << "SPE" <<"   ";;
  efile << setiosflags(ios::left) << setw(5) << "Gain" << endl;
  while(!file.eof()){
    file >> ped[i]>>peak[i];
    if(i==3)
      peak[i]+=10;
    val[i] = peak[i]-ped[i];
    gain[i]=100./val[i];
    num[i]=i;
    efile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped[i] << "   ";
    efile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << peak[i] << "   ";
    efile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << val[i]<< "   ";
    efile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(2) << gain[i] <<endl;
    i++;
  }
  
  file.close();
  int N=i;
  cerr <<"Total: "<<N<<endl;
  for(int j=0;j<=N;j++){

    efile <<setiosflags(ios::left) << setw(5)<<gain[j]<<" ";
  }
  cerr<<endl;

  gStyle->SetOptStat(0);
  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",35,0,35,10,0,200);
  hh->GetXaxis()->SetTitle("L-HRS PRL1 PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraph *gr = new TGraph(34,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");

}
