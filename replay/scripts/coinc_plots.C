//Coincidence plots
//Reynier Cruz Torres, Feb 2017
void coinc_plots(Int_t flag){
  gStyle->SetOptStat(0);
  
  TTree *tree = (TTree*)gDirectory->Get("T");

  // =============== TRACKING VARIABLES ===============
  if(flag==1){
    TH2F *ht1 = new TH2F("ht1","Delta",60,-.2,.2,60,-.2,.2);
    ht1->GetXaxis()->SetTitle("#delta (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("#delta (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_dp:L.tr.tg_dp>>ht1","DR.evtype==1","COLZ");
  }

  else if(flag==2){
    TH2F *ht1 = new TH2F("ht1","Theta",60,-.2,.2,60,-.2,.2);
    ht1->GetXaxis()->SetTitle("#theta (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("#theta (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_th:L.tr.tg_th>>ht1","DR.evtype==1","COLZ");
  }

  else if(flag==3){
    TH2F *ht1 = new TH2F("ht1","Phi",60,-.2,.2,60,-.2,.2);
    ht1->GetXaxis()->SetTitle("#phi (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("#phi (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_ph:L.tr.tg_ph>>ht1","DR.evtype==1","COLZ");
  }

  else if(flag==4){
    TH2F *ht1 = new TH2F("ht1","Target y",60,-.2,.2,60,-.2,.2);
    ht1->GetXaxis()->SetTitle("y (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("y (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_y:L.tr.tg_y>>ht1","DR.evtype==1","COLZ");
  }

  // =============== REACTION VERTEX VARIABLES ===============
  else if(flag==5){
    TH2F *ht1 = new TH2F("ht1","Reaction vertex - x",100,-.2,.2,100,-.2,.2);
    ht1->GetXaxis()->SetTitle("x (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("x (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("rpr.x:rpl.x>>ht1","DR.evtype==1","COLZ");
  }

  else if(flag==6){
    TH2F *ht1 = new TH2F("ht1","Reaction vertex - y",100,-.2,.2,100,-.2,.2);
    ht1->GetXaxis()->SetTitle("y (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("y (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("rpr.y:rpl.y>>ht1","DR.evtype==1","COLZ");
  }

  else if(flag==7){
    TH2F *ht1 = new TH2F("ht1","Reaction vertex - z",100,-.2,.2,100,-.2,.2);
    ht1->GetXaxis()->SetTitle("z (RHRS)");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("z (LHRS)");ht1->GetYaxis()->CenterTitle();
    tree->Draw("rpr.z:rpl.z>>ht1","DR.evtype==1","COLZ");
  }

}
