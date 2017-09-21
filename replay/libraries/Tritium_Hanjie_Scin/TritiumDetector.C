//*-- Author :    Ole Hansen   15-May-00

//////////////////////////////////////////////////////////////////////////
//
// TritiumDetector
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumDetector.h"
#include "THaApparatus.h"

ClassImp(TritiumDetector)

//_____________________________________________________________________________
TritiumDetector::TritiumDetector( const char* name, const char* description,
			  THaApparatus* apparatus )
  : TritiumDetectorBase(name,description), fApparatus(apparatus)
{
  // Constructor

  if( !name || !*name ) {
    Error( "TritiumDetector()", "Must construct detector with valid name! "
	   "Object construction failed." );
    MakeZombie();
    return;
  }
}

//_____________________________________________________________________________
TritiumDetector::TritiumDetector( ) : fApparatus(0) {
  // for ROOT I/O only
}

//_____________________________________________________________________________
TritiumDetector::~TritiumDetector()
{
  // Destructor
}

//_____________________________________________________________________________
THaApparatus* TritiumDetector::GetApparatus() const
{
  return static_cast<THaApparatus*>(fApparatus.GetObject());
}

//_____________________________________________________________________________
void TritiumDetector::SetApparatus( THaApparatus* apparatus )
{
  // Associate this detector with the given apparatus.
  // Only possible before initialization.

  if( IsInit() ) {
    Warning( Here("SetApparatus()"), "Cannot set apparatus. "
	     "Object already initialized.");
    return;
  }
  fApparatus = apparatus;
}

//_____________________________________________________________________________
void TritiumDetector::MakePrefix()
{
  // Set up name prefix for global variables. Internal function called
  // during initialization.

  THaApparatus *app = GetApparatus();
  const char* basename = (app != 0) ? app->GetName() : 0;
  TritiumDetectorBase::MakePrefix( basename );

}

