#ifndef ROOT_TriVDCCluster
#define ROOT_TriVDCCluster

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriVDCCluster                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"

class TriVDCHit;
class TriVDCPlane;
//class TriVDCUVTrack;
//class THaTrack;

class TriVDCCluster : public TObject {

public:
  TriVDCCluster( TriVDCPlane* owner = NULL ) :
    fSize(0), fPlane(owner), fSlope(kBig), fSigmaSlope(kBig), fInt(kBig),
    fSigmaInt(kBig), fT0(0.0), fSigmaT0(kBig), fPivot(NULL),
    fTimeCorrection(0.0),
    fFitOK(false), fLocalSlope(kBig), fChi2(kBig), fNDoF(0.0)  {}
  TriVDCCluster( const TriVDCCluster& );
  TriVDCCluster& operator=( const TriVDCCluster& );
  virtual ~TriVDCCluster();

  enum EMode { kSimple, kT0, kFull };

  virtual void   AddHit(TriVDCHit * hit);
  virtual void   EstTrackParameters();
  virtual void   ConvertTimeToDist();
  virtual void   FitTrack( EMode mode = kSimple );
  virtual void   ClearFit();
  virtual void   CalcChisquare(Double_t& chi2, Int_t& nhits) const;


  // TObject functions redefined
  virtual void   Clear( Option_t* opt="" );
  virtual Int_t  Compare( const TObject* obj ) const;
  virtual Bool_t IsSortable() const        { return kTRUE; }
  virtual void   Print( Option_t* opt="" ) const;

  //Get and Set Functions
  TriVDCHit**    GetHits()                 { return fHits; } // Get array of pointers
  TriVDCHit *    GetHit(Int_t i)     const { return fHits[i]; }
  TriVDCPlane*   GetPlane()          const { return fPlane; }
  Int_t          GetSize ()          const { return fSize; }
  Double_t       GetSlope()          const { return fSlope; }
  Double_t       GetLocalSlope()     const { return fLocalSlope; }
  Double_t       GetSigmaSlope()     const { return fSigmaSlope; }
  Double_t       GetIntercept()      const { return fInt; }
  Double_t       GetSigmaIntercept() const { return fSigmaInt; }
  TriVDCHit*     GetPivot()          const { return fPivot; }
  Int_t          GetPivotWireNum()   const;
  Double_t       GetTimeCorrection() const { return fTimeCorrection; }
  
  bool           IsFitOK()           const { return fFitOK; }

  void           SetPlane( TriVDCPlane* plane )     { fPlane = plane; }
  void           SetIntercept( Double_t intercept ) { fInt = intercept; }
  void           SetSlope( Double_t slope)          { fSlope = slope;}
  void           SetPivot( TriVDCHit* piv)          { fPivot = piv; }
  void           SetTimeCorrection(Double_t deltat) { fTimeCorrection = deltat; }
  //    void SetUVTrack(TriVDCUVTrack * uvtrack) {fUVTrack = uvtrack;} 
  //    void SetTrack(THaTrack * track) {fTrack = track;} 

protected:
  static const Int_t MAX_SIZE = 16;  // Assume no more than 16 hits per cluster
  static const Double_t kBig;

  Int_t          fSize;              // Size of cluster (no. of hits)
  TriVDCHit*     fHits[MAX_SIZE];    // [fSize] Hits associated w/this cluster
  TriVDCPlane*   fPlane;             // Plane the cluster belongs to
  //  TriVDCUVTrack * fUVTrack;      // UV Track the cluster belongs to
  //  THaTrack * fTrack;             // Track the cluster belongs to

  //Track Parameters 
  Double_t       fSlope, fSigmaSlope;// Slope and error in slope
  Double_t       fInt, fSigmaInt;    // Intercept and error
  Double_t       fT0, fSigmaT0;      // Fitted common timing offset and error
  TriVDCHit*     fPivot;             // Pivot - hit with smallest drift time
  Double_t       fTimeCorrection;    // correction to be applied when fitting
                                     // drift times
  bool           fFitOK;             // Flag indicating that fit results valid
  Double_t       fLocalSlope;        // Local slope, from FitTrack()
  Double_t       fChi2;              // chi2 for the cluster
  Double_t       fNDoF;              // NDoF in local chi2 calculation

  virtual void   CalcDist();         // calculate the track to wire distances
  virtual void   FitSimpleTrack();
  virtual void   FitSimpleTrackWgt(); // present for testing

  ClassDef(TriVDCCluster,0)          // A group of VDC hits
};

//////////////////////////////////////////////////////////////////////////////

#endif
