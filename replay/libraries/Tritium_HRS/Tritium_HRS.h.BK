#ifndef ROOT_Tritium_HRS
#define ROOT_Tritium_HRS

//////////////////////////////////////////////////////////////////////////
//
// Tritium_HRS
//
//////////////////////////////////////////////////////////////////////////

#include "THaSpectrometer.h"

class THaScintillator;

class Tritium_HRS : public THaSpectrometer {
  
public:
  Tritium_HRS( const char* name, const char* description );
  virtual ~Tritium_HRS();

  virtual Int_t   FindVertices( TClonesArray& tracks );
  virtual Int_t   TrackCalc();
  virtual Int_t   TrackTimes( TClonesArray* tracks );

  Bool_t SetTrSorting( Bool_t set = kFALSE );
  Bool_t GetTrSorting() const;
  
protected:
  THaScintillator *s2_scin;  // Instantiation of s2

  // Flag for fProperties indicating that tracks are to be sorted by chi2
  static const UInt_t kSortTracks = BIT(16);

  ClassDef(Tritium_HRS,0) //A Hall A High Resolution Spectrometer
};

#endif

