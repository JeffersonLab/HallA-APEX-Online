
#include "TriBCM.h"
//#include "THaGlobals.h"
#include "THaVarList.h"
#include "THaVar.h"
//#include "VarDef.h"
//#include "VarType.h"
//#include "THaAnalyzer.h"
//#include "TList.h"
//#include "THaEvData.h"
//#include "TDatime.h"
//#include "../TriScalerEvtHandler/TriScalerEvtHandler.h"

#include "TString.h"



#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


//_____________________________________________________________________________
TriBCM::TriBCM( const char* name, const char* descript, const char* arm1, const char* scaler1, Int_t Debug) :
  THaPhysicsModule( name, descript ), //, TriScalerEvtHandler( name, descript )
	debug(Debug), scaler(scaler1), arm(arm1)
{
 // Normal constructor.
}
  //bcm_u1=0; bcm_d1=0; bcm_d3=0; bcm_d10=0; bcm_unew=0; bcm_dnew=0;
  //charge_u1=0; charge_d1=0; charge_d3=0; charge_d10=0; charge_unew=0; charge_dnew=0;
//_____________________________________________________________________________


TriBCM::~TriBCM()
{
  // Destructor
  DefineVariables( kDelete ); 

}


//_____________________________________________________________________________
Int_t TriBCM::ReadDatabase( const TDatime& date) {

   // static const char* const here = "ReadDatabase()";
	

    FILE *file = OpenFile( date );
    if (!file) return kFileError;



    const DBRequest calib_request[] = {
        { "u1.gain",      &gain[0],   kDouble, 0 , 1 },
        { "u1.offset",    &off[0],    kDouble, 0 , 1},
        { "u3.gain",      &gain[1],   kDouble, 0 , 1},
        { "u3.offset",    &off[1],    kDouble, 0 , 1},
        { "u10.gain",     &gain[2],  kDouble, 0 , 1},
        { "u10.offset",   &off[2],   kDouble, 0 , 1},
        { "unew.gain",    &gain[3], kDouble, 0 , 1},
        { "unew.offset",  &off[3],  kDouble, 0 , 1},
        { "d1.gain",      &gain[4],   kDouble, 0 , 1},
        { "d1.offset",    &off[4],    kDouble, 0 , 1},
        { "d3.gain",      &gain[5],   kDouble, 0 , 1},
        { "d3.offset",    &off[5],    kDouble, 0 , 1},
        { "d10.gain",     &gain[6],  kDouble, 0 , 1},
        { "d10.offset",   &off[6],   kDouble, 0 , 1},
        { "dnew.gain",    &gain[7], kDouble, 0 , 1},
        { "dnew.offset",  &off[7],  kDouble, 0 , 1},
        { 0 }
    };

  Int_t err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;
	
	
	cout << "dnew offest " << off[7]<<endl;
	
  return kOK;
}


//_____________________________________________________________________________
Int_t TriBCM::DefineVariables( EMode mode )
{
  
  // Initialize global variables

 // if( mode == kDefine && fIsSetup ) return kOK;
 // fIsSetup = ( mode == kDefine );

  // Register variables in global list
 // fIsSetup = ( mode == kDefine );

  RVarDef vars[] = {

    { "current_u1",    "Beam current from bcm u1",           "current[0]" },  
    { "current_u3",    "Beam current from bcm u3",           "current[1]" },  
    { "current_u10",   "Beam current from bcm u10",          "current[2]" }, 
    { "current_unew",  "Beam current from bcm unew",         "current[3]" }, 
    { "current_d1",    "Beam current from bcm d1",           "current[4]" },  
    { "current_d3",    "Beam current from bcm d3",           "current[5]" },  
    { "current_d10",   "Beam current from bcm d10",          "current[6]" },  
    { "current_dnew",  "Beam current from bcm dnew",         "current[7]" },  
    { "charge_u1",     "Integrated charge from bcm u1",      "charge[0]" },  
    { "charge_u3",     "Integrated charge from bcm u3",      "charge[1]" },  
    { "charge_u10",    "Integrated charge from bcm u10",     "charge[2]" }, 
    { "charge_unew",   "Integrated charge from bcm unew",    "charge[3]" }, 
    { "charge_d1",     "Integrated charge from bcm d1",      "charge[4]" },  
    { "charge_d3",     "Integrated charge from bcm d3",      "charge[5]" },  
    { "charge_d10",    "Integrated charge from bcm d10",     "charge[6]" },  
    { "charge_dnew",   "Integrated charge from bcm dnew",    "charge[7]" },
    { "total_charge",  "Total Charge averaged over all bcms","total_charge"},
    { "average_current","Current average over all bcms",     "avg_current" },  
    { "isrenewed",     "Scaler reading updated or not",      "isrenewed"},
    { 0 }
  };

  
  return DefineVarsFromList( vars, mode );
}
//_______________________________________________________________________________
/*Int_t SetInputModule( const char* arm1, const char* scaler1, const char* bcm_name2[] )
{


//	TString bcm_name[8];
	TString bname[8] ={"_u1","_u3","_u10","_unew","_d1","_d3","_d10","_dnew"};
	for(int i =0; i<8;i++){
		bcm_name2[i] = TString::Format("%s%s%s",arm1,scaler1,bname[i].Data());
	  }

 return 0;
}
*/
//_____________________________________________________________________________
Int_t TriBCM::Process(const THaEvData& evdata)
{
	bcm_u1=0; bcm_d1=0; bcm_d3=0; bcm_d10=0; bcm_unew=0; bcm_dnew=0;
    charge_u1=0; charge_d1=0; charge_d3=0; charge_d10=0; charge_unew=0; charge_dnew=0;
	
	//debug = 10; //Hard coded debug flag!!!!!
	
	THaVar* var[8];
	THaVar* Rate[8];

//Needs to be arm switchable and scaler switchable
	TString bcm_name[8];// = {"evLeftu1", "evLeftu3", "evLeftu10","evLeftunew","evLeftd1", "evLeftd3", "evLeftd10","evLeftdnew"};
//	TString bcm_name_R[8] = {"evLeftu1_r", "evLeftu3_r", "evLeftu10_r","evLeftunew_r","evLeftd1_r", "evLeftd3_r", "evLeftd10_r","evLeftdnew_r"};
////////////////////////////////////////////////


	TString bname[8] ={"u1","u3","u10","unew","d1","d3","d10","dnew"};
	for(int i =0; i<8;i++){
		bcm_name[i] = TString::Format("%s%s%s",scaler.Data(),arm.Data(),bname[i].Data());
		
		bcm_name_R[i] = TString::Format("%s%s%s_r",scaler.Data(),arm.Data(),bname[i].Data());
		//cout << bcm_name[i].Data() <<endl;
	  }



//needs to arm and scalar switchable
	double clock_freq=  103700; //1024
///////////////////////////////////////

	double bcms[8]={0};
	double bcms_diff[8] ={0};
	double time_diff=0;
	double bcms_R[8]={0};

//needs to be arm and scalar switchable 
	THaVar* clock_Count_pt = gHaVars->Find(Form("%s%sLclock",scaler.Data(),arm.Data()));
	THaVar* clock_Rate_pt  = gHaVars->Find(Form("%s%sLclock_r",scaler.Data(),arm.Data()));
///////////////////////////////////////////
	
	///Defining if this is new scalar event.
	isrenewed=0;
	
	
	if(clock_Count_pt !=0){ clock_count_new= clock_Count_pt->GetValue(); }
	if(clock_count_new!=clock_count_old){isrenewed=1;}
		
	double time_sec =0;
	if(clock_Count_pt !=0 && clock_Rate_pt!=0){
		if(isrenewed){
	  time_diff = (clock_count_new - clock_count_old);
	  time_sec  =  time_diff/clock_freq;
	if(debug==1||debug==10){
	  cout << "time_sec  "<<time_sec<<"  total "<<clock_Count_pt->GetValue()/clock_freq<< endl;
	    }
	  time_diff = (clock_count_new - clock_count_old)/(clock_Rate_pt->GetValue()*1.0);
	  }}    
	
	clock_count_old=clock_count_new;
	double total_charge_event=0;
	double average_current_event=0;
	count=0;
	for(int i =0;i<8;i++){
//Retrieve info from the Vars list::::		
		TString varname = bcm_name[i];
		var[i] = gHaVars->Find(varname.Data());
		TString varnameR = bcm_name_R[i];
		Rate[i] = gHaVars->Find(varnameR.Data());

		//cout << var[i]<<endl;
		//cout << varname.Data()<<endl;
		
		if(var[i]!=0){
			bcms[i] = var[i]->GetValue();
			bcms_R[i] = Rate[i]->GetValue();

// Calculate the charge and current if scalar is renewed
			if(bcms[i]!=0 && bcm_old[i]!=bcms[i] && isrenewed){			
				count++;
				bcms_diff[i] = bcms[i]-bcm_old[i];
				total_charge_event+=bcms[i]*gain[i] + off[i]*time_sec;
				average_current_event+= bcms_R[i]*gain[i]+ off[i];
				
				charge[i] = bcms_diff[i]*gain[i] + off[i]*time_sec;
				current[i] =bcms_R[i]*gain[i] + off[i];
				}
 	 		bcm_old[i]=bcms[i];
		}

	}
	if(debug==10&& isrenewed){
		cout << "bcm_unew qe" << " " <<charge[3]<<"\t";
		cout << "bcm_unew I" << " " <<current[3]<<endl;
		cout << "bcm_cnew qe" << " " <<charge[7]<<"\t";
		cout << "bcm_new I" << " " <<current[7]<<endl;
	}
	
/* Work in progress!!!!		
	if(isrenewed){
		total_charge+=total_charge_event/(count);
		avg_current=average_current_event/(count);
		if(debug==10&& isrenewed){
			cout <<"tot eq "<<total_charge << "| avg I "<<avg_current <<endl;
		}
	}
*/
		
 return 0;
}
//_____________________________________________________________________________



//ClassImp(TriBCM)
