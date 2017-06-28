///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// GmpShower                                                                 //
//                                                                           //
// Shower counter class, describing a generic segmented shower detector      //
// (preshower or shower).                                                    //
// Currently, only the "main" cluster, i.e. cluster with the largest energy  //
// deposition is considered. Units of measurements are MeV for energy of     //
// shower and centimeters for coordinates.                                   //
//                                                                           //
// This class finds all clusters that have an energy deposition above the    //
// threshold defined in db. The clusters are sorted based on the energy      //
// deposition. The clusters with larger energy deposition comes before the   //
// ones with smaller signals in the relevant arrays (see below). Similarly,  // 
// in each cluster, the blocks are also sorted based on the energy           //
// deposition.                                                               //
//                                                                           //
// Definition of the variables (these are prl1 on the left arm. These        //
// variables have similiar names for prl2, ps, or sh detectors):             //
//                                                                           // 
// L.prl1.nclust        Double, number of clusters                           //
// L.prl1.e             Array, energy deposision in each cluster, largest    //
//                      energy comes first (dimension = L.prl1.nclust)       //
// L.prl1.mult          Array, multiplicity of clusters (dimension =         //
//                      L.prl1.nclust)                                       //
// L.prl1.x             Array, x-position (cm) of clusters (dimension =      //
//                      L.prl1.nclust)                                       //
// L.prl1.y             Array, y-position (cm) of clusters (dimension =      //
//                      L.prl1.nclust)                                       //
// L.prl1.nhitblock     Double, total number of blocks included in any       //
//                      cluster (equal to L.prl1.mult[0]+L.prl1.mult[1]+...  //
//                      +L.prl1.mult[L.prl1.nclust-1])                       //
// L.prl1.nblk          Array, logical number of blocks (dimension =         //
//                      L.prl1.nhitblk). e.g.,                               //
//      L.prl1.nblk[0]  logical number of first block in first cluster       //
//      L.prl1.nblk[1]  logical number of second block in first cluster      //
//          .                                                                //
//          .                                                                //
//          .                                                                //
//      L.prl1.nblk[L.prl1.mult[0]-1]   logical number of last block in      //
//                                      first cluster                        //
//      L.prl1.nblk[ L.prl1.mult[0] ]   logical number of first block in     //
//                                      second cluster                       //
//          .                                                                //
//          .                                                                //
//          .                                                                //
//      L.prl1.nblk[L.prl1.nhitblock-1] logical number of last block in      //
//                                      last cluster                         //
//                                                                           //
// L.prl1.eblk          Array, energy deposition in blocks (dimension =      //
//                      L.prl1.nhitblock)                                    //
// L.prl1.trx           Array, track x-position in det plane (dimension =    //
//                      number of tracks)                                    //
// L.prl1.try           Array, track y-position in det plane (dimension =    //
//                      number of tracks)                                    //
// L.prl1.trpath        Array, TRCS pathlen of track to det plane (dimension //
//                       = number of tracks)                                 //
//                                                                           //
// Other variables, i.e., L.prl1.nhit, L.prl1.a, L.prl1.a_p, L.prl1.a_c,     //
// L.prl1.asum_p, L.prl1.asum_c    are the same as in standard Hall A        // 
// analyzer                                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "GmpShower.h"
#include "THaTrackProj.h"
#include "THaGlobals.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "VarDef.h"
#include "VarType.h"
#include "THaTrack.h"
#include "TClonesArray.h"
#include "TDatime.h"
#include "TMath.h"

#include <cstring>
#include <iostream>

ClassImp(GmpShower)

using namespace std;

//_____________________________________________________________________________
GmpShower::GmpShower( const char* name, const char* description,
		      THaApparatus* apparatus ) :
  THaShower(name,description,apparatus), fBlksptr(NULL)
{
  // Constructor.
  fBlocks = new TClonesArray("GmpShowerBlock",200);
  fClusters = new TClonesArray("GmpShowerCluster",10);
  fTrackProj = new TClonesArray("THaTrackProj",5);

}

//_____________________________________________________________________________
Int_t GmpShower::ReadDatabase( const TDatime& date )
{
  // Read this detector's parameters from the database file 'fi'.
  // This function is called by THaDetectorBase::Init() once at the
  // beginning of the analysis.
  // 'date' contains the date/time of the run being analyzed.

  static const char* const here = "ReadDatabase()";
  const int LEN = 100;
  char buf[LEN];
  Int_t nelem, ncols, nrows, nclbl;

  // Read data from database

  FILE* fi = OpenFile( date );
  if( !fi ) return kFileError;

  // Blocks, rows, max blocks per cluster
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );          
  fscanf ( fi, "%d%d", &ncols, &nrows );  

  nelem = ncols * nrows;
  //nclbl = TMath::Min( 3, nrows ) * TMath::Min( 3, ncols );
  nclbl = nelem;
  // Reinitialization only possible for same basic configuration 
  if( fIsInit && (nelem != fNelem || nclbl != fNclublk) ) {
    Error( Here(here), "Cannot re-initalize with different number of blocks or "
	   "blocks per cluster. Detector not re-initialized." );
    fclose(fi);
    return kInitError;
  }

  if( nrows <= 0 || ncols <= 0 || nclbl <= 0 ) {
    Error( Here(here), "Illegal number of rows or columns: "
	   "%d %d", nrows, ncols );
    fclose(fi);
    return kInitError;
  }
  fNelem = nelem;
  fNrows = nrows;
  fNclublk = nclbl;

  // Clear out the old detector map before reading a new one
  UShort_t mapsize = fDetMap->GetSize();
  delete [] fNChan;
  if( fChanMap ) {
    for( UShort_t i = 0; i<mapsize; i++ )
      delete [] fChanMap[i];
  }
  delete [] fChanMap;
  fDetMap->Clear();

  // Read detector map

  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );
  while (1) {
    Int_t crate, slot, first, last;
    fscanf ( fi,"%d%d%d%d", &crate, &slot, &first, &last );
    fgets ( buf, LEN, fi );
    if( crate < 0 ) break;
    if( fDetMap->AddModule( crate, slot, first, last ) < 0 ) {
      Error( Here(here), "Too many DetMap modules (maximum allowed - %d).", 
	    THaDetMap::kDetMapSize);
      fclose(fi);
      return kInitError;
    }
  }

  // Set up the new channel map
  mapsize = fDetMap->GetSize();
  if( mapsize == 0 ) {
    Error( Here(here), "No modules defined in detector map.");
    fclose(fi);
    return kInitError;
  }

  fNChan = new UShort_t[ mapsize ];
  fChanMap = new UShort_t*[ mapsize ];
  for( UShort_t i=0; i < mapsize; i++ ) {
    THaDetMap::Module* module = fDetMap->GetModule(i);
    fNChan[i] = module->hi - module->lo + 1;
    if( fNChan[i] > 0 )
      fChanMap[i] = new UShort_t[ fNChan[i] ];
    else {
      Error( Here(here), "No channels defined for module %d.", i);
      delete [] fNChan; fNChan = NULL;
      for( UShort_t j=0; j<i; j++ )
	delete [] fChanMap[j];
      delete [] fChanMap; fChanMap = NULL;
      fclose(fi);
      return kInitError;
    }
  }
  // Read channel map
  //
  // Loosen the formatting restrictions: remove from each line the portion
  // after a '#', and do the pattern matching to the remaining string
  fgets ( buf, LEN, fi );

  // get the line and end it at a '#' symbol
  *buf = '\0';
  char *ptr=buf;
  int nchar=0;
  for ( UShort_t i = 0; i < mapsize; i++ ) {
    for ( UShort_t j = 0; j < fNChan[i]; j++ ) {
      while ( !strpbrk(ptr,"0123456789") ) {
	fgets ( buf, LEN, fi );
	if( (ptr = strchr(buf,'#')) ) *ptr = '\0';
	ptr = buf;
	nchar=0;
      }
      sscanf (ptr, "%hu%n", *(fChanMap+i)+j, &nchar );
      ptr += nchar;
    }
  }
  
  fgets ( buf, LEN, fi );

  Float_t x,y,z;
  fscanf ( fi, "%f%f%f", &x, &y, &z );               // Detector's X,Y,Z coord
  fOrigin.SetXYZ( x, y, z );
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );
  fscanf ( fi, "%f%f%f", fSize, fSize+1, fSize+2 );  // Sizes of det in X,Y,Z
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );

  Float_t angle;
  fscanf ( fi, "%f", &angle );                       // Rotation angle of det
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );
  const Double_t degrad = TMath::Pi()/180.0;
  tan_angle = TMath::Tan(angle*degrad);
  sin_angle = TMath::Sin(angle*degrad);
  cos_angle = TMath::Cos(angle*degrad);

  DefineAxes(angle*degrad);

  // Dimension arrays
  if( !fIsInit ) {
    fBlockX = new Float_t[ fNelem ];
    fBlockY = new Float_t[ fNelem ];
    fPed    = new Float_t[ fNelem ];
    fGain   = new Float_t[ fNelem ];

    // Per-event data
    fA    = new Float_t[ fNelem ];
    fA_p  = new Float_t[ fNelem ];
    fA_c  = new Float_t[ fNelem ];
    fNblk = new Int_t[ fNclublk ];
    fEblk = new Float_t[ fNclublk ];

    fIsInit = true;
  }

  fscanf ( fi, "%f%f", &x, &y );                     // Block 1 center position
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );
  Float_t dx, dy;
  fscanf ( fi, "%f%f", &dx, &dy );                   // Block spacings in x and y
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );
  fscanf ( fi, "%f", &fEmin );                       // Emin thresh for center
  fgets ( buf, LEN, fi );

  // Read calibrations.
  // Before doing this, search for any date tags that follow, and start reading from
  // the best matching tag if any are found. If none found, but we have a configuration
  // string, search for it.
  if( SeekDBdate( fi, date ) == 0 && fConfig.Length() > 0 && 
      SeekDBconfig( fi, fConfig.Data() )) {}

  fgets ( buf, LEN, fi );  
  // Crude protection against a missed date/config tag
  if( buf[0] == '[' ) fgets ( buf, LEN, fi );

  // Read ADC pedestals and gains (in order of logical channel number)
  for (int j=0; j<fNelem; j++)
    fscanf (fi,"%f",fPed+j);
  fgets ( buf, LEN, fi ); fgets ( buf, LEN, fi );
  for (int j=0; j<fNelem; j++) 
    fscanf (fi, "%f",fGain+j);


  fBlksptr = new GmpShowerBlock*[fNelem];
  // Compute block positions
  for( int c=0; c<ncols; c++ ) {
    for( int r=0; r<nrows; r++ ) {
      int k = nrows*c + r;
      fBlockX[k] = x + r*dx;                         // Units are meters
      fBlockY[k] = y + c*dy;
      fBlksptr[k] = new GmpShowerBlock(fBlockX[k],fBlockY[k],r,c,k);
      //new ((*fBlocks)[k]) GmpShowerBlock(fBlockX[k],fBlockY[k],r,c,k);
    }
  }
  fclose(fi);
  return kOK;
}

//_____________________________________________________________________________
Int_t GmpShower::DefineVariables( EMode mode )
{
  // Initialize global variables

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  // Register variables in global list

  RVarDef vars[] = {
    { "nhit",   "Number of hits",                     "fNhits" },
    { "a",      "Raw ADC amplitudes",                 "fA" },
    { "a_p",    "Ped-subtracted ADC amplitudes",      "fA_p" },
    { "a_c",    "Calibrated ADC amplitudes",          "fA_c" },
    { "asum_p", "Sum of ped-subtracted ADCs",         "fAsum_p" },
    { "asum_c", "Sum of calibrated ADCs",             "fAsum_c" },
    { "nclust", "Number of clusters",                 "fNclust" },
    //{ "e",      "Energy (MeV) of largest cluster",    "fE" },
    //{ "x",      "x-position (cm) of largest cluster", "fX" },
    //{ "y",      "y-position (cm) of largest cluster", "fY" },
    //{ "mult",   "Multiplicity of largest cluster",    "fMult" },
    //{ "nblk",   "Numbers of blocks in main cluster",  "fNblk" },
    //{ "eblk",   "Energies of blocks in main cluster", "fEblk" },
    { "nhitblock",   "Number of blocks in all clusters",    "fNHitBlock" },
    { "e",      "Energy (MeV) of clusters",    "fClusters.GmpShowerCluster.fE" },
    { "x",      "x-position (cm) of clusters", "fClusters.GmpShowerCluster.fCenterX" },
    { "y",      "y-position (cm) of clusters", "fClusters.GmpShowerCluster.fCenterY" },
    { "mult",   "Multiplicity of clusters",    "fClusters.GmpShowerCluster.GetMult()" },
    { "nblk",   "Numbers of blocks in clusters",  "fNblk" },
    { "eblk",   "Energies of blocks in clusters", "fEblk" },
    //{ "nblk",   "Numbers of blocks in main cluster",  "fNblk" },
    //{ "eblk",   "Energies of blocks in main cluster", "fEblk" },
    //{ "trx",    "track x-position in det plane",      "fTRX" },
    //{ "try",    "track y-position in det plane",      "fTRY" },
    { "trx",    "track x-position in det plane",      "fTrackProj.THaTrackProj.fX" },
    { "try",    "track y-position in det plane",      "fTrackProj.THaTrackProj.fY" },
    { "trpath",    "TRCS pathlen of track to det plane",      "fTrackProj.THaTrackProj.fPathl" },
    { 0 }
  };
  return DefineVarsFromList( vars, mode );
}

//_____________________________________________________________________________
GmpShower::~GmpShower()
{
  // Destructor. Removes internal arrays and global variables.

  if (fIsSetup) RemoveVariables();

  for (Int_t i=0; i<fNelem; i++) {
      delete fBlksptr[i];
      fBlksptr[i] = 0;
  }

  delete [] fBlksptr; fBlksptr = 0;
  fBlocks->Clear();
  delete fBlocks;
  fBlocks = 0;
  fClusters->Clear();
  delete fClusters;
  fClusters = 0;
  fTrackProj->Clear();
  delete fTrackProj;
  fTrackProj = 0;
}

//_____________________________________________________________________________
inline
void GmpShower::ClearLastEvent()
{
  // Reset all local data to prepare for next event.

  fNHitBlock = 0;

  fBlocks->Clear();
  fClusters->Clear();
  fTrackProj->Clear();
}

//_____________________________________________________________________________
Int_t GmpShower::Decode( const THaEvData& evdata )
{
  // Decode shower data, scale the data to energy deposition
  // ( in MeV ), and copy the data into the following local data structure:
  //
  // fNhits           -  Number of hits on shower;
  // fA[]             -  Array of ADC values of shower blocks;
  // fA_p[]           -  Array of ADC minus ped values of shower blocks;
  // fA_c[]           -  Array of corrected ADC values of shower blocks;
  // fAsum_p          -  Sum of shower blocks ADC minus pedestal values;
  // fAsum_c          -  Sum of shower blocks corrected ADC values;
    
    ClearLastEvent();

    Int_t nHit = THaShower::Decode(evdata);

    for (Int_t k=0; k<fNelem; k++) {
        GmpShowerBlock* block = new ((*fBlocks)[k]) GmpShowerBlock(fBlksptr[k]);
        block->SetEnergy(TMath::Max(0.0,(Double_t)fA_c[k]));
    }

    return nHit;
}

//_____________________________________________________________________________
Int_t GmpShower::CoarseProcess( TClonesArray& tracks )
{
    // Reconstruct Clusters in shower detector and copy the data 
    // into the following local data structure:
    //
    // fNclust        -  Number of clusters in shower;
    // fE             -  Energy (in MeV) of the "main" cluster;
    // fX             -  X-coordinate (in cm) of the cluster;
    // fY             -  Y-coordinate (in cm) of the cluster;
    // fMult          -  Number of blocks in the cluster;
    // fNblk[0]...[5] -  Numbers of blocks composing the cluster;
    // fEblk[0]...[5] -  Energies in blocks composing the cluster;
    // fTRX;          -  X-coordinate of track cross point with shower plane
    // fTRY;          -  Y-coordinate of track cross point with shower plane
    //
    // Only one ("main") cluster, i.e. the cluster with the largest energy 
    // deposition is considered. Units are MeV for energies and cm for 
    // coordinates.

    ///////////////////////////////////////////////////////////////////////////////

    Int_t nextClust = fClusters->GetLast()+1;   // Should be zero

    fBlocks->Sort();
    Double_t MaxBlockEnergy = 0.0;
    GmpShowerBlock* emaxblk = static_cast<GmpShowerBlock*>(fBlocks->At(0));
    if (emaxblk) MaxBlockEnergy = emaxblk->GetEnergy();

    while (MaxBlockEnergy>fEmin) {

        Int_t MaxBlockRow = emaxblk->GetRow();
        Int_t MaxBlockCol = emaxblk->GetColumn();

        GmpShowerCluster* cluster = new ((*fClusters)[nextClust++]) GmpShowerCluster;
        cluster->AddBlock(emaxblk);
        fBlocks->RemoveAt(0);

        for (Int_t i=1; i<fBlocks->GetLast()+1; i++) {
            GmpShowerBlock* blk = static_cast<GmpShowerBlock*>(fBlocks->At(i));
            if (blk) {
                Int_t row = blk->GetRow();
                Int_t col = blk->GetColumn();
                if (TMath::Abs(row-MaxBlockRow)<2 && TMath::Abs(col-MaxBlockCol)<2 
                        && blk->GetEnergy()>0) {
                    cluster->AddBlock(blk);
                    fBlocks->RemoveAt(i);
                }
            }
        }

        cluster->CalcCluster();

        MaxBlockEnergy = 0.0;
        fBlocks->Sort();
        emaxblk = static_cast<GmpShowerBlock*>(fBlocks->At(0));
        if (emaxblk) MaxBlockEnergy = emaxblk->GetEnergy();

    }

    Int_t nHitBlock = 0;
    fClusters->Sort();
    const GmpShowerCluster* mainCluster = static_cast<const GmpShowerCluster*>(fClusters->At(0));
    if (mainCluster) {
        fE = mainCluster->GetE();
        fX = mainCluster->GetCtrX();
        fY = mainCluster->GetCtrY();
        fMult=mainCluster->GetMult();

        for (Int_t i=0; i<mainCluster->GetMult(); i++) {
            const GmpShowerBlock* hitblk = static_cast<const GmpShowerBlock*>(mainCluster->GetBlocks()->At(i));
            fNblk[nHitBlock] = hitblk ? hitblk->GetNumber() : -1;
            fEblk[nHitBlock++] = hitblk ? hitblk->GetEnergy() : 0.0;
        }
    }

    for (Int_t i=1; i<fClusters->GetLast()+1; i++) {
        const GmpShowerCluster* cluster = static_cast<const GmpShowerCluster*>(fClusters->At(i));
        if (cluster) {
            for (Int_t i=0; i<cluster->GetMult(); i++) {
                const GmpShowerBlock* hitblk = static_cast<const GmpShowerBlock*>(cluster->GetBlocks()->At(i));
                fNblk[nHitBlock] = hitblk ? hitblk->GetNumber() : -1;
                fEblk[nHitBlock++] = hitblk ? hitblk->GetEnergy() : 0.0;
            }
        }
    }

    fNclust = fClusters->GetLast()+1;
    fNHitBlock = nHitBlock;


    int n_track = tracks.GetLast()+1;   // Number of reconstructed tracks

    for ( int i=0; i<n_track; i++ ) {
        THaTrack* theTrack = static_cast<THaTrack*>( tracks[i] );
        Double_t pathl=kBig, xc=kBig, yc=kBig;
        Double_t dx=0.; // unused
        Int_t pad=-1;   // unused

        CalcTrackIntercept(theTrack, pathl, xc, yc);
        // if it hit or not, store the information (defaults if no hit)
        new ( (*fTrackProj)[i] ) THaTrackProj(xc,yc,pathl,dx,pad,this);
    }

    return 0;
}
//_____________________________________________________________________________
Int_t GmpShower::FineProcess( TClonesArray&  tracks  )
{
  // Fine Shower processing.
  // Not implemented. Does nothing, returns 0.
  fTrackProj->Clear();
  int n_track = tracks.GetLast()+1;   // Number of reconstructed tracks

  for ( int i=0; i<n_track; i++ ) {
    THaTrack* theTrack = static_cast<THaTrack*>( tracks[i] );
    Double_t pathl=kBig, xc=kBig, yc=kBig;
    Double_t dx=0.; // unused
    Int_t pad=-1;   // unused

    CalcTrackIntercept(theTrack, pathl, xc, yc);
    // if it hit or not, store the information (defaults if no hit)
    new ( (*fTrackProj)[i] ) THaTrackProj(xc,yc,pathl,dx,pad,this);
  }


  return 0;
}

///////////////////////////////////////////////////////////////////////////////

