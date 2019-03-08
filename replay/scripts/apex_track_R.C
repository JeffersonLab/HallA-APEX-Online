// -------------------
// plot the rawtime vs wire number per VDC plane per event for RHRS
// shujie, 2019.3.7
// -------------------

void apex_track_R(Int_t runnum, Int_t entry=1){


  gStyle->SetTitleYSize(0.06);
  gStyle->SetTitleXSize(0.06);
  gStyle->SetLabelSize(0.06,"X");
  gStyle->SetLabelSize(0.06,"Y");
  gStyle->SetPaperSize(18,22);
  gStyle->SetOptStat(0);

  // only load the first split of rootfiles
  TChain *t = new TChain("T");
  TString rootfile = Form("../rootfiles/apex_%d.root",runnum);
  t->Add(rootfile.Data());
  if(!t){
   cout<< "can't find rootfile for run "<<runnum<<endl;
   exit(0);
   }
  Int_t nn=t->GetEntries();
  cout<<"=="<<rootfile<<" loaded, "<<nn<<" events in total==\n";


  TCanvas *c1  = new TCanvas("c1","rawtime : wire number",1200,800);
  TH2F    *h1  = new TH2F("h1","u1 rawtime : wire number",400,0,400,200,2200,3000);
  TH2F    *h2  = new TH2F("h2","v1 rawtime : wire number",400,0,400,200,2200,3000);
  TH2F    *h3  = new TH2F("h3","u2 rawtime : wire number",400,0,400,200,2200,3000);
  TH2F    *h4  = new TH2F("h4","v2 rawtime : wire number",400,0,400,200,2200,3000);

  c1->Divide(2,2);

  //--------------------------------
  t->SetBranchStatus("*"       ,0);
  t->SetBranchStatus("R.vdc.*" ,1);
  t->SetBranchStatus("R.tr.n"  ,1);
 
  Double_t    nclust1,nclust2,nclust3,nclust4, ntrk;
  Double_t    clbeg1[40], clend1[40];
  Double_t    clbeg2[40], clend2[40];
  Double_t    clbeg3[40], clend3[40];
  Double_t    clbeg4[40], clend4[40];

  t->SetBranchAddress("R.tr.n",&ntrk);

  t->SetBranchAddress("R.vdc.u1.nclust",&nclust1);
  t->SetBranchAddress("R.vdc.v1.nclust",&nclust2);
  t->SetBranchAddress("R.vdc.u2.nclust",&nclust3);
  t->SetBranchAddress("R.vdc.v2.nclust",&nclust4);

  t->SetBranchAddress("R.vdc.u1.clbeg", clbeg1);
  t->SetBranchAddress("R.vdc.v1.clbeg", clbeg2);
  t->SetBranchAddress("R.vdc.u2.clbeg", clbeg3);
  t->SetBranchAddress("R.vdc.v2.clbeg", clbeg4);

  t->SetBranchAddress("R.vdc.u1.clend", clend1);
  t->SetBranchAddress("R.vdc.v1.clend", clend2);
  t->SetBranchAddress("R.vdc.u2.clend", clend3);
  t->SetBranchAddress("R.vdc.v2.clend", clend4);

  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextAlign(11);

  while(entry>0){
    t->GetEntry(entry);
    cout<<"\n--now plot VDC rawtime vs wire number from event "<<entry<<endl;

    //t->Project("h1","R.vdc.u1.rawtime:R.vdc.u1.wire",Form("Entry$==%d",entry));
    //t->Project("h2","R.vdc.v1.rawtime:R.vdc.v1.wire",Form("Entry$==%d",entry));
    //t->Project("h3","R.vdc.u2.rawtime:R.vdc.u2.wire",Form("Entry$==%d",entry));
    //t->Project("h4","R.vdc.v2.rawtime:R.vdc.v2.wire",Form("Entry$==%d",entry));
    c1->cd(1);t->Draw("R.vdc.u1.rawtime:R.vdc.u1.wire>>h1",Form("Entry$==%d",entry),"box");
    tex.SetTextColor(kRed);
    tex.DrawLatexNDC(0.12,0.85,Form("Run %d, event #%d",runnum,entry));
    // cout<<Form("%g track, %g clusters found:",ntrk, nclust1)<<endl;
    tex.DrawLatexNDC(0.12,0.80,Form("%g track, %g clusters found:",ntrk, nclust1));
    tex.SetTextColor(kBlack);

    for(int ii=0;ii<nclust1;ii++){
      tex.DrawLatexNDC(0.12,0.75-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg1[ii],clend1[ii]));
    }


    c1->cd(2);
    t->Draw("R.vdc.v1.rawtime:R.vdc.v1.wire>>h2",Form("Entry$==%d",entry),"box");
    for(int ii=0;ii<nclust2;ii++){
      tex.DrawLatexNDC(0.12,0.85-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg2[ii],clend2[ii]));
    }

    c1->cd(3);
    t->Draw("R.vdc.u2.rawtime:R.vdc.u2.wire>>h3",Form("Entry$==%d",entry),"box");
    for(int ii=0;ii<nclust3;ii++){
      tex.DrawLatexNDC(0.12,0.85-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg3[ii],clend3[ii]));
    }

    c1->cd(4);
    t->Draw("R.vdc.v2.rawtime:R.vdc.v2.wire>>h4",Form("Entry$==%d",entry),"box");
    for(int ii=0;ii<nclust4;ii++){
      tex.DrawLatexNDC(0.12,0.85-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg4[ii],clend4[ii]));
    }
    // c1->cd(1);h1->Draw("box");

    // c1->cd(2);h2->Draw("box");
    // c1->cd(3);h3->Draw("box");
    // c1->cd(4);h4->Draw("box");
    // break;
    int index;
    c1->Update();
    cout << "\n--Which event you want to check next ?\n(Please enter an event number. ENTER = next event, -1 to exit) ";
    if(cin.peek() == '\n')
      entry++;
    else{
      cin >> index;
      cin.ignore();
      if (index==-1) break;
      else if (index>0 && index<nn){
        entry = index;
      }
      else{
        cout<<"!! Please enter a valid event number !!";
        continue;
      }
    }

  } 
  
}
