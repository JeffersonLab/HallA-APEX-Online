//======================================
// DeadTime Calculation for Tritium run
// Sheren Alsalmi &  Dien Nguyen
// Nov, 8, 2017 
//======================================


void deadtime(){


    Int_t irun;
       const TString rootfilePath = "/chafs1/work1/tritium/Rootfiles/";
    int PS[8];
    char *rate = new char[50];
    char hname[8][50];
    char *h = new char[50];
    Double_t LT[8], DT[8];
    int icount[8];
    int daqcount[8];
    TH1F *his[8];
    TBranch *br[9];
    
    cout << "\nreplay: Please enter a Run Number (-1 to exit):";
    cin >> irun;
    if(irun==-1) return;

    TChain *T =new TChain("T");
    TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),irun),"read");
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

      
    for (int i=0; i<8; i++){
            TCut t_cut = Form("DL.evtypebits&(1<<%i)",i+1);
        sprintf(rate,"l_raw_T%i",i+1);
        icount[i] = T->GetMaximum(rate);
        sprintf(hname[i],"t%i",i+1);
        sprintf(h,"DL.evtypebits>>%s", hname[i]);
        his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
        T->Draw(h,t_cut, "same");
        daqcount[i] = his[i]->GetEntries();
           if(icount[i]>0){
        LT[i] = 100.*PS[i]*daqcount[i]/icount[i];
        DT[i] = 100. - LT[i];
      
        cout<<LT[i]<<"   "<<DT[i]<<endl;
    }}

    cout<<"Run # "<<"   "<<irun<<endl; 
    cout<<"DT[T1]"<<"   "<<setprecision (3)<<DT[0]<<"  %"<<endl;
    cout<<"DT[T2]"<<"   "<<DT[1]<<"  %"<<endl;    
    cout<<"DT[T3]"<<"   "<<DT[2]<<"  %"<<endl;
    cout<<"DT[T4]"<<"   "<<DT[3]<<"  %"<<endl;
    cout<<"DT[T5]"<<"   "<<DT[4]<<"  %"<<endl;
    cout<<"DT[T6]"<<"   "<<DT[5]<<"  %"<<endl;
    cout<<"DT[T7]"<<"   "<<DT[6]<<"  %"<<endl;
    cout<<"DT[T8]"<<"   "<<DT[7]<<"  %"<<endl;




}
