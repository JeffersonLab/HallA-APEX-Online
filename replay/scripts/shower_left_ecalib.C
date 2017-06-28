#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include  <stdio.h>
#include  <stdlib.h>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"

/*
functions
1. read_db()         - to read DB pedestals
2. fit_pedestals()   - to fit the pedestals (if needed)
3. draw_xy()         - to draw x, y coordinates and show ranges for the calibration events selection
4. minimization_matrix() - minimization matrixes calculation
5. calib_calc()      - Matrix operations and coefficients calculations
6. draw_calibrated() - To Draw the calibrated energy
*/

Double_t ped_prl1[34]={0};
Double_t ped_prl2[34]={0};
Double_t prl1_coef[34], prl2_coef[34];
Double_t prl1_coef_db[34], prl2_coef_db[34];
Double_t ped_db_prl1[34], ped_db_prl2[34];

string DB_path;
double xmin[17]={100., 0.78, 0.65, 0.49, 0.33, 0.20, 0.05, -0.09, -0.24, -0.38, -0.53, -0.68, -0.83, -0.97, -1.11, -1.3, -100.};
double xcen[17]={100., 0.82,0.675, 0.53, 0.385,0.24, 0.095,-0.05, -0.195,-0.34, -0.485,-0.63, -0.775,-0.92, -1.065,-1.21,-100.};
double xmax[17]={100., 0.87, 0.72, 0.57, 0.43, 0.29, 0.14, -0.01, -0.16, -0.30, -0.45, -0.60, -0.75, -0.89, -1.06, -1.2, -100.};
double ymin_l=0.04, ymax_l=0.12;
double ymin_r=-0.16, ymax_r=-0.06;
double ymin = -0.16, ymax = 0.12;
const int n_runs=10;
int runs[n_runs]={13862,13863,13864,13866,13867,13870,13876,13877,13878,13874};
double S[68][68];
double S0[68];

void shower_left_ecalib()
{
  using namespace std;
  DB_path="/adaqfs/home/a-onl/gmp/replay/DB/20160910/";
  string ped_qstn;
  cout<<"\nPlease chose one of the following options \n\nInput 1 if you want the code to read the pedestal positions from the database
or 2 if you want to obtain the positions now (default -> 1 [press Enter])"<<endl;

  getline( cin, ped_qstn );
  while ( (ped_qstn!="1") && (ped_qstn!="2") && (!ped_qstn.empty()) )
  {
    cout<<"Please type 1 or 2 or press Enter for default(1)"<<endl;
    getline( cin, ped_qstn );
  }
  if ( ped_qstn.empty() ) 
  {
    ped_qstn="1";
  }
  cout<<"You choose option  "<<ped_qstn<<endl;
  if (ped_qstn=="1") {cout<<"DataBase directory is "<<DB_path<<endl;}

  cout<<"Press Enter to continue"<<endl;
  system("read");
  if (ped_qstn=="1") read_db();
  if (ped_qstn=="2") fit_pedestals();
  set_ped_and_coef();
//  draw_xy();
//  minimization_matrix();
//  calib_calc();
  draw_calibrated();
}


int x_to_n(double x_tmp)
{
  int nn=-99;
  for (int i=0; i<17; i++)
  {
    if ( (xmin[i]<x_tmp) && (x_tmp<xmax[i]) )
    {
      nn=i;
      break;
    }
  }
  return nn;
}

int x1_to_n(double x_tmp)
{
  int nn=-99;
  double xcen[17]={100., 0.82,0.675, 0.53, 0.385,0.24, 0.095,-0.05, -0.195,-0.34, -0.485,-0.63, -0.775,-0.92, -1.065,-1.21,-100.};
  for (int i=0; i<17; i++)
  {
    if ( ((xcen[i]-0.0725)<x_tmp) && (x_tmp<(xcen[i]+0.0725)) )
    {
      nn=i;
      break;
    }
  }
  return nn;
}


void read_db()
{
  int n=34;
  int n2=34;

  string ps_db;
  string sh_db;
  ps_db  = DB_path + "db_L.prl1.dat";
  sh_db  = DB_path + "db_L.prl2.dat";
  cout<<DB_path <<endl;
  cout<<ps_db <<endl;
  cout<<sh_db <<endl;

  string line;

  ifstream ps_file(ps_db.c_str(),ios::in);
  if (ps_file.is_open())
  {
    while (! ps_file.eof() )
    {
      getline(ps_file,line);
      int i_l=0;
      if (line.substr(0,13) == "ADC pedestals")
      {
        std::ofstream output;
        output.open("temp.txxt");
        getline(ps_file,line);
        while((line.substr(0,6) != "Energy") && (! ps_file.eof() ))
        {
          output<<line<<endl;
          i_l++;
          getline(ps_file, line);
        }
        cout<<"i_l="<<i_l<<endl;
        output.close();
        ifstream infile;
        infile.open("temp.txxt");
        for(int i = 0; i < n; i++) infile >> ped_prl1[i];
        infile.close();
      }


      if(line.substr(0,18) == "Energy calibration")
      {
        std::ofstream output;
        output.open("temp.txxt");
        getline(ps_file,line);
        while(! ps_file.eof() )
        {
          output<<line<<endl;
          i_l++;
          getline(ps_file, line);
        }
        cout<<"i_l="<<i_l<<endl;
        output.close();
        ifstream infile;
        infile.open("temp.txxt");
        for(int i = 0; i < n; i++) infile >> prl1_coef_db[i];
        infile.close();
      }

    }
  }
  else 
  {
    cout<<"missing PRL1 DB input file:"<<ps_db<<endl;
  }
  for (int i=0; i<n; i++)
  {
    cout<<i<<" ps   "<<ped_prl1[i]<<"   "<<prl1_coef_db[i]<<endl;
  }
  ps_file.close();



  ifstream sh_file(sh_db.c_str(),ios::in);
  if (sh_file.is_open())
  {
    while (! sh_file.eof() )
    {
      getline(sh_file,line);
      int i_l=0;
      if (line.substr(0,13) == "ADC pedestals")
      {
        std::ofstream output;
        output.open("temp.txxt");
        cout<<line<<endl;
        getline(sh_file,line);
        while((line.substr(0,6) != "Energy") && (! sh_file.eof() ))
        {
          output<<line<<endl;
          i_l++;
          getline(sh_file, line);
        }
        cout<<"i_l="<<i_l<<endl;
        output.close();
        ifstream infile;
        infile.open("temp.txxt");
        for(int i = 0; i < n2; i++) infile >> ped_prl2[i];
        infile.close();
      }

      if(line.substr(0,18) == "Energy calibration")
      {
        std::ofstream output;
        output.open("temp.txxt");
        cout<<line<<endl;
        getline(sh_file,line);
        while(! sh_file.eof() )
        {
          output<<line<<endl;
          i_l++;
          getline(sh_file, line);
        }
        cout<<"i_l="<<i_l<<endl;
        output.close();
        ifstream infile;
        infile.open("temp.txxt");
        for(int i = 0; i < n2; i++) infile >> prl2_coef_db[i];
        infile.close();
      }
    }
  }
  else
  {
    cout<<"missing PRL2 DB input file:"<<sh_db<<endl;
  }

  for (int i=0; i<n2; i++)
  {
    cout<<i<<" sh   "<<ped_prl2[i]<<"   "<<prl2_coef_db[i]<<endl;
  }
  sh_file.close();
}



void fit_pedestals()
{
  {

    TChain *T1 = new TChain("T");
    for (int i_r=0; i_r<1; i_r++)
//    for (int i_r=0; i_r<n_runs; i_r++)
    {
      char rname[256];
      sprintf (rname, "/adaqfs/home/a-onl/gmp/replay/Rootfiles/left_gmp_%d*.root", runs[i_r]);
      cout<<"file(s):"<<rname<<"  are added"<<endl;
      T1->Add(rname);
    }
    int nentries1 = T1->GetEntries();
    cout<<"total "<<nentries1<<" events"<<endl;

    TCanvas * cc1 = new TCanvas("cc1", "cc1");
    cc1->SetLogy();
    cc1->SetTickx();
    cc1->SetGridx();
    for (int i=0; i<34; i++)
//    for (int i=0; i<1; i++)
    {
      char name1[256];
      char name2[256];
      TH1D *h1 = new TH1D("h1","h1",1300, -100, 1200);
      TH1D *h2 = new TH1D("h2","h2",1300, -100, 1200);
//      sprintf (name1, "\"x+4.57*th\">>h1,\"(prl1[%d]-%f)>400\"", i,ped_prl1[i]);
//      cout<<name1<<endl;
//      cin>>name1;
      sprintf (name1, "(L.prl1.a[%d]-%f)>>h1", i, ped_prl1[i]);
      sprintf (name2, "(L.prl2.a[%d]-%f)>>h2", i, ped_prl2[i]);
      cout<<name1<<endl;
      cout<<name2<<endl;
      T1->Draw(name1);
      T1->Draw(name2);
      {
        int binmax = h1->GetMaximumBin();
        double x1 = h1->GetXaxis()->GetBinCenter(binmax);
        TH1D *htmp = (TH1D*)h1->Clone("htmp");
        htmp->GetXaxis()->SetRange(binmax-20, binmax+20);
        double ped_fit_range = 2.5*htmp->GetRMS();
        h1->Fit("gaus", "R", "", x1-ped_fit_range, x1+ped_fit_range);
        h1->Fit("gaus", "R", "", gaus->GetParameter(1)-2.*gaus->GetParameter(2), gaus->GetParameter(1)+2.*gaus->GetParameter(2));
        delete htmp;
        ped_prl1[i]=gaus->GetParameter(1);
      }

      binmax = h2->GetMaximumBin();
      double x2 = h2->GetXaxis()->GetBinCenter(binmax);
      TH1D *htmp = (TH1D*)h2->Clone("htmp");
      htmp->GetXaxis()->SetRange(binmax-20, binmax+20);
      ped_fit_range = 2.5*htmp->GetRMS();
      h2->Fit("gaus", "R", "", x2-ped_fit_range, x2+ped_fit_range);
      h2->Fit("gaus", "R", "", gaus->GetParameter(1)-2.*gaus->GetParameter(2), gaus->GetParameter(1)+2.*gaus->GetParameter(2));
      delete htmp;
      ped_prl2[i]=gaus->GetParameter(1);

      h1->Draw();
      h2->Draw("same");
      char gifname[256];
      sprintf (gifname, "ped_fit_ch_%d.gif", i);
      cc1->SaveAs(gifname);
      delete h1;
      delete h2;
    }

    for (int i=0; i<33; i++)
    {
      cout<<"ped_prl1["<<i<<"]="<<ped_prl1[i]<<";"<<endl;
    }
    for (int i=0; i<48; i++)
    {
      cout<<"ped_prl2["<<i<<"]="<<ped_prl2[i]<<";"<<endl;
    }
  }
}


void draw_xy()
{
  Double_t trig;
  Double_t x[100];
  Double_t th[100];
  Double_t y[100];
  Double_t ph[100];
  Double_t p[100];
  Double_t chi2[100];
  Double_t ndof[100];
  Double_t scer;
  Double_t prl1[34], prl2[34];
  Double_t e1, e2;
  {

    TChain *T1 = new TChain("T");
    for (int i_r=0; i_r<1; i_r++)
//    for (int i_r=0; i_r<n_runs; i_r++)
    {
      char rname[256];
//      sprintf (rname, "/adaqfs/home/a-onl/apex/vardan/calo/calo_tree_run_%d.root", runs[i_r]);
      sprintf (rname, "/adaqfs/home/a-onl/gmp/replay/Rootfiles/left_gmp_%d*.root", runs[i_r]);
      cout<<"file(s):"<<rname<<"  are added"<<endl;
      T1->Add(rname);
    }
    int nentries1 = T1->GetEntries();
    cout<<"total "<<nentries1<<" events"<<endl;

    T1->SetBranchStatus("*",0);
    T1->SetBranchStatus("L.prl1.e",1);
    T1->SetBranchStatus("L.prl2.e",1);
    T1->SetBranchStatus("L.cer.asum_c",1);
    T1->SetBranchStatus("L.tr.n",1);
    T1->SetBranchStatus("L.tr.p",1);
    T1->SetBranchStatus("L.tr.x",1);
    T1->SetBranchStatus("L.tr.y",1);
    T1->SetBranchStatus("L.tr.ph",1);
    T1->SetBranchStatus("L.tr.th",1);
    T1->SetBranchStatus("L.tr.chi2",1);
    T1->SetBranchStatus("L.tr.ndof",1);
    T1->SetBranchStatus("L.prl1.a",1);
    T1->SetBranchStatus("L.prl2.a",1);

    T1->SetBranchAddress("L.prl1.e",&e1);
    T1->SetBranchAddress("L.prl2.e",&e2);
    T1->SetBranchAddress("L.cer.asum_c",&scer);
    T1->SetBranchAddress("L.tr.p",&p);
    T1->SetBranchAddress("L.tr.x",&x);
    T1->SetBranchAddress("L.tr.y",&y);
    T1->SetBranchAddress("L.tr.ph",&ph);
    T1->SetBranchAddress("L.tr.th",&th);
    T1->SetBranchAddress("L.tr.chi2",&chi2);
    T1->SetBranchAddress("L.tr.ndof",&ndof);
    T1->SetBranchAddress("L.prl1.a",&prl1);
    T1->SetBranchAddress("L.prl2.a",&prl2);

    TCanvas * cc1 = new TCanvas("cc1", "cc1", 20, 20, 2400, 800);
    cc1->Divide(2,2);
    for (int i=8; i<9; i++)
    {
      char cut1[256];
      char cut1_left[256];
      char cut1_right[256];
      char cut2[256];
      char cut2_left[256];
      char cut2_right[256];
      TH1D *h1 = new TH1D("h1","h1",1500, -1.50, 1.50);
      TH1D *h1_left = new TH1D("h1_left","h1_left",1500, -1.50, 1.50);
      TH1D *h1_right = new TH1D("h1_right","h1_right",1500, -1.50, 1.50);
      TH1D *h2 = new TH1D("h2","h2",1600, -.4, .4);
      TH1D *h3 = new TH1D("h3","h3",1500, -1.50, 1.50);
      TH1D *h3_left = new TH1D("h3_left","h3_left",1500, -1.50, 1.50);
      TH1D *h3_right = new TH1D("h3_right","h3_right",1500, -1.50, 1.50);
      TH1D *h4 = new TH1D("h4","h4",1600, -.4, .4);
      h3_left->SetLineColor(2);
      h3->SetLineColor(1);
      h3_right->SetLineColor(4);

      h1_left->SetLineColor(2);
      h1->SetLineColor(1);
      h1_right->SetLineColor(4);

      sprintf (cut1, "(L.prl1.a[%d]-%f)>400 && L.tr.n==1 && (L.prl1.e+L.prl2.e)>500 && L.cer.asum_c>500 && L.tr.chi2[0]/L.tr.ndof[0]<40.", i,ped_prl1[i]);
      sprintf (cut1_left, "(L.prl1.a[%d]-%f)>400 && L.tr.n==1 && (L.prl1.e+L.prl2.e)>500 && L.cer.asum_c>500 && L.tr.chi2[0]/L.tr.ndof[0]<40.", i-1,ped_prl1[i-1]);
      sprintf (cut1_right, "(L.prl1.a[%d]-%f)>400 && L.tr.n==1 && (L.prl1.e+L.prl2.e)>500 && L.cer.asum_c>500 && L.tr.chi2[0]/L.tr.ndof[0]<40.", i+1,ped_prl1[i+1]);
      sprintf (cut2, "(L.prl1.a[%d]-%f)>400 && L.tr.n==1 && (L.prl1.e+L.prl2.e)>500 && L.cer.asum_c>500 && L.tr.chi2[0]/L.tr.ndof[0]<40.", i+17,ped_prl1[i+17]);
      sprintf (cut2_left, "(L.prl1.a[%d]-%f)>400 && L.tr.n==1 && (L.prl1.e+L.prl2.e)>500 && L.cer.asum_c>500 && L.tr.chi2[0]/L.tr.ndof[0]<40.", i+17-1,ped_prl1[i+17-1]);
      sprintf (cut2_right, "(L.prl1.a[%d]-%f)>400 && L.tr.n==1 && (L.prl1.e+L.prl2.e)>500 && L.cer.asum_c>500 && L.tr.chi2[0]/L.tr.ndof[0]<40.", i+17+1,ped_prl1[i+17+1]);

      cc1->cd(1);
      T1->Draw("L.tr.x[0]+4.57*L.tr.th[0]>>h1", cut1);
      T1->Draw("L.tr.x[0]+4.57*L.tr.th[0]>>h1_left", cut1_left);
      T1->Draw("L.tr.x[0]+4.57*L.tr.th[0]>>h1_right", cut1_right);
      T1->Draw("L.tr.y[0]+4.57*L.tr.ph[0]>>h2", cut1);
      T1->Draw("L.tr.x[0]+4.57*L.tr.th[0]>>h3", cut2);
      T1->Draw("L.tr.x[0]+4.57*L.tr.th[0]>>h3_left", cut2_left);
      T1->Draw("L.tr.x[0]+4.57*L.tr.th[0]>>h3_right", cut2_right);
      T1->Draw("L.tr.y[0]+4.57*L.tr.ph[0]>>h4", cut2);
//      T1->Draw(name2);
      gPad->SetTickx();
      gPad->SetLogy();
      gPad->SetGridx();
      h1->GetXaxis()->SetRange(950-int(i*72.5), 1450-int(i*72.5));
      h1->Draw();
      h1_left->Draw("same");
      h1_right->Draw("same");



      double xh1max=h1->GetMaximum();
      double xh3max=h3->GetMaximum();
      double yh2max=h2->GetMaximum();
      double yh4max=h4->GetMaximum();
      TLine *l1h1 = new TLine(xmin[i],0,xmin[i],xh1max);
      TLine *l2h1 = new TLine(xmax[i],0,xmax[i],xh1max);
      l1h1->SetLineColor(1);
      l2h1->SetLineColor(1);
      l1h1->SetLineStyle(2);
      l2h1->SetLineStyle(2);
      l1h1->SetLineWidth(2);
      l2h1->SetLineWidth(2);
      l1h1->Draw();
      l2h1->Draw();



      cc1->cd(2);
      gPad->SetLogy();
      gPad->SetTickx();
      gPad->SetGridx();
      h2->Draw();

      TLine *l1h2 = new TLine(ymin,0,ymin,yh2max);
      TLine *l2h2 = new TLine(ymax,0,ymax,yh2max);
      l1h2->SetLineColor(1);
      l2h2->SetLineColor(1);
      l1h2->SetLineStyle(2);
      l2h2->SetLineStyle(2);
      l1h2->SetLineWidth(2);
      l2h2->SetLineWidth(2);
      l1h2->Draw();
      l2h2->Draw();




      cc1->cd(3);
      gPad->SetLogy();
      gPad->SetTickx();
      gPad->SetGridx();
      h3->GetXaxis()->SetRange(950-int(i*72.5), 1450-int(i*72.5));
      h3->Draw();
      h3_left->Draw("same");
      h3_right->Draw("same");
      TLine *l1h3 = new TLine(xmin[i],0,xmin[i],xh3max);
      TLine *l2h3 = new TLine(xmax[i],0,xmax[i],xh3max);
      l1h3->SetLineColor(1);
      l2h3->SetLineColor(1);
      l1h3->SetLineStyle(2);
      l2h3->SetLineStyle(2);
      l1h3->SetLineWidth(2);
      l2h3->SetLineWidth(2);
      l1h3->Draw();
      l2h3->Draw();

      cc1->cd(4);
      gPad->SetLogy();
      gPad->SetTickx();
      gPad->SetGridx();
      h4->Draw();
      TLine *l1h4 = new TLine(ymin,0,ymin,yh4max);
      TLine *l2h4 = new TLine(ymax,0,ymax,yh4max);
      l1h4->SetLineColor(1);
      l2h4->SetLineColor(1);
      l1h4->SetLineStyle(2);
      l2h4->SetLineStyle(2);
      l1h4->SetLineWidth(2);
      l2h4->SetLineWidth(2);
      l1h4->Draw();
      l2h4->Draw();

      char gifname[256];
      char pdfname[256];
      sprintf (gifname, "ch_xy_%d.gif", i);
      sprintf (pdfname, "ch_xy_%d.pdf", i);
      cc1->SaveAs(gifname);
      cc1->SaveAs(pdfname);
      delete h1;
      delete h2;
      delete h3;
      delete h4;
      delete h1_left;
      delete h3_left;
      delete h1_right;
      delete h3_right;
    }
  }
}


void minimization_matrix()
{
  Double_t trig;
  Double_t x[100];
  Double_t th[100];
  Double_t y[100];
  Double_t ph[100];
  Double_t p[100];
  Double_t chi2[100];
  Double_t ndof[100];
  Double_t scer[100];
  Double_t prl1[34], prl2[34];
  Double_t e1, e2;
  Double_t n_tr;
  unsigned long int good_el=0;


  {
    TChain *T1 = new TChain("T");
    for (int i_r=0; i_r<2; i_r++)
//    for (int i_r=0; i_r<n_runs; i_r++)
    {
      char rname[256];
      sprintf (rname, "/adaqfs/home/a-onl/gmp/replay/Rootfiles/left_gmp_%d*.root", runs[i_r]);
      cout<<"file(s):"<<rname<<"  are added"<<endl;
      T1->Add(rname);
    }
    int nentries1 = T1->GetEntries();
    cout<<"total "<<nentries1<<" events"<<endl;

    T1->SetBranchStatus("*",0);
    T1->SetBranchStatus("L.prl1.e",1);
    T1->SetBranchStatus("L.prl2.e",1);
    T1->SetBranchStatus("L.cer.asum_c",1);
    T1->SetBranchStatus("L.tr.n",1);
    T1->SetBranchStatus("L.tr.p",1);
    T1->SetBranchStatus("L.tr.x",1);
    T1->SetBranchStatus("L.tr.y",1);
    T1->SetBranchStatus("L.tr.ph",1);
    T1->SetBranchStatus("L.tr.th",1);
    T1->SetBranchStatus("L.tr.chi2",1);
    T1->SetBranchStatus("L.tr.ndof",1);
    T1->SetBranchStatus("L.prl1.a",1);
    T1->SetBranchStatus("L.prl2.a",1);

    T1->SetBranchAddress("L.prl1.e",&e1);
    T1->SetBranchAddress("L.prl2.e",&e2);
    T1->SetBranchAddress("L.cer.asum_c",&scer);
    T1->SetBranchAddress("L.tr.n",&n_tr);
    T1->SetBranchAddress("L.tr.p",&p);
    T1->SetBranchAddress("L.tr.x",&x);
    T1->SetBranchAddress("L.tr.y",&y);
    T1->SetBranchAddress("L.tr.ph",&ph);
    T1->SetBranchAddress("L.tr.th",&th);
    T1->SetBranchAddress("L.tr.chi2",&chi2);
    T1->SetBranchAddress("L.tr.ndof",&ndof);
    T1->SetBranchAddress("L.prl1.a",&prl1);
    T1->SetBranchAddress("L.prl2.a",&prl2);

    double A[68];
    for (int i = 0; i<68; i++)
    {
      S0[i] = 0.;
      A[i] = 0.;
      for (int j = 0; j<68; j++)
      {
        S[i][j] = 0.;
      }
    }

//    if (0)
    {
     for (int i_evt=0; i_evt<nentries1/20; i_evt++)
     {
      T1->GetEntry(i_evt);
      if ( (scer>500) && (e1+e2)>500. && (n_tr==1) && (chi2[0]/ndof[0] < 40.) && ((y[0]+4.57*ph[0])>-0.16) && ((y[0]+4.57*ph[0])<0.12) )
      {
        good_el++;
        double sum_tmp=0;
        double Ai[68]={0};
        for (int j=0; j<34; j++) Ai[j]=0.;
        int nn=x_to_n(x[0]+4.57*th[0]);
        if (nn>=0)
        {
//          for (int j=0; j<34; j++)
          for (int j=nn-1; j<=nn+1; j++)
          {
            Ai[j]=(prl1[j]-ped_prl1[j])/1000.;
            Ai[j+17]=(prl1[j+17]-ped_prl1[j+17])/1000.;
            Ai[j+34]=(prl2[j]-ped_prl2[j])/1000.;
            Ai[j+34+17]=(prl2[j+17]-ped_prl2[j+17])/1000.;

            sum_tmp+=(Ai[j]+Ai[j+34]);
          }
        }

        for (int i=0; i<68; i++)
        {
          S0[i] += p[0]*Ai[i];
          for (int j=0; j<68; j++)
          {
            S[i][j] += Ai[i]*Ai[j];
          }
        }
        if ( (i_evt % 1000) == 0 )
        {
          if (nn>=0)
          {
            double e11=Ai[nn]+Ai[nn+17]+Ai[nn-1]+Ai[nn-1+17]+Ai[nn+1]+Ai[nn+1+17];
            double e12=Ai[nn]+Ai[nn+17]+Ai[nn-1]+Ai[nn-1+17]+Ai[nn+1]+Ai[nn+1+17]+Ai[nn+34]+Ai[nn+17+34]+Ai[nn-1+34]+Ai[nn-1+17+34]+Ai[nn+1+34]+Ai[nn+1+17+34];
            cout<<i_evt<<"/"<<nentries1<<" events are analyzed, selected "<<good_el<<" electrons "<<endl;
            for (int i=0; i<68; i++)
            {
              if (S0[i]==0) 
              {
                cout<<"below are the blocks without electrons, we need electrons in these blocks too, otherwise the matrixes will be singular."<<endl;
                break;
              }
            }

            for (int i=0; i<68; i++)
            {
              if (S0[i]==0) cout<<i<<","<<endl;
            }
          }
        }
      }
     }
    }
//    ofstream mat("matrix.txt", std::ios_base::app);
    ofstream mat("matrix.txt");
    if (mat.is_open())
    {
      for (int i_sig=0; i_sig<68; i_sig++)
      {
        for (int j_sig=0; j_sig<68; j_sig++)
        {
          mat<<setw(4)<<j_sig<<setw(4)<<i_sig<<setw(14)<<S[i_sig][j_sig];
        }
        mat<<endl;
      }
    }
    mat.close();

    ofstream col("col.txt");
    if (col.is_open())
    {
      for (int i_sig=0; i_sig<68; i_sig++)
      {
        col<<setw(4)<<i_sig<<setw(14)<<S0[i_sig]<<endl;
      }
    }
    col.close();
  }

}


void calib_calc()
{
  const Int_t nrVar  = 68;
  TMatrixD A(68,68);
  TMatrixD S_mat(68,1);
  for(int i=0;i<68;i++)
  {
    S_mat(i,0)=S0[i];
    for(int j=0;j<68;j++)
    {
      A(i,j)=S[i][j];
    }
  }

  TMatrixD ST(TMatrixD::kTransposed,S_mat);

  TMatrixD A_1 = A;
  double d;
  A_1.Invert(&d);
  TMatrixD C = (A_1*A);

  TMatrixD Coef = (A_1*S_mat);


  ofstream ooo("matrix_1.txt");


  if (ooo.is_open()) 
  {
      for (int i_sig=0; i_sig<68; i_sig++)
      {
        for (int j_sig=0; j_sig<68; j_sig++)
        {
          ooo<<setw(14)<<A(i_sig,j_sig);
        }
//        cout<<ST(i_sig,0)<<endl;
        ooo<<endl;
      }
  }
  ooo<<endl;
  ooo<<endl;




  if (ooo.is_open()) 
  {
      for (int i_sig=0; i_sig<68; i_sig++)
      {
        for (int j_sig=0; j_sig<68; j_sig++)
        {
          ooo<<setw(14)<<A_1(i_sig,j_sig);
        }
        ooo<<endl;
      }
  }
  ooo<<endl;
  ooo<<endl;


  if (ooo.is_open()) 
  {
      for (int i_sig=0; i_sig<68; i_sig++)
      {
        for (int j_sig=0; j_sig<68; j_sig++)
        {
          ooo<<setw(14)<<C(i_sig,j_sig);
        }
        ooo<<endl;
      }
  }


  if (ooo.is_open()) 
  {
    for (int j_sig=0; j_sig<68; j_sig++)
    {
      ooo<<setw(14)<<ST(0,j_sig);
      ooo<<endl;
    }
  }


  for (int iblck=0; iblck<68; iblck++)
  {
    if (Coef(iblck,0) <=0 ) Coef(iblck,0) = 1.;
  }


  for (int iblck=0; iblck<68; iblck++)
  {
    if (iblck<34) ooo<<"prl1_coef["<<iblck<<"]="<<Coef(iblck,0)<<";"<<endl;
    if (iblck>=34) ooo<<"prl2_coef["<<iblck-34<<"]="<<Coef(iblck,0)<<";"<<endl;
  }

  double coef1_db[34] = {1.060, 1.029, 0.978, 1.034, 1.010, 0.968, 1.002, 0.983, 0.963, 1.046, 1.039, 1.073, 1.069, 1.059, 1.034,  1.105, 1.036, 1.034, 1.040, 0.938, 0.993, 1.052, 1.019, 1.031, 1.013, 1.061, 1.027, 1.058, 1.056, 1.083, 1.017, 1.154, 1.139, 1.036};
  double coef2_db[34] = {1.809, 1.735, 1.778, 1.643, 1.701, 1.729, 1.667, 1.771, 1.822, 1.819, 1.822, 1.810, 1.958, 1.935, 1.939, 2.047, 1.778, 1.729, 1.679, 1.671, 1.662, 1.689, 1.624, 1.680, 1.712, 1.780, 1.843, 1.706, 1.766, 1.724, 1.800, 1.870, 1.964, 1.778};
  for (int ieta=0; ieta<68; ieta++)
  {
    if (ieta<34) cout<<ieta<<setw(15)<<Coef(ieta,0)<<setw(15)<<prl1_coef_db[ieta]<<endl;
    if (ieta>=34) cout<<ieta-34<<setw(15)<<Coef(ieta,0)<<setw(15)<<prl2_coef_db[ieta-34]<<endl;
  }
  ooo.close();
}


void draw_calibrated()
{
  gStyle->SetOptStat(1);
  TH1D *hh = new TH1D("h","h",80, 0.6, 1.4);
  TProfile2D *prof = new TProfile2D("prof","prof",250, -1.5, 1.5, 200, -0.5, 0.5);
  Double_t trig;
  Double_t x[100];
  Double_t th[100];
  Double_t y[100];
  Double_t ph[100];
  Double_t p[100];
  Double_t chi2[100];
  Double_t ndof[100];
  Double_t scer[100];
  Double_t prl1[34], prl2[34];
  Double_t e1, e2;
  Double_t n_tr;
  unsigned long int good_el=0;
  {
    TChain *T1 = new TChain("T");
    for (int i_r=0; i_r<2; i_r++)
//    for (int i_r=0; i_r<n_runs; i_r++)
    {
      char rname[256];
      sprintf (rname, "/adaqfs/home/a-onl/gmp/replay/Rootfiles/left_gmp_%d*.root", runs[i_r]);
      cout<<"file(s):"<<rname<<"  are added"<<endl;
      T1->Add(rname);
    }
    int nentries1 = T1->GetEntries();
    cout<<"total "<<nentries1<<" events"<<endl;

    T1->SetBranchStatus("*",0);
    T1->SetBranchStatus("L.prl1.e",1);
    T1->SetBranchStatus("L.prl2.e",1);
    T1->SetBranchStatus("L.cer.asum_c",1);
    T1->SetBranchStatus("L.tr.n",1);
    T1->SetBranchStatus("L.tr.p",1);
    T1->SetBranchStatus("L.tr.x",1);
    T1->SetBranchStatus("L.tr.y",1);
    T1->SetBranchStatus("L.tr.ph",1);
    T1->SetBranchStatus("L.tr.th",1);
    T1->SetBranchStatus("L.tr.chi2",1);
    T1->SetBranchStatus("L.tr.ndof",1);
    T1->SetBranchStatus("L.prl1.a",1);
    T1->SetBranchStatus("L.prl2.a",1);

    T1->SetBranchAddress("L.prl1.e",&e1);
    T1->SetBranchAddress("L.prl2.e",&e2);
    T1->SetBranchAddress("L.cer.asum_c",&scer);
    T1->SetBranchAddress("L.tr.n",&n_tr);
    T1->SetBranchAddress("L.tr.p",&p);
    T1->SetBranchAddress("L.tr.x",&x);
    T1->SetBranchAddress("L.tr.y",&y);
    T1->SetBranchAddress("L.tr.ph",&ph);
    T1->SetBranchAddress("L.tr.th",&th);
    T1->SetBranchAddress("L.tr.chi2",&chi2);
    T1->SetBranchAddress("L.tr.ndof",&ndof);
    T1->SetBranchAddress("L.prl1.a",&prl1);
    T1->SetBranchAddress("L.prl2.a",&prl2);
    {
      for (int i_evt=0; i_evt<nentries1/100; i_evt++)
      {
        T1->GetEntry(i_evt);
        if ( ((y[0]+4.58*ph[0])>-0.2) && ((y[0]+4.58*ph[0])<0.15) )  //change to x1cen
        if ( (scer>500) && (e1+e2)>500. && (n_tr==1) && (chi2[0]/ndof[0] < 40.) )
        {
          good_el++;
          int nn=x1_to_n(x[0]+4.58*th[0]);
          int nn2=x1_to_n(x[0]+4.77*th[0]);
//          cout<<nn<<"    "<<nn2<<endl;
          if ( (nn>=0) && (nn2>=0) )
          {
            double E_ps = prl1_coef[nn]*(prl1[nn]-ped_prl1[nn]) + prl1_coef[nn-1]*(prl1[nn-1]-ped_prl1[nn-1]) + prl1_coef[nn+1]*(prl1[nn+1]-ped_prl1[nn+1]) + prl1_coef[17+nn]*(prl1[17+nn]-ped_prl1[17+nn]) + prl1_coef[17+nn-1]*(prl1[17+nn-1]-ped_prl1[17+nn-1]) + prl1_coef[17+nn+1]*(prl1[17+nn+1]-ped_prl1[17+nn+1]);
            double E_sh = prl2_coef[nn2]*(prl2[nn2]-ped_prl2[nn2]) + prl2_coef[nn2-1]*(prl2[nn2-1]-ped_prl2[nn2-1]) + prl2_coef[nn2+1]*(prl2[nn2+1]-ped_prl2[nn2+1]) + prl2_coef[17+nn2]*(prl2[17+nn2]-ped_prl2[17+nn2]) + prl2_coef[17+nn2-1]*(prl2[17+nn2-1]-ped_prl2[17+nn2-1]) + prl2_coef[17+nn2+1]*(prl2[17+nn2+1]-ped_prl2[17+nn2+1]);

            if (nn==8) hh->Fill(0.001*(E_ps+E_sh)/p[0]);
            prof->Fill(x[0]+4.58*th[0],y[0]+4.58*ph[0],0.001*(E_ps+E_sh)/p[0]);
            if ( (i_evt % 500) == 0 )
            {
              cout<<i_evt<<"/"<<nentries1<<" events are analyzed, selected "<<good_el<<" electrons "<<endl;
            }
          }
        }
      }
    }
  }
  TCanvas *cres = new TCanvas("cres","cres");
  
  hh->SetTitle("E/P");
  hh->Draw();
  hh->Fit("gaus","R","",0.9, 1.1);
  hh->Draw();
  cres->Print("plots/calib_results1.pdf");
  TCanvas *c2 = new TCanvas("c2","c2");

  c2->cd();
  prof->SetTitle("E/P vs [XY]");
  prof->GetXaxis()->SetTitle("X [m]");
  prof->GetYaxis()->SetTitle("Y [m]");
  prof->Draw("colz");
  c2->Print("plots/calib_results2.pdf");
}


void  set_ped_and_coef()
{
  prl1_coef[0]=1.65662;
  prl1_coef[1]=1.52878;
  prl1_coef[2]=1.32318;
  prl1_coef[3]=1.36788;
  prl1_coef[4]=1.41576;
  prl1_coef[5]=1.32466;
  prl1_coef[6]=1.29216;
  prl1_coef[7]=1.29195;
  prl1_coef[8]=1.28932;
  prl1_coef[9]=1.41479;
  prl1_coef[10]=1.35722;
  prl1_coef[11]=1.39455;
  prl1_coef[12]=1.42094;
  prl1_coef[13]=1.41418;
  prl1_coef[14]=1.36995;
  prl1_coef[15]=1.51102;
  prl1_coef[16]=1.62223;
  prl1_coef[17]=1.47845;
  prl1_coef[18]=1.41087;
  prl1_coef[19]=1.36465;
  prl1_coef[20]=1.35352;
  prl1_coef[21]=1.32109;
  prl1_coef[22]=1.54929;
  prl1_coef[23]=1.37512;
  prl1_coef[24]=1.2761;
  prl1_coef[25]=1.35022;
  prl1_coef[26]=1.28504;
  prl1_coef[27]=1.37307;
  prl1_coef[28]=1.35026;
  prl1_coef[29]=1.37806;
  prl1_coef[30]=1.22703;
  prl1_coef[31]=1.43184;
  prl1_coef[32]=1.5453;
  prl1_coef[33]=1.65361;
  prl2_coef[0]=1.11148;
  prl2_coef[1]=1.64149;
  prl2_coef[2]=1.74065;
  prl2_coef[3]=1.58269;
  prl2_coef[4]=1.63944;
  prl2_coef[5]=1.54678;
  prl2_coef[6]=1.45746;
  prl2_coef[7]=1.45205;
  prl2_coef[8]=1.6666;
  prl2_coef[9]=1.58165;
  prl2_coef[10]=1.54051;
  prl2_coef[11]=1.40413;
  prl2_coef[12]=1.56223;
  prl2_coef[13]=1.55677;
  prl2_coef[14]=1.35086;
  prl2_coef[15]=1.35908;
  prl2_coef[16]=1.08207;
  prl2_coef[17]=1.07258;
  prl2_coef[18]=1.61449;
  prl2_coef[19]=1.66872;
  prl2_coef[20]=1.49813;
  prl2_coef[21]=1.6295;
  prl2_coef[22]=1.56302;
  prl2_coef[23]=1.71303;
  prl2_coef[24]=1.57416;
  prl2_coef[25]=1.68275;
  prl2_coef[26]=1.68076;
  prl2_coef[27]=1.38941;
  prl2_coef[28]=1.49525;
  prl2_coef[29]=1.54456;
  prl2_coef[30]=1.70451;
  prl2_coef[31]=1.47066;
  prl2_coef[32]=1.62132;
  prl2_coef[33]=1.77275;

  ped_prl1[0]=300.718;
  ped_prl1[1]=303.12;
  ped_prl1[2]=233.214;
  ped_prl1[3]=271.434;
  ped_prl1[4]=381.004;
  ped_prl1[5]=311.102;
  ped_prl1[6]=361.582;
  ped_prl1[7]=332.561;
  ped_prl1[8]=354.62;
  ped_prl1[9]=302.845;
  ped_prl1[10]=329.546;
  ped_prl1[11]=260.039;
  ped_prl1[12]=342.72;
  ped_prl1[13]=261.466;
  ped_prl1[14]=306.077;
  ped_prl1[15]=409.377;
  ped_prl1[16]=247.116;
  ped_prl1[17]=362.269;
  ped_prl1[18]=361.402;
  ped_prl1[19]=286.094;
  ped_prl1[20]=344.099;
  ped_prl1[21]=594.085;
  ped_prl1[22]=435.275;
  ped_prl1[23]=503.523;
  ped_prl1[24]=543.523;
  ped_prl1[25]=474.599;
  ped_prl1[26]=306.515;
  ped_prl1[27]=503.55;
  ped_prl1[28]=327.718;
  ped_prl1[29]=486.659;
  ped_prl1[30]=427.293;
  ped_prl1[31]=473.84;
  ped_prl1[32]=489.733;
  ped_prl1[33]=311.381;
  ped_prl2[0]=447.052;
  ped_prl2[1]=505.891;
  ped_prl2[2]=504.066;
  ped_prl2[3]=289.85;
  ped_prl2[4]=353.378;
  ped_prl2[5]=569.276;
  ped_prl2[6]=422.831;
  ped_prl2[7]=515.17;
  ped_prl2[8]=385.565;
  ped_prl2[9]=316.517;
  ped_prl2[10]=384.541;
  ped_prl2[11]=476.199;
  ped_prl2[12]=531.838;
  ped_prl2[13]=506.904;
  ped_prl2[14]=469.014;
  ped_prl2[15]=563.195;
  ped_prl2[16]=291.492;
  ped_prl2[17]=521.267;
  ped_prl2[18]=518.977;
  ped_prl2[19]=491.875;
  ped_prl2[20]=495.477;
  ped_prl2[21]=565.679;
  ped_prl2[22]=503.363;
  ped_prl2[23]=540.188;
  ped_prl2[24]=538.351;
  ped_prl2[25]=501.172;
  ped_prl2[26]=494.453;
  ped_prl2[27]=527.327;
  ped_prl2[28]=490.522;
  ped_prl2[29]=599.65;
  ped_prl2[30]=544.519;
  ped_prl2[31]=530.592;
  ped_prl2[32]=579.587;
  ped_prl2[33]=249.392;
}
