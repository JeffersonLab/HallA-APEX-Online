//*-- Author :    Bodo Reitz April 2003
//*-- Updated to include new Raster: Barak Schmookler, June 2015

//////////////////////////////////////////////////////////////////////////
//
// Gmp_Rastered_Beam
//
// Apparatus describing an rastered beam.
// 
//////////////////////////////////////////////////////////////////////////

#include "Gmp_Rastered_Beam.h"
#include "Gmp_BPM.h"
#include "Gmp_Raster.h"
#include "TMath.h"
#include "TDatime.h"
#include "TList.h"

#include "VarDef.h"


ClassImp(Gmp_Rastered_Beam)

//_____________________________________________________________________________

Gmp_Rastered_Beam::Gmp_Rastered_Beam( const char* name, const char* description ) :
    THaBeam( name, description ) 
{
  
  //This detector is used to for beam position/direction
  AddDetector( new Gmp_Raster("Raster","upstream raster ",this) );

  //These detectors are processed
  AddDetector( new Gmp_BPM("BPMA","1st bpm ",this) );
  AddDetector( new Gmp_BPM("BPMB","2nd bpm ",this) );
  AddDetector( new Gmp_Raster("Raster2","downstream raster ",this) );
}


//_____________________________________________________________________________
Int_t Gmp_Rastered_Beam::Reconstruct()
{

  TIter nextDet( fDetectors ); 

  nextDet.Reset();

  // This apparatus assumes that there is only one detector 
  // in the list. If someone adds detectors by hand, the first 
  // detector in the list will be used to get the beam position
  // the others will be processed

  

  if (THaBeamDet* theBeamDet=
      static_cast<THaBeamDet*>( nextDet() )) {
    theBeamDet->Process();
    fPosition = theBeamDet->GetPosition();
    fDirection = theBeamDet->GetDirection();
  }
  else {
    Error( Here("Reconstruct"), 
	   "Beamline Detectors Missing in Detector List" );
  }


  // Process any other detectors that may have been added (by default none)
  while (THaBeamDet * theBeamDet=
	 static_cast<THaBeamDet*>( nextDet() )) {
    theBeamDet->Process();
  }

  Update();

  return 0;

}
