void position(Int_t run, Double_t slope1x, Double_t slope1y, Double_t slope2x, Double_t slope2y){
  TString rpath = "/chafs1/work1/tritium/tmp_data/src_fall";
  Double_t kx, ky;
  kx = 1.;
  ky = -1.;

  cout << "Would you like to calibrate L or R?    ";
  char lr;
  cin >> lr;

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
  if(lr=='R'||lr=='r'){
    arm="Rrb";
  }else{
    arm="Lrb";
  }

  TString current;
  if(lr=='R'||lr=='r'){
    current += "(evRight";
  }else{
    current += "(evLeft";
  }
  current += "dnew_r)>21";

  //Plot X and Y Position for both BPMs
  TH1F *bpmaxpos = new TH1F("bpmaxpos", "BPM A-X Position (m)", 400, -0.02, 0.02);
  TH1F *bpmaypos = new TH1F("bpmaypos", "BPM A-Y Position (m)", 400, -0.02, 0.02);
  TH1F *bpmbxpos = new TH1F("bpmbxpos", "BPM B-X Position (m)", 400, -0.02, 0.02);
  TH1F *bpmbypos = new TH1F("bpmbypos", "BPM B-Y Position (m)", 400, -0.02, 0.02);

  TCanvas *position_canvas = new TCanvas("position_canvas");
  position_canvas->Divide(2,2);

  position_canvas->cd(1);
  rootfile->Draw(arm + ".BPMA.x>>bpmaxpos",TCut(current));
  Double_t bpmaxm = bpmaxpos->GetMean();
  Double_t bpmaxrms = bpmaxpos->GetRMS();

  position_canvas->cd(2);
  rootfile->Draw(arm + ".BPMA.y>>bpmaypos",TCut(current));
  Double_t bpmaym = bpmaypos->GetMean();
  Double_t bpmayrms = bpmaypos->GetRMS();

  position_canvas->cd(3);
  rootfile->Draw(arm + ".BPMB.x>>bpmbxpos",TCut(current));
  Double_t bpmbxm = bpmbxpos->GetMean();
  Double_t bpmbxrms = bpmbxpos->GetRMS();

  position_canvas->cd(4);
  rootfile->Draw(arm + ".BPMB.y>>bpmbypos",TCut(current));
  Double_t bpmbym = bpmbypos->GetMean();
  Double_t bpmbyrms = bpmbypos->GetRMS();

  //Project the mean positions to the target
  Double_t txm = bpmbxm + (2.214 * ((bpmbxm - bpmaxm) / 5.131));
  Double_t tym = bpmbym + (2.214 * ((bpmbym - bpmaym) / 5.131));

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

  meanC->cd(1);
  rootfile->Draw(arm+".Raster.rawcur.x>>r1xcurr",TCut(current)); 
  meanC->cd(2);
  rootfile->Draw(arm+".Raster.rawcur.y>>r1ycurr",TCut(current));
  meanC->cd(3);
  rootfile->Draw(arm+".Raster2.rawcur.x>>r2xcurr",TCut(current));
  meanC->cd(4);
  rootfile->Draw(arm+".Raster2.rawcur.y>>r2ycurr",TCut(current));
  meanC->cd(5);
  rootfile->Draw(targx_string + ">>targxpos",TCut(current));
  meanC->cd(6);
  rootfile->Draw(targy_string + ">>targypos",TCut(current));

  Double_t r1xm, r1xrms, r1ym, r1yrms, r2xm, r2xrms, r2ym, r2yrms, txrms, tyrms;
  r1xm = r1xcurr->GetMean();
  r1xrms = r1xcurr->GetRMS();
  r1ym = r1ycurr->GetMean();
  r1yrms = r1ycurr->GetRMS();
  r2xm = r2xcurr->GetMean();
  r2xrms = r2xcurr->GetRMS();
  r2ym = r2ycurr->GetMean();
  r2yrms = r2ycurr->GetRMS();
  txrms = targxpos->GetRMS();
  tyrms = targypos->GetRMS();

  //Slope scaling factor
  Double_t r1x_scale, r1y_scale, r2x_scale, r2y_scale;
  r1x_scale = slope1x/(txrms/r1xrms);
  r1y_scale = slope1y/(tyrms/r1yrms);
  r2x_scale = slope2x/(txrms/r2xrms);
  r2y_scale = slope2y/(tyrms/r2yrms);

  //Calculate Calibrations at BPMs
  Double_t UbpmAx_slope = bpmaxrms*r1x_scale/(r1xrms*kx);
  Double_t UbpmAy_slope = bpmayrms*r1y_scale/(r1yrms*ky);
  Double_t UbpmAx_offset = bpmaxm - (r1xm*UbpmAx_slope);
  Double_t UbpmAy_offset = bpmaym - (r1ym*UbpmAy_slope);

  Double_t UbpmBx_slope = bpmbxrms*r1x_scale/(r1xrms*kx);
  Double_t UbpmBy_slope = bpmbyrms*r1y_scale/(r1yrms*ky);
  Double_t UbpmBx_offset = bpmbxm - (r1xm*UbpmBx_slope);
  Double_t UbpmBy_offset = bpmbym - (r1ym*UbpmBy_slope);

  Double_t DbpmAx_slope = bpmaxrms*r2x_scale/(r2xrms*kx);
  Double_t DbpmAy_slope = bpmayrms*r2y_scale/(r2yrms*ky);
  Double_t DbpmAx_offset = bpmaxm - (r2xm*DbpmAx_slope);
  Double_t DbpmAy_offset = bpmaym - (r2ym*DbpmAy_slope);

  Double_t DbpmBx_slope = bpmbxrms*r2x_scale/(r2xrms*kx);
  Double_t DbpmBy_slope = bpmbyrms*r2y_scale/(r2yrms*ky);
  Double_t DbpmBx_offset = bpmbxm - (r2xm*DbpmBx_slope);
  Double_t DbpmBy_offset = bpmbym - (r2ym*DbpmBy_slope);

  cout << std::setprecision(7) << arm << ".Raster.raw2posA = " << UbpmAx_offset << " " << UbpmAy_offset << " " << UbpmAx_slope << " " << UbpmAy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster.raw2posB = " << UbpmBx_offset << " " << UbpmBy_offset << " " << UbpmBx_slope << " " << UbpmBy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster.raw2posT = " << txm-(r1xm*slope1x) << " " << tym-(r1ym*slope1y) << " " << slope1x << " " << slope1y << " 0.0 0.0" << endl << endl;

  cout << arm << ".Raster2.raw2posA = " << DbpmAx_offset << " " << DbpmAy_offset << " " << DbpmAx_slope << " " << DbpmAy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster2.raw2posB = " << DbpmBx_offset << " " << DbpmBy_offset << " " << DbpmBx_slope << " " << DbpmBy_slope << " 0.0 0.0" << endl; 
  cout << arm << ".Raster2.raw2posT = " << txm-(r2xm*slope2x) << " " << tym-(r2ym*slope2y) << " " << slope2x << " " << slope2y << " 0.0 0.0" << endl;

}

