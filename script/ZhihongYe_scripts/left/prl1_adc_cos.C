#include <iostream>
#include <iomanip>
using namespace std;
void prl1_adc_cos()
{
  Int_t right_or_left, run_number;    
//   cout << "enter run_number: ";
//   cin >> run_number;
  TString filename;
//   filename = Form("/w/halla-2/e08014/Rootfiles/e08014_less_%d.root",run_number);
  TChain *T = new TChain("T");
  //  T->Add(filename);
//   T->Add(Form("../Rootfiles/e07006_HRS_%d.root",run_number));
 //  T->Add("../Rootfiles/e07006_HRS_2840.root");
//   T->Add("../Rootfiles/e07006_HRS_2840_1.root");
//   T->Add("../Rootfiles/e07006_HRS_2841.root");
//   T->Add("../Rootfiles/e07006_HRS_2841_1.root");
//   T->Add("../Rootfiles/e07006_HRS_2842.root");
//   T->Add("../Rootfiles/e07006_HRS_2842_1.root");
  T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3957.root");
  T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3958.root");
  T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3959.root");
  T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3960.root");
  T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3961.root");
  T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3962.root");
   T->Add("/w/halla-2/e08014/Rootfiles/e08014_less_3965.root");
   

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
  Double_t num[34], val[34],gain[34];

  Double_t ped[34]={ 540,   522,   503,   426,   576,   549,   614,   586,
	    537,   500,   494,   470,   630,   602,   554,   595,
	    511,   400,   347,   333,   300,   491,   543,   422,
	    499,   344,   337,   398,   330,   578,   513,   524,
	    471,   498};
  TLatex *tex;
  ofstream myfile;
  myfile.open("L.prl1_adj.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Peak_obs" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "Gain" << endl;;
   
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);

    ped_val =ped[kk];

    TH1F *tt1 = new TH1F("tt1",Form("PRL1_%d",i+1),200,30.,500);
    T->Draw(Form("L.prl1.a_p[%d]>>tt1",i));//,Form("L.prl1.a_p[%d]>100.0",i));
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 55;
    max = tt1->GetBinCenter(max_bin) + 55;
    tt1->Fit("gaus","","",min,max);

//     min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
//     max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
 
//   delete tt1;
//     TH1F *h1 = new TH1F("h1",Form("PRL1_%d",i+1),100,min-40,min+400);
//     T->Draw(Form("L.prl1.a_p[%d]>>h1",i));
//     //    T->Draw(Form("L.prl1.a_p[%d]>>h1",i),Form("L.prl1.a_p[%d]>100.0&&L.cer.asum_p<400.&&L.prl2.a<600.",i));
//     h1->Fit("gaus","Q","",min,max);
//     ped_val = gaus->GetParameter(1);
//     max = ped_val + 2.5*gaus->GetParameter(2);   
//     if(i==0||i==1||i==2||i==14)
//       h1->Fit("landau","+Q","",max+10,max+300);
//     else if(i==3||i==7)
//       h1->Fit("landau","+Q","",max+30,max+300);
//     else if(i==15)
//       h1->Fit("landau","+Q","",max+50,max+250);
//     else
//       h1->Fit("landau","+Q","",max+40,max+300);
//     peak_val = landau->GetParameter(1);

    peak_val = gaus->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();
    
  
    val[i] = peak_val-ped_val;
    num[i] = i+1;
    //peak_ratio = peak_need/(peak_val-ped_val); 
    peak_ratio = peak_need/peak_val;
    gain[kk]=  peak_ratio; kk++;
    
    cout << "PRL1_ " << i << endl;
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(2) << gain[kk] <<endl;
  } 
  c1->Print(Form("L_prl1_adj1_%d.ps",run_number));
  c1->Print(Form("L_prl1_adj1_%d.png",run_number));

  
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4); 
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *tt2 = new TH1F("tt2",Form("PRL1_%d",i+17),200,30,500);
//     T->Draw(Form("L.prl1.a_p[%d]>>tt2",i+16),Form("L.prl1.a_p[%d]>100.0&&L.cer.asum_p<400.&&L.prl2.a<600.",i+16));
    T->Draw(Form("L.prl1.a_p[%d]>>tt2",i+16));

    max_bin = tt2->GetMaximumBin();
    min = tt2->GetBinCenter(max_bin) - 25;
    max = tt2->GetBinCenter(max_bin) + 25;
    tt2->Fit("gaus","","",min,max);
 
    //   min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
//     max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
//     delete tt2;
//     TH1F *h2 = new TH1F("h2",Form("PRL1_%d",i+17),100,min-40,min+400);
//     T->Draw(Form("L.prl1.a_p[%d]>>h2",i+16),Form("L.prl1.a_p[%d]>100.0",i+16));
//     h2->Fit("gaus","Q","",min,max);
//     ped_val = gaus->GetParameter(1);
//     max = ped_val + 2.5*gaus->GetParameter(2);   
//     if(i==0)
//       h2->Fit("landau","+Q","",max+90,max+300);
//     else if(i==1)
//       h2->Fit("landau","+Q","",max+10,max+300);
//     else if(i==2)
//       h2->Fit("landau","+Q","",max+10,max+250);
//       else if(i==3)
// 	h2->Fit("landau","+Q","",max+60,max+300);
//       else
//       h2->Fit("landau","+Q","",max+40,max+300);
//    peak_val = landau->GetParameter(1);
    
    peak_val = gaus->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();
    
    ped_val = ped[kk];
    val[i] = peak_val-ped_val;
    num[i] = i+1;
    //  peak_ratio = peak_need/(peak_val-ped_val);
    peak_ratio = peak_need/peak_val;
    
    gain[kk]=  peak_ratio; kk++;
    
    cout << "PRL1_ " << i << endl;
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(2) << gain[kk] <<endl;
  } 
  c2->Print(Form("L_prl1_adj2_%d.eps",run_number));
  c2->Print(Form("L_prl1_adj2_%d.png",run_number));
    
  
  TCanvas *c3 = new TCanvas("c3","c3",600,300);
  c3->Divide(2,1); 
  for(i=0;i<2;i++){
    c3->cd(i+1);
    TH1F *tt3 = new TH1F("tt3",Form("PRL1_%d",i+33),200,30,500);
    T->Draw(Form("L.prl1.a_p[%d]>>tt3",i+32));//,Form("L.prl1.a_p[%d]>100.0",i+32));
    max_bin = tt3->GetMaximumBin();
    min = tt3->GetBinCenter(max_bin) - 25;
    max = tt3->GetBinCenter(max_bin) + 40;
    tt3->Fit("gaus","","",min,max);
   //  min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
//     max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
//     delete tt3;
    
//     TH1F *h3 = new TH1F("h3",Form("PRL1_%d",i+33),100,min-40,min+600);
//     //    T->Draw(Form("L.prl1.a_[%d]>>h3",i+32),Form("L.prl1.a_p[%d]>100.0&&L.cer.asum_p<600.",i+32));
//     T->Draw(Form("L.prl1.a_p[%d]>>tt2",i+32));

//     h3->Fit("gaus","Q","",min,max);
//     ped_val = gaus->GetParameter(1);
//     max = ped_val + 2.5*gaus->GetParameter(2);   
//     if(i==0)
//       h3->Fit("landau","+Q","",max+60,max+300);
//     else if(i==1)
//       h3->Fit("landau","+Q","",max+80,max+330);
//     else
//       h3->Fit("landau","+Q","",max+40,max+300);
//    peak_val = landau->GetParameter(1);
    peak_val = gaus->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();
   
    val[i] = peak_val-ped_val;
    num[i] = i+1;
    ped_val=ped[kk];
    //peak_ratio = peak_need/(peak_val-ped_val); 
    peak_ratio = peak_need/peak_val;
    gain[kk]=  peak_ratio; kk++;
    
    cout << "PRL1_ " << i << endl;
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(2) << gain[kk] << endl;
  } 
  c3->Print(Form("L_prl1_adj3_%d.ps",run_number));

  for(int kb=0;kb<34;kb++){
    
    myfile<<"   "<<int(ped[kb]);
  }
  myfile<<endl;

  for(int kb=0;kb<34;kb++){
    
    myfile<<"   "<<gain[kb];
  }
  myfile<<endl;

  cout << T->GetEntries() << endl;
  myfile.close();

}

