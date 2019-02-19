#ifndef ROOT_TriFadcUnRasteredBeam
#define ROOT_TriFadcUnRasteredBeam

//////////////////////////////////////////////////////////////////////////
//
// TriFadcRasteredBeam
//
//////////////////////////////////////////////////////////////////////////

#include "THaBeam.h"

class TriFadcUnRasteredBeam : public THaBeam {

public:
  TriFadcUnRasteredBeam( const char* name, const char* description ) ; // no runningsum for now

  virtual ~TriFadcUnRasteredBeam() {}
  
  virtual Int_t Reconstruct() ;

protected:


  ClassDef(TriFadcUnRasteredBeam,0)    // A beam with rastered beam, analyzed event by event using raster currents
};

#endif

