

//*********************************************************************//
//                                                                     //
//    Script to check the Right HRS timing for S0 and S2               //
//                                                                     //
//                                                                     //
//    Author Tong Su ---------Aug 2017                                 //
//                                                                     //
//                                                                     //
//                                                                     //
//*********************************************************************//



#include "TTree.h"
#include "TROOT.h"
#include "TStyle.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "TH1.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TSystem.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH2.h"
#include "TLine.h"
#include "TEllipse.h"
#include "TCut.h"
#include "TProfile.h"

void Check_R_Time()

{
    gROOT->SetStyle("Plain");
    gStyle->SetPaperSize(TStyle::kUSLetter);
    gStyle->SetPaperSize(18,22);
    gStyle->SetOptFit(1111);
    gStyle->SetPalette(1);
    gStyle->SetNdivisions(505);
    
    gStyle->SetCanvasColor(10);
    gStyle->SetPadTopMargin(.05);
    gStyle->SetPadLeftMargin(.15);
    gStyle->SetPadRightMargin(.1);
    gStyle->SetPadBottomMargin(.15);
    gStyle->SetTitleYOffset(1.3);
    gStyle->SetLabelFont(42,"X");
    gStyle->SetLabelFont(42,"Y");
    
    gStyle->SetLineStyleString(1,"[]");
    gStyle->SetLineStyleString(2,"[30 10]");
    gStyle->SetLineStyleString(3,"[4 8]");
    gStyle->SetLineStyleString(4,"[15 12 4 12]");
    gStyle->SetLineStyleString(5,"[15 15]");
    gStyle->SetLineStyleString(6,"[15 12 4 12 4 12]");
    gStyle->SetLabelSize(0.045,"X");
    gStyle->SetLabelSize(0.045,"Y");
    gStyle->SetNdivisions(505,"Y");
    gStyle->SetOptDate(0);
    gStyle->SetDateY(.98);
    gStyle->SetStripDecimals(kFALSE);
    gStyle->SetOptTitle(0);
    gStyle->SetTitleFont();
    gStyle->SetTitleFontSize(20);
    gStyle->SetOptStat(0);

    
    TChain * t = new TChain("T");
    Int_t run_no;
    cout<<"Please enter a run unmber:"<<endl;
    cin>>run_no;
    t->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_no));     
    



      TH2F *S2_Pad =new TH2F("S2_Pad","S2_Pad",16,-0.5,15.5,400,-2,2);
     S2_Pad->GetYaxis()->SetTitle("x_s2[m]");
     S2_Pad->GetXaxis()->SetTitle("Pad No");
     S2_Pad->GetYaxis()->CenterTitle();
     S2_Pad->GetXaxis()->CenterTitle();
    
     TH1F *S2_Pad_check =new TH1F("S2_Pad_check","S2_Pad_check",16,-0.5,15.5);
    
    
    
    
    TH2F *S0_Pad =new TH2F("S0_Pad","S0_trx VS S2_Pad ",16,-0.5,15.5,400,-2,2);
    S0_Pad->GetYaxis()->SetTitle("x_s0[m]");
    S0_Pad->GetXaxis()->SetTitle("Pad No for S2");
    S0_Pad->GetYaxis()->CenterTitle();
    S0_Pad->GetXaxis()->CenterTitle();
    
    
     TH2F *S2_rt_Time[16];
     TH2F *S2_lt_Time[16];
     TH2F *S0_rt_Time[16];
     TH2F *S0_lt_Time[16];
     TH2F *S0_coinc_Time[16];
     TH2F *S2_coinc_Time[16];
     TH2F *S0_Time_Diff[16];
    
    
    
    
     for(Int_t i=0;i<16;i++)
        
     {S2_rt_Time[i]=new TH2F(Form("S2_rt_Time[%d]",i),Form("S2_rt VS S2_try for paddle %d",i+1),200,-1,1,500,1700,2200);
      S2_rt_Time[i]->GetXaxis()->SetTitle("y_s2[m]");
      S2_rt_Time[i]->GetYaxis()->SetTitle(Form("S2_rt TDC for paddle %d",i+1));
      S2_rt_Time[i]->GetXaxis()->CenterTitle();
      S2_rt_Time[i]->GetYaxis()->CenterTitle();
         
      S2_lt_Time[i]=new TH2F(Form("S2_lt_Time[%d]",i),Form("S2_lt VS S2_try for paddle %d",i+1),200,-1,1,500,1700,2200);
      S2_lt_Time[i]->GetXaxis()->SetTitle("y_s2[m]");
      S2_lt_Time[i]->GetYaxis()->SetTitle(Form("S2_lt TDC for paddle %d",i+1));
      S2_lt_Time[i]->GetXaxis()->CenterTitle();
      S2_lt_Time[i]->GetYaxis()->CenterTitle();
         
      S0_rt_Time[i]=new TH2F(Form("S0_rt_Time[%d]",i),Form("S0_rt VS S0_trx for paddle %d",i+1),200,-1,1,500,1700,2200);
      S0_rt_Time[i]->GetXaxis()->SetTitle("x_s0[m]");
      S0_rt_Time[i]->GetYaxis()->SetTitle(Form("S0_rt TDC for paddle %d",i+1));
      S0_rt_Time[i]->GetXaxis()->CenterTitle();
      S0_rt_Time[i]->GetYaxis()->CenterTitle();
         
      S0_lt_Time[i]=new TH2F(Form("S0_lt_Time[%d]",i),Form("S0_lt VS S0_trx for paddle %d",i+1),200,-1,1,500,1700,2200);
      S0_lt_Time[i]->GetXaxis()->SetTitle("x_s0[m]");
      S0_lt_Time[i]->GetYaxis()->SetTitle(Form("S0_lt TDC for paddle %d",i+1));
      S0_lt_Time[i]->GetXaxis()->CenterTitle();
      S0_lt_Time[i]->GetYaxis()->CenterTitle();
         
      S2_coinc_Time[i]=new TH2F(Form("S2_coinc_Time[%d]",i),Form("S2_coinc VS S2_try for paddle %d",i+1),200,-1,1,500,1700,2200);
      S2_coinc_Time[i]->GetXaxis()->SetTitle("y_s2[m]");
      S2_coinc_Time[i]->GetYaxis()->SetTitle(Form("S2_coinc TDC for paddle %d",i+1));
      S2_coinc_Time[i]->GetXaxis()->CenterTitle();
      S2_coinc_Time[i]->GetYaxis()->CenterTitle();
        
      S0_coinc_Time[i]=new TH2F(Form("S0_coinc_Time[%d]",i),Form("S0_coinc VS S0_trx for paddle %d",i+1),200,-1,1,500,1700,2200);
      S0_coinc_Time[i]->GetXaxis()->SetTitle("x_s0[m]");
      S0_coinc_Time[i]->GetYaxis()->SetTitle(Form("S0_coinc TDC for paddle %d",i+1));
      S0_coinc_Time[i]->GetXaxis()->CenterTitle();
      S0_coinc_Time[i]->GetYaxis()->CenterTitle();
     
         
      S0_Time_Diff[i]=new TH2F(Form("S0_Time_Diff[%d]",i),Form("S0_Diff VS S0_trx for paddle %d",i+1),200,-1,1,300,-150,150);
      S0_Time_Diff[i]->GetXaxis()->SetTitle("x_s0[m]");
      S0_Time_Diff[i]->GetYaxis()->SetTitle(Form("S0_Diff TDC for paddle %d",i+1));
      S0_Time_Diff[i]->GetXaxis()->CenterTitle();
      S0_Time_Diff[i]->GetYaxis()->CenterTitle();
     
     
     
     }
    
    
    //Define Variables
    Double_t R_tr_n,R_cer_asum_c,R_ps_e,R_sh_e;
    Double_t R_tr_p[100],R_s2_try[100];
    Double_t R_s2_lt[16],R_s2_rt[16];
    Double_t R_s2_nthit;
    Double_t R_s2_t_pads[16];
    Int_t evtypebits;
    Double_t R_s0_trpath[100],R_s2_trpath[100];
    Double_t R_s0_lt[10],R_s0_rt[10];
    Double_t R_s0_nthit;
    Double_t R_s0_trx[100],R_s0_try[100];
    Double_t R_s0_ladc;
    Double_t R_s0_radc;
    Double_t R_tr_beta[100];
    Double_t R_s2_trx[100];
    Double_t s0_coinc_time;
    Double_t s2_coinc_time;
    
    
    
    
    //Define Branch Status/Addresses
    
    
    
    t->SetBranchStatus("*",0);
    t->SetBranchStatus("R.tr.n",1);
    t->SetBranchStatus("R.tr.p",1);
    t->SetBranchStatus("R.cer.asum_c",1);
    t->SetBranchStatus("R.ps.e",1);
    t->SetBranchStatus("R.sh.e",1);
    t->SetBranchStatus("R.s2.lt",1);
    t->SetBranchStatus("R.s2.rt",1);
    t->SetBranchStatus("R.s2.try",1);
    t->SetBranchStatus("R.s2.nthit",1);
    t->SetBranchStatus("R.s2.t_pads",1);
    t->SetBranchStatus("R.s0.trpath",1);
    t->SetBranchStatus("R.s2.trpath",1);
    t->SetBranchStatus("R.s0.lt",1);
    t->SetBranchStatus("R.s0.rt",1);
    t->SetBranchStatus("R.s0.nthit",1);
    t->SetBranchStatus("R.s0.la_p",1);
    t->SetBranchStatus("R.s0.ra_p",1);
    t->SetBranchStatus("DR.evtypebits",1);
    t->SetBranchStatus("R.s2.trx",1);
    t->SetBranchStatus("R.tr.beta",1);
    //t->SetBranchStatus("R.s0.trx",1);
    t->SetBranchStatus("R.s0.try",1);
    t->SetBranchStatus("R.s0.trx",1);
    t->SetBranchStatus("DR.tS0coinc",1);
    //t->SetBranchStatus("th.tS2coinc",1);
    t->SetBranchStatus("DR.l_raw_T13",1);
    
    
    t->SetBranchAddress("R.tr.n",&R_tr_n);
    t->SetBranchAddress("R.tr.p",R_tr_p);
    t->SetBranchAddress("R.cer.asum_c",&R_cer_asum_c);
    t->SetBranchAddress("R.ps.e",&R_ps_e);
    t->SetBranchAddress("R.sh.e",&R_sh_e);
    t->SetBranchAddress("R.s2.lt",R_s2_lt);
    t->SetBranchAddress("R.s2.rt",R_s2_rt);
    t->SetBranchAddress("R.s2.try",R_s2_try);
    t->SetBranchAddress("R.s2.nthit",&R_s2_nthit);
    t->SetBranchAddress("R.s2.t_pads",R_s2_t_pads);
    t->SetBranchAddress("R.s0.trpath",R_s0_trpath);
    t->SetBranchAddress("R.s2.trpath",R_s2_trpath);
    t->SetBranchAddress("R.s0.lt",R_s0_lt);
    t->SetBranchAddress("R.s0.rt",R_s0_rt);
    t->SetBranchAddress("R.s0.trx",R_s0_trx);
    t->SetBranchAddress("R.s0.try",R_s0_try);
    t->SetBranchAddress("R.s0.nthit",&R_s0_nthit);
    t->SetBranchAddress("DR.evtypebits",&evtypebits);
    t->SetBranchAddress("R.s0.la_p",&R_s0_ladc);
    t->SetBranchAddress("R.s0.ra_p",&R_s0_radc);
    t->SetBranchAddress("R.s2.trx",R_s2_trx);
    t->SetBranchAddress("R.tr.beta",R_tr_beta);
    t->SetBranchAddress("DR.tS0coinc",&s0_coinc_time);
    //t->SetBranchAddress("th.tS2coinc",&s2_coinc_time);
    t->SetBranchAddress("DR.tS2coinc",&s2_coinc_time);
    
    Int_t nentries = t->GetEntries();
    cout<<"Total Number of Events = "<<nentries<<endl;
    //Loop over events
    
    for(Int_t i=0;i<nentries;i++){
        
    //******************print how many events processed for every 100000 events*********//
    if(i%100000==0) cout << " events processed = " << i << endl;
    //**********************************************************************************//
        
    t->GetEntry(i);
    for(Int_t j=0;j<16;j++){
      if(R_tr_n==1&&R_s2_t_pads[0]==j&&((int)evtypebits>>4&1))
            
        {
           
           
           S2_Pad_check->Fill(R_s2_t_pads[0]);
           S2_Pad->Fill(R_s2_t_pads[0],R_s2_trx[0]);
           S0_Pad->Fill(R_s2_t_pads[0],R_s0_trx[0]);
            S2_lt_Time[j]->Fill(R_s2_try[0],R_s2_lt[j]);
           S2_rt_Time[j]->Fill(R_s2_try[0],R_s2_rt[j]);
           S0_lt_Time[j]->Fill(R_s0_trx[0],R_s0_lt[0]);
           S0_rt_Time[j]->Fill(R_s0_trx[0],R_s0_rt[0]);
           S2_coinc_Time[j]->Fill(R_s2_try[0],s2_coinc_time);
           S0_coinc_Time[j]->Fill(R_s0_trx[0],s0_coinc_time);
           S0_Time_Diff[j]->Fill(R_s0_trx[0],R_s0_lt[0]-R_s0_rt[0]);
            
            
        
        }
    }
    
}
    TString name1=Form("check_time_R_%d.pdf",run_no);
    
    TString name2=name1+Form("[");
    TString name3=name1+Form("]");
    
    TCanvas *c1[16];
    TCanvas *c2[16];
    TCanvas *c3[16];
    TCanvas *c4[16];
    TCanvas *c5[16];
    TCanvas *c6[16];
    TCanvas *c7[17];
    
    TCanvas *cc=new TCanvas("cc","cc",1200,1200);
    
    cc->cd();
    S2_Pad_check->Draw();
    cc->SaveAs(name2);
    cc->SaveAs(name1);

    
    for(Int_t index=0;index<16;index++)
    
    { c1[index]=new TCanvas(Form("c1[%d]",index),Form("c1[%d]",index),1200,1200);
      c2[index]=new TCanvas(Form("c2[%d]",index),Form("c2[%d]",index),1200,1200);
      c3[index]=new TCanvas(Form("c3[%d]",index),Form("c3[%d]",index),1200,1200);
      c4[index]=new TCanvas(Form("c4[%d]",index),Form("c4[%d]",index),1200,1200);
      c5[index]=new TCanvas(Form("c5[%d]",index),Form("c5[%d]",index),1200,1200);
      c6[index]=new TCanvas(Form("c6[%d]",index),Form("c6[%d]",index),1200,1200);
      c7[index]=new TCanvas(Form("c7[%d]",index),Form("c7[%d]",index),1200,1200);
     
        
        
        c1[index]->cd();
        S2_lt_Time[index]->Draw("COLZ");
        c2[index]->cd();
        S2_rt_Time[index]->Draw("COLZ");
        c3[index]->cd();
        S0_lt_Time[index]->Draw("COLZ");
        c4[index]->cd();
        S0_rt_Time[index]->Draw("COLZ");
        c5[index]->cd();
        S2_coinc_Time[index]->Draw("COLZ");
        c6[index]->cd();
        S0_coinc_Time[index]->Draw("COLZ");
   
        c7[index]->cd();
        S0_Time_Diff[index]->Draw("COLZ");
        
       
        
        
        
        c1[index]->SaveAs(name1);
        c2[index]->SaveAs(name1);
        c3[index]->SaveAs(name1);
        c4[index]->SaveAs(name1);
        c5[index]->SaveAs(name1);
        c6[index]->SaveAs(name1);
        c7[index]->SaveAs(name1);
        
    }
    
    
    
    
    
    TCanvas *cc2=new TCanvas("cc2","cc2",1200,1200);
    
    cc2->cd();
    S0_Pad->Draw("COLZ");
    cc2->SaveAs(name1);
    cc2->SaveAs(name3);
        
    
    
    
    

}
