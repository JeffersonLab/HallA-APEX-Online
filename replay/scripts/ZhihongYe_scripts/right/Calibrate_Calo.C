#include "XGT2.h"
#include "TVirtualFitter.h"
#include "TMinuit.h"
#define MAXNBLK 9 //number of block for each a_p

/*README{{{*/
/*
  Calibrate_Calo(IsPed=kFALSE)
  Example:
  3924 Calibrate_Calo(0)
  3924 Calibrate_Calo

  if you try to calibrate amp, please make sure to set all amp=1,then replay the file and use this script
  Because analyzer will only choose 9 blocks with a_c, so if your amp=0, you may lose your signal.


  const char* calo_name[NCALO]={"ps","sh"};
  1. Read db from Form("%s/db_%s.%s.dat",RunNo_DB_DIR.Data(),Arm.Data(),calo_name[i].Data()), copy to
  gSystem->Exec(Form("cp %s %s/old_db_%s.cer.dat",DB_FileName.Data(),BaseDir.Data(),Arm.Data()))
  2. if ( IsPed ), calibrate pedestal
  3. if ( !IsPed ), calibrate amplitude
  chi_square= pow((C_ps*a_p(ps)+C_sh*a_p(sh)-p),2)
  4. Save result to Form("%s/db_%s.%s.dat",BaseDir.Data(),Arm.Data(),calo_name[i].Data())

  int Calibrate_PS_SH(const TString& aFileName,const Int_t& aBaseSize,const TString& aLogFileName)
  RunNo
  T_Tree_Name
  Arm
  RunNo_DB_DIR
  DECAT_Name
  P
  Main_Trigger

  IsPed=kFALSE;

  Save log to XGT2_DIR/logs/aLogFileName
*/
/*}}}*/

Int_t n;//number of valid entries
Int_t NCALO;//number of calorimeter
Int_t*** nblk; //nblk[i][j][k]: (i: number of valid entries) (j: 0/1 ps or sh) (k:Ndata.Arm.ps/sh.nblk)
//in ith valid entry, for jth calorimeter, nblk[i][j][k]=the kth block in the main cluster
Int_t** Ndata_nblk;//Ndata_nblk[i][j]: (i: number of valid entries) (j: 0/1 ps or sh)
//in ith valid entry, for jth calorimeter, Ndata_nblk[i][j]=number of blocks in the main cluster
Double_t*** a_p;//a_p[i][j][k](i: number of valid entries) (j: 0/1 ps or sh) (k:nblk[i][j][k])
//in ith valid entry, for jth calorimeter, nblk[i][j][k]=(raw_adc-ped) of the kth block in the main cluster
Double_t* p;//momentum
Int_t* nelem;


Int_t cluster_n;
Int_t*** cluster_nblk;
Int_t** cluster_Ndata_nblk;
Double_t*** cluster_a_p;
Double_t* cluster_p;

/*void calc_chi_square(Int_t& npar, Double_t* gin, Double_t &f, Double_t* x, Int_t iflag){{{*/
void calc_chi_square(Int_t& npar, Double_t* gin, Double_t &f, Double_t* x, Int_t iflag)
{
  Int_t i,j,k,pmt;
  Double_t chisq=0,delta;
  Bool_t Isbadblock=kFALSE;
  for ( i=0; i<cluster_n; i++ )
    {
      delta=0;
      //Printf("Ndata_nblk[%d][%d]=%d",i,j,Ndata_nblk[i][j]);
      for ( j=0; j<NCALO; j++ )
	{
	  for ( k=0; k<cluster_Ndata_nblk[i][j]; k++ )
	    {
	      if ( fabs(cluster_a_p[i][j][k])>10 )
		delta+=x[cluster_nblk[i][j][k]]*cluster_a_p[i][j][k];
	    }
	}
      delta-=cluster_p[i];
      chisq+=delta*delta;
    }
  f=chisq;
  //cout<<"DEBUG-->chisq="<<chisq<<endl;
}
/*}}}*/

/*int Calibrate_Calo(const TString& aFileName,const Int_t& aBaseSize,const TString& aLogFileName){{{*/
int Calibrate_Calo(const TString& aFileName,const Int_t& aBaseSize,const TString& aLogFileName)
{
  TStopwatch timer;
  timer.Start();

  XGT2_Logger* log=XGT2_Logger::Instance();
  log->SetLogFileName(aLogFileName);
  Int_t i=0,j=0,k=0;//i=valid entry j=det k=nblk
  /*Read InputFile{{{*/
  Int_t RunNo=-1;
  RunNo=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_RUNNO).Atoi();
  const TString T_Tree_Name=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_T_TREE_NAME);
  const TString Arm=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_ARM);
  const TString RunNo_DB_DIR=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_RUNNO_DB_DIR);
  const TString DECAT_Name=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_ROOTDECAT_NAME);
  const Double_t P=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_SPEC_SET_MOM).Atof();
  const Int_t Main_Trigger=gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_MAIN_TRIGGER).Atoi();

  Bool_t IsPed=kFALSE;
  if ( !gGet_InputFile_Var(aFileName,aBaseSize+i).IsNull() )
    IsPed=gGet_InputFile_Var(aFileName,aBaseSize+i).Atoi();

  i++;
  Bool_t IsRunMinuit=kTRUE;
  if ( !gGet_InputFile_Var(aFileName,aBaseSize+i).IsNull() )
    IsRunMinuit=gGet_InputFile_Var(aFileName,aBaseSize+i).Atoi();

  i++;
  Int_t Config_Tag=-1;
  if ( !gGet_InputFile_Var(aFileName,aBaseSize+i).IsNull() )
    Config_Tag=gGet_InputFile_Var(aFileName,aBaseSize+i).Atoi();

  i++;
  TString Detector_Name="";//only work for left arm
  if ( !gGet_InputFile_Var(aFileName,aBaseSize+i).IsNull() )
    Detector_Name=gGet_InputFile_Var(aFileName,aBaseSize+i);

  i++;
  Bool_t IsDraw_a_p=kFALSE;
  if ( !gGet_InputFile_Var(aFileName,aBaseSize+i).IsNull() )
    IsDraw_a_p=gGet_InputFile_Var(aFileName,aBaseSize+i).Atoi();

  i++;
  Bool_t IsPed_FitGaus=kTRUE;
  if ( !gGet_InputFile_Var(aFileName,aBaseSize+i).IsNull() )
    IsPed_FitGaus=gGet_InputFile_Var(aFileName,aBaseSize+i).Atoi();

  //Printf(Form("RunNo=%d, A_BullFile_Name=%s,B_BullFile_Name",RunNo,A_BullFile_Name.Data(),B_BullFile_Name.Data()));
  /*}}}*/
  RunNo=gAskRunNo(RunNo);
  if ( RunNo<0 )
    {
      log->WriteToLogFile(__FILE__,__LINE__,Form("Error No %d is input.",RunNo));
      log->Destroy();
      return -1;
    }
  const vector<Int_t> RunNoChain=gGet_RunNoChain(RunNo,gGet_InputFile_Var(aFileName,E_INPUT_VARS::k_INPUT_RUNNOCHAIN));
  TString BaseDir=Form("%s/results/%s",XGT2_DIR.Data(),gGetScriptname(__FILE__).Data());
  gCheckDirectory(BaseDir.Data());
  BaseDir+=Form("/%d",RunNo);
  gCheckDirectory(BaseDir.Data());

  TChain* T_Tree=gGetTree(RunNoChain,T_Tree_Name);

  vector<TString> calo_name;
  calo_name.clear();
  //first det is main part
  if ( Arm=="L" )
    {
      if ( Detector_Name.IsNull() )
	{
	  calo_name.push_back("XGT2Box1");
	  calo_name.push_back("XGT2Box2");
	  calo_name.push_back("XGT2Box3");
	  calo_name.push_back("prl1");
	  calo_name.push_back("prl2");
	}
      else
	calo_name.push_back(Detector_Name);
    }
  else
    {
      if ( Detector_Name.IsNull() )
	{
	  calo_name.push_back("sh");
	  calo_name.push_back("ps");
	}
      else
	calo_name.push_back(Detector_Name);
    }
  NCALO=calo_name.size();

  Double_t** pos=new Double_t*[NCALO];//center position
  Double_t** first_block_pos=new Double_t*[NCALO];//first block center position
  Double_t** space=new Double_t*[NCALO];//dx and dy block spacing
  Double_t** oldped=new Double_t*[NCALO];//0: ps 1: sh
  Double_t** newped=new Double_t*[NCALO];
  Double_t** oldamp=new Double_t*[NCALO];//0: ps 1: sh
  Double_t** newamp=new Double_t*[NCALO];
  Int_t* ncols=new Int_t[NCALO];
  Int_t* nrows=new Int_t[NCALO];
  TString epsfilename;
  nelem=new Int_t[NCALO];
  Bool_t IsSuggestPed=kFALSE;
  Bool_t IsThisTag=kFALSE;


  /*Read DB_FileName and Create outDB_FileName{{{*/
  TString* DB_FileName=new TString[NCALO];
  TString* outDB_FileName=new TString[NCALO];
  ifstream* dbfile=new ifstream[NCALO];
  ofstream* outdbfile=new ofstream[NCALO];
  TString comment;
  for ( i=0; i<NCALO; i++ )
    {
      DB_FileName[i]=Form("%s/db_%s.%s.dat",RunNo_DB_DIR.Data(),Arm.Data(),calo_name[i].Data());
      outDB_FileName[i]=Form("%s/db_%s.%s.dat",BaseDir.Data(),Arm.Data(),calo_name[i].Data());
      //Printf("DB_FileName[%d]=%s\n outDB_FileName[%d]=%s",i,DB_FileName[i].Data(),i,outDB_FileName[i].Data());
      gSystem->Exec(Form("cp %s %s/old_db_%s.%s.dat",DB_FileName[i].Data(),BaseDir.Data(),Arm.Data(),calo_name[i].Data()));

      Printf("Reading %s",DB_FileName[i].Data());
      dbfile[i].open(DB_FileName[i].Data());
      outdbfile[i].open(outDB_FileName[i].Data());
      while ( comment.ReadLine(dbfile[i]) )
	{
	  outdbfile[i]<<comment<<endl;
	  if ( comment.Contains("column",TString::kIgnoreCase) && comment.Contains("row",TString::kIgnoreCase) )
	    break;
	}
      dbfile[i]>>ncols[i]>>nrows[i];
      outdbfile[i]<<Form("%*d%*d",8,ncols[i],8,nrows[i])<<endl;
      nelem[i]=ncols[i]*nrows[i];

      pos[i]=new Double_t[3];
      first_block_pos[i]=new Double_t[2];
      space[i]=new Double_t[2];
      while ( comment.ReadLine(dbfile[i]) )
	{
	  outdbfile[i]<<comment<<endl;
	  if ( comment.Contains("x,y,z",TString::kIgnoreCase) && comment.Contains("coords",TString::kIgnoreCase) )
	    break;
	}
      dbfile[i]>>pos[i][0]>>pos[i][1]>>pos[i][2];
      comment.ReadLine(dbfile[i]);
      outdbfile[i]<<Form("%*.3f%*.3f%*.3f%*s",8,pos[i][0],8,pos[i][1],8,pos[i][2],40,comment.Data())<<endl;
      while ( comment.ReadLine(dbfile[i]) )
	{
	  outdbfile[i]<<comment<<endl;
	  if ( comment.Contains("x,y",TString::kIgnoreCase) && comment.Contains("position",TString::kIgnoreCase) )
	    break;
	}
      dbfile[i]>>first_block_pos[i][0]>>first_block_pos[i][1];
      comment.ReadLine(dbfile[i]);
      outdbfile[i]<<Form("%*.3f%*.3f%*s",8,first_block_pos[i][0],8,first_block_pos[i][1],40,comment.Data())<<endl;

      comment.ReadLine(dbfile[i]);
      outdbfile[i]<<comment<<endl;
      dbfile[i]>>space[i][0]>>space[i][1];
      comment.ReadLine(dbfile[i]);
      outdbfile[i]<<Form("%*.3f%*.3f%*s",8,space[i][0],8,space[i][1],40,comment.Data())<<endl;


      oldped[i]=new Double_t[nelem[i]];
      newped[i]=new Double_t[nelem[i]];
      oldamp[i]=new Double_t[nelem[i]];
      newamp[i]=new Double_t[nelem[i]];
      //only support [ config=runno tag ]
      if ( Config_Tag<0 )
	IsThisTag=kTRUE;

      while ( comment.ReadLine(dbfile[i]) )
	{
	  if ( comment.Contains("ADC",TString::kIgnoreCase) && comment.Contains("pedestal",TString::kIgnoreCase) && IsThisTag )
	    {
	      for ( j=0; j<nelem[i]; j++ )
		{
		  dbfile[i]>>oldped[i][j];
		  newped[i][j]=oldped[i][j];
		}
	      comment.ReadLine(dbfile[i]);
	      for ( j=0; j<nelem[i]; j++ )
		{
		  dbfile[i]>>oldamp[i][j];
		}

	      break;
	    }
	  else if ( comment.BeginsWith("[") && comment.EndsWith("]") )
	    {
	      if ( Config_Tag>0 )
		{
		  if ( comment.Contains(Form("%d",Config_Tag)) )
		    IsThisTag=kTRUE;
		}
	      outdbfile[i]<<comment<<endl;
	    }
	  else
	    {
	      outdbfile[i]<<comment<<endl;
	    }
	  if ( !dbfile[i].good() )
	    break;
	}
    }
  /*}}}*/
  /*BadBlocks{{{*/
  Int_t XGT2Box1_BadBlocks[]={50,59,61,66,68,70,-1};
  Int_t XGT2Box2_BadBlocks[]={0,59,81,89,-1};
  Int_t XGT2Box3_BadBlocks[]={82,-1};
  vector<Int_t> BadBlocks;
  for ( i=0; i<NCALO; i++ )
    {
      j=0;
      if ( calo_name[i]=="XGT2Box1" )
	{
	  while ( XGT2Box1_BadBlocks[j]>=0 )
	    {
	      k=0;
	      for ( int jj=0; jj<i; jj++ )
		k += nelem[jj];
	      k+=XGT2Box1_BadBlocks[j];
	      BadBlocks.push_back(k);
	      j++;
	    }
	}
      else if ( calo_name[i]=="XGT2Box2" )
	{
	  while ( XGT2Box2_BadBlocks[j]>=0 )
	    {
	      k=0;
	      for ( int jj=0; jj<i; jj++ )
		k += nelem[jj];
	      k+=XGT2Box2_BadBlocks[j];
	      BadBlocks.push_back(k);
	      j++;
	    }
	}
      else if ( calo_name[i]=="XGT2Box3" )
	{
	  while ( XGT2Box3_BadBlocks[j]>=0 )
	    {
	      k=0;
	      for ( int jj=0; jj<i; jj++ )
		k += nelem[jj];
	      k+=XGT2Box3_BadBlocks[j];
	      BadBlocks.push_back(k);
	      j++;
	    }
	}
    }
  /*}}}*/

  TH1F* h1;
  TF1* f1;
  TLine* n0;
  TLine* n1;
  TLegend* leg;
  TCanvas* c;
  TString cuts;

  Double_t center,height;
  Int_t cx=4,cy=4,ci;
  if ( IsPed )
    {
      /*Ped{{{*/
      for ( i=0; i<NCALO; i++ )
	{
	  epsfilename=Form("%s/%d_%s_Ped.eps",BaseDir.Data(),RunNo,calo_name[i].Data());
	  c=new TCanvas(Form("c_%s_Ped",calo_name[i].Data()),Form("c_%s_Ped",calo_name[i].Data()));
	  c->Divide(cx,cy);
	  for ( j=0; j<nelem[i]; j++ )
	    {
	      if ( j%(cx*cy)==0 )
		{
		  c->Clear();
		  c->Divide(cx,cy);
		}
	      c->cd(j%(cx*cy)+1);
	      gGet_Leaf_Mean_and_RMS(T_Tree,Form("%s.%s.a[%d]",Arm.Data(),calo_name[i].Data(),j),Form("abs(%s.%s.a[%d]-740)<700",Arm.Data(),calo_name[i].Data(),j),center,height);
	      if ( IsPed_FitGaus )
		{
		  if ( center>40 )
		    {
		      if ( center>100 )
			h1=new TH1F(Form("h1_%s_a[%d]",calo_name[i].Data(),j),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,center-100,center+100);
		      else
			h1=new TH1F(Form("h1_%s_a[%d]",calo_name[i].Data(),j),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,10,120);
		      T_Tree->Draw(Form("%s.%s.a[%d]>>h1_%s_a[%d]",Arm.Data(),calo_name[i].Data(),j,calo_name[i].Data(),j),"","",100000);

		      center=h1->GetBinCenter(h1->GetMaximumBin());
		      if ( center>50 )
			h1->Fit("gaus","Q","",center-50,center+50);
		      else
			h1->Fit("gaus","Q","",10,80);
		      f1=h1->GetFunction("gaus");
		      newped[i][j]=f1->GetParameter(1)+f1->GetParameter(2);//add one sigma more because wide spread of gaus
		    }
		  else
		    {
		      if ( oldped[i][j]>100 )
			h1=new TH1F(Form("h1_%s_a[%d]",calo_name[i].Data(),j),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,oldped[i][j]-100,oldped[i][j]+100);
		      else
			h1=new TH1F(Form("h1_%s_a[%d]",calo_name[i].Data(),j),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,30,200);
		      T_Tree->Draw(Form("%s.%s.a[%d]>>h1_%s_a[%d]",Arm.Data(),calo_name[i].Data(),j,calo_name[i].Data(),j),"","",100000);
		      newped[i][j]=oldped[i][j];
		    }
		}
	      else
		{
		  if ( center<1e-10 )
		    center=10;
		  if ( height<1e-10 )
		    height=10;
		  h1=new TH1F(Form("h1_%s_a[%d]",calo_name[i].Data(),j),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,center-height,center+height);
		  T_Tree->Draw(Form("%s.%s.a[%d]>>h1_%s_a[%d]",Arm.Data(),calo_name[i].Data(),j,calo_name[i].Data(),j));
		  center=h1->GetBinCenter(h1->GetMaximumBin());
		  height=h1->GetBinContent(h1->GetMaximumBin());
		  if ( height<=10 )
		    {
		      newped[i][j]=oldped[i][j];
		      delete h1;
		      h1=new TH1F(Form("h1_%s_a[%d]",calo_name[i].Data(),j),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,oldped[i][j]-50,oldped[i][j]+50);
		      T_Tree->Draw(Form("%s.%s.a[%d]>>h1_%s_a[%d]",Arm.Data(),calo_name[i].Data(),j,calo_name[i].Data(),j));
		    }
		  else
		    {
		      newped[i][j]=center+10;
		    }
		}

	      height=h1->GetBinContent(h1->GetMaximumBin());
	      if ( fabs(height)<1e-10 )
		height=10;

	      n0=new TLine(oldped[i][j],0,oldped[i][j],height);
	      n0->SetLineColor(kBlue);
	      n0->Draw();

	      n1=new TLine(newped[i][j],0,newped[i][j],height);
	      n1->SetLineColor(kRed);
	      n1->Draw();

	      leg=new TLegend(0.2,0.2,0.4,0.4);
	      leg->Clear();
	      leg->AddEntry(n0,Form("%s Old Ped",calo_name[i].Data()),"L");
	      leg->AddEntry(n1,Form("%s New Ped",calo_name[i].Data()),"L");
	      leg->Draw();

	      ci=(j+1)%(cx*cy);
	      if ( j==15 && nelem[i]>16 )
		c->Print(Form("%s(",epsfilename.Data()));
	      else if ( ci==0 && j!=(nelem[i]-1))
		{
		  c->Print(Form("%s",epsfilename.Data()));
		}
	      else if ( j==(nelem[i]-1) )
		c->Print(Form("%s)",epsfilename.Data()));
	    }

	  outdbfile[i]<<"ADC pedestals"<<endl;
	  for ( j=0; j<nelem[i]; j++ )
	    {
	      outdbfile[i]<<Form("%*.0f",6,newped[i][j]);
	      if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
	    }
	  outdbfile[i]<<endl;
	  outdbfile[i]<<"Energy calibration coefficients"<<endl;
	  for ( j=0; j<nelem[i]; j++ )
	    {
	      outdbfile[i]<<Form("%*.5f",12,oldamp[i][j]);
	      if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
	    }
	  outdbfile[i]<<endl;
	}
      /*}}}*/
    }
  else
    {
      /*Amp{{{*/
      Double_t Dipole;
      Double_t P0=gCal_P0(T_Tree,Arm,P,Dipole)->Value*1000;//GeV To MeV
      cuts=Form("%s.tr.n==1",Arm.Data());
      for ( i=0; i<NCALO; i++ )
	{
	  cuts+=Form("&&%s.%s.nclust==1",Arm.Data(),calo_name[i].Data());
	}
      cuts+=Form("&&(%s.evtypebits&(1<<%d))>0",DECAT_Name.Data(),Main_Trigger);
      cuts+=Form("&&%s.cer.asum_c>300",Arm.Data());
      cuts+=Form("&&%s.vdc.u1.nclust==1&&%s.vdc.u2.nclust==1&&%s.vdc.v1.nclust==1&&%s.vdc.v2.nclust==1",Arm.Data(),Arm.Data(),Arm.Data(),Arm.Data());

      /*Draw Calibration_Check{{{*/
      /*E_over_P{{{*/
      epsfilename=Form("%s/%d_%s_Calo_Amp_Check.eps",BaseDir.Data(),RunNo,Arm.Data());
      c=new TCanvas(Form("c_%s_Calo_E_over_P",Arm.Data()),Form("c_%s_Calo_E_over_P",Arm.Data()));
      h1=new TH1F(Form("h1_%s_Calo_EVP",Arm.Data()),Form("h1_%s_Calo_EVP",Arm.Data()),100,0.1,2);
      TString h1name;
      for ( i=0; i<NCALO; i++ )
	{
	  if ( i==0 )
	    h1name=Form("%s.%s.e",Arm.Data(),calo_name[i].Data());
	  else
	    h1name+=Form("+%s.%s.e",Arm.Data(),calo_name[i].Data());
	}
      //Fix me
      //cuts+=Form("&&%s/%f/(1+%s.gold.dp)>0.5",h1name.Data(),P0,Arm.Data());
      //cuts+=Form("&&abs(L.XGT2Box2.trx+0.075873)<=0.321",h1name.Data(),P0,Arm.Data());


      T_Tree->Draw(Form("(%s)/%f/(1+%s.gold.dp)>>h1_%s_Calo_EVP",h1name.Data(),P0,Arm.Data(),Arm.Data()),cuts.Data());
      h1->SetTitle(Form("%d (%s)/P",RunNo,h1name.Data()));
      h1->GetXaxis()->SetTitle(Form("(%s)/%f/(1+%s.gold.dp)",h1name.Data(),P0,Arm.Data()));
      h1->GetXaxis()->CenterTitle();

      height=h1->GetBinContent(h1->GetMaximumBin());
      center=h1->GetBinCenter(h1->GetMaximumBin());
      h1->Fit("gaus","Q","",center-0.2,center+0.2);
      f1=h1->GetFunction("gaus");
      center=f1->GetParameter(1);

      n0=new TLine(1.,0,1.,height);
      n0->SetLineColor(kBlue);
      n0->Draw("same");

      n1=new TLine(center,0,center,height);
      n1->SetLineColor(kRed);
      n1->Draw("same");

      leg=new TLegend(0.1,0.7,0.3,0.9);
      leg->Clear();
      leg->AddEntry(n0,Form("theory peak"),"L");
      leg->AddEntry(n1,Form("gaus peak"),"L");
      leg->Draw();
      c->Print(Form("%s",epsfilename.Data()));
      /*}}}*/

      if ( IsDraw_a_p )
	{
	  /*a check{{{*/
	  for ( i=0; i<NCALO; i++ )
	    {
	      epsfilename=Form("%s/%d_%s_a_p.eps",BaseDir.Data(),RunNo,calo_name[i].Data());
	      c=new TCanvas(Form("c_%s_a",calo_name[i].Data()),Form("c_%s_a",calo_name[i].Data()));
	      c->Divide(cx,cy);
	      for ( j=0; j<nelem[i]; j++ )
		{
		  if ( j%(cx*cy)==0 )
		    {
		      c->Clear();
		      c->Divide(cx,cy);
		    }
		  c->cd(j%(cx*cy)+1);

		  h1name=Form("h1_%s_a_p[%d]",calo_name[i].Data(),j);

		  h1=new TH1F(h1name.Data(),Form("h1_%s_a_p[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,50,5000);
		  T_Tree->Draw(Form("%s.%s.a_p[%d]>>%s",Arm.Data(),calo_name[i].Data(),j,h1name.Data()),cuts.Data());
		  //center=h1->GetMean();
		  //center=h1->GetBinCenter(h1->GetMaximumBin());
		  //delete h1;

		  //if ( 4*center<=(center/2.+oldped[i][j]) )
		  //	center=500;
		  //h1=new TH1F(h1name.Data(),Form("h1_%s_a[%d] for Run %d",calo_name[i].Data(),j,RunNo),100,center/2.+oldped[i][j],4*center);
		  //T_Tree->Draw(Form("%s.%s.a[%d]>>%s",Arm.Data(),calo_name[i].Data(),j,h1name.Data()),cuts.Data());
		  h1->SetTitle(Form("%d %s.a_p[%d]",RunNo,calo_name[i].Data(),j));
		  h1->GetXaxis()->SetTitle(Form("%s.a_p[%d]",calo_name[i].Data(),j));
		  h1->GetXaxis()->CenterTitle();
		  center=h1->GetBinCenter(h1->GetMaximumBin());

		  if ( center>(50+30) )
		    {
		      h1->Fit("gaus","Q","",center-80,center+80);
		      f1=h1->GetFunction("gaus");
		      height=f1->GetParameter(1);
		      if ( fabs(center-height)<100 )
			{
			  center=height+f1->GetParameter(2);//one sigma
			  f1->SetLineColor(kRed);
			}
		      Printf("For %s a_p[%d], the ped seems to be %.5f instead of %.5f. Difference is %.5f. I will save it.",calo_name[i].Data(),j,oldped[i][j]+center,oldped[i][j],center);
		      newped[i][j]=oldped[i][j]+center;
		      IsSuggestPed=kTRUE;
		    }


		  ci=(j+1)%(cx*cy);
		  if ( j==15 && nelem[i]>16 )
		    c->Print(Form("%s(",epsfilename.Data()));
		  else if ( ci==0 && j!=(nelem[i]-1))
		    {
		      c->Print(Form("%s",epsfilename.Data()));
		    }
		  else if ( j==(nelem[i]-1) )
		    c->Print(Form("%s)",epsfilename.Data()));
		}
	    }
	  /*}}}*/
	}

      /*}}}*/
      if ( IsRunMinuit )
	{
	  /*Load nblk a_p p{{{*/
	  Printf("Loading values from %s",T_Tree_Name.Data());
	  Int_t nentries=T_Tree->GetEntries();
	  n=T_Tree->GetEntries(cuts.Data());
	  T_Tree->SetNotify(0);

	  Double_t** thisnblk=new Double_t*[NCALO];
	  Double_t** thisa_p=new Double_t*[NCALO];
	  Double_t thisdp;
	  Int_t* Ndata_thisnblk=new Int_t[NCALO];

	  for ( i=0; i<NCALO; i++ )
	    {
	      thisnblk[i]=new Double_t[MAXNBLK];
	      thisa_p[i]=new Double_t[nelem[i]];
	      T_Tree->SetBranchAddress(Form("Ndata.%s.%s.nblk",Arm.Data(),calo_name[i].Data()),&Ndata_thisnblk[i]);
	      T_Tree->SetBranchAddress(Form("%s.%s.nblk",Arm.Data(),calo_name[i].Data()),thisnblk[i]);
	      T_Tree->SetBranchAddress(Form("%s.%s.a_p",Arm.Data(),calo_name[i].Data()),thisa_p[i]);
	    }
	  T_Tree->SetBranchAddress(Form("%s.gold.dp",Arm.Data()),&thisdp);

	  Double_t** thisa_c=new Double_t*[NCALO];
	  for ( i=0; i<NCALO; i++ )
	    {
	      thisa_c[i]=new Double_t[nelem[i]];
	      T_Tree->SetBranchAddress(Form("%s.%s.a_c",Arm.Data(),calo_name[i].Data()),thisa_c[i]);
	    }


	  nblk=new Int_t**[n];
	  Ndata_nblk=new Int_t*[n];
	  a_p=new Double_t**[n];
	  p=new Double_t[n];

	  Int_t tmpblksize=0;
	  Double_t tmpnblk[MAXNBLK];
	  Double_t tmpa_p[MAXNBLK];

	  TTreeFormula ttf("selection",cuts.Data(),T_Tree);
	  Int_t numofvalid=0;
	  for ( i=0; i<nentries; i++ )
	    {
	      T_Tree->LoadTree(i);
	      ttf.UpdateFormulaLeaves();
	      if ( ttf.EvalInstance()>0 )
		{
		  if ( numofvalid%TMath::Nint(nentries/5.)==0 || numofvalid==(n-1) ) Printf("Valid Entry %6d",numofvalid+1);
		  nblk[numofvalid]=new Int_t*[NCALO];
		  Ndata_nblk[numofvalid]=new Int_t[NCALO];
		  a_p[numofvalid]=new Double_t*[NCALO];

		  T_Tree->GetEntry(i);
		  p[numofvalid]=P0*(1+thisdp);
		  for ( j=0; j<NCALO; j++ )
		    {
		      tmpblksize=0;

		      for ( k=0; k<Ndata_thisnblk[j]; k++ )
			{
			  if ( fabs(thisa_p[j][(Int_t)thisnblk[j][k]])>1e-10 )
			    {
			      tmpnblk[tmpblksize]=(Int_t)thisnblk[j][k];
			      tmpa_p[tmpblksize]=thisa_p[j][(Int_t)thisnblk[j][k]];
			      tmpblksize++;
			    }
			}
		      Ndata_nblk[numofvalid][j]=tmpblksize;

		      nblk[numofvalid][j]=new Int_t[tmpblksize];
		      a_p[numofvalid][j]=new Double_t[tmpblksize];
		      for ( k=0; k<tmpblksize; k++ )
			{
			  nblk[numofvalid][j][k]=tmpnblk[k];
			  a_p[numofvalid][j][k]=tmpa_p[k];
			  //Printf("nblk[%d][%d][%d]=%d,a_p[%d][%d][%d]=%.3e",numofvalid,j,k,nblk[numofvalid][j][k],numofvalid,j,k,a_p[numofvalid][j][k]);
			  //if ( thisa_c[j][(Int_t)thisnblk[j][k]]!=0 )
			  //	Printf("thisa_p[%d][%d]=%.3f,thisa_c[%d][%d]=%.3f,coef=%.3f",j,(Int_t)thisnblk[j][k],thisa_p[j][(Int_t)thisnblk[j][k]],j,(Int_t)thisnblk[j][k],thisa_c[j][(Int_t)thisnblk[j][k]],thisa_c[j][(Int_t)thisnblk[j][k]]/thisa_p[j][(Int_t)thisnblk[j][k]]);
			}
		    }
		  numofvalid++;
		}
	    }
	  for ( i=0; i<NCALO; i++ )
	    {
	      delete thisnblk[i];
	      delete thisa_p[i];
	      delete thisa_c[i];
	    }
	  delete thisnblk;
	  delete thisa_p;
	  delete thisa_c;
	  delete Ndata_thisnblk;
	  /*}}}*/
	  /*Fit and Get newamp{{{*/
	  Printf("Trying to minimize with MINUIT Package");
	  Bool_t* IsParFixed;
	  Int_t pmt;
	  Int_t totalelem=0;

	  for ( i=0; i<NCALO; i++ )
	    totalelem+=nelem[i];
	  IsParFixed=new Bool_t[totalelem];
	  for ( i=0; i<totalelem; i++ )
	    IsParFixed[i]=kTRUE;
	  for ( i=0; i<n; i++ )
	    {
	      for ( j=0; j<NCALO; j++ )
		{
		  for ( k=0; k<Ndata_nblk[i][j]; k++ )
		    {
		      //Printf("nblk[%d][%d][%d]=%d,a_p[%d][%d][%d]=%.3e",i,j,k,nblk[i][j][k],i,j,k,a_p[i][j][k]);
		      pmt=0;
		      for ( int jj=0; jj<j; jj++ )
			pmt += nelem[jj];
		      pmt+=nblk[i][j][k];
		      IsParFixed[pmt]=kFALSE;
		    }
		}
	    }
	  for ( i=0; i<BadBlocks.size(); i++ )
	    IsParFixed[BadBlocks[i]]=kTRUE;

	  cout<<"Fix";
	  for ( i=0; i<totalelem; i++ )
	    {
	      if ( IsParFixed[i] )
		cout<<Form(" %d",i+1);
	    }
	  cout<<endl;

	  Int_t npar=0;
	  for ( i=0; i<NCALO; i++ )
	    {
	      npar+=nelem[i];
	    }
	  TMinuit* minuit =new TMinuit(npar);
	  Double_t gin[100];
	  for ( int i=0; i<100; i++ )
	    {
	      gin[i]=i;
	    }
	  minuit->SetFCN(calc_chi_square);
	  minuit->SetPrintLevel(-1);

	  Double_t arglist[10];
	  Int_t ierflg=0;
	  arglist[0] = 1;
	  minuit->mnexcm("SET ERR",arglist,1,ierflg);
	  Double_t maxlimit;
	  if ( Arm=="L" )
	    maxlimit=10;
	  else
	    maxlimit=2;
	  for ( i=0; i<NCALO; i++ )
	    {
	      for ( j=0; j<nelem[i]; j++ )
		{
		  pmt=0;
		  for ( int jj=0; jj<i; jj++ )
		    pmt += nelem[jj];
		  pmt+=j;

		  minuit->mnparm(pmt, Form("%s_%d_%d",calo_name[i].Data(),i,j), oldamp[i][j], 1e-4, 1e-3,maxlimit,ierflg);
		}
	    }
	  for ( j=0; j<totalelem; j++ )
	    {
	      minuit->FixParameter(j);
	    }

	  vector<Int_t> ClustBlocks;
	  Int_t bi;
	  Bool_t IsPushBack_pmt;

	  Int_t entry_i,det_j,blk_k;
	  Int_t key[2],row,col;
	  if ( calo_name[0].Contains("XGT2Box") )
	    {
	      key[0]=ncols[0];
	      key[1]=nrows[0];
	    }
	  else if ( calo_name[0]=="sh" )
	    { 
	      key[0]=nrows[0];
	      key[1]=ncols[0];
	    }
	  //first det is main part
	  for ( k=0; k<nelem[0]; k++ )
	    {
	      //XGT2Box 9cx10r
	      //0 1 2 3 4 5 6 7 8
	      //9....
	      //shower 5cx16r c=col r=row
	      //		16	0
	      //		17	1
	      //		18	2
	      //		19	3
	      //		20	4
	      //		21	5
	      //		22	6
	      //....

	      //if ( (k%key[0])!=0 &&  ((k+1)%key[0])!=0 && ((Int_t)(k/key[0]))!=0 && ((Int_t)(k/key[0]))!=(key[1]-1) && !IsParFixed[k]) //do not use edge block as central block
	      if ( !IsParFixed[k]) //do not use edge block as central block
		{
		  /*Get Cluster {{{*/
		  ClustBlocks.clear();
		  cluster_n=0;
		  for ( i=0; i<n; i++ )
		    {
		      if ( nblk[i][0][0]==k )
			{
			  for ( det_j=0; det_j<NCALO; det_j++ )
			    {
			      for ( blk_k=0; blk_k<Ndata_nblk[i][det_j]; blk_k++ )
				{
				  pmt=0;
				  for ( int jj=0; jj<det_j; jj++ )
				    pmt += nelem[jj];
				  pmt+=nblk[i][det_j][blk_k];

				  IsPushBack_pmt=kTRUE;
				  for ( bi=0; bi<ClustBlocks.size(); bi++ )
				    {
				      if ( ClustBlocks[bi]==pmt )
					IsPushBack_pmt=kFALSE;
				    }
				  if ( IsPushBack_pmt )
				    ClustBlocks.push_back(pmt);
				}
			    }
			  cluster_n++;
			}
		    }
		  cluster_p=new Double_t[cluster_n];
		  cluster_Ndata_nblk=new Int_t*[cluster_n];
		  cluster_nblk=new Int_t**[cluster_n];
		  cluster_a_p=new Double_t**[cluster_n];
		  entry_i=0;
		  for ( i=0; i<n; i++ )
		    {
		      if ( nblk[i][0][0]==k )
			{
			  cluster_Ndata_nblk[entry_i]=new Int_t[NCALO];
			  cluster_nblk[entry_i]=new Int_t*[NCALO];
			  cluster_a_p[entry_i]=new Double_t*[NCALO];
			  cluster_p[entry_i]=p[i];
			  for ( det_j=0; det_j<NCALO; det_j++ )
			    {
			      cluster_Ndata_nblk[entry_i][det_j]=Ndata_nblk[i][det_j];
			      cluster_nblk[entry_i][det_j]=new Int_t[cluster_Ndata_nblk[entry_i][det_j]];
			      cluster_a_p[entry_i][det_j]=new Double_t[cluster_Ndata_nblk[entry_i][det_j]];
			      for ( blk_k=0; blk_k<cluster_Ndata_nblk[entry_i][det_j]; blk_k++ )
				{
				  pmt=0;
				  for ( int jj=0; jj<det_j; jj++ )
				    pmt += nelem[jj];
				  pmt+=nblk[i][det_j][blk_k];

				  cluster_nblk[entry_i][det_j][blk_k]=pmt;
				  cluster_a_p[entry_i][det_j][blk_k]=a_p[i][det_j][blk_k];
				}
			    }
			  entry_i++;
			}
		    }
		  for ( bi=0; bi<ClustBlocks.size(); bi++ )
		    {
		      pmt=ClustBlocks[bi];
		      if ( !IsParFixed[pmt] )
			minuit->Release(pmt);
		    }
		  /*}}}*/

		  arglist[0] = 500;
		  arglist[1] = 1.;
		  minuit->mnexcm("MIGRAD",arglist,2,ierflg);

		  for ( bi=0; bi<ClustBlocks.size(); bi++ )
		    {
		      pmt=ClustBlocks[bi];
		      if ( bi==0 )
			IsParFixed[pmt]=kTRUE;//fix the block which has been calibrated.
		      minuit->FixParameter(pmt);
		    }

		  /*Cleanup{{{*/
		  for ( entry_i=0; entry_i<cluster_n; entry_i++ )
		    {
		      for ( det_j=0; det_j<NCALO; det_j++ )
			{
			  delete cluster_nblk[entry_i][det_j];
			  delete cluster_a_p[entry_i][det_j];
			}
		      delete cluster_nblk[entry_i];
		      delete cluster_a_p[entry_i];
		      delete cluster_Ndata_nblk[entry_i];
		    }
		  delete cluster_nblk;
		  delete cluster_a_p;
		  delete cluster_Ndata_nblk;
		  delete cluster_p;
		  /*}}}*/
		  Printf("%d/%d has been calibrated.",k+1,nelem[0]);
		}
	      else
		Printf("Ignore %d/%d since it's at the edge.",k+1,nelem[0]);
	    }

	  // if you want to access to these parameters, use:
	  Double_t amin,edm,errdef;
	  Int_t nvpar,nparx,icstat;
	  minuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
	  //void mnstat(Double_t &fmin, Double_t &fedm, Double_t &errdef, Int_t &npari, Int_t &nparx, Int_t &istat)
	  //*-*-*-*-*Returns concerning the current status of the minimization*-*-*-*-*
	  //*-*      =========================================================
	  //*-*       User-called
	  //*-*          Namely, it returns:
	  //*-*        FMIN: the best function value found so far
	  //*-*        FEDM: the estimated vertical distance remaining to minimum
	  //*-*        ERRDEF: the value of UP defining parameter uncertainties
	  //*-*        NPARI: the number of currently variable parameters
	  //*-*        NPARX: the highest (external) parameter number defined by user
	  //*-*        ISTAT: a status integer indicating how good is the covariance
	  //*-*           matrix:  0= not calculated at all
	  //*-*                    1= approximation only, not accurate
	  //*-*                    2= full matrix, but forced positive-definite
	  //*-*                    3= full accurate covariance matrix
	  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
	  cout << "\n";
	  cout << " Minimum chi square = " << amin << "\n";
	  cout << " Estimated vert. distance to min. = " << edm << "\n";
	  cout << " Number of variable parameters = " << nvpar << "\n";
	  cout << " Highest number of parameters defined by user = " << nparx << "\n";
	  cout << " Status of covariance matrix = " << icstat << "\n";
	  Double_t err;
	  for ( i=0; i<NCALO; i++ )
	    {
	      for ( j=0; j<nelem[i]; j++ )
		{
		  pmt=0;
		  for ( int jj=0; jj<i; jj++ )
		    pmt += nelem[jj];
		  pmt+=j;

		  minuit->GetParameter(pmt,newamp[i][j],err);
		}
	    }
	  /*}}}*/
	  /*Save to outdbfile{{{*/
	  for ( i=0; i<NCALO; i++ )
	    {
	      outdbfile[i]<<"ADC pedestals"<<endl;
	      for ( j=0; j<nelem[i]; j++ )
		{
		  outdbfile[i]<<Form("%*.0f",6,oldped[i][j]);
		  if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
		}
	      outdbfile[i]<<endl;
	      outdbfile[i]<<"Energy calibration coefficients"<<endl;
	      for ( j=0; j<nelem[i]; j++ )
		{
		  outdbfile[i]<<Form("%*.5f",12,newamp[i][j]);
		  if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
		}
	      outdbfile[i]<<endl;
	    }
	  /*}}}*/
	  /*Cleanup{{{*/
	  for ( i=0; i<n; i++ )
	    {
	      delete Ndata_nblk[i];
	      for ( j=0; j<NCALO; j++ )
		{
		  delete nblk[i][j];
		  delete a_p[i][j];
		}
	      delete nblk[i];
	      delete a_p[i];
	    }
	  delete nblk;
	  delete a_p;
	  delete Ndata_nblk;
	  delete p;
	  delete minuit;
	  /*}}}*/
	}
      else
	{
	  /*Save to outdbfile{{{*/
	  for ( i=0; i<NCALO; i++ )
	    {
	      outdbfile[i]<<"ADC pedestals"<<endl;
	      for ( j=0; j<nelem[i]; j++ )
		{
		  outdbfile[i]<<Form("%*.0f",6,oldped[i][j]);
		  if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
		}
	      outdbfile[i]<<endl;
	      outdbfile[i]<<"Energy calibration coefficients"<<endl;
	      for ( j=0; j<nelem[i]; j++ )
		{
		  outdbfile[i]<<Form("%*.5f",12,oldamp[i][j]);
		  if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
		}
	      outdbfile[i]<<endl;
	    }
	  /*}}}*/
	}
      /*}}}*/
    }
  /*Save the rest{{{*/
  for ( i=0; i<NCALO; i++ )
    {
      while ( comment.ReadLine(dbfile[i]) )
	{
	  outdbfile[i]<<comment<<endl;
	  if ( !dbfile[i].good() )
	    break;
	}
      dbfile[i].close();

      if ( IsSuggestPed )
	{
	  outdbfile[i]<<Form("[ config = %d ]",RunNo)<<endl;
	  outdbfile[i]<<"ADC pedestals"<<endl;
	  for ( j=0; j<nelem[i]; j++ )
	    {
	      outdbfile[i]<<Form("%*.0f",6,newped[i][j]);
	      if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
	    }
	  outdbfile[i]<<endl;
	  outdbfile[i]<<"Energy calibration coefficients"<<endl;
	  for ( j=0; j<nelem[i]; j++ )
	    {
	      outdbfile[i]<<Form("%*.5f",12,oldamp[i][j]);
	      if ( (j+1)%6==0 && j!=(nelem[i]-1) ) outdbfile[i]<<endl;
	    }
	  outdbfile[i]<<endl;
	}
      outdbfile[i].close();
    }
  /*}}}*/
  /*Generate cptoDB.sh{{{*/
  Printf("Use following command to copy new db to $DB_DIR");
  for ( i=0; i<NCALO; i++ )
    {
      TString cpbashcmd=Form("cp %s %s",outDB_FileName[i].Data(),DB_FileName[i].Data());
      Printf(cpbashcmd.Data());

      TString cpbashfilename=Form("%s/cp%s%stoDB.sh",BaseDir.Data(),Arm.Data(),calo_name[i].Data());
      ofstream cpbashfile(cpbashfilename.Data());
      cpbashfile<<"#!/bin/sh"<<endl<<endl;
      cpbashfile<<cpbashcmd<<endl;
      cpbashfile.close();
      gSystem->Exec(Form("chmod u+x %s",cpbashfilename.Data()));
    }
  /*}}}*/
  /*Generate apply_newdb.sh and restore_olddb.sh{{{*/
  TString apply_newdbfilename=Form("%s/apply_newdb.sh",BaseDir.Data());
  ofstream apply_newdbfile(apply_newdbfilename.Data());
  apply_newdbfile<<"#!/bin/sh"<<endl<<endl;
  apply_newdbfile<<"for command in `ls cp*toDB.sh`"<<endl;
  apply_newdbfile<<"do"<<endl;
  apply_newdbfile<<"	echo \"./$command\""<<endl;
  apply_newdbfile<<"	./$command"<<endl;
  apply_newdbfile<<"done"<<endl;
  apply_newdbfile.close();
  gSystem->Exec(Form("chmod u+x %s",apply_newdbfilename.Data()));

  TString restore_olddbfilename=Form("%s/restore_olddb.sh",BaseDir.Data());
  ofstream restore_olddbfile(restore_olddbfilename.Data());
  restore_olddbfile<<"#!/bin/sh"<<endl<<endl;
  restore_olddbfile<<"for olddb in `ls old_db_*`"<<endl;
  restore_olddbfile<<"do"<<endl;
  restore_olddbfile<<"	db=${olddb#old_}"<<endl;
  restore_olddbfile<<"	echo \"cp $olddb $db\""<<endl;
  restore_olddbfile<<"	cp $olddb $db"<<endl;
  restore_olddbfile<<"done"<<endl;
  restore_olddbfile<<""<<endl;
  restore_olddbfile<<"for command in `ls cp*toDB.sh`"<<endl;
  restore_olddbfile<<"do"<<endl;
  restore_olddbfile<<"	echo \"./$command\""<<endl;
  restore_olddbfile<<"	./$command"<<endl;
  restore_olddbfile<<"done"<<endl;
  restore_olddbfile.close();
  gSystem->Exec(Form("chmod u+x %s",restore_olddbfilename.Data()));
  /*}}}*/

  /*Cleanup{{{*/
  for ( i=0; i<NCALO; i++ )
    {
      delete oldped[i];
      delete newped[i];
      delete oldamp[i];
      delete newamp[i];
      delete pos[i];
      delete first_block_pos[i];
      delete space[i];
    }
  delete pos;
  delete first_block_pos;
  delete space;
  delete oldped;
  delete newped;
  delete oldamp;
  delete newamp;
  delete ncols;
  delete nrows;
  delete nelem;
  /*}}}*/

  printf("Time = %f seconds\n",timer.CpuTime());

  log->WriteToLogFile(__FILE__,__LINE__,Form("%d Success. Result is saved at %s",RunNo,BaseDir.Data()));
  log->Destroy();
  return 0;
}
/*}}}*/
