///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Scifi                                                                     //
//                                                                           //
// Class for Scifi detector read out by 4 fadc250 modules with 16 channels   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//#include "TriFadcCherenkov.h"
#include "SciFi.h"
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
#define MAX_FADC_SAMPLES 400 // Max number of samples allowed
#include "Fadc250Module.h"


using namespace std;
using namespace Decoder;
const int debug=1;

//_____________________________________________________________________________
SciFi::SciFi( const char* name, const char* description, THaApparatus* apparatus )
: THaNonTrackingDetector(name,description,apparatus), fPed(0), fGain(0), fA(0),
  fAHits(0), fA_p(0), fA_c(0),fPeak(0),fT_FADC(0),fT_FADC_c(0),
  foverflow(0), funderflow(0),fpedq(0), fNhits(0), fNhits_arr(0), fNoise(0),
  fX(0),fY(0),fhit_X_Y(0),fno_x_hits(0),fno_y_hits(0),fTime_0(0)
{
  // Constructor
//  fFADC=NULL;
}

//_____________________________________________________________________________
SciFi::SciFi()
: THaNonTrackingDetector(), fPed(0), fGain(0), fA(0), fAHits(0),
  fA_p(0), fA_c(0),fPeak(0),foverflow(0), funderflow(0),fpedq(0),fNhits(0), fNhits_arr(0), fNoise(0),
  fX(0),fY(0),fhit_X_Y(0),fno_x_hits(0),fno_y_hits(0),fTime_0(0)
{
  // Default constructor (for ROOT I/O)
}

//_____________________________________________________________________________
Int_t SciFi::ReadDatabase( const TDatime& date )
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

  vector<Int_t> detmap, chanmap;
  Int_t nelem;
  Double_t angle = 0.0;

  // Read configuration parameters
  DBRequest config_request[] = {
    { "detmap",  &detmap,  kIntV },
    { "chanmap", &chanmap, kIntV },
    { "nch",    &nelem,   kInt },
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
 
  //  UInt_t flags = THaDetMap::kFillLogicalChannel | THaDetMap::kFillModel;

  UInt_t flags = THaDetMap::kFillModel;

  // potential change depending on how database is structured 
  // what is difference between channel and logical channel number

  // if( !err && FillDetMap(detmap, flags, here) <= 0 ) {
  //   err = kInitError;  // Error already printed by FillDetMap
  // }


  // Change:: Don't think this is needed
 // if( !err && (nelem = fDetMap->GetTotNumChan()) != 2*fNelem ) {
  //   Error( Here(here), "Number of detector map channels (%d) "
  // 	   "inconsistent with 2*number of PMTs (%d)", nelem, 2*fNelem );
  //   err = kInitError;
  // }

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
    // fOff  = new Float_t[ nval ];
    fPed  = new Float_t[ nval ];
    fGain = new Float_t[ nval ];

    // Per-event data
    // fT    = new Float_t[ nval ];
    // fT_c  = new Float_t[ nval ];
    fhitsperchannel = new Int_t[ nval];
    fA    = new Float_t[ nval ];
    fAHits = new Int_t[ 64];
    fA_p  = new Float_t[ nval ];
    fA_c  = new Float_t[ nval ];

    fPeak      = new Float_t[ nval ];
    fT_FADC    = new Float_t[ nval ];
    fT_FADC_c  = new Float_t[ nval ];
    foverflow  = new Int_t[ nval ]; 
    funderflow = new Int_t[ nval ];
    fpedq      = new Int_t[ nval ];
    fNhits_arr     = new Int_t[ nval ];

    fhit_fibre =  new Int_t[ nval ];

    fAHits_raw = new Int_t[ nval];
    fPtime_raw = new Double_t[ nval];
    
    fTime_fine_0 = new Double_t[ nval];

    fTime_0 = new Double_t[ nval];
    fTime_1 = new Double_t[ nval];
    fTime_2 = new Double_t[ nval];
    fTime_3 = new Double_t[ nval];
    fTime_4 = new Double_t[ nval];


    //fIsInit = true;
  }

  // Read calibration parameters

  // Set DEFAULT values here
  // TDC resolution (s/channel)
  //  fTdc2T = 0.1e-9;      // seconds/channel

  // Default TDC offsets (0), ADC pedestals (0) and ADC gains (1)
  //  memset( fOff, 0, nval*sizeof(fOff[0]) );
  memset( fPed, 0, nval*sizeof(fPed[0]) );

  fNPED = 1; //number of samples included in FADC pedestal sum
  fNSA = 1;  //number of integration samples after threshold crossing
  fNSB = 1;  //number of integration samples before threshold crossing
  fWin = 1;  //total number of sample in FADC window
  fTFlag = 1;  //Threshold On: 1, Off: 0
  
  fno_x_hits = 0;
  fno_y_hits = 0;

  fno_x_hits = 0;
  fno_y_hits = 0;




  for( UInt_t i=0; i<nval; ++i ) { fGain[i] = 1.0; }

  DBRequest calib_request[] = {
    // { "tdc.offsets",      fOff,         kFloat, nval, 1 },
    { "pedestals",    fPed,         kFloat, nval, 1 },
    { "gains",        fGain,        kFloat, nval, 1 },
    //    { "tdc.res",          &fTdc2T,      kDouble },
    { "NPED",             &fNPED,        kInt, 1, 1},
    { "NSA",              &fNSA,         kInt},
    { "NSB",              &fNSB,         kInt},
    { "Win",              &fWin,         kInt},
    { "TFlag",            &fTFlag,       kInt},
    { 0 }
  };
  err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;


  if( !fIsInit ) {
    // Compute block positions and creates blocks array
    //    fBlkGrid.clear();
    //    fBlkGrid.resize(fNrows);
    //    for (int i=0;i<fNrows;i++) fBlkGrid[i].resize(fNcols);
    //2//fClusters = new SBSHCalCluster*[fMaxNClust];
    // fBlocks.clear();
    // fBlocks.resize(fNelem); 
    fA_raw.resize(fNelem);
    fA_raw_p.resize(fNelem);
    fA_raw_c.resize(fNelem);
    fNumSamples.resize(fNelem);

    
    for(Int_t i = 0; i < fNelem; i++){

      fA_raw[i].resize(MAX_FADC_SAMPLES);
      fA_raw_p[i].resize(MAX_FADC_SAMPLES);
      fA_raw_c[i].resize(MAX_FADC_SAMPLES);


    }



  
    fA_raw_sum.resize(fNelem); // MAPC
    fX_hits.clear();
    fY_hits.clear();

    // Yup, hard-coded in because it's only a test
    // TODO: Fix me, don't hard code it in
    //    fMaxNClust = 9;
    //    fE = new Float_t[UInt_t(fMaxNClust)];
    //    fE_c = new Float_t[UInt_t(fMaxNClust)];
    //    fX = new Float_t[UInt_t(fMaxNClust)];
    // fY = new Float_t[UInt_t(fMaxNClust)];
    // fMult = new Int_t[UInt_t(fMaxNClust)];

    // fNblk = new Int_t[UInt_t(fNclublk)];
    // fEblk = new Float_t[UInt_t(fNclublk)];

    // for( int r=0; r<nrows; r++ ) {
    //   for( int c=0; c<ncols; c++ ) {
    //     int k = nrows*c + r;
    //     Float_t x = xy[0] + r*dxy[0];
    //     Float_t y = xy[1] + r*dxy[1];
    //     SBSShowerBlock* block = 
    //       new SBSShowerBlock(x,y,fPed[k],fGain[k],r,c);
    //     fBlocks[k]=block;
    //     fBlkGrid[r][c]=fBlocks[k];
    //   }

    //    std::cout << " process happened !!!!!!!!!!!!!!!!!!! " << std::endl;

    // Read channel map


  if( !err ) {
    // Clear out the old channel map before reading a new one
    fChanMap.clear();
    if( FillDetMap(detmap, flags, here) <= 0 ) {
      err = kInitError;  // Error already printed by FillDetMap
    } else if( (nelem = fDetMap->GetTotNumChan()) != fNelem ) {
      Error( Here(here), "Number of detector map channels (%d) "
	     "inconsistent with number of blocks (%d)", nelem, fNelem );
      err = kInitError;
    }
  }

  if( !err ) {
    if( !chanmap.empty() ) {
      // If a map is found in the database, ensure it has the correct size
      Int_t cmapsize = chanmap.size();
      cout << "cmapsize = " << cmapsize ;
      if( cmapsize != fNelem ) {
	Error( Here(here), "Channel map size (%d) and number of detector "
	       "channels (%d) must be equal. Fix database.", cmapsize, fNelem );
	err = kInitError;
      }
    }
    if( !err ) {
      // Set up the new channel map
      Int_t nmodules = fDetMap->GetSize();
      assert( nmodules > 0 );
      fChanMap.resize(nmodules);
      for( Int_t i=0, k=0; i < nmodules && !err; i++ ) {
	THaDetMap::Module* module = fDetMap->GetModule(i);
	Int_t nchan = module->hi - module->lo + 1;
	if( nchan > 0 ) {
	  fChanMap.at(i).resize(nchan);
	  for( Int_t j=0; j<nchan; ++j ) {
	    assert( k < fNelem );
	    fChanMap.at(i).at(j) = chanmap.empty() ? k+1 : chanmap[k];
	    ++k;
	  }
	} else {
	  Error( Here(here), "No channels defined for module %d.", i);
	  fChanMap.clear();
	  err = kInitError;
	}
      }
    }
  }
 
     
  fIsInit = true;
}


  return kOK;
}

//_____________________________________________________________________________
Int_t SciFi::DefineVariables( EMode mode )
{
  // Initialize global variables

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  RVarDef vars[] = {
    // { "nthit",  "Number of Left paddles TDC times",  "fNThit" },
    // { "nahit",  "Number of Right paddles TDC times", "fNAhit" },
    // { "t",      "TDC values",                        "fT" },
    // { "t_c",    "Corrected TDC values",              "fT_c" },
    { "A_hitperchannel", "hits per chanel", "fhitsperchannel"},
    { "a",      "ADC values",                        "fA" },
    { "a_AHits",      "Number of hits in an ADC",     "fAHits" },
    { "a_p",    "Ped-subtracted ADC values ",        "fA_p" },
    { "a_c",    "Corrected ADC values",              "fA_c" },
    { "peak",   "FADC ADC peak values",              "fPeak" },
    { "t_fadc", "FADC TDC values",                   "fT_FADC" },
    { "tc_fadc","FADC corrected TDC values",         "fT_FADC_c" },
    { "Asum_p", "Sum of ADC minus pedestal values",  "fASUM_p" },
    { "Asum_c", "Sum of corrected ADC amplitudes",   "fASUM_c" },
    { "Asum",   "Sum of ADC amplitudes",             "fASUM" },
    { "trx",    "x-position of track in det plane",  "fTrackProj.THaTrackProj.fX" },
    { "try",    "y-position of track in det plane",  "fTrackProj.THaTrackProj.fY" },
    { "trpath", "TRCS pathlen of track to det plane","fTrackProj.THaTrackProj.fPathl" },
    { "noverflow",  "overflow bit of FADC pulse",    "foverflow" },
    { "nunderflow",  "underflow bit of FADC pulse",  "funderflow" },
    { "nbadped",  "pedestal quality bit of FADC pulse","fpedq" },
    { "nhits",  "Number of hits for each PMT",       "fNhits_arr" },
    { "hit_X_Y",  "Bool showing if there has been any hit",       "fhit_X_Y" },
    { "X_hits",  "X-position of hits",               "fX_hits"},
    { "Y_hits",  "Y-position of hits",               "fY_hits"},
    { "nX_hits", "Number of X-hits per event",       "fno_x_hits"},
    { "nY_hits", "Number of Y-hits per event",       "fno_y_hits"},
    
    
    
//    { "nhits",  "Number of hits for each PMT",       "fNhits" },
    // raw mode (10) variables

    { "a_raw",  "Raw mode ADC values", "fA_raw"},
    // { "a_raw_p", "Raw mode Ped-subtracted ADC values", "fA_raw_p"},
    // { "a_raw_c",    "Raw mode Corrected ADC values",  "fA_raw_c" }, 
    { "a_raw_sum", "Raw mode sum of pulse", "fA_raw_sum"},
    { "Noise", "Noise present? 0 if not, 1 if yes", "fNoise"},    
    { "hit_X_Y",  "Bool showing if there has been any hit",       "fhit_X_Y" },
    { "hit_fibre", "Displays channel hit(1) or no hit(0)",        "fhit_fibre"},
    { "a_hits_raw", "Displays number of 'hits' perS fibre per event", "fAHits_raw"},
    { "ptime_raw", "Time of first pulse in event (if applicable)", "fPtime_raw"},
    { "time_0", "Time of the zeroth pulse in event (if applicable)", "fTime_0"},
    { "time_1", "Time of the first pulse in event (if applicable)", "fTime_1"},
    { "time_2", "Time of the second pulse in event (if applicable)", "fTime_2"},
    { "time_3", "Time of the third pulse in event (if applicable)", "fTime_3"},
    { "time_4", "Time of the fourth pulse in event (if applicable)", "fTime_4"},
    { "fine_time", "Time of the zeroth pulse in event (if applicable), uses refined timing method", "fTime_fine_0"},
    { 0 }
  };

  //  gHaVars->Define( "a_raw_c",    "Raw mode Corrected ADC values", fA_raw_c);



  // return DefineVarsFromList( vars, mode );

  Int_t err = DefineVarsFromList(vars, mode);
  if( err != kOK ){
    return err;
  }

  //  raw mode pulse data variables

  std::vector<VarDef> vars2;

// // Needs to be fixed before adding to the main repo

  for(Int_t m = 0; m < fNelem; m++) {
    VarDef v;
    char *name   =  new char[128];
    // char *name_p = new char[128];
    // char *name_c = new char[128];
    sprintf(name,"a_raw%.2d",m);
    // sprintf(name_p,"a_p_%.2d",m);
    // sprintf(name_c,"a_c_%.2d",m);
    char *desc = new char[256];
    sprintf(desc,"Raw ADC samples for Module %d",m);
    v.name = name;
    v.desc = "Raw ADC samples";
    v.type = kDouble;
    v.size = 40;
    v.loc  = &(fA_raw[m].data()[0]); //JW: location of data
    v.count = &fNumSamples[m];
    vars2.push_back(v);
    // v.name = name_p;
    // v.desc = "Pedestal subtracted ADC samples";
    // v.loc = &(fA_raw_p[m].data()[0]);
    // vars2.push_back(v);
    // v.name = name_c;
    // v.desc = "Pedestal subtracted calibrated ADC samples";
    // v.loc = &(fA_raw_cl[m].data()[0]);
    // vars2.push_back(v);
  }


  vars2.push_back(VarDef());
  return DefineVarsFromList( vars2.data(), mode );




}

//_____________________________________________________________________________
SciFi::~SciFi()
{
  // Destructor. Remove variables from global list.

  if( fIsSetup )
    RemoveVariables();
  if( fIsInit )
    DeleteArrays();
  //  fFADC=NULL;
}

//_____________________________________________________________________________
void SciFi::DeleteArrays()
{
  // Delete member arrays. Internal function used by destructor.

  fChanMap.clear();
  delete [] fhitsperchannel; fhitsperchannel = NULL;
  delete [] fA_c;    fA_c    = NULL;
  delete [] fAHits;    fAHits    = NULL;
  delete [] fA_p;    fA_p    = NULL;
  delete [] fA;      fA      = NULL;
  delete [] fAHits_raw;  fAHits_raw = NULL;
  delete [] fPtime_raw;  fPtime_raw = NULL;
  delete [] fTime_0;  fTime_0 = NULL;
  delete [] fTime_1;  fTime_1 = NULL;
  delete [] fTime_2;  fTime_2 = NULL;
  delete [] fTime_3;  fTime_3 = NULL;
  delete [] fTime_4;  fTime_4 = NULL;
  delete [] fTime_fine_0;  fTime_fine_0 = NULL;

  // delete [] fA_raw_c;    fA_raw_c    = NULL;
  // delete [] fA_raw_p;    fA_raw_p    = NULL;
  // delete [] fA_raw;      fA_raw      = NULL;
  
  
  
  // delete [] fT_c;    fT_c    = NULL;
  // delete [] fT;      fT      = NULL;

  delete [] fGain;   fGain   = NULL;
  delete [] fPed;    fPed    = NULL;
  // delete [] fOff;    fOff    = NULL;

  delete [] fPeak;      fPeak      = NULL;
  delete [] fT_FADC;    fT_FADC    = NULL;
  delete [] fT_FADC_c;  fT_FADC_c  = NULL;
  delete [] foverflow;  foverflow  = NULL;
  delete [] funderflow; funderflow = NULL;
  delete [] fpedq;      fpedq      = NULL;
  delete [] fNhits_arr;     fNhits_arr     = NULL;


  delete [] fhit_fibre; fhit_fibre = NULL;



  //  delete []
}

//_____________________________________________________________________________
void SciFi::Clear( Option_t* opt )
{

  // clear raw mode fadc data
  

  // Clear event data
  THaNonTrackingDetector::Clear(opt);
  // fNThit = fNAhit = 0;
  assert(fIsInit);
  for( Int_t i=0; i<fNelem; ++i ) {
    // fT[i] = fT_c[i] = 0.0;
    fA[i] = fA_p[i] = fA_c[i] = 0.0;

    fhit_fibre[i] = 0.0;


    fhitsperchannel[i] = 0;
    fAHits_raw[i] = 0;
    fPtime_raw[i] = 0;

    fTime_0[i] = 0;
    fTime_1[i] = 0;
    fTime_2[i] = 0;
    fTime_3[i] = 0;
    fTime_4[i] = 0;

    fTime_fine_0[i] = 0;

    fPeak[i]=0.0;
    fT_FADC[i]=0.0;
    fT_FADC_c[i]=0.0;
    fAHits[i] = 10;
  }
  fASUM_p = fASUM_c = fASUM = 0.0;

  fNoise = 0;
  fno_x_hits = fno_y_hits = 0.0;



  if( !strchr(opt,'I') ) {
    memset( foverflow, 0, fNelem*sizeof(foverflow[0]) );
    memset( funderflow, 0, fNelem*sizeof(funderflow[0]) );
    memset( fpedq, 0, fNelem*sizeof(fpedq[0]) );
    memset( fNhits_arr, 0, fNelem*sizeof(fNhits_arr[0]) );
    memset( fAHits, 0, fNelem*sizeof(fAHits[0]) );
    memset( fAHits_raw, 0, fNelem*sizeof(fAHits_raw[0]) );
    memset( fPtime_raw, 0, fNelem*sizeof(fPtime_raw[0]) );
    memset( fTime_0, 0, fNelem*sizeof(fTime_0[0]) );
    memset( fTime_1, 0, fNelem*sizeof(fTime_1[0]) );
    memset( fTime_2, 0, fNelem*sizeof(fTime_2[0]) );
    memset( fTime_3, 0, fNelem*sizeof(fTime_3[0]) );
    memset( fTime_4, 0, fNelem*sizeof(fTime_4[0]) );
    memset( fTime_fine_0, 0, fNelem*sizeof(fTime_fine_0[0]) );
  }
}

//_____________________________________________________________________________
inline
void SciFi::ClearEvent()
{
  // Reset all local data to prepare for next event.
  ResetVector(fNumSamples,0);

  ResetVector(fX_hits,0);
  ResetVector(fY_hits,0);

  ResetVector(fA_raw, 0.0);
  ResetVector(fA_raw_p,0.0);
  ResetVector(fA_raw_c,0.0);

  fX_hits.clear();
  fY_hits.clear();
  




    // fCoarseProcessed = 0;
    // fFineProcessed = 0;

  //    const int lsh = fNelem*sizeof(Float_t);
    // const int lshh = fMaxNClust*sizeof(Float_t);
    // const int lsc = fNclublk*sizeof(Float_t);
    // const int lsi = fNclublk*sizeof(Int_t);
    // const int lsj = fMaxNClust*sizeof(Int_t);

    fNhits = 0;
    // memset( fE, 0, lshh );
    // memset( fE_c, 0, lshh );
    // memset( fX, 0, lshh );
    // memset( fY, 0, lshh );
    //memset( fXtarg, 0, lshh );
    //memset( fYtarg, 0, lshh );
    //memset( fZtarg, 0, lshh );
    //    memset( fMult, 0, lsj );
    fASUM_p = 0.0;
    fASUM_c = 0.0;
    fASUM = 0.0;
    //    fNclust = 0;
    // memset( fNblk, 0, lsi );
    // memset( fEblk, 0, lsc );
    // fTRX = 0.0;
    // fTRY = 0.0;
  
    //    fAHits_raw = 0.0;


    fNoise = 0;

    for(size_t i=0; i<fA_raw_sum.size(); i++) fA_raw_sum[i] = 0.0; // MAPC


    //2//for (int i=0;i<fNelem;i++) 
        //2//fBlocks[i]->ClearEvent();

}



//_____________________________________________________________________________
Int_t SciFi::Decode( const THaEvData& evdata )
{
  // Decode Cherenkov data, correct TDC times and ADC amplitudes, and copy
  // the data into the local data members.
  // This implementation assumes that the first half of the detector map
  // entries corresponds to ADCs, and the second half, to TDCs.

  // this next loop should go through 4 entries (4 fadc modules with 16 channels apeice)
  Int_t noevents = 1000;

  
  Int_t mode, num_events, num_samples;
  Bool_t raw_mode = kFALSE;
  
  ClearEvent();
  Clear();

 
  for( Int_t i = 0; i < fDetMap->GetSize(); i++ ) {

    THaDetMap::Module* d = fDetMap->GetModule( i );
    
    bool adc = fDetMap->IsADC(d);


    const char* const here = "Decode";
    
    // if (!adc){
    //  Error( Here(here), "Module %d in database is not of type fadc250 "
    // 	   "Fix database", i);
    //  Int_t err = kDecodeErr; // defined somwhere in ThaAnalyzer
    // }

    
    // change: removed if condition for fadc (should be only thing present)
    
    Decoder::Module *m = evdata.GetModule(d->crate,d->slot);
    //Make sure the Module is found!!  BANE!!
    if (m ==nullptr)
    {
	mod_Err_cnt++;
	if(mod_Err_cnt<5)
	{
		Error( Here(here), "Can not Get Module crate %d, slot(%d)!!!!",d->crate,d->slot );
	}
	if(mod_Err_cnt==5)
	{
		Error( Here(here), "Can not Get Module crate %d, slot(%d)!!!!",d->crate,d->slot );
		cout << "Last time this error will display!!!!!!"<<endl;
	}
	continue;
    }
   //    Decoder::Fadc250Module *fFADC = dynamic_cast <Fadc250Module*> (evdata.GetModule(d->crate, d->slot));
    Decoder::Fadc250Module *fFADC = dynamic_cast<Decoder::Fadc250Module*>(m);

    mode = fFADC->GetFadcMode();

    raw_mode = (mode == 1) || (mode == 8) || (mode == 10);

    //    std::cout << "Fadc mode is " << mode << std::endl;

    // Loop over all channels that have a hit.
    for( Int_t j = 0; j < evdata.GetNumChan( d->crate, d->slot ); j++) {

      fNhits++;


      Int_t chan = evdata.GetNextChan( d->crate, d->slot, j );
      if( chan < d->lo || chan > d->hi ) continue;     // Not one of my channels

      //      cout << " check 2 " << endl;

      // Get the detector channel number, starting at 0
      // removed -1 at end, not sure of purpose
      //      Int_t k = d->first + ((d->reverse) ? d->hi - chan : chan - d->lo);
      Int_t k = ((d->reverse) ? d->hi - chan : chan - d->lo);
      //      fAHits[k] = fFADC->GetNumFadcEvents(chan);

      //      std::cout << "Channel number : " << k << "\n" << "Loop number : " << j << std::endl;

      // section for processing non-raw mode Fadc output
#ifdef WITH_DEBUG
	if( k<0 || k>= fNelem ) {
	  Warning( Here("Decode()"), "Illegal detector channel: %d, chan = %d, hit no = %d,  i = %d", k, chan, j, i );
	  continue;
	}
#endif	
	
	// cout << " [i][k] = [" << i << "][" << k << "] = " <<  fChanMap[i][k] -1  <<  endl;

	// #ifdef NDEBUG
	Int_t fibre = fChanMap[i][k] - 1;

       
	// #else
	// 	Int_t fibre = fChanMap.at(i).at(k) - 1;
	// #endif
	
	
	if( fibre<0 || fibre>=fNelem ) {
	  Error( Here(here), "Bad array index: %d. Your channel map is "
		 "invalid. Data skipped.", fibre );
	  continue;
	}
	
	
	
	if(!raw_mode){
	  
//		cout << "fAHits[k] [" << k << "] = fFADC->GetNumFadcEvents(chan) =  (" << chan << ") " << fFADC->GetNumFadcEvents(chan) << endl;

#ifdef WITH_DEBUG
	if( k<0 || k>= fNelem ) {
	  Warning( Here("Decode()"), "Illegal detector channel: %d, chan = %d, hit no = %d,  i = %d", k, chan, j, i );
	  continue;
	}
#endif
	
#ifdef NDEBUG
	//	Int_t fibre= fChanMap[i][k] - 1;
	//	Int_t fibre = k;
#else
	//	Int_t fibre = fChanMap.at(i).at(k) - 1;
	//	Int_t fibre = k;
#endif
	
	if( fibre<0 || fibre>=fNelem ) {
	  Error( Here(here), "Bad array index: %d. Your channel map is "
		 "invalid. Data skipped.", fibre );
	  continue;
	}
	
	// Get the data. fADCs are assumed to have only single hit (hit=0)
	Int_t data;
	//	Int_t ftime=0;
	Int_t fpeak=0;
	Float_t tempPed = fPed[fibre];             // Dont overwrite DB pedestal value!!! -- REM -- 2018-08-21
	Float_t gain = fGain[fibre];

	// if(adc){
	
	
	
	data = evdata.GetData(kPulseIntegral,d->crate,d->slot,chan,0);
	//	ftime = evdata.GetData(kPulseTime,d->crate,d->slot,chan,0);
	fpeak = evdata.GetData(kPulsePeak,d->crate,d->slot,chan,0);

	// }
	// else{ 
	//	fNhits_arr[k]=evdata.GetNumHits(d->crate, d->slot, chan);     
	// 	data = evdata.GetData( d->crate, d->slot, chan, fNhits_arr[k]-1 );
	// }
	
	// if(adec){
	
	//noevents = 1000;
	noevents = fFADC->GetNumFadcEvents(chan);
	
	if(fFADC!=NULL){
	  foverflow[fibre] = fFADC->GetOverflowBit(chan,0);
 	  funderflow[fibre] = fFADC->GetUnderflowBit(chan,0);
	  fpedq[fibre] = fFADC->GetPedestalQuality(chan,0);
	  
	  noevents = fFADC->GetNumFadcEvents(chan);
	  //	fAHits[k] = fFADC->GetNumFadcEvents(chan);
	  //       if(foverflow[k]+funderflow[k]+fpedq[k] != 0) printf("Bad Quality: (over, under, ped)= (%i,%i,%i)\n",foverflow[k],funderflow[k],fpedq[k]);
	}
	
	
	//	std::cout << " tempped/ fpedq[k] = " << fPed[k] << std::endl;
	if(fpedq[fibre]==0) 
	  {
	    //	    std::cout << " passed 100000 condition " << std::endl;
	    if(fTFlag == 1)
	      {
		tempPed=(fNSA+fNSB)*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;
	      }
	    else
	      {
		tempPed=fWin*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;
	      }
	  }
	
	if(fpedq[fibre]!=0)
	  {
	    printf("\nWARNING: BAD ADC PEDESTAL\n");
	    // if fadc gives bad pedestal then use 
	    // give different pedestal based on raw or production mode data (different window sizes)

	    if(fTFlag == 1)
	      {
		tempPed = (fNSA+fNSB)*tempPed;
	      }
	    else
	      {
		tempPed = (fWin)*tempPed;
	      }


	  }
	
	// }
	
	// Copy the data to the local variables.
	// if ( adc ) {
	fA[fibre]   = data;
	//      fAHits[k] = noevents; 
	fPeak[fibre] = static_cast<Float_t>(fpeak);
	//	fT_FADC[fibre]=static_cast<Float_t>(ftime);
	fT_FADC_c[fibre]=fT_FADC[fibre]*0.0625;
	fA_p[fibre] = data - Int_t(tempPed);
	fhitsperchannel[fibre] = 10;
	fA_c[fibre] = fA_p[fibre]*gain;
	// only add channels with signals to the sums
	fASUM += fA[fibre];
	if( fA_p[fibre] > 0.0 )
	  fASUM_p += fA_p[fibre];
	if( fA_c[fibre] > 0.0 )
	  fASUM_c += fA_c[fibre];


	// Decide whether there is a hit or not here 


       
	if(fA_c[fibre] > 300){    
	 //       cout << " hit if condition started! " << endl;
         fhit_X_Y = 1;
         fhit_fibre[fibre] = 1;
	 
         // cout << " hit if condition, before x and y loops " << endl;
         // cout << " fhit_X_Y = "  << fhit_X_Y << ", fibre = " << fibre <<  endl;
	 
	 

         if(fibre >= 0 && fibre <=31){
           fX_hits.push_back(fibre);
           //      fX_hits[fno_x_hits] = fibre;            
           fno_x_hits++;
	 }
         
         if(fibre >= 32 && fibre <=63){
           fY_hits.push_back(fibre);
           //      fY_hits[fno_y_hits] = fibre;
           fno_y_hits++;
	 }
         
	 //       cout << " hit if condition worked! " << endl;
       

       if (fA_c[fibre] < 100){
	 //       cout << "hit condition missed :(" << endl;
       }
       }	
	



	//      fNAhit++;
	// } else {
	// 	// fT[k]   = data;
	// 	// fT_c[k] = data - fOff[k];
	// 	fNThit++;
	// }


	}
    
    
      if ( fDebug > 3 ) {
	printf("\nSciFi %s:\n",GetPrefix());
	int ncol=2;
	for (int i=0; i<ncol; i++) {
	  printf("  ADC  ADC_p  ");
	}
	printf("\n");
	
	for (int i=0; i<(fNelem+ncol-1)/ncol; i++ ) {
	  for (int c=0; c<ncol; c++) {
	    int ind = c*fNelem/ncol+i;
	    if (ind < fNelem) {
	      printf("  %3d  %5.0f  %5.0f  ",ind+1,fA[ind],fA_p[ind]);
	    } else {
	    //	  printf("\n");
	      break;
	    }
	  }
	  printf("\n");
	}
      }
      

      // section for processing raw mode
      
      if (raw_mode){
	
	
	//	Int_t p = fDetMap[i][k] - 1;
	//  Int_t k = d->first + ((d->reverse) ? d->hi - chan : chan - d->lo);	
	
	//	std::cout << "Entered raw mode" << std::endl;
	
	
	num_events = fFADC->GetNumFadcEvents(chan);
	
	//	std::cout << "First Check " << std::endl;
	
	num_samples = fFADC->GetNumFadcSamples(chan,0);

	//	std::cout << "2nd Check: num_samples =  " << std::endl;
	
	if(num_samples > MAX_FADC_SAMPLES || num_samples < 0) {
	  Error( Here(here),
		 "Too manu samples in fFADC: %d out of %d MAX",num_samples,
		 MAX_FADC_SAMPLES);
	} else {
	  
	  ///	  std::cout << "Bonus Check " << std::endl;
	  //	  Double_t John =  fNumSamples[1];
	  
	  //	  std::cout << "3rd Check " << std::endl;
	  fNumSamples[fibre] = num_samples;
	  //	  std::cout << "4th Check " << std::endl;
	  std::vector<UInt_t> samples = fFADC->GetPulseSamplesVector(chan);
	  //	  std::cout << "5th Check " << std::endl;

	  first_hit = true;

	  Int_t max_sample = 0; // value of max sample in pulse

	  Int_t max_value = 0; // sample with max value
	  
	  // variables for timing peak of first pulse
	  Int_t s0 = 0; // sample before halfmax
	  Int_t s1 = 0; // sample after halfmax

	  Int_t a0 = 0; // value of sample at s0
	  Int_t a1 = 0; // value of sample at s1
	  
	  Double_t halfmax = 0;
		  
	  Double_t tfine = 0;
	  
	  Double_t final_t = 0;

	  Double_t newped = 0.0;
     


	  for(Int_t s = 0; s < num_samples; s++) {

	    fA_raw[fibre][s] = samples[s];

	    max_sample = 0;
	    max_value = 0;
	    s0 = 0;
	    s1 = 0;
	    halfmax = 0;
	    tfine = 0;
	    final_t = 0;
	    newped = 0.;

	    // raw-mode analysis

	    // if(s == 0 && fA_raw[fibre][s] > (fPed[fibre] + Threshold) ){

	    //   // condition for first sample (can't check if previous sample also had high value)
	    //   fAHits_raw[fibre]++;
	    //   fPtime_raw[fibre] = (s+1)*4.;
	    //   //	      cout << "First condition true!" << endl;

	    //   first_hit = false;
	      
	    //   fTime_0[fibre] =  (s+1)*4.;
	      

	    // }
	    
	    if (s > 4 && fA_raw[fibre][s] > (fPed[fibre] + Threshold) && fA_raw[fibre][s-1] <  (fPed[fibre] + Threshold) ){
	      

	      
	      // Loop over length of pulse to find maximum sample
	      // only fo this for the first event

	      if(fAHits_raw[fibre] == 0){	       
		

		// calculate a pedestal from the first 4 bins 

		for (Int_t sam3 = 0; sam3 <4; sam3++){

		  newped = newped + fA_raw[fibre][sam3];
		  

		}
		//   cout << "newped sum " << newped << endl;
		newped = newped / 4.;

		for (Int_t sam = 0; sam < min(5,(num_samples -s)) ; sam++){
		  
		  if (fA_raw[fibre][sam + s] > max_value){
		    
		    max_sample = sam + s;
		    max_value = fA_raw[fibre][sam + s];
		    
		  }		  

		}
		

		// find sample before halfmax
		halfmax = (max_value - newped) / 2.0;

		for (Int_t sam1 = 0;  sam1 < min(5,(num_samples -s)) ; sam1++){

		  if((fA_raw[fibre][sam1+s-1]-newped) <= halfmax && (fA_raw[fibre][s + sam1]-newped) > halfmax){
		    s0 =  sam1+s-1;
		    break;
		  }
		}

		// find sample after halfmax


		for (Int_t sam2 = 0;  sam2 < min(5,(num_samples -s)) ; sam2++){

		  if((fA_raw[fibre][s + sam2]-newped) > halfmax && (fA_raw[fibre][s + sam2 + 1]-newped) <= halfmax){
		    s1 =  s + sam2;
		    break;
		  }
		}

		
		//	halfmax = (fA_raw[fibre][s1] + fA_raw[fibre][s0]) /2;

		// interpolate time between s0 and s2

		a0 = fA_raw[fibre][s0]-newped;
		a1 = fA_raw[fibre][s0 +1 ]-newped;
		//dirty if condition
		if (a1<a0 || a1==a0) continue;
		//   cout << "s0 " << s0 << " , a0 " << a0 << " , a1 " << a1 << " halfmax " << halfmax << endl;
		tfine =  (( ( halfmax - a0)/(a1-a0) * 64.0));
                //final_t = (s0 << 6) + tfine;
	       	final_t = 4.0*s0  + tfine*0.0625;

		//	cout << "tfine is " << tfine  << " , and final time is : " << final_t << endl;


		fTime_fine_0[fibre] = 	final_t;


		// need to convert s0 sample to compatible time


		fTime_0[fibre] =  (s+1)*4.;
	      }



	      if(fAHits_raw[fibre] == 1){	       
		fTime_1[fibre] =  (s+1)*4.;
	      }
	      if(fAHits_raw[fibre] == 2){	       
		fTime_2[fibre] =  (s+1)*4.;
	      }
	      if(fAHits_raw[fibre] == 3){	       
		fTime_3[fibre] =  (s+1)*4.;
	      }
	      if(fAHits_raw[fibre] == 4){	       
		fTime_4[fibre] =  (s+1)*4.;
	      }


	      fAHits_raw[fibre]++;
	      fPtime_raw[fibre] = (s+1) * 4.;
	      first_hit = false;
	      


	    }


	    if( fAHits_raw[fibre] != 0){
	      //	      cout << "fAHits_raw[" << fibre << "] = " << fAHits_raw[fibre] << endl; 
	    }


	    if( fibre == 15){
	      if(fA_raw[fibre][s] > 800 || fA_raw[fibre][s] < 160){    //  for the left
		fNoise = 1;
	    
	      }

	    }
	    // fA_raw_p[k][s] = fA_raw[k][s]-fPed[k];
	    // fA_raw_c[k][s] = fA_raw_p[k][s]*fGain[k];
	    fA_raw_sum[fibre] += samples[s];

	    
	  }
	  //	  std::cout << "6th Check " << std::endl;
	}

  
 


  // Compute x and y co-ordinates of hits if there are any
 

	Int_t data;
	//	Int_t ftime=0;
	Int_t fpeak=0;
	Float_t tempPed = fPed[fibre];
	Float_t gain = fGain[fibre];

	
	foverflow[fibre] = fFADC->GetOverflowBit(chan,0);
	funderflow[fibre] = fFADC->GetUnderflowBit(chan,0);
	fpedq[fibre] = fFADC->GetPedestalQuality(chan,0);
	  
	noevents = fFADC->GetNumFadcEvents(chan);
	
	if(fpedq[fibre]==0) 
	  {
	    //	    std::cout << " passed 100000 condition " << std::endl;
	    if(fTFlag == 1)
	      {
		tempPed=(fNSA+fNSB)*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;
	      }
	    else
	      {
		tempPed=fWin*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;
	      }
	  }
	
	if(fpedq[fibre]!=0)
	  {
	    printf("\nWARNING: BAD ADC PEDESTAL\n");
	    // if fadc gives bad pedestal then use 
	    // give different pedestal based on raw or production mode data (different window sizes)

	    if(fTFlag == 1)
	      {
		tempPed = (fNSA+fNSB)*tempPed;
	      }
	    else
	      {
		tempPed = (fWin)*tempPed;
	      }


	  }
	
	

	data = evdata.GetData(kPulseIntegral,d->crate,d->slot,chan,0);
	//	ftime = evdata.GetData(kPulseTime,d->crate,d->slot,chan,0);
	fpeak = evdata.GetData(kPulsePeak,d->crate,d->slot,chan,0);

	fA[fibre]   = data;
	//      fAHits[k] = noevents; 
	fPeak[fibre] = static_cast<Float_t>(fpeak);
	//	fT_FADC[fibre]=static_cast<Float_t>(ftime);
	fT_FADC_c[fibre]=fT_FADC[fibre]*0.0625;
	fA_p[fibre] = data - tempPed;
	fhitsperchannel[fibre] = 10;
	fA_c[fibre] = fA_p[fibre]*gain;
	// only add channels with signals to the sums

	fASUM += fA[fibre];
	if( fA_p[fibre] > 0.0 )
	  fASUM_p += fA_p[fibre];
	if( fA_c[fibre] > 0.0 )
	  fASUM_c += fA_c[fibre];



       if(fA_c[fibre] > 450){    
	 //       cout << " hit if condition started! " << endl;
         fhit_X_Y = 1;
         fhit_fibre[fibre] = 1;
	 
         // cout << " hit if condition, before x and y loops " << endl;
         // cout << " fhit_X_Y = "  << fhit_X_Y << ", fibre = " << fibre <<  endl;
	 
	 

         if(fibre >= 0 && fibre <=31){
           fX_hits.push_back(fibre);
           //      fX_hits[fno_x_hits] = fibre;            
           fno_x_hits++;
	 }
         
         if(fibre >= 32 && fibre <=63){
           fY_hits.push_back(fibre);
           //      fY_hits[fno_y_hits] = fibre;
           fno_y_hits++;
	 }
         
	 //       cout << " hit if condition worked! " << endl;
       

	 if (fA_c[fibre] < 100){
	 //       cout << "hit condition missed :(" << endl;
	 }
       }	
       



      }
      
    }


    // Finally process x and y by taking the values of the gain adjusted, pedestal subtracted output
    // then add timing


    
  }


  
    if(!fhit_X_Y){
    // no hits in any fibre
    }
    else{
      
    //    cout << " sorting reached " << endl;
      
      sort(fX_hits.begin(),fX_hits.end());
      sort(fY_hits.begin(),fY_hits.end());
      
      
      //+    cout << " sorting passed " << endl;  
      
      
      for( Int_t i =0; i<fno_x_hits; i++){
	// do x checks 
	
      }
      
      
      for( Int_t i =0; i<fno_y_hits; i++){
	
	// do y-checks
      }
    }
    
    // std::cout << "End of decoding " << noevents <<  std::endl;
    return noevents;

  // return fNThit;
  //  return fNAhit++;
}

//_____________________________________________________________________________
Int_t SciFi::CoarseProcess( TClonesArray& tracks )
{
  // Reconstruct coordinates of where a particle track crosses
  // the Cherenkov plane, and copy the point into the fTrackProj array.

  CalcTrackProj( tracks );

  return 0;
}

//_____________________________________________________________________________
Int_t SciFi::FineProcess( TClonesArray& tracks )
{
  // Fine Cherenkov processing

  // Redo the track matching since tracks might have been thrown out
  // during the FineTracking stage.
  fTrackProj->Clear();
  CalcTrackProj( tracks );

  return 0;
}

//_____________________________________________________________________________
ClassImp(SciFi)
