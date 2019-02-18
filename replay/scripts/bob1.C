{



  TFile *_file1 = TFile::Open("apex_online_3087.root");
  TH1F *h1 = new TH1F("h1","Trues + Randoms, Run 3087",100,1500,1900);
  T->Draw("DR.rrawt5>>h1");


  TFile *_file2 = TFile::Open("apex_online_3088.root");
  TH1F *h2 = new TH1F("h2","Trues Only, Run 3088",100,1500,1900);
  T->Draw("DR.rrawt5>>h2");

  TFile *_file3 = TFile::Open("apex_online_3089.root");
  TH1F *h3 = new TH1F("h3","Randoms Only, Run 3089",100,1500,1900);
  T->Draw("DR.rrawt5>>h3");



}
