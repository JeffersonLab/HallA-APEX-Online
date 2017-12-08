#ifndef ROOT_TriNonTrackingDetector
#define ROOT_TriNonTrackingDetector

//////////////////////////////////////////////////////////////////////////
//
// TriNonTrackingDetector.h
//
// Abstract base class for a generic non-tracking spectrometer detector. 
//
// This is a special TriSpectrometerDetector -- any detector that
// is not a tracking detector.  This includes PID detectors.
//
//////////////////////////////////////////////////////////////////////////

#include "TriSpectrometerDetector.h"

class TClonesArray;

class TriNonTrackingDetector : public TriSpectrometerDetector {
  
public:
  TriNonTrackingDetector(); // only for ROOT I/O

  virtual ~TriNonTrackingDetector();
  
  virtual Int_t    CoarseProcess( TClonesArray& tracks ) = 0;
  virtual Int_t    FineProcess( TClonesArray& tracks )  = 0;
          Bool_t   IsTracking() { return kFALSE; }
  virtual Bool_t   IsPid()      { return kFALSE; }

protected:

  //Only derived classes may construct me for real

  TriNonTrackingDetector( const char* name, const char* description,
			  THaApparatus* a = NULL);

  ClassDef(TriNonTrackingDetector,1)  //ABC for a non-tracking spectrometer detector
};

#endif
