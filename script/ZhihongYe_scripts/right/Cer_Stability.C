#include <iostream>
#include <iomanip>
using namespace std;
void Cer_Stability()
{ 

  // TString gDir = "/w/halla-2/e08014/Rootfiles/";
  TString gDir = "/GoFlex/Sample/";
  
  Int_t  run_number;    
  TString filename, kin, target, histo, outfile;

  float mean[30],sig[30],runm[30],sigx[30];

  ifstream sample; sample.open("runsampleR.txt");
  //  ifstream sample; sample.open("/w/halla-2/e08014/yez/HRS_Cali/Run_Sample.txt");
  ofstream myfile; myfile.open("R.cer.check.txt");

  float peds[10] ={502,353,300,345,453,582,473,550,303,283};

  //  TCut time_cut = "DBB.evtypebits&(1<<1)&&R.cer.t>1000."; //Only T1 trigger, no EDTM pulse
    TCut time_cut = "R.cer.t>1000."; //Only T1 trigger, no EDTM pulse
  //  TCut time_cut = "DBB.evtypebits>>3&1"; //Only T1 trigger, no EDTM pulse
  TCut sh_cut = "R.sh.asum_c>1200.&&R.ps.asum_c>1400.";
  
  //  TCut all_cut = "DBB.evtypebits&(1<<1)&&DBB.edtpr[0]==0&&R.sh.asum_c>1200.&&R.ps.asum_c>1400.";
  TCut all_cut="1";
  TCut tcut="((DBB.evtypebits>>2)&1)&&(!((DBB.evtypebits>>1)&1))";
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  

  myfile << "Cer#";
  myfile << "       Mean   ";
  myfile << "       Sigma   "<<endl;

  float mom, angle, t1r,t6r,t3r,t7r;

  TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
  // c1->Divide(4,3); 
  
  for(int i=0;i<29;i++){
    
    sample >> kin >> target >> run_number >> mom >> angle >> t1r >> t6r >> t3r >> t7r;

    cerr <<"#"<< i +1 <<" Checking "<<kin<<" "<<target<<" "<<run_number<<"..."<<endl;

    filename = gDir + Form("e08014_HRS_%d.root",run_number);
    TChain *T = new TChain("T");
    T->Add(filename);
    //   T->Add(Form("/w/halla-2/e08014/yez/Rootfiles/R-HRS/e07006_R_%d_1.root",run_number));
    //   T->Add(Form("/w/halla-2/e08014/yez/Rootfiles/R-HRS/e07006_R_%d_2.root",run_number));
    
    histo = "R-HRS Cerenkov Sum for "+kin+" "+target;
    histo = histo +Form(" Run%d",run_number);
    TH1F *tt1 = new TH1F("tt1",histo,250,40,1000);
    
    c1->cd();
    T->Draw("R.cer.asum_c>>tt1",time_cut);
  
    tt1->Fit("gaus","","",50,150);

    mean[i] = gaus->GetParameter(1);
    sig[i] = gaus->GetParameter(2);
    runm[i] = run_number;
    sigx[i]=0.0;

    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(0) << runm[i] << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(0) << kin << "   ";  
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(0) << target << "   ";  
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(3) << mom << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(1) << mean[i] << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << sig[i] << "   "<<endl;
    
    outfile = kin+target+Form("_%d.png",run_number);
    
    c1->Update(); c1->Print(outfile);

  } 
 
  mean[29] = 100.;
  sig[29] = 30.;
  runm[29] = 4300;
    

  TCanvas *c3 = new TCanvas("c3","c3");
  TH2F *hh = new TH2F("hh","",30,3500,4300,100,0,150);
  hh->GetXaxis()->SetTitle("PMT number");
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->SetTitle("Peak value (ch)");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();
  TGraphErrors *gr = new TGraphErrors(30,runm,mean,sigx,sig);
  gr->SetMarkerStyle(20);
  gr->Draw("P");
}


void plot(){
  
  float meanR[30],sigR[30],runmR[30],sigxR[30],momR[30];
  float meanL[30],sigL[30],runmL[30],sigxL[30],momL[30];
  TString a,b,c, kinR, targetR, kinL, targetL;

  ifstream sampleR; sampleR.open("R.cer.check.txt");
  sampleR >> a >>b >> c;
  for(int i=0;i<29;i++){
    sampleR >> runmR[i] >> kinR >> targetR >> momR[i]>> meanR[i] >> sigR[i];
    sigxR[i]=0;
  }

  ifstream sampleL; sampleL.open("L.cer.check.txt");
  sampleL >> a >>b >> c;
  for(int i=0;i<29;i++){
    sampleL >> runmL[i] >> kinL >> targetL >> momL[i]>> meanL[i] >> sigL[i];
    sigxL[i]=0;
  }



  // mean[29] = 100.;
  // sig[29] = 30.;
  // runm[29] = 4300;
    
  gStyle->SetOptStat(0);
  TCanvas *c3 = new TCanvas("c3","c3",1200,800);
  c3->Divide(1,2);
  c3->cd(1);
  TH2F *hh = new TH2F("hh","",200,2.45,3.1,100,0,200);
  hh->GetXaxis()->SetTitle("Momentum(GeV/c)");
  hh->GetXaxis()->CenterTitle(); 
  hh->GetXaxis()->SetTitleFont(2);
  hh->GetYaxis()->SetTitleFont(2);
  hh->GetYaxis()->SetTitle("Peak_{s.p.e.}");
  hh->GetYaxis()->CenterTitle();
  hh->Draw();

  TGraphErrors *gr1 = new TGraphErrors(9,momL,meanL,sigxR,sigR);
  gr1->SetMarkerStyle(20);
  gr1->Draw("P");

  c3->cd(2);

  TH2F *h1 = new TH2F("h1","",200,2.45,3.1,100,0,200);
  h1->GetXaxis()->SetTitle("Momentum(GeV/c)");
  h1->GetXaxis()->CenterTitle(); 
  h1->GetXaxis()->SetTitleFont(2);
  h1->GetYaxis()->SetTitleFont(2);
  h1->GetYaxis()->SetTitle("Peak_{s.p.e.}");
  h1->GetYaxis()->CenterTitle();
  h1->Draw();

  TGraphErrors *gr2 = new TGraphErrors(9,momR,meanR,sigxL, sigL);
  gr2->SetMarkerStyle(20);
  gr2->Draw("P");

}
