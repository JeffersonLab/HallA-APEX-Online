#ifndef ROOT_Gmp_Rastered_Beam
#define ROOT_Gmp_Rastered_Beam

//////////////////////////////////////////////////////////////////////////
//
// Gmp_Rastered_Beam
//
//////////////////////////////////////////////////////////////////////////

#include "THaBeam.h"

class Gmp_Rastered_Beam : public THaBeam {

public:
  Gmp_Rastered_Beam( const char* name, const char* description ) ;

  virtual ~Gmp_Rastered_Beam() {}
  
  virtual Int_t Reconstruct() ;

protected:


  ClassDef(Gmp_Rastered_Beam,0)    // A beam with rastered beam, analyzed event by event using raster currents
};

#endif

