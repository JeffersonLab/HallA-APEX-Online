#ifndef ROOT_Tritium_HRS
#define ROOT_Tritium_HRS

//////////////////////////////////////////////////////////////////////////
//
// Tritium_HRS
//
//////////////////////////////////////////////////////////////////////////

#include "THaSpectrometer.h"

class THaNonTrackingDetector;

class Tritium_HRS : public THaSpectrometer {
  
public:
  Tritium_HRS( const char* name, const char* description );
  virtual ~Tritium_HRS();

  virtual Int_t   FindVertices( TClonesArray& tracks );
  virtual Int_t   TrackCalc();
  virtual Int_t   TrackTimes( TClonesArray* tracks );

  virtual Int_t   SetRefDet( const char* name );
  virtual Int_t   SetRefDet( const THaNonTrackingDetector* obj );

  THaNonTrackingDetector* GetRefDet() const { return fRefDet; }

  Int_t ReadDatabase( const TDatime& date );
  
  Bool_t GetTrSorting() const;
  Bool_t SetTrSorting( Bool_t set = kFALSE );
  Bool_t AutoStandardDetectors( Bool_t set = kTRUE );
  
  virtual EStatus Init( const TDatime& run_time );

protected:
  THaNonTrackingDetector* fRefDet;  // calculate time track hits this plane


  Int_t fFitMode = 0; // read in FitMode from DB
  Int_t fConvMode = 0; // read in ConvMode from DB
  Int_t fSeptum = 0; // read in ConvMode from DB
  
  // Bit flags
  enum {
    kSortTracks   = BIT(14), // Tracks are to be sorted by chi2
    kAutoStdDets  = BIT(15)  // Auto-create standard detectors if no "vdc"
  };

  ClassDef(Tritium_HRS,0) //A Hall A High Resolution Spectrometer
};

#endif

