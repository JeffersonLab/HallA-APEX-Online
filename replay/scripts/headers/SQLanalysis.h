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
#include "TString.h"
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
  Double_t lumi      = 0; // luminosity per uA
  Double_t dens_par0 = 1;
  Double_t dens_par1 = 0;
  Double_t dens_err1 = 0;
  Double_t dens_par2 = 0;
  Double_t dens_err2 = 0;
  Int_t    tarid     = -1;
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
  target.tarid    = atoi(row->GetField(15)); 
  if(target.type=="gas"){
    target.dens_par1 = atof(row->GetField(6)); 
    target.dens_err1 = atof(row->GetField(7)); 
    target.dens_par2 = atof(row->GetField(8)); 
    target.dens_err2 = atof(row->GetField(9));     
  }
  // get luminosity
  Double_t rho =  atof(row->GetField(10)); 
  if(rho>0){
    Double_t avg = 6.02e23;                     // N/mol
    Double_t I   = 1.0;                           // uA = 1e-6 A = 1e-6 C/sec
    Double_t Ne  = 1.0/(1.602e-19) ;               // electrons / C
    Double_t amu = atof(row->GetField(14));     // g/mol
    Double_t A   = atof(row->GetField(12));     // g/mol
    target.lumi  = rho * avg/amu * A * I *1e-6 *Ne; // #/cm2, for QE and DIS
    target.lumi  = target.lumi /1e-4 * 1e-37 ;  // cm2 to nb
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


//----------------------
// Run condition
//----------------------
struct RunInfo {
  // use dnew from 2018.1 as default setting
  TString  kinid     =  "unknown"; 
  TString  type      =  "unknown";
  TString  quality   =  "unknown";
};

RunInfo GetRunInfo(Int_t runnum){
  CODASetting coda    = GetCODASetting(runnum);
  RunInfo     runinfo;

  TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  TString  query1;
  // find the latest bcm calibration results from database
  query1=Form("select kin,run_type,quality from `%srunlist` where run_number=%d", coda.experiment.Data(),runnum); // 
  TSQLResult* result1=Server1->Query(query1.Data());
  Server1->Close();
  // skip the run if it's not on the runlist
  if(result1==0){
    cout<<"Error: can not find column kin in "<<coda.experiment.Data()<<"runlist!"<<endl;
    return runinfo;

  }
  if(result1->GetRowCount()==0){
    cout<<"Error: can not find run "<<runnum<<" in "<<coda.experiment.Data()<<"runlist!"<<endl;
    return runinfo;
  }

  // get the first row 
  TSQLRow *row1   =    result1->Next();
  runinfo.kinid   =    row1->GetField(0);
  runinfo.type    =    row1->GetField(1);
  runinfo.quality =    row1->GetField(2);

  runinfo.kinid.ToLower();
  runinfo.type.ToLower();
  runinfo.quality.ToLower();

  return runinfo;
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
  TString  kinid      =  "unknown"; 
  Int_t    tarid      =  -1;
};

Int_t GetNCurrents(Int_t runnum, Int_t verb=1){
  CODASetting    coda     = GetCODASetting(runnum);
  AnalysisInfo   runinfo;
  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString        query    = Form("select current, charge from %sanalysis where run_number=%d order by current desc", coda.experiment.Data(),runnum);
  TSQLResult*    result   = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  Int_t   nrows = result->GetRowCount(); 
  Int_t nfields = result->GetFieldCount();
  
  if(verb) printf("%20s", "current_id");
  for (Int_t i = 0; i < nfields; i++)
    if(verb) printf("%20s", result->GetFieldName(i));
  if(verb) cout<<endl;
 for (Int_t j=0; j<nrows; j++){
    TSQLRow*    row  = result->Next(); // load row for the corresponding current
    if(verb) printf("%20d", j);
    for (Int_t i = 0; i < nfields; i++)
      if(verb) printf("%20s", row->GetField(i));
    if(verb) cout<<endl;
  }
  return nrows;
}

AnalysisInfo GetAnalysisInfo(Int_t runnum, Int_t current_id=0, Int_t verb=1){
  CODASetting    coda     = GetCODASetting(runnum);
  TargetInfo     target   = GetTarget(runnum);
  AnalysisInfo   ana;
  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString        query    = Form("select * from %sanalysis where run_number=%d order by current desc", coda.experiment.Data(),runnum);
  TSQLResult*    result   = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!
  // Int_t nrows = result->GetRowCount();
  Int_t nrows = GetNCurrents(runnum,verb); 
  if(nrows==0){
    cout<<"Error: Can't find run "<<runnum<<" in the table "<<coda.experiment<<"analysis"<<endl;
    ana.status = -1;
    return ana;
  }
  else if(current_id >= nrows){
    cout<<"Error: only "<<nrows<<" available current values for run "<<endl;
    ana.status = -1;
    return ana;

  }
  TSQLRow *row = nullptr;
  for (Int_t i=0; i<=current_id; i++){
    row  = result->Next(); // load row for the corresponding current
  }

  ana.current    = atof(row->GetField(1)); // get the second column (current)
  ana.charge     = atof(row->GetField(2)); // get the third  column (charge )
  ana.trigger    = row->GetField(3); 
  ana.livetime   = atof(row->GetField(4)); 
  ana.ntrigger   = atoi(row->GetField(5)); 
  ana.ntriggered = atoi(row->GetField(6)); 
  ana.elist      = row->GetField(7); 
  ana.status     = 1;
  ana.kinid      = GetRunInfo(runnum).kinid;

// calculate density correction factor (boiling)
  ana.target = target.name;
  if(target.type=="gas"){
    ana.dens_cor = 1 + target.dens_par1 * ana.current + target.dens_par2 * ana.current*ana.current;
  }

// tarid in simulation
  ana.tarid = target.tarid;

  return ana;

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
  	"/w/halla-scifs17exp/triton/SRC_elist/"+listname,
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

//////////////////////////////////

//A few quick very user friend queries for SQL list.
//
//////////////
class RunList {
	public:
	int runnumber;
	string kin;
	void set_values(int a, string b);
	void print();
};

	void RunList::set_values(int a, string b){
		runnumber=a;
		kin=b;
	}
	void RunList::print(){
		cout << "Run number : " << runnumber << "  Kinematic " << kin <<endl;
	}

vector<int> RLtoint(vector<RunList> a, string b=""){
	vector<int> runlist;
	for(unsigned int i=0;i<a.size();i++){
		if(b==""){runlist.push_back(a[i].runnumber);}
		else{
			if(a[i].kin==b)runlist.push_back(a[i].runnumber);
		}
	}
	return runlist;
}

 
///////////////


//This will make a Class RunList output from the query to give you the choice to look at all the kinemtaics of one type. 
vector<RunList> SQL_Kin_Target_RL(TString kin="", TString tgt=""){
	vector<RunList> runlist;
	
	if(kin =="" || tgt=="")
	{
		cout << "Please use this function with (kin,tgt),,, example (1,Tritium)" <<"\n\n";
		return runlist;
	}
	if(tgt=="H3"||tgt=="T2") tgt = "Tritium";
	else if(tgt=="D2")  tgt = "Deuterium";
	else if(tgt=="He3") tgt = "Helium-3";
	else if(tgt=="EM")  tgt = "Empty Cell";
	else if(tgt=="DM")  tgt = "25 cm Dummy";
	else if(tgt=="CH")  tgt = "Carbon Hole";

	/////Make a SQL querey in 	
        TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  	TString  query1;
     	query1=Form("select run_number, Kinematic , prescale_T2, prescale_T5 from MARATHONrunlist where (Kinematic = '%s' or Kinematic like '%s[_]') and target='%s' order by run_number asc",kin.Data(),kin.Data(),tgt.Data());  
       TSQLResult* result1=Server1->Query(query1.Data());
       Server1->Close();
	
	if(result1->GetRowCount()==0){ 
		cout <<"Sorry could not find that kin tgt" <<"\n";
		return runlist;
	}
	RunList tmp;
	TSQLRow *row1;
	for(int i =0; i<result1->GetRowCount();i++){
		row1 =  result1->Next();	
		tmp.set_values(atoi(row1->GetField(0)),row1->GetField(1));
		if((atoi(row1->GetField(2))+atoi(row1->GetField(3)))>0){//make sure main trigger fired
			runlist.push_back(tmp);}

	}
	return runlist;
}

vector<int> SQL_Kin_Target(TString kin="", TString tgt=""){
	vector<int> runlist;
	if(kin =="" || tgt=="")
	{
		cout << "Please use this function with (kin,tgt),,, example (1,Tritium)" <<"\n\n";
		return runlist;
	}
	if(tgt=="H3"||tgt=="T2") tgt = "Tritium";
	else if(tgt=="D2")  tgt = "Deuterium";
	else if(tgt=="He3") tgt = "Helium-3";
	else if(tgt=="EM")  tgt = "Empty Cell";
	else if(tgt=="DM")  tgt = "25 cm Dummy";
	else if(tgt=="CH")  tgt = "Carbon Hole";

	/////Make a SQL querey in 	
        TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  	TString  query1;
     	query1=Form("select run_number from MARATHONrunlist where Kinematic='%s' and target='%s' order by run_number asc",kin.Data(),tgt.Data());  
       TSQLResult* result1=Server1->Query(query1.Data());
       Server1->Close();
	
	if(result1->GetRowCount()==0){ 
		cout <<"Sorry could not find that kin tgt" <<"\n";
		return runlist;
	}
	TSQLRow *row1;
	for(int i =0; i<result1->GetRowCount();i++){
		row1 =  result1->Next();	
		runlist.push_back(atoi(row1->GetField(0)));

	}
	return runlist;
}

double Binomial_Error(double p, int n)
{
        return sqrt(p*(1-p)/(n*1.0));
}


//////////////////////////////////////////////////
//Use the SQL analysis DB to get all the corrections value
double SQLRunCorrection(int runnum, double &correction_error, int correction_Level=1,  int debug=1)
{
///These will be added as I get more into the DB
string cols ="livetime, trigger_counts, trigger_events";
/// livetime DAQ LT
//  trigger counte and events used in binomal error cal for livetime;

  double correction=1.0;
  correction_error=0.0;
  CODASetting   coda     = GetCODASetting(runnum);
  AnalysisInfo  runinfo;
  TSQLServer*  	Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString 	query    = Form("select %s from %sanalysis where run_number=%d",cols.c_str(), coda.experiment.Data(),runnum);
  TSQLResult*   result   = Server->Query(query.Data());
  Server->Close();
  if(result->GetRowCount()==0){cout << "No corrections "<<"\n"; return correction;} 
  TSQLRow *row = result->Next();
/////I can Add these in as I get more corrections into the DB
 int count = result->GetFieldCount();
/* for(int i=0; i<count;i++){
    	cout<<result->GetFieldName(i)<<endl;
	}*/
//Livetime
 correction *= atof(row->GetField(0));
 correction_error = Binomial_Error(correction,atoi(row->GetField(1)));
///
  return correction;
}



int SQLRunlistStatus(int runnum)
{
  CODASetting   coda     = GetCODASetting(runnum);
  AnalysisInfo  runinfo;
  TSQLServer*   Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString       query    = Form("select run_number from %srunlist where run_number=%d", coda.experiment.Data(),runnum);
  TSQLResult*   result   = Server->Query(query.Data());
  Server->Close();
  if(result->GetRowCount()==0){cout << "Run list does not contain this run "<<"\n"; return 0;}
  else{return 1;}
}


struct RunInfo{
	int runnum=0;
	TString target="";
	TString type="";
	TString kinematic="";
	double time_mins=0.0;
	int PS_main=0;
	int good_run=0;
};


RunInfo GetRunInfo(int runnum){
	RunInfo runinfo;
	CODASetting   coda     = GetCODASetting(runnum);
  	TSQLServer*   Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  	TString       query    = Form("select run_number,target,run_type,Kinematic,time_mins, prescale_T%d from %srunlist where run_number=%d", coda.bit,coda.experiment.Data(),runnum);
  	TSQLResult*   result   = Server->Query(query.Data());
	Server->Close();
  	if(result->GetRowCount()==0){cout << "Not in list\n";return runinfo;}
	TSQLRow *row = result->Next();	
	runinfo.runnum=runnum;
	runinfo.target=row->GetField(1);
	runinfo.type=row->GetField(2);
	runinfo.kinematic=row->GetField(3);
//	cout<< row->GetField(4) <<endl;
	if(row->GetField(4)==nullptr){runinfo.time_mins=1000000000000;}
	else{runinfo.time_mins=atof(row->GetField(4));}
	runinfo.PS_main=atoi(row->GetField(5));	
	if(runinfo.PS_main>0){
		if(runinfo.type!="Cosmic" && runinfo.time_mins>2.0){
			runinfo.good_run=1;}
	}
	return runinfo;
}

void PrintRunInfo(int run){
	RunInfo RI= GetRunInfo(run);
	cout << Form("This the run info for run %d\n", RI.runnum);
	cout<< "target = " <<RI.target <<"\ttype ="<<RI.type<<"\n";
	cout<< "time in mins:"<<RI.time_mins<<"\tPS main trigger " <<RI.PS_main<<"\n";
	} 





#endif
