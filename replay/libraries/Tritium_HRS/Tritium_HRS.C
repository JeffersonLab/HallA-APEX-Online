//*-- Author :    Ole Hansen   2-Oct-01

//////////////////////////////////////////////////////////////////////////
//
// THaHRS
//
// The standard Hall A High Resolution Spectrometers (HRS).
//
// The usual name of this object is either "R" or "L", for Left 
// and Right HRS, respectively.
//
// Defines the functions FindVertices() and TrackCalc(), which are common
// to both the LeftHRS and the RightHRS.
//
// By default, the THaHRS class does not define any detectors. (This is new
// in analyzer 1.6 and later.) In this way, the user has full control over
// the detector configuration in the analysis script.
// Detectors should be added with AddDetector() as usual.
//
// However: /chafs1/work1/tritium/rootfiles_1.6_root6/Rootfiles/tritium_90849.rootTo maintain backward compatibility with old scripts, the THaHRS
// will auto-create the previous set of standard detectors, "vdc", "s1" and
// "s2", if no "vdc" detector is defined at Init() time.
// This can be turned off by calling AutoStandardDetectors(kFALSE).
//
// For timing calculations, one can specify a reference detector via SetRefDet
// (usually a scintillator) as the detector at the 'reference distance',
// corresponding to the pathlength correction matrix.
//
//////////////////////////////////////////////////////////////////////////

#include "Tritium_HRS.h"
#include "THaTrackingDetector.h"
#include "THaTrack.h"
#include "THaScintillator.h"  // includes THaNonTrackingDetector
#include "THaVDC.h"
#include "THaTrackProj.h"
#include "THaTriggerTime.h"
#include "TMath.h"
#include "TList.h"
#include <cassert>
#include "TriFadcScin.h"
#include "TriFadcXscin.h"
#include <iostream>


using namespace std;

//_____________________________________________________________________________
Tritium_HRS::Tritium_HRS( const char* name, const char* description ) :
  THaSpectrometer( name, description ), fRefDet(0)
{
  // Constructor

  SetTrSorting(kFALSE);
  AutoStandardDetectors(kTRUE); // for backward compatibility
}

//_____________________________________________________________________________
Tritium_HRS::~Tritium_HRS()
{
  // Destructor
}

//_____________________________________________________________________________
Bool_t Tritium_HRS::SetTrSorting( Bool_t set )
{
  Bool_t oldset = TestBit(kSortTracks);
  SetBit( kSortTracks, set );
  return oldset;
}

//_____________________________________________________________________________
Bool_t Tritium_HRS::GetTrSorting() const
{
  return TestBit(kSortTracks);
}
 
//_____________________________________________________________________________
Bool_t Tritium_HRS::AutoStandardDetectors( Bool_t set )
{
  Bool_t oldset = TestBit(kAutoStdDets);
  SetBit( kAutoStdDets, set );
  return oldset;
}

//_____________________________________________________________________________
THaAnalysisObject::EStatus Tritium_HRS::Init( const TDatime& run_time )
{
  // Special HRS Init method for approximate backward compatibility with old
  // scripts. If no "vdc" detector has been defined by the user, assume we
  // are being run from an old script and create the old set of "standard
  // detectors" ("vdc", "s1", "s2") at the beginning of the detector list.
  // Note that the old script may have defined non-standard detectors, e.g.
  // Cherenkov, Shower, FPP etc.
  // This behavior can be turned off by calling AutoStandardDetectors(kFALSE).

  if( TestBit(kAutoStdDets) ) {
    THaDetector* pdet = static_cast<THaDetector*>( fDetectors->FindObject("vdc") );
    if( !pdet ) {
      AddDetector( new THaScintillator("s2", "S2 scintillator"), true, true );
      AddDetector( new THaScintillator("s1", "S1 scintillator"), true, true );
#ifndef NDEBUG
      Int_t ret =
#endif
	AddDetector( new THaVDC("vdc", "Vertical Drift Chamber"), true, true );
      assert(ret==0);  // else "vdc" was already defined after all
      assert( fDetectors->GetSize() >= 3 );
    }
  }

  // If the reference detector hasn't been defined yet (via SetRefDet),
  // use "s1", if it exists.
  if( !fRefDet )
    fRefDet = static_cast<THaScintillator*>( GetDetector("s1") );

  // Continue with standard initialization as before
  return THaSpectrometer::Init(run_time);
}

//_____________________________________________________________________________
Int_t Tritium_HRS::SetRefDet( const char* name )
{
  // Set reference detector for TrackTimes calculation to the detector
  // with the given name (typically a scintillator)

  const char* const here = "SetRefDet";

  if( !name || !*name ) {
    Error( Here(here), "Invalid detector name" );
    return 1;
  }

  fRefDet = static_cast<THaNonTrackingDetector*>
    ( fNonTrackingDetectors->FindObject(name) );

  if( !fRefDet ) {
    Error( Here(here), "Can't find detector \"%s\"", name );
    return 1;
  }

  return 0;
}

//_____________________________________________________________________________
Int_t Tritium_HRS::SetRefDet( const THaNonTrackingDetector* obj )
{
  const char* const here = "SetRefDet";

  if( !obj ) {
    Error( Here(here), "Invalid detector pointer" );
    return 1;
  }

  fRefDet = static_cast<THaNonTrackingDetector*>
    ( fNonTrackingDetectors->FindObject(obj) );

  if( !fRefDet ) {
    Error( Here(here), "Can't find given detector. "
	   "Is it a THaNonTrackingDetector?");
    return 1;
  }

  return 0;
}

//_____________________________________________________________________________
Int_t Tritium_HRS::FindVertices( TClonesArray& tracks )
{
  // Reconstruct target coordinates for all tracks found in the focal plane.

  TIter nextTrack( fTrackingDetectors );

  nextTrack.Reset();
  while( THaTrackingDetector* theTrackDetector =
	 static_cast<THaTrackingDetector*>( nextTrack() )) {
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "Call FineTrack() for " 
			<< theTrackDetector->GetName() << "... ";
#endif
    theTrackDetector->FindVertices( tracks );
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "done.\n";
#endif
  }

  // If enabled, sort the tracks by chi2/ndof
  if( GetTrSorting() ) {
    fTracks->Sort();
    // Reassign track indexes. Sorting may have changed the order
    for( int i = 0; i < fTracks->GetLast()+1; i++ ) {
      THaTrack* theTrack = static_cast<THaTrack*>( fTracks->At(i) );
      assert( theTrack );
      theTrack->SetIndex(i);
    }
  }

  // Find the "Golden Track". 
  if( GetNTracks() > 0 ) {
    // Select first track in the array. If there is more than one track
    // and track sorting is enabled, then this is the best fit track
    // (smallest chi2/ndof).  Otherwise, it is the track with the best
    // geometrical match (smallest residuals) between the U/V clusters
    // in the upper and lower VDCs (old behavior).
    // 
    // Chi2/dof is a well-defined quantity, and the track selected in this
    // way is immediately physically meaningful. The geometrical match
    // criterion is mathematically less well defined and not usually used
    // in track reconstruction. Hence, chi2 sortiing is preferable, albeit
    // obviously slower.

    fGoldenTrack = static_cast<THaTrack*>( fTracks->At(0) );
    fTrkIfo      = *fGoldenTrack;
    fTrk         = fGoldenTrack;
  } else
    fGoldenTrack = NULL;

  return 0;
}

//_____________________________________________________________________________
Int_t Tritium_HRS::TrackCalc()
{
  // Additioal track calculations. At present, we only calculate beta here.
//cout<<"hahaha3"<<endl;
  return TrackTimes( fTracks );
}

//_____________________________________________________________________________
Int_t Tritium_HRS::TrackTimes( TClonesArray* Tracks )
{
  if(!Tracks) {return -1;} //cout<<"no track"<<endl;}
 TriFadcXscin *s0_scin=static_cast<TriFadcXscin*>(GetDetector("s0"));
 TriFadcScin *s2_scin=static_cast<TriFadcScin*>(GetDetector("s2"));
 THaTrack *track=0;
 Int_t ntrack=GetNTracks();

//Loop the tracks
for(Int_t i=0;i<ntrack;i++)
{ track=static_cast<THaTrack*>(Tracks->At(i));
       

  Double_t beta=kBig;
  Double_t dbeta=kBig;
  Double_t time=kBig;
  Double_t dt=kBig;

  THaTrackProj* tr_s2=static_cast<THaTrackProj*>(s2_scin->GetTrackHits()->At(i));
  Double_t s2pathl=tr_s2->GetPathLen();
  Int_t s2pad=tr_s2->GetChannel();
 // cout<<"1="<<s2pad<<endl;
  if(s2pad<0)
  { track->SetBeta(beta);
   track->SetdBeta(dbeta);
   track->SetTime(time);
   track->SetdTime(dt);
   continue;
  }
 Double_t s2time=(s2_scin->GetTimes())[s2pad];
 //cout <<"2="<<s2time<<endl;
// Double_t s0time=(s0_scin->GetTimes())[s0pad];



 THaTrackProj* tr_s0=static_cast<THaTrackProj*>(s0_scin->GetTrackHits()->At(i));
 Double_t s0pathl=tr_s0->GetPathLen();
 Int_t s0pad=tr_s0->GetChannel();
 if(s0pad<0)
  { track->SetBeta(beta);
   track->SetdBeta(dbeta);
   track->SetTime(time);
   track->SetdTime(dt);
   continue;
  
}

 Double_t s0time=(s0_scin->GetTimes())[s0pad];
 time=s2time-s0time;
// cout<<"time="<<time<<endl;
 Double_t pathl=s2pathl-s0pathl;
 Double_t speed=pathl/time;
//cout<<"speed="<<speed<<endl;
 Double_t c=-2.99792458e8;
 beta=speed/c;
// cout<<beta<<endl;
   track->SetBeta(beta);
   track->SetdBeta(dbeta);
   track->SetTime(time);
   track->SetdTime(dt);
}
return 0;

}

//_____________________________________________________________________________



ClassImp(Tritium_HRS)
