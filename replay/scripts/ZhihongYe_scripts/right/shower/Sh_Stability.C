#include <iostream>
#include <iomanip>
using namespace std;
void Sh_Stability()
{ 

  // TString gDir = "/w/halla-2/e08014/Rootfiles/";
   TString gDir = "/GoFlex/Sample/";

  Int_t  run_number;    
  TString filename, kin, target, histo, outfile;

  double mean[30],sig[30], mean_err[30],sig_err[30],runm[30],sigx[30],mom[30];

  ifstream sample; sample.open("/w/halla-2/e08014/yez/HRS_Cali/Runlist/runsampleR.txt");
  ofstream myfile; myfile.open("R.sh.check_err.txt");

  TCut all_cut = "(DBB.evtypebits>>1&1)&&DBB.edtpr[0]==0&&R.cer.asum_c>50.&R.tr.p>0.5&& R.tr.p<4.5";
  // TCut all_cut="R.tr.p>0.5&& R.tr.p<4.5";
  //  TCut tcut="((DBB.evtypebits>>2)&1)&&(!((DBB.evtypebits>>1)&1))";
  TCut tcut="1";

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  
 float angle, t1r,t6r,t3r,t7r;

  myfile << "Cer#";
  myfile << "       Mom";
  myfile << "       Mean";
  myfile << "       Mean_Err";
  myfile << "       Sigma";
  myfile << "       Sigma_Err   "<<endl;
  

  TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
  // c1->Divide(4,3); 

  TString e_p;
  
  for(int i=0;i<29;i++){
    
    //    sample >> kin >> target >> run_number>> mom[i];
    sample >> kin >> target >> run_number >> mom[i] >> angle >> t1r >> t6r >> t3r >> t7r;
    
    cerr <<"#"<< i +1 <<" Checking "<<kin<<" "<<target<<" "<<run_number<<"..."<<endl;
    
    filename = gDir + Form("e08014_HRS_%d.root",run_number);
    TChain *T = new TChain("T");
    T->Add(filename);
    //   T->Add(Form("/w/halla-2/e08014/yez/Rootfiles/R-HRS/e07006_R_%d_1.root",run_number));
    //   T->Add(Form("/w/halla-2/e08014/yez/Rootfiles/R-HRS/e07006_R_%d_2.root",run_number));
    
    histo = "R-HRS Cerenkov Sum for "+kin+" "+target;
    histo = histo +Form(" Run%d",run_number);
    TH1F *tt1 = new TH1F("tt1",histo,250,0.5,1.5);
    
    c1->cd();
    
    e_p = "(R.sh.e + R.ps.e)/1000./R.gold.p>>tt1";
    T->Draw(e_p,all_cut);
    
    tt1->Fit("gaus","","",0.93,1.1);

    mean[i] = gaus->GetParameter(1);
    mean_err[i]=gaus->GetParError(1);
    sig[i] = gaus->GetParameter(2);
    sig_err[i] = gaus->GetParError(2);
    runm[i] = run_number;
    sigx[i]=0.0;

    cerr << "Mean = " <<mean[i] <<" Sigma =  "<<sig[i]<<endl;

    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(0) << runm[i] << "   ";
    myfile << setiosflags(ios::left) << setw(6) << setiosflags(ios::fixed) << setprecision(4) << mom[i] << "   ";
    myfile << setiosflags(ios::left) << setw(12) << setiosflags(ios::fixed) << setprecision(10) << mean[i] << "   ";
    myfile << setiosflags(ios::left) << setw(12) << setiosflags(ios::fixed) << setprecision(10) << mean_err[i] << "   ";
    myfile << setiosflags(ios::left) << setw(12) << setiosflags(ios::fixed) << setprecision(10) << sig[i] << "   ";
    myfile << setiosflags(ios::left) << setw(12) << setiosflags(ios::fixed) << setprecision(10) << sig_err[i] << "   "<<endl;
    
    outfile = kin+target+Form("_%d.png",run_number);
    
    c1->Update(); c1->Print(outfile);

  } 
}


void plot(){
  gStyle->SetOptStat(0);
  float mean[30],sig[30],mean_err[30],sig_err[30],runm[30],sigx[30],mom[30];
  
  TString a,b,c,d;

  ifstream sample; sample.open("R.sh.check_err.txt");

  // sample >> a >>b >> c >>d >>c>>d;

  for(int i=0;i<29;i++){
    sample >> runm[i] >> mom[i] >> mean[i] >> mean_err[i]  >> sig[i]>>sig_err[i];
    sigx[i]=0;
    sig[i]=sig[i]/mom[i];
    sig_err[i]=sig_err[i]/mom[i];

  }

  mean[29] = 100.;
  sig[29] = 30.;
  runm[29] = 4300;
    

  TCanvas *c3 = new TCanvas("c3","c3",1200,800);
  c3->Divide(1,2);
  c3->cd(1);
  TH2F *hh = new TH2F("hh","",200,2.45,3.1,100,0.5,1.5);
  hh->GetXaxis()->SetTitle("Momentum( GeV/c)");
  hh->GetXaxis()->CenterTitle();

  hh->GetYaxis()->SetTitle("E/P");
  hh->GetYaxis()->CenterTitle();

  hh->GetXaxis()->SetTitleFont(2);
  hh->GetYaxis()->SetTitleFont(2);
  hh->SetTitle("");
  hh->Draw();
  TGraphErrors *gr = new TGraphErrors(29,mom,mean,sigx,mean_err);
  gr->SetMarkerStyle(20);
  gr->Draw("P");

  c3->cd(2);
  TH2F *hf = new TH2F("hf","",30,2.45,3.1,100,0.0,0.03);
  hf->GetXaxis()->SetTitle("Momentum(GeV/c)");
  hf->GetXaxis()->CenterTitle();

  hf->GetYaxis()->SetTitle("#sigma/P");
  hf->GetYaxis()->CenterTitle();

  hf->GetXaxis()->SetTitleFont(2);
  hf->GetYaxis()->SetTitleFont(2);

  hf->Draw();
  TGraphErrors *gr1 = new TGraphErrors(29,mom,sig,sigx,sig_err);
  gr1->SetMarkerStyle(20);
  gr1->Draw("P");

  gStyle->SetOptFit(0);
  TF1 *sp = new TF1("fit",sigmap,2.5,3.06,3);
  sp->SetParNames("a","b","c");
  sp->SetParameters(0.01,0.01,0.0);
  sp->SetLineColor(3);
  
  sp->FixParameter(2,0.0);
  sp->FixParameter(0,0.0);

  gr1->Fit("fit","","",2.5,3.06);

  Float_t apar = fit->GetParameter(0);
  Float_t bpar = fit->GetParameter(1);
  Float_t cpar = fit->GetParameter(2);

  // TLatex *tex = new TLatex(0.4,0.8,Form("#frac{#sigma}{P}=%7.6f#oplus#frac{%7.6f}{#sqrt{P}}#oplus#frac{%7.6f}{P}",apar,bpar,cpar)) ;
  TLatex *tex = new TLatex(0.4,0.8,Form("#frac{#sigma}{P}=#frac{%7.6f}{#sqrt{P}}",bpar));
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->Draw();

  Float_t resol = sqrt(apar*apar+bpar*bpar+cpar*cpar)*100.;
  
  // TLatex *tex1 = new TLatex(0.4,0.2,Form("HRS-R Calorimeters Resolution = %4.2f",resol));
  // tex1->SetNDC();
  // tex1->Draw();
}

Double_t sigmap(double* x, double* par){

  return sqrt(par[0]*par[0]+par[1]/sqrt(x[0])*par[1]/sqrt(x[0])+par[2]/x[0]*par[2]/x[0]);

}
