#include "headers/rootalias.h"
#include "headers/SQLanalysis.h"
#include "headers/SRCanalysis.h"
Int_t good_electron(Int_t runnum, Int_t timecut=5,Int_t current_id=0){

  //----------------------
  // load events after beamtrip cut
  //----------------------
  beamtrip_sql(runnum,timecut);
  TChain*            t = LoadList(runnum);
  CODASetting     coda = GetCODASetting(runnum);
  AnalysisInfo     ana = GetAnalysisInfo(runnum, current_id);

  // if(ana.tarid==-1) {
  //    cout<<"run "<<runnum<<" : not production target, skipped\n";
  //    return -1;
  //  }
  //-------------------------
  // get good electrons
  //-------------------------
 
  TCut trigger  = Form("%s>0",coda.trigger.Data());  
  TCut cut_good_e;
  if(coda.arm      == "L")
    cut_good_e  = acc_cut_L_e + electron_cut_L + track_L + trigger;
  else if(coda.arm == "R")
    cut_good_e  = acc_cut_R_e + electron_cut_R + track_R + trigger;


  TH1F *h=new TH1F("h","",30,0,3);
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
 
  
  //record raw counts per xbj bin (bin size 0.1)
  for(int jj=0;jj<30;jj++){
    xbj     = h->GetBinCenter(jj);
    counts  = h->GetBinContent(jj);
    if(counts>0){
      query   = Form("insert into %sstats values (%d, %d, %g, %d) on duplicate key update run_number=values(run_number),current_id=values(current_id),xbj=values(xbj),counts=values(counts)",coda.experiment.Data(),runnum,current_id,xbj,counts);
    Server->Query(query.Data());
    }
  }
  Server->Close();

 
  return ecounts;
  //return chain;
}
