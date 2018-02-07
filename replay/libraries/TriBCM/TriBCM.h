#ifndef TriBCM_
#define TriBCM_

/////////////////////////////////////////////////////////////////////
//
//   TriBCM
//   Class to handle Hall A scaler events (type 1-14)
//   author  Robert Michaels (rom@jlab.org)
//
/////////////////////////////////////////////////////////////////////

//#include "../TriScalerEvtHandler/TriScalerEvtHandler.h"
#include "THaPhysicsModule.h"
#include <string>
#include <vector>
#include "TTree.h"
#include "TString.h"  

//public THaPhysicsModule, public TriScalerEvtHandler
class TriBCM : public THaPhysicsModule {

public:

   TriBCM(const char* name, const char* description, const char* arm1, const char* scaler1, Int_t Debug);
   virtual ~TriBCM();

   //virtual Int_t Analyze(THaEvData *evdata);
   //virtual EStatus Init( const TDatime& run_time);
   //virtual Int_t End( THaRunBase* r=0 );
   virtual Int_t Process(const THaEvData& evdata);
   //virtual Int_t SetInputModule( const char* arm, const char* scaler );
  // virtual Int_t SetInputModule( const char* arm1, const char* scaler1, const char* bcm_name2[] );

  protected:

  Int_t debug;
  TString scaler;
  TString arm;
  //TString arm1;
  //TString scaler1;
  
  
  Float_t bcm_u1;
  Float_t bcm_d1; 
  Float_t bcm_d3; 
  Float_t bcm_d10;
  Float_t bcm_unew;
  Float_t bcm_dnew;
  
  TString bcm_name[8];
  TString bcm_name_R[8];
  
  Float_t bcm_u1_c;
  Float_t bcm_d1_c;
  Float_t bcm_d3_c;
  Float_t bcm_d10_c; 
  Float_t bcm_unew_c;
  Float_t bcm_dnew_c;
  
  Float_t bcm_u1_c_old;
  Float_t bcm_d1_c_old;
  Float_t bcm_d3_c_old;
  Float_t bcm_d10_c_old; 
  Float_t bcm_unew_c_old;
  Float_t bcm_dnew_c_old;
  
  Float_t bcm_old[8];
  Float_t current[8];
  Float_t charge[8];
  Float_t bcm_new[8];
  Double_t gain[8];
  Double_t off[8];
  Double_t count;


  Float_t clock_count_new;
  Float_t clock_count_old;
  
  
    Float_t current_u1;
    Float_t current_u3;
    Float_t current_u10;
  Float_t current_d1;
  Float_t current_d3;
  Float_t current_d10;
  Float_t current_unew;
  Float_t current_dnew;
  
  Float_t charge_u1;
    Float_t charge_u3;
    Float_t charge_u10;
  Float_t charge_d1;
  Float_t charge_d3;
  Float_t charge_d10;
  Float_t charge_unew;
  Float_t charge_dnew;
  Float_t isrenewed;
  Float_t total_charge;
  Float_t avg_current;
  
  Double_t gain_u1;
  Double_t off_u1;
  Double_t gain_u3;
  Double_t off_u3;
  Double_t gain_u10;
  Double_t off_u10;
  Double_t gain_unew;
  Double_t off_unew;
  Double_t gain_d1;
  Double_t off_d1;
  Double_t gain_d3;
  Double_t off_d3;
  Double_t gain_d10;
  Double_t off_d10;
  Double_t gain_dnew;
  Double_t off_dnew;
  
  
  void           DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );

   ClassDef(TriBCM,0)  // Scaler Event handler

};

#endif
