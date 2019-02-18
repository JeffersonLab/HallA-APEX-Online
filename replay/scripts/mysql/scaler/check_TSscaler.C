// loop over TS scaler tree to get the mean beam-on value, write into SQL database.
// Shujie Li, 2018.6

#include "rootalias.h"



void check_TSscaler(int runnum, int rightarm=0){
  
  TString   arm    = "LHRS";
  TString   table  = "L_Scaler";
  TChain    *fChain;

  if (rightarm){
    fChain= LoadRun(runnum,"TSRight");
    arm      = "RHRS";
    table    = "R_Scaler";
    system(Form(" rm -rf plots/RHRS/%d",runnum));
    system(Form(" mkdir  plots/RHRS/%d",runnum));
  }
  else{
    fChain   = LoadRun(runnum,"TSLeft");
    system(Form(" rm -rf plots/LHRS/%d",runnum));
    system(Form(" mkdir  plots/LHRS/%d",runnum));

  }
  if (!fChain) {
   cout<< "can't find rootfile for run "<<runnum<<endl;
   exit(0);
   }
   // ------------
   // read from the sql database to see if this run needs to be checked
   // ------------
   TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
   TString  query1,query2;
   // check if its scaler info exists
   query1=Form("select distinct `run_number` from `%s` where `run_number`=%d",table.Data(),runnum); // 
   // check if its scaler info has issue and need to be rechecked.
   query2=Form("select distinct `run_number` from `%s` where `run_number`=%d and value=-1",table.Data(), runnum);
   TSQLResult* result1=Server1->Query(query1.Data());
   TSQLResult* result2=Server1->Query(query2.Data());
   Server1->Close();
   // skip the run if it's checked and has valid scaler info
   if(result1->GetRowCount()>0 && result2->GetRowCount()==0)  {
    cout<<"This run has been checked. Will skip"<<endl;
    exit(0);
  }


  int     nentries  = fChain->GetEntries(); 

  TObjArray *branchList;
  branchList               = fChain->GetListOfBranches();
  const  Int_t nBranch     = fChain->GetNbranches();
  TString   varnames[nBranch];
  TLeaf     *var[nBranch];
  TH1F      *h1[nBranch];
  vector<vector<double> > counts(nBranch,vector<double> (1,0));

   for(int i=0;i<nBranch;i++){
		branchList = fChain->GetListOfBranches();
	varnames[i] = branchList->At(i)->GetName();
     }

  // branchList->Print();
  double  value[nBranch];
  double  value_old[nBranch];
  memset( value_old, 0, nBranch*sizeof(double) );


  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = fChain->LoadTree(jentry);
    int  nb = fChain->GetEntry(jentry);
    // cout<<ientry<<" "<<nb<<endl;
	 for(int i=0;i<nBranch;i++){
		// branchList = fChain->GetListOfBranches();
	// varnames[i] = branchList->At(i)->GetName();
	string tempname = string(varnames[i]);
	var[i] = fChain->GetBranch(varnames[i])->GetLeaf(varnames[i]);
	value[i] = var[i]->GetValue();
		// if (jentry==0){
      // h1[i]=new TH1F(Form("h1[%d]",i),"",1000,0,50000);
		// }
	if (tempname.find("_r")<30)
        counts[i].push_back(value[i]);
		// h1[i]->Fill(value[i]);
	else
		// h1[i]->Fill(value[i]-value_old[i]);
        counts[i].push_back((value[i]-value_old[i])/10.0); // assume the TS tree is updated every 10 seconds

	 value_old[i]=value[i];

	}
  }


int attention = 0;
for (int ii=0;ii<nBranch;ii++){
  string tempname = string(varnames[ii]);

  if(tempname.find("Fake")<30||tempname.find("C1")<30||tempname.find("C2")<30||tempname.find("C3")<30||tempname.find("MLU")<30||tempname.find("evcount")<30) continue;
  Double_t mean_value=-1;

  auto       hmin      = *min_element(counts[ii].begin(),counts[ii].end());
  auto       hmax      = *max_element(counts[ii].begin(),counts[ii].end());
  Int_t      nbin      = max(100,(int)(hmax - hmin)/20);
  Int_t      sigma     = 10;
  Double_t   threshold = 0.1;
  

  if (tempname.find("unser")<30||tempname.find("dnew")<30||tempname.find("unew")<30||tempname.find("d10")<30||tempname.find("d3")<30||tempname.find("d1")<30||tempname.find("u1")<30) {
    nbin      = 500;
    threshold = 0.02;
  }
  TCanvas *c1 = new TCanvas("c1","c1",800,600); 
  h1[ii]=new TH1F(Form("h1[%d]",ii),"",nbin,hmin*0.9+10,hmax*1.2+10);
  for (int jj=0;jj<counts[ii].size();jj++){
    h1[ii]->Fill(counts[ii][jj]);
  }
  // h1[ii]->SetAxisRange(0.9*h1[ii]->GetBinCenter(h1[ii]->FindFirstBinAbove())-10,1.2*h1[ii]->GetBinCenter(h1[ii]->FindLastBinAbove())+10);
  h1[ii]->Draw();
  h1[ii]->SetTitle(varnames[ii]+" rate from TS tree");
  TSpectrum *s = new TSpectrum(10);
 
  Int_t nfound = s->Search(h1[ii],sigma,"",threshold);//sigma=20 channels(bins)=>1uA. threshold=0.1*highest peak
  if (nfound<1) {
    sigma=1;
    nfound=s->Search(h1[ii],sigma,"",threshold);
  }
  if (nfound>4) {
    h1[ii]->Rebin(5);
    nfound=s->Search(h1[ii],sigma,"",threshold);
  }
  if (nfound>0){
  Double_t *xpeaks = s->GetPositionX();
  sort(xpeaks, xpeaks+nfound,isGreater);//,wayToSort);
  mean_value = xpeaks[0];
  // for (int jj=0;jj<nfound;jj++) cout<<xpeaks[jj]<<endl;
  }
  if(mean_value<(hmin+0.2*(hmax-hmin))){
    cout<<mean_value<<"  "<<hmin<<"  "<<hmax<<endl;
  c1->Print(Form("plots/%s/%d/%s.png",arm.Data(),runnum,varnames[ii].Data()));
    mean_value=-1;
    attention = 1;
  }
  c1->Close();

  // write results into mysql
    TSQLServer* Server = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
    TString  query(Form("insert into %s values ('%d', '%s', %g) on duplicate key update value=values(value)",table.Data(),runnum,varnames[ii].Data(),mean_value));
    //cout<<query<<endl;
    Server->Query(query.Data());
    Server->Close();
   
}
  if (attention == 0){
    if(rightarm) system(Form(" rm -rf plots/RHRS/%d",runnum));
    else         system(Form(" rm -rf plots/LHRS/%d",runnum));
  }
  delete fChain;
  exit(0);
}
