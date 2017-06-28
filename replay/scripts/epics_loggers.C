//Check the rate the epics loggers are inserting info
//into the data stream.
//Barak Schmookler, Feb 2016

void epics_loggers(){

  gStyle->SetOptStat(0);

  TTree *etree = (TTree*)gDirectory->Get("E");

  //Entries in E-tree
  Int_t nentries = etree->GetEntries();

  etree->Draw("timestamp:Entry$","","");
}
