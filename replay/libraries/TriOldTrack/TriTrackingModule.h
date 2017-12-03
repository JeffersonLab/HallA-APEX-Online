#ifndef ROOT_TriTrackingModule
#define ROOT_TriTrackingModule

//////////////////////////////////////////////////////////////////////////
//
// TriTrackingModule
//
//////////////////////////////////////////////////////////////////////////

#include "TriTrackInfo.h"

class TriTrack;
struct RVarDef;

class TriTrackingModule {
  
public:
  TriTrackingModule(); // needed public for ROOT I/O
  virtual ~TriTrackingModule();
  
  TriTrackInfo*  GetTrackInfo() { return &fTrkIfo; }
  TriTrack*      GetTrack()     { return fTrk; }

  void TrkIfoClear();
  static const RVarDef* GetRVarDef();

protected:

  TriTrackInfo  fTrkIfo;          //  Track information
  TriTrack*     fTrk;             //! Pointer to associated track

  ClassDef(TriTrackingModule,2)   // ABC for a tracking module

};

#endif
