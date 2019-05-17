#include "headers/rootalias.h"
#include "headers/SQLanalysis.h"
#include "headers/SRCanalysis.h"
Int_t good_electrons(Int_t runnum, Int_t timecut=5,Int_t current_id=0, Int_t cut_id=0, Int_t xbin=30){

  //Int_t xbin = 30;
  //----------------------
  // load events after beamtrip cut
  //----------------------
  
  // check if the run exists
  RunInfo         info = GetRunInfo(runnum);
  if(info.status==-1) return 0;
   // check if the run is analyzed and is with a production target
  AnalysisInfo     ana = GetAnalysisInfo(runnum, current_id);
 

  // load rootfile
  TChain*            t = LoadList(runnum,current_id);
  CODASetting     coda = GetCODASetting(runnum);
  

  // if(ana.tarid==-1) {
  //    cout<<"run "<<runnum<<" : not production target, skipped\n";
  //    return -1;
  //  }
  //-------------------------
  // get good electrons with selected cut:
  // 0. standard cut (tight)
  // 1. standard + Q2>1.4
  // 2. loose    cut
  // 3. loose    + Q2>1.4
  //-------------------------
 
  TCut trigger  = Form("%s>0",coda.trigger.Data());  
  TCut cut_good_e;
  if(coda.arm      == "L"){
    if(cut_id<2)
      cut_good_e  = acc_cut_L_e + electron_cut_L + track_L + trigger;
    else
      cut_good_e  = acc_cut_L   + electron_cut_L + track_L + trigger;
      }
  else if(coda.arm == "R"){
    if(cut_id<2)
      cut_good_e  = acc_cut_R_e + electron_cut_R + track_R + trigger;
    else
      cut_good_e  = acc_cut_R   + electron_cut_R + track_R + trigger;
  }
  if (cut_id==1 || cut_id==3)
    cut_good_e = cut_good_e + Form("EK%sx.Q2>1.4",coda.arm.Data());

  TH1F *h=new TH1F("h","",xbin,0,3);
  //cout<<"----------------- "<<endl;
  //cout<<"Good Electron Cuts: "<<cut_good_e <<endl;
  t->Project("h",Form("EK%sx.x_bj",coda.arm.Data()),cut_good_e);
  Int_t ecounts = 0, counts=0;
  Double_t xbj;
  ecounts=h->GetEntries();

  //  ecounts=t->GetEntries(cut_good_e);
  //  cout<<"\n\n"<<ecounts<<" good electron events after cuts\n\n";

  // update the calculated ecounts to runlist
  TSQLServer* Server  = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  TString      query  = Form("update %srunlist set target='%s', e_count = %d,modify_time=now() where run_number=%d", coda.experiment.Data(), ana.target.Data(),ecounts,runnum);
  TSQLResult* result  = Server->Query(query.Data());
   
  TString table = coda.experiment+"stats";
  if(xbin>30)
    table=table+"2";
  //record raw counts per xbj bin (bin size 0.1)
  for(int jj=0;jj<xbin;jj++){
    xbj     = h->GetBinCenter(jj);
    counts  = h->GetBinContent(jj);
    if(counts>0){
      query   = Form("insert into %s values (%d, %d, %d, %g, %d) on duplicate key update run_number=values(run_number),current_id=values(current_id),cut_id = values(cut_id), xbj=values(xbj),counts=values(counts)",table.Data(),runnum,current_id,cut_id, xbj,counts);
    Server->Query(query.Data());
    }
  }
  Server->Close();

 
  return ecounts;
  //return chain;
}


void electron_counts(Int_t run_number, Int_t xbin=30){
  
  Int_t        timecut = 5;
  AnalysisInfo     ana = GetAnalysisInfo(run_number);
  if (ana.status==-1)  
    beamtrip_sql(run_number,timecut);
 
  ana = GetAnalysisInfo(run_number);
 if(ana.tarid<0) return 0;
 for(int i=0;i<GetNCurrents(run_number,0);i++){
   good_electrons(run_number, timecut, i, 0, xbin);
   good_electrons(run_number, timecut, i, 1, xbin);
   good_electrons(run_number, timecut, i, 2, xbin);
   good_electrons(run_number, timecut, i, 3, xbin);
 }
}

void runlist(Int_t startrun, Int_t endrun,TString kin="",Int_t xbin=30){

  for (Int_t i = startrun; i<=endrun; i++){
    if(GetRunInfo(i).kinid==kin || kin=="")
      electron_counts(i,xbin);
  }
  return;
}
