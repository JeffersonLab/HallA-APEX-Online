/**************************************************************
  calibVDCtdc.C

  Calibrate the offsets (t_drift=0 point) of the HRS-VDC TDCs.
  For better statistics, groups of 16 adjacent wires are analyzed
  together and assigned a common offset.

  Ole Hansen
  ole@jlab.org
  June 22, 2010

  To run:

  analyzer
  .L calibVDCtdc.C+
  calibVDCtdc("L", "rootfile.root" )   // or "R" for right arm

*************************************************************/

#include <TChain.h>
#include <TF1.h>
#include <TH1I.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TSpectrum.h>
#include <TLine.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

static const Int_t MAX_HIT = 1000;
static const Int_t NPLANE = 4;
static const Int_t MAX_GRP = 6; //6 trigger types
static const Int_t NWIRES = 368;
static const Int_t NPERROW = 8;

using namespace std;

void multirun_calibTriggerTimes( const char *arm, const vector<string>& rootfiles );
bool passtrg(Int_t a,Int_t b);

// User interface
void calibTriggerTimes( const char *arm = "L", const char* rootfilename = "Afile.root" ) {
  vector<string> rootfiles;
  rootfiles.push_back(string(rootfilename));
  multirun_calibTriggerTimes( arm, rootfiles );
}


// Expert interface
void multirun_calibTriggerTimes( const char *arm, const vector<string>& rootfiles ) {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  TChain* T = new TChain("T");

  for( vector<string>::const_iterator it = rootfiles.begin(); it != rootfiles.end(); ++it ) {
    T->Add( (*it).c_str() );
    T->Add("../Rootfiles/e07006_HRS_2858.root" );
    T->Add("../Rootfiles/e07006_HRS_2859.root" ); 
    T->Add("../Rootfiles/e07006_HRS_2858_1.root" );
    T->Add("../Rootfiles/e07006_HRS_2859_1.root" );
    T->Add("../Rootfiles/e07006_HRS_2858_2.root" );
    T->Add("../Rootfiles/e07006_HRS_2859_2.root" );
  
  }
  T->SetBranchStatus("*", kFALSE);

  const char plane[NPLANE][8] = {"u1", "u2", "v1", "v2"};

  Double_t nhit[NPLANE];
  Double_t wire[NPLANE][MAX_HIT], rawtdc[NPLANE][MAX_HIT], time[NPLANE][MAX_HIT];
  Double_t evttype;
  TH1D* h_tdc[NPLANE][MAX_GRP];

  T->SetBranchStatus("DBB.evtypebits", kTRUE);
  T->SetBranchAddress("DBB.evtypebits", &evttype);

  // Set up branches & histograms
  for( Int_t i = 0; i < NPLANE; i++ ){
    T->SetBranchStatus(Form("%s.vdc.%s.nhit", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.nhit", arm, plane[i]), &nhit[i]);

    T->SetBranchStatus(Form("%s.vdc.%s.wire", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.wire", arm, plane[i]), &wire[i]);

    T->SetBranchStatus(Form("%s.vdc.%s.rawtime", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.rawtime", arm, plane[i]), &rawtdc[i]);

    T->SetBranchStatus(Form("%s.vdc.%s.time", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.time", arm, plane[i]), &time[i]);

    for( int grp = 0; grp < MAX_GRP; grp++ ) {
      //      h_tdc[i][grp] = new TH1I(Form("h_tdc_%s_%d",plane[i],grp+1), Form("%s TDC grp %d", plane[i], grp+1), 750, 500, 1250);
      h_tdc[i][grp] = new TH1D(Form("h_tdc_%s_%d",plane[i],grp+1), Form("%s TDC grp %d", plane[i], grp+1), 800, -100, 300);
    }
  }

  // Read TDC values of wires
  for( Int_t i = 0; i < T->GetEntries(); i++ ){
    T->GetEntry(i);
    //    cout<<evttype<<endl;
    if( (i%5000)==0 ){ printf("Entry %d\n", i ); }
    for( Int_t j = 0; j < NPLANE; j++ ){
      for( Int_t hit = 0; hit < nhit[j] && hit < MAX_HIT; hit++ ){
	for(Int_t trg = 1; trg<MAX_GRP+1;trg++){
	  Int_t grp   = trg-1;
	  Double_t dt  =  time[j][hit]*1e9;
	  //	  cout<<dt<<endl;
	  // if(passtrg(Int_t(evttype), trg))
	    h_tdc[j][grp]->Fill( dt);
	}
      }
    }
  }
  
  cout << "Start calibration" << endl;

  TSpectrum s;
  TCanvas* ct;
  //  Double_t sigma_est = 30, thresh_init = 0.9;
  Double_t sigma_est = 20, thresh_init = 0.9;
  bool any_failed = false;
  ct = new TCanvas("TDC Values", "TDC Values",1280, 960);
  ct->Divide(6,4);
  int ntot=0;
  for( Int_t i = 0; i < NPLANE; i++ ) {
    Int_t nfail = 0;

    Float_t tdcoff[MAX_GRP];
    memset( tdcoff, 0, sizeof(tdcoff) );

    for( int grp=0; grp<MAX_GRP; grp++ ) {
      ntot++;
      ct->cd(ntot);
      cout << "plane = " << plane[i] << " grp = " << grp+1;
      //      TH1I* h = h_tdc[i][grp];
      TH1D* h = h_tdc[i][grp];

      h->Draw();

      // Search for highest peak
      Double_t thr = thresh_init;
      while( s.Search( h, sigma_est, "", thr ) == 0 && thr > 0.1 ) { thr -= 0.1; }
      if( s.GetNPeaks() == 0 ) {
	cout << " ==== Warning: no peaks " << endl;
	continue;
      }
      //      cout << " thr = " << thr;
      Float_t* xpk = s.GetPositionX();
      Float_t* ypk = s.GetPositionY();
      Float_t xmax = h->GetBinCenter( h->GetMaximumBin() );
      Float_t xpeak = 0;
      Int_t ipk = -1;
      for( int k = 0; k < s.GetNPeaks(); k++ ) {
	// Require peaks within 1 sigma of the histogram maximum
	if( TMath::Abs( xpk[k]-xmax ) < sigma_est ) {
	  if( ipk < 0 ) {
	    ipk = k;
	    xpeak = xpk[k];
	  } else {
	    cout << " ==== Warning: multiple peak candidates. Poor statistics?" << endl;
	    any_failed = true;
	    // choose the one with the largest x
	    if( xpk[k] > xpeak ) {
	      ipk = k;
	      xpeak = xpk[k];
	    }
	  }
	}
      }
      if( ipk == -1 ) {
	cout << " ==== Error: no good peak" << endl;
	++nfail;
	any_failed = true;
	continue;
      }
      
      // Fit Gaussian to peak. The offset is then roughly 2 fitted sigma to the right of the fitted peak position
      
      h->Fit("gaus","S","", xpk[ipk]-2*sigma_est, xpk[ipk]+1.0*sigma_est);
      const TF1* gaus = h->GetFunction("gaus");
      if( !gaus ) {
	cout << "Error: no fit result in plane " << plane[i] << " group " << grp << endl;
	continue;
      }
      Double_t mean  = gaus->GetParameter(1);
      Double_t sigma = gaus->GetParameter(2);
      //      if( TMath::
      
      // Quick and dirty: t0 is 1.4 sigma to the right of the peak center, period
      //      tdcoff[grp] = xpk[ipk] + 1.4*sigma_est;
      //      tdcoff[grp] = xpk[ipk] - 1.4*sigma_est;
      //      tdcoff[grp] = xpk[ipk] - 2.0*sigma;
      tdcoff[grp] = xpk[ipk] - 1.4*sigma;
      cout<<"grp = "<<grp<<";  plane = "<<i<<"; sigma(gaus) = "<<sigma<<";  sigma(spec) = "<<sigma_est<<endl;
      cout << " tdcoff = " << setprecision(6) << tdcoff[grp] << endl;

      // Show the offsets in the histograms
      ct->cd(ntot);
      TLine* line = new TLine( tdcoff[grp], 0, tdcoff[grp], 0.2*ypk[ipk] );
      line->SetLineColor(2); // red
      line->SetLineWidth(3);
      line->Draw();
    }

    // Deal with failures -- set them to same offset as an available adjacent bin
    if( nfail >= MAX_GRP ) { //OOPS
      cout << " ===== TOTAL FAILURE ====   No data????" << endl;
      continue;
    }
   
   // Write data out in VDC database format

    ofstream outp;
    outp.open( Form("trgoff_%s_%s.txt", arm, plane[i]), ios_base::out | ios_base::trunc );
    if( !outp ) {
      cout << "ERROR: canot open output file tdcoff_" << arm << "_" << plane[i] << ".txt" << endl;
      any_failed = true;
      continue;
    }
    for(Int_t trg = 1; trg<MAX_GRP+1;trg++){
      Int_t grp=trg-1;
	outp << setw(4) << trg<< " " << setw(6) << setprecision(5) << tdcoff[grp];
	
	outp << endl;
	if( !outp ) {
	  cout << "ERROR writing output file tdcoff_" << arm << "_" << plane[i] << ".txt" << endl;
	  any_failed = true;
	  break;
	}
    }
    outp.close();
    
  }// end for i over planes
  ct->SaveAs("TriggerTimingAlignment.eps");
  
  if( any_failed ) {
    cout << "WARNING: some groups failed. Bad channels or low statistics? Offsets in blue are approximate." << endl;
  }
}

bool passtrg(Int_t evttype, Int_t trg){
  //  cout<<evttype<<"   "<<trg<<endl;
  return evttype&(1<<trg);
}
