#ifndef ROOT_Tri_Track_Eloss
#define ROOT_Tri_Track_Eloss

//////////////////////////////////////////////////////////////////////////
//
// Tri_Track_Eloss
//
//////////////////////////////////////////////////////////////////////////

#include "../Tri_ElossCorrection/Tri_ElossCorrection.h"
#include "THaTrackingModule.h"

class Tri_Track_Eloss : public Tri_ElossCorrection, public THaTrackingModule {
  
 public:
  Tri_Track_Eloss( const char* name, const char* description,
		   const char* input_tracks = "", const char* vertex = "",
		   const Double_t t_length = 0.25/* m */, const Double_t z_offset = 0.0,
		   const Double_t air_length = 0.3868,
		   Double_t particle_mass = 0.511e-3 /* GeV/c^2 */,
		   Int_t hadron_charge = 1 );
  virtual ~Tri_Track_Eloss();
  
  virtual void      Clear( Option_t* opt="" );
  
  virtual EStatus   Init( const TDatime& run_time );
  virtual Int_t     Process( const THaEvData& );
  
  
 protected:

  Double_t fTLength ;              // Length of target (m)
  Double_t fAirLength;             // Air between scattering chamber and spectrometer (m)
  THaTrackingModule* fTrackModule; // Pointer to tracking module
  
  // Function for updating fEloss based on input trkifo.
  virtual void       CalcEloss( THaTrackInfo* trkifo );
  
  // Setup functions
  virtual Int_t DefineVariables( EMode mode = kDefine );
  
  ClassDef(Tri_Track_Eloss,0)   //Track energy loss correction module
};

#endif
