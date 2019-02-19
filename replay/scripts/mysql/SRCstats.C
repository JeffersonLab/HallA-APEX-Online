// read table SRCstats and show counts per target per kinematics

#include "rootalias.h"
#include "SQLanalysis.h"
void SRCstats(Int_t run_number, Int_t verb=1){

  AnalysisInfo   ana   = GetAnalysisInfo(run_number,0,verb);
  TSQLServer* Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString      query   = Form("select xbj, counts from SRCstats where run_number = %d", run_number);
  TSQLResult* result   = Server->Query(query.Data());
  Server->Close();// Always remember to CLOSE the connection!


  int nrows = result->GetRowCount();
  if(nrows==0){
    cout<<"Error: run "<<run_number<<" is not in SRCstats table!!"<<endl;
    return;
  }
  Double_t xbj[nrows], counts[nrows];
  for (int i=0;i<nrows;i++){
    TSQLRow *row    = result->Next();
    xbj[i]          = atof(row->GetField(0));
    counts[i]       = atof(row->GetField(1));
  }
  cout<<setw(15)<<ana.current<<setw(10)<<ana.charge<<setw(10)<<ana.livetime<<endl;
  if(verb){
    cout<<setw(20)<<"xbj:";
    for (int i=0;i<nrows;i++){
      cout<<setw(15)<<xbj[i];
    }
    cout<<endl;
  }
    cout<<setw(20)<<"counts:";
    for (int i=0;i<nrows;i++){
      cout<<setw(15)<<counts[i];
    }
    cout<<endl;
 if(verb){ 
  cout<<setw(20)<<"counts/charge:";
  for (int i=0;i<nrows;i++){
    cout<<setw(15)<<counts[i]/ana.charge;
  }
  cout<<endl;

 }
 return;

}


void kin(TString target,Double_t p0){ // distinct kin by p0 is good for now
  
  TSQLServer* Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString      query   = Form("select run_number from SRCrunlist where momentum = %g and target = '%s' and run_type='Production'", p0, target.Data());
  TSQLResult* result   = Server->Query(query.Data());
  
  int nrows = result->GetRowCount();
  if(nrows==0){
    cout<<"Error: "<<target<<" with momentum = "<<p0<<" does not exist in SRCrunlist table!!"<<endl;
    return;
  }
  Int_t verb=1;
  for (int i=0;i<nrows;i++){
    TSQLRow *row         = result->Next();
    Int_t    run_number  = atoi(row->GetField(0));
    SRCstats(run_number,verb);
    verb  = 0;

  }

}
