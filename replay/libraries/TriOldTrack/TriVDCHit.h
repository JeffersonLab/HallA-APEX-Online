#ifndef ROOT_TriVDCHit
#define ROOT_TriVDCHit

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriVDCHit                                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TriVDCWire.h"
#include <cstdio>

class TriVDCHit : public TObject {

public:
  TriVDCHit( TriVDCWire* wire=NULL, Int_t rawtime=0, Double_t time=0.0 ) : 
    fWire(wire), fRawTime(rawtime), fTime(time), fDist(0.0), ftrDist(kBig) {}
  virtual ~TriVDCHit() {}

  virtual Double_t ConvertTimeToDist(Double_t slope);
  Int_t  Compare ( const TObject* obj ) const;
  Bool_t IsSortable () const { return kTRUE; }
  
  // Get and Set Functions
  TriVDCWire* GetWire() const { return fWire; }
  Int_t    GetWireNum() const { return fWire->GetNum(); }
  Int_t    GetRawTime() const { return fRawTime; }
  Double_t GetTime()    const { return fTime; }
  Double_t GetDist()    const { return fDist; }
  Double_t GetPos()     const { return fWire->GetPos(); } //Position of hit wire
  Double_t GetdDist()   const { return fdDist; }

  void     SetWire(TriVDCWire * wire) { fWire = wire; }
  void     SetRawTime(Int_t time)     { fRawTime = time; }
  void     SetTime(Double_t time)     { fTime = time; }
  void     SetDist(Double_t dist)     { fDist = dist; }
  void     SetdDist(Double_t ddist)   { fdDist = ddist; }
  void     SetFitDist(Double_t dist)  { ftrDist = dist; }
  
protected:
  static const Double_t kBig;  //!
  
  TriVDCWire* fWire;     // Wire on which the hit occurred
  Int_t       fRawTime;  // TDC value (channels)
  Double_t    fTime;     // Time corrected for time offset of wire (s)
  Double_t    fDist;     // (Perpendicular) Drift Distance
  Double_t    fdDist;    // uncertainty in fDist (for chi2 calc)
  Double_t    ftrDist;   // (Perpendicular) distance from the track
  
 private:
  TriVDCHit( const TriVDCHit& );
  TriVDCHit& operator=( const TriVDCHit& );
  
  ClassDef(TriVDCHit,2)             // VDCHit class
};

////////////////////////////////////////////////////////////////////////////////

#endif
