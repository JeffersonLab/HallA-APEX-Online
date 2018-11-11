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

void hole_fit(Int_t r1=0, Int_t r2=0, Double_t r2_current=0, Double_t x_true=0.){
  TString rpath = "/chafs1/work1/tritium/tmp_data/src_fall";
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

  if(!gSystem->AccessPathName(TString::Format("%s/tritium_%d.root",rpath.Data(),run),kFileExists)){
    rootfile->Add(TString::Format("%s/tritium_%d.root",rpath.Data(),run));
    cout << "Added file: tritium_" << run << ".root" << endl;
  }else{
    cout << "Requested run has not been replayed. Exiting." << endl << endl;
    return;
  }

  while(!gSystem->AccessPathName(TString::Format("%s/tritium_%d_%d.root",rpath.Data(),run,i),kFileExists)){
    rootfile->Add(TString::Format("%s/tritium_%d_%d.root",rpath.Data(),run,i));
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
  cut += "dnew_r*0.0003299)>0)";
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
  cut += ".tr.vz)<0.05)";

  //Define the fits and the plots
  //Parameter definitions:
  //[0] - Approximate signal level outside of carbon hole
  //[1] - Current to size conversion factor for horizontal raster
  //[2] - Center of the carbon hole in horizontal raster current
  //[3] - Current to size conversion factor for vertical raster
  //[4] - Center of the carbon hole in vertical raster current
  //[5] - "Hardness" factor for the sigmoid (higher = closer to step function)
  //[6] - Approximate signal lever inside of carbon hole

  TF2 *ell_fit = new TF2("ell_fit","([0]/(1. + exp(-1. * [5] * ((([1]*([2]-x))^2 + ([3]*([4]-y))^2)-1.)))) + [6]",65000,95000,65000,95000);

  Double_t param[7] = {25,0.000066,70000,0.000027,65000, 1., 1.};
  ell_fit->SetParameters(param);
  ell_fit->SetParLimits(0,0,250);
  ell_fit->SetParLimits(1,.00001,.0002);
  ell_fit->SetParLimits(2,65000,95000);
  ell_fit->SetParLimits(3,.00001,.0002);
  ell_fit->SetParLimits(4,65000,95000);
  ell_fit->SetParLimits(5,.5,10);
  ell_fit->SetParLimits(6,0,250);
  
  TF2 *ell_fit2 = new TF2("ell_fit2","([0]/(1. + exp(-1. * [5] * ((([1]*([2]-x))^2 + ([3]*([4]-y))^2)-1.)))) + [6]",74500,80500,75500,79500);

  Double_t param2[7] = {25,0.000064,70000,0.000023,65000, 1., 1.};
  ell_fit2->SetParameters(param2);
  ell_fit2->SetParLimits(0,0,250);
  ell_fit2->SetParLimits(1,.00001,.0005);
  ell_fit2->SetParLimits(2,75000,80000);
  ell_fit2->SetParLimits(3,.00001,.0005);
  ell_fit2->SetParLimits(4,76000,78000);
  ell_fit2->SetParLimits(5,.5,10);
  ell_fit2->SetParLimits(6,0,250);

  TH2F *R1 = new TH2F("R1","Raster 1 Carbon Hole",60,67000,90000,60,71000,88000);
  TH2F *R2 = new TH2F("R2","Raster 2 Carbon Hole",60,72000,82000,60,74000,81000);

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  c1->cd(0);
  rootfile->Draw(arm + ".Raster.rawcur.y:" + arm + ".Raster.rawcur.x>>R1",TCut(cut),"colz");
  R1->Fit(ell_fit,"L");

  c2->cd(0);
  rootfile->Draw(arm + ".Raster2.rawcur.y:" + arm + ".Raster2.rawcur.x>>R2",TCut(cut),"colz");
  R2->Fit(ell_fit2,"L");
 
  //***************************************************************************
  //We know that this fit does not give a perfect size for the carbon hole.
  //Instead, we can use the reconstructed vertex z vs. the raster-x current to 
  //improve the raster-x slope. For experiments (such as MARATHON) that don't
  //take Hydrogen elastic data, we can use this to approximate the true 
  //raster-y slope. This will calculate the location of the true x-slope along
  //the fit sigmoid function. Then, it will find the corresponding y-slope at
  //the same point on the sigmoid.
  //***************************************************************************
  
  Double_t x_slope = ell_fit2->GetParameter(1);
  Double_t y_slope = ell_fit2->GetParameter(3);
  Double_t y_true = ky * y_slope / 1000.;

  if(x_true!=0.){
    Double_t s_pos = 1. / (1. + TMath::Exp(-1. * ell_fit2->GetParameter(5) * (TMath::Power(x_slope/(x_true * 1000.),2.) - 1.)));
//    cout << endl << "test " << TMath::Exp(-1. * ell_fit->GetParameter(5) * (TMath::Power(x_slope/x_true,2.) - 1.)) << endl;
    cout << endl << "True position on the sigmoid: " << s_pos << endl << endl;

    y_true = ky * y_slope / TMath::Sqrt((-1. * TMath::Log((1. / s_pos) - 1.) / ell_fit2->GetParameter(5)) + 1.) / 1000.;
    cout << endl << "True y_slope: " << y_true << endl << endl;
  }else{
    x_true = kx * x_slope / 1000.;
  }

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
  if(run2==0){
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

  if(!gSystem->AccessPathName(TString::Format("%s/tritium_%d.root",rpath.Data(),run2),kFileExists)){
    foilrun->Add(TString::Format("%s/tritium_%d.root",rpath.Data(),run2));
    cout << "Added file: tritium_" << run2 << ".root" << endl;
  }else{
    cout << "Requested run has not been replayed. Exiting." << endl << endl;
    return;
  }

  while(!gSystem->AccessPathName(TString::Format("%s/tritium_%d_%d.root",rpath.Data(),run2,j),kFileExists)){
    foilrun->Add(TString::Format("%s/tritium_%d_%d.root",rpath.Data(),run2,j));
    cout << "Added file: tritium_" << run2 << "_" << j << ".root" << endl;
    j=j+1;
  }                      

  //Create strings that will get the position of the beam at the target from BPM info
  //Hardcoded numbers are the position of BPMB w.r.t. the target and the distance between the two BPMs
  TString targx_string = "(" + arm + ".BPMB.x) + ((2.214) * ((" + arm + ".BPMB.x - " + arm + ".BPMA.x) / (5.131)))";
  TString targy_string = "(" + arm + ".BPMB.y) + ((2.214) * ((" + arm + ".BPMB.y - " + arm + ".BPMA.y) / (5.131)))";

  //Plot X and Y Currents for both Rasters
  TH1F *r1xcurr = new TH1F("r1xcurr", "Raster 1-X Current vs ADC Channel", 1000, 45000, 95000);
  TH1F *r1ycurr = new TH1F("r1ycurr", "Raster 1-Y Current vs ADC Channel", 1000, 20000, 120000);
  TH1F *r2xcurr = new TH1F("r2xcurr", "Raster 2-X Current vs ADC Channel", 1000, 45000, 95000);
  TH1F *r2ycurr = new TH1F("r2ycurr", "Raster 2-Y Current vs ADC Channel", 1000, 20000, 120000);
  
  //Plot X and Y Position at the Target
  TH1F *targxpos = new TH1F("targxpos", "Target X Position from BPMs (m)", 400, -0.02, 0.02);
  TH1F *targypos = new TH1F("targypos", "Target Y Position from BPMs (m)", 400, -0.02, 0.02);

  TCanvas *meanC = new TCanvas("Mean Canvas");
  meanC->Divide(2,3);

  //Using clock trigger cut
  TString clock;
  /*if(RIGHT_ARM_CONDITION){
    clock = "(DR.evtypebits>>8&1)";
  }else if(LEFT_ARM_CONDITION){
    clock = "(DL.evtypebits>>8&1)";
  }
  clock += "&&";*/
  if(RIGHT_ARM_CONDITION){
    clock += "(evRight";
  }else if(LEFT_ARM_CONDITION){
    clock += "(evLeft";
  }
  clock += "dnew_r)*0.0003297>";
  clock += r2_current;

  //Draw the histograms
  //It is easy to ge the mean when put into a histogram
  //The plots should also be checked to ensure that the entire spectra is
  //included in the plot. If not, the means will be thrown off.
  meanC->cd(1);
  foilrun->Draw(arm+".Raster.rawcur.x>>r1xcurr",TCut(clock)); 
  meanC->cd(2);
  foilrun->Draw(arm+".Raster.rawcur.y>>r1ycurr",TCut(clock));
  meanC->cd(3);
  foilrun->Draw(arm+".Raster2.rawcur.x>>r2xcurr",TCut(clock));
  meanC->cd(4);
  foilrun->Draw(arm+".Raster2.rawcur.y>>r2ycurr",TCut(clock));
  meanC->cd(5);
  foilrun->Draw(targx_string + ">>targxpos",TCut(clock));
  meanC->cd(6);
  foilrun->Draw(targy_string + ">>targypos",TCut(clock));

  Double_t r1xm, r1xrms, r1ym, r1yrms, r2xm, r2xrms, r2ym, r2yrms, txm, txrms, tym, tyrms;
  r1xm = r1xcurr->GetMean();
  r1xrms = r1xcurr->GetRMS();
  r1ym = r1ycurr->GetMean();
  r1yrms = r1ycurr->GetRMS();
  r2xm = r2xcurr->GetMean();
  r2xrms = r2xcurr->GetRMS();
  r2ym = r2ycurr->GetMean();
  r2yrms = r2ycurr->GetRMS();
  txm = targxpos->GetMean();
  txrms = targxpos->GetRMS();
  tym = targypos->GetMean();
  tyrms = targypos->GetRMS();

  //***************************************************************************
  //Scale the @BPM slopes by the size scaling of the 'true' @target calibration
  //so that the calculated direction is closer to truth.
  //***************************************************************************

  Double_t Utarg_xscale = x_true / (txrms * kx / r1xrms);
  Double_t Utarg_yscale = y_true / (tyrms * ky / r1yrms);
  Double_t Dtarg_xscale = ell_fit2->GetParameter(1) / (1000. * txrms / r2xrms);
  Double_t Dtarg_yscale = ell_fit2->GetParameter(3) / (1000. * tyrms / r2yrms);

  //Plot X and Y Position for both BPMs
  TH1F *bpmaxpos = new TH1F("bpmaxpos", "BPM A-X Position (m)", 400, -0.02, 0.02);
  TH1F *bpmaypos = new TH1F("bpmaypos", "BPM A-Y Position (m)", 400, -0.02, 0.02);
  TH1F *bpmbxpos = new TH1F("bpmbxpos", "BPM B-X Position (m)", 400, -0.02, 0.02);
  TH1F *bpmbypos = new TH1F("bpmbypos", "BPM B-Y Position (m)", 400, -0.02, 0.02);

  TCanvas *position_canvas = new TCanvas("position_canvas");
  position_canvas->Divide(2,2);

  position_canvas->cd(1);
  foilrun->Draw(arm + ".BPMA.x>>bpmaxpos",TCut(clock));
  Double_t bpmaxm = bpmaxpos->GetMean();
  Double_t bpmaxrms = bpmaxpos->GetRMS();

  position_canvas->cd(2);
  foilrun->Draw(arm + ".BPMA.y>>bpmaypos",TCut(clock));
  Double_t bpmaym = bpmaypos->GetMean();
  Double_t bpmayrms = bpmaypos->GetRMS();

  position_canvas->cd(3);
  foilrun->Draw(arm + ".BPMB.x>>bpmbxpos",TCut(clock));
  Double_t bpmbxm = bpmbxpos->GetMean();
  Double_t bpmbxrms = bpmbxpos->GetRMS();

  position_canvas->cd(4);
  foilrun->Draw(arm + ".BPMB.y>>bpmbypos",TCut(clock));
  Double_t bpmbym = bpmbypos->GetMean();
  Double_t bpmbyrms = bpmbypos->GetRMS();

  //Calculate Calibrations at BPMs
  Double_t UbpmAx_slope = bpmaxrms*Utarg_xscale/(r1xrms*kx);
  Double_t UbpmAy_slope = bpmayrms*Utarg_yscale/(r1yrms*ky);
  Double_t UbpmAx_offset = bpmaxm - (r1xm * UbpmAx_slope);
  Double_t UbpmAy_offset = bpmaym - (r1ym * UbpmAy_slope);

  Double_t UbpmBx_slope = bpmbxrms*Utarg_xscale/(r1xrms*kx);
  Double_t UbpmBy_slope = bpmbyrms*Utarg_yscale/(r1yrms*ky);
  Double_t UbpmBx_offset = bpmbxm - (r1xm*UbpmBx_slope);
  Double_t UbpmBy_offset = bpmbym - (r1ym*UbpmBy_slope);

  Double_t DbpmAx_slope = bpmaxrms*Dtarg_xscale/(r2xrms*kx);
  Double_t DbpmAy_slope = bpmayrms*Dtarg_yscale/(r2yrms*ky);
  Double_t DbpmAx_offset = bpmaxm - (r2xm*DbpmAx_slope);
  Double_t DbpmAy_offset = bpmaym - (r2ym*DbpmAy_slope);

  Double_t DbpmBx_slope = bpmbxrms*Dtarg_xscale/(r2xrms*kx);
  Double_t DbpmBy_slope = bpmbyrms*Dtarg_yscale/(r2yrms*ky);
  Double_t DbpmBx_offset = bpmbxm - (r2xm*DbpmBx_slope);
  Double_t DbpmBy_offset = bpmbym - (r2ym*DbpmBy_slope);

  cout << arm << ".Raster.raw2posA = " << UbpmAx_offset << " " << UbpmAy_offset << " " << UbpmAx_slope << " " << UbpmAy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster.raw2posB = " << UbpmBx_offset << " " << UbpmBy_offset << " " << UbpmBx_slope << " " << UbpmBy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster.raw2posT = " << txm-r1xm*kx*ell_fit->GetParameter(1)/1000. << " " << tym-r1ym*ky*ell_fit->GetParameter(3)/1000. << " " << ell_fit->GetParameter(1)*kx/1000. << " " << ell_fit->GetParameter(3)*ky/1000. << " 0.0 0.0" << endl << endl;

  cout << arm << ".Raster2.raw2posA = " << DbpmAx_offset << " " << DbpmAy_offset << " " << DbpmAx_slope << " " << DbpmAy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster2.raw2posB = " << DbpmBx_offset << " " << DbpmBy_offset << " " << DbpmBx_slope << " " << DbpmBy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster2.raw2posT = " << txm-r2xm*x_true << " " << tym-r2ym*y_true << " " << x_true << " " << y_true << " 0.0 0.0" << endl;
}
