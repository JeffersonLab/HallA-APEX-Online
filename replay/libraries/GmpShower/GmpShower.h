#ifndef ROOT_GmpShower
#define ROOT_GmpShower

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// GmpShower                                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaShower.h"
#include "TClonesArray.h"
#include "GmpShowerBlock.h"
#include "GmpShowerCluster.h"

class GmpShower : public THaShower {

public:
  GmpShower( const char* name, const char* description = "",
	     THaApparatus* a = NULL );
  virtual ~GmpShower();

  virtual Int_t      Decode( const THaEvData& );
  virtual Int_t      CoarseProcess( TClonesArray& tracks );
  virtual Int_t      FineProcess( TClonesArray& tracks );
  const TClonesArray* GetTrackHits() const { return fTrackProj; }
        Int_t GetNTracks() const { return fTrackProj->GetLast()+1; }

protected:

  TClonesArray* fBlocks;
  TClonesArray* fClusters;
  TClonesArray* fTrackProj;
  GmpShowerBlock** fBlksptr;

  // Per-event data
  Int_t      fNHitBlock;      // Number of blocks in clusters

  void           ClearLastEvent();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );
  
  ClassDef(GmpShower,0)     //Generic shower detector class
};

////////////////////////////////////////////////////////////////////////////////

#endif
