#ifndef ROOT_THaFppHit
#define ROOT_THaFppHit

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaFppHit                                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include <cstdio>

class THaFppHit : public TObject {

public:
//	THaFppHit( Short_t wire=0, Float_t letdc=0., Float_t tetdc=0.) : 
//	  fWire(wire), fLtdc(letdc), fTtdc(tetdc),fDist(0.) {}
 	THaFppHit( Int_t wire=0, Double_t letdc=0., Double_t tetdc=0.) : 
	  fWire(wire), fLtdc(letdc), fTtdc(tetdc),fDist(0.) {}
	  virtual ~THaFppHit() {}

/*	  Short_t    GetWire()     const { return fWire; }
	  Float_t GetLEtime()  const { return fLtdc;}
	  Float_t GetTEtime() const { return fTtdc;}
	  Float_t GetStraw()     const { return fStraw;}
	  Float_t GetWidth()     const { return fWidth;}
	  Float_t GetDrftDist()  const { return fDist;}
	  Float_t  GetResidual() const { return fResidual;} */

	  Int_t    GetWire()     const { return fWire; }
	  Double_t GetLEtime()  const { return fLtdc;}
	  Double_t GetTEtime() const { return fTtdc;}
	  Double_t GetStraw()     const { return fStraw;}
	  Double_t GetWidth()     const { return fWidth;}
	  Double_t GetDrftDist()  const { return fDist;}
	  Double_t  GetResidual() const { return fResidual;}

	  void     SetWire (Short_t wire) {fWire = wire;}
	  void     SetLETime (Float_t ltdc) {fLtdc = ltdc;}
	  void     SetTETime (Float_t ttdc) {fTtdc = ttdc;}
	  void     SetStraw  (Float_t straw) {fStraw = straw;}
	  void     SetWidth  (Float_t width) {fWidth = width;}
	  void     SetDrftDist (Float_t dist) {fDist = dist;}
	  void     SetResidual  (Float_t resid)  {fResidual = resid;}

protected:

	Int_t     fWire; // Wire group number
	Double_t   fLtdc; // Leading TDC value  
	Double_t   fTtdc; // Trailing TDC value 
	Double_t   fStraw;  // Straw number, after demultiplexing
	Double_t   fWidth;
	Double_t   fDist; // Drift distance to wire
	Double_t   fResidual; // Residual

//	Short_t     fWire; // Wire group number
//	Float_t   fLtdc; // Leading TDC value  
//	Float_t   fTtdc; // Trailing TDC value 
//	Float_t   fStraw;  // Straw number, after demultiplexing
//	Float_t   fWidth;
//	Float_t   fDist; // Drift distance to wire
//	Float_t   fResidual; // Residual

	THaFppHit( const THaFppHit& ) {}
	THaFppHit& operator=( const THaFppHit& ) { return *this; }

	ClassDef(THaFppHit,1)             // FppHit class, number should be 1?
};

////////////////////////////////////////////////////////////////////////////////

#endif
