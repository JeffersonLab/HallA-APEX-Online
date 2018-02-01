#ifndef V1495Module_
#define V1495Module_

/////////////////////////////////////////////////////////////////////
//
//   V1495 Clock Counter Readout Module
//   R. Evan McClellan -- 2018-01-29
//
/////////////////////////////////////////////////////////////////////

#include "VmeModule.h"

namespace Decoder {

class V1495Module : public VmeModule {

public:


   V1495Module() {};
   V1495Module(Int_t crate, Int_t slot);
   virtual ~V1495Module();

   using Module::GetData;
   using Module::LoadSlot;


   virtual void Init();
   virtual void Clear(const Option_t *opt="");
   virtual Bool_t IsSlot(UInt_t rdata);

   Int_t Decode(const UInt_t *p) { return 0; };
   UInt_t GetCount();

   // Loads slot data for bank structures
   virtual Int_t LoadSlot(THaSlotData *sldat, const UInt_t *evbuffer, Int_t pos, Int_t len);
// Loads sldat and increments ptr to evbuffer
   Int_t LoadSlot(THaSlotData *sldat,  const UInt_t* evbuffer, const UInt_t *pstop );

private:


   UInt_t *vCount;

   Bool_t IsInit;
   static TypeIter_t fgThisType;

   ClassDef(V1495Module,0)  //  CAEN V1495 custom Clock Counter

};

}

#endif
