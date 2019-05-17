//*-- Author :    Ole Hansen   09-Mar-02

//////////////////////////////////////////////////////////////////////////
//
// TriVDCTrackID
//
// This is a utility class for the VDC tracking classes.
// It allows unique identification of tracks.
//
//////////////////////////////////////////////////////////////////////////

#include "TriVDCTrackID.h"
#include "TriVDCCluster.h"
#include "TriVDCUVTrack.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

//_____________________________________________________________________________
TriVDCTrackID::TriVDCTrackID( const TriVDCUVTrack* lower,
			      const TriVDCUVTrack* upper) :
  TriTrackID()
{
  // Constructor that automatically determines pivot numbers
  // from the given TriVDCUVTracks.

  TriVDCCluster* cluster;
  if( lower ) {
    if( (cluster = lower->GetUCluster()))
      fLowerU = cluster->GetPivotWireNum();
    if( (cluster = lower->GetVCluster()))
      fLowerV = cluster->GetPivotWireNum();
  }
  if( upper ) {
    if( (cluster = upper->GetUCluster()))
      fUpperU = cluster->GetPivotWireNum();
    if( (cluster = upper->GetVCluster()))
      fUpperV = cluster->GetPivotWireNum();
  }
}

//_____________________________________________________________________________
TriVDCTrackID::TriVDCTrackID( const TriVDCTrackID& rhs ) : TriTrackID(rhs),
  fLowerU(rhs.fLowerU), fLowerV(rhs.fLowerV),
  fUpperU(rhs.fUpperU), fUpperV(rhs.fUpperV)
{
  // Copy constructor.
}

//_____________________________________________________________________________
TriVDCTrackID& TriVDCTrackID::operator=( const TriVDCTrackID& rhs )
{
  // Assignment operator.

  TriTrackID::operator=(rhs);
  if ( this != &rhs ) {
    fLowerU = rhs.fLowerU;
    fLowerV = rhs.fLowerV;
    fUpperU = rhs.fUpperU;
    fUpperV = rhs.fUpperV;
  }
  return *this;
}

//_____________________________________________________________________________
void TriVDCTrackID::Print( Option_t* ) const
{
  // Print ID description

  cout << " " 
       << setw(5) << fLowerU
       << setw(5) << fLowerV
       << setw(5) << fUpperU
       << setw(5) << fUpperV
       << endl;
}
///////////////////////////////////////////////////////////////////////////////
ClassImp(TriVDCTrackID)
