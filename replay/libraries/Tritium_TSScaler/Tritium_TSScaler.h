#ifndef Tritium_TSScaler_
#define Tritium_TSScaler_

/////////////////////////////////////////////////////////////////////
//
//   Tritium_TSScaler
//   Class to handle Hall A scaler events (type 1-14)
//   author  Robert Michaels (rom@jlab.org)
//
/////////////////////////////////////////////////////////////////////

#include "THaEvtTypeHandler.h"
#include "Decoder.h"
#include <string>
#include <vector>
#include "TTree.h"
#include "TString.h"  

class ScalerLoc4 { // Utility class used by Tritium_TSScaler
 public:
 ScalerLoc4(TString nm, TString desc, Int_t idx, Int_t sl, Int_t ich, Int_t iki) :
  name(nm), description(desc), index(idx), islot(sl), ichan(ich), ikind(iki) { };
  ~ScalerLoc4();
  TString name, description;
  UInt_t index, islot, ichan, ivar, ikind;
  Bool_t found;
};

class Tritium_TSScaler : public THaEvtTypeHandler {

public:

   Tritium_TSScaler(const char* name, const char* description);
   virtual ~Tritium_TSScaler();

   virtual Int_t Analyze(THaEvData *evdata);
   virtual EStatus Init( const TDatime& run_time);
   virtual Int_t End( THaRunBase* r=0 );


private:

   void AddVars(TString name, TString desc, Int_t iscal, Int_t ichan, Int_t ikind);
   void DefVars();

   std::vector<Decoder::GenScaler*> scalers;
   std::vector<ScalerLoc4*> scalerloc;
   Double_t evcount;
   UInt_t *rdata;
   Int_t fNormIdx, fNormSlot;
   Double_t *dvars;
   TTree *fScalerTree;

   Tritium_TSScaler(const Tritium_TSScaler& fh);
   Tritium_TSScaler& operator=(const Tritium_TSScaler& fh);

   ClassDef(Tritium_TSScaler,0)  // Scaler Event handler

};

#endif
