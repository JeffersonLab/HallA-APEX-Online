///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaFppPlane                                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Here:                                                                     //
//        Units of measurements:                                             //
//        For cluster position (center) and size  -  wires;                  //
//        For X, Y, and Z coordinates of track    -  meters;                 //
//        For Theta and Phi angles of track       -  in tangents.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaFpp.h"
#include "THaFppPlane.h"
#include "THaFppHit.h"
#include "THaDetMap.h"
#include "THaEvData.h"
#include "THaDBFile.h"
#include "THaDB.h"
#include "TString.h"
#include "TClass.h"
#include "TMath.h"
#include "VarDef.h"
#include "VarType.h"
#include "TDatime.h"
#include "THaApparatus.h"

#include <cstring>
#include <vector>
#include <iostream>

using namespace std;


//_____________________________________________________________________________
THaFppPlane::THaFppPlane( const char* name, const char* description,
						 THaDetectorBase* parent )
  : THaSubDetector(name,description,parent), fFpp(NULL)
{
  // Constructor
  fName = name;

  // Since TCloneArrays can resize, the size here is fairly unimportant
  fHits     = new TClonesArray("THaFppHit", 35 );
  fGHits     = new TClonesArray("THaFppHit", 35 );
  THaDetectorBase *det = GetDetector();
  if( det )
    fFpp = static_cast<THaSubDetector*>(det)->GetDetector();
}
//_____________________________________________________________________________
Int_t THaFppPlane::ReadDatabase( const TDatime& date )
{

// Read this detector's parameters from the database file 'fi'.
	// This function is called by THaDetectorBase::Init() once at the
	// beginning of the analysis.
	// 'date' contains the date/time of the run being analyzed.

	THaDBFile *dbf = new THaDBFile("db_L.fpp.dat","detmap.config");
//	TDatime now; // defaults to today's date and time

	//read detector map for this plane

	TString detstring = "fpp_";
	detstring.Append(fName);
        char* detname;
        detname = new char[ detstring.Length()+1 ];
        strcpy( detname, detstring.Data() );

	//read detector map for this plane
        if(!dbf->GetDetMap(detname,fDetMap,date)){
           cout << "Error Reading 'detmap.config' for " << detname << endl;
           return kInitError;
        }
	//read geometry for this plane

	Double_t geom[17];

	TagDef planegeom[] = {
		{ "geometry", &geom, 0, 17 },
		{0}
	};

	if(!dbf->LoadValues(GetPrefix(),planegeom,date)){
          cout << "Error reading 'db_L.fpp.dat' for " << GetPrefix() << endl; 
          return kInitError;
        }

	fX = geom[0];
	fY = geom[1];
	fZ = geom[2];
	fdX = geom[3];
	fdY = geom[4];
	fdZ = geom[5];
	fZZ = geom[6];
	fNWire = static_cast<Int_t>(geom[7]);
	fWangle = geom[8];
	fWspacing = geom[9];
	fUVXzero = geom[10];
	fXoff = geom[11];
	fYoff = geom[12];
	fPhoff = geom[13];
	fThoff = geom[14];
	fPsoff = geom[15];
	fStrawd = geom[16];

  return kOK;
}

//_____________________________________________________________________________
Int_t THaFppPlane::DefineVariables( EMode mode )
{
  // initialize global variables

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  // Register variables in global list

  RVarDef vars[] = {
    { "nhit",   "Number of hits",                    "GetNHits()" },
    { "wire",   "Raw wire group number",             "fHits.THaFppHit.fWire" },
    { "ltdc",  "Raw leading edge time",              "fHits.THaFppHit.fLtdc" },
    { "ttdc",   "Raw trailing edge time",            "fHits.THaFppHit.fTtdc" },
//    { "opt",   "Should all be nonzero",              "fHits.THaFppHit.fOpt" },
    { "straw",  "Raw straw number of hit",           "fHits.THaFppHit.fStraw" },
    { "width", "Raw pulse width",                    "fHits.THaFppHit.fWidth" },
	{ "drfdst","Uncorrected drift distance of hit",  "fHits.THaFppHit.fDist"},
        { "ngoodhit", "Number of good hits",              "GetNGHits()"          },
	{ "ltdc_c", "Corrected le time",                 "fGHits.THaFppHit.fLtdc"},
	{ "ttdc_c", "Corrected te time",                 "fGHits.THaFppHit.fTtdc"},
	{ "width_c","Corrected pulse width",             "fGHits.THaFppHit.fWidth"},
	{ "drfdst_c","Corrected drift distance",         "fGHits.THaFppHit.fDist"},
	{ "straw_c", "Corrected straw number",           "fGHits.THaFppHit.fStraw"},
	{ "wire_c"   , "Corrected wire group number",      "fGHits.THaFppHit.fWire"},
	{ "resid_c" , "Corrected residual",              "fGHits.THaFppHit.fResidual"},
	{ "efficiency" , "efficiency measure (=1 for good hit)","itrgood"},
	{ "strwgood" , "predicted straw hit for good hit",       "strwgood"},
	{ "strwbad" , "predicted straw hit for no hit",         "strwbad"},
    { 0 }
  };
  return DefineVarsFromList( vars, mode );

}

//_____________________________________________________________________________
THaFppPlane::~THaFppPlane()
{
  // Destructor.

  if( fIsSetup )
    RemoveVariables();
  delete fHits;
  delete fGHits;
 
}
//_____________________________________________________________________________
inline
void THaFppPlane::Clear( Option_t* opt )
{    
  // Clears the contents of the hits 
  fHits->Clear();
  fGHits->Clear();
}

//_____________________________________________________________________________
Int_t THaFppPlane::Decode( const THaEvData& evData)
{    
  // Converts the raw data into hit information
  // Assumes channels & wires  are numbered in order
  // TODO: Make sure the wires are numbered in order, even if the channels
  //       aren't
              
  Clear();  //Clear the last event
  Int_t fpp_first = 1, fpp_cnt = 0;
  Short_t wire;
  Float_t ltdc, ttdc;
  Int_t nextHit = 0, prev_wire=-1,hitcount=0;


  // Loop over all detector modules for this plane
  for (Int_t i = 0; i < fDetMap.GetSize(); i++) {
    THaDetMap::Module * d = fDetMap.GetModule(i);
    
    // Get number of channels with hits
    Int_t nChan = evData.GetNumChan(d->crate, d->slot);
    for (Int_t chNdx = 0; chNdx < nChan; chNdx++) {
      // Use channel index to loop through channels that have hits
    fpp_first = 1;
    fpp_cnt = 0;

      Int_t chan = evData.GetNextChan(d->crate, d->slot, chNdx);
      if (chan < d->lo || chan > d->hi) 
	  continue; //Not part of this detector

      // Get number of hits for this channel and loop through hits
	  Int_t nHits = evData.GetNumHits(d->crate, d->slot, chan);
	  if(nHits > fMaxHit){nHits = fMaxHit;}
         
	  for (Int_t hit = 0; hit < nHits; hit++) {

		  //opt from digitizer (opt = 0 is LE; opt = 1 is TE)
		  Bool_t opt = (evData.GetRawData( d->crate, d->slot, chan, hit ) & 0x10000 ) > 0;
                //  if(fName == "v11"){
                //     cout << d->first+chan-d->lo  << " " << opt << " " << fpp_first <<" "  << fpp_cnt << endl;
                //  }             
		  if (fpp_first == 1){
			  fpp_cnt++;
			  if (!opt) {continue;} //first word must be TE
                          if (fpp_cnt == nHits) {break;} //must be LE and TE pair
			  //implement call to get next opt: opt_next
             	          Bool_t opt_next = (evData.GetRawData( d->crate, d->slot, chan, hit+1) & 0x10000)>0;
                  //        if(fName == "v11"){
                  //           cout << d->first+chan-d->lo  << " " << opt << " " << opt_next << " " << fpp_first <<" "  
                  //                << fpp_cnt << " " << hitcount << endl;
                  //         }             
			  if (opt && !opt_next){
                      //            if(hitcount+1 > fMaxHit){break;}
                                  hitcount++;
				  fpp_first = 2;

				  // Get the wire number, starting at 1
				  wire = d->first + chan - d->lo;
                                  if(wire == prev_wire) {nextHit--;}
                                  if(nextHit+1>fMaxHit) {break;}
				  //Copy data to local variables, trailing edge is the first hit, followed by leading
				  //edge at later time (pulse width = leading edge - trailing edge)
				  ltdc = static_cast<Float_t>(evData.GetData(d->crate,d->slot,chan,hit+1));
				  ttdc = static_cast<Float_t>(evData.GetData(d->crate,d->slot,chan,hit));

                                  new( (*fHits)[nextHit++] )  THaFppHit( wire, ltdc, ttdc);
                                  prev_wire = wire;
                                  
			  }
			  else {continue;}
		  }
		  else{ 
			  fpp_first = 1;
			  fpp_cnt++;
		  }
	  } // End hit loop
    } // End channel index loop
  } // End slot loop

/*  nextHit=0;
  for(Int_t i=0; i < hitcount; i++){
     if(i==0){prev_wire = wire[i];}
     if(i +1 == hitcount) {
       if(wire[i] != prev_wire){
         for(Int_t j=1; j <= 0; j--){
            if(i - j < 0) {break;}
            new( (*fHits)[nextHit++] )  THaFppHit( wire[i-j], ltdc[i-j], ttdc[i-j]);            
         }
       }
       else {
            new( (*fHits)[nextHit++] )  THaFppHit( wire[i], ltdc[i], ttdc[i]);            
       }
     }
     else if(wire[i] != prev_wire){
            if(i-1 < 0){prev_wire = wire[i]; continue;}
            new( (*fHits)[nextHit++] )  THaFppHit( wire[i-1], ltdc[i-1], ttdc[i-1]);            

     }
     prev_wire = wire[i];
   }*/

/*  if ( fDebug > 3 ) {
    printf("\nFpp %s:\n",GetPrefix());
    int ncol=8;
    for (int i=0; i<ncol; i++) {
      printf("     Wire    LEtdc     TEtdc     Opt");
    }
    printf("\n");
    
    for (int i=0; i<(nextHit+ncol-1)/ncol; i++ ) {
      for (int c=0; c<ncol; c++) {
	int ind = c*nextHit/ncol+i;
	if (ind < nextHit) {
	  THaFppHit* hit = static_cast<THaFppHit*>((*fHits)[ind]);
	  printf("     %3d    %5.0f    %5.0f    %3d",hit->GetWire(),hit->GetLEtime(),hit->GetTEtime(),hit->GetOpt());
	} else {
	  //	  printf("\n");
	  break;
	}
      }
      printf("\n");
    }
  }*/

  return 0;

}
///////////////////////////////////////////////////////////////////////////////
ClassImp(THaFppPlane)
