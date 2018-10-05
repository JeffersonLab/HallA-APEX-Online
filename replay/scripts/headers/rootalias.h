// For Tritium:
// extracting info from rootfiles and other useful functions
// adapted from Ou,Longwu's GMp code
// -shujie 03,2018

#ifndef ROOTALIAS_H
#define ROOTALIAS_H


#include <string>
#include <stdio.h> 
#include <cstdlib>
#include <iomanip>
#include <locale>
#include <algorithm>
#include <array>
#include <vector>
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TTree.h"
#include "TROOT.h"
#include "TStyle.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TH1.h"
#include "TChain.h"
#include "TString.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLatex.h"
#include "TLeaf.h"
#include "TGraphErrors.h"
#include "TSystem.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH2.h"
#include "TLine.h"
#include "TEllipse.h"
#include "TCut.h"
#include "TProfile.h"
#include "TFile.h"
#include "TEventList.h"
#include "TSpectrum.h"
#include "TArray.h"
#include "THaRun.h"
#include "THaRunParameters.h"
#include "TVector.h"

using namespace std;

const char* ROOTPATHS[] = {
  "/chafs1/work1/tritium/Rootfiles/",
  "/home/shujie/jlab/MyTritium/Rootfiles/",
  "/home/shujie/analyzer/rootfiles/",
  "/work/halla/triton/shujie/tritium/rootfiles/",
  "/cache/halla/triton/prod/src/spring2018/pass1/",
  "/volatile/halla/triton/nathaly/Rootfiles/",  
  "/cache/halla/triton/prod/coinc_eep/pass2.3/all/",
  "/chafs1/work1/tritium/Tritium_Rootfiles/",
  "/volatile/halla/triton/shujie/replay/Rootfiles/",
  "/volatile/halla/triton/Tritium_Rootfiles/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin0/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin13/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin16/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/optics/",
  "/v/lustre2/expphy/volatile/halla/triton/shujie/replay/pass1/",
  "./",
  0
};


const char* MCPATHS[] = {
	"/run/media/jbane/Slim/",
	"./mcroot/",
	0
};

const double pi=3.1415926535897932;
const double rad=pi/180.0;

//-----------LHRS-----------------------------------------
const double beta_min_L = 0.5;
const double sh_min_L   = 0.7;
const double cer_min_L  = 1500;

TCut sh_cut_L       = Form("(L.prl1.e+L.prl2.e)>HacL_D1_P0rb*1000*%g",sh_min_L);
TCut cer_cut_L      = Form("L.cer.asum_c>%g",cer_min_L);
TCut beta_cut_L     = Form("L.tr.beta>%g",beta_min_L);
TCut electron_cut_L = cer_cut_L+sh_cut_L; // remove beta cut which requires tracking

TCut track_L        = "L.tr.n==1";
//TCut aperture_L     = "((L.tr.tg_y+L.tr.tg_ph*1.5)^2/0.64+(L.tr.tg_th*1.5)^2/0.49)<0.01";
const double tg_dp_L=0.04;
const double tg_th_L=0.04;
const double tg_ph_L=0.03;
const double tg_vz_L=0.09;
TCut dp_cut_L=Form("fabs(L.tr.tg_dp)<%g",tg_dp_L);
TCut th_cut_L=Form("fabs(L.tr.tg_th)<%g",tg_th_L);
TCut ph_cut_L=Form("fabs(L.tr.tg_ph)<%g",tg_ph_L);
TCut z_cut_L =Form("fabs(L.tr.vz)<%g",tg_vz_L);
TCut acc_cut_L=dp_cut_L+th_cut_L+ph_cut_L+z_cut_L;//+aperture_L;
const double tg_dp_L_e=0.035;
const double tg_th_L_e=0.035;
const double tg_ph_L_e=0.025;
TCut dp_cut_L_e  = Form("fabs(L.tr.tg_dp)<%g",tg_dp_L_e);
TCut th_cut_L_e  = Form("fabs(L.tr.tg_th)<%g",tg_th_L_e);
TCut ph_cut_L_e  = Form("fabs(L.tr.tg_ph)<%g",tg_ph_L_e);
TCut acc_cut_L_e = dp_cut_L_e+th_cut_L_e+ph_cut_L_e+z_cut_L;//+aperture_L;

// to project track from focal plane back to an aperture ( e.g. Q3 exit at -3.57m )
const char* fp_proj    = "L.tr.x+L.tr.th*%g:L.tr.y+L.tr.ph*%g";
TString     q3_proj    = Form(fp_proj,-3.57,-3.57);

const char* tg_proj    = "L.tr.tg_th*%g-Lrb.y:L.tr.tg_ph*%g+L.tr.tg_y";
TString     q1_proj    = Form(tg_proj,1.1573, 1.1573);
TString     sieve_proj = Form(tg_proj,1.0707, 1.0707);

//--------------RHRS-------------------------------------
const double beta_min_R = 0.6;
const double sh_min_R   = 0.8;
const double cer_min_R  = 2000;

TCut sh_cut_R       = Form("(R.ps.e+R.sh.e)>HacR_D1_P0rb*1000*%g",sh_min_R);
TCut cer_cut_R      = Form("R.cer.asum_c>%g",cer_min_R);
TCut beta_cut_R     = Form("R.tr.beta>%g",beta_min_R);
TCut electron_cut_R = cer_cut_R+sh_cut_R+beta_cut_R;
TCut track_R        = "R.tr.n==1";

const double tg_dp_R=0.05;
const double tg_th_R=0.07;
const double tg_ph_R=0.05;
const double tg_vz_R=0.07;
TCut dp_cut_R  = Form("fabs(R.tr.tg_dp)<%g",tg_dp_R);
TCut th_cut_R  = Form("fabs(R.tr.tg_th)<%g",tg_th_R);
TCut ph_cut_R  = Form("fabs(R.tr.tg_ph)<%g",tg_ph_R);
TCut z_cut_R   = Form("fabs(R.tr.vz)<%g",tg_vz_R);
TCut acc_cut_R = dp_cut_R+th_cut_R+ph_cut_R+z_cut_R;//+aperture_L;
const double tg_dp_R_e=0.035;
const double tg_th_R_e=0.035;
const double tg_ph_R_e=0.02;
TCut dp_cut_R_e  = Form("fabs(R.tr.tg_dp)<%g",tg_dp_R_e);
TCut th_cut_R_e  = Form("fabs(R.tr.tg_th)<%g",tg_th_R_e);
TCut ph_cut_R_e  = Form("fabs(R.tr.tg_ph)<%g",tg_ph_R_e);
TCut acc_cut_R_e = dp_cut_R_e+th_cut_R_e+ph_cut_R_e+z_cut_R;//+aperture_L;




//--------------------------------
// Chain rootfiles given run number, taken from Longwu Ou's GMP code
//--------------------------------

TChain* LoadRun(Int_t run, const char* path, const char* tree, Int_t skim=0, Int_t max_split=99)
{
  TChain* tt       = new TChain(tree);
  TString basename = Form("tritium_%d",run);
  if(skim)
    basename = Form("skim_%d",run);

  TString rootfile = basename + ".root";
  TString dir      = path;
  Long_t  split    = 0;

  if (!dir.EndsWith("/")) dir.Append("/");
  rootfile.Prepend(dir.Data());

  while ( !gSystem->AccessPathName(rootfile.Data()) ) {
    tt->Add(rootfile.Data());
    cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
    split++;
    if(split>=max_split) break;
    rootfile = basename + "_" + split + ".root";
    rootfile.Prepend(dir.Data());

  }

  if (split<=0) {
   //if (debug>0) cerr << "Can not find ROOT file for run " << run << endl;
   delete tt;
   tt = 0;
  }

  return tt;
}

// Load TTree "tree" form all the ROOT files generated from a specific run
// The ROOT files are searched for from the given "ROOTPATHS" array given at the beginning of the file
//      run  --- run number
//      tree --- The name of the tree to be loaded
TChain* LoadRun(Int_t run, const char* tree = "T", Int_t skim=0, Int_t max_split = 99)
{
  Int_t   i  = 0;
  TChain* tt = 0;
  while (ROOTPATHS[i]) {
   tt = LoadRun(run,ROOTPATHS[i++],tree, skim, max_split);
   if (tt) return tt;
  }
//    if (!tt)
//  cerr << "Can not find ROOT file for run " << run << endl;
  delete tt;
  tt = 0;
  return tt;
}

//----------------------------
// Load online replay
//----------------------------


TChain* LoadOnline(Int_t run, const char* path, const char* tree)
{
    TChain* tt = new TChain(tree);

    TString  basename = Form("tritium_online_%d",run);
    TString  rootfile = basename + ".root";
    
    TString dir = path;
    if (!dir.EndsWith("/")) dir.Append("/");

    rootfile.Prepend(dir.Data());

    Long_t split = 0;
    // cout<<rootfile<<endl;
    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
  tt->Add(rootfile.Data());
  cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
  split++;
  rootfile = basename + "_" + split + ".root";
  rootfile.Prepend(dir.Data());

    }

    return tt;
}

TChain* LoadOnline(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (ROOTPATHS[i]) {
      // cout<<ROOTPATHS[i]<<endl;
      tt = LoadOnline(run,ROOTPATHS[i++],tree);
      if (tt) return tt;
    }
    if (!tt)
      cerr << "Can not find online replay file for run " << run << endl;
    delete tt;
    tt=0;
    return tt;
}


TChain* LoadMC(Int_t run, int tarid=0, const char* tree = "h9040")
{
	TString tgt="";
	if(tarid>0){ tgt=Form("_%d",tarid);}
	TChain *tt = new TChain(tree);
	tt->Add(Form("%smc%d%s.root",MCPATHS[0],run,tgt.Data()));
	if(tt->GetEntries()==0){return nullptr;}
	else{cout << "adding " <<Form("%smc%d.root",MCPATHS[0],run)<<"\n";}
	return tt;
}
// get rootfile path

TString GetPath(Int_t run, Int_t skim=0)
{
    Int_t i=0;
    TString rootpath="unknown";

    TChain* T = 0;
    while (ROOTPATHS[i]) {
  T = LoadRun(run,ROOTPATHS[i++],"T",skim,1);
  if (T) {
   rootpath=ROOTPATHS[i-1];
    return rootpath;
  }
    }

    if (!T)
  cerr << "Can not find ROOT file for run " << run << endl;

    return rootpath;
}

//-----------------------
// Get the THaRun object from Ttree;
//-----------------------
THaRun* GetRunData(TTree* tt)
{
    THaRun* run = 0;
    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
      // cout<<"========Run_Data from rootfile=========\n";
      // run->Print();
      // cout<<"_______________________________________\n";
      return run;
    }

    return 0;
}


// ---------------------------
// Return PS for a given Ttree
// ---------------------------
TArrayI GetPS(TTree* tt)
{
    THaRun* run = GetRunData(tt);
    if (run) {
      TArrayI ps = run->GetParameters()->GetPrescales();
      delete run;
      return ps;
    }
    return -1;

}

Int_t GetPS(TTree* tt,Int_t trigger)
{
    THaRun* run = GetRunData(tt);
    Int_t ps=0;
    if (run) {
      TArrayI ps = GetPS(tt);
      delete run;
      return ps[trigger-1];
    }
    return -1;
}

// ---------------------------
// Return rawdata taken time (as unix /epoch time) for a given Ttree
// t2day = 1: return days since the Tritium cell filled 
// 
// ---------------------------
Int_t GetTimeStamp(TTree* tt, Int_t decay_days=0)
{
    THaRun*   run       = GetRunData(tt);
    TDatime   datetime  = run->GetDate();
    // cout<<datetime.AsString()<<endl;
    Int_t     timestamp = datetime.Convert();    // convert to epoch time 

    if (decay_days==1){
        vector<Int_t> filldate={1508774400};// the first tritium cell filled on 2017.10.23. please add newer date in ascending order
        Int_t days = 0;
        for (Int_t dd: filldate){
          if(timestamp > dd){
            days = (timestamp - dd)/24/3600;
          }
        }
        return days;

    }
    return timestamp;
}



// called by sort function to do descending sort
// bool wayToSort(int i, int j) { return i > j; }
// bool wayToSort(double i, double j) { return i > j; }
bool isGreater(double i, double j)
{
    return i > j;
}



// poisson function for cherenkov PMT # of photons
Double_t pois_fit(Double_t *x, Double_t *par)
{
   Double_t arg = 0;
  // par[1]-->normalization (y-axis), par[2]-->Lambda, par[0]=scale (x-axis)
   arg = x[0]*1.0/par[0];
   // Poisson=Power(par[1],x)/TMath::Gamma(x+1)/TMath::Exp(par[1])
   //        =prob. of occuring x times if the averg. event rate is par[2]
   // x[0]-->k
   Double_t fitval = par[1]*TMath::Poisson(arg,par[2]);
   //   cout<<fitval<<endl;
   return fitval;
}


#endif
