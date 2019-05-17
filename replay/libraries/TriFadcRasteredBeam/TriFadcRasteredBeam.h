#ifndef ROOT_TriFadcRasteredBeam
#define ROOT_TriFadcRasteredBeam

//////////////////////////////////////////////////////////////////////////
//
// TriFadcRasteredBeam
//
//////////////////////////////////////////////////////////////////////////

#include "THaBeam.h"

class TriFadcRasteredBeam : public THaBeam {

public:
  TriFadcRasteredBeam( const char* name, const char* description ) ;

  virtual ~TriFadcRasteredBeam() {}
  
  virtual Int_t Reconstruct() ;

protected:


  ClassDef(TriFadcRasteredBeam,0)    // A beam with rastered beam, analyzed event by event using raster currents
};

#endif

