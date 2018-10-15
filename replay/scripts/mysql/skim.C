#include "rootalias.h"
#include "SQLanalysis.h"
//-------------------------
// load the beamtrip cut event list, cut on good electrons 
// put good events into new rootfile
// use for optics run for now, don't need beamtrip cut
// shujie 2018.9
//-------------------------
Int_t skim(Int_t runnum, Int_t production=1){

  //----------------------
  // load events after beamtrip cut
  //----------------------
  TChain*   t          = LoadList(runnum);

  // read info from SQL 
  // highest currrent has id 0
  CODASetting     coda = GetCODASetting(runnum);

  //-------------------------
  // get good electrons
  //-------------------------

  TCut trigger  = Form("%s>0",coda.trigger.Data());  
  TCut cut_good_e;
  if(coda.arm=="L")
    cut_good_e  = electron_cut_L + track_L + trigger;
  else 
    return 0;
  t->Draw(">>elist",cut_good_e);
  TEventList *elist = (TEventList*)gDirectory->Get("elist");

 // take intersect of events passed good electron cuts and beamtrip cuts
  t->SetEventList(0);
  t->SetEventList(elist);

  // select branches to be written into new rootfile
  t->SetBranchStatus("*",0);
  t->SetBranchStatus("Lrb.*",1);
  t->SetBranchStatus("Lurb.*",1);
  t->SetBranchStatus("L.cer.*",1);
  t->SetBranchStatus("L.prl*.e",1);
  t->SetBranchStatus("L.tr.*",1);
  t->SetBranchStatus("Hac*",1);

  if(production){
  t->SetBranchStatus("EKLx*",1);
  t->SetBranchStatus("evLeft*",1);
  t->SetBranchStatus("L.s0.*",1);
  t->SetBranchStatus("L.s2.*",1);
  t->SetBranchStatus("DL.*");


  }
 
 
  TFile *fout = new TFile(Form("skim_dir/skim_%d.root",runnum),"recreate");
  TTree *small = t->CopyTree("");
  // TTree *small = (TTree*)t->GetTree()->CloneTree(0); 
  small->Write();
  // small->Print();
  fout->Close();
  delete fout;
  return t->GetEntries();
}
