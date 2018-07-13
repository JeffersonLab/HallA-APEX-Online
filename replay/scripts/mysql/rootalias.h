// Useful functions for tritium analysis
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



const char* PATHS[] = {
  "/home/shujie/jlab/MyTritium/Rootfiles/",
  "/volatile/halla/triton/nathaly/Rootfiles/",  
  "/volatile/halla/triton/eep_Rootfiles/pass1/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1_calibration/",
  "/chafs1/work1/tritium/Tritium_Rootfiles/",
  "/chafs1/work1/tritium/Rootfiles/",
  "/volatile/halla/triton/shujie/replay/Rootfiles/",
  "/volatile/halla/triton/Tritium_Rootfiles/",
  "./",
  0
};

const TString mysql_connection = "mysql://halladb/triton-work";
const TString mysql_user       = "triton-user";
const TString mysql_password   = "3He3Hdata";


const double pi=3.1415926535897932;
const double rad=pi/180.0;

//-----------LHRS-----------------------------------------
const double beta_min_L=0.5;
const double sh_min_L=0.8;
const double cer_min_L=2000;

TCut sh_cut_L       = Form("(L.prl1.e+L.prl2.e)>HacL_D1_P0rb*1000*%g",sh_min_L);
TCut cer_cut_L      = Form("L.cer.asum_c>%g",cer_min_L);
TCut beta_cut_L     = Form("L.tr.beta>%g",beta_min_L);
TCut electron_cut_L = cer_cut_L+sh_cut_L+beta_min_L;
TCut track_L        = "L.tr.n==1";
TCut aperture_L     = "((L.tr.tg_y+L.tr.tg_ph*1.5)^2/0.64+(L.tr.tg_th*1.5)^2/0.49)<0.01";
const double tg_dp_L=0.05;
const double tg_th_L=0.07;
const double tg_ph_L=0.05;
const double tg_vz_L=0.07;
TCut dp_cut_L=Form("fabs(L.tr.tg_dp)<%g",tg_dp_L);
TCut th_cut_L=Form("fabs(L.tr.tg_th)<%g",tg_th_L);
TCut ph_cut_L=Form("fabs(L.tr.tg_ph)<%g",tg_ph_L);
TCut z_cut_L =Form("fabs(L.tr.vz)<%g",tg_vz_L);
TCut acc_cut =dp_cut_L+th_cut_L+ph_cut_L+z_cut_L+aperture_L;
const double tg_dp_L_e=0.035;
const double tg_th_L_e=0.035;
const double tg_ph_L_e=0.02;
TCut dp_cut_L_e  = Form("fabs(L.tr.tg_dp)<%g",tg_dp_L_e);
TCut th_cut_L_e  = Form("fabs(L.tr.tg_th)<%g",tg_th_L_e);
TCut ph_cut_L_e  = Form("fabs(L.tr.tg_ph)<%g",tg_ph_L_e);
TCut acc_cut_L_e = dp_cut_L_e+th_cut_L_e+ph_cut_L_e+z_cut_L+aperture_L;

//--------------RHRS-------------------------------------
const double beta_min_R=0.6;
const double sh_min_R=0.8;
const double cer_min_R=2000;

TCut sh_cut_R       = Form("(R.ps.e+R.sh.e)>HacR_D1_P0rb*1000*%g",sh_min_R);
TCut cer_cut_R      = Form("R.cer.asum_c>%g",cer_min_R);
TCut beta_cut_R     = Form("R.tr.beta>%g",beta_min_R);
TCut electron_cut_R = cer_cut_R+sh_cut_R+beta_min_R;
TCut track_R        = "R.tr.n==1";

//TCut aperture_L="((L.tr.tg_y+L.tr.tg_ph*1.5)^2/0.64+(L.tr.tg_th*1.5)^2/0.49)<0.01";
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



struct CODASetting {
  TString experiment; //EP, SRC,MARATHON
  TString arm; // Left, Right
  TString side;// L, R
  TString HRS; // LHRS, RHRS
  TString trigger;
  Int_t   bit; // 2, 5
};

CODASetting GetCODASetting(Int_t runnum){

  CODASetting coda;
  coda.side = "L";
  if(runnum<50000){
    coda.arm            = "Left";
    coda.HRS            = "LHRS";
    coda.trigger        = "DL.bit2";
    coda.bit            = 2;
    if(runnum>1000 && runnum<3000)
      coda.experiment        = "MARATHON";
    else coda.experiment     = "SRC";
  }
  else if(runnum<100000) {
    coda.side           = "R";
    coda.arm            = "Right"; 
    coda.HRS            = "RHRS";
    coda.trigger        = "DR.bit5";
    coda.bit            = 5;
    if(runnum<93000)
      coda.experiment     = "MARATHON";
    else
      coda.experiment     = "SRC";
  }
  else{ // for LHRS in coinc mode
      coda.arm            = "Right"; // uses rhrs scaler
      coda.HRS            = "LHRS";
      coda.trigger        = "DR.bit2";
      coda.experiment     = "EP";
      coda.bit            = 2;

  }
  return coda;
}

//-----------------------------
//-----------------------------
struct AnalysisInfo {
  TString  elist      =  '0'; // location for beamtrip cut elist
  TString  trigger    =  '0'; //
  Double_t current    =  -1;
  Double_t charge     =  -1;
  Double_t livetime   =  -1;
  Int_t    ntrigger   =  -1;
  Int_t    ntriggered =  -1;
  Int_t    status     =   0;
};

Int_t GetNCurrents(Int_t runnum){
  CODASetting    coda     = GetCODASetting(runnum);
  AnalysisInfo   runinfo;
  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString        query    = Form("select current, charge from %sanalysis where run_number=%d order by current desc", coda.experiment.Data(),runnum);
  TSQLResult*    result   = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  TSQLRow*       row;
  Int_t   nrows = result->GetRowCount(); 
  Int_t nfields = result->GetFieldCount();
  
  printf("%20s", "current_id");
  for (Int_t i = 0; i < nfields; i++)
    printf("%20s", result->GetFieldName(i));
  cout<<endl;
 for (Int_t j=0; j<nrows; j++){
    row  = result->Next(); // load row for the corresponding current
    printf("%20d", j);
    for (Int_t i = 0; i < nfields; i++)
      printf("%20s", row->GetField(i));
    cout<<endl;
  }
  return nrows;
}

AnalysisInfo GetAnalysisInfo(Int_t runnum, Int_t current_id=0){
  CODASetting    coda     = GetCODASetting(runnum);
  AnalysisInfo   runinfo;
  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString        query    = Form("select * from %sanalysis where run_number=%d order by current desc", coda.experiment.Data(),runnum);
  TSQLResult*    result   = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  // Int_t nrows = result->GetRowCount();
  Int_t nrows = GetNCurrents(runnum); 
  if(nrows==0){
    cout<<"Error: Can't find run "<<runnum<<" in the table "<<coda.experiment<<"analysis"<<endl;
    runinfo.status = -1;
    return runinfo;
  }
  else if(current_id >= nrows){
    cout<<"Error: only "<<nrows<<" available current values for run "<<endl;
    runinfo.status = -1;
    return runinfo;

  }
  TSQLRow *row;
  for (Int_t i=0; i<=current_id; i++){
    row  = result->Next(); // load row for the corresponding current
  }

  runinfo.current    = atof(row->GetField(1)); // get the second column (current)
  runinfo.charge     = atof(row->GetField(2)); // get the third  column (charge )
  runinfo.trigger    = row->GetField(3); 
  runinfo.livetime   = atof(row->GetField(4)); 
  runinfo.ntrigger   = atoi(row->GetField(5)); 
  runinfo.ntriggered = atoi(row->GetField(6)); 
  runinfo.elist      = row->GetField(7); 
  runinfo.status     = 1;
  return runinfo;

}



  //---------------
  // get bcm info from SQL database
  //--------------

struct BCMInfo {
  // use dnew from 2018.1 as default setting
  TString  name      =  "dnew"; 
  TString  HRS       =  "LHRS";
  Double_t gain      = 0.0003358;
  Double_t gain_err  = 2.74e-6;
  Double_t offset    = -0.09753;
  Double_t offset_err= 0.09938;
};

BCMInfo GetBCMInfo(Int_t runnum, TString bcm_name="dnew"){
  CODASetting coda    = GetCODASetting(runnum);
  BCMInfo     bcm;

  TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  TString  query1;
  // find the latest bcm calibration results from database
  query1=Form("select * from bcm where HRS='%s' and name='%s' and date<(select start_time from `%srunlist` where run_number=%d) order by date desc",coda.HRS.Data(),bcm_name.Data(), coda.experiment.Data(),runnum); // 
  TSQLResult* result1=Server1->Query(query1.Data());
  Server1->Close();
  // skip the run if it's not on the runlist
  if(result1->GetRowCount()==0){
    cout<<"Error: can not find matched BCM information, please check your runlist!"<<endl;
    cout<<"Will use default setting: LHRS dnew calibration from Jan 2018"<<endl;
    return bcm;
  }

  // get the first row ( should be before and closest to the run datetime)
  TSQLRow *row1 =    result1->Next();
  bcm.HRS       =    row1->GetField(1);
  bcm.name      =    row1->GetField(2);
  bcm.gain      =    atof(row1->GetField(3));
  bcm.gain_err  =    atof(row1->GetField(4));
  bcm.offset    =    atof(row1->GetField(5));
  bcm.offset_err=    atof(row1->GetField(6));

  return bcm;
}



//--------------------------------
// Chain rootfiles given run number, taken from Longwu Ou's GMP code
//--------------------------------

TChain* LoadRun(Int_t run, const char* path, const char* tree, Int_t debug)
{
  TChain* tt       = new TChain(tree);
  TString basename = Form("tritium_%d",run);
  TString rootfile = basename + ".root";
  TString dir      = path;
  Long_t  split    = 0;

  if (!dir.EndsWith("/")) dir.Append("/");
  rootfile.Prepend(dir.Data());

  while ( !gSystem->AccessPathName(rootfile.Data()) ) {
    tt->Add(rootfile.Data());
    cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
    split++;
    rootfile = basename + "_" + split + ".root";
    rootfile.Prepend(dir.Data());
  }

  if (split<=0) {
	 if (debug>0) cerr << "Can not find ROOT file for run " << run << endl;
	 delete tt;
	 tt = 0;
  }

  return tt;
}

// Load TTree "tree" form all the ROOT files generated from a specific run
// The ROOT files are searched for from the given "PATHS" array given at the beginning of the file
//      run  --- run number
//      tree --- The name of the tree to be loaded
TChain* LoadRun(Int_t run, const char* tree = "T")
{
  Int_t   i  = 0;
  TChain* tt = 0;
  while (PATHS[i]) {
	 tt = LoadRun(run,PATHS[i++],tree,0);
	 if (tt) break;
  }
//    if (!tt)
//	cerr << "Can not find ROOT file for run " << run << endl;
  return tt;
}

//----------------------------
// Load online replay
//----------------------------


TChain* LoadOnline(Int_t run, const char* path, const char* tree,Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString  basename = Form("tritium_online_%d",run);
    TString  rootfile = basename + ".root";
    
    TString dir = path;
    if (!dir.EndsWith("/")) dir.Append("/");

    rootfile.Prepend(dir.Data());

    Long_t split = 0;

    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
	tt->Add(rootfile.Data());
	cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
	split++;
	rootfile = basename + "_" + split + ".root";
	rootfile.Prepend(dir.Data());

    }

    if (split<=0) {
	if (debug>0) cerr << "Can not find online replay file for run " << run << endl;
	delete tt;
	tt = 0;
    }

    return tt;
}

TChain* LoadOnline(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (PATHS[i]) {
	tt = LoadOnline(run,PATHS[i++],tree,1);

	if (tt) break;
    }
   
 //    if (!tt)
	// cerr << "Can not find online replay file for run " << run << endl;

    return tt;
}



// Return PS of a given run
TArrayI GetPS(TTree* tt)
{
  //  TDatime run_time("2018-01-01 00:00:00");
 
    THaRun* run = 0;
    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TArrayI ps;
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
      ps = run->GetParameters()->GetPrescales();
      delete run;
    }

    return ps;
}

Int_t GetPS(TTree* tt,Int_t trigger)
{
  //  TDatime run_time("2018-01-01 00:00:00");

    THaRun* run = 0;
    Int_t ps;
    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
       ps = run->GetParameters()->GetPrescales()[trigger-1];
      delete run;
    }
    return ps;
}


// get rootfile path

TString GetPath(Int_t run)
{
    Int_t i=0;
    TString rootpath="unknown";

    TChain* T = 0;
    while (PATHS[i]) {
  T = LoadRun(run,PATHS[i++],"T",0);
  if (T) {
   rootpath=PATHS[i-1];
    return rootpath;
  }
    }

    if (!T)
  cerr << "Can not find ROOT file for run " << run << endl;

    return rootpath;
}


//-------------------------------------------
//get target info from epics encoder position
// works for tritium since 2018.1
//----------------------------------------- 

struct TargetInfo
{
  TString name     = "unknown";
  TString type     = "unknown";
  Int_t   pos      = -999;
  Int_t   pos_err  = 0;
};


TargetInfo GetTargetInfo(TString name, Int_t pos=-999){
  TargetInfo   target;
  TString      query;
  if (pos == -999)
    query = Form("select * from TargetInfo where name='%s' order by time desc", name.Data());
  else
    query = Form("select * from TargetInfo where abs(%d-encoder)<encoder_err order by time desc", pos);

  TSQLServer* Server = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TSQLResult* result = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  if(result->GetRowCount()==0){
    cout<<"Error: can not find matched target in TargetInfo table"<<endl;
    return target;
  }
  TSQLRow *row    = result->Next(); // load first row of results
  target.name     = row->GetField(1); 
  target.type     = row->GetField(2); 
  target.pos      = atoi(row->GetField(3)); 
  target.pos_err  = atoi(row->GetField(4)); 
  return target;

}

TargetInfo GetTarget(TChain *tree2){
  Double_t   pos,i=0;
  Int_t      nn = tree2->GetEntries(); 
  tree2->SetBranchAddress("haBDSPOS",&pos);
  tree2->GetEntry(Int_t(nn/2)); 
  TargetInfo target = GetTargetInfo("",pos);

  cout<<pos<<"  "<<target.name<<endl;
  return target;
}

TargetInfo GetTarget(Int_t run)
{
  TargetInfo target;
  TChain*    etree  = LoadRun(run,"E");
  if(!etree) etree  = LoadOnline(run,"E");
  if(!etree) {
    cout<<"Can't find rootfile for run "<<run<<endl;
    return target;
  }
  return GetTarget(etree);
  
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
