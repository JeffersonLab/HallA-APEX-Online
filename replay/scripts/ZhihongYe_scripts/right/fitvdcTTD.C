/**************************************************************
  fitvdcTTD.C
  Seamus Riordan
  spr4y@virginia.edu
  June 16, 2010

  This script fits the VDC analytic drift time-to-distance 
  form in the THaVDCAnalyticTTDConv class.

  It's best run compiled

  analyzer
  .L fitvdcTTD.C+
  fitvdcTTD("L")

  or

  fitvdcTTD("R")

  for the left and right arms
  
*************************************************************/

#include <TMinuit.h>
#include <TChain.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include <TString.h>
#include <iostream>
#include "THaEvent.h"

#define MAX_ENTRIES 100000
#define MAX_HIT 1000
#define NPLANE 4

#define DEG_TO_RAD 0.017453278

// Approx center of 1/tanTheta distribution
static const Double_t TT0 = 1.4;
// Old known good calibration, for comparison
static const Double_t test_vel[] = { 49.28e3, 49.73e3, 49.13e3, 49.19e3 };
static const Double_t test_apars[] = { 2.12e-3, 0, 0, 0,
				       -4.2e-4, 1.3e-3, 1.06e-4, 0 };
using namespace std;

Int_t    this_plane;
Int_t    nent[NPLANE];
Double_t wtime[NPLANE][MAX_ENTRIES], tanTh[NPLANE][MAX_ENTRIES], trdist[NPLANE][MAX_ENTRIES];

Double_t TTDform( Double_t dtime, Double_t tanTheta, Double_t fDriftVel, const Double_t *par, Double_t invTanTheta0 ){
  Double_t a1 = 0.0, a2 = 0.0;

  const Double_t* fA1tdcCor = &(par[0]);
  const Double_t* fA2tdcCor = &(par[4]);

  if( fabs(tanTheta) > 1e-7 ){
    tanTheta = 1.0 / tanTheta - invTanTheta0;
  } else {
    cerr << "TTDform: Invalid tanTheta = " << tanTheta << endl;
    return 0.0;
  }

  for (Int_t i = 3; i >= 1; i--) {
    a1 = tanTheta * (a1 + fA1tdcCor[i]);
    a2 = tanTheta * (a2 + fA2tdcCor[i]);
  }
  a1 += fA1tdcCor[0];
  a2 += fA2tdcCor[0];

  Double_t dist = fDriftVel * dtime;

  if (dist < 0) {
    //    cerr << "ttdForm: invalid dist = " << dist << endl;
    return 0.0;
  } else if (dist < a1 ) { 
    dist *= ( 1.0 + a2 / a1);
  }  else {
    dist +=  a2;
  }

  return dist;
}

Int_t Nbin = 100;

// Function to minimize
void fcn(Int_t& /*npar*/, Double_t* /*gin*/, Double_t& f, Double_t* par, Int_t /*iflag*/) {
  Int_t i;

  Double_t chisq = 0.0;
  Double_t delta;

  for (i=0; i<nent[this_plane]; i++) {
    delta =  TTDform(wtime[this_plane][i] - par[9], 
		     tanTh[this_plane][i],
		     par[0], &(par[1]), TT0 )
      - trdist[this_plane][i];

    //		printf("%g %g %g -> delta %f\n", wtime[this_plane][i] - par[9],  tanTh[this_plane][i], trdist[this_plane][i],  delta );

    chisq += delta*delta;
  }

  //	printf("%f\n", chisq );

  f = chisq;

  return;
}

TString getrun( Int_t irun )
{
  if(irun<0){
    cout << "Run number?" << endl;
    cin >> irun;
  }
  if( irun<0 )
    return TString();

  return TString(Form("rootfiles/ttdcalib_%d.root", irun));
}

void fitvdcTTD( const char *arm = "L", Int_t runnumber = -1 ){

  TString rootfile = getrun( runnumber );
  if( rootfile.IsNull() ) return;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  TChain *T = new TChain("T");

  T->Add(rootfile);
  //  T->SetBranchStatus("*", kFALSE);

  const char plane[NPLANE][8] = {"u1", "u2", "v1", "v2"};
  Double_t ang[NPLANE] = {-45.0, -45.0, 45.0, 45.0};

  Double_t nhit[NPLANE], ntr;
  Double_t hittime[NPLANE][MAX_HIT], hittrknum[NPLANE][MAX_HIT],
    hittrdist[NPLANE][MAX_HIT];
  Double_t d_th[MAX_HIT], d_ph[MAX_HIT];
  THaEvent* evt = 0;

  Int_t i, j, hit;

  // Set up branches

  T->SetBranchStatus("Event_Branch*", kTRUE);
  T->SetBranchAddress("Event_Branch", &evt);

  T->SetBranchStatus(Form("%s.tr.n", arm), kTRUE);
  T->SetBranchAddress(Form("%s.tr.n", arm), &ntr);

  T->SetBranchStatus(Form("%s.tr.d_th", arm), kTRUE);
  T->SetBranchAddress(Form("%s.tr.d_th", arm), &d_th);
  T->SetBranchStatus(Form("%s.tr.d_ph", arm), kTRUE);
  T->SetBranchAddress(Form("%s.tr.d_ph", arm), &d_ph);

  for( i = 0; i < NPLANE; i++ ){
    T->SetBranchStatus(Form("%s.vdc.%s.nhit", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.nhit", arm, plane[i]), &nhit[i]);

    T->SetBranchStatus(Form("%s.vdc.%s.time", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.time", arm, plane[i]), hittime[i]);

    T->SetBranchStatus(Form("%s.vdc.%s.trknum", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.trknum", arm, plane[i]), hittrknum[i]);

//     T->SetBranchStatus(Form("%s.vdc.%s.ltrdist", arm, plane[i]), kTRUE);
//     T->SetBranchAddress(Form("%s.vdc.%s.ltrdist", arm, plane[i]), hittrdist[i]);
    T->SetBranchStatus(Form("%s.vdc.%s.trdist", arm, plane[i]), kTRUE);
    T->SetBranchAddress(Form("%s.vdc.%s.trdist", arm, plane[i]), hittrdist[i]);

    nent[i] = 0;
  }

  Double_t this_slope;

  // Load up drift times and tangents
  for( i = 0; i < T->GetEntries(); i++ ){
    T->GetEntry(i);
    if( (i%5000)==0 ) { cout << "Entry " << i << endl; }

    if( ntr == 1 && evt->GetHeader()->GetEvtType() == 1 ){
      for( j = 0; j < NPLANE; j++ ){
	this_slope = d_th[0]*cos(ang[j]*DEG_TO_RAD) 
	  + d_ph[0]*sin(ang[j]*DEG_TO_RAD);
	for( hit = 0; hit < nhit[j] && nent[j] < MAX_ENTRIES; hit++ ){
	  if( 12e-9 < hittime[j][hit] 
	      &&  hittime[j][hit]  < 260.0e-9
	      && hittrdist[j][hit]< 0.015
	      && hittrknum[j][hit] == 1 ){
	    wtime[j][nent[j]]  = hittime[j][hit];
	    tanTh[j][nent[j]] = this_slope;
	    trdist[j][nent[j]] = hittrdist[j][hit];
	    nent[j]++;
	  }
	}
      }
    }
  }

  printf("Start fitting\n");

  Double_t arglist[10];
  Int_t ierflg = 0;
  // Drift velocity + 6 constants + offset
  TMinuit *gMinuit;

  Double_t velstart = 50.0e3;
  Double_t velstep  =  1.0e1;

  Double_t conststep = 5e-7;

  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;

  TH2F *hfit[NPLANE];

  Double_t finalvel[NPLANE], apars[NPLANE][8], finaloff[NPLANE], dummy;

  for( i = 0; i < NPLANE; i++ ){
    this_plane = i;

    gMinuit	= new TMinuit(10); 

    gMinuit->SetFCN(fcn);

    arglist[0] = 1;
    gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);


    gMinuit->mnparm(0, "vel", velstart, velstep, 4e4, 5.5e4,ierflg);

    //		gMinuit->mnparm(1, "a1_0", 0.0, conststep, 0,0,ierflg);
    gMinuit->mnparm(1, "a1_0", 0.002, 1e-6, 0.001,0.003,ierflg);
    gMinuit->mnparm(2, "a1_1", 0.0, conststep, -2e-3,2e-3,ierflg);
    gMinuit->mnparm(3, "a1_2", 0.0, conststep, -1e-3,1e-3,ierflg);
    gMinuit->mnparm(4, "a1_3", 0.0, conststep, -1e-3,1e-3,ierflg);
    /*
    gMinuit->mnparm(2, "a1_1", 0.0, 0.0, 0,0,ierflg);
    gMinuit->mnparm(3, "a1_2", 0.0, 0.0, 0,0,ierflg);
    gMinuit->mnparm(4, "a1_3", 0.0, 0.0, 0,0,ierflg);
    */

    //		gMinuit->mnparm(5, "a2_0", 0.0, conststep, 0,0,ierflg);
    gMinuit->mnparm(5, "a2_0", 0.0, 1e-6, -2e-3, 2e-3,ierflg);
    gMinuit->mnparm(6, "a2_1", 0.0, conststep, -2e-3, 2e-3,ierflg);
    gMinuit->mnparm(7, "a2_2", 0.0, conststep, -1e-3, 1e-3,ierflg);
    gMinuit->mnparm(8, "a2_3", 0.0, conststep, -1e-3, 1e-3,ierflg);
    /*
    gMinuit->mnparm(6, "a2_1", 0.0, 0.0, 0,0,ierflg);
    gMinuit->mnparm(7, "a2_2", 0.0, 0.0, 0,0,ierflg);
    gMinuit->mnparm(8, "a2_3", 0.0, 0.0, 0,0,ierflg);
    */

    //gMinuit->mnparm(9, "offset", 0.0e-7, 0.5e-9, -25e-9, 25e-9,ierflg);
    gMinuit->mnparm(9, "offset", 0.0e-7, 0.0e-9, 0,0,ierflg);

    arglist[0] = 20000;
    arglist[1] = 1.;
    gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);

    gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);

    gMinuit->GetParameter(0, finalvel[i], dummy );
    gMinuit->GetParameter(9, finaloff[i], dummy );

    for( j = 0; j < 8; j++ ){
      gMinuit->GetParameter(j+1, apars[i][j], dummy );
    }

    hfit[i] = new TH2F(Form("hfit_%s", plane[i]), Form("%s TTD", plane[i]), 200, 0.0, 0.015, 200, -0.002, 0.015 );
    hfit[i]->GetXaxis()->SetTitle("Track Dist (m)");
    hfit[i]->GetXaxis()->CenterTitle();
    hfit[i]->GetYaxis()->SetTitle("Dist from Time (m)");
    hfit[i]->GetYaxis()->CenterTitle();

    for( j = 0; j < nent[i]; j++ ){
      hfit[i]->Fill( trdist[i][j], TTDform( wtime[i][j] - finaloff[i], tanTh[i][j], finalvel[i], apars[i], TT0 ) );
      //      hfit[i]->Fill( trdist[i][j], TTDform( wtime[i][j], tanTh[i][j], test_vel[i], test_apars, 0.0 ) );
    }

    TCanvas *c = new TCanvas(plane[i], plane[i], 640, 480);
    c->SetGridx();
    c->SetGridy();
    hfit[i]->Draw("COL");
    c->SetWindowPosition(660*(i/2)+20,520*(i%2)+20);
  }

  double apars_ex[NPLANE][8];

  for( i = 0; i < NPLANE; i++ ){
    printf("\n\n");
    printf("Plane %s\n", plane[i]);
    printf("vel    = %g\n", finalvel[i] );
    printf("offset = %5.1f ns\n", finaloff[i]*1e9 );

    apars_ex[i][0] = apars[i][0] - apars[i][1]*TT0 + apars[i][2]*pow(TT0,2) - apars[i][3]*pow(TT0, 3);
    apars_ex[i][1] = apars[i][1] - 2.0*apars[i][2]*pow(TT0,1) + 3.0*apars[i][3]*pow(TT0, 2);
    apars_ex[i][2] = apars[i][2] - 3.0*apars[i][3]*pow(TT0, 1);
    apars_ex[i][3] = apars[i][3];

    apars_ex[i][4] = apars[i][4] - apars[i][5]*TT0 + apars[i][6]*pow(TT0,2) - apars[i][7]*pow(TT0, 3);
    apars_ex[i][5] = apars[i][5] - 2.0*apars[i][6]*pow(TT0,1) + 3.0*apars[i][7]*pow(TT0, 2);
    apars_ex[i][6] = apars[i][6] - 3.0*apars[i][7]*pow(TT0, 1);
    apars_ex[i][7] = apars[i][7];

    for( j = 0; j < 4; j++ ){
      printf("fA1tdcCor[%d] = %g;\n", j, apars_ex[i][j] );
    }
    for( j = 4; j < 8; j++ ){
      printf("fA2tdcCor[%d] = %g;\n", j-4, apars_ex[i][j] );
    }
  }

}
