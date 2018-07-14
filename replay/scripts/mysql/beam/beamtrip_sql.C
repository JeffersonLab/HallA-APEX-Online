// for Tritium: 
// cut on beam trip, record the good entry numbers to eventlist->rootfile, and write current, charge, livetime to SQL database
// also calculate the mean current , charge, and livetime
// Shujie Li, 02.2018
#include "rootalias.h"

// where to store/find the beamtrip cut eventlist
TString  listdir    = Form("%s/elist/",getenv("PWD")); 
TString  plotdir    = "plots/";
Int_t    maxpeaks   = 6; // maximum number of currents allowed in one run. 

Double_t beamtrip_sql(Int_t runnum,Int_t timecut=5){
  
  gStyle->SetOptStat(0);
  //---------------
  // rootfiles ->TChain
  //----------------
  TChain* chain = LoadRun(runnum,"T");
  TChain* Epics = LoadRun(runnum,"E"); 

  if (!chain) {
   cout<< "can't find rootfile for run "<<runnum<<endl;
   exit(0);
   }

// get info from SQL database

  CODASetting    coda     = GetCODASetting(runnum);
  TargetInfo     target   = GetTarget(runnum);
  BCMInfo        bcm      = GetBCMInfo(runnum,"dnew");
  Int_t gas               = 0;
  if (target.type=="gas") gas  =  1;
  
  // get prescale factor of the production trigger
  Int_t   prescale;
  prescale = GetPS(chain,coda.bit);
 


  //-----------------
  // set branch, disable all unused to speed up
  //----------------

  // the bcm class is not working, comment out for now
  // TString curr=arm+"BCMev.current_"+bcm;
  TString charr          = "ev"+coda.arm+bcm.name;
  TString curr           = "ev"+coda.arm+bcm.name+"_r";
  TString trigger_scaler = Form("ev%sT%d",coda.arm.Data(),coda.bit);
  TString clock_scaler   = "ev"+coda.arm+"Lclock";
  chain->SetBranchStatus("*"         ,0);
  chain->SetBranchStatus(coda.trigger,1);
  chain->SetBranchStatus(charr  ,1);
  chain->SetBranchStatus(curr   ,1);
  // chain->SetBranchStatus(arm+"BCMev.isrenewed",1);
  chain->SetBranchStatus(clock_scaler    ,1);
  chain->SetBranchStatus(trigger_scaler  ,1);

  //--------------------------------
  Double_t bcmcnt,clk,renew, trgbit, trgscaler; 
  chain->SetBranchAddress(charr            ,&bcmcnt   );
  chain->SetBranchAddress(coda.trigger     ,&trgbit   );
  chain->SetBranchAddress(clock_scaler     ,&clk      );
  chain->SetBranchAddress(trigger_scaler   ,&trgscaler);
  // chain->SetBranchAddress(arm+"BCMev.isrenewed",&renew); // BCMev is associated with evLeft tree
  
  Int_t nn=chain->GetEntries();
  cout<<nn<<" TTree:"<<nn<<" entries in total"<<endl;

  //---------------------
  // plot beamstrip and identify beam on current
  //---------------------
  chain->GetEntry(nn-1);
  Double_t charge = bcmcnt*bcm.gain;
  Double_t time   = clk/103700; // fast clock
  cout<<"raw charge:"<<charge<<endl;
  curr = Form("%s * %f + %f", curr.Data(),bcm.gain,bcm.offset); // convert bcm rate to current

  TCanvas *c1 = new TCanvas("c1","c1",800,600); 
  c1->Divide(1,2);
  c1->cd(2);

  // get average beam current
  c1->cd(1);
  TH1F *beam=new TH1F("beam","",100,0,25);
  chain->Draw(curr+">>beam",curr+">2");
  beam->GetXaxis()->SetTitle("current(uA)");
  
  TSpectrum *s = new TSpectrum(10);
  Int_t nfound = s->Search(beam,2,"",0.10);//sigma=10 channels(bins)=>0.5uA. threshold=0.1*highest peak
  if (nfound>maxpeaks) {
    cout<<"More than "<<maxpeaks<< "peaks are found. Beam unstable. This code can not handle it!!"<<endl;
    return -1;
  }

    // delete previous results;
  TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  TString     query1;
  //cout<<query2<<endl;
  query1 = Form("delete from %sanalysis where run_number=%d",coda.experiment.Data(),runnum);
  Server1->Query(query1.Data());
  Server1->Close();



  s->Print();
  Double_t *xpeaks_original = s->GetPositionX();
  Double_t *ypeaks_original = s->GetPositionY();
  vector<double> xpeaks;
  for (int i =0; i<nfound;i++){
    xpeaks.push_back(xpeaks_original[i]);
  }
  // sort descending
  // not work with root5!!!!!!
  sort(xpeaks.begin(), xpeaks.end(),isGreater);
  Double_t gap=1.5; // allowed deviation from average current
  xpeaks.push_back(0);
  for (int i =0; i<nfound;i++){
    Double_t temp=(xpeaks[i]-xpeaks[i+1])/2.0;
    temp=fabs(temp);
    if (temp<gap) gap=temp;
  }
  cout<<gap<<endl;
  //--------------------------------------
  //     cut beam trip and calculate avreage deadtime at each stable current
  //-------------------------------------
  vector<int> dbcm,dclock,entry,dtrgscaler,dtrgbit; //to store results
  Int_t      bcm_old  = 0,clk_old  = 0, trg_old= 0;
  Int_t      trg_temp = 0;
  Int_t      ctemp    = 0;

  for(Int_t j=0;j<nn;j++){
    chain->GetEntry(j);
    if (trgbit>0) trg_temp++;
    if(clk!=ctemp){ //  could use isrenew variable instead
      Double_t temp=clk-clk_old;
      if (temp<0.001*103700) continue; // smallest timescale set to be 1ms
      dbcm.push_back(bcmcnt-bcm_old);
      dclock.push_back(temp);
      dtrgscaler.push_back(trgscaler-trg_old);
      dtrgbit.push_back(trg_temp);
      trg_temp = 0;
      trg_old  = trgscaler;
      bcm_old  = bcmcnt;
      clk_old  = clk;
      entry.push_back(j);
    }
    ctemp=clk;
  }

  //  for gas cell cut on <timecut> secs after beam stable
  Int_t      cnt=0;
  Double_t   goodcharge[10];
  //if(gas==0) timecut=0;

  charge=0;
  // loop over all currents
  for (int i=0; i<nfound;i++){
    TEventList *list      = new TEventList("elist");
    Int_t trip            = 1;
    Int_t stable_time     = 0;
    Int_t trgscaler_total = 0;
    Int_t trgbit_total    = 0;
    goodcharge[i]         = 0;

    for(int tt=0;tt<(int)dbcm.size()-1;tt++){
      Double_t current=dbcm[tt]*bcm.gain/dclock[tt]*103700+bcm.offset;
      if(fabs(current-xpeaks[i])<gap) {
        if(trip==0){
          goodcharge[i]   += current*dclock[tt]/103700.0;
          trgbit_total    += dtrgbit[tt];
          trgscaler_total += dtrgscaler[tt];
          for(int pp=entry[tt-1];pp<=entry[tt]-1;pp++){
            cnt++;
            list->Enter(pp);
          }
        }
        stable_time+=dclock[tt];
        if (gas==0||stable_time>timecut*103700.0) trip=0;
      }
      else {
        stable_time=0;
        trip=1;
      }
    }
    cout<<xpeaks[i]<<"uA: "<<goodcharge[i]<<" uC"<<endl;
    charge+=goodcharge[i];
    
    Double_t livetime = trgbit_total * prescale / (1.0 * trgscaler_total);
    // store average current, charge into rootfile
    TString  listname = listdir + Form("bcm%d_%d_%d.root",runnum,i,timecut);
    
    TFile    efile(listname,"recreate");
    TVectorD v1(1), v2(1), v3(3);
    v1[0]=xpeaks[i];
    v2[0]=goodcharge[i];
    v3[0]=livetime;
    v3[1]=trgbit_total;
    v3[2]=trgscaler_total;
    v1.Write("current");
    v2.Write("charge");
    v3.Write("livetime");
    // store eventlist into rootfile
    list->SetName("elist");
    list->Print();
    list->Write();
    delete list;
    efile.Close();

    //---------------
    // write results into database
    //---------------
  TSQLServer* Server2 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  TString     query2;


  query2=Form("insert into %sanalysis values (%d, %.1f, %g, '%s',%g,%d,%d, '%s') on duplicate key update charge=values(charge),livetime=values(livetime),trigger_counts=values(trigger_counts),trigger_events=values(trigger_events),elist=values(elist)",coda.experiment.Data(),runnum,xpeaks[i],goodcharge[i],coda.trigger.Data(),livetime,trgscaler_total,trgbit_total,listname.Data());
  //cout<<query2<<endl;
  Server2->Query(query2.Data());
  Server2->Close();
  }

  // -----------------
  //  read the stored list and variables
  //  plot the beam strip passed beamtrip cut
  // -----------------

  c1->cd(2);
  TEventList *list=new TEventList("list");

  for(Int_t i=0;i<nfound;i++){
    TString listname=listdir+Form("bcm%d_%d_%d.root",runnum,i,timecut);
    cout<<listname<<endl;
    TFile* ff=TFile::Open(listname);
    TVectorD *cc=(TVectorD*)ff->Get("current");
    TVectorD *ch=(TVectorD*)ff->Get("charge");
    TVectorD *dt=(TVectorD*)ff->Get("livetime");
    cout<<"current, charge, livetime = "<<cc[0][0]<<","<<ch[0][0]<<","<<dt[0][0]<<endl;
    TEventList* elist = (TEventList*)ff->Get("elist");
    list->Add(elist);
    //elist->Delete();
    delete elist;
    delete ff;
  }
  TH2F *ss=new TH2F("ss","",10000,0,time,100,0,25);
  if (list) list->SetDirectory(0); //otherwise the file destructor will delete elist
  // TTree *T = (TTree*)gDirectory->Get("T");

  // before beam cut
  chain->SetEventList(0);
  TString draw;
  draw=curr+":ev"+coda.arm+"Lclock/103700";
  chain->Draw(draw+">>ss","","col");
  ss->SetMarkerSize(0.3);
  ss->SetMarkerStyle(20);
  ss->GetYaxis()->SetTitle("current(uA)"); 
  ss->GetXaxis()->SetTitle("time(sec)");

  // after beam cut
  chain->SetEventList(list);
  TH2F *hh=new TH2F("hh","hh",10000,0,time,100,0,25);
  chain->Draw(draw+">>hh","","same");
  hh->SetMarkerColor(kRed);
  hh->SetMarkerStyle(8);
  hh->SetMarkerSize(0.5);
  hh->Draw("same");
  gPad->SetGrid();
  //Delete T;

  c1->Print(plotdir+Form("bcm_%d.eps",runnum));

  return charge;
}



