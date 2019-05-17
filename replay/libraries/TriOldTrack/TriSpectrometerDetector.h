#ifndef ROOT_TriSpectrometerDetector
#define ROOT_TriSpectrometerDetector

//////////////////////////////////////////////////////////////////////////
//
// TriSpectrometerDetector
//
// Abstract base class for a generic spectrometer detector. 
//
// This is a specialized detector class that supports the concept of
// "tracking" and "PID" detectors.
//
//////////////////////////////////////////////////////////////////////////

#include "THaDetector.h"

class TriTrack;

class TriSpectrometerDetector : public THaDetector {
  
public:
  virtual ~TriSpectrometerDetector();
  
  virtual Bool_t   IsTracking() = 0;
  virtual Bool_t   IsPid()      = 0;

          bool     CheckIntercept( TriTrack* track );
          bool     CalcInterceptCoords( TriTrack* track, 
					Double_t& x, Double_t& y );
          bool     CalcPathLen( TriTrack* track, Double_t& t );

  TriSpectrometerDetector();       // for ROOT I/O only

protected:

  // Geometry data
  TVector3  fXax;                  // X axis of the detector plane
  TVector3  fYax;                  // Y axis of the detector plane
  TVector3  fZax;                  // Normal to the detector plane

  virtual void  DefineAxes( Double_t rotation_angle );

          bool  CalcTrackIntercept( TriTrack* track, Double_t& t, 
				    Double_t& ycross, Double_t& xcross);

  //Only derived classes may construct me
  TriSpectrometerDetector( const char* name, const char* description,
			   THaApparatus* a = NULL );

  ClassDef(TriSpectrometerDetector,1)   //ABC for a spectrometer detector
};

#endif
