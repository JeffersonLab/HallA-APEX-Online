//*-- Author :    Ole Hansen   13-Mar-02

//////////////////////////////////////////////////////////////////////////
//
// TriVDCTrackPair
//
// This is a utility class for the VDC tracking classes.
// It allows unique identification of tracks.
//
//////////////////////////////////////////////////////////////////////////

#include "TriVDCTrackPair.h"
#include "TriVDCUVTrack.h"
#include "TClass.h"

#include <iostream>

using namespace std;

//_____________________________________________________________________________
TriVDCTrackPair& TriVDCTrackPair::operator=( const TriVDCTrackPair& rhs )
{
  // Assignment operator.

  TObject::operator=(rhs);
  if ( this != &rhs ) {
    fLowerTrack = rhs.fLowerTrack;
    fUpperTrack = rhs.fUpperTrack;
    fError      = rhs.fError;
    fStatus     = rhs.fStatus;
  }
  return *this;
}

//_____________________________________________________________________________
void TriVDCTrackPair::Analyze( Double_t spacing )
{
  // Compute goodness of match paremeter for the two UV tracks.
  // Essentially, this is a measure of how closely the two tracks
  // point at each other. 'spacing' is the separation of the 
  // upper and lower UV planes (in m).
  //
  // This method is critical for the quality of the VDC multi-track
  // reconstruction.

  // For now, a simple approach: Calculate projected positions of tracks in
  // opposite planes, measure how far they differ from partner track
  // intercept, and store the sum of the distances

  // Project the lower track into the upper plane
  fError  = GetProjectedDistance( fLowerTrack, fUpperTrack, spacing );
  fError += GetProjectedDistance( fUpperTrack, fLowerTrack, -spacing );

  return;
}

//_____________________________________________________________________________
Double_t TriVDCTrackPair::GetProjectedDistance( pUV here, pUV there,
						Double_t spacing )
{
  // Project 'here' to plane of 'there' and return square of distance between
  // projected position and intercept of 'there'

  Double_t px = here->GetX() + spacing * here->GetTheta();  // Projected x
  Double_t py = here->GetY() + spacing * here->GetPhi();    // Projected y
  
  Double_t x = there->GetX();
  Double_t y = there->GetY();

  return (px-x)*(px-x) + (py-y)*(py-y);
}

//_____________________________________________________________________________
Int_t TriVDCTrackPair::Compare( const TObject* obj ) const
{
  // Compare this object to another TriVDCTrackPair.
  // Used for sorting tracks.

  if( !obj || IsA() != obj->IsA() )
    return -1;

  const TriVDCTrackPair* rhs = static_cast<const TriVDCTrackPair*>( obj );

  if( fError < rhs->fError )
    return -1;
  if( fError > rhs->fError )
    return 1;

  return 0;
}

//_____________________________________________________________________________
void TriVDCTrackPair::Print( Option_t* ) const
{
  // Print this object

  cout << fError << endl;
}

///////////////////////////////////////////////////////////////////////////////
ClassImp(TriVDCTrackPair)
