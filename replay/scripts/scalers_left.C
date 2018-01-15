
void scalers_left(Int_t flag, TString drawoption){

  gStyle->SetOptStat(0);

  TTree *tree = (TTree*)gDirectory->Get("TSLeft");
  Double_t nentries = TSLeft->GetEntries();
	
  if(flag ==1){

    Double_t max = TSLeft->GetMaximum("LeftLclock");

    TH2F *h1 = new TH2F("h1","L clock",100,0,nentries,100,0,max);
    tree->Draw("LeftLclock:Entry$>>h1","",drawoption);
    h1->GetXaxis()->SetTitle("Entry #");h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("LeftLclock (counts)");h1->GetXaxis()->CenterTitle();

 }

  if(flag ==2){

    Double_t max1 = TSLeft->GetMaximum("LeftRclock");	
    TH2F *h2 = new TH2F("h2","R clock",100,0,nentries,100,0,max1);
    h2->GetXaxis()->SetTitle("Entry #");h2->GetXaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("LeftRclock (counts)");h2->GetYaxis()->CenterTitle();

    tree->Draw("LeftRclock:Entry$>>h2","",drawoption);

  }
  if(flag ==3){
    Double_t max2 = TSLeft->GetMaximum("Leftunser");
    TH2F *h3 = new TH2F("h3","Unser",100,0,nentries,100,0,max2);
    h3->GetXaxis()->SetTitle("Entry #");h3->GetXaxis()->CenterTitle();
    h3->GetYaxis()->SetTitle("Unser (counts)");h3->GetYaxis()->CenterTitle();

    tree->Draw("Leftunser:Entry$>>h3","",drawoption);

  }
  if(flag ==4){
    Double_t max3 = TSLeft->GetMaximum("Leftd3");
    TH2F *h4 = new TH2F("h4","d3",100,0,nentries,100,0,max3);
    h4->GetXaxis()->SetTitle("Entry #");h4->GetXaxis()->CenterTitle();
    h4->GetYaxis()->SetTitle("Leftd3 (counts)");h4->GetYaxis()->CenterTitle();

    tree->Draw("Leftd3:Entry$>>h4","",drawoption);

  }
  if(flag ==5){
    Double_t max4 = TSLeft->GetMaximum("Leftunew");
    TH2F *h5 = new TH2F("h5","unew",100,0,nentries,100,0,max4);
    h5->GetXaxis()->SetTitle("Entry #");h5->GetXaxis()->CenterTitle();
    h5->GetYaxis()->SetTitle("Leftunew (counts)");h5->GetYaxis()->CenterTitle();

    tree->Draw("Leftunew:Entry$>>h5","",drawoption);

  }
   if(flag ==6){
    Double_t max5 = T->GetMaximum("Leftdnew");
    TH2F *h6 = new TH2F("h6","dnew",100,0,nentries,100,0,max5);
    h6->GetXaxis()->SetTitle("Entry #");h6->GetXaxis()->CenterTitle();
    h6->GetYaxis()->SetTitle("Leftdnew (counts)");h6->GetYaxis()->CenterTitle();

    tree->Draw("Leftdnew:Entry$>>h6","",drawoption);

  }

 if(flag ==7){
    Double_t max6 = TSLeft->GetMaximum("LeftL1A");
    TH2F *h7 = new TH2F("h7","L1A",100,0,nentries,100,0,max6);
    h7->GetXaxis()->SetTitle("Entry #");h7->GetXaxis()->CenterTitle();
    h7->GetYaxis()->SetTitle("L1A (counts)");h7->GetYaxis()->CenterTitle();

    tree->Draw("LeftL1A:Entry$>>h7","",drawoption);

  }
   if(flag ==8){
    Double_t max7 = TSLeft->GetMaximum("LeftT1");
    TH2F *h8 = new TH2F("h8","LeftT1",100,0,nentries,100,0,max7);
    h8->GetXaxis()->SetTitle("Entry #");h8->GetXaxis()->CenterTitle();
    h8->GetYaxis()->SetTitle("T1 (counts)");h8->GetYaxis()->CenterTitle();

    tree->Draw("LeftT1:Entry$>>h8","",drawoption);

  }

 if(flag ==9){
    Double_t max8 = TSLeft->GetMaximum("LeftT2");
    TH2F *h9 = new TH2F("h9","LeftT2",100,0,nentries,100,0,max8);
    h9->GetXaxis()->SetTitle("Entry #");h9->GetXaxis()->CenterTitle();
    h9->GetYaxis()->SetTitle("T2 (counts)");h9->GetYaxis()->CenterTitle();

    tree->Draw("LeftT2:Entry$>>h9","",drawoption);
  }

  if(flag ==10){
    Double_t max9 = TSLeft->GetMaximum("LeftT3");
    TH2F *h10 = new TH2F("h10","LeftT3",100,0,nentries,100,0,max9);
    h10->GetXaxis()->SetTitle("Entry #");h10->GetXaxis()->CenterTitle();
    h10->GetYaxis()->SetTitle("T3 (counts)");h10->GetYaxis()->CenterTitle();

    tree->Draw("LeftT3:Entry$>>h10","",drawoption);

  }





}
