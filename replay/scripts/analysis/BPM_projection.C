#include "TStyle.h"
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
#include <TChain.h>
#include <TH1F.h>
#include <TChain.h>
#include <TTree.h>
#include <TF1.h>
#include <TVector.h>
#include <TNtuple.h>
#include <iostream>     //for using cout
#include <stdlib.h>     //for using the function sleep



void BPM_projection(int Run=0, int first=0, int last=25000, TString type = "Fbus", char arm ='L'){

	  const TString rootfilePath = "/chafs1/work1/tritium/Rootfiles/";
	  
	  
  if(Run==0){
	  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  	  cin >> Run;
  	  }
  if(Run==-1){cout << "Thank you, come again " <<endl; exit(1);}
  
    TString fname = Form("%stritium_online_%d.root",rootfilePath.Data(),Run);
    
    TChain * T = new TChain("T");
	T->Add(Form("%s",fname.Data()));
	
	T->SetBranchStatus("*",0);	
	T->SetBranchStatus(Form("%s%crb*",type.Data(),arm),1);	
	T->SetBranchStatus("hac_bcm_average",1);
	
	Double_t BPMA_x, BPMA_y, BPMB_x, BPMB_y;
	Double_t tar_x, tar_y, m_x,m_y;
	Double_t current;
	
	T->SetBranchAddress(Form("%s%crb.BPMA.x",type.Data(),arm),&BPMA_x);
	T->SetBranchAddress(Form("%s%crb.BPMA.y",type.Data(),arm),&BPMA_y);
	T->SetBranchAddress(Form("%s%crb.BPMB.x",type.Data(),arm),&BPMB_x);
	T->SetBranchAddress(Form("%s%crb.BPMB.y",type.Data(),arm),&BPMB_y);
	
	TH1F *BPMA_X = new TH1F("BPMA_X",Form("BPMA x pos for %c arm, for run %d",arm,Run),200,-0.005,0.005);
	TH1F *BPMB_X = new TH1F("BPMB_X",Form("BPMB x pos for %c arm, for run %d",arm,Run),200,-0.005,0.005);
	TH1F *BPMA_Y = new TH1F("BPMA_Y",Form("BPMA y pos for %c arm, for run %d",arm,Run),200,-0.005,0.005);
	TH1F *BPMB_Y = new TH1F("BPMB_Y",Form("BPMB y pos for %c arm, for run %d",arm,Run),200,-0.005,0.005);
	TH1F *Target_x = new TH1F("Target_X",Form("Target x projected for %c arm, for run %d",arm,Run),200,-0.005,0.005);
	TH1F *Target_y = new TH1F("Target_Y",Form("Target y projected for %c arm, for run %d",arm,Run),200,-0.005,0.005);
		
	TH2F *Target_xy = new TH2F("Target_xy",Form("Target xy projected for %c arm, for run %d",arm,Run),200,-0.005,0.005,200,-0.005,0.005);
	TH2F *BPMA_XY = new TH2F("BPMA_xy",Form("BPMA XY pos for %c arm, for run %d",arm,Run),200,-0.005,0.005,200,-0.005,0.005);
	TH2F *BPMB_XY = new TH2F("BPMB_xy",Form("BPMB XY pos for %c arm, for run %d",arm,Run),200,-0.005,0.005,200,-0.005,0.005);
	
	//Double_t A_z =7.345 , B_z=2.214 ,T_z =0;
	Double_t A_z =7.523897 , B_z=2.377568 ,T_z =0;
	Int_t nentries = T->GetEntries();
	if( nentries < last){ last = nentries; cout << "Maximum number of events is " << last <<endl;}
	if( first >= nentries){ cout << "Error: First entry is out of bounds, please change the first entry! "<<endl; exit(1);}
	
	
	
	for(int i=first; i<last;i++){T->GetEntry(i);
		//Find the slope of the line between BPMA and B
		m_x = (BPMB_x - BPMA_x)/(A_z-B_z);
		m_y = (BPMB_y - BPMA_y)/(A_z-B_z);
		// calulate the pos at target using the slope of the beam between bpms.		
		tar_x = m_x*(A_z-T_z) + BPMA_x;
		tar_y = m_y*(A_z-T_z) + BPMA_y;
	
		Target_x ->Fill(tar_x);
		Target_y ->Fill(tar_y);
		Target_xy ->Fill(tar_x,tar_y);
	}
	
	TCanvas *POS = new TCanvas;
		POS->Divide(2,3);
		POS->cd(1);
		T->Draw(Form("%s%crb.BPMA.x>>BPMA_X",type.Data(),arm),Form("Entry$>=%d&&Entry$<=%d",first,last));
		BPMA_X->GetXaxis()->SetTitle("Pos(m)");
		POS->cd(3);
		T->Draw(Form("%s%crb.BPMB.x>>BPMB_X",type.Data(),arm),Form("Entry$>=%d&&Entry$<=%d",first,last));
		BPMB_X->GetXaxis()->SetTitle("Pos(m)");
		POS->cd(5);
		Target_x->Draw();
		Target_x->GetXaxis()->SetTitle("Pos(m)");

		POS->cd(2);
		T->Draw(Form("%s%crb.BPMA.y>>BPMA_Y",type.Data(),arm),Form("Entry$>=%d&&Entry$<=%d",first,last));
		BPMB_X->GetXaxis()->SetTitle("Pos(m)");
		POS->cd(4);
		T->Draw(Form("%s%crb.BPMB.y>>BPMB_Y",type.Data(),arm),Form("Entry$>=%d&&Entry$<=%d",first,last));
		BPMB_X->GetXaxis()->SetTitle("Pos(m)");
		POS->cd(6);
		Target_y->Draw();
		Target_y->GetXaxis()->SetTitle("Pos(m)");

		
		TCanvas *POS_2D = new TCanvas;	
		POS_2D ->Divide(1,3);
		POS_2D->cd(1);
		gPad->SetGridx();
		gPad->SetGridy();
		T->Draw(Form("%s%crb.BPMA.y:%s%crb.BPMA.x>>BPMA_xy",type.Data(),arm,type.Data(),arm),Form("Entry$>=%d&&Entry$<=%d",first,last));
		BPMA_XY->GetXaxis()->SetTitle("X pos(m)");
		BPMA_XY->GetYaxis()->SetTitle("Y pos(m)");
		POS_2D->cd(2);
		gPad->SetGridx();
		gPad->SetGridy();
		T->Draw(Form("%s%crb.BPMB.y:%s%crb.BPMB.x>>BPMB_xy",type.Data(),arm,type.Data(),arm),Form("Entry$>=%d&&Entry$<=%d",first,last));
		BPMB_XY->GetXaxis()->SetTitle("X pos(m)");
		BPMB_XY->GetYaxis()->SetTitle("Y pos(m)");
		POS_2D->cd(3);
		gPad->SetGridx();
		gPad->SetGridy();
		Target_xy->Draw();
		Target_xy->GetXaxis()->SetTitle("X pos(m)");
		Target_xy->GetYaxis()->SetTitle("Y pos(m)");
		
		
		
	
	
}


