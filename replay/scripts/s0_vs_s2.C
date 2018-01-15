void s0_vs_s2(Int_t spec=0,int plot=0)     //LHRS: spec=0; RHRS: spec=1
{
  gStyle->SetOptStat(0);

  Double_t s2la[16],s2ra[16];
  Double_t s0la,s0ra;

  Double_t Cersum=0,pse=0,she=0;

  Double_t trp; 

  TTree *tree = (TTree*)gDirectory->Get("T");

  if(spec==0){
     tree->SetBranchAddress("L.s2.la_c",s2la);
     tree->SetBranchAddress("L.s2.ra_c",s2ra);
     tree->SetBranchAddress("L.s0.la_c",&s0la);
     tree->SetBranchAddress("L.s0.ra_c",&s0ra);

     tree->SetBranchAddress("L.cer.asum_c",&Cersum);
     tree->SetBranchAddress("L.prl1.e",&pse);
     tree->SetBranchAddress("L.prl2.e",&she);

     tree->SetBranchAddress("L.gold.p",&trp);
   }

  if(spec==1){
     tree->SetBranchAddress("R.s2.la_c",s2la);
     tree->SetBranchAddress("R.s2.ra_c",s2ra);
     tree->SetBranchAddress("R.s0.la_c",&s0la);
     tree->SetBranchAddress("R.s0.ra_c",&s0ra);

     tree->SetBranchAddress("R.cer.asum_c",&Cersum);
     tree->SetBranchAddress("R.ps.e",&pse);
     tree->SetBranchAddress("R.sh.e",&she);

     tree->SetBranchAddress("R.gold.p",&trp);
   }

  Double_t ep = 0;
  Double_t s2_adc=0;
  Double_t s0_adc=0;

  TH2F *hs0s2 = new TH2F("hs0s2","s0 adc vs s2 adc",1000,0,4000,1000,0,4000);
  hs0s2->GetXaxis()->SetTitle("s0_adc");hs0s2->GetXaxis()->CenterTitle();
  hs0s2->GetYaxis()->SetTitle("s2_adc");hs0s2->GetYaxis()->CenterTitle();


  TH2F *hs0s2w = new TH2F("hs0s2w","s0 adc vs s2 adc w cut",1000,0,4000,1000,0,4000);
  hs0s2w->GetXaxis()->SetTitle("s0_adc");hs0s2w->GetXaxis()->CenterTitle();
  hs0s2w->GetYaxis()->SetTitle("s2_adc");hs0s2w->GetYaxis()->CenterTitle();

  Int_t entries = tree->GetEntries();
  for(Int_t ii=0; ii<entries; ii++){
     tree->GetEntry(ii);

     s2_adc=0;s0_adc=0;
     for(int i=0;i<32;i++){
        if(i/16==0) s2_adc += s2la[i];
        if(i/16==1) s2_adc += s2ra[i-16];
      }
     s0_adc = s0la+s0ra;
     hs0s2->Fill(s0_adc,s2_adc);

     if(trp>0)ep = (pse+she)/(1000*trp);
     if(ep>0.7 && Cersum>2000)hs0s2w->Fill(s0_adc,s2_adc);

   }
  if(plot==0)
    hs0s2->Draw("colz");
  if(plot==1)
    hs0s2w->Draw("colz");

}
