#ifndef ROOT_THaFppPlane
#define ROOT_THaFppPlane

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaFppPlane                                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaSubDetector.h"
#include "TClonesArray.h"
#include "THaDetMap.h"
//#include "THaDetectorBase.h"

class THaEvData;
class THaFppHit;

class THaFppPlane : public THaSubDetector {

        friend class THaFpp;
public:

	THaFppPlane( const char* name="", const char* description="",
		THaDetectorBase* parent = NULL );
	virtual ~THaFppPlane();

	virtual Int_t   Decode( const THaEvData& ); // Raw data -> hits

	Int_t          GetNHits()     const { return fHits->GetLast()+1; }
	TClonesArray*  GetHits()      const { return fHits; }
	THaFppHit*     GetHit(Int_t i) const
	{ return (THaFppHit*)fHits->At(i); }

	Int_t          GetNGHits()     const { return fGHits->GetLast()+1; }
	TClonesArray*  GetGHits()      const { return fGHits; }
	THaFppHit*     GetGHit(Int_t i) const
	{ return (THaFppHit*)fGHits->At(i); }

	//	THaDetMap fDetMap;
 
	Double_t    GetX()   const { return fX; }
	Double_t    GetY()   const { return fY; }
	Double_t    GetZ()   const { return fZ; }
	Double_t    GetdX()  const { return fdX; }    // Size: numbers are 1/2 widths
	Double_t    GetdY()  const { return fdY; }
	Double_t    GetdZ()  const { return fdZ; }
	Double_t    GetZZ()  const { return fZZ; }
	Int_t       GetNWire() const { return fNWire; }     // Number of wires in plane
	Double_t    GetWAngle() const { return fWangle; }    // Wire angle
	Double_t    GetWSpac()  const { return fWspacing; }  // Wire spacing
	Double_t    GetUVX0()   const { return fUVXzero; } 
	Double_t    GetXoff()   const { return fXoff; }      // X offset
	Double_t    GetYoff()   const { return fYoff; }      // Y offset
	Double_t    GetPhoff()  const { return fPhoff; }     // Phi offset
	Double_t    GetThoff()  const { return fThoff; }     // Theta offset
	Double_t    GetPsoff()  const { return fPsoff; }     // Psi offset
	Double_t    GetStrawd() const { return fStrawd; }    // straw diameter

        void        Setitrgd (Int_t itr) {itrgood = itr;}
        void        Setstrwgd (Int_t str) {strwgood = str;}
        void        Setstrwbd (Int_t strb) {strwbad = strb;}
  
protected:

	//Use TClonesArray::GetLast()+1 to get the number of hits
        static const Int_t fMaxHit = 35;
	TClonesArray*  fHits;      // Hits 
	TClonesArray*  fGHits;      // Good hits 
	const char* fName;
        Int_t hitcount;

	// The following parameters are read from database.

	Double_t    fX;
	Double_t    fY;
	Double_t    fZ;
	Double_t    fdX;    // Size: numbers are 1/2 widths
	Double_t    fdY;
	Double_t    fdZ;
	Double_t    fZZ; 
	Int_t       fNWire;     // Number of wires in plane
	Double_t    fWangle;    // Wire angle
	Double_t    fWspacing;  // Wire spacing
	Double_t    fUVXzero; 
	Double_t    fXoff;      // X offset
	Double_t    fYoff;      // Y offset
	Double_t    fPhoff;     // Phi offset
	Double_t    fThoff;     // Theta offset
	Double_t    fPsoff;     // Psi offset
	Double_t    fStrawd;    // straw diameter

        Int_t       itrgood;
        Int_t       strwgood;
        Int_t       strwbad;

	THaDetectorBase* fFpp;  // Fpp detector to which this plane belongs

	virtual void  Clear( Option_t* opt="" );

//	virtual void  MakePrefix();
	virtual Int_t ReadDatabase( const TDatime& date );
	virtual Int_t DefineVariables( EMode mode = kDefine );

	ClassDef(THaFppPlane,0)             // FppPlane class
};

///////////////////////////////////////////////////////////////////////////////

#endif
