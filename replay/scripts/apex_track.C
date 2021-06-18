// -------------------
// plot the rawtime vs wire number per VDC plane per event for LHRS
// shujie, 2019.3.7
// -------------------

#include "Load_more_rootfiles.C"

#define NPLANE 4
#define MAX_ENTRIES 100000
#define MAX_TRACKS 20
#define MAX_HIT 1000



void apex_track(Int_t runnum, Int_t entry=1){


  const char *arm = "L";

  // S2 params
  const Int_t MaxS2Hit = 15;
  const Int_t NS2Pad = 16;

  

  // VDC params
  const char plane[NPLANE][8] = {"u1", "v1", "u2", "v2"};

  const Int_t NWires = 368;
  const Double_t WSep = 4.2426e-3; // wire seperation in VDC
  Double_t PlaneWidth = NWires*WSep;
  
  const Double_t PlaneSep = 0.3348; // LHRS old = 0.3348 // 0.3327 (old RHRS) 

  const Double_t LPlaneS = 0.77852; // lower plane starting pos
  const Double_t UPlaneS =  1.02718; // upper plane starting pos
  Double_t PlaneDiff = UPlaneS - LPlaneS;

  Double_t PlaneSize = (NWires-1)*WSep; // size of plane
  
  Double_t Stretch = 1.0; // factor to 'stretch' lines representing slopes
  
  gStyle->SetTitleYSize(0.06);
  gStyle->SetTitleXSize(0.06);
  gStyle->SetLabelSize(0.06,"X");
  gStyle->SetLabelSize(0.06,"Y");
  gStyle->SetPaperSize(18,22);
  gStyle->SetOptStat(0);


  TChain *t = Load_more_rootfile(runnum);

  
  //Loads all them splits
 
  //  TString ROOTFILE_DIR =   "/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/apex_root/Rootfiles/apex_%d.root";
  Int_t nn=t->GetEntries();


  TCanvas *c1  = new TCanvas("c1","rawtime : wire number",1200,800);

  Double_t WHratio  = (1.2*PlaneSep+0.2*PlaneSep)/(PlaneWidth+PlaneDiff); // ratio of width to height of VDC chamber plot
  
  TCanvas *c2  = new TCanvas("c2","U perp : U/V",1400,1400*2*WHratio);
  c2->Divide(1,2);

  //  TCanvas *c3  = new TCanvas("c3","V perp : U/V",1400,1400*WHratio);

  TCanvas *c4 = new TCanvas("c4","Timing offsets",800,800);


  TCanvas *c5 = new TCanvas("c5","S2 timing coincidence (between arms)",1400,1400*2*WHratio);
  c5->Divide(1,2);


  TH2F* hraw[NPLANE];


  Int_t nbins = (PlaneWidth+PlaneDiff)/WSep;

  TH2F* hUtime = new TH2F("hUtime","UV perp  : U",nbins,0,PlaneWidth+PlaneDiff,200,-0.2*PlaneSep,1.2*PlaneSep);
  TH2F* hVtime = new TH2F("hVtime","UV perp  : V",nbins,0,PlaneWidth+PlaneDiff,200,-0.2*PlaneSep,1.2*PlaneSep);

  TH2F* hUVtime[2] = {hUtime,hVtime}; // used to select U or V histogram
  

  for(Int_t i = 0; i < NPLANE; i++ ){
    hraw[i]  = new TH2F(Form("hraw_%d",i),Form("%s rawtime : wire number",plane[i]),330,20,350,200,2000,3200);
  }
  


  
  // format lines to represent planes
  TLine*  LLow = new TLine(0,0,PlaneWidth,0);
  LLow->SetLineColor(kRed);

  TLine*  LUp = new TLine(PlaneDiff,PlaneSep,PlaneDiff+PlaneWidth,PlaneSep);
  LUp->SetLineColor(kRed);



  // vectors to hold positio of hit in cluster and status of pre/post pivot
  std::vector<Int_t> iClustV;
  std::vector<Bool_t> iPivV;
  std::vector<Bool_t> iAddV; // keeps track of whether cluster slopes have already been added

  
  // lines to represent local slopes of clusters

  std::vector<TLine*> LlSlope_v[2]; // vector of local slope lines
  std::vector<TLine*> UlSlope_v[2]; // vector of local slope lines

  std::vector<TLine*> LgSlope_v[2]; // vector of local slope lines
  std::vector<TLine*> UgSlope_v[2]; // vector of local slope lines


  // vector to hold timing offsets of clusters
  std::vector<TH2F*> hToffV;
  std::vector<Double_t> t0MeanV;
  // colours for timing histograms
  Int_t TColours[] = {kGreen+2,kMagenta,kBlue,kRed,kCyan,kOrange,kYellow,kViolet};

  // format plot of S2 timing

  Int_t coinc_bins = 300;
  Double_t coinc_start = 0;
  Double_t coinc_end = 200;

  TH1F *hCoinc = new TH1F("hCoinc"," S2-Time difference",coinc_bins,coinc_start,coinc_end);
  hCoinc->GetXaxis()->SetTitle("Coinc time (ns)");

  TH1F *hS2Hits = new TH1F("hS2Hits","S2 paddle hits (LHRS)",2*NS2Pad,0,NS2Pad);
  hS2Hits->GetXaxis()->SetTitle("Paddle number");

    // S2 cut

  Double_t CutCen = 163.5;
  Double_t CutWidth = 7.5;
  
  Double_t TLowCut = 156.0;
  Double_t THighCut = 171.0;

  TLine* TLowLine = new TLine(TLowCut,0,TLowCut,1.0);
  TLowLine->SetLineColor(kMagenta);
  TLowLine->SetLineStyle(kDashed);

  TLine* THighLine = new TLine(THighCut,0,THighCut,1.0);
  THighLine->SetLineColor(kMagenta);
  THighLine->SetLineStyle(kDashed);

  TLegend *leg_LtoffCut =  new TLegend(.1,.65,.45,.9,"Key");
  leg_LtoffCut->SetFillColor(0);
  leg_LtoffCut->SetTextSize(0.025);

  leg_LtoffCut->AddEntry(TLowLine,"Cut for coincidence peak","l");
  
  
  c1->Divide(2,2);
  


  //--------------------------------
  t->SetBranchStatus("*"       ,0);
  t->SetBranchStatus("L.vdc.*" ,1);
  t->SetBranchStatus("L.tr.n"  ,1);
 
  
  // cluster slope and intercept from analyzer
  Double_t intercept[NPLANE][MAX_HIT]; 

  Double_t clsiz[NPLANE][MAX_HIT];
  Double_t clpivot[NPLANE][MAX_HIT];
  Double_t clbeg[NPLANE][MAX_HIT];
  Double_t clend[NPLANE][MAX_HIT];
  Double_t clsnum[NPLANE][MAX_HIT];
  Double_t trknum[NPLANE][MAX_HIT];
  Double_t t0[NPLANE][MAX_HIT];

  Double_t trknum_H[NPLANE][MAX_HIT];
  
  Double_t trknum_Tr[MAX_TRACKS];
  Double_t t0_Tr[MAX_TRACKS];
  

  Double_t slope[NPLANE][MAX_HIT];
  Double_t lslope[NPLANE][MAX_HIT];  

  

  Double_t hittrdist[NPLANE][MAX_HIT], hitltrdist[NPLANE][MAX_HIT], hitdist[NPLANE][MAX_HIT];
  
  Double_t wireno[NPLANE][MAX_HIT];

  Double_t hittime[NPLANE][MAX_HIT];
  Double_t rhittime[NPLANE][MAX_HIT];
  
  Double_t nclust[NPLANE];
  
  Double_t nhit[NPLANE], ntrk;

  Double_t trclustU1[MAX_TRACKS];
  Double_t trclustV1[MAX_TRACKS];
  Double_t trclustU2[MAX_TRACKS];
  Double_t trclustV2[MAX_TRACKS];

  Double_t t0_av[MAX_TRACKS];

  // S2 timing
  Double_t L_s2_t_pads[MaxS2Hit];
  Double_t L_s2_nthit;
  Double_t L_s2_lt_c[NS2Pad],L_s2_rt_c[NS2Pad];

  Double_t R_s2_t_pads[MaxS2Hit];
  Double_t R_s2_nthit;
  Double_t R_s2_lt_c[NS2Pad],R_s2_rt_c[NS2Pad];
  
  
  
  t->SetBranchAddress("L.tr.n",&ntrk);

  t->SetBranchStatus("L.tr.trknum",kTRUE);
  t->SetBranchAddress("L.tr.trknum",&trknum_Tr);

  t->SetBranchStatus("L.tr.toff",kTRUE);
  t->SetBranchAddress("L.tr.toff",&t0_Tr);

  t->SetBranchStatus("L.tr.U1clsnum",kTRUE);
  t->SetBranchAddress("L.tr.U1clsnum",trclustU1);

  t->SetBranchStatus("L.tr.V1clsnum",kTRUE);
  t->SetBranchAddress("L.tr.V1clsnum",trclustV1);

  t->SetBranchStatus("L.tr.U2clsnum",kTRUE);
  t->SetBranchAddress("L.tr.U2clsnum",trclustU2);

  t->SetBranchStatus("L.tr.V2clsnum",kTRUE);
  t->SetBranchAddress("L.tr.V2clsnum",trclustV2);

  t->SetBranchStatus("L.tr.toff",kTRUE);
  t->SetBranchAddress("L.tr.toff",t0_av);

  t->SetBranchStatus("L.s2.nthit",kTRUE);
  t->SetBranchAddress("L.s2.nthit",&L_s2_nthit);
  
  t->SetBranchStatus("L.s2.t_pads",kTRUE);
  t->SetBranchAddress("L.s2.t_pads",L_s2_t_pads);

  t->SetBranchStatus("L.s2.lt_c",kTRUE);
  t->SetBranchAddress("L.s2.lt_c",L_s2_lt_c);
    
  t->SetBranchStatus("L.s2.rt_c",kTRUE);
  t->SetBranchAddress("L.s2.rt_c",L_s2_rt_c);

  
  t->SetBranchStatus("R.s2.nthit",kTRUE);
  t->SetBranchAddress("R.s2.nthit",&R_s2_nthit);
  
  t->SetBranchStatus("R.s2.t_pads",kTRUE);
  t->SetBranchAddress("R.s2.t_pads",R_s2_t_pads);

  t->SetBranchStatus("R.s2.lt_c",kTRUE);
  t->SetBranchAddress("R.s2.lt_c",R_s2_lt_c);
    
  t->SetBranchStatus("R.s2.rt_c",kTRUE);
  t->SetBranchAddress("R.s2.rt_c",R_s2_rt_c);
 
  
  for(Int_t i = 0; i < NPLANE; i++ ){      
    
    t->SetBranchStatus(Form("%s.vdc.%s.clbeg", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.clbeg", arm, plane[i]), clbeg[i]);
    
    t->SetBranchStatus(Form("%s.vdc.%s.clend", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.clend", arm, plane[i]), clend[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.clpivot", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.clpivot", arm, plane[i]), clpivot[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.clsnum", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.clsnum", arm, plane[i]), clsnum[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.cltrknum", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.cltrknum", arm, plane[i]), trknum[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.trknum", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.trknum", arm, plane[i]), trknum_H[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.t0", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.t0", arm, plane[i]), t0[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.slope", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.slope", arm, plane[i]), slope[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.lslope", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.lslope", arm, plane[i]), lslope[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.clpos", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.clpos", arm, plane[i]), intercept[i]);
        
    t->SetBranchStatus(Form("%s.vdc.%s.nhit", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.nhit", arm, plane[i]), &nhit[i]);
    
    t->SetBranchStatus(Form("%s.vdc.%s.nclust", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.nclust", arm, plane[i]), &nclust[i]);


    t->SetBranchStatus(Form("%s.vdc.%s.time", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.time", arm, plane[i]), hittime[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.rawtime", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.rawtime", arm, plane[i]), rhittime[i]);
    
    t->SetBranchStatus(Form("%s.vdc.%s.wire", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.wire", arm, plane[i]), wireno[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.trdist", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.trdist", arm, plane[i]), hittrdist[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.ltrdist", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.ltrdist", arm, plane[i]), hitltrdist[i]);

    t->SetBranchStatus(Form("%s.vdc.%s.dist", arm, plane[i]), kTRUE);
    t->SetBranchAddress(Form("%s.vdc.%s.dist", arm, plane[i]), hitdist[i]);

  }
  
  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextAlign(11);


  TLine *l1 = new TLine(20, 2850, 350, 2850);
  l1->SetLineColor(2);

  
  while(entry>0){
    
    t->GetEntry(entry);

    
    cout<<"\n--now plot VDC rawtime vs wire number from event "<<entry<<endl;


    cout << "track t0s: " << endl;
    for(Int_t i = 0; i<ntrk; i++){
      cout << "tnum[" << i << "] = " << (Int_t)trknum_Tr[i] << ", t0[" << i << "] = " << t0_Tr[i] << endl;
      cout << "Clustnums: U1 = " << trclustU1[i] << ", V1 = " << trclustV1[i] << ", U2 = " << trclustU2[i] << ", V2 = " << trclustV2[i] << endl;
    }
    
    
    // reset t0 histograms
    hToffV.clear();
    t0MeanV.clear();

    for(Int_t i = 0; i<ntrk+1; i++){

      if(i==0){
	// case of clusters with no associated track
	hToffV.push_back(new TH2F(Form("hToffV_%i",i),Form("hToffV_%i",i),200,-200,200,4,0,4));
	hToffV[i]->SetLineColor(kBlack);
	hToffV[i]->SetMarkerColor(kBlack);
	//	hToffV[i]->SetMarkerSize(5);	
      }
      else{
	// case of clusters with associated track
	hToffV.push_back(new TH2F(Form("hToffV_%i",i),Form("hToffV_%i",i),200,-200,200,4,0,4));
	hToffV[i]->SetLineColor(TColours[i-1]);
	hToffV[i]->SetMarkerColor(TColours[i-1]);

	cout << "t0 for track " << i << " = " << t0_av[i-1] << endl;
	t0MeanV.push_back(t0_av[i-1]);
      }
      
      for(Int_t j = 0; j<hToffV[i]->GetYaxis()->GetNbins(); j++){      
	hToffV[i]->GetYaxis()->SetBinLabel(j+1,plane[j]);
      }
    }
    

    hUtime->Reset("ICESM");
    hVtime->Reset("ICESM");


    for(Int_t i = 0; i < 2; i++){
      // clear vectiors holding slope lines
	LlSlope_v[i].clear();
	UlSlope_v[i].clear();

	LgSlope_v[i].clear();
	UgSlope_v[i].clear();
    }

    // S2 histogram filling

    for(Int_t j = 0; j<L_s2_nthit; j++){

      cout << "L S2 hit pad = " << L_s2_t_pads[j] << endl;
      hS2Hits->Fill((Int_t)L_s2_t_pads[j]);
      
      for(Int_t k = 0; k<R_s2_nthit; k++){
	
	hCoinc->Fill( ( (L_s2_lt_c[(Int_t)L_s2_t_pads[j]]+L_s2_rt_c[(Int_t)L_s2_t_pads[j]])/2 - (R_s2_lt_c[(Int_t)R_s2_t_pads[k]]+R_s2_rt_c[(Int_t)R_s2_t_pads[k]])/2 )*1e9);	
	
      }      
    }

    

    
    for(Int_t i = 0; i < NPLANE; i++){

      // vectors to hold cluster position and pre/post cluster status for each cluster

      iClustV.clear();
      iPivV.clear();
      iAddV.clear();
      for(Int_t j= 0; j<nclust[i]; j++){
	iClustV.push_back(0);
	iPivV.push_back(false);
	iAddV.push_back(false);
      }
      
     
      Int_t iClust = 0; // number of hit in cluster (ie 1st hit in cluster)
      Double_t DistSign = -1; // controls whether disttance from plane is positive or negative

      Bool_t Piv = false; // pivot condition 

            
      // fill distance plot

      
      cout << "Plane " << plane[i] << " : " << endl;
      
      
      for(Int_t ii = 0; ii<nhit[i]; ii++, iClust++ ){


	Int_t ClsNum = clsnum[i][ii];
	Int_t ClsPiv = clpivot[i][ClsNum] - clbeg[i][ClsNum];

	if(ClsNum < 0){
	  cout << endl;
	  cout << "ClsNum = " << ClsNum << "(duff)" << endl;
	  cout << "Wireno = " << wireno[i][ii] << ", TrKnum = " << trknum[i][ClsNum] << ", TrkNum (Hit) = " << trknum_H[i][ii] << endl;
	  continue;
	}

      

	if(ClsNum >= 0){

	  if(iClustV[ClsNum] < ClsPiv){
	    DistSign = 1;
	  }
	  else if(iClustV[ClsNum] == ClsPiv){ // pivot
	    DistSign = -1;
	    if(!iPivV[ClsNum]){
	      cout << "Pivot condition (negative)" << endl;
	      iPivV[ClsNum] = true;

	    }
	    else if(iPivV[ClsNum]){
	      cout << "Pivot condition (positive)" << endl;
	      DistSign = 1;
	      iPivV[ClsNum] = false;
	    }	  
	  }
	  else{
	    //cout << "Post-piv reality" << endl;
	    DistSign = -1;
	  }
	  cout << " iClustV = " << iClustV[ClsNum] << endl;
	  iClustV[ClsNum]++;
	}

      
	
	Int_t TrkNum = trknum[i][ClsNum];

	
	Double_t ClSlope = 1./lslope[i][ClsNum];
	Double_t ClInvSlope = lslope[i][ClsNum];

	Double_t CgSlope = 1./slope[i][ClsNum];
	Double_t CgInvSlope = slope[i][ClsNum];
	
	//	Double_t CInt = -intercept[i][ClsNum]/(lslope[i][ClsNum]);
	Double_t CInt = -intercept[i][ClsNum];

	Double_t Ct0 = (t0[i][ClsNum])*1e9;


	if(ii==0){

	  
	  // fill in t0
	  
	  cout << "start of plane, ClsNum = " << ClsNum << ", ClsPiv = " << ClsPiv << " (" << clpivot[i][ClsNum] << ")" <<  endl;
	  cout << "t0 = " << Ct0 << ", (i+0.5 = " << (Double_t)i+0.5 << ")" << endl;

	  if(ClsNum != -3){
	    hToffV[TrkNum]->Fill(Ct0,i+0.5);
	  }	  


	  
	  if(!iAddV[ClsNum]){
	    if(i<2){

	      CInt += LPlaneS;
	    
	      LlSlope_v[i].push_back(new TLine(PlaneSize - CInt - Stretch*ClInvSlope*0.1,ClSlope*(ClInvSlope*-0.1),PlaneSize - CInt + Stretch*ClInvSlope*PlaneSep, ClSlope*(Stretch*ClInvSlope*PlaneSep)));

	      if(TrkNum == 1){ // global track
		LlSlope_v[i][0]->SetLineColor(kGreen+2);
	      }
	    
	      LgSlope_v[i].push_back(new TLine(PlaneSize - CInt - Stretch*CgInvSlope*0.1,CgSlope*(CgInvSlope*-0.1),PlaneSize - CInt + Stretch*CgInvSlope*PlaneSep, CgSlope*(Stretch*CgInvSlope*PlaneSep)));

	    }
	    else if(i>1){

	      CInt += UPlaneS;

	      UlSlope_v[i%2].push_back(new TLine(PlaneDiff + PlaneSize - CInt - Stretch*ClInvSlope*PlaneSep, PlaneSep - ClSlope*(Stretch*ClInvSlope*PlaneSep) , PlaneDiff + PlaneSize - CInt + Stretch*ClInvSlope*0.1, PlaneSep + ClSlope*(Stretch*ClInvSlope*0.1)));

	      if(TrkNum == 1){ // global track
		UlSlope_v[i%2][0]->SetLineColor(kGreen+2);
	      }
	    
	      UgSlope_v[i%2].push_back(new TLine(PlaneDiff + PlaneSize -CInt - Stretch*CgInvSlope*PlaneSep, PlaneSep - CgSlope*(Stretch*CgInvSlope*PlaneSep) , PlaneDiff + PlaneSize -CInt + Stretch*CgInvSlope*0.1, PlaneSep + CgSlope*(Stretch*CgInvSlope*0.1)));
	    
	    }
	  
	  }
	}

	// if(iAddV[ClsNum]){
	//   //	    continue;
	// }
	// else if (!iAddV[ClsNum]){
	//   iAddV[ClsNum] = true;
	// }
      

	
	
	if(ii>0 && clsnum[i][ii] != clsnum[i][ii-1]){
	  iClust = 0;
	  cout << endl << endl;
	  cout << "New ClsNum: " << ClsNum << ", ClsPiv = " << ClsPiv << " (" << clpivot[i][ClsNum] << "), iAddV = " << iAddV[ClsNum] <<  endl;


	  // fill in t0 
	  cout << "t0 = " << Ct0 << ", (i+0.5 = " << (Double_t)i+0.5 << ")" << endl;

	  if(ClsNum != -3 || ClsNum != -1){
	    hToffV[TrkNum]->Fill(Ct0,i+0.5);
	  }

	  // if(iAddV[ClsNum]){
	  //   //	    continue;
	  // }
	  // else if (!iAddV[ClsNum]){
	  //   iAddV[ClsNum] = true;
	  // }

     
	  
	  if(!iAddV[ClsNum]){
	    if(i<2){

	      CInt += LPlaneS;
	    	  
	      LlSlope_v[i].push_back(new TLine(PlaneSize - CInt - Stretch*ClInvSlope*0.1,ClSlope*(ClInvSlope*-0.1),PlaneSize - CInt + Stretch*ClInvSlope*PlaneSep, ClSlope*(Stretch*ClInvSlope*PlaneSep)));

	      if(TrkNum == 1){ // global track
		//	      LlSlope_v[i][]->SetLineColor(kGreen+2);
		LlSlope_v[i].back()->SetLineColor(kGreen+2);
	      }

	      LgSlope_v[i].push_back(new TLine(PlaneSize - CInt - Stretch*CgInvSlope*0.1,CgSlope*(CgInvSlope*-0.1),PlaneSize - CInt + Stretch*CgInvSlope*PlaneSep, CgSlope*(Stretch*CgInvSlope*PlaneSep)));
	    

	    }
	    else if(i>1){

	      CInt += UPlaneS;

	      UlSlope_v[i%2].push_back(new TLine(PlaneDiff + PlaneSize -CInt - Stretch*ClInvSlope*PlaneSep, PlaneSep - ClSlope*(Stretch*ClInvSlope*PlaneSep) , PlaneDiff + PlaneSize -CInt + Stretch*ClInvSlope*0.1, PlaneSep + ClSlope*(Stretch*ClInvSlope*0.1)));
	    
	      cout << "TrkNum = " << TrkNum << endl;
	      if(TrkNum == 1){ // global track
		UlSlope_v[i%2][ClsNum]->SetLineColor(kGreen+2);
	      }

	      UgSlope_v[i%2].push_back(new TLine(PlaneDiff + PlaneSize -CInt - Stretch*CgInvSlope*PlaneSep, PlaneSep - CgSlope*(Stretch*CgInvSlope*PlaneSep) , PlaneDiff + PlaneSize -CInt + Stretch*CgInvSlope*0.1, PlaneSep + CgSlope*(Stretch*CgInvSlope*0.1)));

	    }
	  
	  
	  }
	}


	if(iAddV[ClsNum]){
	  //	    continue;
	}
	else if (!iAddV[ClsNum]){
	  iAddV[ClsNum] = true;
	}
	
	if(i<2){
	  hUVtime[i]->Fill(PlaneSize - WSep*wireno[i][ii],DistSign*hitdist[i][ii]);
	}	
	else if(i>1){       
	  hUVtime[i%2]->Fill(PlaneSize + PlaneDiff - WSep*wireno[i][ii],PlaneSep + DistSign*hitdist[i][ii]);
	}


	
      	cout << "Wireno = " << wireno[i][ii] << ", TrKnum = " << TrkNum << ", TrkNum (Hit) = " << trknum_H[i][ii] << ", (rawtime = " << rhittime[i][ii] << ", DistSign = " << DistSign << "), iClustV = " << iClustV[ClsNum] << endl;


	if(iClustV[ClsNum] == ClsPiv+1){ // pivot
	  if(iPivV[ClsNum]){
	    iClustV[ClsNum]--;
	    ii--;
	  }	  
	}
      
      }

      cout << endl << endl;
    
      
      c1->cd(i+1);

      
      hraw[i]->Reset("ICESM");
      for(Int_t ii = 0; ii<nhit[i]; ii++){
	hraw[i]->Fill(wireno[i][ii],rhittime[i][ii]);
      }
      hraw[i]->Draw("box");

      if(i == 0){
      tex.SetTextColor(kRed);
      tex.DrawLatexNDC(0.12,0.85,Form("Run %d, event #%d",runnum,entry));
      tex.DrawLatexNDC(0.12,0.80,Form("%g track, %g clusters found:",ntrk, nclust[i]));
      tex.SetTextColor(kGreen);	
      }

      for(int ii=0;ii<nclust[i];ii++){
	tex.DrawLatexNDC(0.12,0.75-ii*0.05,Form("cluster %d: start wire %g, end wire %g",ii+1, clbeg[i][ii],clend[i][ii]));
      }
      
      l1->Draw("Same");
      
      
    }


    c2->cd(1);
    
    hUtime->Draw("box");
    LLow->Draw("same");
    LUp->Draw("same");

    for (auto & element : LlSlope_v[0]){
      
      if(element->GetLineColor()==kGreen+2){
	// global slope, keep green
      }
      else{      
	element->SetLineColor(kMagenta);
      }
      element->Draw("same");
    }
    

    for (auto & element : LgSlope_v[0]){
      

      element->SetLineColor(kBlack);
      element->SetLineStyle(kDotted);
      element->Draw("same");
    }
    
    
    for (auto & element : UlSlope_v[0]) {

      if(element->GetLineColor()==kGreen+2){
	// global slope, keep green
      }
      else{      
	element->SetLineColor(kMagenta);
      }
      element->Draw("same");
    }

    for (auto & element : UgSlope_v[0]){
      

      element->SetLineColor(kBlack);
      element->SetLineStyle(kDotted);
      element->Draw("same");
    }

  


    c2->cd(2);
   
    hVtime->Draw("box");
    LLow->Draw("same");
    LUp->Draw("same");


    for (auto & element : LlSlope_v[1]) {
     
      if(element->GetLineColor()==kGreen+2){
	// global slope, keep green
      }
      else{      
	element->SetLineColor(kMagenta);
      }
      element->Draw("same");
    }
    
    for (auto & element : LgSlope_v[1]){      

      element->SetLineColor(kBlack);
      element->SetLineStyle(kDotted);
      element->Draw("same");
    }


    
    
    for (auto & element : UlSlope_v[1]) {

      if(element->GetLineColor()==kGreen+2){
	// global slope, keep green
      }
      else{      
	element->SetLineColor(kMagenta);
      }
      element->Draw("same");    
    }


    for (auto & element : UgSlope_v[1]){      

      element->SetLineColor(kBlack);
      element->SetLineStyle(kDotted);
      element->Draw("same");
    }


    c4->cd(0);

    THStack* hT0Stack = new THStack("hT0Stack","LHRS Timing offsets ; Timing offset (ns)");

    TLegend *t0_leg =  new TLegend(.65,.65,.9,.9,"Key");

    Int_t t0It = 0;
    
    for (auto & element : hToffV){
      hT0Stack->Add(element);
      if(t0It == 1){
	cout << "Global track th2 entries = " << element->GetEntries() << endl;
      }
      
      if(t0It == 0){
	t0_leg->AddEntry(element,"(no track)","l");
      }
      else{
	
       
	t0_leg->AddEntry(element,Form("track %i (|t|_{av} = %.2f ns)",t0It,t0MeanV[t0It-1]*1e9) ,"l");
      }
	
      t0It++;
    }
   
    
    hT0Stack->Draw("box nostack");

    
    for(Int_t i = 0; i<hT0Stack->GetYaxis()->GetNbins(); i++){      
      hT0Stack->GetYaxis()->SetBinLabel(i+1,plane[i]);
    }

    hT0Stack->Draw("box");
    t0_leg->Draw();


    c5->cd(1);
    hCoinc->Draw();
    TLowLine->Draw("same");
    THighLine->Draw("same");
    leg_LtoffCut->Draw();

    c5->cd(2);
    hS2Hits->Draw();

    int index;
    c2->Update();
    c4->Update();
    c5->Update();
    //    c3->Update();
    c1->Update();
    cout << "\n--Which event you want to check next ?\n(Please enter an event number. ENTER = next event, -1 to exit) ";
    if(cin.peek() == '\n'){
      cin.ignore();
      entry++;
    }
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
