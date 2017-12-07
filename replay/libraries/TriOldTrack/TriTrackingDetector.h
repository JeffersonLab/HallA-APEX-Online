#ifndef ROOT_TriTrackingDetector
#define ROOT_TriTrackingDetector

//////////////////////////////////////////////////////////////////////////
//
// TriTrackingDetector.h
//
//////////////////////////////////////////////////////////////////////////

#include "TriSpectrometerDetector.h"

class TClonesArray;
class TriTrack;
class TriTrackID;

class TriTrackingDetector : public TriSpectrometerDetector {
  
public:
  virtual ~TriTrackingDetector();
  
  virtual Int_t    CoarseTrack( TClonesArray& tracks ) = 0;
  virtual Int_t    FineTrack( TClonesArray& tracks )  = 0;
  // For backward-compatibility
  virtual Int_t    FindVertices( TClonesArray& /* tracks */ ) { return 0; }
          Bool_t   IsTracking() { return kTRUE; }
          Bool_t   IsPid()      { return kFALSE; }

	  TriTrackingDetector();   // for ROOT I/O only
protected:

  virtual TriTrack* AddTrack( TClonesArray& tracks,
			      Double_t x, Double_t y, 
			      Double_t theta, Double_t phi,
			      TriTrackID* ID = NULL );

  //Only derived classes may construct me

  TriTrackingDetector( const char* name, const char* description,
		       THaApparatus* a = NULL );

  ClassDef(TriTrackingDetector,1)   //ABC for a generic tracking detector
};

#endif
