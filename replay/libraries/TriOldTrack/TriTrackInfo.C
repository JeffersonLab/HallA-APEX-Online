//*-- Author :    Ole Hansen   04-Apr-03

//////////////////////////////////////////////////////////////////////////
//
// TriTrackInfo
//
// Utility class/structure for holding track information.
//
//////////////////////////////////////////////////////////////////////////

#include "TriTrackInfo.h"
#include "TriTrack.h"
#include "TriSpectrometer.h"
#include "TriTrackingDetector.h"

using namespace std;

const Double_t TriTrackInfo::kBig = 1e38;

//_____________________________________________________________________________
TriTrackInfo& TriTrackInfo::operator=( const TriTrackInfo& rhs )
{
  // Assignment operator

  if( this != &rhs ) {
    fP     = rhs.fP;
    fDp    = rhs.fDp;
    fX     = rhs.fX;
    fY     = rhs.fY;
    fTheta = rhs.fTheta;
    fPhi   = rhs.fPhi;
    fPvect = rhs.fPvect;
    fOK    = rhs.fOK;
    fSpectro = rhs.fSpectro;
  }
  return *this;
}

//_____________________________________________________________________________
TriTrackInfo& TriTrackInfo::operator=( const TriTrack& track )
{
  // Assignment to a track

  fP     = track.GetP();
  fDp    = track.GetDp();
  fX     = track.GetTX();
  fY     = track.GetTY();
  fTheta = track.GetTTheta();
  fPhi   = track.GetTPhi();
  fPvect = const_cast<TriTrack&>(track).GetPvect();
  fOK    = 1;
  TriTrackingDetector* td = track.GetCreator();
  if( td )
    fSpectro = static_cast<TriSpectrometer*>(td->GetApparatus());

  return *this;
}

//_____________________________________________________________________________
ClassImp(TriTrackInfo)
