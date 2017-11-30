const Int_t npaddles=6;

Double_t offsetsL[npaddles];
Double_t offsetsR[npaddles];
TCanvas *cBs = 0;

void time_paddles(TTree *T=0) {
  if (!T) T=(TTree*)gDirectory->Get("T");
  if (!T) return;
  gStyle->SetPalette(1);


  cBs = new TCanvas("cdt","Scint. Timing Calib.",800,1000);
  cBs->Divide(1,2);
  cBs->cd(1);

  // simply by looking at events where multiple paddles fired,
  // calculate the time-offsets of each paddle.
  // NOT doing left-to-right yet, but will split the paddle time
  // between the left and right sides.

  TString t1="(.5*(R.s1.lt[R.s1.t_pads[0]]+R.s1.rt[R.s1.t_pads[0]]))";
  TString t2="(.5*(R.s1.lt[R.s1.t_pads[1]]+R.s1.rt[R.s1.t_pads[1]]))";
  TString cut="R.s1.nthit==2&&(R.s1.t_pads[0]+1==R.s1.t_pads[1])";
  cut += "&&R.tr.n==1&&R.vdc.u1.nclust==1&&R.vdc.u2.nclust==1&&R.vdc.v1.nclust==1&&R.vdc.v2.nclust==1";
  cut += "&&((DBB.evtypebits>>1)&1)&&DBB.edtpr[0]==0&&(R.ps.e+R.sh.e)>1200&&R.cer.asum_c>500";
  
  TString hist = Form("hs(%d,-0.5,%.1f,100,-100,100)",npaddles,npaddles-.5);
  TString sel = t1 + "-" + t2 + ":R.s1.t_pads[0]>>"+hist;
  cout << "selection is \n" << sel << "\n and the cut is \n" << cut << endl;
  T->Draw(sel.Data(),cut.Data(),"colz");

  TH2F* h2 = (TH2F*)gDirectory->Get("hs");
  Double_t offsets[npaddles];
  offsets[0]=0.;
  
  for (int i=1; i<npaddles; i++) {
    cBs->cd(2);
    
    TH1* h = h2->ProjectionY("py",i,i);
     Double_t dt = h->GetMean();
    // Double_t dt = h->GetBinCenter(h->GetMaximumBin());
    offsets[i] = offsets[i-1] + dt;
    h->Draw();
    cBs->Print(Form("s1_paddles_fit1_%d.png",i));
  }

  // set the central (paddle 7) offset to 0.
  int mid = (npaddles+1)/2;
  Double_t shift = offsets[mid];
  for (int i=0; i<npaddles; i++) {
    offsets[i] -= shift;
    offsetsL[i]=-offsets[i];
    offsetsR[i]=-offsets[i];
  }
  
}


void Regularize(TProfile* h) {
  Double_t aveE=0;
  Int_t npts=0;
  Double_t err;
  for (int i=1; i<=h->GetNbinsX(); i++) {
    err = h->GetBinError(i);
    if (err>0) {
      aveE+=err;
      npts++;
    }
  }
  aveE/=npts;
  for (int i=1; i<=h->GetNbinsX(); i++) {
    err = h->GetBinError(i);
    if (err<aveE) {
      //      h->SetBinError(i,aveE);
    }
  }
}

void time_lr(TTree* T=0) {
  if (!T) T=(TTree*)gDirectory->Get("T");
  if (!T) return;
  gStyle->SetPalette(1);

  // create lt-rt vs. B.ts.sh.y[0] histograms. x
  // actually vs. R.s1.try -> position along the scint. paddles!
  // and fit to get the L-R time offsets, and collect
  // the slope

  //  TString t1="(R.s1.lt[R.s1.t_pads[0]]-R.s1.rt[R.s1.t_pads[0]]):B.ts.sh.y[0]";
  TString t1="(R.s1.lt[R.s1.t_pads[0]]-R.s1.rt[R.s1.t_pads[0]]):R.s1.try[0]";
  TString cut="R.s1.nthit==1";
  cut += "&&R.tr.n==1&&R.vdc.u1.nclust==1&&R.vdc.u2.nclust==1&&R.vdc.v1.nclust==1&&R.vdc.v2.nclust==1";
  cut += "&&((DBB.evtypebits>>1)&1)&&DBB.edtpr[0]==0&&(R.ps.e+R.sh.e)>1200&&R.cer.asum_c>500";

  TString hist = "hdtvy(50,-.1,.1,1000,-1000.,1000.)";

  Double_t lrdiff[npaddles];
  Double_t cn[npaddles];

  
  for (int i=0; i<npaddles; i++) {
    TString cuts = cut+Form("&&R.s1.t_pads[0]==%d",i);
    cBs && cBs->cd(1);
    T->Draw(t1+">>"+hist,cuts);
    TH2F* h = (TH2F*)gDirectory->Get("hdtvy");
    Double_t avey = h->GetMean(2);
    T->Draw(t1+Form(">>hdtvy_%d(50,-.05,.05,100,%f,%f)",i,avey-200.,avey+200.),
	    cuts);
    TH2F* hd2 = (TH2F*)gDirectory->Get(Form("hdtvy_%d",i));

    cBs && cBs->cd(2);
    TProfile *pf = hd2->ProfileX();
    Regularize(pf);
    pf->Fit("pol1");
    TF1 *fnc = pf->GetFunction("pol1");
    lrdiff[i] = fnc->GetParameter(0);
    cn[i]     = 2./fnc->GetParameter(1);
    gPad->GetCanvas()->Update();
    cBs->Print(Form("s1_paddles_fit2_%d.png",i));
  }

  // adjust the offsets
  for (int i=0; i<npaddles; i++) {
    offsetsL[i] += lrdiff[i]/2.;
    offsetsR[i] -= lrdiff[i]/2.;
  }

  for (int i=0; i<npaddles; i++) {
    printf("s1_paddle %2d : cn = %f\n",i,cn[i]);
  }
  
}


void writeOffsets() {
  // apply to both left and right sides
  FILE *fo = fopen("R.s1.toff.dat","w");
  fprintf(fo,"TDC time offsets of S in TDC channels -------------------\n");
  printf("TDC time offsets of S in TDC channels -------------------\n");
  for (int i=0; i<npaddles; i++) {
    fprintf(fo," %.2f",offsetsL[i]);
    printf(" %.2f",offsetsL[i]);
  }
  fprintf(fo," - Left Paddles\n");
  printf(" - Left Paddles\n");
  for (int i=0; i<npaddles; i++) {
    fprintf(fo," %.2f",offsetsR[i]);
    printf(" %.2f",offsetsR[i]);
  }
  fprintf(fo," - Right Paddles\n");
  printf(" - Right Paddles\n");
  fclose(fo);
}

void time_s1(TTree *T=0) {
  TProfile::Approximate();
  for (int i=0; i<npaddles; i++) {
    offsetsL[i]=offsetsR[i]=0.;
  }
  time_paddles(T);
  time_lr(T);
  writeOffsets();
}

void time_s1_paddles()
{
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d.root",run_number);
  TChain *T = new TChain("T");
  T->Add(filename);
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d.root",run_number+1);
  T->Add(filename);
  filename = Form("/GoFlex/Rootfiles/e08014_R_%d_1.root",run_number+1);
  T->Add(filename);


  time_s1(T);
}
