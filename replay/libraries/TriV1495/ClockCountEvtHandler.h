#ifndef ClockCountEvtHandler_
#define ClockCountEvtHandler_

/////////////////////////////////////////////////////////////////////
//
//   ClockCountEvtHandler
//   For more details see the implementation file.
//   The idea would be to copy this and modify it for your purposes.
//   author  Robert Michaels (rom@jlab.org)
//
/////////////////////////////////////////////////////////////////////

#include "THaEvtTypeHandler.h"
#include <string>
#include <vector>
#include <map>

class ClockCountEvtHandler : public THaEvtTypeHandler {

public:

   ClockCountEvtHandler(const char* name, const char* description);
   virtual ~ClockCountEvtHandler();

   virtual Int_t Analyze(THaEvData *evdata);
   virtual EStatus Init( const TDatime& run_time);
   Float_t GetData(const std::string& tag) const;
   //   Bool_t IsMyEvent(Int_t evnum) const;

private:

   std::map<std::string, Float_t> theDataMap;
   std::vector<std::string> dataKeys;
   Double_t *dvars;
   // VETROC
   std::vector<UInt_t> nHits;
   std::vector<UInt_t> FirstHit;
   //F1
   std::vector<UInt_t> F1nHits;
   std::vector<UInt_t> F1FirstHit;
   //CAEN
   std::vector<UInt_t> CnHits;
   std::vector<UInt_t> CFirstHit;
   //V1495
//   std::vector<UInt_t> V1495ClockCount;
   UInt_t V1495ClockCount[2];

   // to get channel N only
   Int_t nchs;
   std::vector<Int_t> chN;
   std::vector<std::vector<UInt_t> > AllHitNp;
   std::vector<std::vector<UInt_t> > AllHitNpFine;
   Int_t F1nchs;
   std::vector<Int_t> F1chN;
   std::vector<std::vector<UInt_t> > F1AllHitNp;
   Int_t Cnchs;
   std::vector<Int_t> CchN;
   std::vector<std::vector<UInt_t> > CAllHitNp;
   //vector<vector<UInt_t> > AllHitNp(NTDCCHAN);
   //std::vector<Int_t> AllHitN;
   
   Int_t totalF1channels;

   ClockCountEvtHandler(const ClockCountEvtHandler& fh);
   ClockCountEvtHandler& operator=(const ClockCountEvtHandler& fh);

   ClassDef(ClockCountEvtHandler,0)  // Analyze some TDC data

};

#endif
