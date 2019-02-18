//*-- Author :    Ole Hansen   7-Sep-00

//////////////////////////////////////////////////////////////////////////
//
// TriNonTrackingDetector
//
//////////////////////////////////////////////////////////////////////////

#include "TriNonTrackingDetector.h"

ClassImp(TriNonTrackingDetector)

//______________________________________________________________________________
TriNonTrackingDetector::TriNonTrackingDetector( const char* name, 
						const char* description,
						THaApparatus* apparatus )
  : TriSpectrometerDetector(name,description,apparatus)
{
  // Normal constructor with name and description

}

//______________________________________________________________________________
TriNonTrackingDetector::TriNonTrackingDetector( )
  : TriSpectrometerDetector( )
{
  // for ROOT I/O only

}

//______________________________________________________________________________
TriNonTrackingDetector::~TriNonTrackingDetector()
{
  // Destructor

}

