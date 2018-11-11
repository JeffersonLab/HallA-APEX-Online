void e_peak(Int_t run=0, Double_t current=0){
  TString rpath = "/chafs1/work1/tritium/tmp_data/src_fall";

  TChain *rootfile = new TChain("T");
  
  //Load rootfiles
  if(run==0){
    cout << "Please make the first parameter the desired run number" << endl << endl;
    return;
  }else{
    int i = 1;
  
    if(!gSystem->AccessPathName(TString::Format("%s/tritium_%d.root",rpath.Data(),run),kFileExists)){
      rootfile->Add(TString::Format("%s/tritium_%d.root",rpath.Data(),run));
      cout << "Added file: tritium_" << run << ".root" << endl;
    }else{
      cout << "Requested run has not been replayed. Exiting." << endl << endl;
      return;
    }
  
    while(!gSystem->AccessPathName(TString::Format("%s/tritium_%d_%d.root",rpath.Data(),run,i),kFileExists)){
	rootfile->Add(TString::Format("%s/tritium_%d_%d.root",rpath.Data(),run,i));
      cout << "Added file: tritium_" << run << "_" << i << ".root" << endl;
      i=i+1;
    }                      
  }

  TH2D *wvy = new TH2D("wvy","W vs Raster Y",25,76000,80000,150,.932,.942);

  TString cut;

  if(run<90000){
    cut = "L.tr.n==1&&(evLeftdnew_r*0.0003297)>";
    cut += current;
    rootfile->Draw("TMath::Sqrt(EKLx.W2):Lrb.Raster2.rawcur.y>>wvy",TCut(cut));
  }else{
    cut = "R.tr.n==1&&(evRightdnew_r*0.0003297)>";
    cut += current;
    rootfile->Draw("TMath::Sqrt(EKRx.W2):Rrb.Raster2.rawcur.y>>wvy",TCut(cut));
  }

  TF1 *gaus_fit = new TF1("Gaussian","gaus",.92,.95);
  //TObjArray arr;

  wvy->FitSlicesY(gaus_fit,1,25,0,"QNR");
  
  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  TH1D *out = (TH1D*)gDirectory->Get("wvy_1");
  out->Draw();
}
