
void scalers_right(Int_t flag, TString drawoption){

  gStyle->SetOptStat(0);

  TTree *tree = (TTree*)gDirectory->Get("evRight");
  Double_t nentries = evRight->GetEntries();
	
  if(flag ==1){

    Double_t max = evRight->GetMaximum("evRightLclock");

    TH2F *r1 = new TH2F("h1","L clock",100,0,nentries,100,0,max);
    r1->GetXaxis()->SetTitle("Entry #");r1->GetYaxis()->CenterTitle();
    r1->GetYaxis()->SetTitle("evRightLclock (counts)");r1->GetXaxis()->CenterTitle();
    tree->Draw("evRightLclock:Entry$>>r1","",drawoption);

 }

  if(flag ==2){

    Double_t max1 = evRight->GetMaximum("evRightRclock");	
    TH2F *h2 = new TH2F("h2","R clock",100,0,nentries,100,0,max1);
    h2->GetXaxis()->SetTitle("Entry #");h2->GetXaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("evRightRclock (counts)");h2->GetYaxis()->CenterTitle();

    tree->Draw("evRightRclock:Entry$","",drawoption);

  }
  if(flag ==3){
    Double_t max2 = evRight->GetMaximum("evRightunser");
    TH2F *h3 = new TH2F("h3","Unser",100,0,nentries,100,0,max2);
    h3->GetXaxis()->SetTitle("Entry #");h3->GetXaxis()->CenterTitle();
    h3->GetYaxis()->SetTitle("Unser (counts)");h3->GetYaxis()->CenterTitle();

    tree->Draw("evRightunser:Entry$>>h3","",drawoption);

  }
  if(flag ==4){
    Double_t max3 = evRight->GetMaximum("evRightd3");
    TH2F *h4 = new TH2F("h4","d3",100,0,nentries,100,0,max3);
    h4->GetXaxis()->SetTitle("Entry #");h4->GetXaxis()->CenterTitle();
    h4->GetYaxis()->SetTitle("evRightd3 (counts)");h4->GetYaxis()->CenterTitle();

    tree->Draw("evRightd3:Entry$>>h4","",drawoption);

  }
  if(flag ==5){
    Double_t max4 = evRight->GetMaximum("evRightunew");
    TH2F *h5 = new TH2F("h5","unew",100,0,nentries,100,0,max4);
    h5->GetXaxis()->SetTitle("Entry #");h5->GetXaxis()->CenterTitle();
    h5->GetYaxis()->SetTitle("evRightunew (counts)");h5->GetYaxis()->CenterTitle();

    tree->Draw("evRightunew:Entry$>>h5","",drawoption);

  }
   if(flag ==6){
    Double_t max5 = T->GetMaximum("evRightdnew");
    TH2F *h6 = new TH2F("h6","dnew",100,0,nentries,100,0,max5);
    h6->GetXaxis()->SetTitle("Entry #");h6->GetXaxis()->CenterTitle();
    h6->GetYaxis()->SetTitle("evRightdnew (counts)");h6->GetYaxis()->CenterTitle();

    tree->Draw("evRightdnew:Entry$>>h6","",drawoption);

  }

 if(flag ==7){
    Double_t max6 = evRight->GetMaximum("evRightL1A");
    TH2F *h7 = new TH2F("h7","L1A",100,0,nentries,100,0,max6);
    h7->GetXaxis()->SetTitle("Entry #");h7->GetXaxis()->CenterTitle();
    h7->GetYaxis()->SetTitle("L1A (counts)");h7->GetYaxis()->CenterTitle();

    tree->Draw("evRightL1A:Entry$>>h7","",drawoption);

  }
   if(flag ==8){
    Double_t max7 = evRight->GetMaximum("evRightT4");
    TH2F *h8 = new TH2F("h8","evRightT4",100,0,nentries,100,0,max7);
    h8->GetXaxis()->SetTitle("Entry #");h8->GetXaxis()->CenterTitle();
    h8->GetYaxis()->SetTitle("T4 (counts)");h8->GetYaxis()->CenterTitle();

    tree->Draw("evRightT4:Entry$>>h8","",drawoption);

  }

 if(flag ==9){
    Double_t max8 = evRight->GetMaximum("evRightT5");
    TH2F *h9 = new TH2F("h9","evRightT5",100,0,nentries,100,0,max8);
    h9->GetXaxis()->SetTitle("Entry #");h9->GetXaxis()->CenterTitle();
    h9->GetYaxis()->SetTitle("T5 (counts)");h9->GetYaxis()->CenterTitle();

    tree->Draw("evRightT5:Entry$>>h9","",drawoption);
  }

  if(flag ==10){
    Double_t max9 = evRight->GetMaximum("evRightT6");
    TH2F *h10 = new TH2F("h10","evRightT6",100,0,nentries,100,0,max9);
    h10->GetXaxis()->SetTitle("Entry #");h10->GetXaxis()->CenterTitle();
    h10->GetYaxis()->SetTitle("T6 (counts)");h10->GetYaxis()->CenterTitle();

    tree->Draw("evRightT6:Entry$>>h10","",drawoption);

  }





}
