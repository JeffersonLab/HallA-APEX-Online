#ifndef ROOT_SciFi
#define ROOT_SciFi

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// SciFi                                                              //
//                                                                           //
// Generic Cherenkov detector.                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaNonTrackingDetector.h"
//#include "Fadc250Module.h"

class TClonesArray;

class SciFi : public THaNonTrackingDetector {

public:
  SciFi( const char* name, const char* description = "", THaApparatus* a = NULL );
  SciFi(); // for ROOT I/O
  virtual ~SciFi();

  void           ClearEvent();
  virtual void       Clear( Option_t* ="" );
  virtual Int_t      Decode( const THaEvData& );
  virtual Int_t      CoarseProcess( TClonesArray& tracks );
  virtual Int_t      FineProcess( TClonesArray& tracks );
  Float_t    GetAsum() const { return fASUM_c; }
	  
 protected:

  // Simple and quick routine to clear most vectors (of integers, floats,
  // doubles, etc...)
	  
  template<class T>
    void ResetVector(std::vector<T> &vec, T val = 0) {
    for(size_t i = 0; i < vec.size(); i++) {
      vec[i] = val;
    }
  }

  template<class T>
    void ResetVector(std::vector<std::vector<T> > &vec, T val = 0) {
    for(size_t i = 0; i < vec.size(); i++) {
      ResetVector(vec[i],val);
    }
  }

  

  // Calibration
  /* Float_t*   fOff;        // [fNelem] TDC offsets (chan) */
  Float_t*   fPed;        // [fNelem] ADC pedestals (chan)
  Float_t*   fGain;       // [fNelem] ADC gains

  Int_t    fNPED;        //number of samples included in FADC pedestal sum
  Int_t    fNSA;         //number of integration samples after threshold crossing
  Int_t    fNSB;         //number of integration samples before threshold crossing
  Int_t    fWin;         //total number of samples in FADC window
  Int_t    fTFlag;       //flag for FADC threshold on vs FADC threshold off
  Int_t    mod_Err_cnt=0;// Num of err calls for module issue (BANE)

  // Changes: getting rid of TDC values
  // Per-event data
  /* Int_t      fNThit;      // Number of mirrors with non zero TDC times */
  /* Float_t*   fT;          // [fNelem] Array of TDC times of channels */
  /* Float_t*   fT_c;        // [fNelem] Array of corrected TDC times of channels */
  //  Int_t      fNAhit;      // Number of ADCs with non zero ADC amplitudes
  Int_t*     fhitsperchannel; // hits for each channel
  Float_t*   fA;          // [fNelem] Array of ADC amplitudes of channels
  Int_t*     fAHits;      // Numbers of hits on each ADC
  Float_t*   fA_p;        // [fNelem] Array of ADC minus pedestal values of chans
  Float_t*   fA_c;        // [fNelem] Array of corrected ADC amplitudes of chans
  Float_t    fASUM_p;     // Sum of ADC minus pedestal values of channelso
  Float_t    fASUM_c;     // Sum of corrected ADC amplitudes of channels
  Float_t    fASUM;     // Sum of corrected ADC amplitudes of channels

  Int_t*      fAHits_raw;      // [fNelem] Array: hits for each channel calculated with raw mode variables
  Double_t*   fPtime_raw;   // [fNelem] Array: collects time of first peak (first sample to cross threshold)


  Double_t*   fTime_0;      // [fNelem] Array: Time of zeroth pulse
  Double_t*   fTime_1;      // [fNelem] Array: Time of first pulse
  Double_t*   fTime_2;      // [fNelem] Array: Time of second pulse
  Double_t*   fTime_3;      // [fNelem] Array: Time of third pulse
  Double_t*   fTime_4;      // [fNelem] Array: Time of fourth pulse


  //FADC
  Float_t*   fPeak;         // [fNelem] Array of FADC ADC peak values
  Float_t*   fT_FADC;       // [fNelem] Array of FADC TDC times of channels
  Float_t*   fT_FADC_c;     // [fNelem] Array of FADC corrected TDC times of channels
  Int_t* foverflow;         //[fNelem] FADC overflowbit
  Int_t* funderflow;        //[fNelem] FADC underflowbit
  Int_t* fpedq;             //[fNelem] FADC pedestal quality bit

  //  Decoder::Fadc250Module *fFADC;     //pointer to FADC250Module class
  Int_t fNhits;           // Number of hits (taken from SBSHCal.h)
  Int_t* fNhits_arr;           //[fNelem] number of hits for each PMT (taken from TriFadcCherenkov)
  
   

  // SciFi final output variables

  Int_t fX;                   // X-coordinate of hit
  Int_t fY;                   // Y-coordinate of hit


  //  Int_t fTime;                // time of (x,y) hit
  Int_t fhit_X_Y;             // Bool to show if there was a hit or not in any 
  Int_t* fhit_fibre;          // [fNelem] Bool to show if there was a hit or no
  
  std::vector<Int_t> fX_hits; // X-cords of hits
  Int_t fno_x_hits;           // Number of x-hits

  std::vector<Int_t> fY_hits; // Y-cords of hits
  Int_t fno_y_hits;           // Number of y-hits

  Bool_t first_hit;






  // raw mode variables
   
  std::vector<Int_t>   fNumSamples; // [fNelem] Number of samples in each ADC/module
  std::vector<std::vector<Double_t> > fA_raw; // [fNumSamples] Raw ADC samples
  std::vector<std::vector<Double_t> > fA_raw_p; // [fNumSamples] Ped correct ADC samples
  std::vector<std::vector<Double_t> > fA_raw_c; // [fNumSamples] Calibrated ADC samples
  std::vector<Float_t> fA_raw_sum; // [fNelem] Sum of raw ADC data for each block // MAPC

  std::vector< std::vector<Int_t> > fChanMap; // Logical channel numbers

  //  std::vector<std::vector<Int_t> > fA_ptime_raw;


  Int_t fOverall_sum; // Sum of pulse integrals for all channels

  Int_t fNoise; // Flag for noise events



  virtual Int_t  DefineVariables( EMode mode = kDefine );
          void   DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );

  ClassDef(SciFi,0)    //Generic Scifi class
};

////////////////////////////////////////////////////////////////////////////////

#endif
