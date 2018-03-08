
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
    DeleteArrays();

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
        { "current_cuts", c_cuts, 	kDouble, 5 , 1},
        { 0 }
    };

 	if( !fIsInit ) {    
   		BeamOn = new Double_t[ 5 ];
		BeamUp = new Double_t[ 5 ];
		BeamUp_S = new Double_t[ 5 ];
		
	}

  Int_t err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;
    cout << "Dnew offset = " << off[7] << "  Dnew gain = " << gain[7] <<endl;
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
//    { "total_charge_dnew",  "Total Charge for dnew",		 "total_charge_event[7]"},
//    { "average_current","Current average over all bcms",     "avg_current" },  
    { "isrenewed",  	"Scaler reading updated or not",      "isrenewed"},
    { "BeamUp_time_v1495[5]","Time the beam has been up in seconds","BeamUp"},
	{ "BeamUp_time_scaler[5]","Time the beam has been up in seconds","BeamUp_S"},
    { "BeamUp_events[5]","Time the beam has been up in events","BeamOn"},
    { 0 }
  };

  
  return DefineVarsFromList( vars, mode );
}
//_____________________________________________________________________________
Int_t TriBCM::Process(const THaEvData& evdata)
{
	bcm_u1=0; bcm_d1=0; bcm_d3=0; bcm_d10=0; bcm_unew=0; bcm_dnew=0;
    charge_u1=0; charge_d1=0; charge_d3=0; charge_d10=0; charge_unew=0; charge_dnew=0;
		
	THaVar* var[8];
	THaVar* Rate[8];
	TString bcm_name[8];

	TString bname[8] ={"u1","u3","u10","unew","d1","d3","d10","dnew"};
	for(int i =0; i<8;i++){
		bcm_name[i] = TString::Format("%s%s%s",scaler.Data(),arm.Data(),bname[i].Data());
		
		bcm_name_R[i] = TString::Format("%s%s%s_r",scaler.Data(),arm.Data(),bname[i].Data());
		//cout << bcm_name[i].Data() <<endl;
	  }
//needs to arm and scalar switchable
	double clock_freq;
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
		
	if(clock_Count_pt !=0){ clock_count_new= clock_Count_pt->GetValue(); 
							clock_freq = clock_Rate_pt->GetValue();}
	if(clock_count_new!=clock_count_old){isrenewed=1;}
		
	double time_sec =0;
	if(clock_Count_pt !=0 && clock_Rate_pt!=0){
		if(isrenewed){
	  		time_diff = (clock_count_new - clock_count_old);
	  		time_sec  =  time_diff/clock_freq;
//Debug statement
	if(debug==1||debug==10){
	  cout << "time_sec  "<<time_sec<<"  total "<<clock_Count_pt->GetValue()/clock_freq<< endl;
	   }
////////////////////
	  		time_diff = (clock_count_new - clock_count_old)/(clock_Rate_pt->GetValue()*1.0);
	  }}    
	
	double average_current_event=0;
	count=0;
	for(int i =0;i<8;i++){
//Retrieve info from the Vars list::::		
		TString varname = bcm_name[i];
		var[i] = gHaVars->Find(varname.Data());
		TString varnameR = bcm_name_R[i];
		Rate[i] = gHaVars->Find(varnameR.Data());
	if(debug==10&& isrenewed){
		//cout << var[i]<<endl;
		//cout << varname.Data()<<endl;
		}
		if(var[i]!=0){
			bcms[i] = var[i]->GetValue();
			bcms_R[i] = Rate[i]->GetValue();
// Calculate the charge and current if scalar is renewed
			count++;
			bcms_diff[i] = bcms[i]-bcm_old[i];
			average_current_event+= bcms_R[i]*gain[i]+ off[i];
			charge[i] = bcms_diff[i]*gain[i] + off[i]*time_sec;
			current[i] =bcms_R[i]*gain[i] + off[i];
			total_charge_event[i]+=charge[i]; //bcms[i]*gain[i] + off[i]*time_sec
 	 		bcm_old[i]=bcms[i];
		}
	}//end of bcm loop
//Debug statement 
	if(debug==10&& isrenewed){
		cout << "bcm_unew qe" << " " <<charge[3]<<"\t";
		cout << "bcm_unew I" << " " <<current[3]<<endl;
		cout << "bcm_dnew qe" << " " <<charge[7]<<"\t";
		cout << "bcm_dnew I" << " " <<current[7]<<endl;
		cout << "dnew qe ("<<gain[7]<<")  "<< total_charge_event[7]<<"  "<<bcms[7]*gain[7]<<endl;

	}
//Beam quality function/
		int qua = BeamQuality(evdata);
		if(qua){cout << "error"<<endl;}

/* Work in progress!!!!		
	if(isrenewed){
		total_charge+=total_charge_event/(count);
		avg_current=average_current_event/(count);
		if(debug==10&& isrenewed){
			cout <<"tot eq "<<total_charge << "| avg I "<<avg_current <<endl;
		}
	}

*/
//Reset for time diff.
	clock_count_old=clock_count_new;
	return 0;
}
//____________________________________________________
//  
Int_t TriBCM::BeamQuality(const THaEvData& evdata)
{
	//Retrieve info for V1495
	THaVar* V1495CC = gHaVars->Find("V1495ClockCount");
	V1495 = V1495CC->GetValue()/103700.0;
	Double_t V1495_diff = V1495 - V1495_old;
	
	//Retreive info for the scaler clock;
	THaVar* cc_pt = gHaVars->Find(Form("%s%sLclock",scaler.Data(),arm.Data()));
	THaVar* cr_pt  = gHaVars->Find(Form("%s%sLclock_r",scaler.Data(),arm.Data()));
	
	//Make sure the varible is there!!
	if(cc_pt !=0 && cr_pt!=0){
		cfreq = cr_pt->GetValue();
		cc_new= cc_pt->GetValue()/cfreq;
		t_diff = (cc_new - cc_old);
  		t_sec  =  t_diff;
  		}
  	else{t_sec=0.0;	}
  	if(cfreq==0||t_diff==0){t_sec=0.0;}

	//Looping for the differenve current cuts;
	for(int i =0;i<5;i++){
		//Greated then the DB cut vaule;
		if(current[7]>=c_cuts[i]){
			BeamUp[i]+=V1495_diff;	BeamOn[i]++;
			if(isrenewed){BeamUp_S[i]+=t_sec;}
			}
		else{//Reset the count otherwise
			BeamUp[i]=0.0;			BeamOn[i]=0.0;
			BeamUp_S[i]=0.0;
			}
	}//end of for loop
	
	//cout << BeamUp[4] << " "<< BeamUp_S[4] <<" "<< BeamUp_S[4] - BeamUp[4]<<endl;
//Debug statement 10 for all 3 for just this
	if(debug==3||debug==10){
		cout <<current[7]<<" "<< BeamOn[2] <<"  "<< BeamUp[2]<< "  "<< V1495<<endl;
	}
//Set for finding time diff.
	V1495_old = V1495;
	cc_old = cc_new;
	return 0;
}
//_____________________________________________________________________________
void TriBCM::DeleteArrays()
{
  // Delete member arrays. Internal function used by destructor.

  	delete [] BeamOn;    BeamOn   = NULL;
	delete [] BeamUp;    BeamUp   = NULL;
	delete [] BeamUp_S;  BeamUp_S = NULL;

}
//ClassImp(TriBCM)
