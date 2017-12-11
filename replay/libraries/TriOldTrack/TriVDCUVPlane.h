#ifndef ROOT_TriVDCUVPlane
#define ROOT_TriVDCUVPlane
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriVDCUVPlane                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include "THaSubDetector.h"
#include "TriVDCPlane.h"

#include "TClonesArray.h"
#include <cassert>

class TriVDCUVTrack;
class TriVDC;
class THaEvData;

class TriVDCUVPlane : public THaSubDetector {

  friend class TriVDCUVTrack;

public:

  TriVDCUVPlane( const char* name="", const char* description="",
		 THaDetectorBase* parent = NULL );
  virtual ~TriVDCUVPlane();

  virtual void    Clear( Option_t* opt="" );    // Reset event-by-event data
  virtual Int_t   Decode( const THaEvData& evData );
  virtual Int_t   CoarseTrack();          // Find clusters & estimate track
  virtual Int_t   FineTrack();            // More precisely calculate track
  virtual EStatus Init( const TDatime& date );

  // Get and Set Functions
  TriVDCPlane*   GetUPlane()      const { return fU; }
  TriVDCPlane*   GetVPlane()      const { return fV; } 
  Int_t          GetNUVTracks()   const { return fUVTracks->GetLast()+1; }
  TClonesArray*  GetUVTracks()    const { return fUVTracks; }
  TriVDC*        GetVDC()         const { return (TriVDC*)GetDetector(); }
  Double_t       GetSpacing()     const { return fSpacing;}
  TriVDCUVTrack* GetUVTrack( Int_t i ) const 
    { assert( i>=0 && i<GetNUVTracks() );
      return (TriVDCUVTrack*)fUVTracks->UncheckedAt(i); }

protected:

  TriVDCPlane*  fU;           // The U plane
  TriVDCPlane*  fV;           // The V plane

  TClonesArray* fUVTracks;    // UV tracks
  
  //UV Plane Geometry
  Double_t fSpacing;          // Space between U & V planes (m)
  Double_t fSin_u;            // Trig functions for the U plane axis angle
  Double_t fCos_u;            // 
  Double_t fSin_v;            // Trig functions for the V plane axis angle
  Double_t fCos_v;            // 
  Double_t fInv_sin_vu;       // 1/Sine of the difference between the
                              // V axis angle and the U axis angle

  // For CoarseTrack
  void FindClusters()        // Find clusters in U & V planes
    { fU->FindClusters(); fV->FindClusters(); }
  Int_t MatchUVClusters();   // Match U plane clusters 
                             //  with V plane clusters
  // For FineTrack
  void FitTracks()      // Fit data to recalculate cluster position
    { fU->FitTracks(); fV->FitTracks(); }

  // For Both
  Int_t CalcUVTrackCoords(); // Compute UV track coords in detector cs
  
  ClassDef(TriVDCUVPlane,0)             // VDCUVPlane class
};

////////////////////////////////////////////////////////////////////////////////

#endif
