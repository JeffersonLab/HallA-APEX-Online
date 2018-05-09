// 
// adapted from Ou,Longwu's GMp code
// -shujie 03,2018

#ifndef ROOTALIAS_H
#define ROOTALIAS_H


#include <string>
#include <stdio.h> 
#include <iomanip>
#include <locale>
#include <algorithm>
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
  "/chafs1/work1/tritium/Tritium_Rootfiles/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1/",
  "/chafs1/work1/tritium/Rootfiles/",
  "/volatile/halla/triton/shujie/replay/Rootfiles/",
  "/volatile/halla/triton/Tritium_Rootfiles/",
  // 
  "./",
  0
};


const double pi=3.1415926535897932;
const double rad=pi/180.0;
const double sh_min_L=0.8;
const double cer_min_L=4000;

 //const double beta_min_L=0.6;
 //TString sh_cut_L=Form("(L.prl1.e+L.prl2.e)>HacL_D1_P0rb*1000*%g",sh_min_L);
 //TString cer_cut_L=Form("L.cer.asum_c>%g",cer_min_L);
 //TString beta_cut_L=Form("L.tr.beta>%g",beta_min_L);
 //TString electron_cut_L=cer_cut_L+"&&"+sh_cut_L+"&&"+beta_cut_L;
 //TString track_L="L.tr.n==1";
 //TString trigger_2_R="DR.bit2>0";
 //TString trigger_2_L="DL.bit2>0";


const double beta_min_L=0.6;
TCut sh_cut_L=Form("(L.prl1.e+L.prl2.e)>HacL_D1_P0rb*1000*%g",sh_min_L);
TCut cer_cut_L=Form("L.cer.asum_c>%g",cer_min_L);
TCut beta_cut_L=Form("L.tr.beta>%g",beta_min_L);
TCut electron_cut_L=cer_cut_L+sh_cut_L+beta_min_L;
TCut track_L="L.tr.n==1";
TCut trigger_2_R="DR.bit2>0";
TCut trigger_2_L="DL.bit2>0";

const double tg_dp_L=0.05;
const double tg_th_L=0.07;
const double tg_ph_L=0.05;
const double tg_vz_L=0.07;
// TString dp_cut_L=Form("abs(L.tr.tg_dp)<%g",tg_dp_L);
// TString th_cut_L=Form("abs(L.tr.tg_th)<%g",tg_th_L);
// TString ph_cut_L=Form("abs(L.tr.tg_ph)<%g",tg_ph_L);
// TString acc_cut=dp_cut_L+"&&"+th_cut_L+"&&"+ph_cut_L;
TCut dp_cut_L=Form("abs(L.tr.tg_dp)<%g",tg_dp_L);
TCut th_cut_L=Form("abs(L.tr.tg_th)<%g",tg_th_L);
TCut ph_cut_L=Form("abs(L.tr.tg_ph)<%g",tg_ph_L);
TCut z_cut_L=Form("abs(L.tr.vz)<%g",tg_vz_L);
TCut acc_cut=dp_cut_L+th_cut_L+ph_cut_L+z_cut_L;
const double tg_dp_L_e=0.035;
const double tg_th_L_e=0.035;
const double tg_ph_L_e=0.02;
// TString dp_cut_L_e=Form("abs(L.tr.tg_dp)<%g",tg_dp_L_e);
// TString th_cut_L_e=Form("abs(L.tr.tg_th)<%g",tg_th_L_e);
// TString ph_cut_L_e=Form("abs(L.tr.tg_ph)<%g",tg_ph_L_e);
// TString z_cut_L="abs(L.tr.vz)<0.07";
// TString acc_e_cut=dp_cut_L_e+"&&"+th_cut_L_e+"&&"+ph_cut_L_e+"&&"+z_cut_L;
TCut dp_cut_L_e=Form("abs(L.tr.tg_dp)<%g",tg_dp_L_e);
TCut th_cut_L_e=Form("abs(L.tr.tg_th)<%g",tg_th_L_e);
TCut ph_cut_L_e=Form("abs(L.tr.tg_ph)<%g",tg_ph_L_e);
TCut acc_e_cut=dp_cut_L_e+th_cut_L_e+ph_cut_L_e+z_cut_L;


struct target
{
  Double_t pos;
  TString name;
};





// called by sort function to do descending sort
bool wayToSort(int i, int j) { return i > j; }
bool wayToSort(double i, double j) { return i > j; }



// poisson function for TF1 fit
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





//-------------------------------------------
//get target info from epics encoder position
// check if target moved during the run
// works for tritium snice 2018.1
//----------------------------------------- 
TString GetTarget(TChain *tree2){
  TString targname;
  target t2={33106235,"Tritium"};
  target d2={29367355,"Deuterium"};
  target h={25610043,"Hydrogen"};
  target he3={21875520,"Helium3"};
  target empty={18119995,"Empty Cell"};
  target dummy={15175217,"Dummy"};
  target optics={14394929,"Multifoils"};
  target hole={13108119,"Carbon Hole"};
  target raster={12444209,"Raster Target"};
  target al={11728945,"Thick Aluminum"};
  target single={11013681,"Single Carbon Foil"};
  target ti={10298417,"Titanium"};
  target beo={9583153,"BeO"};

  Double_t pos,pos1,i=0;
  tree2->SetBranchAddress("haBDSPOS",&pos);
  Int_t nn=tree2->GetEntries(); 
  for(i=0;i<nn;i++){
    tree2->GetEntry(i);     
    if(pos<0) continue;
    pos1=pos;
  } 
  tree2->GetEntry(Int_t(nn-2)); 
  if (pos1!=pos) targname="target moved during the run"; 
  else if(abs(pos)<= 0.000000001 ) {targname ="Unkown";}
  else if(abs(pos)<50) targname="HOME";
  else if(abs(pos-t2.pos)<20000)     targname=t2.name;
  else if(abs(pos-d2.pos)<20000)     targname=d2.name;
  else if(abs(pos-h.pos)<20000)      targname=h.name;
  else if(abs(pos-he3.pos)<20000)    targname=he3.name;
  else if(abs(pos-empty.pos)<20000)  targname=empty.name;
  else if(abs(pos-dummy.pos)<50)  targname=dummy.name;
  else if(abs(pos-optics.pos)<50) targname=optics.name;
  else if(abs(pos-hole.pos)<50)   targname=hole.name;
  else if(abs(pos-raster.pos)<50) targname=raster.name;
  else if(abs(pos-al.pos)<50)     targname=al.name;
  else if(abs(pos-single.pos)<50) targname=single.name;
  else if(abs(pos-ti.pos)<50)     targname=ti.name;
  else if(abs(pos-beo.pos)<50)    targname=beo.name;

  cout<<pos<<"  "<<pos1<<"  "<<targname<<endl;
  return targname;
}

//-------------------------
//   get time duration of the run from timestamp
//------------------------

/* TString GetTarget(TChain *tree2){ */
/*   TString targname; */
/*   target t2={33106235,"Tritium"}; */
/*   target d2={29367355,"Deuterium"}; */
/*   target h={25610043,"Hydrogen"}; */
/*   target he3={21875520,"Helium3"}; */
/*   target empty={18119995,"Empty Cell"}; */
/*   target dummy={15175217,"Dummy"}; */
/*   target optics={14394929,"Multifoils"}; */
/*   target hole={13108119,"Carbon Hole"}; */
/*   target raster={12444209,"Raster Target"}; */
/*   target al={11728945,"Thick Aluminum"}; */
/*   target single={11013681,"Single Carbon Foil"}; */
/*   target ti={10298417,"Titanium"}; */
/*   target beo={9583153,"BeO"}; */

/*   Double_t pos,pos1,i=0; */
/*   tree2->SetBranchAddress("haBDSPOS",&pos); */
/*   Int_t nn=tree2->GetEntries();  */
/*   for(i=0;i<nn;i++){ */
/*     tree2->GetEntry(i);      */
/*     if(pos<0) continue; */
/*     pos1=pos; */
/*   }  */
/*   tree2->GetEntry(Int_t(nn-2));  */
/*   if (pos1!=pos) targname="target moved during the run";  */
/*   else if(abs(pos)<= 0.000000001 ) {targname ="Unkown";} */
/*   else if(abs(pos)<50) targname="HOME"; */
/*   else if(abs(pos-t2.pos)<20000)     targname=t2.name; */
/*   else if(abs(pos-d2.pos)<20000)     targname=d2.name; */
/*   else if(abs(pos-h.pos)<20000)      targname=h.name; */
/*   else if(abs(pos-he3.pos)<20000)    targname=he3.name; */
/*   else if(abs(pos-empty.pos)<20000)  targname=empty.name; */
/*   else if(abs(pos-dummy.pos)<50)  targname=dummy.name; */
/*   else if(abs(pos-optics.pos)<50) targname=optics.name; */
/*   else if(abs(pos-hole.pos)<50)   targname=hole.name; */
/*   else if(abs(pos-raster.pos)<50) targname=raster.name; */
/*   else if(abs(pos-al.pos)<50)     targname=al.name; */
/*   else if(abs(pos-single.pos)<50) targname=single.name; */
/*   else if(abs(pos-ti.pos)<50)     targname=ti.name; */
/*   else if(abs(pos-beo.pos)<50)    targname=beo.name; */

/*   cout<<pos<<"  "<<pos1<<"  "<<targname<<endl; */
/*   return targname; */
/* } */



TChain* LoadRun(Int_t run, const char* path, const char* tree,Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString basename = Form("tritium_%d",run);
    TString rootfile = basename + ".root";
    
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
    Int_t i=0;

    TChain* tt = 0;
    while (PATHS[i]) {
	tt = LoadRun(run,PATHS[i++],tree,1);

	if (tt) break;
    }
   
    if (!tt)
	cerr << "Can not find ROOT file for run " << run << endl;

    return tt;
}

//----------------------------
// Load online replay
//----------------------------


TChain* LoadOnline(Int_t run, const char* path, const char* tree,Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString  basename = Form("tritium_online_%d",run);
    TString rootfile = basename + ".root";
    
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
   
    if (!tt)
	cerr << "Can not find online replay file for run " << run << endl;

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


TString GetTarget(Int_t run)
{
  TChain* etree=LoadRun(run,"E");
  if(!etree) etree=LoadOnline(run,"E");
  TString targname=GetTarget(etree);
  return targname;
  
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

#endif
