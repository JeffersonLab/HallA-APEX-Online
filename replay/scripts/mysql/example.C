#include "rootalias.h"
#include "SQLanalysis.h"
//-------------------------
// load the beamtrip cut event list, and calculate the good electrons after cuts
// optional: write events passed cuts into a new file, please read code for details
//-------------------------
Int_t good_electron(Int_t runnum, Int_t current_id=0, Int_t newtree=0){

  //----------------------
  // load events after beamtrip cut
  //----------------------
  TChain*   t          = LoadList(runnum);

  // get run information
  THaRun*   arun       = GetRunData(t); 
  arun->Print();
  TArrayI   ps         = GetPS(t); // array of Prescale factors
  Int_t     ps2        = GetPS(t,2);  // precale 2
  cout<<"-----------------"<<endl;
  cout<<GetTimeStamp(t,1)<<" days since Tritium cell filled"<<endl; 
  cout<<"-----------------"<<endl;

  AnalysisInfo     ana = GetAnalysisInfo(runnum, current_id); // highest currrent has id 0
  CODASetting     coda = GetCODASetting(runnum);

  //-------------------------
  // get good electrons
  //-------------------------

  TCut trigger  = Form("%s>0",coda.trigger.Data());  
  TCut cut_good_e;
  if(coda.arm=="L")
    cut_good_e  = acc_cut_L_e + electron_cut_L + track_L + trigger;
  else if(coda.arm=="R")
    cut_good_e  = acc_cut_R_e + electron_cut_R + track_R + trigger;

  
  TH1F *h=new TH1F("h","",500,0,10);
  cout<<"----------------- "<<endl;
  cout<<"Good Electron Cuts:\n "<<cut_good_e <<endl;
  t->Project("h","EKLx.x_bj",cut_good_e);
  Int_t ecounts = 0;
  ecounts=h->GetEntries();

  //  ecounts=t->GetEntries(cut_good_e);
  cout<<"\n\n"<<ecounts<<" good electron events after cuts\n\n";

  if(newtree){

  t->Draw(">>electron_list",cut_good_e+"Entry$<10000");
  TEventList *electron_list = (TEventList*)gDirectory->Get("electron_list"); // list of events from good electrons
  TEventList *elist = GetList(runnum); // list of events from good beam

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
  }
  return ecounts;

}
