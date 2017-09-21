//*-- Author :    Ole Hansen   7-Sep-00

//////////////////////////////////////////////////////////////////////////
//
// TritiumSpectrometerDetector
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumSpectrometerDetector.h"
#include "THaTrack.h"
#include "TMath.h"

ClassImp(TritiumSpectrometerDetector)

//______________________________________________________________________________
TritiumSpectrometerDetector::TritiumSpectrometerDetector( const char* name,
						  const char* description,
						  THaApparatus* apparatus )
  : TritiumDetector(name,description,apparatus)
{
  // Constructor

}

//______________________________________________________________________________
TritiumSpectrometerDetector::TritiumSpectrometerDetector( )
{
  // Constructor for ROOT I/O only
}

//______________________________________________________________________________
TritiumSpectrometerDetector::~TritiumSpectrometerDetector()
{
  // Destructor

}

//_____________________________________________________________________________
bool TritiumSpectrometerDetector::CalcTrackIntercept(THaTrack* theTrack,
					 Double_t& t, Double_t& xcross,
					 Double_t& ycross)
{
  // projects a given track on to the plane of the detector
  // xcross and ycross are the x and y coords of this intersection
  // t is the distance from the origin of the track to the given plane.
  //
  // If a hit is NOT found, then t, xcross, and ycross are unchanged.
  TVector3 t0( theTrack->GetX(), theTrack->GetY(), 0.0 );
  Double_t norm = TMath::Sqrt(1.0 + theTrack->GetTheta()*theTrack->GetTheta() +
			      theTrack->GetPhi()*theTrack->GetPhi());
  TVector3 t_hat( theTrack->GetTheta()/norm, theTrack->GetPhi()/norm, 1.0/norm );

  TVector3 v;
  if( !IntersectPlaneWithRay( fXax, fYax, fOrigin, t0, t_hat, t, v ))
    return false;
  v -= fOrigin;
  xcross = v.Dot(fXax);
  ycross = v.Dot(fYax);

  return true;
}

//_____________________________________________________________________________
bool TritiumSpectrometerDetector::CheckIntercept(THaTrack *track)
{
  Double_t x, y, t;
  return CalcTrackIntercept(track, t, x, y);
}

//_____________________________________________________________________________
bool TritiumSpectrometerDetector::CalcInterceptCoords(THaTrack* track, Double_t& x, Double_t& y)
{
  Double_t t;
  return CalcTrackIntercept(track, t, x, y);
}

//_____________________________________________________________________________
bool TritiumSpectrometerDetector::CalcPathLen(THaTrack* track, Double_t& t)
{
  Double_t x, y;
  return CalcTrackIntercept(track, t, x, y);
}
