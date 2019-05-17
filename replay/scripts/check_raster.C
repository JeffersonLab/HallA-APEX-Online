// check raster shape, hotspot
// Shujie Li, 2018.10
#include "headers/SQLanalysis.h"
void raster(Int_t runnum, Int_t track=0){
  TChain *t = LoadRun(runnum);
  if(!t)
    return;
  
  TCanvas *c1=new TCanvas("c1","",1200,600);
  TCut beam_on="evLeftdnew_r*0.00033>3";
  if(track==0) beam_on=beam_on+"L.tr.n==0";
  

  c1->Divide(3,1);
  c1->cd(1);
  TH2F *h = new TH2F("h","raster rawcur x:y",100,60000,100000,100,60000,100000);
  t->Draw("Lrb.Raster.rawcur.x:Lrb.Raster.rawcur.y>>h",beam_on,"colz");
  c1->cd(2);
  TH2F *b = new TH2F("b","BPM x:y (unrastered)",200,-0.003,0.003,200,-0.003,0.003);
  t->Draw("Lurb.x:Lurb.y>>b",beam_on,"colz");
  c1->cd(3);
  TH1F *y = new TH1F("y","target z",200,-0.2,0.2);
  t->Draw("L.tr.vz>>y","evLeftdnew_r*0.00033>3","colz");


  TString file = Form("/volatile/halla/triton/shujie/replay/raster/%d.png",runnum);
  if(track==0)
    file=Form("/volatile/halla/triton/shujie/replay/raster/notrack_%d.png",runnum);
  c1->Print(file);
  
}

void check_raster(Int_t startrun,Int_t endrun, Int_t track){


  for (int i=startrun;i<=endrun;i++){

    raster(i,track);
  }




}
