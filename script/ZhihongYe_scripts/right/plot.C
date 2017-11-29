{
  TChain *T = new TChain("T");
  //  T->Add("/GoFlex/Rootfiles/e08014_R_3863.root");
  T->Add("/work/halla/e08014/disk1/Rootfiles/e08014_R4_3858.root");
  T->Add("/work/halla/e08014/disk1/Rootfiles/e08014_R4_3858_1.root");
  T->Add("/work/halla/e08014/disk1/Rootfiles/e08014_R4_3858_2.root");
 
  TCanvas *c1 = new TCanvas("c1","c1",1000,1000);;
  c1->Divide(2,5);



  TH1F *h1 = new TH1F("h1","h1",300,10,3000);  
  TH1F *h2 = new TH1F("h2","h2",300,10,3000);  
  TH1F *h3 = new TH1F("h3","h3",300,10,3000); 

  Int_t evt1,evt2,evt3;

  for(int i=0;i<10;i++){
    c1->cd(i+1);
    gPad->SetLogy(0);
    TH1F *h1 = new TH1F("h1","h1",100,10,400);  
    TH1F *h2 = new TH1F("h2","h2",100,10,400);  
    TH1F *h3 = new TH1F("h3","h3",100,10,400); 
        //    T->Draw(Form("R.cer.a[%d]:R.cer.t[%d]>>h1(300,10.,3000.,300,0.,2000.)",i,i),Form("R.cer.t[%d]>10&&R.cer.t[%d]<3000.&&DBB.edtpr[0]==0&&DBB.evtypebits&&(1<<2)&&R.sh.asum_c>1100&&R.ps.asum_c>1500",i,i));
    T->Draw(Form("R.cer.a_c[%d]>>h1",i),Form("R.cer.t[%d]>0 && R.sh.e+R.ps.e<600.",i));
    evt1 = h1->GetEntries(); c1->Update();
    h1->SetLineColor(1); h1->SetXTitle(Form("R.cer.a_c[%d]",i)); h1->SetTitle(Form("R-Cer#%d",i+1));

    T->Draw(Form("R.cer.a_c[%d]>>h2",i),Form("R.cer.t[%d]>0 && ((DBB.evtypebits>>1)&1) && R.sh.e+R.ps.e<600.",i));
    evt2 = h2->GetEntries(); c1->Update();// h2->Scale(evt1/evt2);
    h2->SetLineColor(2); h3->SetXTitle(Form("R.cer.a_c[%d]",i)); h3->SetTitle(Form("R-Cer#%d",i+1));
    h1->Draw();
    T->Draw(Form("R.cer.a_c[%d]>>h3",i),Form("R.cer.t[%d]>0 && ((DBB.evtypebits>>2)&1)",i));
    evt3 = h3->GetEntries(); c1->Update(); //h3->Scale(evt1/evt3);
    h3->SetLineColor(4); h3->SetXTitle(Form("R.cer.a_c[%d]",i)); h3->SetTitle(Form("R-Cer#%d",i+1));

    h1->Draw();
    h2->Draw("same");
     h3->Draw("same");

  }
}
