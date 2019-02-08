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
  foverflow(0), funderflow(0),fpedq(0), fNhits(0), fNhits_arr(0), fNoise(0)
{
  // Constructor
//  fFADC=NULL;
}

//_____________________________________________________________________________
SciFi::SciFi()
: THaNonTrackingDetector(), fPed(0), fGain(0), fA(0), fAHits(0),
  fA_p(0), fA_c(0),fPeak(0),foverflow(0), funderflow(0),fpedq(0),fNhits(0), fNhits_arr(0), fNoise(0)
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
    for(Int_t i = 0; i < fNelem; i++) {
      // We'll resize the vector now to make sure the data are contained
      // in a contigous part of memory (needed by THaOutput when writing
      // to the ROOT file)
      fA_raw[i].resize(MAX_FADC_SAMPLES);
      fA_raw_p[i].resize(MAX_FADC_SAMPLES);
      fA_raw_c[i].resize(MAX_FADC_SAMPLES);
      //      std::cout << "check for resizing" << std::endl;

    }
    fA_raw_sum.resize(fNelem); // MAPC
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
    { "nbadped",  "pedestal quality bit of FADC pulse",   "fpedq" },
    { "nhits",  "Number of hits for each PMT",       "fNhits_arr" },
//    { "nhits",  "Number of hits for each PMT",       "fNhits" },
    // raw mode (10) variables
    { "a_raw",  "Raw mode ADC values", "fA_raw"},
    // { "a_raw_p", "Raw mode Ped-subtracted ADC values", "fA_raw_p"},
    // { "a_raw_c",    "Raw mode Corrected ADC values",  "fA_raw_c" }, 
    { "a_raw_sum", "Raw mode sum of pulse", "fA_raw_sum"},
    { "Noise", "Noise present? 0 if not, 1 if yes", "fNoise"},    
    { 0 }
  };

  //  gHaVars->Define( "a_raw_c",    "Raw mode Corrected ADC values", fA_raw_c);



  //return DefineVarsFromList( vars, mode );
  Int_t err = DefineVarsFromList(vars, mode);
  if( err != kOK ){
    return err;
  }

  //  raw mode pulse data variables

  std::vector<VarDef> vars2;

// Needs to be fixed before adding to the main repo

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

    fhitsperchannel[i] = 0;
    fPeak[i]=0.0;
    fT_FADC[i]=0.0;
    fT_FADC_c[i]=0.0;
    fAHits[i] = 10;
  }
  fASUM_p = fASUM_c = fASUM = 0.0;

  fNoise = 0;

  if( !strchr(opt,'I') ) {
    memset( foverflow, 0, fNelem*sizeof(foverflow[0]) );
    memset( funderflow, 0, fNelem*sizeof(funderflow[0]) );
    memset( fpedq, 0, fNelem*sizeof(fpedq[0]) );
    memset( fNhits_arr, 0, fNelem*sizeof(fNhits_arr[0]) );
    memset( fAHits, 0, fNelem*sizeof(fAHits[0]) );
  }
}

//_____________________________________________________________________________
inline
void SciFi::ClearEvent()
{
  // Reset all local data to prepare for next event.
  ResetVector(fNumSamples,0);
  ResetVector(fA_raw, 0.0);
  ResetVector(fA_raw_p,0.0);
  ResetVector(fA_raw_c,0.0);

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
	Int_t ftime=0;
	Int_t fpeak=0;
	Float_t tempPed = fPed[fibre];             // Dont overwrite DB pedestal value!!! -- REM -- 2018-08-21
	// if(adc){
	
	
	
	data = evdata.GetData(kPulseIntegral,d->crate,d->slot,chan,0);
	ftime = evdata.GetData(kPulseTime,d->crate,d->slot,chan,0);
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
	fT_FADC[fibre]=static_cast<Float_t>(ftime);
	fT_FADC_c[fibre]=fT_FADC[fibre]*0.0625;
	fA_p[fibre] = data - Int_t(tempPed);
	fhitsperchannel[fibre] = 10;
	fA_c[fibre] = noevents;
	// only add channels with signals to the sums
	fASUM += fA[fibre];
	if( fA_p[fibre] > 0.0 )
	  fASUM_p += fA_p[fibre];
	if( fA_c[fibre] > 0.0 )
	  fASUM_c += fA_c[fibre];
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
	  for(Int_t s = 0; s < num_samples; s++) {

	    fA_raw[fibre][s] = samples[s];

	    if(fA_raw[15][s] > 800 || fA_raw[15][s] < 160){    //  for the left
	      fNoise = 1;
	    
	    }
	    // fA_raw_p[k][s] = fA_raw[k][s]-fPed[k];
	    // fA_raw_c[k][s] = fA_raw_p[k][s]*fGain[k];
	    fA_raw_sum[fibre] += samples[s];

	    
	  }
	  //	  std::cout << "6th Check " << std::endl;
	}

	////////////////////////////////////////////////////////////
	////// Also collect raw-mode data in 'normal' format ///////
	/////            ie fA,fA_p,fA_c etc                 ///////           
        ////////////////////////////////////////////////////////////

	Int_t data;
	Int_t ftime=0;
	Int_t fpeak=0;
	Float_t tempPed = fPed[fibre];

	
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
	ftime = evdata.GetData(kPulseTime,d->crate,d->slot,chan,0);
	fpeak = evdata.GetData(kPulsePeak,d->crate,d->slot,chan,0);

	fA[fibre]   = data;
	//      fAHits[k] = noevents; 
	fPeak[fibre] = static_cast<Float_t>(fpeak);
	fT_FADC[fibre]=static_cast<Float_t>(ftime);
	fT_FADC_c[fibre]=fT_FADC[fibre]*0.0625;
	fA_p[fibre] = data - tempPed;
	fhitsperchannel[fibre] = 10;
	fA_c[fibre] = noevents;
	// only add channels with signals to the sums

	fASUM += fA[fibre];
	if( fA_p[fibre] > 0.0 )
	  fASUM_p += fA_p[fibre];
	if( fA_c[fibre] > 0.0 )
	  fASUM_c += fA_c[fibre];


      }
      
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
