#include <iostream>
#include <iomanip>
using namespace std;
void cer_adcR()
{ 
  // Fill color
  gStyle->SetStatColor(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(0);
  gStyle->SetFrameFillColor(0);
	
  // Border mode
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPalette(1,0);
	
  // Font
  gStyle->SetTextFont(22);
  gStyle->SetLabelFont(22, "xyz");
  gStyle->SetTitleFont(22, "xyz");
  gStyle->SetStatFont(22);
  gStyle->SetTitleFillColor(0);
  // Opt
  gStyle->SetOptDate(1);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  T->Add(Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_1.root",run_number));
  T->Add(Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_2.root",run_number));
  T->Add(Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_3.root",run_number));
  T->Add(Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_4.root",run_number));
  T->Add(Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_5.root",run_number));

  float HV[10]={-2050.0,-2125.0,-2222.0,-2104.0,-2166.0,-2007.0,-2147.0,-2100.0,-2135.0,-2020.0};

  cout << T->GetEntries() << endl;

  TCut time_cut = "DBB.evtypebits&(1<<1)&&DBB.edtpr[0]==0"; //Only T1 trigger, no EDTM pulse
  TCut sh_cut = "R.sh.asum_c>1200.&&R.ps.asum_c>1400.";
  
  TCut sh_cut = "1";
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Double_t min, max, temp;
  Double_t ped_val, peak_val;
  Double_t peak_ratio, hv_ratio;
  Double_t peak_need = 60;
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
  myfile << setiosflags(ios::left) << setw(8) << "HV_old"<< "   ";
  myfile << setiosflags(ios::left) << setw(8) << "HV_new" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "hv_ratio"  << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,500);
  c1->Divide(5,2); 
  for(i=0;i<10;i++){
    c1->cd(i+1);
    TH1F *tt1 = new TH1F("tt1",Form("R-HRS Cer_0%d",i+1),250,200,1500);
    T->Draw(Form("R.cer.a[%d]>>tt1",i));
    max_bin = tt1->GetMaximumBin();
   
    if(i==6)
      min = tt1->GetBinCenter(max_bin) - 25;
    else
      min = tt1->GetBinCenter(max_bin) - 35;
    
    max = tt1->GetBinCenter(max_bin) + 25;
    tt1->Fit("gaus","NQ","",min,max);

    float kmin = tt1->GetBinCenter(max_bin) - 10;
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt1;

    TH1F *h1 = new TH1F("h1",Form("R-HRS Cer_0%d",i+1),250,200,1000);
    TH1F *j1 = new TH1F("j1",Form("R-HRS Cer_0%d",i+1),250,200,1000);

    h1->SetLineColor(2);
    T->Draw(Form("R.cer.a[%d]>>h1",i));  
    j1->SetLineColor(4);
    T->Draw(Form("R.cer.a[%d]>>j1",i), "R.sh.asum_c>1200.&&R.ps.asum_c>1400."); 
     
    h1->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    max = ped_val + 2.5*gaus->GetParameter(2);   
    if(i==0)
      j1->Fit("landau","+Q","",510,610);
    else if(i==1)
      j1->Fit("landau","+Q","",360,460);
    else if(i==2)
      j1->Fit("landau","+Q","",323,430);
    else if(i==3)
      j1->Fit("landau","+Q","",340,430);
    else if(i==4)
      j1->Fit("landau","+Q","",480,630);
    else if(i==5)
      j1->Fit("landau","+Q","",600,670);
    else if(i==6)
      j1->Fit("landau","+Q","",490,600);
    else if(i==7)
      j1->Fit("landau","+Q","",560,650);
    else if(i==8)
      j1->Fit("landau","+Q","",320,450);
    else if(i==9)
      j1->Fit("landau","+Q","",320,450);
  
    h1->Draw();j1->Draw("same");
    peak_val = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val,peak_val));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    val[i] = peak_val-ped_val;
    num[i] = i+1;
    peak_ratio = peak_need/(peak_val-ped_val);
    hv_ratio = TMath::Power(peak_ratio,0.1);
   //  if(i==0)
//       hv_ratio=1;
    cout << "Cer " << i << endl;
    myfile << "Cer#" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << (ped_val+peak_need) <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(3) << val[i]<< "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << HV[i]<< "   ";
    myfile << setiosflags(ios::left) << setw(8) 
	   << setiosflags(ios::fixed) << setprecision(1) << HV[i]*hv_ratio<<"   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(3) << hv_ratio << endl;

  } 
  c1->Print(Form("cer_adcl_R_%d.ps",run_number));
  myfile.close();


  cerr<<"====New HV Setting"<<endl;
  for(int j=0;j<10;j++)
    cerr<<HV[j]*hv_ratio<<" ";
  cerr<<endl;



  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",10,0,10,100,0,100);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraph *gr = new TGraph(10,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}

