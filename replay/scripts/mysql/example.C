
//-------------------------
// Example code to do analysis with elist. Must run beamtrip_sql.C first. 
// Shujie Li, 2018.07
//-------------------------
// load the beamtrip cut event list, and calculate the good electrons after cuts
// flag:
// current_id  : select event list assoiciated with which current. id=0 is the highest current. 
//               If not sure which value to use, call GetNCurrents(runnum)  
// newtree = 1 : write events passed cuts into a new file, please read the code for details
//-------------------------
#include "rootalias.h"

Int_t good_electron(Int_t runnum, Int_t current_id=0, Int_t newtree=0){

  //----------------------
  // load events after beamtrip cut
  //----------------------
  TChain*          t   = LoadRun(runnum);

  // read info from SQL 
  // highest currrent has id 0
  AnalysisInfo     ana = GetAnalysisInfo(runnum, current_id);
  CODASetting     coda = GetCODASetting(runnum);

  TFile            *f  = new TFile(ana.elist,"read");

  if(f->IsZombie() || ana.status < 1){
    cout<<"Error: invalid run information. Did you run beamtrip_sql.C? Check your analysis table!\n";
    return -1;
  } 

  TEventList* elist = (TEventList*)f->Get("elist");
  if(elist) elist->SetDirectory(0); //otherwise the file destructor will delete elist
  t->SetEventList(elist);

  //-------------------------
  // get good electrons
  //-------------------------

  TCut trigger  = Form("%s>0",coda.trigger.Data());  
  TCut cut_good_e;
  if(coda.side=="L")
    cut_good_e  = acc_cut_L_e + electron_cut_L + track_L + trigger;
  else if(coda.side=="R")
    cut_good_e  = acc_cut_R_e + electron_cut_R + track_R + trigger;

  
  TH1F *h=new TH1F("h","",500,0,10);
  cout<<"----------------- "<<endl;
  cout<<"Good Electron Cuts: "<<cut_good_e <<endl;
  t->Project("h","EKLx.x_bj",cut_good_e);
  Int_t ecounts = 0;
  ecounts=h->GetEntries();

  //  ecounts=t->GetEntries(cut_good_e);
  cout<<"\n\n"<<ecounts<<" good electrons events after cuts\n\n";

  if(newtree){

  t->Draw(">>electron_list",cut_good_e+"Entry$<10000");
  TEventList *electron_list = (TEventList*)gDirectory->Get("electron_list");

 // take intersect of events passed good electron cuts and beamtrip cuts
  elist->Intersect(electron_list);
  t->SetEventList(0);
  t->SetEventList(elist);

  // select branches to be written into new rootfile
  t->SetBranchStatus("*",0);
  t->SetBranchStatus("evLeftdnew_r",1);
  t->SetBranchStatus("L.tr.*",1);
 
 
  TFile *fout = new TFile(Form("skim_%d.root",runnum),"recreate");
  TTree *small = t->CopyTree("");
  // TTree *small = (TTree*)t->GetTree()->CloneTree(0); 
  small->Write();
  // small->Print();
  fout->Close();
  delete fout;
  delete f;
  }
  return ecounts;

}
