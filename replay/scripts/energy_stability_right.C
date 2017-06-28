//Check the stability of the beam energy using BPM 1c12
//Barak Schmookler, Feb 2016

void energy_stability_right(Int_t flag){
  
  gStyle->SetOptStat(0);
  
  TTree *ttree = (TTree*)gDirectory->Get("T");
  TTree *etree = (TTree*)gDirectory->Get("E");
  
  //Get Beam Energy
  TH1F *etemp1 = (TH1F*)gROOT->FindObject("etemp1");
  if(etemp1){
    etemp1->Reset();
  }else{
    etemp1 = new TH1F("etemp1","",200,0,20);
  }
  ttree->Project("etemp1","RrbGmp.e");

  //1d X Histogram (to find mean)
  TH1F *etemp2 = (TH1F*)gROOT->FindObject("etemp2");
  if(etemp2){
    etemp2->Reset();
  }else{
    etemp2 = new TH1F("etemp2","",1000,-10,10);
  }
  etree->Project("etemp2","IPM1C12.XPOS","TMath::Abs(IPM1C12.XPOS)>1E-3");
  Float_t xmean = etemp2->GetMean();

  //1d Y Histogram (to find mean)
  TH1F *etemp3 = (TH1F*)gROOT->FindObject("etemp3");
  if(etemp3){
    etemp3->Reset();
  }else{
    etemp3 = new TH1F("etemp3","",1000,-10,10);
  }
  etree->Project("etemp3","IPM1C12.YPOS","TMath::Abs(IPM1C12.YPOS)>1E-3");
  Float_t ymean = etemp3->GetMean();

  //Dispersion at BPM 1c12 (in mm)
  const Float_t dispersion = 1.44E3;

  //Entries in E-tree
  Int_t nentries = etree->GetEntries();

  if(flag==1){

    TH2F *hE1 = (TH2F*)gROOT->FindObject("hE1");
    if(hE1){
      hE1->Reset();
    }else{
      hE1 = new TH2F("hE1","Horrizontal Motion at 1C12",nentries,0,nentries,100,xmean-0.5,xmean+0.5);
      hE1->GetXaxis()->SetTitle("Entry");hE1->GetXaxis()->CenterTitle();
      hE1->GetYaxis()->SetTitle("Position [mm]");hE1->GetYaxis()->CenterTitle();
      hE1->SetMarkerColor(kBlue);hE1->SetMarkerStyle(3);hE1->SetMarkerSize(0.25);
    }

    etree->Draw("IPM1C12.XPOS:Entry$>>hE1","TMath::Abs(IPM1C12.XPOS)>1E-3","");
  }

  if(flag==2){
    
    TH2F *hE2 = (TH2F*)gROOT->FindObject("hE2");
    if(hE2){
      hE2->Reset();
    }else{
      hE2 = new TH2F("hE2","Vertical Motion at 1C12",nentries,0,nentries,100,ymean-0.5,ymean+0.5);
      hE2->GetXaxis()->SetTitle("Entry");hE2->GetXaxis()->CenterTitle();
      hE2->GetYaxis()->SetTitle("Position [mm]");hE2->GetYaxis()->CenterTitle();
      hE2->SetMarkerColor(kBlue);hE2->SetMarkerStyle(3);hE2->SetMarkerSize(0.25);
    }
    
    
    etree->Draw("IPM1C12.YPOS:Entry$>>hE2","TMath::Abs(IPM1C12.YPOS)>1E-3");



  }
    

}
