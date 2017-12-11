void cerR(Int_t run, Int_t flag){
  // Int_t run,flag;    
  // cout << "enter run_number: ";
  // cin >> run;
  // cout << "0=fastbus signal, 1=FADC signal: ";
  // cin >> flag;

  Double_t scale = 1;
  if (flag==1) scale=3;
 
  TString dir = "/chafs1/work1/tritium/Rootfiles/";
  TString fname =Form("tritium_%d.root",run);
  TString filename = dir+fname;

 
  TChain *T = new TChain("T");
  T->Add(filename);

  ofstream myfile,myfile2;
  myfile.open("../output/cer_ped_R.txt",ios::out|ios::app);
  myfile<<run<<"   "<<flag<<"   ";
  
  myfile2.open("../output/cer_peak_R.txt",ios::out|ios::app);
  myfile2<<run<<"   "<<flag<<"   ";
  
  cout << T->GetEntries() << endl;
  TCanvas *c1 = new TCanvas("c1","c1",1200,700);
  c1->Divide(5,2);
  TString name, tit, dr;
  Double_t min, max;
  Int_t max_bin;
  Int_t i;
  Double_t ped[10],peak[10];
  TH1F *h1[10], *hh[10], *hh1[10];
  TString cut = "(DR.evtypebits>>4)&1"; //s0&&S2
  //cut="";
  for(i=0;i<10;i++){
    h1[i] = new TH1F(Form("h%d",i),Form("h%d",i),300,0,10000);
  }
  for(i=0;i<10;i++){
    c1->cd(i+1);
    gPad->SetLogy();
    if (flag==0)
      T->Draw(Form("FbusR.cer.a[%d]>>h%d",i,i),cut);
    if (flag==1)
      T->Draw(Form("R.cer.a[%d]>>h%d",i,i),cut);
    max_bin = h1[i]->GetMaximumBin();
    ped[i] = h1[i]->GetBinCenter(max_bin);
    min = ped[i] - 100*scale;
    max = min + 400*scale;
    name = Form("gc_%d",i);
    tit = Form("gc_%d",i);
    hh[i] = new TH1F(name,tit,100*scale,min,max);
    if(flag==0)
      dr = Form("FbusR.cer.a[%d]",i);
    if(flag==1)
      dr = Form("R.cer.a[%d]",i);
    dr = dr + ">>";
    dr = dr + name;
    T->Draw(dr,cut);
    //fit pedestal
    TF1 *f1 = new TF1("f1", "gaus", ped[i]-20*scale, ped[i]+20*scale);
    hh[i]->Fit(f1, "Rq");
    ped[i] = f1->GetParameter(1);
    TLine *l1 = new TLine(ped[i],0,ped[i],hh[i]->GetMaximum());
    l1->SetLineColor(2);
    l1->Draw();

    //fit SPE
    hh[i]->GetXaxis()->SetRangeUser(ped[i],ped[i]+100*scale);
    Double_t dip=hh[i]->GetMinimumBin(); //find the dip b/w ped and SPE
    //cout<<ped[i]<<"  "<<dip<<endl;
    hh[i]->GetXaxis()->SetRangeUser(min,max);

    TF1 *f2 = new TF1("f2", "gaus", ped[i]+dip+20*scale, ped[i]+dip+100*scale);
    hh[i]->Fit(f2, "Rq");
    peak[i] = f2->GetParameter(1);
    TLine *l2 = new TLine(peak[i],0,peak[i],hh[i]->GetMaximum());
    l1->SetLineColor(2);
    l1->Draw();
  }
  for(i=0;i<10;i++){
    myfile<<setw(10) << ped[i] << "  "; 
    cout<<setw(10) << ped[i] << "   "<<peak[i]-ped[i]<<endl;
    myfile2<<setw(10)<<100.*scale/(peak[i]-ped[i])<<"  ";

  }
  myfile <<endl;
  myfile2 <<endl;
  cout << endl;

  if(flag==1) c1->Print(Form("../plots/cerR_%d.pdf",run));
  if(flag==0) c1->Print(Form("../plots/cerFbusR_%d.pdf",run));
}
