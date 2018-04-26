#ifndef ClockCountEvtHandler_
#define ClockCountEvtHandler_

/////////////////////////////////////////////////////////////////////
//
//   ClockCountEvtHandler
//   For more details see the implementation file.
//   The idea would be to copy this and modify it for your purposes.
//   author  Robert Michaels (rom@jlab.org)
//
//   modified into ClockCountEvtHandler by Evan McClellan -- 2018-02-01
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
   //V1495
   UInt_t V1495ClockCount;
   UInt_t V1495PrevCount;
   UInt_t V1495ClockInterval;
   std::string nameArm1495;

   ClockCountEvtHandler(const ClockCountEvtHandler& fh);
   ClockCountEvtHandler& operator=(const ClockCountEvtHandler& fh);

   ClassDef(ClockCountEvtHandler,0)  // Analyze some TDC data

};

#endif
