#ifndef ROOT_TritiumDetector
#define ROOT_TritiumDetector

//////////////////////////////////////////////////////////////////////////
//
// TritiumDetector
//
// Abstract base class for a generic Hall A detector. This class
// describes an actual detector (not subdetector) and can be added to
// an apparatus.
//
//////////////////////////////////////////////////////////////////////////

#include "TritiumDetectorBase.h"
#include <TRef.h>

class THaApparatus;

class TritiumDetector : public TritiumDetectorBase {

public:
  virtual ~TritiumDetector();
  THaApparatus*  GetApparatus() const;
  virtual void   SetApparatus( THaApparatus* );

  TritiumDetector();  // for ROOT I/O only

protected:

  virtual void MakePrefix();

  //Only derived classes may construct me
  TritiumDetector( const char* name, const char* description,
	       THaApparatus* apparatus = 0 );

private:
  TRef  fApparatus;         // Apparatus containing this detector

  ClassDef(TritiumDetector,1)   //Abstract base class for a Hall A detector
};

#endif
