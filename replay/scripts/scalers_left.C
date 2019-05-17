
void scalers_left(Int_t flag, TString drawoption){

  gStyle->SetOptStat(0);

  TTree *tree = (TTree*)gDirectory->Get("evLeft");
  Double_t nentries = evLeft->GetEntries();
	
  if(flag ==1){

    Double_t max = evLeft->GetMaximum("evLeftLclock");

    TH2F *h1 = new TH2F("h1","L clock",100,0,nentries,100,0,max);
    tree->Draw("evLeftLclock:Entry$>>h1","",drawoption);
    h1->GetXaxis()->SetTitle("Entry #");h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("evLeftLclock (counts)");h1->GetXaxis()->CenterTitle();

 }

  if(flag ==2){

    Double_t max1 = evLeft->GetMaximum("evLeftRclock");	
    TH2F *h2 = new TH2F("h2","R clock",100,0,nentries,100,0,max1);
    h2->GetXaxis()->SetTitle("Entry #");h2->GetXaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("evLeftRclock (counts)");h2->GetYaxis()->CenterTitle();

    tree->Draw("evLeftRclock:Entry$>>h2","",drawoption);

  }
  if(flag ==3){
    Double_t max2 = evLeft->GetMaximum("evLeftunser");
    TH2F *h3 = new TH2F("h3","Unser",100,0,nentries,100,0,max2);
    h3->GetXaxis()->SetTitle("Entry #");h3->GetXaxis()->CenterTitle();
    h3->GetYaxis()->SetTitle("Unser (counts)");h3->GetYaxis()->CenterTitle();

    tree->Draw("evLeftunser:Entry$>>h3","",drawoption);

  }
  if(flag ==4){
    Double_t max3 = evLeft->GetMaximum("evLeftd3");
    TH2F *h4 = new TH2F("h4","d3",100,0,nentries,100,0,max3);
    h4->GetXaxis()->SetTitle("Entry #");h4->GetXaxis()->CenterTitle();
    h4->GetYaxis()->SetTitle("evLeftd3 (counts)");h4->GetYaxis()->CenterTitle();

    tree->Draw("evLeftd3:Entry$>>h4","",drawoption);

  }
  if(flag ==5){
    Double_t max4 = evLeft->GetMaximum("evLeftunew");
    TH2F *h5 = new TH2F("h5","unew",100,0,nentries,100,0,max4);
    h5->GetXaxis()->SetTitle("Entry #");h5->GetXaxis()->CenterTitle();
    h5->GetYaxis()->SetTitle("evLeftunew (counts)");h5->GetYaxis()->CenterTitle();

    tree->Draw("evLeftunew:Entry$>>h5","",drawoption);

  }
   if(flag ==6){
    Double_t max5 = T->GetMaximum("evLeftdnew");
    TH2F *h6 = new TH2F("h6","dnew",100,0,nentries,100,0,max5);
    h6->GetXaxis()->SetTitle("Entry #");h6->GetXaxis()->CenterTitle();
    h6->GetYaxis()->SetTitle("evLeftdnew (counts)");h6->GetYaxis()->CenterTitle();

    tree->Draw("evLeftdnew:Entry$>>h6","",drawoption);

  }

 if(flag ==7){
    Double_t max6 = evLeft->GetMaximum("evLeftL1A");
    TH2F *h7 = new TH2F("h7","L1A",100,0,nentries,100,0,max6);
    h7->GetXaxis()->SetTitle("Entry #");h7->GetXaxis()->CenterTitle();
    h7->GetYaxis()->SetTitle("L1A (counts)");h7->GetYaxis()->CenterTitle();

    tree->Draw("evLeftL1A:Entry$>>h7","",drawoption);

  }
   if(flag ==8){
    Double_t max7 = evLeft->GetMaximum("evLeftT1");
    TH2F *h8 = new TH2F("h8","LeftT1",100,0,nentries,100,0,max7);
    h8->GetXaxis()->SetTitle("Entry #");h8->GetXaxis()->CenterTitle();
    h8->GetYaxis()->SetTitle("T1 (counts)");h8->GetYaxis()->CenterTitle();

    tree->Draw("evLeftT1:Entry$>>h8","",drawoption);

  }

 if(flag ==9){
    Double_t max8 = evLeft->GetMaximum("evLeftT2");
    TH2F *h9 = new TH2F("h9","evLeftT2",100,0,nentries,100,0,max8);
    h9->GetXaxis()->SetTitle("Entry #");h9->GetXaxis()->CenterTitle();
    h9->GetYaxis()->SetTitle("T2 (counts)");h9->GetYaxis()->CenterTitle();

    tree->Draw("evLeftT2:Entry$>>h9","",drawoption);
  }

  if(flag ==10){
    Double_t max9 = evLeft->GetMaximum("evLeftT3");
    TH2F *h10 = new TH2F("h10","evLeftT3",100,0,nentries,100,0,max9);
    h10->GetXaxis()->SetTitle("Entry #");h10->GetXaxis()->CenterTitle();
    h10->GetYaxis()->SetTitle("T3 (counts)");h10->GetYaxis()->CenterTitle();

    tree->Draw("evLeftT3:Entry$>>h10","",drawoption);

  }





}
