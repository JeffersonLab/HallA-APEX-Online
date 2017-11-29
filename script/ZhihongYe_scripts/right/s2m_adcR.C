#include <iostream>
#include <iomanip>
using namespace std;
void s2m_adcR()
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
  T->Add( Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_1.root",run_number));
  T->Add( Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_2.root",run_number));
  T->Add( Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_3.root",run_number));
  T->Add( Form("/adaqfs/home/adaq/SRCs/replay/Rootfiles/e07006_HRS_%d_4.root",run_number));

  cout << T->GetEntries() << endl;
  //TCut time_cut = "TMath::Abs(R.vdc.u1.rawtime-900)<300 && TMath::Abs(R.vdc.u2.rawtime-900)<300 && TMath::Abs(R.vdc.v1.rawtime-900)<300 && TMath::Abs(R.vdc.v2.rawtime-900)<300";
  TCut time_cut = "DBB.evtypebits&(1<<1)&&DBB.edtpr[0]==0"; //Only T1 trigger, no EDTM pulse
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Float_t min, max, temp;
  Float_t ped_val[32], peak_val[32];
  Float_t peak_ratio[32], hv_ratio[32];
  Float_t peak_need = 300;
  Int_t max_bin;
  Int_t i, ii;
  Float_t num[32], val[32];
  TLatex *tex;
  ofstream myfile;
  myfile.open("R.s2_adj.txt");
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "Peak_obs" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "NPE" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "HV_old" << "   ";
  myfile << setiosflags(ios::left) << setw(9) << "HV_new" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "HV_ratio"  << endl;

  Float_t HV[32]={
    -1719.0, -1688.0, -1695.0, -1758.0, -1660.0, -1745.0, -1585.0, -1745.0, -1810.0, -1681.0, -1821.0, -1842.0, -1575.0, -1585.0, -1625.0, -1670.0,
    -1849.0, -1628.0, -1635.0, -1700.0, -1750.0, -1651.0, -1775.0, -1777.0, -1710.0, -1585.0, -1720.0, -1730.0, -1810.0, -1854.0, -1920.0, -1940.0};
  

  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);
    TH1F *tt1 = new TH1F("tt1",Form("R-HRS S2L_0%d",i+1),200,250,800);
    T->Draw(Form("R.s2.la[%d]>>tt1",i),time_cut);
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 25;
    max = tt1->GetBinCenter(max_bin) + 25;
    tt1->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt1;
    TH1F *h1 = new TH1F("h1",Form("R-HRS S2L_0%d",i+1),100,min-100,min+1500);
    T->Draw(Form("R.s2.la[%d]>>h1",i),time_cut);
    h1->Fit("gaus","Q","",min,max);
    ped_val[i] = gaus->GetParameter(1);
    max = ped_val[i] + 2.5*gaus->GetParameter(2);   
    if(i==0)
      h1->Fit("landau","+Q","",max+60,max+1300);
    else if(i==2||i==1)
      h1->Fit("landau","+Q","",max+80,max+1300);
    else
      h1->Fit("landau","+Q","",max+20,max+1300);
    peak_val[i] = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val[i],peak_val[i]));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    val[i] = peak_val[i]-ped_val[i];
    num[i] = i+1;
    peak_ratio[i] = peak_need/val[i];
    //    hv_ratio[i] = TMath::Power(peak_ratio[i],0.1);
    hv_ratio[i] = TMath::Power(peak_ratio[i],0.1);
    cout << "Left " << i << endl;
    myfile << "L" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val[i] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val[i] << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << val[i] <<  "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << HV[i] <<  "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << HV[i]*hv_ratio[i] <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(3) << hv_ratio[i] << endl;
  } 
  c1->Print(Form("R_s2_adcL_%d.png",run_number));
  c1->Print(Form("R_s2_adcL_%d.pdf",run_number));
  
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4);
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *tt2 = new TH1F("tt2",Form("R-HRS S2R_0%d",i+1),200,200,800);
    T->Draw(Form("R.s2.ra[%d]>>tt2",i),time_cut);
    max_bin = tt2->GetMaximumBin();
    min = tt2->GetBinCenter(max_bin) - 25;
    max = tt2->GetBinCenter(max_bin) + 25;
    tt2->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 4*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 4*gaus->GetParameter(2);
    delete tt2;
    TH1F *h2 = new TH1F("h2",Form("R-HRS S2R_0%d",i+1),100,min-100,min+1500);
    T->Draw(Form("R.s2.ra[%d]>>h2",i),time_cut);
    h2->Fit("gaus","Q","",min,max);
    ped_val[i+16] = gaus->GetParameter(1);
    max = ped_val[i+16] + 3*gaus->GetParameter(2);
    if(i==0)
      h2->Fit("landau","+Q","",max+50,max+1300);
    else if(i==1||i==2)
      h2->Fit("landau","+Q","",max+70,max+1300);
    else
      h2->Fit("landau","+Q","",max,max+1300);
    peak_val[i+16] = landau->GetParameter(1);
    tex = new TLatex(0.3,0.9,Form("Ped - %3.2lf,   Peak - %3.2lf",ped_val[i+16],peak_val[i+16]));
    tex->SetNDC();
    tex->Draw();
    gPad->SetLogy();

    val[i+16] = peak_val[i+16]-ped_val[i+16];
    num[i+16] = i+16+1;
    peak_ratio[i+16] = peak_need/(peak_val[i+16]-ped_val[i+16]);
    // hv_ratio[i+16] = TMath::Power(peak_ratio[i+16],0.1);
    hv_ratio[i+16] = TMath::Power(peak_ratio[i+16],0.1);
    cout << "Right " << i << endl;
    myfile << "R" << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << ped_val[i+16] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << peak_val[i+16] << "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << val[i+16] <<  "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << HV[i+16] <<  "   ";
    myfile << setiosflags(ios::left) << setw(9) << setiosflags(ios::fixed) << setprecision(1) << HV[i+16]*hv_ratio[i+16] <<  "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(3) << hv_ratio[i+16] << endl;
  }
  c2->Print(Form("R_s2_adcR_%d.png",run_number));
  c2->Print(Form("R_s2_adcR_%d.pdf",run_number));

 cerr <<"==========Ped Peaks==========="<<endl;
  for(int k=0;k<16;k++){
    cerr << " "<<ped_val[k];
  }
  cerr<<"   -- Left ADC"<<endl;

  for(int k=16;k<32;k++){
    cerr << " "<<ped_val[i];
  }
  cerr<<"   -- Right ADC"<<endl;
 
  cerr <<"============================="<<endl;
  cerr <<"==========HV Ratio==========="<<endl;
  for(int k=0;k<16;k++){
    cerr << " "<<hv_ratio[k];
  }
  cerr<<"   -- Left ADC"<<endl;

  for(int k=16;k<32;k++){
    cerr << " "<<hv_ratio[k];
  }
  cerr<<"   -- Right ADC"<<endl;
  cerr <<"============================="<<endl;


  cout << T->GetEntries() << endl;
  myfile.close();

  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",33,0,33,100,0,400);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraph *gr = new TGraph(32,num,val);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}

