R__LOAD_LIBRARY(../../../libraries/TriFadcRasteredBeam/libTriFadcRasteredBeam.so)

using namespace std;


Int_t get_rastersize_R(TString codafname,TString runNo, Int_t firsteve, Int_t lasteve, TString rootfname)
{

  string exp = getenv("EXPERIMENT");

  // 
  //
  //  Steering script to extract the BPM information out of 
  //  the first few events from a rawdata file
  //  replacement for raster++
  //
  // original by Bodo Reitz, June 2004
  // modified and maintained by Bob Michaels, 2006
  // 
  // 
  // modified for Root 6 and FADC beam line by Tong Su,2017
  // 
  
  gStyle->SetNdivisions(605);
  TString* ca_title = new TString("Raster Size (");
  ca_title->Append(codafname);
  ca_title->Append(")");


   TLatex *t = new TLatex();
  t->SetTextFont(32);
  t->SetTextColor(1);
  t->SetTextSize(0.015);
  t->SetTextAlign(12);
  t->DrawLatex(0.1,0.335,ca_title->Data());


  TCanvas* fc1 = new TCanvas("fc1",ca_title->Data(),900,600);

  fc1->Divide(3,2);

  TLatex *ft = new TLatex();
  ft->SetTextFont(32);
  ft->SetTextColor(1);
  ft->SetTextSize(0.015);
  ft->SetTextAlign(12);
  ft->DrawLatex(0.01,0.335,ca_title->Data());






  // Set up the event layout for the output file
  
  THaEvent* event = new THaEvent;
 
  // Set up the analyzer - we use the standard one,
  // but this could be an experiment-specific one as well.
  THaAnalyzer* analyzer = THaAnalyzer::GetInstance();
  if( analyzer ) {
    analyzer->Close();
  } else {
    analyzer = new THaAnalyzer;
  }

  cout << "Loading spot's private libBeam ..."<<endl;

 // THaApparatus* BEAM1 = new THaUnRasteredBeam("Lurb","Unraster Beamline");
//  THaApparatus* FbusLrb = new THaRasteredBeam("FbusRrb","Raster Beamline"); 
//  FbusLrb->AddDetector( new THaRaster("Raster2","Downstream raster") );
//  FbusLrb->AddDetector( new THaBPM("BPMA","bpmA for raster beam"));
//  FbusLrb->AddDetector( new THaBPM("BPMB","bpmB for raster beam"));
  THaApparatus* Rrb = new TriFadcRasteredBeam("Rrb","Raster Beamline for FADC ");
    THaHRS* HRSR = new THaHRS("R","Right arm HRS");
    HRSR->AutoStandardDetectors(kFALSE);
    gHaApps->Add( HRSR );
    HRSR->AddDetector( new THaVDC("vdc", "Vertical Drift Chamber" ));





//  gHaApps->Add( FbusLrb );
  gHaApps->Add(Rrb);

  analyzer->SetEvent( event );
  analyzer->SetOutFile( rootfname );

  analyzer->SetCompressionLevel(1);
  analyzer->SetOdefFile("output_rastersize.def");
  analyzer->SetCutFile("cuts_rastersize.def");


  THaRun* run = new THaRun( codafname );
    
  run->SetFirstEvent( firsteve );
  run->SetLastEvent( lasteve );
    
  analyzer->Process(*run);
  
  delete run;

  gStyle->SetOptStat(1);
  gStyle->SetStatH(0.3);
  gStyle->SetStatW(0.3);
  gStyle->SetTitleH(0.10);
  gStyle->SetTitleW(0.5);
  gStyle->SetLabelSize(0.06,"x");
  gStyle->SetLabelSize(0.06,"y");
  gROOT->ForceStyle();


 TFile *TritiumSpot = TFile::Open("rastersize.root");
if ( TritiumSpot->IsOpen() ) printf("File opened successfully\n");


// BELOW IS FOR FADC FADC FADC --THREE TIMES FOR EVERYTHING IMPORTANT

 TH1F *fbpma_x; TritiumSpot->GetObject("fbpma_x", fbpma_x);
 TH1F *fbpma_y; TritiumSpot->GetObject("fbpma_y", fbpma_y);
 TH1F *fbpmb_x; TritiumSpot->GetObject("fbpmb_x", fbpmb_x);
 TH1F *fbpmb_y; TritiumSpot->GetObject("fbpmb_y", fbpmb_y);
 TH2F *fbpma_xy; TritiumSpot->GetObject("fbpma_xy", fbpma_xy);
 TH2F *fbpmb_xy; TritiumSpot->GetObject("fbpmb_xy", fbpmb_xy);
 TH2F *fbeam_xy; TritiumSpot->GetObject("fbeam_xy", fbeam_xy);
 TH2F *frastxy1; TritiumSpot->GetObject("frastxy1", frastxy1);
 TH2F *frastxy2; TritiumSpot->GetObject("frastxy2", frastxy2);
 TH2F *frastx1x2; TritiumSpot->GetObject("frastx1x2", frastx1x2);
 TH2F *frasty1y2; TritiumSpot->GetObject("frasty1y2", frasty1y2);
 TH2F *frastx1y2; TritiumSpot->GetObject("frastx1y2", frastx1y2);
 TH2F *frastx2y1; TritiumSpot->GetObject("frastx2y1", frastx2y1);
  
 TH1F *fbeam_x; TritiumSpot->GetObject("fbeam_x", fbeam_x);
 TH1F *fbeam_y; TritiumSpot->GetObject("fbeam_y", fbeam_y);



TH1F *fbpmaraw1; TritiumSpot->GetObject("fbpmaraw1", fbpmaraw1);
TH1F *fbpmaraw2; TritiumSpot->GetObject("fbpmaraw2", fbpmaraw2);
TH1F *fbpmaraw3; TritiumSpot->GetObject("fbpmaraw3", fbpmaraw3);
TH1F *fbpmaraw4; TritiumSpot->GetObject("fbpmaraw4", fbpmaraw4);
TH1F *fbpmbraw1; TritiumSpot->GetObject("fbpmbraw1", fbpmbraw1);
TH1F *fbpmbraw2; TritiumSpot->GetObject("fbpmbraw2", fbpmbraw2);
TH1F *fbpmbraw3; TritiumSpot->GetObject("fbpmbraw3", fbpmbraw3);
TH1F *fbpmbraw4; TritiumSpot->GetObject("fbpmbraw4", fbpmbraw4);
TH1F *fbpmaraws; TritiumSpot->GetObject("fbpmaraws", fbpmaraws);
TH1F *fbpmbraws; TritiumSpot->GetObject("fbpmbraws", fbpmbraws);

TH1F *fURastX; TritiumSpot->GetObject("fURastX", fURastX);
TH1F *fURastY; TritiumSpot->GetObject("fURastY", fURastY);
TH1F *fDRastX; TritiumSpot->GetObject("fDRastX", fDRastX);
TH1F *fDRastY; TritiumSpot->GetObject("fDRastY", fDRastY);

TH2F *furastraw_bpma_x; TritiumSpot->GetObject("furastraw_bpma_x", furastraw_bpma_x);
TH2F *furastraw_bpma_y; TritiumSpot->GetObject("furastraw_bpma_y", furastraw_bpma_y);
TH2F *furastraw_bpmb_x; TritiumSpot->GetObject("furastraw_bpmb_x", furastraw_bpmb_x);
TH2F *furastraw_bpmb_y; TritiumSpot->GetObject("furastraw_bpmb_y", furastraw_bpmb_y);

TH2F *furastraw_y_bpma_x; TritiumSpot->GetObject("furastraw_y_bpma_x", furastraw_y_bpma_x);
TH2F *furastraw_x_bpma_y; TritiumSpot->GetObject("furastraw_x_bpma_y", furastraw_x_bpma_y);
TH2F *furastraw_y_bpmb_x; TritiumSpot->GetObject("furastraw_y_bpmb_x", furastraw_y_bpmb_x);
TH2F *furastraw_x_bpmb_y; TritiumSpot->GetObject("furastraw_x_bpmb_y", furastraw_x_bpmb_y);

TH2F *fdrastraw_bpma_x; TritiumSpot->GetObject("fdrastraw_bpma_x", fdrastraw_bpma_x);
TH2F *fdrastraw_bpma_y; TritiumSpot->GetObject("fdrastraw_bpma_y", fdrastraw_bpma_y);
TH2F *fdrastraw_bpmb_x; TritiumSpot->GetObject("fdrastraw_bpmb_x", fdrastraw_bpmb_x);
TH2F *fdrastraw_bpmb_y; TritiumSpot->GetObject("fdrastraw_bpmb_y", fdrastraw_bpmb_y);

TH2F *fdrastraw_y_bpma_x; TritiumSpot->GetObject("fdrastraw_y_bpma_x", fdrastraw_y_bpma_x);
TH2F *fdrastraw_x_bpma_y; TritiumSpot->GetObject("fdrastraw_x_bpma_y", fdrastraw_x_bpma_y);
TH2F *fdrastraw_y_bpmb_x; TritiumSpot->GetObject("fdrastraw_y_bpmb_x", fdrastraw_y_bpmb_x);
TH2F *fdrastraw_x_bpmb_y; TritiumSpot->GetObject("fdrastraw_x_bpmb_y", fdrastraw_x_bpmb_y);




  fbpma_x->GetXaxis()->SetLabelSize(0.075);
  fbpma_x->GetYaxis()->SetLabelSize(0.075);
  fbpma_y->GetXaxis()->SetLabelSize(0.075);
  fbpma_y->GetYaxis()->SetLabelSize(0.075);
  fbpma_xy->GetXaxis()->SetLabelSize(0.075);
  fbpma_xy->GetYaxis()->SetLabelSize(0.075);
  fbpmb_x->GetXaxis()->SetLabelSize(0.075);
  fbpmb_x->GetYaxis()->SetLabelSize(0.075);

  fbpmb_y->GetXaxis()->SetLabelSize(0.075);
  fbpmb_y->GetYaxis()->SetLabelSize(0.075);
  fbpmb_xy->GetXaxis()->SetLabelSize(0.075);
  fbpmb_xy->GetYaxis()->SetLabelSize(0.075);
  fbeam_x->GetXaxis()->SetLabelSize(0.075);
  fbeam_x->GetYaxis()->SetLabelSize(0.075);
  fbeam_y->GetXaxis()->SetLabelSize(0.075);
  fbeam_y->GetYaxis()->SetLabelSize(0.075);
  fbeam_xy->GetXaxis()->SetLabelSize(0.075);
  fbeam_xy->GetYaxis()->SetLabelSize(0.075);




//// BELOW IS FOR FADC FADC FADC --THREE TIMES FOR EVERYTHING IMPORTANT////
  fc1->cd(1);  
  gPad->SetGridx();  
  gPad->SetGridy();
  fbpma_xy->Draw("colz");
  fc1->cd(4);
  fbpmb_xy->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();

  fc1->cd(2);
  fbpma_x->Draw();
  fc1->cd(5);
  fbpmb_x->Draw();
 
  fc1->cd(3);
  fbpma_y->Draw();
  fc1->cd(6);
  fbpmb_y->Draw();


  TCanvas* fc2 =  new TCanvas("fc2","FADC Upstream Raster Plots ",1200,600);
  fc2->Divide(3,1);
  fc2->cd(1);
  // Added to fix axes, show raster x-y in full window
  // TH2F *fhaxis = new TH2F("fhaxis","FADC Fast Raster 1 X vs.Y",500,60000,70000,500,60000,70000);
  //fhaxis->Draw();
  frastxy1->Draw("colz");
  frastxy1->SetTitle("FADC Upstream Raster X vs.Y");
  frastxy1->SetAxisRange(20000, 30000,"Y");
  frastxy1->SetAxisRange(20000, 30000,"X");
  frastxy1->GetXaxis()->SetLabelSize(.04);
  frastxy1->GetYaxis()->SetLabelSize(.03);
  gPad->SetGrid(1,1);
  gStyle->SetOptStat(1);

  fc2->cd(2);
  fURastX->Draw();
  fc2->cd(3);
  fURastY->Draw();

  TCanvas* fc2A =  new TCanvas("fc2A","FADC Downstream Raster Plots",1200,600);
  fc2A->Divide(3,1);
  fc2A->cd(1);
  // Added to fix axes, show raster x-y in full window
  // TH2F *fhaxis2 = new TH2F("fhaxis","FADC Fast Raster 2 X vs.Y",500,60000,70000,500,60000,70000);
  // fhaxis2->Draw();
  frastxy2->Draw("colz");
  frastxy2->SetTitle("FADC Downstream Raster X vs.Y");
  frastxy2->SetAxisRange(10000, 30000,"Y");
  frastxy2->SetAxisRange(10000, 30000,"X");
  frastxy2->GetXaxis()->SetLabelSize(.04);
  frastxy2->GetYaxis()->SetLabelSize(.03);
  gPad->SetGrid(1,1);
  //frastxy2->Draw("same colz");
  gStyle->SetOptStat(1);
  fc2A->cd(2);
  fDRastX->Draw();
  fDRastX->GetXaxis()->SetLabelSize(.04);
  fDRastX->GetYaxis()->SetLabelSize(.04);

  fc2A->cd(3);
  fDRastY->Draw();
  fDRastY->GetXaxis()->SetLabelSize(.04);
  fDRastY->GetYaxis()->SetLabelSize(.04);


 
  TCanvas* fc2B =  new TCanvas("fc2B","fadc check",900,600);
  //  gStyle->SetOptStat(0);
  fc2B->Divide(2,1);
  fc2B->cd(1);
  frastx1x2->Draw("colz");
  frastx1x2->GetXaxis()->SetLabelSize(.05);
  frastx1x2->GetYaxis()->SetLabelSize(.038);
  gPad->SetGrid(1,1);
  gStyle->SetOptStat(1);
  fc2B->cd(2);
  frasty1y2->Draw("colz");
  frasty1y2->GetXaxis()->SetLabelSize(.05);
  frasty1y2->GetYaxis()->SetLabelSize(.038);
  gPad->SetGrid(1,1);
  
  TCanvas* fc3 =  new TCanvas("fc3","FADC BPM Antenna Plots",900,600);
  //  gStyle->SetOptStat(0);
  fc3->Divide(4,2);

  fc3->cd(1);
  fbpmaraw1->Draw();
  fbpmaraw1->GetXaxis()->SetLabelSize(.04);
  fc3->cd(2);
  fbpmaraw2->Draw();
  fbpmaraw2->GetXaxis()->SetLabelSize(.04);

  fc3->cd(3);
  fbpmaraw3->Draw();
  fbpmaraw3->GetXaxis()->SetLabelSize(.04);

  fc3->cd(4);
  fbpmaraw4->Draw(); 
  fbpmaraw4->GetXaxis()->SetLabelSize(.04);

  fc3->cd(5);
  fbpmbraw1->Draw();
  fbpmbraw1->GetXaxis()->SetLabelSize(.04);

  fc3->cd(6);
  fbpmbraw2->Draw();
  fbpmbraw2->GetXaxis()->SetLabelSize(.04);

  fc3->cd(7);
  fbpmbraw3->Draw();
  fbpmbraw3->GetXaxis()->SetLabelSize(.04);

  fc3->cd(8);
  fbpmbraw4->Draw(); 
  fbpmbraw4->GetXaxis()->SetLabelSize(.04);


  TCanvas* fc4 =  new TCanvas("c4","FADC Raster vs BPM Plots",1600,800);
  //  gStyle->SetOptStat(0);
  fc4->Divide(4,2);

  fc4->cd(1);
  furastraw_bpma_x->Draw("col");
  fc4->cd(2);
  furastraw_bpma_y->Draw("col");
  fc4->cd(3);
  furastraw_bpmb_x->Draw("col");
  fc4->cd(4);
  furastraw_bpmb_y->Draw("col");
  fc4->cd(5);
  fdrastraw_bpma_x->Draw("col");
  fc4->cd(6);
  fdrastraw_bpma_y->Draw("col");
  fc4->cd(7);
  fdrastraw_bpmb_x->Draw("col");
  fc4->cd(8);
  fdrastraw_bpmb_y->Draw("col");

  TCanvas* fc5 =  new TCanvas("fc5","FADC Beam Positions (A,B,Target)",1200,1200);
  fc5->Divide(3,3);

  fc5->cd(1);
  fbpma_xy->Draw("col");
    gPad->SetGridx();  
  gPad->SetGridy();

  fc5->cd(2);
  
  fbpma_x->Draw();
  fc5->cd(3);
  fbpma_y->Draw();
  fc5->cd(4);
  fbpmb_xy->Draw("col");
    gPad->SetGridx();  
  gPad->SetGridy();

  fc5->cd(5);
  fbpmb_x->Draw();
  fc5->cd(6);
  fbpmb_y->Draw();
  fc5->cd(7);
  fbeam_xy->Draw("col");
    gPad->SetGridx();  
  gPad->SetGridy();

  fc5->cd(8);
  fbeam_x->Draw();
  fc5->cd(9);
  fbeam_y->Draw();









  cout<<"++++++++++++++++++++++++++++++++"<<endl;
  cout<<"*****Beam Check For TRITIUM*****"<<endl;
  cout<<"++++++++++++++++++++++++++++++++"<<endl;


  cout<<"CODA-file:   "<<codafname<<endl;
  cout<<"first event: "<<firsteve<<endl;
  cout<<"last event:  "<<lasteve<<endl;
  cout<<"++++++++++++++++++++++++++++++++"<<endl;
  cout<<""<<endl;
  cout<<"type .q when you are done"<<endl;
  
 
    TString name4=Form("FADC_%s_R_",exp.c_str());
   name4.Append(runNo);
   name4.Append(".pdf[");

   TString name5=Form("FADC_%s_R_",exp.c_str());
   name5.Append(runNo);
   name5.Append(".pdf");

   TString name6=Form("FADC_%s_R_",exp.c_str());
   name6.Append(runNo);
   name6.Append(".pdf]");

  

   fc5->SaveAs(name4);
   fc5->SaveAs(name5);
   fc3->SaveAs(name5);
   fc2->SaveAs(name5);
   fc2A->SaveAs(name5);
   fc2B->SaveAs(name5);
   fc2B->SaveAs(name6);
  return 1;


}


