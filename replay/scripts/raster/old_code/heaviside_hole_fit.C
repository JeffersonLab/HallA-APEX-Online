/*
* hole_fit.C
* Author: Tyler Hague
* Fits the Carbon Hole in order to calibrate the raster
* 
* Changelog:
* 23 March 2018 - Created
*/


#define RIGHT_ARM_CONDITION run>=90000
#define LEFT_ARM_CONDITION run<90000

#include "TString.h"

void heaviside_hole_fit(){
  Int_t run = 0;
  cout << "What run number would you like to calibrate with?    ";
  cin >> run;
  cout << endl << endl;
  if(run<=0){
    cout << "Invalid run number. Exiting." << endl << endl;
    return;
  }

  Double_t kx = 1.;
  Double_t ky = -1.;

  //Open Root File

  TChain *rootfile = new TChain("T");

  int i = 1;

  if(!gSystem->AccessPathName(TString::Format("/chafs1/work1/tritium/tmp_data/tritium_%d.root",run),kFileExists)){
    rootfile->Add(TString::Format("/chafs1/work1/tritium/tmp_data/tritium_%d.root",run));
    cout << "Added file: tritium_" << run << ".root" << endl;
  }else{
    cout << "Requested run has not been replayed. Exiting." << endl << endl;
    return;
  }

  while(!gSystem->AccessPathName(TString::Format("/chafs1/work1/tritium/tmp_data/tritium_%d_%d.root",run,i),kFileExists)){
    rootfile->Add(TString::Format("/chafs1/work1/tritium/tmp_data/tritium_%d_%d.root",run,i));
    cout << "Added file: tritium_" << run << "_" << i << ".root" << endl;
    i=i+1;
  }                      
 
  TString arm;
  if(RIGHT_ARM_CONDITION){
    arm="Rrb";
  }else if(LEFT_ARM_CONDITION){
    arm="Lrb";
  }
  //Define cuts

  TString cut = "(TMath::Abs(" + arm + ".BPMA.x)<100)&&((ev";
  if(RIGHT_ARM_CONDITION){
    cut += "Right";
  }else if(LEFT_ARM_CONDITION){
    cut += "Left";
  }
  cut += "dnew_r*0.0003299)>10)";
  cut += "&&(";
  if(RIGHT_ARM_CONDITION){
    cut += "R";
  }else if(LEFT_ARM_CONDITION){
    cut += "L";
  }
  cut += ".tr.n==1)";
  /*cut += "&&(";
  if(RIGHT_ARM_CONDITION){
    cut += "R";
  }else if(LEFT_ARM_CONDITION){
    cut += "L";
  }
  cut += ".cer.asum_c)>2000";*/
  cut += "&&(abs(";
  if(RIGHT_ARM_CONDITION){
    cut += "R";
  }else if(LEFT_ARM_CONDITION){
    cut += "L";
  }
  cut += ".tr.vz)<0.02)";

  //Define the fits and the plots

  TH2F *R1 = new TH2F("R1","Raster 1 Carbon Hole",50,45000,95000,100,20000,120000);
  TH2F *R2 = new TH2F("R2","Raster 2 Carbon Hole",25,45000,95000,50,20000,120000);

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  c1->cd(0);
  rootfile->Draw(arm + ".Raster.rawcur.y:" + arm + ".Raster.rawcur.x>>R1",TCut(cut),"colz");
  //ell_fit->Draw("cont1 same");
  c2->cd(0);
  rootfile->Draw(arm + ".Raster2.rawcur.y:" + arm + ".Raster2.rawcur.x>>R2",TCut(cut),"colz");
  //ell_fit2->Draw("cont1 same");

  TF2 *ell_fit = new TF2("ell_fit","[0]*((([1]*([2]-x))^2 + ([3]*([4]-y))^2) > 2 ? 0 : 1 ) + [5]",57000,87000,37000,110000);

  Double_t param[6] = {25,0.00008,72500,0.00003,80000, 1.};
  ell_fit->SetParameters(param);
  R1->Fit(ell_fit);
  
  TF2 *ell_fit2 = new TF2("ell_fit2","[0]*((([1]*([2]-x))^2 + ([3]*([4]-y))^2) > 1 ? 0 : 1 ) + [5]",R2->FindFirstBinAbove(0,1),R2->FindLastBinAbove(0,1),R2->FindFirstBinAbove(0,2),R2->FindLastBinAbove(0,2));

  Double_t param2[6] = {25,0.00008,72500,0.00003,80000, 1.};
  ell_fit2->SetParameters(param2);
  R2->Fit(ell_fit2);
}
