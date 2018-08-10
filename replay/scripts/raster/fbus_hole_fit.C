/*
* hole_fit.C
* Author: Tyler Hague
* Fits the Carbon Hole in order to calibrate the raster
* 
* Changelog:
* 23 March 2018 - Created
* 12 July 2018  - Updated the position calibration. Mean raster is now mapped to mean BPM.
*/


#define RIGHT_ARM_CONDITION run>=90000
#define LEFT_ARM_CONDITION run<90000

#include "TString.h"

void fbus_hole_fit(Int_t r1=0, Int_t r2=0){
  Int_t run = r1;
  if(run==0){
    cout << "What run number would you like to calibrate with?    ";
    cin >> run;
    cout << endl << endl;
  }
  if(run<=0){
    cout << "Invalid run number. Exiting." << endl << endl;
    return;
  }

  Double_t kx = 1.;
  Double_t ky = -1.;

  //Open Root File

  TChain *rootfile = new TChain("T");

  int i = 1;

  if(!gSystem->AccessPathName(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d.root",run),kFileExists)){
    rootfile->Add(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d.root",run));
    cout << "Added file: tritium_" << run << ".root" << endl;
  }else{
    cout << "Requested run has not been replayed. Exiting." << endl << endl;
    return;
  }

  while(!gSystem->AccessPathName(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d_%d.root",run,i),kFileExists)){
    rootfile->Add(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d_%d.root",run,i));
    cout << "Added file: tritium_" << run << "_" << i << ".root" << endl;
    i=i+1;
  }                      
 
  TString arm;
  if(RIGHT_ARM_CONDITION){
    arm="FbusRrb";
  }else if(LEFT_ARM_CONDITION){
    arm="FbusLrb";
  }
  //Define cuts

  TString cut = "(TMath::Abs(" + arm + ".BPMA.x)<100)&&((ev";
  if(RIGHT_ARM_CONDITION){
    cut += "Right";
  }else if(LEFT_ARM_CONDITION){
    cut += "Left";
  }
  cut += "dnew_r*0.0003299)>3)";
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

  TF2 *ell_fit = new TF2("ell_fit","[0]/(1. + exp(-1. * [5] * ((([1]*([2]-x))^2 + ([3]*([4]-y))^2)-1.)))",3400,5750,2200,6600);

  Double_t param[6] = {25,0.00095,4500,0.001,4750, 1.};
  ell_fit->SetParameters(param);
  ell_fit->SetParLimits(0,3,50);
  ell_fit->SetParLimits(1,.0001,.002);
  ell_fit->SetParLimits(2,4250,4750);
  ell_fit->SetParLimits(3,.0001,.002);
  ell_fit->SetParLimits(4,4000,5500);
  ell_fit->SetParLimits(5,.5,10);
  
  TF2 *ell_fit2 = new TF2("ell_fit2","[0]/(1. + exp(-1. * [5] * ((([1]*([2]-x))^2 + ([3]*([4]-y))^2)-1.)))",3400,5500,2000,6800);

  Double_t param2[6] = {25,0.001,4500,0.001,4750, 1.};
  ell_fit2->SetParameters(param2);
  ell_fit2->SetParLimits(0,3,50);
  ell_fit2->SetParLimits(1,.0001,.002);
  ell_fit2->SetParLimits(2,4250,4750);
  ell_fit2->SetParLimits(3,.0001,.002);
  ell_fit2->SetParLimits(4,4000,5500);
  ell_fit2->SetParLimits(5,.5,10);

  TH2F *R1 = new TH2F("R1","Raster 1 Carbon Hole",26,3000,6000,47,1500,7500);
  TH2F *R2 = new TH2F("R2","Raster 2 Carbon Hole",33,3000,6000,55,1500,7500);

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  c1->cd(0);
  rootfile->Draw(arm + ".Raster.rawcur.y:" + arm + ".Raster.rawcur.x>>R1",TCut(cut),"colz");
  R1->Fit(ell_fit);
  //ell_fit->Draw("cont1 same");
  c2->cd(0);
  rootfile->Draw(arm + ".Raster2.rawcur.y:" + arm + ".Raster2.rawcur.x>>R2",TCut(cut),"colz");
  R2->Fit(ell_fit2);
  //ell_fit2->Draw("cont1 same");

  //***************************************************************************
  //Now plotting the raster in 1D as well as the projection of the BPMs to the 
  //target. Previously this code set the center of the carbon hole to (0,0,0).
  //Further investigation has revealed this to be the wrong way to go about it.
  //The average position of the raster should be set to the average of the
  //projection of the BPMs to the target.
  //
  //This is ideally done with a carbon foil run as it has no features to bias
  //the raster spectrum (e.g. a 2mm hole) and is not susceptible to beam
  //heating affects. Alternatively, the clock trigger can be used.
  //***************************************************************************

  Int_t run2 = r2;
  if(run2 == 0){
    cout << "What run number would you like to calibrate with?    ";
    cin >> run2;
    cout << endl << endl;
  }
  if(run2<=0){
    cout << "Invalid run number. Exiting." << endl << endl;
    return;
  }

  TChain *foilrun = new TChain("T");

  int j = 1;

  if(!gSystem->AccessPathName(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d.root",run2),kFileExists)){
    foilrun->Add(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d.root",run2));
    cout << "Added file: tritium_" << run2 << ".root" << endl;
  }else{
    cout << "Requested run has not been replayed. Exiting." << endl << endl;
    return;
  }

  while(!gSystem->AccessPathName(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d_%d.root",run2,j),kFileExists)){
    foilrun->Add(TString::Format("/cache/halla/triton/prod/marathon/pass1_calibration/kin1/tritium_%d_%d.root",run2,j));
    cout << "Added file: tritium_" << run2 << "_" << j << ".root" << endl;
    j=j+1;
  }                      

  //Create strings that will get the position of the beam at the target from BPM info
  //Hardcoded numbers are the position of BPMB w.r.t. the target and the distance between the two BPMs
  TString targx_string = "(" + arm + ".BPMB.x) + ((2.214) * ((" + arm + ".BPMB.x - " + arm + ".BPMA.x) / (5.131)))";
  TString targy_string = "(" + arm + ".BPMB.y) + ((2.214) * ((" + arm + ".BPMB.y - " + arm + ".BPMA.y) / (5.131)))";

  //Plot X and Y Currents for both Rasters
  TH1F *r1xcurr = new TH1F("r1xcurr", "Raster 1-X Current vs ADC Channel", 1000, 3000, 6000);
  TH1F *r1ycurr = new TH1F("r1ycurr", "Raster 1-Y Current vs ADC Channel", 1000, 1500, 7500);
  TH1F *r2xcurr = new TH1F("r2xcurr", "Raster 2-X Current vs ADC Channel", 1000, 3000, 6000);
  TH1F *r2ycurr = new TH1F("r2ycurr", "Raster 2-Y Current vs ADC Channel", 1000, 1500, 7500);
  
  //Plot X and Y Position at the Target
  TH1F *targxpos = new TH1F("targxpos", "Target X Position from BPMs (m)", 400, -0.02, 0.02);
  TH1F *targypos = new TH1F("targypos", "Target Y Position from BPMs (m)", 400, -0.02, 0.02);

  TCanvas *meanC = new TCanvas("Mean Canvas");
  meanC->Divide(2,3);

  //Using clock trigger cut
  TCut clock;
  /*if(RIGHT_ARM_CONDITION){
    clock = "DR.evtypebits>>8&1";
  }else if(LEFT_ARM_CONDITION){
    clock = "DL.evtypebits>>8&1";
  }*/
  if(RIGHT_ARM_CONDITION){
    clock += "evRight";
  }else if(LEFT_ARM_CONDITION){
    cut += "evLeft";
  }
  cut += "dnew_r*0.0003299)>9.6)";

  //Draw the histograms
  //It is easy to ge the mean when put into a histogram
  //The plots should also be checked to ensure that the entire spectra is
  //included in the plot. If not, the means will be thrown off.
  meanC->cd(1);
  foilrun->Draw(arm+".Raster.rawcur.x>>r1xcurr",clock); 
  meanC->cd(2);
  foilrun->Draw(arm+".Raster.rawcur.y>>r1ycurr",clock);
  meanC->cd(3);
  foilrun->Draw(arm+".Raster2.rawcur.x>>r2xcurr",clock);
  meanC->cd(4);
  foilrun->Draw(arm+".Raster2.rawcur.y>>r2ycurr",clock);
  meanC->cd(5);
  foilrun->Draw(targx_string + ">>targxpos",clock);
  meanC->cd(6);
  foilrun->Draw(targy_string + ">>targypos",clock);

  Double_t r1xm, r1ym, r2xm, r2ym, txm, tym;
  r1xm = r1xcurr->GetMean();
  r1ym = r1ycurr->GetMean();
  r2xm = r2xcurr->GetMean();
  r2ym = r2ycurr->GetMean();
  txm = targxpos->GetMean();
  tym = targypos->GetMean();

  //cout << std::setprecision(9) << r1xm << " " << r1ym << " " << r2xm << " " << r2ym << endl << endl; 
 
  cout << arm << ".Raster.raw2posT = " << txm-r1xm*kx*ell_fit->GetParameter(1)/1000. << " " << tym-r1ym*ky*ell_fit->GetParameter(3)/1000. << " " << ell_fit->GetParameter(1)*kx/1000. << " " << ell_fit->GetParameter(3)*ky/1000. << " 0.0 0.0" << endl;

  cout << arm << ".Raster2.raw2posT = " << txm-r2xm*kx*ell_fit2->GetParameter(1)/1000. << " " << tym-r2ym*ky*ell_fit2->GetParameter(3)/1000. << " " << ell_fit2->GetParameter(1)*kx/1000. << " " << ell_fit2->GetParameter(3)*ky/1000. << " 0.0 0.0" << endl;
}
