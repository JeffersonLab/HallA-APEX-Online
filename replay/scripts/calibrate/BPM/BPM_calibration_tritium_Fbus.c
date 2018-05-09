#include "TVector.h"
#include <sstream> 
#include <string>
#include <cmath>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <TSystem.h> 
#include <TGraph.h>
#include <TString.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1F.h>
#include <TChain.h>
#include <TTree.h>
#include <TMatrix.h>
#include <TF1.h>
#include <TChain.h>
#include <TLine.h>
#include <TMath.h>
#include <TCut.h>
#include <TNtuple.h>
#include <stdlib.h>     //for using the function sleep
//
//  Steering script to extract the bpm calibration out of a bulls eye scan
//
// by Bodo Reitz, April 2003
//
// needed: ascii table, containing run numbers and results from
//         corresponding harp scans
//         always adjust pedestals before analyzing bulls eye scans
//         never change pedestals without reanalyzing it
//         do not worry, if you dont have new pedestals
//         since the bpm calibration here corrects for them
//         anyhow

// not perfect yet, feel free to improve ;)
// currently the fits put an equal weight on each data point
// one could use a weighted fit instead


//Changed in Feb. 2017 slightly.

/* In order to update this code, please do the following
	-Update the pedestals
	-Update the harp results file name and location.
	-Update the root file location for the rootfiles from the harp scans. 
*/	

/* Previosuly used
//double pedestal[8] = {3510, 3505, 3579, 3662, 3563, 3462, 3330, 3646};//RHRS
{ 3401,3372,3473,3554,3732,3613,3362,3389}

*/


using namespace std;

void BPM_calibration_tritium_Fbus (char side ='R', int quiet =1){
  	gStyle->SetOptStat(0);
	char expnr[25];
	char exppath[200];
	char codafname[255];
	char buf[255];
	char *filestatus;
	int numofscans=0;
	const char* exp="tritium_online";//getenv("EXPERIMENT");
	const char* root_dir="/chafs1/work1/tritium/Rootfiles";//"/adaqfs/home/a-onl/tritium/replay/RootFiles";

//Use vectors instead of an array, to allow for resizing!
	//Harp positions!
	TVector hax(1);	TVector hay(1);
	TVector hbx(1); TVector hby(1);
	//BMP positions!
	TVector bax(1); TVector bay(1);
	TVector bbx(1); TVector bby(1);

//Input varibles!
	//just used for input, and storage into an array.
	int run_number,epics_number;
	double dum3,dum4,dum5,dum6,dum7,dum8,dum9,dum10,dum11,dum12,dum13,dum14;


 	TCanvas *c1[20];
  	
//Position of the four BPM wires	
	double xp, xm, yp, ym;
//BMP pedestals
	//char side = 'L';
	double pedestal[9] = {0, 4260,  4227,  4355,  4448, 4585,  4461,  4161,  4158};//Right
	double pedestalL[9] ={0, 4711,  4494 , 4467 , 4470, 4864,  4699 , 4602,  4396};//Left
	
	if(side=='L'){for(int i=0;i<9;i++){pedestal[i]=pedestalL[i];}}
	
	double BCMcuts[5][2]={{61e6,120E6},{0,10E10},{6E6,42E6},{0,10E10},{45E6,140E6}};
	double cur[5] ={ 0};//{0.02,3.5,3.5,3.5,3.5};

//These arrays stand for the 8 wires of the BPMs 4 for A and 4 for B
// {blank, A Xp, A Xm, A yp, Aym, B Xp, B Xm, B yp, B ym}
	double signal[9];
	double peak[9];	
	double Sigma[9];	
	TH1F *H[9];
	TH1F *HH[9];
	TLine *TL[9];
	TLine *TL2[9];
//	int min,max;
//
	double calib = 0.01887;

  //Day of harp runs in hall A
  	char date[256] = {"05032018"};
	ifstream fi;
	char Hresults[256];
	sprintf(Hresults,"harp_results_%s.txt",date);
	if(side=='R'){sprintf(Hresults,"harp_results_R_%s.txt",date);}
	fi.open(Hresults);
	TFile *filein;
	TTree *T;
	
	if(!fi.good()){exit(1); cout << "No Harp scan file! \n"; return;}
	//////////////////////////////////////////////////////////////////
	while (fi.good()) { 

	// Start of input loop for harp_results and BMP Run.
   		fi>>run_number>>epics_number>>dum3>>dum4>>dum5>>dum6>>dum7>>dum8>>dum9>>dum10;
		if(!fi.good()){break;}
		cout<<"Will look at run "<< run_number<<endl;

	//Resize the harp and BMP vectors- add one element to the vector length for each loop, or each harp scan.   	
		hax.ResizeTo(numofscans+1); hbx.ResizeTo(numofscans+1); 
		hay.ResizeTo(numofscans+1); hby.ResizeTo(numofscans+1);
		bax.ResizeTo(numofscans+1); bbx.ResizeTo(numofscans+1);
		bay.ResizeTo(numofscans+1); bby.ResizeTo(numofscans+1);
   
	//Store the harp locations
		hax(numofscans)=dum3*1e-3; hay(numofscans)=dum5*1e-3;
		hbx(numofscans)=dum7*1e-3; hby(numofscans)=dum9*1e-3;
	
		//Input file from harp scans
		filein = TFile::Open(Form("%s/%s_%d.root",root_dir,exp,run_number));
		T =(TTree*) filein->Get("T");

	//New divided canvas for the BPM locations
 		c1[numofscans] = new TCanvas(Form("c%d",numofscans),Form("Run %d",run_number),500,500,1300,1000);
        c1[numofscans]->Divide(2,4);
	
	////Setting up the branches and the histograms to be used!
	T->SetBranchStatus("*",0);
	//T->SetBranchStatus("right_clkcount",1);
	T->SetBranchStatus("*RrbGmp*",1);
	T->SetBranchStatus("*LrbGmp*",1);
	T->SetBranchStatus("*Rrb*",1);
	T->SetBranchStatus("*Lrb*",1);	
	
	T->SetBranchStatus("hac_bcm_average",1);
	double BCM_avg;
	T->SetBranchAddress("hac_bcm_average",&BCM_avg);
	
//Setting up the BPM histograms and tree varibles!!
	//For BPMA
	for(int A=1;A<5;A++){T->SetBranchAddress(Form("Fbus%crb.BPMA.rawcur.%d",side,A),&peak[A]);
			H[A] = new TH1F(Form("H%d",A),Form("%c BPMA_%d",side,A),5000,0,10000);
			HH[A]= new TH1F(Form("HH%d",A),Form("%c BPMA_%d",side,A),2500,pedestal[A]+200,10000);}
	//For BPMB			
	for(int B=1;B<5;B++){T->SetBranchAddress(Form("Fbus%crb.BPMB.rawcur.%d",side,B),&peak[B+4]);
			H[B+4] = new TH1F(Form("H%d",B+4),Form("%c BPMB_%d",side,B),5000,0,10000);
			HH[B+4]= new TH1F(Form("HH%d",B+4),Form("%c BPMA_%d",side,B),2500,pedestal[B+4]+200,10000);}
	c1[numofscans]->cd(1);

	//Event[numofscans]="right_clkcount>=%f&&right_clkcount<=%f",BCMcuts[numofscans][0],BCMcuts[numofscans][1];
	TCut current = Form("hac_bcm_average>=%f",cur[numofscans]);
	

		///// Calculate the BPM postion from the 4 differrent wire signals, xp,xm,yp,ym!	
		for(int m=1;m<9;m++){c1[numofscans]->cd(m);
			
			//M<5 means BPMA				
			if(m<5){T->Draw(Form("Fbus%crb.BPMA.rawcur.%d>>H%d",side,m,m),current);
				peak[m] = H[m]->GetBinCenter(H[m]->GetMaximumBin());
			
			//If the pedestal is high, this will make sure we do not fit it.
				//if(peak[m] - pedestal[m]-200 <=0){T->Draw(Form("rb.BPMA.rawcur.%d>>HH%d",m,m));
				//			peak[m] = HH[m]->GetBinCenter(HH[m]->GetMaximumBin());}
				}
			////////////////////////////////////////////////////////////////////////
			//M>=5 means BPMB
			if(m>=5){T->Draw(Form("Fbus%crb.BPMB.rawcur.%d>>H%d",side,m-4,m),current);
				peak[m] = H[m]->GetBinCenter(H[m]->GetMaximumBin());
				
			//If the pedestal is high, this will make sure we do not fit it.
				//if(peak[m] - pedestal[m]-200 <=0){T->Draw(Form("rb.BPMB.rawcur.%d>>HH%d",m-4,m));
				//			peak[m] = HH[m]->GetBinCenter(HH[m]->GetMaximumBin());}
				}
			H[m]->GetXaxis()->SetRangeUser(pedestal[m]-20,peak[m]+200);
			
			H[m]->SetLineWidth(4);
	
	//Fit the raw BPM signal with a gausian to extract a value for the peak. 					
			TString drawop="Q";
			if(quiet==0){drawop="";}	
			H[m]->Fit("gaus","Q","",peak[m]-60, peak[m]+60);
        		Sigma[m] = H[m]->GetFunction("gaus")->GetParameter(2);
			H[m]->Fit("gaus",Form("%s",drawop.Data()),"",peak[m]-2.5*Sigma[m], peak[m]+2.5*Sigma[m]);
			peak[m] = H[m]->GetFunction("gaus")->GetParameter(1);
        		signal[m] =peak[m] - pedestal[m];   
  
        	//cout<<signal[m]<<" ";
        	double high= H[m]->GetMaximum();
        	TL[m] = new TLine(pedestal[m],0,pedestal[m],high*0.5);
        	TL[m]->SetLineColor(8);
        	TL[m]->SetLineWidth(3);
        	TL[m]->Draw("same");
        	

        //	TL2[m] = new TLine(pedestal2[m],0,pedestal2[m],high*0.5);
        //	TL2[m]->SetLineColor(kMagenta+2);
        //	TL2[m]->SetLineWidth(3);
        //	TL2[m]->Draw("same");
			
		 }//end of one run. 
		if(quiet==1){delete c1[numofscans];}
   	//////////////////////////////////////////////
	cout<<endl;
	
//Use those four positions to calculate the x and Y pos for BPM A!
	   bax(numofscans)=calib*(signal[1]-signal[2])/(signal[1]+signal[2]);
	   bay(numofscans)=calib*(signal[3]-signal[4])/(signal[3]+signal[4]);
   
//Use those four positions to calculate the x and Y pos for BPM B!
  		bbx(numofscans)=calib*(signal[5]-signal[6])/(signal[5]+signal[6]);
   		bby(numofscans)=calib*(signal[7]-signal[8])/(signal[7]+signal[8]);
   		
cout << bax(numofscans) << " "<< bay(numofscans) << "  "<<bbx(numofscans) << " "<<bby(numofscans)<<endl;

 //increase the number of input files
		numofscans++;

    }//END of the harp results file!!!
 	fi.close();
////////////////////////////////////////////////////////////////////////////

//Varibles with a slighlty difficult nomenclature. x11 -> BPM (x*x)
//x1yy -> BPM x * Harp y. Good luck!
// x = bpm ; //1 = x; 2= y
// y = harp : then x and y are self defined
// So yy = harp y : x2 = bpm y ; x2yy = bpm y* harp y

	Double_t x11=0.;  Double_t x12=0.; Double_t x1yx=0.; 
	Double_t x1yy=0.; Double_t x21=0.; Double_t x22=0.; 
	Double_t x2yx=0.; Double_t x2yy=0.;Double_t x1=0.; 
	Double_t x2=0.;   Double_t yx=0.;  Double_t yy=0.;

//For incrementing each varible per harp scan
	for(Int_t j=0;j<numofscans;j++) {
	  x11=x11+bax(j)*bax(j);
	  x12=x12+bax(j)*bay(j);
	  x1yx=x1yx+bax(j)*hax(j);
	  x1yy=x1yy+bax(j)*hay(j);
	  x1=x1+bax(j);
	  x2=x2+bay(j);
	  yx=yx+hax(j);
	  yy=yy+hay(j);
	  x21=x21+bay(j)*bax(j);
	  x22=x22+bay(j)*bay(j);
	  x2yx=x2yx+bay(j)*hax(j);
	  x2yy=x2yy+bay(j)*hay(j);
	}
/////////////////


//Store incremented values in Matrices
	TMatrix trafo(3,3);
	TVector inhomo(3);

	trafo(0,0)=x11; trafo(1,0)=x21; trafo(2,0)=x1;
	trafo(0,1)=x12; trafo(1,1)=x22; trafo(2,1)=x2;
	trafo(0,2)=x1;  trafo(1,2)=x2;  trafo(2,2)=numofscans;
	
	inhomo(0)=x1yx; inhomo(1)=x2yx; inhomo(2)=yx;
////////////////////////////////////////////////

// Make a copy of the transfer matrix
	TMatrix itrafo(trafo);
	//Invert said matrix	
	itrafo.Invert();
	//Multiple the one D vector by the inverted transfer matrix
	TVector solu1(inhomo); 	solu1*=itrafo;
  
//Reset and make the same calcs for BPM the other 3 coif!

	inhomo(0)=x1yy; inhomo(1)=x2yy; inhomo(2)=yy;
	TVector solu2(inhomo);
	solu2*=itrafo;

///////////////////////////////////////////////////

//Reset and make the same calcs for BPM B!
	x11=0.; x12=0.; x1yx=0.; x1yy=0.;
	x21=0.; x22=0.; x2yx=0.; x2yy=0.;
	x1=0.;  x2=0.;  yx=0.;   yy=0.;

//For incrementing each varible per harp scan
	for(Int_t j=0;j<numofscans;j++) {
	  x11=x11+bbx(j)*bbx(j);
	  x12=x12+bbx(j)*bby(j);
	  x1yx=x1yx+bbx(j)*hbx(j);
	  x1yy=x1yy+bbx(j)*hby(j);
	  x1=x1+bbx(j);
	  x2=x2+bby(j);
	  yx=yx+hbx(j);
	  yy=yy+hby(j);
	  x21=x21+bby(j)*bbx(j);
	  x22=x22+bby(j)*bby(j);
	  x2yx=x2yx+bby(j)*hbx(j);
	  x2yy=x2yy+bby(j)*hby(j);
	}

//Store incremented values in Matrices
	trafo(0,0)=x11; trafo(1,0)=x21; trafo(2,0)=x1;
	trafo(0,1)=x12; trafo(1,1)=x22; trafo(2,1)=x2;
	trafo(0,2)=x1;  trafo(1,2)=x2;  trafo(2,2)=numofscans;
	
	inhomo(0)=x1yx; inhomo(1)=x2yx; inhomo(2)=yx;

// Make a copy of the transfer matrix
	TMatrix i2trafo(trafo);
	//Invert said matrix
	i2trafo.Invert();
	//Multiply the invert matrix by the oneD vector
	TVector solu3(inhomo);
	solu3*=i2trafo;
 
//Reset and make the same calcs for BPM the other 3 coif! 
	inhomo(0)=x1yy; inhomo(1)=x2yy; inhomo(2)=yy;
	TVector solu4(inhomo);
	solu4*=i2trafo;

//////////////////////////////////////////////////



//Output results!!!!
cout<<endl<<endl;

cout<<"Please change the " << side <<" BPMA constants to:"<<endl;

cout<<solu1(0)<<" "<<solu1(1)<<" "<<solu2(0)<<" "<<solu2(1)<<" "<<solu1(2)<<" "<<solu2(2)<<endl;

cout<<"Please change the " << side <<" BPMB constants to:"<<endl;

cout<<solu3(0)<<" "<<solu3(1)<<" "<<solu4(0)<<" "<<solu4(1)<<" "<<solu3(2)<<" "<<solu4(2)<<endl;
 cout <<"\n\n\n\n";
//////////////////////////////////////////////////////////////////
/*
//Calibration without offsets
//For incrementing each varible per harp scan
	for(Int_t j=0;j<numofscans;j++) {
	  x11=x11+bax(j)*bax(j);
	  x12=x12+bax(j)*bay(j);
	  x1yx=x1yx+bax(j)*hax(j);
	  x1yy=x1yy+bax(j)*hay(j);
	  x1=x1+bax(j);
	  x2=x2+bay(j);
	  yx=yx+hax(j);
	  yy=yy+hay(j);
	  x21=x21+bay(j)*bax(j);
	  x22=x22+bay(j)*bay(j);
	  x2yx=x2yx+bay(j)*hax(j);
	  x2yy=x2yy+bay(j)*hay(j);
	}
/////////////////

//Store incremented values in Matrices
	TMatrix trafotrial(3,3);
	TVector inhomotrial(3);

	trafotrial(0,0)=x11; trafotrial(1,0)=x21; trafotrial(2,0)=0;
	trafotrial(0,1)=x12; trafotrial(1,1)=x22; trafotrial(2,1)=0;
	trafotrial(0,2)=0;  trafotrial(1,2)=0;  trafotrial(2,2)=numofscans;	
	inhomotrial(0)=x1yx; inhomotrial(1)=x2yx; inhomotrial(2)=0; 
////////////////////////////////////////////////

// Make a copy of the transfer matrix
	TMatrix itrafotrial(trafotrial);
	//Invert said matrix	
	itrafotrial.Invert();
	//Multiple the one D vector by the inverted transfer matrix
	TVector solu1trial(inhomotrial); 	solu1trial*=itrafotrial;
	//Reset and make the same calcs for BPM the other 3 coif!
	inhomotrial(0)=x1yy; inhomotrial(1)=x2yy; 
	TVector solu2trial(inhomotrial);
	solu2trial*=itrafotrial;

//Reset and make the same calcs for BPM B!
	x11=0.; x12=0.; x1yx=0.; x1yy=0.;
	x21=0.; x22=0.; x2yx=0.; x2yy=0.;
	x1=0.;  x2=0.;  yx=0.;   yy=0.;

//For incrementing each varible per harp scan
	for(Int_t j=0;j<numofscans;j++) {
	  x11=x11+bbx(j)*bbx(j);
	  x12=x12+bbx(j)*bby(j);
	  x1yx=x1yx+bbx(j)*hbx(j);
	  x1yy=x1yy+bbx(j)*hby(j);
	  x1=x1+bbx(j);
	  x2=x2+bby(j);
	  yx=yx+hbx(j);
	  yy=yy+hby(j);
	  x21=x21+bby(j)*bbx(j);
	  x22=x22+bby(j)*bby(j);
	  x2yx=x2yx+bby(j)*hbx(j);
	  x2yy=x2yy+bby(j)*hby(j);
	}

	trafotrial(0,0)=x11; trafotrial(1,0)=x21; 
	trafotrial(0,1)=x12; trafotrial(1,1)=x22; 
	
	inhomotrial(0)=x1yx; inhomotrial(1)=x2yx; 

// Make a copy of the transfer matrix
	TMatrix itrafo2trial(trafotrial);
	//Invert said matrix	
	itrafo2trial.Invert();
	//Multiple the one D vector by the inverted transfer matrix
	TVector solu3trial(inhomotrial); 	solu3trial*=itrafo2trial;
	//Reset and make the same calcs for BPM the other 3 coif!
	inhomotrial(0)=x1yy; inhomotrial(1)=x2yy; 
	TVector solu4trial(inhomotrial); solu4trial*=itrafotrial;



	cout<<"\n\n\n\n";
cout<<"Please change the " << side <<" BPMA constants to:"<<endl;
cout<<solu1trial(0)<<" "<<solu1trial(1)<<" "<<solu2(0)<<" "<<solu2(1)<<endl;
cout<<"Please change the " << side <<" BPMB constants to:"<<endl;

cout<<solu3trial(0)<<" "<<solu3trial(1)<<" "<<solu4trial(0)<<" "<<solu4trial(1)<<endl;
*/
}


