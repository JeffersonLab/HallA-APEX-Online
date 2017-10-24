#ifndef Tritium_THaScaler100EvtHandler_
#define Tritium_THaScaler100EvtHandler_

/////////////////////////////////////////////////////////////////////
//
//   Tritium_THaScaler100EvtHandler
//   Class to handle Hall A scaler events (type 100)
//   author  Robert Michaels (rom@jlab.org)
//
/////////////////////////////////////////////////////////////////////

#include "THaEvtTypeHandler.h"
#include "Decoder.h"
#include <string>
#include <vector>
#include "TTree.h"
#include "TString.h"  

class ScalerLoc5 { // Utility class used by Tritium_THaScaler100EvtHandler
 public:
 ScalerLoc5(TString nm, TString desc, Int_t idx, Int_t sl, Int_t ich, Int_t iki) :
  name(nm), description(desc), index(idx), islot(sl), ichan(ich), ikind(iki) { };
  ~ScalerLoc5();
  TString name, description;
  UInt_t index, islot, ichan, ivar, ikind;
  Bool_t found;
};

class Tritium_THaScaler100EvtHandler : public THaEvtTypeHandler {

public:

   Tritium_THaScaler100EvtHandler(const char* name, const char* description);
   virtual ~Tritium_THaScaler100EvtHandler();

   virtual Int_t Analyze(THaEvData *evdata);
   virtual EStatus Init( const TDatime& run_time);
   virtual Int_t End( THaRunBase* r=0 );


private:

   void AddVars(TString name, TString desc, Int_t iscal, Int_t ichan, Int_t ikind);
   void DefVars();

   std::vector<Decoder::GenScaler*> scalers;
   std::vector<ScalerLoc5*> scalerloc;
   Double_t evcount;
   UInt_t *rdata;
   Int_t fNormIdx, fNormSlot;
   Double_t *dvars;
   TTree *fScalerTree;

   Tritium_THaScaler100EvtHandler(const Tritium_THaScaler100EvtHandler& fh);
   Tritium_THaScaler100EvtHandler& operator=(const Tritium_THaScaler100EvtHandler& fh);

   ClassDef(Tritium_THaScaler100EvtHandler,0)  // Scaler Event handler

};

#endif
