//*-- Author :    Bodo Reitz April 2003
// modified for FADC class for tritium experiment -Shujie Li, 07.2018
//////////////////////////////////////////////////////////////////////////
//
// TriFadcUnRasteredBeam
//
// Apparatus describing an unrastered beam.
// Can be used to get the average beam position 
// of rastered beam, though the instaneous beam
// position is off ( BPM readout is too slow) 
//////////////////////////////////////////////////////////////////////////

#include "TriFadcUnRasteredBeam.h"
#include "TriFadcBPM.h"
#include "TMath.h"
#include "TDatime.h"
#include "TList.h"

#include "VarDef.h"


ClassImp(TriFadcUnRasteredBeam)

//_____________________________________________________________________________

TriFadcUnRasteredBeam::TriFadcUnRasteredBeam( const char* name, const char* description ) :
    THaBeam( name, description ) 
{ // the order matters !!
  AddDetector( new TriFadcBPM("BPMA","1st BPM") );
  AddDetector( new TriFadcBPM("BPMB","2nd BPM") );

}


//_____________________________________________________________________________
Int_t TriFadcUnRasteredBeam::Reconstruct()
{
   // only the first two detectors in the list (assummed to be bpma and bpmb) are used to get
  // get the beam position in two points, and to extrapolate that 
  // to the nominal target point
  // the following detectors are processed, but not used

  TIter nextDet( fDetectors ); 
  nextDet.Reset();
  TVector3 p[2]; 
  for( Int_t i = 0; i<2; i++ ) {
    THaBeamDet* theBeamDet = static_cast<THaBeamDet*>( nextDet() );
    if( theBeamDet ) {
      theBeamDet->Process();
      p[i] = theBeamDet->GetPosition();
    } else {
      Error( Here("Reconstruct"), 
       "Beamline Detectors Missing in Detector List" );
    }
  }
 
  // Process any other detectors that may have been added (by default none)
  while (THaBeamDet * theBeamDet=
	 static_cast<THaBeamDet*>( nextDet() )) {
    theBeamDet->Process();
  }

  // ignore the runningsum calculation for now, may add it back later
  fDirection = p[1]-p[0];
  fPosition = p[1] + (p[1](2)/(p[0](2)-p[1](2))) * fDirection ;

  Update();

  return 0;

}
