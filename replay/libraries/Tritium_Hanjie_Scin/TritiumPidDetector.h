#ifndef ROOT_TritiumPidDetector
#define ROOT_TritiumPidDetector

//////////////////////////////////////////////////////////////////////////
//
// TritiumPidDetector.h
//
// Abstract base class for a generic spectrometer detector capable of
// particle identification.
//
// This is a special TritiumNonTrackingDetector that is capable of
// providing particle identification information.  Typical examples are
// Cherenkov detectors and shower counters.
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumNonTrackingDetector.h"

class TritiumPidDetector : public TritiumNonTrackingDetector {
  
public:
  TritiumPidDetector() {} // for ROOT I/O
  virtual ~TritiumPidDetector();
  
          Bool_t   IsPid() { return kTRUE; }

protected:

  //Only derived classes may construct me

  TritiumPidDetector( const char* name, const char* description,
		  THaApparatus* a = NULL );

  ClassDef(TritiumPidDetector,0)  //ABC for a PID detector
};

#endif
