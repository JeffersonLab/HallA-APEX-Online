/*
 * Author: Tyler Hague
 * Date 2 Nov 17
 *
 * This code will calibrate the raster both in the Fastbus and fADCs.
 * Work in progress
 */

#define RIGHT_ARM_CONDITION run>=90000
#define LEFT_ARM_CONDITION run<90000

#include "TString.h"

void raster_calib(){
  TString rpath = "/chafs1/work1/tritium/tmp_data/src_fall";

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
  //TFile *test_file = new TFile(Form("/volatile/halla/triton/tjhague/rootfiles/coinc_test_%d_%d.root",run,i));

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

  //TTree* T = nullptr;
  //rootfile->GetObject("T",T);
  
  TString arm;
  if(RIGHT_ARM_CONDITION){
    arm="Rrb";
  }else if(LEFT_ARM_CONDITION){
    arm="Lrb";
  }


  //Create strings that will get the position of the beam at the target from BPM info
  //Hardcoded numbers are the position of BPMB w.r.t. the target and the distance between the two BPMs
  TString targ_x_pos = "(" + arm + ".BPMB.x) + ((2.214) * ((" + arm + ".BPMB.x - " + arm + ".BPMA.x) / (5.131)))";
  TString targ_y_pos = "(" + arm + ".BPMB.y) + ((2.214) * ((" + arm + ".BPMB.y - " + arm + ".BPMA.y) / (5.131)))";

  //Define Plots of BPM and Raster Current (and Beam Position at Target?)
  //Also get the Mean and RMS of each plot
  
  //Plot X and Y Currents for both Rasters
  TH1F *r1xcurr = new TH1F("r1xcurr", "Raster 1-X Current vs ADC Channel", 1000, 45000, 95000);
  TH1F *r1ycurr = new TH1F("r1ycurr", "Raster 1-Y Current vs ADC Channel", 1000, 20000, 120000);
  TH1F *r2xcurr = new TH1F("r2xcurr", "Raster 2-X Current vs ADC Channel", 1000, 45000, 95000);
  TH1F *r2ycurr = new TH1F("r2ycurr", "Raster 2-Y Current vs ADC Channel", 1000, 20000, 120000);

  //Plot X and Y Position for both BPMs
  TH1F *bpmaxpos = new TH1F("bpmaxpos", "BPM A-X Position (m)", 400, -0.02, 0.02);
  TH1F *bpmaypos = new TH1F("bpmaypos", "BPM A-Y Position (m)", 400, -0.02, 0.02);
  TH1F *bpmbxpos = new TH1F("bpmbxpos", "BPM B-X Position (m)", 400, -0.02, 0.02);
  TH1F *bpmbypos = new TH1F("bpmbypos", "BPM B-Y Position (m)", 400, -0.02, 0.02);

  //Plot X and Y Position at the Target
  TH1F *targxpos = new TH1F("targxpos", "Target X Position (m)", 400, -0.02, 0.02);
  TH1F *targypos = new TH1F("targypos", "Target Y Position (m)", 400, -0.02, 0.02);

  //Populate the plots
  //Need to start with a cut for when the beam is off
  TString cut = "";
  if(RIGHT_ARM_CONDITION){
    cut += "evRight";
  }else if(LEFT_ARM_CONDITION){
    cut += "evLeft";
  }
  cut += "dnew_r>9";
  cut+="&&";
  if(RIGHT_ARM_CONDITION){
    cut+="DR";
  }else if(LEFT_ARM_CONDITION){
    cut+="DL";
  }
  cut+=".evtypebits>>8&1";
  TCut beamcut = cut.Data();

  //The plots are added to a canvas as they are populated
  TCanvas *raster_canvas = new TCanvas("raster_canvas");
  raster_canvas->Divide(2,2);

  raster_canvas->cd(1);
  rootfile->Draw(arm + ".Raster.rawcur.x>>r1xcurr",beamcut);
  Double_t r1x_mean = r1xcurr->GetMean();
  Double_t r1x_rms = r1xcurr->GetRMS();

  raster_canvas->cd(2);
  rootfile->Draw(arm + ".Raster.rawcur.y>>r1ycurr",beamcut);
  Double_t r1y_mean = r1ycurr->GetMean();
  Double_t r1y_rms = r1ycurr->GetRMS();

  raster_canvas->cd(3);
  rootfile->Draw(arm + ".Raster2.rawcur.x>>r2xcurr",beamcut);
  Double_t r2x_mean = r2xcurr->GetMean();
  Double_t r2x_rms = r2xcurr->GetRMS();

  raster_canvas->cd(4);
  rootfile->Draw(arm + ".Raster2.rawcur.y>>r2ycurr",beamcut);
  Double_t r2y_mean = r2ycurr->GetMean();
  Double_t r2y_rms = r2ycurr->GetRMS();

  TCanvas *position_canvas = new TCanvas("position_canvas");
  position_canvas->Divide(2,3);

  position_canvas->cd(1);
  rootfile->Draw(arm + ".BPMA.x>>bpmaxpos",beamcut);
  Double_t bpmax_mean = bpmaxpos->GetMean();
  Double_t bpmax_rms = bpmaxpos->GetRMS();

  position_canvas->cd(2);
  rootfile->Draw(arm + ".BPMA.y>>bpmaypos",beamcut);
  Double_t bpmay_mean = bpmaypos->GetMean();
  Double_t bpmay_rms = bpmaypos->GetRMS();

  position_canvas->cd(3);
  rootfile->Draw(arm + ".BPMB.x>>bpmbxpos",beamcut);
  Double_t bpmbx_mean = bpmbxpos->GetMean();
  Double_t bpmbx_rms = bpmbxpos->GetRMS();

  position_canvas->cd(4);
  rootfile->Draw(arm + ".BPMB.y>>bpmbypos",beamcut);
  Double_t bpmby_mean = bpmbypos->GetMean();
  Double_t bpmby_rms = bpmbypos->GetRMS();

  position_canvas->cd(5);
  rootfile->Draw(targ_x_pos + ">>targxpos",beamcut);
  Double_t targx_mean = targxpos->GetMean();
  Double_t targx_rms = targxpos->GetRMS();

  position_canvas->cd(6);
  rootfile->Draw(targ_y_pos + ">>targypos",beamcut);
  Double_t targy_mean = targypos->GetMean();
  Double_t targy_rms = targypos->GetRMS();

  position_canvas->cd(6);

  //Calculate and display coeffiecients
  //Give instructions on how to update database?
  Double_t UbpmAx_offset = bpmax_mean - ((r1x_mean*bpmax_rms)/(r1x_rms*kx));
  Double_t UbpmAx_slope = bpmax_rms/(r1x_rms*kx);
  Double_t UbpmAy_offset = bpmay_mean - ((r1y_mean*bpmay_rms)/(r1y_rms*ky));
  Double_t UbpmAy_slope = bpmay_rms/(r1y_rms*ky);

  Double_t UbpmBx_offset = bpmbx_mean - ((r1x_mean*bpmbx_rms)/(r1x_rms*kx));
  Double_t UbpmBx_slope = bpmbx_rms/(r1x_rms*kx);
  Double_t UbpmBy_offset = bpmby_mean - ((r1y_mean*bpmby_rms)/(r1y_rms*ky));
  Double_t UbpmBy_slope = bpmby_rms/(r1y_rms*ky);

  Double_t Utargx_offset = targx_mean - ((r1x_mean*targx_rms)/(r1x_rms*kx));
  Double_t Utargx_slope = targx_rms/(r1x_rms*kx);
  Double_t Utargy_offset = targy_mean - ((r1y_mean*targy_rms)/(r1y_rms*ky));
  Double_t Utargy_slope = targy_rms/(r1y_rms*ky);

  Double_t DbpmAx_offset = bpmax_mean - ((r2x_mean*bpmax_rms)/(r2x_rms*kx));
  Double_t DbpmAx_slope = bpmax_rms/(r2x_rms*kx);
  Double_t DbpmAy_offset = bpmay_mean - ((r2y_mean*bpmay_rms)/(r2y_rms*ky));
  Double_t DbpmAy_slope = bpmay_rms/(r2y_rms*ky);

  Double_t DbpmBx_offset = bpmbx_mean - ((r2x_mean*bpmbx_rms)/(r2x_rms*kx));
  Double_t DbpmBx_slope = bpmbx_rms/(r2x_rms*kx);
  Double_t DbpmBy_offset = bpmby_mean - ((r2y_mean*bpmby_rms)/(r2y_rms*ky));
  Double_t DbpmBy_slope = bpmby_rms/(r2y_rms*ky);

  Double_t Dtargx_offset = targx_mean - ((r2x_mean*targx_rms)/(r2x_rms*kx));
  Double_t Dtargx_slope = targx_rms/(r2x_rms*kx);
  Double_t Dtargy_offset = targy_mean - ((r2y_mean*targy_rms)/(r2y_rms*ky));
  Double_t Dtargy_slope = targy_rms/(r2y_rms*ky);

  cout << arm << ".Raster.raw2posA = " << UbpmAx_offset << " " << UbpmAy_offset << " " << UbpmAx_slope << " " << UbpmAy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster.raw2posB = " << UbpmBx_offset << " " << UbpmBy_offset << " " << UbpmBx_slope << " " << UbpmBy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster.raw2posT = " << Utargx_offset << " " << Utargy_offset << " " << Utargx_slope << " " << Utargy_slope << " 0.0 0.0" << endl << endl; 
  
  cout << arm << ".Raster2.raw2posA = " << DbpmAx_offset << " " << DbpmAy_offset << " " << DbpmAx_slope << " " << DbpmAy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster2.raw2posB = " << DbpmBx_offset << " " << DbpmBy_offset << " " << DbpmBx_slope << " " << DbpmBy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster2.raw2posT = " << Dtargx_offset << " " << Dtargy_offset << " " << Dtargx_slope << " " << Dtargy_slope << " 0.0 0.0" << endl; 
}
