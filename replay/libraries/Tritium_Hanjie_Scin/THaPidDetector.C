//*-- Author :    Ole Hansen   7-Sep-00

//////////////////////////////////////////////////////////////////////////
//
// TritiumPidDetector
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumPidDetector.h"

	ClassImp(TritiumPidDetector)

//______________________________________________________________________________
TritiumPidDetector::TritiumPidDetector( const char* name, const char* description,
				THaApparatus* apparatus )
  : TritiumNonTrackingDetector(name,description,apparatus)
{
  // Normal constructor with name and description

}

//______________________________________________________________________________
TritiumPidDetector::~TritiumPidDetector()
{
  // Destructor

}

