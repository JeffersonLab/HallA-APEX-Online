#ifndef ROOT_Gmp_Raster
#define ROOT_Gmp_Raster

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Gmp_Raster                                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaBeamDet.h"
#include "TVector.h"

class Gmp_Raster : public THaBeamDet {

public:
  Gmp_Raster( const char* name, const char* description = "",
		   THaApparatus* a = NULL );
  virtual ~Gmp_Raster();

  virtual Int_t      Decode( const THaEvData& );
  virtual Int_t      Process();

  virtual TVector3 GetPosition()  const { return fPosition[2]; }
  virtual TVector3 GetDirection() const { return fDirection; }

  // as soon as someone finds a better solution, he should change the following
  // lines, it is kind of ridiculus to have nine methodes to get the the components
  // of the beam position at various locations, but i do not know how else to 
  // get them into histograms, besides i would write my own event loop
  //lsm: upstream raster ("old") unchanged
  Double_t GetRawPosX() { return fRawPos(0); }
  Double_t GetRawPosY() { return fRawPos(1); }
  Double_t GetRawSlopeX() { return fRawSlope(0); }
  Double_t GetRawSlopeY() { return fRawSlope(1); }
 
  Double_t GetPosBPMAX() { return fPosition[0](0); }
  Double_t GetPosBPMAY() { return fPosition[0](1); }
  Double_t GetPosBPMAZ() { return fPosition[0](2); }

  Double_t GetPosBPMBX() { return fPosition[1](0); }
  Double_t GetPosBPMBY() { return fPosition[1](1); }
  Double_t GetPosBPMBZ() { return fPosition[1](2); }

  Double_t GetPosTarX() { return fPosition[2](0); }
  Double_t GetPosTarY() { return fPosition[2](1); }
  Double_t GetPosTarZ() { return fPosition[2](2); }



protected:

  void           ClearEvent();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );

  //  Gmp_Raster() {}
  //  Gmp_Raster( const Gmp_Raster& ) {}
  Gmp_Raster& operator=( const Gmp_Raster& ) { return *this; }


  TVector  fRawPos;        // current in Raster ADCs for position
  TVector  fRawSlope;      // current in Raster ADCs for the derivative
  //lsm: add for new raster
  //TVector  fRawPos2;        // current in Raster ADCs for position
  //TVector  fRawSlope2;      // current in Raster ADCs for the derivative

  TVector3  fPosition[3];   // Beam position at 1st, 2nd BPM or at the target (meters)
  TVector3  fDirection;  // Beam angle at the target (meters)

  TMatrix   fRaw2Pos[3];
  TVector3  fPosOff[3];

  TVector  fRasterFreq;
  TVector  fSlopePedestal;
  TVector  fRasterPedestal;

  Int_t fNfired;

  ClassDef(Gmp_Raster,0)   // Generic Raster class
};

////////////////////////////////////////////////////////////////////////////////

#endif
