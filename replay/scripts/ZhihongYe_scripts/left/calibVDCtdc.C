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
#include <TH1I.h>
#include <TF1.h>
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
static const Int_t MAX_GRP = 23; //16 wires/group
static const Int_t NWIRES = 368;
static const Int_t NPERROW = 8;

using namespace std;
bool passtrg(Int_t a,Int_t b);
void multirun_calibVDCtdc( const char *arm, const vector<string>& rootfiles );

// User interface
void calibVDCtdc( const char *arm = "L", const char* rootfilename = "Afile.root" ) {
  vector<string> rootfiles;
  rootfiles.push_back(string(rootfilename));
  multirun_calibVDCtdc( arm, rootfiles );
}



// Expert interface
void multirun_calibVDCtdc( const char *arm, const vector<string>& rootfiles ) {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  TChain* T = new TChain("T");

//   for( vector<string>::const_iterator it = rootfiles.begin(); it != rootfiles.end(); ++it ) {
//     T->Add( (*it).c_str() );
//   }

  T->Add("/w/halla-2/e08014/yez/HRS_Cali/Left/replay/Rootfiles/e08014_L1_3590.root" );
  T->Add("/w/halla-2/e08014/yez/HRS_Cali/Left/replay/Rootfiles/e08014_L1_3590_1.root" );
  T->Add("/w/halla-2/e08014/yez/HRS_Cali/Left/replay/Rootfiles/e08014_L1_3590_2.root" );
  T->Add("/w/halla-2/e08014/yez/HRS_Cali/Left/replay/Rootfiles/e08014_L1_3591.root" );
  T->Add("/w/halla-2/e08014/yez/HRS_Cali/Left/replay/Rootfiles/e08014_L1_3591_1.root" );
  
  T->SetBranchStatus("*", kFALSE);

  const char plane[NPLANE][8] = {"u1", "u2", "v1", "v2"};
  Double_t evttype;
  Double_t nhit[NPLANE];
  Double_t wire[NPLANE][MAX_HIT], rawtdc[NPLANE][MAX_HIT];
  TH1I* h_tdc[NPLANE][MAX_GRP];

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

    for( int grp = 0; grp < MAX_GRP; grp++ ) {
      h_tdc[i][grp] = new TH1I(Form("h_tdc_%s_%d",plane[i],grp+1), Form("%s TDC grp %d", plane[i], grp+1), 500, 900, 2000);
    }
  }

  Int_t trg=1;

  // Read TDC values of wires
  for( Int_t i = 0; i < T->GetEntries(); i++ ){
    T->GetEntry(i);
    if( (i%5000)==0 ){ printf("Entry %d\n", i ); }

    for( Int_t j = 0; j < NPLANE; j++ ){
      for( Int_t hit = 0; hit < nhit[j] && hit < MAX_HIT; hit++ ){
	Int_t iwire = Int_t( wire[j][hit] + 0.5 );
	Int_t grp   = iwire/(NWIRES/MAX_GRP);
	Int_t itdc  = Int_t( rawtdc[j][hit] + 0.5 );
	//	 if(passtrg(Int_t(evttype), trg))
	h_tdc[j][grp]->Fill( itdc );
      }
    }
  }

  cout << "Start calibration" << endl;

  TSpectrum s;
  TCanvas* c[NPLANE];
  Double_t sigma_est = 30, thresh_init = 0.9;
  bool any_failed = false;
  for( Int_t i = 0; i < NPLANE; i++ ) {
    Int_t nfail = 0;
    c[i] = new TCanvas(Form("c_%s",plane[i]), Form("%s raw TDCs",plane[i]), 1280, 960);
    c[i]->Divide(6,4);

    Float_t tdcoff[MAX_GRP];
    memset( tdcoff, 0, sizeof(tdcoff) );

    for( int grp=0; grp<MAX_GRP; grp++ ) {
      c[i]->cd(grp+1);
      cout << "plane = " << plane[i] << " grp = " << grp+1;
      TH1I* h = h_tdc[i][grp];

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
      cout<<"xpk[ipk] = "<<xpk[ipk]<<" +/- "<<sigma_est<<endl;
      TF1* gaus=new TF1("gaus","gaus", xpk[ipk]-1*sigma_est, xpk[ipk]+2.5*sigma_est);
      gaus->SetParameter(1,xpk[ipk]);
      gaus->SetParameter(2,sigma_est);
      h->Fit(gaus,"S","", xpk[ipk]-1.*sigma_est, xpk[ipk]+2.5*sigma_est);
      if( !gaus ) {
	cout << "Error: no fit result in plane " << plane[i] << " group " << grp << endl;
	continue;
      }
      Double_t mean  = gaus->GetParameter(1);
      Double_t sigma = gaus->GetParameter(2);
      
      // Quick and dirty: t0 is 1.4 sigma to the right of the peak center, period
      //      tdcoff[grp] = xpk[ipk] + 1.4*sigma_est;

      tdcoff[grp] = mean + 1.4*sigma;
      //      cout << " tdcoff = " << setprecision(6) << tdcoff[grp] <<" +/- "<<sigma_est<< endl
      cout << " tdcoff = " << setprecision(6) << tdcoff[grp] <<" +/- "<<sigma<< endl;

      // Show the offsets in the histograms
      c[i]->cd(grp+1);
      TLine* line = new TLine( tdcoff[grp], 0, tdcoff[grp], 0.2*ypk[ipk] );
      line->SetLineColor(2); // red
      line->SetLineWidth(3);
      line->Draw();
      if(i==0&&grp==7){
	TCanvas* ct=new TCanvas();
	h->Draw();
	line->SetLineColor(2); // red
	line->SetLineWidth(3);
	line->Draw();
	ct->SaveAs("t0-u1-grp7.eps");
      }
      
    }

    // Deal with failures -- set them to same offset as an available adjacent bin
    if( nfail >= MAX_GRP ) { //OOPS
      cout << " ===== TOTAL FAILURE ====   No data????" << endl;
      continue;
    }
    while( nfail > 0 ) {
      for( int grp = 0; grp < MAX_GRP; grp++ ) {
	if( tdcoff[grp] == 0.0 ) {
	  if( grp > 0 && tdcoff[grp-1] != 0.0 ) {
	    tdcoff[grp] = tdcoff[grp-1];
	    --nfail;
	  }
	  else if( grp < MAX_GRP-1 && tdcoff[grp+1] != 0.0 ) {
	    tdcoff[grp] = tdcoff[grp+1];
	    --nfail;
	  } else
	    continue;

	  // Draw the estimated offsets in a different color
	  c[i]->cd(grp+1);
	  TLine* line = new TLine( tdcoff[grp], 0, tdcoff[grp], 0.2*h_tdc[i][grp]->GetMaximum() );
	  line->SetLineColor(4);
	  line->SetLineWidth(3);
	  line->Draw();
	}
      }
    }
    TString out="vdc-";
    out+=arm;
    out+=plane[i];
    out+="-calib.eps";
    c[i]->SaveAs(out);
   // Write data out in VDC database format

    ofstream outp; 
    outp.open( Form("tdcoff_%s_%s.txt", arm, plane[i]), ios_base::out | ios_base::trunc );
    if( !outp ) {
      cout << "ERROR: canot open output file tdcoff_" << arm << "_" << plane[i] << ".txt" << endl;
      any_failed = true;
      continue;
    }
    for( Int_t kwire = 0; kwire < NWIRES; kwire += NPERROW ) {
      for( int k=0; k<NPERROW; k++ ) {
	Int_t iwire = kwire+k;
	Int_t grp   = iwire/(NWIRES/MAX_GRP);
	if( k > 0 ) outp << " ";
	outp << setw(4) << iwire+1 << " " << setw(6) << setprecision(5) << tdcoff[grp];
      }
      outp << endl;
      if( !outp ) {
	cout << "ERROR writing output file tdcoff_" << arm << "_" << plane[i] << ".txt" << endl;
	any_failed = true;
	break;
      }
    }
    outp.close();
    
  }// end for i over planes

  if( any_failed ) {
    cout << "WARNING: some groups failed. Bad channels or low statistics? Offsets in blue are approximate." << endl;
  }
}
bool passtrg(Int_t evttype, Int_t trg){
  //  cout<<evttype<<"   "<<trg<<endl;
  return evttype&(1<<trg);
}
