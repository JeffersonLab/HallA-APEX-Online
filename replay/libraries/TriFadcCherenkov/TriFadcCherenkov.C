///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriFadcCherenkov                                                              //
//                                                                           //
// Class for a generic Cherenkov consisting of one or more phototubes.       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TriFadcCherenkov.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "VarDef.h"
#include "VarType.h"
#include "THaTrack.h"
#include "TClonesArray.h"
#include "TDatime.h"
#include "TMath.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;
using namespace Decoder;
//_____________________________________________________________________________
TriFadcCherenkov::TriFadcCherenkov( const char* name, const char* description,
			    THaApparatus* apparatus )
  : THaPidDetector(name,description,apparatus), fOff(0), fPed(0), fGain(0),
    fNThit(0), fT(0), fT_c(0), fNAhit(0), fA(0), fA_p(0), fA_c(0),
    foverflow(0), funderflow(0),fpedq(0)
{
  // Constructor
  fFADC=NULL;
}

//_____________________________________________________________________________
TriFadcCherenkov::TriFadcCherenkov()
  : THaPidDetector(), fOff(0), fPed(0), fGain(0), fT(0), fT_c(0),
    fA(0), fA_p(0), fA_c(0),foverflow(0), funderflow(0),fpedq(0)
{
  // Default constructor (for ROOT I/O)
}

//_____________________________________________________________________________
Int_t TriFadcCherenkov::ReadDatabase( const TDatime& date )
{
  // Read parameters from the database.
  // 'date' contains the date/time of the run being analyzed.

  const char* const here = "ReadDatabase";

  // Read database

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
    { "detmap",  &detmap,  kIntV },
    { "npmt",    &nelem,   kInt },
    { "angle",   &angle,   kDouble, 0, 1 },
    { 0 }
  };
  err = LoadDB( file, date, config_request, fPrefix );

  // Sanity checks
  if( !err && nelem <= 0 ) {
    Error( Here(here), "Invalid number of PMTs: %d. Must be > 0. "
	   "Fix database.", nelem );
    err = kInitError;
  }

  // Reinitialization only possible for same basic configuration
  if( !err ) {
    if( fIsInit && nelem != fNelem ) {
      Error( Here(here), "Cannot re-initalize with different number of PMTs. "
	     "(was: %d, now: %d). Detector not re-initialized.", fNelem, nelem );
      err = kInitError;
    } else
      fNelem = nelem;
  }
 
  UInt_t flags = THaDetMap::kFillLogicalChannel;

  if( !err && FillDetMap(detmap, flags, here) <= 0 ) {
    err = kInitError;  // Error already printed by FillDetMap
  }

  if( !err && (nelem = fDetMap->GetTotNumChan()) != 2*fNelem ) {
    Error( Here(here), "Number of detector map channels (%d) "
	   "inconsistent with 2*number of PMTs (%d)", nelem, 2*fNelem );
    err = kInitError;
  }

  if( err ) {
    fclose(file);
    return err;
  }

  DefineAxes( angle*TMath::DegToRad() );

  // Dimension arrays
  //FIXME: use a structure!
  UInt_t nval = fNelem;
  if( !fIsInit ) {
    // Calibration data
    fOff  = new Float_t[ nval ];
    fPed  = new Float_t[ nval ];
    fGain = new Float_t[ nval ];

    // Per-event data
    fT    = new Float_t[ nval ];
    fT_c  = new Float_t[ nval ];
    fA    = new Float_t[ nval ];
    fA_p  = new Float_t[ nval ];
    fA_c  = new Float_t[ nval ];

    foverflow  = new Int_t[ nval ]; 
    funderflow = new Int_t[ nval ];
    fpedq      = new Int_t[ nval ];

    fIsInit = true;
  }

  // Read calibration parameters

  // Set DEFAULT values here
  // TDC resolution (s/channel)
  //  fTdc2T = 0.1e-9;      // seconds/channel

  // Default TDC offsets (0), ADC pedestals (0) and ADC gains (1)
  memset( fOff, 0, nval*sizeof(fOff[0]) );
  memset( fPed, 0, nval*sizeof(fPed[0]) );

  fNPED = 1; //number of samples included in FADC pedestal sum
  fWin = 1;  //number of samples included in FADC integration

  for( UInt_t i=0; i<nval; ++i ) { fGain[i] = 1.0; }

  DBRequest calib_request[] = {
    { "tdc.offsets",      fOff,         kFloat, nval, 1 },
    { "adc.pedestals",    fPed,         kFloat, nval, 1 },
    { "adc.gains",        fGain,        kFloat, nval, 1 },
    //    { "tdc.res",          &fTdc2T,      kDouble },
    { "NPED",             &fNPED,        kInt},
    { "Win",              &fWin,         kInt},
    { 0 }
  };
  err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;

  return kOK;
}

//_____________________________________________________________________________
Int_t TriFadcCherenkov::DefineVariables( EMode mode )
{
  // Initialize global variables

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  RVarDef vars[] = {
    { "nthit",  "Number of Left paddles TDC times",  "fNThit" },
    { "nahit",  "Number of Right paddles TDC times", "fNAhit" },
    { "t",      "TDC values",                        "fT" },
    { "t_c",    "Corrected TDC values",              "fT_c" },
    { "a",      "ADC values",                        "fA" },
    { "a_p",    "Ped-subtracted ADC values ",        "fA_p" },
    { "a_c",    "Corrected ADC values",              "fA_c" },
    { "asum_p", "Sum of ADC minus pedestal values",  "fASUM_p" },
    { "asum_c", "Sum of corrected ADC amplitudes",   "fASUM_c" },
    { "trx",    "x-position of track in det plane",  "fTrackProj.THaTrackProj.fX" },
    { "try",    "y-position of track in det plane",  "fTrackProj.THaTrackProj.fY" },
    { "trpath", "TRCS pathlen of track to det plane","fTrackProj.THaTrackProj.fPathl" },
    { "noverflow",  "overflow bit of FADC pulse",    "foverflow" },
    { "nunderflow",  "underflow bit of FADC pulse",  "funderflow" },
    { "nbadped",  "pedestal quality bit of FADC pulse",   "fpedq" },
    { 0 }
  };
  return DefineVarsFromList( vars, mode );
}

//_____________________________________________________________________________
TriFadcCherenkov::~TriFadcCherenkov()
{
  // Destructor. Remove variables from global list.

  if( fIsSetup )
    RemoveVariables();
  if( fIsInit )
    DeleteArrays();
  fFADC=NULL;
}

//_____________________________________________________________________________
void TriFadcCherenkov::DeleteArrays()
{
  // Delete member arrays. Internal function used by destructor.

  delete [] fA_c;    fA_c    = NULL;
  delete [] fA_p;    fA_p    = NULL;
  delete [] fA;      fA      = NULL;
  delete [] fT_c;    fT_c    = NULL;
  delete [] fT;      fT      = NULL;

  delete [] fGain;   fGain   = NULL;
  delete [] fPed;    fPed    = NULL;
  delete [] fOff;    fOff    = NULL;

  delete [] foverflow; foverflow = NULL;
  delete [] funderflow; funderflow = NULL;
  delete [] fpedq;    fpedq    = NULL;

}

//_____________________________________________________________________________
void TriFadcCherenkov::Clear( Option_t* opt )
{
  // Clear event data

  fNThit = 0;                             // Number of mirrors with TDC times
  fNAhit = 0;                             // Number of mirrors with ADC ampls
  fASUM_p = 0.0;                          // Sum of ADC minus pedestal values
  fASUM_c = 0.0;                          // Sum of corrected ADC amplitudes
  if( !strchr(opt,'I') ) {
    const int lf = fNelem*sizeof(Float_t);
    memset( fT, 0, lf );                  // TDC times of channels
    memset( fT_c, 0, lf );                // Corrected TDC times of channels
    memset( fA, 0, lf );                  // ADC amplitudes of channels
    memset( fA_p, 0, lf );                // ADC minus ped values of channels
    memset( fA_c, 0, lf );                // Corrected ADC amplitudes of chans

    memset( foverflow, 0, fNelem*sizeof(foverflow[0]) );
    memset( funderflow, 0, fNelem*sizeof(funderflow[0]) );
    memset( fpedq, 0, fNelem*sizeof(fpedq[0]) );

  }
}

//_____________________________________________________________________________
Int_t TriFadcCherenkov::Decode( const THaEvData& evdata )
{
  // Decode Cherenkov data, correct TDC times and ADC amplitudes, and copy
  // the data into the local data members.
  // This implementation assumes that the first half of the detector map
  // entries corresponds to ADCs, and the second half, to TDCs.

  // Loop over all modules defined for Cherenkov detector
  for( Int_t i = 0; i < fDetMap->GetSize(); i++ ) {
    THaDetMap::Module* d = fDetMap->GetModule( i );
    bool adc = (d->model ? fDetMap->IsADC(d) : i < fDetMap->GetSize()/2 );

    if(adc) fFADC = dynamic_cast <Fadc250Module*> (evdata.GetModule(d->crate, d->slot));

    // Loop over all channels that have a hit.
    for( Int_t j = 0; j < evdata.GetNumChan( d->crate, d->slot ); j++) {

      Int_t chan = evdata.GetNextChan( d->crate, d->slot, j );
      if( chan < d->lo || chan > d->hi ) continue;     // Not one of my channels

      // Get the data. Aero mirrors are assumed to have only single hit (hit=0)
      Int_t data;
      if(adc)data = evdata.GetData(kPulseIntegral,d->crate,d->slot,chan,0);
      else data = evdata.GetData( d->crate, d->slot, chan, 0 );

      // Get the detector channel number, starting at 0
      Int_t k = d->first + ((d->reverse) ? d->hi - chan : chan - d->lo) - 1;

#ifdef WITH_DEBUG
      if( k<0 || k>= fNelem ) {
	Warning( Here("Decode()"), "Illegal detector channel: %d", k );
        continue;
      }
#endif

      if(adc){
          if(fFADC!=NULL){
               foverflow[k] = fFADC->GetOverflowBit(chan,0);
               funderflow[k] = fFADC->GetUnderflowBit(chan,0);
               fpedq[k] = fFADC->GetPedestalQuality(chan,0);
          }
          if(fpedq[k]==0)
           fPed[k]=fWin*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;

      }
      
      // Copy the data to the local variables.
      if ( adc ) {
	fA[k]   = data;
	fA_p[k] = data - fPed[k];
	fA_c[k] = fA_p[k] * fGain[k];
	// only add channels with signals to the sums
	if( fA_p[k] > 0.0 )
	  fASUM_p += fA_p[k];
	if( fA_c[k] > 0.0 )
	  fASUM_c += fA_c[k];
	fNAhit++;
      } else {
	fT[k]   = data;
	fT_c[k] = data - fOff[k];
	fNThit++;
      }
    }
  }

  if ( fDebug > 3 ) {
    printf("\nCherenkov %s:\n",GetPrefix());
    int ncol=3;
    for (int i=0; i<ncol; i++) {
      printf("  Mirror TDC   ADC  ADC_p  ");
    }
    printf("\n");

    for (int i=0; i<(fNelem+ncol-1)/ncol; i++ ) {
      for (int c=0; c<ncol; c++) {
	int ind = c*fNelem/ncol+i;
	if (ind < fNelem) {
	  printf("  %3d  %5.0f  %5.0f  %5.0f  ",ind+1,fT[ind],fA[ind],fA_p[ind]);
	} else {
	  //	  printf("\n");
	  break;
	}
      }
      printf("\n");
    }
  }

  return fNThit;
}

//_____________________________________________________________________________
Int_t TriFadcCherenkov::CoarseProcess( TClonesArray& tracks )
{
  // Reconstruct coordinates of where a particle track crosses
  // the Cherenkov plane, and copy the point into the fTrackProj array.

  CalcTrackProj( tracks );

  return 0;
}

//_____________________________________________________________________________
Int_t TriFadcCherenkov::FineProcess( TClonesArray& tracks )
{
  // Fine Cherenkov processing

  // Redo the track matching since tracks might have been thrown out
  // during the FineTracking stage.

  CalcTrackProj( tracks );

  return 0;
}

//_____________________________________________________________________________
ClassImp(TriFadcCherenkov)
