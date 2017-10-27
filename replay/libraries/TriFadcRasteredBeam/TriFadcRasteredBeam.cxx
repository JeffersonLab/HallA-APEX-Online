//*-- Author :    Bodo Reitz April 2003

//////////////////////////////////////////////////////////////////////////
//
// TriFadcRasteredBeam
//
// Apparatus describing an rastered beam.
//
// 
//
//////////////////////////////////////////////////////////////////////////

#include "TriFadcRasteredBeam.h"
#include "TriFadcRaster.h"
#include "TriFadcBPM.h"
#include "TMath.h"
#include "TDatime.h"
#include "TList.h"

#include "VarDef.h"


ClassImp(TriFadcRasteredBeam)

//_____________________________________________________________________________

TriFadcRasteredBeam::TriFadcRasteredBeam( const char* name, const char* description ) :
    THaBeam( name, description ) 
{
  AddDetector( new TriFadcRaster("Raster","upstream raster") );
  AddDetector( new TriFadcRaster("Raster2","downstream raster") );
  AddDetector( new TriFadcBPM("BPMA","1st BPM") );
  AddDetector( new TriFadcBPM("BPMB","2nd BPM") );

}


//_____________________________________________________________________________
Int_t TriFadcRasteredBeam::Reconstruct()
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
