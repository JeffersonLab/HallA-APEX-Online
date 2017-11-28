const Int_t npaddles=16;

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
  cut += "&&((DBB.evtypebits>>3)&1)&&DBB.edtpl[0]==0&&L.prl1.e+L.prl2.e>600&&L.cer.asum_c>100";

  TString hist = "hs(16,-0.5,15.5,100,-100,100)";
  TString sel = t1 + "-" + t2 + ":L.s2.t_pads[0]>>"+hist;
  cout << "selection is \n" << sel << "\n and the cut is \n" << cut << endl;
  T->Draw(sel.Data(),cut.Data(),"colz");

  TH2F* h2 = (TH2F*)gDirectory->Get("hs");
  Double_t offsets[npaddles];
  offsets[0]=0.;
  
  for (int i=1; i<npaddles; i++) {

    cBs->cd(2);
    TH1* h = h2->ProjectionY("py",i,i);
    //    Double_t dt = h->GetMean();
    Double_t dt = h->GetBinCenter(h->GetMaximumBin());
    offsets[i] = offsets[i-1] + dt;
    h->Draw();
    cBs->Print(Form("s2_paddles_fit1_%d.png",i));
  }

  // set the central (paddle 7) offset to 0.
  int mid = (npaddles+1)/2;
  Double_t shift = offsets[mid];
  for (int i=0; i<npaddles; i++) {
    offsets[i] -= shift;
    offsetsL[i]=-offsets[i];
    offsetsR[i]=-offsets[i];
  }

  cBs->cd();
}


void Regularize(TProfile* h) {
  Double_t aveE=0;
  Int_t npts=1;
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
  // actually vs. L.s2.try -> position along the scint. paddles!
  // and fit to get the L-R time offsets, and collect
  // the slope

  //  TString t1="(L.s2.lt[L.s2.t_pads[0]]-L.s2.rt[L.s2.t_pads[0]]):B.ts.sh.y[0]";
  TString t1="(L.s2.lt[L.s2.t_pads[0]]-L.s2.rt[L.s2.t_pads[0]]):L.s2.try[0]";
  TString cut="L.s2.nthit==1";
  cut += "&&L.tr.n==1&&L.vdc.u1.nclust==1&&L.vdc.u2.nclust==1&&L.vdc.v1.nclust==1&&L.vdc.v2.nclust==1";
  cut += "&&((DBB.evtypebits>>3)&1)&&DBB.edtpl[0]==0&&L.prl1.e+L.prl2.e>600&&L.cer.asum_c>100";
  
  TString hist = "hdtvy(50,-.1,.1,1000,-1000.,1000.)";

  Double_t lrdiff[npaddles];
  Double_t cn[npaddles];

  
  for (int i=0; i<npaddles; i++) {
    TString cuts = cut+Form("&&L.s2.t_pads[0]==%d",i);
    
    cBs && cBs->cd(1);
    T->Draw(t1+">>"+hist,cuts);
    TH2F* h = (TH2F*)gDirectory->Get("hdtvy");
    Double_t avey = h->GetMean(2);
    T->Draw(t1+Form(">>hdtvy_%d(50,-.1,.1,100,%f,%f)",i,avey-200.,avey+200.),
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
    cBs->Print(Form("s2_paddles_fit2_%d.png",i));
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
// void time_beta(){
  
//   Double_t tmp,tmp2;
//   Double_t S2_YSize,S1_YSize;
//   Double_t S2_Z,S1_Z;
//   Double_t S1_oldloffset[6] = { 5.80,    3.99,   5.26,   3.93,  -12.74,   1.32,
// 				-11.69,  -19.11,  -2.40,  -3.93,  -22.22,  -2.59};
//   Double_t S2_oldroffset[16] = {  17.11, 18.96, 14.60, 16.15, 17.36, 10.99, 17.21, 12.79, 12.97, 17.74, 22.23, 20.19, 23.50, 22.70, 18.89, 20.38, 
// 				  -10.68, -9.68, -7.53, -9.91, -8.77, -10.87, -8.76, -9.29, -12.97, -10.12, -5.23, -10.75, -6.72, -7.91, -7.94, -9.43};
//   Double_t S1_newloffset[6];
//   Double_t S2_newroffset[16];
//   Double_t S1_oldspeed=1.7e+8,S1_newspeed=1.7e+8;
//   Double_t S2_oldspeed=1.7e+8,S2_newspeed=1.7e+8;
//   Double_t Tdc2T;
//   Double_t pathl;
  
//   S2_oldspeed=1.34281e+08; S2_newspeed=1.34281e+08;
//   S2_YSize=0.216; S2_Z=3.1415;
//   S1_YSize=0.180; S1_Z=1.2873;
//   Tdc2T=0.50e-9;
  
//   //assume S2_Z>S1_Z
//   pathl=S2_Z-S1_Z;
//   cerr << "Path = "<<pathl<<endl;
  
//   TString cut="L.s2.nthit==1";
//   cut += "&&L.tr.n==1&&L.vdc.u1.nclust==1&&L.vdc.u2.nclust==1&&L.vdc.v1.nclust==1&&L.vdc.v2.nclust==1";
//   cut += "&&((DBB.evtypebits>>3)&1)&&DBB.edtpl[0]==0&&L.prl1.e+L.prl2.e>1200&&L.cer.asum_c>500";
  
//   T->Draw("L.tr.beta>>hbeta(100,-1.0,1.0)",cut.Data());
   
//   tmp = hbeta->GetBinCenter(hbeta->GetMaximumBin());
  
//   Double_t newbeta=1;
  

//   T->Draw("L.s2.time-L.s1.time>>h2(100,-1.0,1.0)",cut.Data());
 
//   gGet_Leaf_Mean_and_RMS(T_Tree,Form("%s.time-%s.s1.time",leafbase.Data(),Arm.Data()),Form("abs(%s.time-%s.s1.time)<5e-9",leafbase.Data(),Arm.Data()),tmp,tmp2);
  
//   //tmp=MoreOffset
//   tmp=tmp*(newbeta-beta)/newbeta/S2_Tdc2T;
//   Printf("suggestion: MoreOffset=%.0f",tmp);
  
//   for ( i=0; i<NumOfPaddles; i++ )
//     {
//       if ( IsKeepOld )
// 	{
// 	  newltdc[i]=oldltdc[i];
// 	  newrtdc[i]=oldrtdc[i];
// 	}
//       offsetsL[i]+=MoreOffset;
//       offsetsL[i]+=MoreOffset;
//     }
  
// }

// void time_walk(){
//  /*TimeWalk{{{*/
//       cut=Form("%s.tr.n==1&&%s.nlthit==1&&%s.nrthit==1&&%s.edtpl==0&&%s.tr.beta>0",Arm.Data(),leafbase.Data(),leafbase.Data(),DECAT_Name.Data(),Arm.Data());
//       cut+=Form("&&%s.vdc.u1.nclust==1&&%s.vdc.u2.nclust==1&&%s.vdc.v1.nclust==1&&%s.vdc.v2.nclust==1",Arm.Data(),Arm.Data(),Arm.Data(),Arm.Data());
//       TString side,cuts;
//       TCanvas* c_Timewalk=new TCanvas("c_Timewalk","c_Timewalk");
//       c_Timewalk->Divide(2,(Int_t)NumOfPaddles/2+NumOfPaddles%2);
//       TH2F* h2_tw;
//       epsfilename=Form("%s/%d_%s_TimeWalk.png",BaseDir.Data(),RunNo,scin.Data());
//       for ( j=0; j<2; j++ )
// 	{
// 	  if ( j==0 )
// 	    side="l";
// 	  else
// 	    side="r";

// 	  for ( i=0; i<NumOfPaddles; i++ )
// 	    {
// 	      c_Timewalk->cd(i+1);
// 	      cuts=cut+Form("&&%s.%st_c[%d]*1e9>0&&1/sqrt(%s.%sa_c[%d])<=0.1",leafbase.Data(),side.Data(),i,leafbase.Data(),side.Data(),i);
// 	      //gPad->SetLogy(1);
// 	      hist=Form("%s.%st_c[%d]*1e9:1./sqrt(%s.%sa_c[%d])>>h2_tw_%s%d",leafbase.Data(),side.Data(),i,leafbase.Data(),side.Data(),i,side.Data(),i);
// 	      T_Tree->Draw(hist.Data(),cuts.Data());
// 	      h2_tw=(TH2F*)gROOT->FindObject(Form("h2_tw_%s%d",side.Data(),i));
// 	      h2_tw->SetTitle(Form("%d %s.%s TDC_C[%d] (ns) vs 1/#sqrt{ADC_C[%d]}",RunNo,leafbase.Data(),side.Data(),i,i));
// 	      h2_tw->GetXaxis()->SetTitle(Form("1/#sqrt{ADC_C[%d]}",i));
// 	      h2_tw->GetXaxis()->CenterTitle();
// 	      h2_tw->GetYaxis()->SetTitle(Form("TDC_C[%d] (ns)",i));
// 	      h2_tw->GetYaxis()->CenterTitle();
// 	      h2_tw->SetMarkerStyle(20);
// 	      h2_tw->Fit("pol1","Q");
// 	      if ( j==0 )
// 		newltw.push_back(h2_tw->GetFunction("pol1")->GetParameter("p1"));
// 	      else
// 		newrtw.push_back(h2_tw->GetFunction("pol1")->GetParameter("p1"));
// 	    }
// 	  if ( j==0 )
// 	    c_Timewalk->Print(Form("%s(",epsfilename.Data()));
// 	  else
// 	    c_Timewalk->Print(Form("%s)",epsfilename.Data()));
// 	  c_Timewalk->Update();
// 	}
//       delete c_Timewalk;
//       /*}}}*/

// }


void time_s2(TTree *T=0) {
  TProfile::Approximate();
  for (int i=0; i<npaddles; i++) {
    offsetsL[i]=offsetsR[i]=0.;
  }
  time_paddles(T);
  time_lr(T);
  writeOffsets(0.0);
}

void time_s2m_paddles()
{
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TString filename;
  filename = Form("/work/halla/e08014/disk1/Rootfiles/e08014_L_%d.root",run_number);
 
  // filename = Form("/w/cache/mss/home/yez/root_0818/e08014_L_%d.root",run_number);
  TChain *T = new TChain("T");
    T->Add(filename);
    //  filename = Form("/w/cache/mss/home/yez/root_0818/e08014_L_%d_1.root",run_number);
  // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4014.root");
  // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4015.root");
  // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4016.root");
  // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4017.root");

  // // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4267.root");
  // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4268.root");
  // T->Add("/w/cache/mss/home/yez/root_0818/e08014_L_4269.root");



  time_s2(T);
}
