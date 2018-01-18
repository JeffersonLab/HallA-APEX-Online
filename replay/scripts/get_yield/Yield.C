#include "TRI_Main.h"
#include "TRI_Tools.h"
#include "TRI_Beam.h"
#include "TRI_Target.h"

const double TG_Theta_Max = 0.040;//40mrad
const double TG_Theta_Min =-0.040;//40mrad
const double TG_Phi_Max = 0.025;//25mrad
const double TG_Phi_Min =-0.025;//25mrad
const double TG_Dp_Max = 0.040;//4%mrad
const double TG_Dp_Min =-0.040;//4%mrad
const double TG_VZ_Max = 0.080;//4%mrad
const double TG_VZ_Min =-0.080;//4%mrad
const double P0 = 3.100; //GeV/c

//int main(){    
void Yield(){    
    TString Target = ""; cout<<"--- Target (H2,D2, H3, He3, Empty, Dummy) : "; cin >> Target;
    int kin = 0; cout<<"--- Kin (1 or 2 or 3 ...) : "; cin >> kin;
    TString Kin = Form("Kin%d", kin); 
    TString Run_String = ""; cout<<"--- List of Runs (1234,2345,3456,...): "; cin >> Run_String;
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
    const int Chain_Size = RunNoChain.size();
    
    TString Arm = ""; //cout<<"--- L or R (HRS-L or HRS-R ) : "; cin >> Arm;
    if(RunNoChain[0]<=50000)
	    Arm = "L";
    else		    
	    Arm = "R";

    TChain* T_Tree =(TChain*) gGetTree(RunNoChain, "T");

    TString cut_L = "DL.evtypebits>>2&1 && L.tr.n==1";
    TString cut_e_L = "DL.evtypebits>>2&1 && L.tr.n==1 && L.cer.asum_c>1000. && (L.prl1.e+L.prl2.e)/(L.gold.p*1000.)>0.8 && abs(rpl.z)<0.075 && abs(L.tr.tg_ph)<0.025 && abs(L.tr.tg_th)<0.04 && abs(L.tr.tg_dp)<0.04";
    TString cut_R = "DR.evtypebits>>5&1 && R.tr.n==1 ";
    TString cut_e_R = "DR.evtypebits>>5&1 && R.tr.n==1 && R.cer.asum_c>1000. && (R.ps.e+R.sh.e)>2000. && abs(rpl.z)<0.075 && abs(R.tr.tg_ph)<0.025 && abs(R.tr.tg_th)<0.04 && abs(R.tr.tg_dp)<0.04";
    cout<<"--- Good Electron Cut: "<< cut_e_L<<endl;

    int Total_Ele= T_Tree->GetEntries(TCut(cut_L));
    int Good_Ele= T_Tree->GetEntries(TCut(cut_e_L));
    cout<<endl; 
    cout<<Form("--- For %s at %s on HRS-%s:  ", Target.Data(), Kin.Data(), Arm.Data())<<endl;
    cout<<"--- Total Good Electrons from these runs are = "<<Good_Ele<<endl;

    /*Calclate Yield{{{*/
	///////////////////////////////////
	// Number of Incident particles
	///////////////////////////////////
	/*Get Ne{{{*/
	cout   << endl << "@@@@@@ Calculating Incident Particle ..."<<endl;	
	Double_t runtime;
	Double_t* NeChain = new double[Chain_Size];
	const TRI_VAR* Ne=gCal_Ne(RunNoChain,Arm,S_Tree_Name, runtime, NeChain);
	cout   <<Form("     Ne = %4.3e", Ne->Value)<<endl;	
	/*}}}end of Get Ne*/

	////////////////////////////////
	// Target Luminocity
	////////////////////////////////
	/*Get Ntg{{{*/
	cout   << endl << "@@@@@@ Calculating Target Luminocity ..."<<endl;

    int Target_A=0, Target_Z=0;
    double Target_Thickness=0.0, Target_Thickness_Err=0.0;
    int err = gGet_TargetInfo(Target.Data(), &Target_A, &Target_Z, &Target_Thickness, &Target_Thickness_Err);
    double Target_Length = (TG_VZ_Max - TG_VZ_Min)*100.; //meter to cm
	TRI_VAR* Ntg = new TRI_VAR();
    Double_t* NtgChain = new double[Chain_Size];
    //Boiling Effect for long cryo-targets will be corrected in this subroutine
    Ntg= gCal_Ntg(RunNoChain,Arm.Data(),Target_A,Target_Z,Target_Thickness,Target_Thickness_Err,Target_Length, NtgChain);
    cout   <<Form("     Avg Ntg = %4.3e; Before Boiling Corrected = %4.3e",Ntg->Value,Target_Thickness/Target_A*Na)<<endl;
    /*}}}end of Get Ntg*/

	const double phase_space = (TG_Theta_Max - TG_Theta_Min) * (TG_Phi_Max - TG_Phi_Min) * (TG_Dp_Max - TG_Dp_Min) * P0; 
    double XS = Good_Ele / Ne->Value / Ntg->Value / phase_space * CM2ToNB;
    //double XS_Err = 0.0;
    double XS_Err = XS * sqrt( 1/Good_Ele + pow(Ntg->Sys_Err/Ntg->Value, 2) + pow(Ne->Sys_Err/Ntg->Value, 2) );

    cout<<"##### Raw Cross Section = "<< XS <<" nb/GeV/sr"<<endl;
    /*}}}*/

    /*Making Plots{{{*/
    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    TH1F *h1 = new TH1F("h1","",200, -0.2,0.2);
    h1->SetXTitle("ReactZ (m)");
    T_Tree->Draw("rpl.z>>h1",TCut(cut_L));
    T_Tree->SetLineColor(2);
    T_Tree->Draw("rpl.z",TCut(cut_e_L),"same");

    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(32);
    t1->SetTextSize(0.06);
    t1->SetTextColor(1); 
    t1->DrawLatex(0.2, 0.92, Form("Target = %s at %s on HRS-%s ",Target.Data(), Kin.Data(), Arm.Data() ));

    t1->SetTextColor(4); 
    t1->SetTextSize(0.055);
    t1->DrawLatex(0.2, 0.70, Form("Total Good Electrons = %4.1f K (out of %4.1fK) ", (Good_Ele/1000.), (Total_Ele/1000.) ));
    t1->SetTextColor(2); 
    //t1->DrawLatex(0.2, 0.60, Form("   Raw Cross Section = %6.3e nb/GeV/sr", XS));
    t1->DrawLatex(0.2, 0.60, Form("    Normalized Yield = %6.3e #pm %2.1%% nb/GeV/sr", XS, XS_Err/XS*100.));

    t1->SetTextColor(6); 
    t1->SetTextSize(0.04);
    t1->DrawLatex(0.1, 0.32, Form("Run-List = %s",Run_String.Data() ));
    c1->Print(Form("good_ele_%s_%s_%s_%d.png", Target.Data(),Kin.Data(),Arm.Data(), RunNoChain[0]));
    c1->Print(Form("good_ele_%s_%s_%s_%d.pdf", Target.Data(),Kin.Data(),Arm.Data(), RunNoChain[0]));/*}}}*/
}
