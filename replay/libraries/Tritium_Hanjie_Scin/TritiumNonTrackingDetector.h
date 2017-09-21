#ifndef ROOT_TritiumNonTrackingDetector
#define ROOT_TritiumNonTrackingDetector

//////////////////////////////////////////////////////////////////////////
//
// TritiumNonTrackingDetector.h
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumSpectrometerDetector.h"

class TClonesArray;

class TritiumNonTrackingDetector : public TritiumSpectrometerDetector {

public:
  TritiumNonTrackingDetector(); // only for ROOT I/O

  virtual ~TritiumNonTrackingDetector();

  virtual Int_t    CoarseProcess( TClonesArray& tracks ) = 0;
  virtual Int_t    FineProcess( TClonesArray& tracks )  = 0;
          Bool_t   IsTracking() { return kFALSE; }
  virtual Bool_t   IsPid()      { return kFALSE; }

  Int_t GetNTracks() const;  // Number of tracks crossing this detector
  const TClonesArray* GetTrackHits() const { return fTrackProj; }

protected:

  TClonesArray*  fTrackProj;  // projection of track(s) onto detector plane

  Int_t CalcTrackProj( TClonesArray& tracks );

  //Only derived classes may construct me
  TritiumNonTrackingDetector( const char* name, const char* description,
			  THaApparatus* a = NULL);

  ClassDef(TritiumNonTrackingDetector,2)  //ABC for a non-tracking spectrometer detector
};

#endif
