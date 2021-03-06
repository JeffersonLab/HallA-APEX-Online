#ifndef ROOT_Tri_Beam_Eloss
#define ROOT_Tri_Beam_Eloss

//////////////////////////////////////////////////////////////////////////
//
// Tri_Beam_Eloss
//
//////////////////////////////////////////////////////////////////////////

#include "../Tri_ElossCorrection/Tri_ElossCorrection.h"
#include "THaBeamModule.h"

class Tri_Beam_Eloss : public Tri_ElossCorrection, public THaBeamModule {
  
public:
  Tri_Beam_Eloss( const char* name, const char* description,
		  const char* input_beam = "", const char* vertex = "",
		  const Double_t z_offset = 0.0 );
  virtual ~Tri_Beam_Eloss();
  
  virtual void      Clear( Option_t* opt="" );

  virtual EStatus   Init( const TDatime& run_time );
  virtual Int_t     Process( const THaEvData& );


protected:

  THaBeamModule*    fBeamModule; // Pointer to beam module

  // Function for updating fEloss based on input beamifo.
  virtual void      CalcEloss( THaBeamInfo* beamifo );

  // Setup functions
  virtual Int_t DefineVariables( EMode mode = kDefine );

  ClassDef(Tri_Beam_Eloss,0)   //Beam energy loss correction module
};

#endif
