#ifndef ROOT_Gmp_HRS
#define ROOT_Gmp_HRS

//////////////////////////////////////////////////////////////////////////
//
// Gmp_HRS
//
//////////////////////////////////////////////////////////////////////////

#include "THaSpectrometer.h"

class THaScintillator;

class Gmp_HRS : public THaSpectrometer {
  
public:
  Gmp_HRS( const char* name, const char* description );
  virtual ~Gmp_HRS();

  virtual Int_t   FindVertices( TClonesArray& tracks );
  virtual Int_t   TrackCalc();
  virtual Int_t   TrackTimes( TClonesArray* tracks );

  Bool_t SetTrSorting( Bool_t set = kFALSE );
  Bool_t GetTrSorting() const;
  
protected:
  THaScintillator *s2_scin;  // Instantiation of s2

  // Flag for fProperties indicating that tracks are to be sorted by chi2
  static const UInt_t kSortTracks = BIT(16);

  ClassDef(Gmp_HRS,0) //A Hall A High Resolution Spectrometer
};

#endif

