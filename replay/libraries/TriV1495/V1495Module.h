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

   Double_t GetTevent() const { return tEVT; };
   Int_t Decode(const UInt_t *p) { return 0; };
   Int_t GetCount();

   // Loads slot data for bank structures
   virtual Int_t LoadSlot(THaSlotData *sldat, const UInt_t *evbuffer, Int_t pos, Int_t len);
// Loads sldat and increments ptr to evbuffer
   Int_t LoadSlot(THaSlotData *sldat,  const UInt_t* evbuffer, const UInt_t *pstop );

private:


   Int_t *fNumHitsP;
   Int_t *fNumHitsN;
   UInt_t *vCount;
   Double_t tEVT;

   Bool_t IsInit;
   Int_t slotmask, chanmask, datamask;

   static TypeIter_t fgThisType;
   ClassDef(V1495Module,0)  //  CAEN V1495 custom Clock Counter

};

}

#endif
