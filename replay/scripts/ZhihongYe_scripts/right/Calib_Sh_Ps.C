#include "Calib_Sh_Ps.hh"

//////////////////////////////////////////////////////////////
/////                  Main Program                     //////
//////////////////////////////////////////////////////////////

//void offsetfit()
int main()
{  

  cout << "*************************************************" << endl; 
  cout << "*       Shower & PreShower Calibration          *" << endl;
  cout << "*            for x>2 experimetn                 *" << endl;
  cout << "*             Z. Ye 04/24/2011                  *" << endl;
  cout << "*************************************************" << endl; 
  cout << endl;
  
  gSystem->Load("libMinuit");

  TMinuit *pMinuit = new TMinuit(iParaNum);  //initialize TMinuit with a maximum of iParaNum params
  pMinuit->SetFCN(myFcn);

  Double_t arglist[10];
  Int_t ierflg = 0;

  /// set print level
  /*  SET PRIntout  <level>
      Sets the print level, determining how much output will be
      produced. Allowed values and their meanings are displayed
      after a SHOw PRInt command, and are currently <level>=:
       [-1]  no output except from SHOW commands
       [0]  minimum output
       [1]  default value, normal output
       [2]  additional output giving intermediate results.
       [3]  maximum output, showing progress of minimizations. */
  arglist[0] = 1;
  pMinuit->mnexcm("SET PRIntout",arglist,1,ierflg);

  /*
    SET NOWarnings
    Supresses Minuit warning messages.
    SET WARnings
    Instructs Minuit to output warning messages when suspicious
    conditions arise which may indicate unreliable results.
    This is the default.
  */
  arglist[0] = 0;
  pMinuit->mnexcm("SET NOWarnings",arglist,0,ierflg);

  //Initializing Parameters
  for(int ii=0;ii<iParaNum;ii++)
    {
      pMinuit->DefineParameter(ii,namePar[ii], 
			       inPar[ii], stepSize, minVal[ii], maxVal[ii]); }
//Fix values of  Parameters
  //arglist[0] = 1;  pMinuit->mnexcm("FIX ", arglist ,1,ierflg);
    
  
  //  arglist[0] = 1;
  //  pMinuit->mnexcm("SET ERR", arglist,1,ierflg);
  /*
  //SET LIMits  0->[parno]  1->[lolim]  2->[uplim]
  //	  arglist[0] = 0;	   //this means set all parameters with the same limit
  //	  arglist[0] = 1;	   //this means set 1st parameters with the specified limit
  //	  arglist[0] = 2;	   //this means set 2nd parameters with the sepecified limit
  arglist[0] = 0;
  arglist[1] = 0.;
  arglist[2] = 0.;
  pMinuit->mnexcm("SET LIMits",arglist,3,ierflg);
  */

 //Set Iteration Times and Use Migrad to start minimization
  arglist[0] = 1500;

  arglist[1] = 1.;
  pMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  //Get Results
  Double_t outPar[iParaNum], err[iParaNum];
  for(int ii = 0; ii < iParaNum; ii++)
    {
      pMinuit->GetParameter(ii,outPar[ii],err[ii]);    
      
      for(ii==0)
	fpar <<"#PreShower"<<endl;
      for(ii=48)
      	fpar <<"#Shower"<<endl;

      fpar <<" "<<outPar[ii]<<"   ";
      if(ii<48){
	if(!(ii%6))
	  fpar<<endl;
      }

      else{
	if(!(ii%6))
	  fpar<<endl;
      }
    }

  // free the memory
  delete pMinuit;
  Double_t result= GetChisq(outPar);
  cerr<< " >>>> Result== " << result<<endl;
}

///////////////////////////////////////////////////
//   Define Function, where only *par is used    //
///////////////////////////////////////////////////
void myFcn(Int_t& npar, Double_t* deriv, Double_t& f, Double_t *par, Int_t flag)
//void myFcn(Int_t& npar, Double_t* gin, Double_t& f, Double_t *par, Int_t flag)
{
  Double_t chisq=1.0E-6;
  chisq = GetChisq(par);
  cerr<<">>> Chisq =  "<<chisq<<endl;
  f=chisq;
}




////////////////////////////////////////////////////
///             Calculate Chi-Square  1           //
////////////////////////////////////////////////////
Double_t GetChisq(Double_t *par)
{
  Double_t adc[128], cnt[128];
  Double_t p0;
  Double_t chisq=1.0E-6;
  Int_t OneEvt, Cnt;  
  
  Double_t ADC_Shower[80], ADC_PreShower[48];
  Int_t Cnt_Shower;


  Int_t R_tr_n; 
  Double_t R_gold_th,R_gold_ph, R_gold_dp,R_cer_asum_c;
  

  T->SetBranchAddress("R.sh.a_p",   &ADC_Shower);
  T->SetBranchAddress("R.ps.a_p",   &ADC_PreShower);
 
  for(int i=0;i<nEvt;i++){
    
    T->GetEntry(i);
    //Corrected sum amp in Cherenkov is less than 3000 but more than 2500
    if(R_cer_asum_c>300&&R_cer_asum_c<6000){                 
      
      //The momentum of the track cut
      if(R_tr_n==1&&fabs(R_gold_th)<0.5&&fabs(R_gold_ph)<0.5&&fabs(R_gold_dp)<1.){
	
      }
    }
    
  }
     




  return chisq;
}

Bool_t GetCut(){
  


}
