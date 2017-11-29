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

  TString t1="(.5*(L.s2.lt[L.s2.t_pads[0]]+L.s2.rt[L.s2.t_pads[0]]))";
  TString t2="(.5*(L.s2.lt[L.s2.t_pads[1]]+L.s2.rt[L.s2.t_pads[1]]))";
  TString cut="L.s2.nthit==2&&(L.s2.t_pads[0]+1==L.s2.t_pads[1])";
  
  TString hist = "hs(13,-0.5,12.5,100,-400,400)";
  TString sel = t1 + "-" + t2 + ":L.s2.t_pads[0]>>"+hist;
  cout << "selection is \n" << sel << "\n and the cut is \n" << cut << endl;
  T->Draw(sel.Data(),cut.Data(),"colz");

  TH2F* h2 = (TH2F*)gDirectory->Get("hs");
  Double_t offsets[npaddles];
  offsets[0]=0.;
  
  for (int i=1; i<npaddles; i++) {
    TH1* h = h2->ProjectionY("py",i,i);
    Double_t dt = h->GetMean();
    offsets[i] = offsets[i-1] + dt;
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


  cBs && cBs->cd(2);

  // create lt-rt vs. B.ts.sh.y[0] histograms. x
  // actually vs. L.s2.try -> position along the scint. paddles!
  // and fit to get the L-R time offsets, and collect
  // the slope

  //  TString t1="(L.s2.lt[L.s2.t_pads[0]]-L.s2.rt[L.s2.t_pads[0]]):B.ts.sh.y[0]";
  TString t1="(L.s2.lt[L.s2.t_pads[0]]-L.s2.rt[L.s2.t_pads[0]]):L.s2.try[0]";
  TString cut="L.s2.nthit==1";
  cut += "&&L.tr.n==1&&L.vdc.u1.nclust==1&&L.vdc.u2.nclust==1&&L.vdc.v1.nclust==1&&L.vdc.v2.nclust==1";
  
  TString hist = "hdtvy(50,-.1,.1,1000,-1000.,1000.)";

  Double_t lrdiff[npaddles];
  Double_t cn[npaddles];

  
  for (int i=0; i<npaddles; i++) {
    TString cuts = cut+Form("&&L.s2.t_pads[0]==%d",i);
    T->Draw(t1+">>"+hist,cuts);
    TH2F* h = (TH2F*)gDirectory->Get("hdtvy");
    Double_t avey = h->GetMean(2);
    T->Draw(t1+Form(">>hdtvy_%d(50,-.1,.1,100,%f,%f)",i,avey-200.,avey+200.),
	    cuts);
    TH2F* hd2 = (TH2F*)gDirectory->Get(Form("hdtvy_%d",i));
    TProfile *pf = hd2->ProfileX();
    Regularize(pf);
    pf->Fit("pol1");
    TF1 *fnc = pf->GetFunction("pol1");
    lrdiff[i] = fnc->GetParameter(0);
    cn[i]     = 2./fnc->GetParameter(1);
    gPad->GetCanvas()->Update();
  }

  // adjust the offsets
  for (int i=0; i<npaddles; i++) {
    offsetsL[i] += lrdiff[i]/2.;
    offsetsR[i] -= lrdiff[i]/2.;
  }

  for (int i=0; i<npaddles; i++) {
    printf("paddle %2d : cn = %f\n",i,cn[i]);
  }
  
}


void writeOffsets(Double_t shiftch=0.) {
  // apply to both left and right sides
  FILE *fo = fopen("L.s2.toff.dat","w");
  fprintf(fo,"TDC time offsets of S in TDC channels -------------------\n");
  printf("TDC time offsets of S in TDC channels -------------------\n");
  for (int i=0; i<npaddles; i++) {
    fprintf(fo," %.2f",offsetsL[i]+shiftch);
    printf(" %.2f",offsetsL[i]+shiftch);
  }
  fprintf(fo," - Left Paddles\n");
  printf(" - Left Paddles\n");
  for (int i=0; i<npaddles; i++) {
    fprintf(fo," %.2f",offsetsR[i]+shiftch);
    printf(" %.2f",offsetsR[i]+shiftch);
  }
  fprintf(fo," - Right Paddles\n");
  printf(" - Right Paddles\n");
  fclose(fo);
}

void time_s2(TTree *T=0) {
  TProfile::Approximate();
  for (int i=0; i<npaddles; i++) {
    offsetsL[i]=offsetsR[i]=0.;
  }
  time_paddles(T);
  time_lr(T);
}

