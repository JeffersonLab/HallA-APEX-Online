//======================================
// DeadTime Calculation for Tritium run
// Sheren Alsalmi &  Dien Nguyen
// Nov, 8, 2017 
//======================================


void deadtimeL(){


    Int_t irun;
       const TString rootfilePath = "/chafs1/work1/tritium/Rootfiles/";
    int PS[8];
    char *rate = new char[500];
    char *clkrate = new char[50];
    char hname[10][50];
    char *h = new char[50];
    Double_t LT[10], DT[10];
    int icount[10];
    int daqcount[10];
    TH1F *his[10];
    TTree *tree;

    cout << "\nreplay: Please enter a Run Number (-1 to exit):";
    cin >> irun;
    if(irun==-1) return;

  
    TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),irun),"read");
    tree = (TTree*)file->Get("T");
    if(file->IsZombie()){
       	cout<<" this rootfile doest not exist: "<<endl;
	cout<<"Please try again with a new run. "<<endl;
	return;
  }

  THaRun *aRun = (THaRun*)file->Get("Run_Data");
  TArrayI ps = aRun->GetParameters()->GetPrescales();

  for (int k=0; k<8; k++){
    PS[k]= ps[k];
    cout<< " Check the prescaler: "<< PS[k]<<endl;
  }


  
    for (int i=1; i<4; i++){
        TCut t_cut = Form("DL.evtypebits&(1<<%i)",i);
        sprintf(rate,"LeftT%i", i);
        icount[i] = tree->GetMaximum(rate);
        sprintf(hname[i],"t%i",i);
        sprintf(h,"DL.evtypebits>>%s", hname[i]);
        his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
        tree->Draw(h,t_cut, "same");
        daqcount[i] = his[i]->GetEntries();
         if(icount[i]>0){
        LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
        DT[i] = 100. - LT[i];
    }}

  // Clock DeadTime :
    //================
       TCut t8_cut = Form("DL.evtypebits&(1<<8)");
        sprintf(clkrate,"LeftLclock");     
        icount[8] = tree->GetMaximum(clkrate);
        sprintf(hname[8],"t8");
        sprintf(h,"DL.evtypebits>>%s", hname[8]);
        his[8] =new TH1F (hname[8], hname[8], 100,0,1000000);
        tree->Draw(h,t8_cut, "same");
        daqcount[8] = his[8]->GetEntries();
           if(icount[8]>0){
        LT[8] = 100.*10.*daqcount[8]/icount[8];
        DT[8] = 100. - LT[8]; }

    cout<<"Run # "<<"   "<<irun<<endl; 
    cout<<"DT[T1]"<<"   "<<setprecision (3)<<DT[1]<<"  %"<<endl;
    cout<<"DT[T2]"<<"   "<<DT[2]<<"  %"<<endl;    
    cout<<"DT[T3]"<<"   "<<DT[3]<<"  %"<<endl;
    cout<<"DT[Clock]"<<"  "<<DT[8]<<"  %"<<endl;




}
