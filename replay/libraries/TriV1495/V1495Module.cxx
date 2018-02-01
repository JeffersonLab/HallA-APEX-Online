/////////////////////////////////////////////////////////////////////
//
//   V1495Module
//
/////////////////////////////////////////////////////////////////////
//
//  v0 -- R. Evan McClellan -- 2018-01-29
//
/////////////////////////////////////////////////////////////////////

#include "V1495Module.h"
#include "VmeModule.h"
#include "THaEvData.h"
#include "TMath.h"
#include <iostream>
#include <string>
#include <sstream>

double evtM = 0;
Bool_t saveForCalib = 0;

using namespace std;

namespace Decoder {


  Module::TypeIter_t V1495Module::fgThisType =
    DoRegister( ModuleType( "Decoder::V1495Module" , 1495 ));

V1495Module::V1495Module(Int_t crate, Int_t slot) : VmeModule(crate, slot) {
  fDebugFile=0;
  Init();
}

V1495Module::~V1495Module() {
  if (vCount) delete [] vCount;
}

void V1495Module::Init() {
  vCount = new UInt_t;
  fDebugFile=0;
  Clear();
  IsInit = kTRUE;
  fName = "V1495";
  fWdcntMask=0;
}


Bool_t V1495Module::IsSlot(UInt_t rdata)
{
  if (fDebugFile)
    *fDebugFile << "is V1495 slot ? "<<hex<<fHeader
		<<"  "<<fHeaderMask<<"  "<<rdata<<dec<<endl;
  return ((rdata != 0xffffffff) & ((rdata & fHeaderMask)==fHeader));
}

void V1495Module::Clear(const Option_t* opt) {
  VmeModule::Clear(opt);
  memset(vCount, 0, sizeof(UInt_t));
}

UInt_t V1495Module::GetCount() {
  cout << "within GetCount(): vCount = " << showbase << hex << *vCount << dec << endl;
  return *vCount;
}

Int_t V1495Module::LoadSlot(THaSlotData *sldat, const UInt_t *evbuffer, Int_t pos, Int_t len) {
// the 4-arg version of LoadSlot.  Let it call the 3-arg version.
// I'm not sure we need both (historical)

    return LoadSlot(sldat, evbuffer+pos, evbuffer+pos+len);

  }

Int_t V1495Module::LoadSlot(THaSlotData *sldat, const UInt_t *evbuffer, const UInt_t *pstop) {
// This is the 3-arg version of LoadSlot
// Note, this increments evbuffer
  fWordsSeen = 0;
  cout << "Clock Count Got Here 0" << endl;

   // look at all the data
   const UInt_t *loc = evbuffer;
   Int_t fDebug=2;

   // VETROC
   const UInt_t VET_BlkHeader = 1<<31;
   const UInt_t VET_BlkHeader_mask = 0xF8000000;
   UInt_t VET_BlkSlot;
   const UInt_t VET_slot_mask      = 0x07C00000;
   const UInt_t VET_id = 0x9;
   const UInt_t VET_id_mask        = 0x003C0000;
   UInt_t VET_BlkNum;
   const UInt_t VET_BlkNum_mask    = 0x0003FF00;
   UInt_t VET_BlkLevel;
   const UInt_t VET_BlkLevel_mask  = 0x000000FF;

   // For checks
   Bool_t gotCntHeader=0;
   Bool_t gotEvtHeader=0;

   UInt_t evtNum, evtSlot;
   UInt_t timeA=0;
   UInt_t timeB=0;
   Int_t t_hit;

//   while ( loc <= pstop && IsSlot(*loc) ) {
   while ( loc <= pstop ) {
	cout << "";
	if((*loc) == 0x14951495) cout << "Got Bank 0x5d7 Blk Header!" << endl; //sanity -- REM -- 2018-01-30
	cout << "ClockCount: loc, *loc = " << loc << ", " << showbase << internal << setfill('0') << hex << setw(8) << *loc << dec << endl;

		// Check for CntHeader start decoding
		if((*loc)==0x14951495) {
			gotCntHeader=1;
			loc++;
			continue; // this was a BlkHeader, so make sure not to check for event in this word
		}
		// Check for CntHeader and stop decoding
		if((*loc)==0x14950000) {
			gotCntHeader=0;
			break; // Done Decoding, Break out of the while loop
		}
		// Do we have gotCntHeader already? So we are readying data!
		if(gotCntHeader) {
			*vCount = (*loc);
			cout << "within loc loop: vCount = " << hex << *vCount << dec << endl;
		}

        loc++;
	}

	// Make sure we got block trailer
	//if(gotBlkHeader==1) cout << "VETROC: Warning! Event finished without BlockTrailer. Problem saving data?" << endl;

  cout << "Clock Count Got Here 1" << endl;
  return fWordsSeen;
}

}

ClassImp(Decoder::V1495Module)
