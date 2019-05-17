//Coincidence plots
//Reynier Cruz Torres, Feb 2017
void coinc_plots1(Int_t flag, Int_t flag1){
  gStyle->SetOptStat(0);
  
  TTree *tree = (TTree*)gDirectory->Get("T");
  
  TString trigger;
  if     (flag1==0){trigger = ""                    ;}
  else if(flag1==1){trigger = "(DR.evtypebits>>3)&1";} 
  else if(flag1==2){trigger = "(DR.evtypebits>>4)&1";}
  else if(flag1==3){trigger = "(DR.evtypebits>>1)&1";}

  // =============== TRACKING VARIABLES ===============
  if(flag==1){
    TH2F *h_Rey = new TH2F("h_Rey","Delta",60,-.2,.2,60,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("#delta (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("#delta (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_dp:L.tr.tg_dp>>h_Rey",trigger,"COLZ");
  }

  else if(flag==2){
    TH2F *h_Rey = new TH2F("h_Rey","Theta",60,-.2,.2,60,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("#theta (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("#theta (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_th:L.tr.tg_th>>h_Rey",trigger,"COLZ");
  }

  else if(flag==3){
    TH2F *h_Rey = new TH2F("h_Rey","Phi",60,-.2,.2,60,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("#phi (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("#phi (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_ph:L.tr.tg_ph>>h_Rey",trigger,"COLZ");
  }

  else if(flag==4){
    TH2F *h_Rey = new TH2F("h_Rey","Target y",60,-.2,.2,60,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("y (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("y (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("R.tr.tg_y:L.tr.tg_y>>h_Rey",trigger,"COLZ");
  }

  // =============== REACTION VERTEX VARIABLES ===============
  else if(flag==5){
    TH2F *h_Rey = new TH2F("h_Rey","Reaction vertex - x",100,-.2,.2,100,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("x (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("x (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("rpr.x:rpl.x>>h_Rey",trigger,"COLZ");
  }

  else if(flag==6){
    TH2F *h_Rey = new TH2F("h_Rey","Reaction vertex - y",100,-.2,.2,100,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("y (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("y (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("rpr.y:rpl.y>>h_Rey",trigger,"COLZ");
  }

  else if(flag==7){
    TH2F *h_Rey = new TH2F("h_Rey","Reaction vertex - z",100,-.2,.2,100,-.2,.2);
    h_Rey->GetXaxis()->SetTitle("z (RHRS)");h_Rey->GetXaxis()->CenterTitle();
    h_Rey->GetYaxis()->SetTitle("z (LHRS)");h_Rey->GetYaxis()->CenterTitle();
    tree->Draw("rpr.z:rpl.z>>h_Rey",trigger,"COLZ");
  }

}
