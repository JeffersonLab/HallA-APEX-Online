#ifndef ROOT_TritiumSpectrometerDetector
#define ROOT_TritiumSpectrometerDetector

//////////////////////////////////////////////////////////////////////////
//
// TritiumSpectrometerDetector
//
// Abstract base class for a generic spectrometer detector.
//
// This is a specialized detector class that supports the concept of
// "tracking" and "PID" detectors.
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumDetector.h"

class THaTrack;

class TritiumSpectrometerDetector : public TritiumDetector {

public:
  virtual ~TritiumSpectrometerDetector();

  virtual Bool_t   IsTracking() = 0;
  virtual Bool_t   IsPid()      = 0;

          bool     CheckIntercept( THaTrack* track );
          bool     CalcInterceptCoords( THaTrack* track,
					Double_t& x, Double_t& y );
          bool     CalcPathLen( THaTrack* track, Double_t& t );

  TritiumSpectrometerDetector();       // for ROOT I/O only

protected:

          bool  CalcTrackIntercept( THaTrack* track, Double_t& t,
				    Double_t& ycross, Double_t& xcross);

  //Only derived classes may construct me
  TritiumSpectrometerDetector( const char* name, const char* description,
			   THaApparatus* a = NULL );

  ClassDef(TritiumSpectrometerDetector,1)   //ABC for a spectrometer detector
};

#endif
