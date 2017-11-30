///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriFadcXscin                                                                 //
//                                                                           //
// Class for a generic X-direction scintillator (hodoscope) consisting       //
// of multiple paddles with phototubes on both ends.                         //
//                                                                           //
// inherited from gmp_Xscin Barak Schmookler, May 2016 
// ReadDatabase is modified to read new DB for analyzer 1.6;
// ADC readout is modified to read out FADC data 
// 
///////////////////////////////////////////////////////////////////////////////

#include "TriFadcXscin.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "VarDef.h"
#include "VarType.h"
#include "THaTrack.h"
#include "TClonesArray.h"
#include "TMath.h"

#include "THaTrackProj.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace Decoder;
//_____________________________________________________________________________
TriFadcXscin::TriFadcXscin( const char* name,
		      const char* description,
		      Bool_t flip,
		      THaApparatus* apparatus ) :
  THaNonTrackingDetector(name,description,apparatus),
  fFlip(flip)
{
  // Constructor
  fTWalkPar = 0;

  fTrackProj = new TClonesArray( "THaTrackProj", 5 );
  fFADC = NULL;
}

//_____________________________________________________________________________
TriFadcXscin::TriFadcXscin( ) :
  THaNonTrackingDetector()
{
  // Constructor
  fTWalkPar = NULL;
  fTrackProj = NULL;
  fRA_c = fRA_p = fRA = fLA_c = fLA_p = fLA = fDownA_c = fDownA_p = fDownA = fUpA_c = fUpA_p = fUpA = NULL;
  fRT_c = fRT = fLT_c = fLT = fDownT_c = fDownT = fUpT_c = fUpT = NULL;
  fRGain = fLGain = fRPed = fLPed = fROff = fLOff = NULL;
  fTrigOff = fTime = fdTime = fXt = fXa = NULL;
  fHitPad = NULL;
  fFADC = NULL;

  floverflow = NULL;
  flunderflow = NULL;
  flpedq = NULL;
  froverflow = NULL;
  frunderflow = NULL;
  frpedq = NULL;


}

//_____________________________________________________________________________
THaAnalysisObject::EStatus TriFadcXscin::Init( const TDatime& date )
{
  // Extra initialization for scintillators: set up DataDest map

  if( THaNonTrackingDetector::Init( date ) )
    return fStatus;
//fDownT, fDownT_c, fDownA, fDownA_p, fDownA_c , fUpT, fUpT_c, fUpA, fUpA_p, fUpA_c 
  const DataDest tmp[NDEST] = {
    { &fRTNhit, &fRANhit, fRT, fRT_c, fRA, fRA_p, fRA_c, fROff, fRPed, fRGain },
    { &fLTNhit, &fLANhit, fLT, fLT_c, fLA, fLA_p, fLA_c, fLOff, fLPed, fLGain }
  };


  memcpy( fDataDest, tmp, NDEST*sizeof(DataDest) );

  return fStatus = kOK;
}

//_____________________________________________________________________________
Int_t TriFadcXscin::ReadDatabase( const TDatime& date )
{
  // Read this detector's parameters from the database file 'fi'.
  // This function is called by THaDetectorBase::Init() once at the
  // beginning of the analysis.
  // 'date' contains the date/time of the run being analyzed.

  // Read this detector's parameters from the database
 
  const char* const here = "ReadDatabase";

  FILE* file = OpenFile( date );
  if( !file ) return kFileError;

  // Read fOrigin and fSize (required!)
  Int_t err = ReadGeometry( file, date, true );
  if( err ) {
    fclose(file);
    return err;
  }

  vector<Int_t> detmap;
  Int_t nelem;
  Double_t angle = 0.0;

  // Read configuration parameters
  DBRequest config_request[] = {
    { "detmap",    &detmap,  kIntV },
    { "npaddles",  &nelem,   kInt },
    { "angle",     &angle,   kDouble, 0, 1 },
    { 0 }
  };
  err = LoadDB( file, date, config_request, fPrefix );

  // Sanity checks
  if( !err && nelem <= 0 ) {
    Error( Here(here), "Invalid number of paddles: %d", nelem );
    err = kInitError;
  }

  // Reinitialization only possible for same basic configuration
  if( !err ) {
    if( fIsInit && nelem != fNelem ) {
      Error( Here(here), "Cannot re-initalize with different number of paddles. "
             "(was: %d, now: %d). Detector not re-initialized.", fNelem, nelem );
      err = kInitError;
    } else
      fNelem = nelem;
  }

  UInt_t flags = THaDetMap::kFillLogicalChannel | THaDetMap::kFillModel;
  if( !err && FillDetMap(detmap, flags, here) <= 0 ) {
    err = kInitError;  // Error already printed by FillDetMap
  }

  if( !err && (nelem = fDetMap->GetTotNumChan()) != 4*fNelem ) {
    Error( Here(here), "Number of detector map channels (%d) "
           "inconsistent with 4*number of paddles (%d)", nelem, 4*fNelem );
    err = kInitError;
  }

  if( err ) {
    fclose(file);
    return err;
  }

  DefineAxes( angle*TMath::DegToRad() );

  // Dimension arrays
  //FIXME: use a structure!
  UInt_t nval = fNelem, nval_twalk = 2*nval;
  if( !fIsInit ) {
    fNTWalkPar = nval_twalk;
    // Calibration data
    fLOff  = new Double_t[ nval ];
    fROff  = new Double_t[ nval ];
    fLPed  = new Double_t[ nval ];
    fRPed  = new Double_t[ nval ];
    fLGain = new Double_t[ nval ];
    fRGain = new Double_t[ nval ];

    fTrigOff = new Double_t[ nval ];

    // Per-event data
    fLT   = new Double_t[ nval ];
    fLT_c = new Double_t[ nval ];
    fRT   = new Double_t[ nval ];
    fRT_c = new Double_t[ nval ];
    fLA   = new Double_t[ nval ];
    fLA_p = new Double_t[ nval ];
    fLA_c = new Double_t[ nval ];
    fRA   = new Double_t[ nval ];
    fRA_p = new Double_t[ nval ];
    fRA_c = new Double_t[ nval ];
    // A/B strangness
    fUpT   = new Double_t[ nval ];
    fUpT_c = new Double_t[ nval ];
    fUpA_p = new Double_t[ nval ];
    fUpA_c = new Double_t[ nval ];
    fUpA   = new Double_t[ nval ];
    
    fDownT   = new Double_t[ nval ];
    fDownT_c = new Double_t[ nval ];
    fDownA   = new Double_t[ nval ];
    fDownA_p = new Double_t[ nval ];
    fDownA_c = new Double_t[ nval ];

    fTWalkPar = new Double_t[ nval_twalk ];

    fHitPad = new Int_t[ nval ];
    fTime   = new Double_t[ nval ]; // analysis indexed by paddle (yes, inefficient)
    fdTime  = new Double_t[ nval ];
    fAmpl   = new Double_t[ nval ];

    fXt     = new Double_t[ nval ];
    fXa     = new Double_t[ nval ];

    floverflow = new Int_t[ nval ];
    flunderflow = new Int_t[ nval ];
    flpedq = new Int_t[ nval ];

    froverflow = new Int_t[ nval ];
    frunderflow = new Int_t[ nval ];
    frpedq = new Int_t[ nval ];

    fIsInit = true;
  }


 // Read calibration parameters

  // Set DEFAULT values here
  // TDC resolution (s/channel)
  fTdc2T = 0.1e-9;      // seconds/channel
  fResolution = kBig;   // actual timing resolution
  // Speed of light in the scintillator material
  fCn = 1.7e+8;    // meters/second
  // Attenuation length
  fAttenuation = 0.7; // inverse meters
  // Time-walk correction parameters
  fAdcMIP = 1.e10;    // large number for offset, so reference is effectively disabled
  // timewalk coefficients for tw = coeff*(1./sqrt(ADC-Ped)-1./sqrt(ADCMip))
  memset( fTWalkPar, 0, nval_twalk*sizeof(fTWalkPar[0]) );
  // trigger-timing offsets (s)
  memset( fTrigOff, 0, nval*sizeof(fTrigOff[0]) );

  fNPED = 1; //number of samples included in FADC pedestal sum
  fWin = 1;  //number of samples included in FADC integration

  // Default TDC offsets (0), ADC pedestals (0) and ADC gains (1)
  memset( fLOff, 0, nval*sizeof(fLOff[0]) );
  memset( fROff, 0, nval*sizeof(fROff[0]) );
  memset( fLPed, 0, nval*sizeof(fLPed[0]) );
  memset( fRPed, 0, nval*sizeof(fRPed[0]) );
  for( UInt_t i=0; i<nval; ++i ) { fLGain[i] = 1.0; fRGain[i] = 1.0; }

  DBRequest calib_request[] = {
    { "L.off",            fLOff,         kDouble, nval, 1 },
    { "R.off",            fROff,         kDouble, nval, 1 },
    { "L.ped",            fLPed,         kDouble, nval, 1 },
    { "R.ped",            fRPed,         kDouble, nval, 1 },
    { "L.gain",           fLGain,        kDouble, nval, 1 },
    { "R.gain",           fRGain,        kDouble, nval, 1 },
    { "tdc.res",          &fTdc2T,       kDouble },
    { "Cn",               &fCn,          kDouble },
    { "MIP",              &fAdcMIP,      kDouble, 0, 1 },
    { "timewalk_params",  fTWalkPar,     kDouble, nval_twalk, 1 },
    { "retiming_offsets", fTrigOff,      kDouble, nval, 1 },
    { "avgres",           &fResolution,  kDouble, 0, 1 },
    { "atten",            &fAttenuation, kDouble, 0, 1 },
    { "NPED",             &fNPED,        kInt},
    { "Win",              &fWin,         kInt},
    { 0 }
  };
  err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;

  if( fResolution == kBig )
    fResolution = fTdc2T;

  // If doing debugging, print the calibration parameters we've just read
  if ( fDebug > 1 ) {
    cout << '\n' << GetPrefix() << " calibration parameters: " << endl;;
    for ( DBRequest *li = calib_request; li->name; li++ ) {
      cout << "  " << li->name;
      UInt_t maxc = li->nelem;
      if (maxc==0)maxc=1;
      for (UInt_t i=0; i<maxc; i++) {
        if (li->type==kDouble) cout << "  " << ((Double_t*)li->var)[i];
        if (li->type==kInt) cout << "  " << ((Int_t*)li->var)[i];
      }
      cout << endl;
    }
  }

  return kOK;
}

//_____________________________________________________________________________
Int_t TriFadcXscin::DefineVariables( EMode mode )
{
  // Initialize global variables and lookup table for decoder

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  // Register variables in global list

  RVarDef vars[] = {
    { "nlthit", "Number of Left paddles TDC times",  "fLTNhit" },
    { "nrthit", "Number of Right paddles TDC times", "fRTNhit" },
    { "nlahit", "Number of Left paddles ADCs amps",  "fLANhit" },
    { "nrahit", "Number of Right paddles ADCs amps", "fRANhit" },
    { "lt",     "TDC values left side",              "fLT" },
    { "lt_c",   "Corrected times left side",         "fLT_c" },
    { "rt",     "TDC values right side",             "fRT" },
    { "rt_c",   "Corrected times right side",        "fRT_c" },
    { "la",     "ADC values left side",              "fLA" },
    { "la_p",   "Corrected ADC values left side",    "fLA_p" },
    { "la_c",   "Corrected ADC values left side",    "fLA_c" },
    { "ra",     "ADC values right side",             "fRA" },
    { "ra_p",   "Corrected ADC values right side",   "fRA_p" },
    { "ra_c",   "Corrected ADC values right side",   "fRA_c" },
    { "ut",     "TDC values top side",               "fUpT" },
    { "ut_c",   "Corrected times top side",          "fUpT_c" },
    { "dt",     "TDC values bottom side",            "fDownT" },
    { "dt_c",   "Corrected times bottom side",       "fDownT_c" },
    { "ua",     "ADC values top side",               "fUpA" },
    { "ua_p",   "Corrected ADC values top side",     "fUpA_p" },
    { "ua_c",   "Corrected ADC values top side",     "fUpA_c" },
    { "da",     "ADC values bottom side",            "fDownA" },
    { "da_p",   "Corrected ADC values bottom side",  "fDownA_p" },
    { "da_c",   "Corrected ADC values bottom side",  "fDownA_c" },
    { "nthit",  "Number of paddles with l&r TDCs",   "fNhit" },
    { "t_pads", "Paddles with l&r coincidence TDCs", "fHitPad" },
    { "x_t",    "x-position from timing (m)",        "fXt" },
    { "x_adc",  "x-position from amplitudes (m)",    "fXa" },
    { "time",   "Time of hit at plane (s)",          "fTime" },
    { "dtime",  "Est. uncertainty of time (s)",      "fdTime" },
    { "dedx",   "dEdX-like deposited in paddle",     "fAmpl" },
    { "troff",  "Trigger offset for paddles",        "fTrigOff"},
    { "trn",    "Number of tracks for hits",         "GetNTracks()" },
    { "trx",    "x-position of track in det plane",  "fTrackProj.THaTrackProj.fX" },
    { "try",    "y-position of track in det plane",  "fTrackProj.THaTrackProj.fY" },
    { "trpath", "TRCS pathlen of track to det plane","fTrackProj.THaTrackProj.fPathl" },
    { "trdy",   "track deviation in y-position (m)", "fTrackProj.THaTrackProj.fdX" }, //Does not have a 'fdY' variable...but not a big deal
    { "trpad",  "paddle-hit associated with track",  "fTrackProj.THaTrackProj.fChannel" },
    { "loverflow",  "overflow bit of FADC pulse left side",         "floverflow" },
    { "lunderflow", "underflow bit of FADC pulse left side",        "flunderflow" },
    { "lbadped",    "pedestal quality bit of FADC pulse left side", "flpedq" },
    { "roverflow",  "overflow bit of FADC pulse right side",        "froverflow" },
    { "runderflow", "underflow bit of FADC pulse right side",       "frunderflow" },
    { "rbadped",    "pedestal quality bit of FADC pulse right side","frpedq" },
    { 0 }
  };
  return DefineVarsFromList( vars, mode );
}

//_____________________________________________________________________________
TriFadcXscin::~TriFadcXscin()
{
  // Destructor. Remove variables from global list.

  if( fIsSetup )
    RemoveVariables();
  if( fIsInit )
    DeleteArrays();
  if (fTrackProj) {
    fTrackProj->Clear();
    delete fTrackProj; fTrackProj = 0;
  }
}

//_____________________________________________________________________________
void TriFadcXscin::DeleteArrays()
{
  // Delete member arrays. Used by destructor.

  delete [] fRA_c;    fRA_c    = NULL;
  delete [] fRA_p;    fRA_p    = NULL;
  delete [] fRA;      fRA      = NULL;
  delete [] fLA_c;    fLA_c    = NULL;
  delete [] fLA_p;    fLA_p    = NULL;
  delete [] fLA;      fLA      = NULL;
  delete [] fRT_c;    fRT_c    = NULL;
  delete [] fRT;      fRT      = NULL;
  delete [] fLT_c;    fLT_c    = NULL;
  delete [] fLT;      fLT      = NULL;
  delete [] fDownA_c;    fDownA_c    = NULL;
  delete [] fDownA_p;    fDownA_p    = NULL;
  delete [] fDownA;      fDownA      = NULL;
  delete [] fUpA_c;    fUpA_c    = NULL;
  delete [] fUpA_p;    fUpA_p    = NULL;
  delete [] fUpA;      fUpA      = NULL;
  delete [] fDownT_c;    fDownT_c    = NULL;
  delete [] fDownT;      fDownT      = NULL;
  delete [] fUpT_c;    fUpT_c    = NULL;
  delete [] fUpT;      fUpT      = NULL;
  
  delete [] fRGain;   fRGain   = NULL;
  delete [] fLGain;   fLGain   = NULL;
  delete [] fRPed;    fRPed    = NULL;
  delete [] fLPed;    fLPed    = NULL;
  delete [] fROff;    fROff    = NULL;
  delete [] fLOff;    fLOff    = NULL;
  delete [] fTWalkPar; fTWalkPar = NULL;
  delete [] fTrigOff; fTrigOff = NULL;

  delete [] fHitPad;  fHitPad  = NULL;
  delete [] fTime;    fTime    = NULL;
  delete [] fdTime;   fdTime   = NULL;
  delete [] fXt;      fXt      = NULL;
  delete [] fXa;      fXa      = NULL;
  delete [] floverflow;  floverflow  = NULL;
  delete [] flunderflow; flunderflow = NULL;
  delete [] flpedq;      flpedq      = NULL;

  delete [] froverflow;  froverflow  = NULL;
  delete [] frunderflow; frunderflow = NULL;
  delete [] frpedq;      frpedq      = NULL;

}

//_____________________________________________________________________________
inline 
void TriFadcXscin::ClearEvent()
{
  // Reset per-event data.

  const int lf = fNelem*sizeof(Double_t);
  fLTNhit = 0;                            // Number of Left paddles TDC times
  memset( fLT, 0, lf );                   // Left paddles TDCs
  memset( fLT_c, 0, lf );                 // Left paddles corrected times
  fRTNhit = 0;                            // Number of Right paddles TDC times
  memset( fRT, 0, lf );                   // Right paddles TDCs
  memset( fRT_c, 0, lf );                 // Right paddles corrected times
  fLANhit = 0;                            // Number of Left paddles ADC amps
  memset( fLA, 0, lf );                   // Left paddles ADCs
  memset( fLA_p, 0, lf );                 // Left paddles ADC minus pedestal
  memset( fLA_c, 0, lf );                 // Left paddles corrected ADCs
  fRANhit = 0;                            // Number of Right paddles ADC smps
  memset( fRA, 0, lf );                   // Right paddles ADCs
  memset( fRA_p, 0, lf );                 // Right paddles ADC minus pedestal
  memset( fRA_c, 0, lf );                 // Right paddles corrected ADCs
  
  memset( fDownA, 0, lf );                   // Right paddles ADCs
  memset( fDownA_p, 0, lf );                 // Right paddles ADC minus pedestal
  memset( fDownA_c, 0, lf );                 // Right paddles corrected ADCs
  memset( fDownT, 0, lf );                   // Right paddles TDCs
  memset( fDownT_c, 0, lf );                 // Right paddles corrected times
  
  memset( fUpT, 0, lf );                   // Left paddles TDCs
  memset( fUpT_c, 0, lf );                 // Left paddles corrected times
  memset( fUpA, 0, lf );                   // Left paddles ADCs
  memset( fUpA_p, 0, lf );                 // Left paddles ADC minus pedestal
  memset( fUpA_c, 0, lf );                 // Left paddles corrected ADCs

  fNhit = 0;
  memset( fHitPad, 0, fNelem*sizeof(fHitPad[0]) );
  memset( fTime, 0, lf );
  memset( fdTime, 0, lf );
  memset( fXt, 0, lf );
  memset( fXa, 0, lf );

  memset( floverflow,  0, fNelem*sizeof(floverflow[0]) );
  memset( flunderflow, 0, fNelem*sizeof(flunderflow[0]) );
  memset( flpedq,      0, fNelem*sizeof(flpedq[0]) );
  
  memset( froverflow,  0, fNelem*sizeof(froverflow[0]) );
  memset( frunderflow, 0, fNelem*sizeof(frunderflow[0]) );
  memset( frpedq,      0, fNelem*sizeof(frpedq[0]) );

  fTrackProj->Clear();
}

//_____________________________________________________________________________
Int_t TriFadcXscin::Decode( const THaEvData& evdata )
{
  // Decode scintillator data, correct TDC times and ADC amplitudes, and copy
  // the data to the local data members.
  // This implementation makes the following assumptions about the detector map:
  // - The first half of the map entries corresponds to ADCs, 
  //   the second half, to TDCs.
  // - The first fNelem detector channels correspond to the PMTs on the
  //   right hand side, the next fNelem channels, to the left hand side.
  //   (Thus channel numbering for each module must be consecutive.)

  ClearEvent();

  // Loop over all modules defined for Scintillator 1 detector

  for( Int_t i = 0; i < fDetMap->GetSize(); i++ ) {
    THaDetMap::Module* d = fDetMap->GetModule( i );
    bool adc = ( d->model ? fDetMap->IsADC(d) : (i < fDetMap->GetSize()/2) );

    if(adc) fFADC = dynamic_cast <Fadc250Module*> (evdata.GetModule(d->crate, d->slot));

    // Loop over all channels that have a hit.
    for( Int_t j = 0; j < evdata.GetNumChan( d->crate, d->slot ); j++) {

      Int_t chan = evdata.GetNextChan( d->crate, d->slot, j );
      if( chan < d->lo || chan > d->hi ) continue;     // Not one of my channels

#ifdef WITH_DEBUG      
      Int_t nhit = evdata.GetNumHits(d->crate, d->slot, chan);
      if( nhit > 1 )
	Warning( Here("Decode"), "Event %d: %d hits on %s channel %d/%d/%d",
		 evdata.GetEvNum(),
		 nhit, adc ? "ADC" : "TDC", d->crate, d->slot, chan );
#endif
      // Get the data. Scintillators are assumed to have only single hit (hit=0)
      Int_t data;
      if(adc)data = evdata.GetData(kPulseIntegral,d->crate,d->slot,chan,0);
      else data = evdata.GetData( d->crate, d->slot, chan, 0 );

      // Get the detector channel number, starting at 0
      Int_t k = d->first + chan - d->lo - 1;   

#ifdef WITH_DEBUG      
      if( k<0 || k>NDEST*fNelem ) {
	// Indicates bad database
	Warning( Here("Decode()"), "Illegal detector channel: %d", k );
	continue;
      }
//        cout << "adc,j,k = " <<adc<<","<<j<< ","<<k<< endl;
#endif
     
      // Copy the data to the local variables.
      DataDest* dest = fDataDest + k/fNelem;
      //decide wether use event by event pedestal
      int jj=0;
      jj = k/fNelem; 
      k = k % fNelem;
      if(adc){
          if(fFADC!=NULL){
            if(jj==1){   
		floverflow[k] = fFADC->GetOverflowBit(chan,0);
                flunderflow[k] = fFADC->GetUnderflowBit(chan,0);
                flpedq[k] = fFADC->GetPedestalQuality(chan,0);
              }
            else {
                froverflow[k] = fFADC->GetOverflowBit(chan,0);
                frunderflow[k] = fFADC->GetUnderflowBit(chan,0);
                frpedq[k] = fFADC->GetPedestalQuality(chan,0);
            }
          }
        if(jj==1 && flpedq[k]==0)
          dest->ped[k]=fWin*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;

        if(jj==0 && frpedq[k]==0)         
          dest->ped[k]=fWin*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;

      }
     
      if( adc ) {
	dest->adc[k]   = static_cast<Double_t>( data );
	dest->adc_p[k] = data - dest->ped[k]; 
	dest->adc_c[k] = dest->adc_p[k] * dest->gain[k];
	(*dest->nahit)++;
      } else {
	dest->tdc[k]   = static_cast<Double_t>( data );
	dest->tdc_c[k] = (data - dest->offset[k])*fTdc2T;
	(*dest->nthit)++;
      }  
    }
  }
  if ( fDebug > 3 ) {
    printf("\n\nEvent %d   Trigger %d Scintillator %s\n:",
	   evdata.GetEvNum(), evdata.GetEvType(), GetPrefix() );
    printf("   paddle  Left(TDC    ADC   ADC_p)   Right(TDC   ADC   ADC_p)\n");
    for ( int i=0; i<fNelem; i++ ) {
      printf("     %2d     %5.0f    %5.0f  %5.0f     %5.0f    %5.0f  %5.0f\n",
	     i+1,fLT[i],fLA[i],fLA_p[i],fRT[i],fRA[i],fRA_p[i]);
    }
  }
  
  return fLTNhit+fRTNhit;
}

//_____________________________________________________________________________
Int_t TriFadcXscin::ApplyCorrections( void )
{
  // Apply the ADC/TDC corrections to get the 'REAL' relevant 
  // TDC and ADC values. No tracking needs to have been done yet.
  //
  // Permits the dividing up of the decoding step (events could come from
  // a different source) to the applying of corrections. For ease when
  // trying to optimize calibrations
  //
  Int_t nlt=0, nrt=0, nla=0, nra=0;
  for (Int_t i=0; i<fNelem; i++) {
    if (fLA[i] != 0.) {
      fLA_p[i] = fLA[i] - fLPed[i];
      fLA_c[i] = fLA_p[i]*fLGain[i];
      nla++;
    }
    if (fRA[i] != 0.) {
      fRA_p[i] = fRA[i] - fRPed[i];
      fRA_c[i] = fRA_p[i]*fRGain[i];
      nra++;
    }
    if (fLT[i] != 0.) {
      fLT_c[i] = (fLT[i] - fLOff[i])*fTdc2T - TimeWalkCorrection(i,kLeft);
      nlt++;
    }
    if (fRT[i] != 0.) {
      fRT_c[i] = (fRT[i] - fROff[i])*fTdc2T - TimeWalkCorrection(i,kRight);
      nrt++;
    }
 
  	if(fFlip){
  		fUpT[i] = fLT[i];
 	 	fUpT_c[i] = fLT_c[i];
  		fUpA[i] = fLA[i];
  		fUpA_p[i] = fLA_p[i];
  		fUpA_c[i] = fLA_c[i];
  		fDownT[i] = fRT[i];
  		fDownT_c[i] = fRT_c[i];
  		fDownA[i] = fRA[i];
  		fDownA_p[i] = fRA_p[i];
  		fDownA_c[i] = fRA_c[i];  		
    	}
    else{
  		fUpT[i] = fRT[i];
 	 	fUpT_c[i] = fRT_c[i];
  		fUpA[i] = fRA[i];
  		fUpA_p[i] = fRA_p[i];
  		fUpA_c[i] = fRA_c[i];
  		fDownT[i] = fLT[i];
  		fDownT_c[i] = fLT_c[i];
  		fDownA[i] = fLA[i];
  		fDownA_p[i] = fLA_p[i];
 		fDownA_c[i] = fLA_c[i];
		}
  }
  // returns FALSE (0) if all matches up
  return !(fLTNhit==nlt && fLANhit==nla && fRTNhit==nrt && fRANhit==nra );
 
}

//_____________________________________________________________________________
Double_t TriFadcXscin::TimeWalkCorrection(const Int_t& paddle,
					     const ESide side)
{
  // Calculate the time-walk correction. The timewalk might be
  // dependent upon the specific PMT, so information about exactly
  // which PMT fired is required.
  Double_t adc=0;
  if (side == kLeft)
    adc = fLA_p[paddle];
  else
    adc = fRA_p[paddle];

  if (fNTWalkPar<=0 || !fTWalkPar) return 0.; // uninitialized return safe 0

  // get the ADC value above the pedestal
  if ( adc <=0. ) return 0.;
  
  // we have an arbitrary timing offset, and will declare here that
  // for a MIP ( peak ~2000 ADC channels ) the timewalk correction is 0
  
  Double_t ref = fAdcMIP;
  Double_t tw(0), tw_ref(0.);
  int npar = fNTWalkPar/(2*fNelem);
  
  Double_t *par = &(fTWalkPar[npar*(side*fNelem+paddle)]);

  tw = par[0]*pow(adc,-.5);
  tw_ref = par[0]*pow(ref,-.5);

  return tw-tw_ref;
}

//_____________________________________________________________________________
Int_t TriFadcXscin::CoarseProcess( TClonesArray& /* tracks */ )
{
  // Calculation of coordinates of particle track cross point with scint
  // plane in the detector coordinate system. For this, parameters of track 
  // reconstructed in THaVDC::CoarseTrack() are used.
  //
  // Apply corrections and reconstruct the complete hits.
  //
  static const Double_t sqrt2 = TMath::Sqrt(2.);
  
  ApplyCorrections();

  // count the number of paddles with complete TDC hits
  // Fill in information available from timing
  fNhit = 0;
  for (int i=0; i<fNelem; i++) {
    if (fLT[i]>0 && fRT[i]>0) {
      fHitPad[fNhit++] = i;
      fTime[i] = .5*(fLT_c[i]+fRT_c[i])-fSize[0]/fCn;
      fdTime[i] = fResolution/sqrt2;
      
      //+x is down...
      //For RHRS S0, PMT Left is at top...set flip to true
      //For LHRS S0, PMT Left is at bottom...set flip to false
      if(fFlip)
	fXt[i] = .5*fCn*(fLT_c[i]-fRT_c[i]);
      else
	fXt[i] = .5*fCn*(fRT_c[i]-fLT_c[i]);
    }

    // rough calculation of position from ADC reading
    if (fLA_c[i]>0&&fRA_c[i]>0) {
      fXa[i] = TMath::Log(fLA_c[i]/fRA_c[i])/(2.*fAttenuation);
      // rough dE/dX-like quantity, not correcting for track angle
      fAmpl[i] = TMath::Sqrt(fLA_c[i]*fRA_c[i]*TMath::Exp(fAttenuation*2*fSize[0]))
	/ fSize[2];
    }
  }
  
  return 0;
}

//_____________________________________________________________________________
Int_t TriFadcXscin::FineProcess( TClonesArray& tracks )
{
  // Reconstruct coordinates of particle track cross point with scintillator
  // plane, and copy the data into the following local data structure:
  //
  // Units of measurements are meters.

  // Calculation of coordinates of particle track cross point with scint
  // plane in the detector coordinate system. For this, parameters of track 
  // reconstructed in THaVDC::FineTrack() are used.

  int n_track = tracks.GetLast()+1;   // Number of reconstructed tracks
  
  Double_t dpady = (2.*fSize[1])/(fNelem); // width of a paddle
  // center of paddle '0'
  Double_t pady0 = -dpady*(fNelem-1)*.5;
  
  for ( int i=0; i<n_track; i++ ) {
    THaTrack* theTrack = static_cast<THaTrack*>( tracks[i] );

    Double_t pathl=kBig, xc=kBig, yc=kBig, dy=kBig;
    Int_t pad=-1;
    
    if ( ! CalcTrackIntercept(theTrack, pathl, xc, yc) ) { // failed to hit
      new ( (*fTrackProj)[i] )
	THaTrackProj(xc,yc,pathl,dy,pad);
      continue;
    }
    
    // xc, yc are the positions of the track intercept
    //  _RELATIVE TO THE DETECTOR PLANE's_ origin.
    //
    // look through set of complete hits for closest match
    // loop through due to possible poor matches
    dy = kBig;
    for ( Int_t j=0; j<fNhit; j++ ) {
      Double_t dy2 = ( pady0 + fHitPad[j]*dpady) - yc;
      if (TMath::Abs(dy2) < TMath::Abs(dy) ) {
	pad = fHitPad[j];
	dy = dy2;
      }
      else if (pad>=0) break; // stop after finding closest in X
    }

    // record information, found or not
    new ( (*fTrackProj)[i] )
      THaTrackProj(xc,yc,pathl,dy,pad);
  }
  
  return 0;
}

ClassImp(TriFadcXscin)
////////////////////////////////////////////////////////////////////////////////
