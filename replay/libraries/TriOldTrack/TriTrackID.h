#ifndef ROOT_TriTrackID
#define ROOT_TriTrackID

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriTrackID                                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include "TObject.h"

class THaVDCUVTrack;

class TriTrackID : public TObject {

public:
  TriTrackID() {}
  TriTrackID( const TriTrackID& rhs ) : TObject(rhs) {}
  TriTrackID& operator=( const TriTrackID& rhs )
  { TObject::operator=(rhs); return *this; }

  virtual ~TriTrackID() {}

  virtual Bool_t  operator==( const TriTrackID& ) = 0;
  virtual Bool_t  operator!=( const TriTrackID& ) = 0;

protected:

  ClassDef(TriTrackID,0)      // Track ID abstract base class
};

#endif
