#ifndef ROOT_TriVDCTrackID
#define ROOT_TriVDCTrackID

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriVDCTrackID                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include "TriTrackID.h"

class TriVDCUVTrack;

class TriVDCTrackID : public TriTrackID {

public:
  TriVDCTrackID() : TriTrackID(),
    fLowerU(0), fLowerV(0), fUpperU(0), fUpperV(0) {}
  TriVDCTrackID( Int_t lowerU, Int_t lowerV,
		 Int_t upperU, Int_t upperV ) : TriTrackID(),
    fLowerU(lowerU), fLowerV(lowerV), fUpperU(upperU), fUpperV(upperV) {}
  TriVDCTrackID( const TriVDCUVTrack* lower, const TriVDCUVTrack* upper );
  TriVDCTrackID( const TriVDCTrackID& );
  TriVDCTrackID& operator=( const TriVDCTrackID& );
  
  virtual ~TriVDCTrackID() {}

  virtual Bool_t  operator==( const TriTrackID& );
  virtual Bool_t  operator!=( const TriTrackID& );
  virtual void    Print( Option_t* opt="" ) const;

protected:

  Int_t           fLowerU;         // Lower U plane pivot wire number
  Int_t           fLowerV;         // Lower V plane pivot wire number
  Int_t           fUpperU;         // Upper U plane pivot wire number
  Int_t           fUpperV;         // Upper V plane pivot wire number

  ClassDef(TriVDCTrackID,0)      // Track ID class
};

//__________________ inlines __________________________________________________
inline
Bool_t TriVDCTrackID::operator==( const TriTrackID& RHS )
{
  if( IsA() != RHS.IsA() ) return kFALSE;
  const TriVDCTrackID& rhs = static_cast<const TriVDCTrackID&>(RHS);
  return ( (fLowerU == rhs.fLowerU) && (fLowerV == rhs.fLowerV) &&
	   (fUpperU == rhs.fUpperU) && (fUpperV == rhs.fUpperV) );
}

//__________________ inlines __________________________________________________
inline
Bool_t TriVDCTrackID::operator!=( const TriTrackID& RHS )
{
  if( IsA() != RHS.IsA() ) return kTRUE;
  const TriVDCTrackID& rhs = static_cast<const TriVDCTrackID&>(RHS);
  return ( (fLowerU != rhs.fLowerU) || (fLowerV != rhs.fLowerV) ||
	   (fUpperU != rhs.fUpperU) || (fUpperV != rhs.fUpperV) );
}

//////////////////////////////////////////////////////////////////////////////

#endif
