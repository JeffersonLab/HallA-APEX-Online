//*-- Author :    Ole Hansen   7-Sep-00

//////////////////////////////////////////////////////////////////////////
//
// TriTrackingDetector
//
// Abstract base class for a generic tracking detector. 
//
// This is a special TriSpectrometerDetector that is capable of
// finding particle tracks. It does not need to find track vertices
// (target coordinates) - this is usually done by the enclosing apparatus.
//
// Note: The FPP is NOT a "tracking detector" in this scheme because,
// with the present FPP algorithms, it is not used for calculating the 
// fp coordinates. 
//
//////////////////////////////////////////////////////////////////////////

#include "TClonesArray.h"
#include "TriTrackingDetector.h"
#include "THaPIDinfo.h"
#include "TriTrack.h"
#include "TriSpectrometer.h"
#include "TError.h"

ClassImp(TriTrackingDetector)

//_____________________________________________________________________________
TriTrackingDetector::TriTrackingDetector( const char* name, 
					  const char* description,
					  THaApparatus* apparatus )
  : TriSpectrometerDetector(name,description,apparatus)
{
  // Constructor

}

//_____________________________________________________________________________
TriTrackingDetector::TriTrackingDetector( )
  : TriSpectrometerDetector( )
{
  // Constructor for ROOT I/O only
}

//_____________________________________________________________________________
TriTrackingDetector::~TriTrackingDetector()
{
  // Destructor
}

//_____________________________________________________________________________
TriTrack* TriTrackingDetector::AddTrack( TClonesArray& tracks,
					 Double_t x, Double_t y,
					 Double_t theta, Double_t phi,
					 TriTrackID* ID )
{
  // Add a track with the given parameters to the array of tracks
  // Returns the index of the created track in the TClonesArray.

  Int_t i = tracks.GetLast()+1;
  THaPIDinfo* pid = 0;
  TriSpectrometer* spect = static_cast<TriSpectrometer*>( GetApparatus() );

  if( spect ) {

    // Create new PIDinfo and vertex objects if necessary

    UInt_t ndet      = spect->GetNpidDetectors();
    UInt_t npart     = spect->GetNpidParticles();
    TClonesArray& c  = *spect->GetTrackPID();

    // caution: c[i] creates an empty object at i if slot i was empty,
    // so use At(i) to check.

    if( i > c.GetLast() || !c.At(i) )  new( c[i] ) THaPIDinfo( ndet, npart );
    pid = static_cast<THaPIDinfo*>( c.At(i) );
  
  } else if( fDebug>0 ) {
    ::Warning("TriTrackingDetector::AddTrack", "No spectrometer defined for "
	      "detector %s. Track has no PID and vertex info.", GetName());
  }

  // Create the track

  return new( tracks[i] ) TriTrack( x, y, theta, phi, this, ID, pid );

}

