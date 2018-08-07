// For tritium
// analysis routines working with SQL database
// -shujie 07,2018

#ifndef SQLANALYSIS_H
#define SQLANALYSIS_H


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
#include "rootalias.h"

const TString mysql_connection = "mysql://halladb/triton-work";
const TString mysql_user       = "triton-user";
const TString mysql_password   = "3He3Hdata";




//----------------------
// CODA related setting
//----------------------

struct CODASetting {
  TString experiment = "unknown"; //EP, SRC,MARATHON
  TString tsscaler   = ""; // Left,   Right
  TString evscaler   = ""; // evLeft, evRight
  TString arm        = ""; // L, R (electron arm)
  TString trigger    = "";
  Int_t   bit        = 0; // 2, 5
};

CODASetting GetCODASetting(Int_t runnum, Int_t sql=1){

  CODASetting coda;
  if(sql){
    TSQLServer* Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
    TString     query    = Form("select * from coda where `first_run`<%d and `last_run`>%d", runnum, runnum);
    TSQLResult* result   = Server->Query(query.Data());
    Server->Close();// Always remember to CLOSE the connection!

    Int_t       nrows    = result->GetRowCount(); 
    if(nrows==0){
      cout<< "Error: run "<<runnum<<" does not match any experiment run range in the table <coda>, will use the hard coded setting";
    }  
    else{
    TSQLRow*    row      = result->Next(); // load row for the corresponding current
    coda.experiment = row->GetField(2);
    coda.tsscaler   = row->GetField(3);
    coda.evscaler   = row->GetField(4);
    coda.arm        = row->GetField(5);
    coda.trigger    = row->GetField(6);
    coda.bit        = atoi(row->GetField(7));
    return coda;
    }
  }

  coda.arm = "L";
  if(runnum<50000){
    coda.arm            = "L";
    coda.tsscaler       = "Left";
    coda.evscaler       = "Right";
    coda.trigger        = "DL.bit2";
    coda.bit            = 2;
    if(runnum>1000 && runnum<3000)
      coda.experiment        = "MARATHON";
    else coda.experiment     = "SRC";
  }
  else if(runnum<100000) {
    coda.arm           = "R";
    coda.tsscaler      = "Right"; 
    coda.evscaler      = "evRight"; 
    coda.trigger       = "DR.bit5";
    coda.bit           = 5;
    if(runnum<93000)
      coda.experiment     = "MARATHON";
    else
      coda.experiment     = "SRC";
  }
  else{ // for LHRS in coinc mode
      coda.tsscaler       = "Right"; // uses rhrs scaler
      coda.evscaler       = "evRight"; // uses rhrs scaler
      coda.arm            = "L"; // electron arm
      coda.trigger        = "DR.bit2";
      coda.experiment     = "EP";
      coda.bit            = 2;
  }
  return coda;
}


//-------------------------------------------
//get target info from epics encoder position
// works for tritium since 2018.1
//----------------------------------------- 

struct TargetInfo
{
  TString  name      = "unknown";
  TString  type      = "unknown";
  Int_t    pos       = -999;
  Int_t    pos_err   = 0;
  Double_t dens_par0 = 1;
  Double_t dens_par1 = 0;
  Double_t dens_err1 = 0;
  Double_t dens_par2 = 0;
  Double_t dens_err2 = 0;
};


TargetInfo GetTargetInfo(TString name, Int_t pos=-999, Int_t runnum=0){
  TargetInfo     target;
  TString        query;
  CODASetting    coda     = GetCODASetting(runnum);

  
  if (pos == -999){ 
    if(runnum>0)// find target name from runlist by runnumber, then get the target info from matching run date
      query = Form("select * from TargetInfo where name=(select target from %srunlist where run_number=%d)  and time<(select start_time from `%srunlist` where run_number=%d) order by time desc", coda.experiment.Data(), runnum, coda.experiment.Data(),runnum);
    else // get latest target info given target name
      query = Form("select * from TargetInfo where name='%s' order by time desc", name.Data());
    }
  else{ // find target info given position
    if(runnum>0)// use run number to locate run date
      query = Form("select * from TargetInfo where abs(%d-encoder)<encoder_err and time<(select start_time from `%srunlist` where run_number=%d) order by time desc", pos, coda.experiment.Data(),runnum);
    else // no time info, use the latest target info
      query = Form("select * from TargetInfo where abs(%d-encoder)<encoder_err order by time desc", pos);
  }
  TSQLServer* Server = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TSQLResult* result = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  if(result->GetRowCount()==0){
    cout<<"Error: can not find matched target information from TargetInfo table"<<endl;
    target.pos = pos;
    return target;
  }
  TSQLRow *row    = result->Next(); // load first row of results
  target.name     = row->GetField(1); 
  target.type     = row->GetField(2); 
  target.pos      = atoi(row->GetField(3)); 
  target.pos_err  = atoi(row->GetField(4)); 
  if(target.type=="gas"){
    target.dens_par1 = atof(row->GetField(6)); 
    target.dens_err1 = atof(row->GetField(7)); 
    target.dens_par2 = atof(row->GetField(8)); 
    target.dens_err2 = atof(row->GetField(9)); 
  }

  return target;

}

TargetInfo GetTarget(Int_t run)
{
  Double_t   position;
  TChain*    etree  = LoadRun(run,"E");
  if(!etree) etree  = LoadOnline(run,"E");
  if(!etree) {
    cout<<"Can't find rootfile for run "<<run<<", will use target name from runlist!"<<endl;
    position = -999;
  }
  else{ // identify target by encoder position, then get specs from TargetInfo table
    Double_t   pos;
    Int_t      nn     = etree->GetEntries(); 
    etree->SetBranchAddress("haBDSPOS",&pos);
    etree->GetEntry(Int_t(nn/2));
    position = pos; 
  }
  TargetInfo target = GetTargetInfo("",position,run);
  return target;
}

//-----------------------------
// Get analysis info from SQL table <experiment>analysis
//-----------------------------
struct AnalysisInfo {
  TString  elist      =  '0'; // location for beamtrip cut elist
  TString  trigger    =  '0'; //
  TString  target     =  "unknown"; 
  Double_t current    =  -1;
  Double_t charge     =  -1;
  Double_t livetime   =  -1;
  Int_t    ntrigger   =  -1;
  Int_t    ntriggered =  -1;
  Double_t dens_cor   =   1;
  Double_t dens_err   =   0;
  Int_t    status     =   0;
};

Int_t GetNCurrents(Int_t runnum){
  CODASetting    coda     = GetCODASetting(runnum);
  AnalysisInfo   runinfo;
  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString        query    = Form("select current, charge from %sanalysis where run_number=%d order by current desc", coda.experiment.Data(),runnum);
  TSQLResult*    result   = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  Int_t   nrows = result->GetRowCount(); 
  Int_t nfields = result->GetFieldCount();
  
  printf("%20s", "current_id");
  for (Int_t i = 0; i < nfields; i++)
    printf("%20s", result->GetFieldName(i));
  cout<<endl;
 for (Int_t j=0; j<nrows; j++){
    TSQLRow*    row  = result->Next(); // load row for the corresponding current
    printf("%20d", j);
    for (Int_t i = 0; i < nfields; i++)
      printf("%20s", row->GetField(i));
    cout<<endl;
  }
  return nrows;
}

AnalysisInfo GetAnalysisInfo(Int_t runnum, Int_t current_id=0){
  CODASetting    coda     = GetCODASetting(runnum);
  TargetInfo     target   = GetTarget(runnum);
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
  TSQLRow *row = nullptr;
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

// calculate density correction factor (boiling)
  runinfo.target = target.name;
  if(target.type=="gas"){
    runinfo.dens_cor = 1 + target.dens_par1 * runinfo.current + target.dens_par2 * runinfo.current*runinfo.current;
  }

  return runinfo;

}

  //---------------
  // get bcm info from SQL table bcm
  //--------------

struct BCMInfo {
  // use dnew from 2018.1 as default setting
  TString  name      =  "dnew"; 
  TString  scaler    =  "evLeft";
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
  query1=Form("select * from bcm where scaler='%s' and name='%s' and date<(select start_time from `%srunlist` where run_number=%d) order by date desc",coda.evscaler.Data(),bcm_name.Data(), coda.experiment.Data(),runnum); // 
  TSQLResult* result1=Server1->Query(query1.Data());
  Server1->Close();
  // skip the run if it's not on the runlist
  if(result1->GetRowCount()==0){
    cout<<"Error: can not find matched BCM information, please check your runlist!"<<endl;
    cout<<"Will use default setting: LHRS scaler dnew calibration from Jan 2018"<<endl;
    return bcm;
  }

  // get the first row ( should be before and closest to the run datetime)
  TSQLRow *row1 =    result1->Next();
  bcm.scaler    =    row1->GetField(1);
  bcm.name      =    row1->GetField(2);
  bcm.gain      =    atof(row1->GetField(3));
  bcm.gain_err  =    atof(row1->GetField(4));
  bcm.offset    =    atof(row1->GetField(5));
  bcm.offset_err=    atof(row1->GetField(6));

  return bcm;
}



  //----------------------
  // load events after beamtrip cut
  //----------------------

TEventList* GetList(TString path){
  TFile  *f  = new TFile(path,"read");
  if (!f->IsZombie()){
  	cout<< "will load eventlist from "<<path.Data()<<endl;
  	TEventList* elist = (TEventList*)f->Get("elist");
  	if(elist) elist->SetDirectory(0); //otherwise the file destructor will delete elist
  	return elist;
  }
  return 0;
}

  // highest currrent has id 0, default cut on 5 seconds after beam stablized
TChain *LoadList(Int_t runnum, Int_t current_id=0, Int_t stable_time=5){
  TChain*         t    = LoadRun(runnum);
  // read info from SQL 
  AnalysisInfo    ana  = GetAnalysisInfo(runnum, current_id);
  CODASetting     coda = GetCODASetting(runnum);
  if(ana.status < 1){
    cout<<"Error: can not find run "<<runnum<<" in analysis table! No beamtrip cut applied\n";
    return t;
  } 
  // load eventlist after beamtrip cut
  TString listname = Form("bcm%d_%d_%d.root",runnum,current_id,stable_time);
  TString LISTPATHS[] = {
	ana.elist,
	"./elist/"+listname,
	"beam/beamtrip/elist/"+listname,
	"~/jlab/MyTritium/Rootfiles/"+listname,
	""
	};

  Int_t       i     = 0;
  TEventList* elist = 0;
  while (LISTPATHS[i].Length()){
  	elist = GetList(LISTPATHS[i++]);
  	if (elist) break;
  }
  t->SetEventList(elist);
  return t;
}


#endif
