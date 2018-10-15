//======================================
// get run info from rootfile -shujie
// !! Run with analyzer NOT root!!
//======================================


#include "headers/rootalias.h"
using namespace std;

bool edit(TString name){
  string temp=name.Data();
  if((temp.find("run_number")<20)||(temp.find("charge")<20)||(temp.find("time")<20)||(temp.find("count")<20))
    return 0;
  return 1;
}


void check(Int_t run=0){
  Int_t irun;
  // const TString rootfilePath = "/volatile/halla/triton/Tritium_Rootfiles/";
  if(run==0){
	  cout << "\nPlease enter a Run Number (-1 to exit):";
  	  cin >> irun;
  	  }
  else{irun=run;}
  if(irun==-1) return;

  TString expname="MARATHONrunlist";
  if (irun>100000) expname="EPrunlist";
  else if (irun>93000) expname="SRCrunlist";
  else if (irun<20000 && irun>3000) expname="SRCrunlist";

 //   get run info from mysql database

  TString run_type,start,end,comment,endcomment;
  Double_t  time,target,evts,n_e,n_coinc,charge,rx,ry,eb,pL,pR,degL,degR,ps[8];

  TSQLServer* Server = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
  TString  query(Form("select * from %s where run_number=%d",expname.Data(),irun));
  TSQLResult* result=Server->Query(query.Data());
  TSQLRow *row;
  int nrows = result->GetRowCount();
  row=result->Next();
    
  int nfields = result->GetFieldCount();
  // cout<<nfields<<endl;
  for (int i = 0; i < nfields; i++){
    string temp=result->GetFieldName(i);
    TString val=row->GetField(i);
    //    if(temp.find("charge")<20) cout<<temp<<endl;
    
 
    if(!edit(temp)) temp=temp+"*";
    //cout<<temp<<endl;
    cout<<Form("(%d) ",i+1)<<setw(15)<<temp;
    cout.setf(ios::left, ios::adjustfield);
    cout<<": "<<setw(50)<<val<<endl;
    
  }
 
  while (1){

   
    int index;
    cout << "\n--Which item you want to modify?\n(Please enter index. -1 to exit, item with \"*\" is NOT editable) ";
    cin >> index;
    cin.ignore();
    if (index==-1) break;
    if (index>nfields) {cout<< "Please enter a valid number!\n"; continue;} 
    string name = result->GetFieldName(index-1);
    if(edit(name)){
      string input;     
      cout<< "--OK. What is the correct value for \""<<name<<"\"? (-1 to exit) ";
      getline(cin, input);
      cout<<"you entered: "<<input<<"!!!!!!!!!!"<<endl;
      cin.ignore();
      //cin >> input;
      if (input=="-1") break;
      cout<< "--try: update ["<< name << "] with value ["<<input<<"]"<<endl;
      //      TString mod=Form("update EPrunlist set %s = %s where run_number=%d",name.c_str());//,input,irun);
      TString mod=Form("update %s set %s =\'%s\'  where run_number=%d;",expname.Data(),name.c_str(),input.c_str(),irun);
      cout<<mod<<endl;
      TSQLResult* res = Server->Query(mod.Data());
      cout<<"done!"<<endl;
      mod=Form("update %s set modify_time = now() where run_number=%d;",expname.Data(),irun);
      TSQLResult* res2 = Server->Query(mod.Data());

    }
    else{
      cout<< "You are not allowed to change item \""<<name<<"\", please report to shift expert if this item contains incorrect information\n";
    
    }
  }
  
   Server->Close();
  return;
}


void test()
{
  string temp,blah;
  cin>>temp;
  cout<<temp<<endl;
  cin.ignore();
  cout<<"Enter a sentence. Blah!!\n";
  getline(cin, blah);
  cout<<"youtry" << blah << " ";
  //cin.get();
  cin.ignore();
}
