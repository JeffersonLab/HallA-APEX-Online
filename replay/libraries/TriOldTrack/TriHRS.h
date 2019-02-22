#ifndef ROOT_TriHRS
#define ROOT_TriHRS

//////////////////////////////////////////////////////////////////////////
//
// TriHRS
//
//////////////////////////////////////////////////////////////////////////

#include "TriSpectrometer.h"

class TriScintillator;

class TriHRS : public TriSpectrometer {
  
public:
  TriHRS( const char* name, const char* description );
  virtual ~TriHRS();

  virtual Int_t   FindVertices( TClonesArray& tracks );
  virtual Int_t   TrackCalc();
  virtual Int_t   TrackTimes( TClonesArray* tracks );

  Bool_t SetTrSorting( Bool_t set = kFALSE );
  Bool_t GetTrSorting() const;
  
protected:
//  TriScintillator *sc_ref;  // calculate time track hits this plane

  // Flag for fProperties indicating that tracks are to be sorted by chi2
  static const UInt_t kSortTracks = BIT(16);

  ClassDef(TriHRS,0) //A Hall A High Resolution Spectrometer
};

#endif

