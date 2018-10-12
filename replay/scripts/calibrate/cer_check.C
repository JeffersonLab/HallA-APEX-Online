// check cherenkov PMTs pedstal and gain
// print the output in db format
// for tritium, shujie 03.2018
#include "../headers/rootalias.h"
void cer_check(Int_t runnum, Int_t rightarm=0, Int_t fadc=1){
  // Int_t run,fadc;    
  // cout << "enter run_number: ";
  // cin >> run;
  // cout << "0=fastbus signal, 1=FADC signal: ";
  // cin >> fadc;

  Double_t scale = 1;
  if (fadc==1) scale=3;
 
  TChain *t = LoadRun(runnum);
  
  if(!t){
   cout<< "can't find rootfile for run "<<runnum<<endl;
   exit(0);
   }
  if(rightarm){
    t->Add(LoadRun(runnum+1));
    t->Add(LoadRun(runnum+2));
    t->Add(LoadRun(runnum+3));
    t->Add(LoadRun(runnum+4));
  }


  t->SetBranchStatus("*"       ,   0);
  t->SetBranchStatus("D*.bit*" ,   1);
  t->SetBranchStatus("*.cer.a*",   1);
 
  TString  arm = "L"; 
  TString  cut = "DL.bit2>0"; //lhrs single
  if(rightarm){
    cut = "DR.bit5>0"; // rhrs single
    arm = "R";
  }
  if(runnum>100000)   
    cut = "DR.bit2>0"; // lhrs coinc
  //cut="";
  // cout << t->GetEntries() << endl;
  TCanvas  *c1 = new TCanvas("c1","c1",1200,700);
  TString  name, tit, dr;
  Double_t min, max;
  Int_t    max_bin;
  Int_t    attention = 0;
  Int_t    i;
  Double_t ped[10],peak[10];
  TH1F     *h[10] , *hh[10];

  c1->Divide(5,2);

  for(i=0;i<10;i++){
    hh[i] = new TH1F(Form("gc_%d",i),Form("gc_%d",i+1),2000,0,40000);
  }
  for(i=0;i<10;i++){
    c1->cd(i+1);
    if (fadc==0)
      t->Draw(Form("Fbus%s.cer.a[%d]>>gc_%d",arm.Data(),i,i),cut);
    if (fadc==1)
      t->Draw(Form("%s.cer.a[%d]>>gc_%d",arm.Data(),i,i),cut);
    max_bin = hh[i]->GetMaximumBin();
    ped[i]  = hh[i]->GetBinCenter(max_bin);
    min     = ped[i] - 100*scale;
    max     = min + 600*scale;
   // name    = Form("gc_%d",i);
   // tit     = Form("gc_%d",i);
   // hh[i]   = new TH1F(name,tit,100,min,max);
   // if(fadc==0)
   //   dr = Form("FbusL.cer.a[%d]",i);
   // if(fadc==1)
   //   dr = Form("L.cer.a[%d]",i);
   // dr = dr + ">>";
   // dr = dr + name;
   // t->Draw(dr,cut);
    //fit SPE
    hh[i]->GetXaxis()->SetRangeUser(ped[i],ped[i]+100*scale);
    Double_t dip=hh[i]->GetBinCenter(hh[i]->GetMinimumBin()); //find the dip b/w ped and SPE
    //cout<<ped[i]<<"  "<<dip<<endl;
    hh[i]->GetXaxis()->SetRangeUser(min,max);
    double_t gap = 110;
    if(rightarm) {
      gap*=1.2;
      if(i==1) gap*=1.2;
    }
    TF1 *f2 = new TF1("f2", "gaus", dip+5*scale, dip+gap*scale);
    hh[i]->Fit(f2, "Rq");
    peak[i] = f2->GetParameter(1);
    TLine *l1 = new TLine(peak[i],0,peak[i],hh[i]->GetMaximum());
    l1->SetLineColor(2);
    l1->Draw();
    gPad->SetLogy();

    if(peak[i]<ped[i]+100 || peak[i]>ped[i]+500)
      attention = 1;


    TSQLServer* Server1 = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
    TString  query1;
    query1=Form("insert into `%s_Cherenkov` (run_number, id, pedestal,SPE) values (%d, %d, %g, %g) on duplicate key update pedestal=values(pedestal), SPE=values(SPE)",arm.Data(), runnum, i+1, ped[i], peak[i]); // 
    Server1->Query(query1.Data());
    Server1->Close();
 
  }
  if(attention)
    c1->Print(Form("plots/%d_cer%s.png",runnum,arm.Data()));

  for (int jj=0;jj<10;jj++)     cout<<jj<< "  ";
  cout<< endl<<arm.Data()<<".cer.adc.pedestals = ";
  for (int jj=0;jj<10;jj++)    cout<<"  "<<ped[jj];
  cout<< endl<<arm.Data()<<".cer.adc.gains = ";
  for (int jj=0;jj<10;jj++)       cout<< "  "<<300.0/(peak[jj]-ped[jj]);
  cout<<endl;
}
