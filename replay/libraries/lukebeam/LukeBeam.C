//*-- Author :    Bodo Reitz April 2003

//////////////////////////////////////////////////////////////////////////
//
// BobBeam
//
// Apparatus describing an unrastered beam.
//      can also be used if one only is interested 
//      in average beam position for a rastered beam
//      but in that latter case the event by event ext-target
//      corrections need to be skipped
//     (e.g. using an idealbeam for the hrs reconstruction and 
//      unrastered beam to fill global variables of the beamline)
// 
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "LukeBeam.h"
#include "LukeBPM.h"
#include "LukeRaster.h"
//#include "THaUnRasteredBeam.h"
#include "TMath.h"
#include "TDatime.h"
#include "VarDef.h"
#include "TVector.h"
#include "TList.h"

using namespace std;

ClassImp(LukeBeam)

//_____________________________________________________________________________

LukeBeam::LukeBeam( const char* name, 
				      const char* description,
				      Int_t runningsum_depth )
  : THaBeam( name, description ) , fRunningSumDepth(runningsum_depth)
{


  cout << "Creating LukeBPM and LukeRaster ..."<<endl;

  AddDetector( new LukeBPM("BPMA","1st bob bpm ",this) );
  AddDetector( new LukeBPM("BPMB","2nd bob bpm ",this) );
  AddDetector( new LukeRaster("Raster","upstream raster ",this) );
  AddDetector( new LukeRaster("Raster2","downstream raster ",this) );

  if (fRunningSumDepth>1) {
    fRunningSumWrap = false ; 
    fRunningSumNext = 0 ;
    fRSPosition.clear();
    fRSDirection.clear();
    fRSPosition.resize(fRunningSumDepth);
    fRSDirection.resize(fRunningSumDepth);
  } else {
    fRunningSumDepth=0;
  }
}


//_____________________________________________________________________________
Int_t LukeBeam::Reconstruct()
{

  // only the first two detectors in the list are used to get
  // get the beam position in two points, and to extrapolate that 
  // to the nominal target point
  // the following detectors are processed, but not used

  TVector3 p[2];

  TIter nextDet( fDetectors );
  nextDet.Reset();
  for( Int_t i = 0; i<2; i++ ) {
    THaBeamDet* theBeamDet = static_cast<THaBeamDet*>( nextDet() );
    if( theBeamDet ) {
      theBeamDet->Process();
      p[i] = theBeamDet->GetPosition();
      //cout << "Look Here! " << p[0][0] << endl;
    } else {
      Error( Here("Reconstruct"), 
	     "Beamline Detectors Missing in Detector List" );
    }
  }

  // Process any other detectors we may have
  while (THaBeamDet * theBeamDet=
	 static_cast<THaBeamDet*>( nextDet() )) {
    theBeamDet->Process();
  }

  if (fRunningSumDepth !=0 ) {


    if (fRunningSumWrap) {
      fRSAvPos = fRSAvPos - ( ( 1./fRunningSumDepth) * fRSPosition[fRunningSumNext] );
      fRSAvDir = fRSAvDir - ( ( 1./fRunningSumDepth) * fRSDirection[fRunningSumNext]);

    }      

    fRSDirection[fRunningSumNext] = p[1]-p[0] ;
    fRSPosition[fRunningSumNext]  = p[1] + 
      (p[1](2)/(p[0](2)-p[1](2))) * fRSDirection[fRunningSumNext] ;

    if (fRunningSumWrap) {
      fRSAvPos = fRSAvPos + ( ( 1./fRunningSumDepth) * fRSPosition[fRunningSumNext] );
      fRSAvDir = fRSAvDir +  (( 1./fRunningSumDepth) * fRSDirection[fRunningSumNext]);
    } else {
      if (fRunningSumNext==0) { 
	fRSAvPos = fRSPosition[fRunningSumNext];
	fRSAvDir = fRSDirection[fRunningSumNext];
      } else {
	fRSAvPos = ((double)fRunningSumNext/(fRunningSumNext+1.))*fRSAvPos 
	  + ( (1./(fRunningSumNext+1)) * fRSPosition[fRunningSumNext] );
	fRSAvDir = ((double)fRunningSumNext/(fRunningSumNext+1.))*fRSAvDir 
	  + ( (1./(fRunningSumNext+1)) * fRSDirection[fRunningSumNext] );
      }
    }
    fRunningSumNext++;
    if (fRunningSumNext==fRunningSumDepth) {
      fRunningSumNext = 0;
      fRunningSumWrap = true ;
    }
    fDirection = fRSAvDir;
    fPosition =  fRSAvPos;

  } else {

    fDirection = p[1]-p[0];
    fPosition = p[1] + (p[1](2)/(p[0](2)-p[1](2))) * fDirection ;

  }
  Update();

  return 0;

}

//_____________________________________________________________________________
void  LukeBeam::ClearRunningSum()
{
  fRunningSumNext = 0 ;
  fRunningSumWrap = false ;
}
