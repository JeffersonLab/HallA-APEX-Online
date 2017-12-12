
#include <iostream>
#include <sstream> 
#include <fstream>
#include <iomanip>
#include <TSystem.h> 
#include <TString.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1F.h>
#include <TChain.h>
#include <TTree.h>
#include <TF1.h>
#include <THaRun.h>

void DT_calculation(){


  int PS[8];  //Prescaler
  int run;
  const TString rootfilePath = "/chafs1/work1/tritium/Rootfiles/";
  double NL_DAQ, NL_sent,NL_DAQ_beam;
  double LT_total, LT_beam;
  double right_t3c;
  double clkcount;
  double evtypebits;

  //runnumber
  
  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  cin >> run; 

  if(run==-1) return; 
  TChain *T = new TChain("T");
  std::ostringstream str;
  str << rootfilePath<<"tritium_"<<run;
  TString basename = str.str().c_str();
  TString rootfile = basename + ".root";
  TFile *file = new TFile(rootfile,"r");
  if(file->IsZombie()){
    cout<<" this rootfile doest not exist: "<<endl;
    return;
  }
    
 //====adding splits rootfiles =======================//
  
  Long_t i=0;
  while ( !gSystem->AccessPathName(rootfile.Data()) ) {
     T->Add(rootfile.Data());
     cout << "ROOT file " << rootfile << " added to TChain." << endl;
     i++;
     rootfile = basename + "_" + i + ".root";
   }

 //==finish adding splits rootfiles=====================//

   THaRun *aRun = (THaRun*)file->Get("Run_Data");
   TArrayI ps = aRun->GetParameters()->GetPrescales();

   for (int k=0; k<8; k++){
     PS[k]= ps[k];
     cout<< " Check the prescaler " << Form("PS[%i]",k+1)<<": "<< PS[k]<<endl;   
   }


  //=======Accesing only the branches we need
  //========Using T3 as main trigger for the Argon runs: (S0&&S2) && (GC||PR) [LEFT]===============

  Double_t Tr[4];	
  T->SetBranchStatus("*",0);
  if (run>80000){ 
  	T->SetBranchStatus("RightT**",1);
  	T->SetBranchAddress("RightT4",&Tr[1]); 
  	T->SetBranchAddress("RightT5",&Tr[2]); 
  	T->SetBranchAddress("RightT6",&Tr[3]); 
  	T->SetBranchAddress("RightLclock",&Tr[4]); 
  	T->SetBranchStatus("DR.evtypebits",1);
  	T->SetBranchAddress("DR.evtypebits",&evtypebits);
   }else{
        T->SetBranchStatus("LeftT**",1);
        T->SetBranchAddress("LeftT1",&Tr[1]);        
        T->SetBranchAddress("LeftT2",&Tr[2]);
        T->SetBranchAddress("LeftT3",&Tr[3]);
        T->SetBranchAddress("LeftLclock",&Tr[4]);
        T->SetBranchStatus("DL.evtypebits",1);
        T->SetBranchAddress("DL.evtypebits",&evtypebits);

  } 

 int NT = T->GetEntries(); 


 cout << std::setw(20) << "run" << std::setw(20) << "Trigger" << std::setw(20) << "PS" << std::setw(20) << "DAQ Events" << std::setw(20) <<
	"Scaler Events" << std::setw(20) << "Deadtime (%)"    << endl;
 for(Int_t i =0; i<4; i++){

    double Tcount = 0;
    double Tc;
    double sumT = 0;
    double prev_entry =0;
    double count_entry;
    double sum_entry; 
    double DAQ_t_inc=0;
    double DAQ_t=0;
    int l;
    T->GetEntry(0);
    Tcount = Tr[i+1];
    int k,b;

    if(run > 80000){
	l = i+4;
        if(i==3){l=8;}
    }else { 
      l = i+1;
      if(i==3){l=8;} 
    }

    
    for ( k=0; k<NT; k++){
  	T->GetEntry(k);     
    	if (Tcount!= Tr[i+1]){
	    Tcount = Tr[i+1];
            break; 
        }
     }

    for (int j=k; j<NT;j++){
    	T->GetEntry(j);
        if (Tcount!= Tr[i+1]){
         	Tc= ( Tr[i+1]- Tcount);
                sumT = sumT + Tc;    
                count_entry = k- prev_entry;
                sum_entry = sum_entry + count_entry;
                Tcount = Tr[i+1];
                prev_entry = j;
                DAQ_t = DAQ_t + DAQ_t_inc;
                DAQ_t_inc=0;
	}

        if (((Int_t)evtypebits>>l) &1){ 
        	DAQ_t_inc++;
        }
      }
      

     if(sumT == 0){ cout << "Very small run" << endl; break;}

     cout << std::setw(20) << run << std::setw(20) << l << std::setw(20) << PS[l-1] << std::setw(20) << DAQ_t << std::setw(20) <<    
        sumT << std::setw(20) << (1 - PS[l-1]*(DAQ_t/sumT))*100    << endl;
}
}
