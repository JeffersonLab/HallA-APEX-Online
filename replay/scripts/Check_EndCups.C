#include "TRI_Tools.h"

void Check_EndCups()
{
    double Theta0 = 0.0;  cout<<"--     Angle = "; cin>>Theta0;
    //double SinT = sin(Theta0*3.1415926/180);
    int kin = 0; cout<<"--- Kin (1 or 2 or 3 ...) : "; cin >> kin;
    TString Kin = Form("Kin%d", kin); 
    
    TString Run_String = ""; 
    cout<<"--- Empty Runs (1234,2345,3456,...): "; cin >> Run_String;
    const vector<Int_t> RunNO_MT=gGet_RunNoChain(Run_String);
    //cout<<"--- H1 Runs (1234,2345,3456,...): "; cin >> Run_String;
    //const vector<Int_t> RunNO_H1=gGet_RunNoChain(Run_String);
    cout<<"--- D2 Runs (1234,2345,3456,...): "; cin >> Run_String;
    const vector<Int_t> RunNO_D2=gGet_RunNoChain(Run_String);
    cout<<"--- H3 Runs (1234,2345,3456,...): "; cin >> Run_String;
    const vector<Int_t> RunNO_H3=gGet_RunNoChain(Run_String);
    cout<<"--- He3 Runs (1234,2345,3456,...): "; cin >> Run_String;
    const vector<Int_t> RunNO_He3=gGet_RunNoChain(Run_String);
 
    TString Arm = ""; //cout<<"--- L or R (HRS-L or HRS-R ) : "; cin >> Arm;
    if(RunNO_MT[0]<=50000)
	    Arm = "L";
    else		    
	    Arm = "R";

    TChain* T0 =(TChain*) gGetTree(RunNO_MT, "T");
    //TChain* T1 =(TChain*) gGetTree(RunNO_H1, "T");
    TChain* T2 =(TChain*) gGetTree(RunNO_D2, "T");
    TChain* T3 =(TChain*) gGetTree(RunNO_H3, "T");
    TChain* T4 =(TChain*) gGetTree(RunNO_He3, "T");

    TH1F* h0 = new TH1F("h0","Empty Target",200,-0.20,0.20);
    h0->SetXTitle("rpl.z");
    TH1F* h01 = new TH1F("h01","Empty Target",200,-0.20,0.20);
    h01->SetXTitle("rpl.z");
    TH1F* h02 = new TH1F("h02","Empty Target",200,-0.20,0.20);
    h02->SetXTitle("rpl.z");

    //TH1F* h1 = new TH1F("h1","H1 Target",200,-0.20,0.20);
    //h1->SetXTitle("rpl.z");
    //TH1F* h11 = new TH1F("h11","H1 Target",200,-0.20,0.20);
    //h11->SetXTitle("rpl.z");
    //TH1F* h12 = new TH1F("h12","H1 Target",200,-0.20,0.20);
    //h12->SetXTitle("rpl.z");

    TH1F* h2 = new TH1F("h2","D2 Target",200,-0.20,0.20);
    h2->SetXTitle("rpl.z");
    TH1F* h21 = new TH1F("h21","D2 Target",200,-0.20,0.20);
    h21->SetXTitle("rpl.z");
    TH1F* h22 = new TH1F("h22","D2 Target",200,-0.20,0.20);
    h22->SetXTitle("rpl.z");

    TH1F* h3 = new TH1F("h3","H3 Target",200,-0.20,0.20);
    h2->SetXTitle("rpl.z");
    TH1F* h31 = new TH1F("h31","H3 Target",200,-0.20,0.20);
    h31->SetXTitle("rpl.z");
    TH1F* h32 = new TH1F("h32","H3 Target",200,-0.20,0.20);
    h32->SetXTitle("rpl.z");

    TH1F* h4 = new TH1F("h4","He3 Target",200,-0.20,0.20);
    h4->SetXTitle("rpl.z");
    TH1F* h41 = new TH1F("h41","He3 Target",200,-0.20,0.20);
    h41->SetXTitle("rpl.z");
    TH1F* h42 = new TH1F("h42","He3 Target",200,-0.20,0.20);
    h42->SetXTitle("rpl.z");

    TString cut0 = Form("((DL.evtypebits>>2)&1) && L.tr.n==1 && L.cer.asum_c>1000 && ((L.prl1.e+L.prl2.e)/(L.gold.p*1000.))>0.8 && abs(rpl.z)<0.18 &&abs(L.gold.dp)<0.04&&abs(L.gold.ph)<0.030&&abs(L.gold.th)<0.04");
    TString cut1 = Form("((DL.evtypebits>>2)&1) && L.tr.n==1 && L.cer.asum_c>1000 && ((L.prl1.e+L.prl2.e)/(L.gold.p*1000.))>0.8 && abs(rpl.z)<0.08 &&abs(L.gold.dp)<0.04&&abs(L.gold.ph)<0.030&&abs(L.gold.th)<0.04");
    TString cut2 = Form("((DL.evtypebits>>2)&1) && L.tr.n==1 && L.cer.asum_c>1000 && ((L.prl1.e+L.prl2.e)/(L.gold.p*1000.))>0.8 &&    (rpl.z)>0.08 &&abs(L.gold.dp)<0.04&&abs(L.gold.ph)<0.030&&abs(L.gold.th)<0.04");

    TCanvas *c1 = new TCanvas("c1","c1", 800,600);
    T0->Draw(Form("rpl.z>>h0" ),TCut(cut0));
    h0->SetLineColor(1);
    T0->Draw(Form("rpl.z>>h01" ),TCut(cut1));
    h01->SetLineColor(1);
    T0->Draw(Form("rpl.z>>h02" ),TCut(cut2));
    h02->SetLineColor(1);

    TCanvas *c2 = new TCanvas("c2","c2", 800,600);
    c2->Divide(2,2);
    c2->cd(1);
    //T1->Draw(Form("rpl.z>>h1" ),TCut(cut0));
    //h1->SetLineColor(8);
    //T1->Draw(Form("rpl.z>>h11" ),TCut(cut1));
    //h11->SetLineColor(8);
    //T1->Draw(Form("rpl.z>>h12" ),TCut(cut2));
    //h12->SetLineColor(8);

    c2->cd(2);
    T2->Draw(Form("rpl.z>>h2" ),TCut(cut0));
    h2->SetLineColor(2);
    T2->Draw(Form("rpl.z>>h21" ),TCut(cut1));
    h21->SetLineColor(2);
    T2->Draw(Form("rpl.z>>h22" ),TCut(cut2));
    h22->SetLineColor(2);

    c2->cd(3);
    T3->Draw(Form("rpl.z>>h3" ),TCut(cut0));
    h3->SetLineColor(4);
    T3->Draw(Form("rpl.z>>h31" ),TCut(cut1));
    h31->SetLineColor(4);
    T3->Draw(Form("rpl.z>>h32" ),TCut(cut2));
    h32->SetLineColor(4);

    c2->cd(4);
    T4->Draw(Form("rpl.z>>h4" ),TCut(cut0));
    h4->SetLineColor(6);
    T4->Draw(Form("rpl.z>>h41" ),TCut(cut1));
    h41->SetLineColor(6);
    T4->Draw(Form("rpl.z>>h42" ),TCut(cut2));
    h42->SetLineColor(6);

    double N0 = h02->GetBinContent(h02->GetMaximumBin());
    double N1 = 0.0;// h12->GetBinContent(h12->GetMaximumBin());
    double N2 = h22->GetBinContent(h22->GetMaximumBin());
    double N3 = h32->GetBinContent(h32->GetMaximumBin());
    double N4 = h42->GetBinContent(h42->GetMaximumBin());

    cerr<<Form("N(H1, D2, H3, He3)=%e, %e, %e", N2/N0,N3/N0,N4/N0)<<endl;

    double CNT0 = h01->Integral();
    //double CNT1 = h11->Integral();
    double CNT2 = h21->Integral();
    double CNT3 = h31->Integral();
    double CNT4 = h41->Integral();

    //h1->Scale(N0/N1);
    h2->Scale(N0/N2);
    h3->Scale(N0/N3);
    h4->Scale(N0/N4);

    //h11->Scale(N0/N1);
    h21->Scale(N0/N2);
    h31->Scale(N0/N3);
    h41->Scale(N0/N4);

    double NT0 = h01->Integral();
    //double NT1 = h11->Integral();
    double NT2 = h21->Integral();
    double NT3 = h31->Integral();
    double NT4 = h41->Integral();

    //cerr<<Form("Cont(H1, D2, H3, He3) new=%3.2f, %3.2f, %3.2f, %3.2f", NT0/ NT1*100.,  NT0/ NT2*100.,  NT0/ NT3*100.,  NT0/ NT4*100.)<<endl;
    cerr<<Form("Cont(D2, H3, He3) new= %3.2f, %3.2f, %3.2f", NT0/ NT2*100.,  NT0/ NT3*100.,  NT0/ NT4*100.)<<endl;

    TCanvas *c3 = new TCanvas("c3","c3",800,600);
    gStyle->SetOptStat(0);
    gPad->SetLogy(1);
    h0->Draw();
    //h1->Draw("HIST same");
    h2->Draw("HIST same");
    h3->Draw("HIST same");
    h4->Draw("HIST same");
    
    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(32);
    //t1->SetTextColor(8); t1->DrawLatex(0.3, 0.66, Form("Empty/H1 = %2.1f %% #pm %2.1f %% ",
//			    NT0/NT1*100., sqrt( 1./NT0 + 1./NT1) *NT0/NT1*100. ));
    t1->SetTextColor(2); t1->DrawLatex(0.3, 0.72, Form("Empty/D2 = %2.1f %% #pm %2.1f %% ", 
			    NT0/NT2*100., sqrt( 1./NT0 + 1./NT2) *NT0/NT2*100. ));
    t1->SetTextColor(4); t1->DrawLatex(0.3, 0.78, Form("Empty/H3 = %2.1f %% #pm %2.1f %% ", 
			    NT0/NT3*100., sqrt( 1./NT0 + 1./NT3) *NT0/NT3*100. ));
    t1->SetTextColor(6); t1->DrawLatex(0.3, 0.84, Form("Empty/He3 = %2.1f %% #pm %2.1f %% ",
			    NT0/NT4*100., sqrt( 1./NT0 + 1./NT4) *NT0/NT4*100. ));
    
    t1->SetTextColor(3); t1->DrawLatex(0.3, 0.50, Form("|VertexZ|<8cm"));
    t1->SetTextColor(3); t1->DrawLatex(0.3, 0.45, Form("%s, #theta = %4.2f", Kin.Data(), Theta0));
    
    c3->Print(Form("gas_vs_empty/%s_%d.pdf",Kin.Data(),RunNO_MT[0]));
    c3->Print(Form("gas_vs_empty/%s_%d.png",Kin.Data(),RunNO_MT[0]));
}
