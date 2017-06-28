#ifndef ROOT_LukeBeam
#define ROOT_LukeBeam

//////////////////////////////////////////////////////////////////////////
//
// LukeBeam
//
//////////////////////////////////////////////////////////////////////////

#include "THaBeam.h"
#include <vector>

class LukeBeam : public THaBeam {

public:
  LukeBeam( const char* name, const char* description, Int_t runningsum_depth = 0 ) ;

  virtual ~LukeBeam() {}
  
  virtual Int_t Reconstruct() ;

  void ClearRunningSum();

  Int_t fRunningSumDepth;

protected:

  Bool_t fRunningSumWrap;
  Int_t fRunningSumNext;
  std::vector<TVector3> fRSPosition;
  std::vector<TVector3> fRSDirection;
  TVector3 fRSAvPos;
  TVector3 fRSAvDir;   

  ClassDef(LukeBeam,0)    // A beam with unrastered beam, analyzed event by event
};

#endif

