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
}


Bool_t V1495Module::IsSlot(UInt_t rdata)
{
  if (fDebugFile)
    *fDebugFile << "is V1495 slot ? "<<hex<<fHeader
		<<"  "<<fHeaderMask<<"  "<<rdata<<dec<<endl;
  return true;	//This function always returned true anyway, might as well make it obvious -- REM -- 2018-02-01
}

void V1495Module::Clear(const Option_t* opt) {
  VmeModule::Clear(opt);
  memset(vCount, 0, sizeof(UInt_t));
}

UInt_t V1495Module::GetCount() {
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

   // look at all the data
   const UInt_t *loc = evbuffer;
   Int_t fDebug=2;

   // For checks
   Bool_t gotCntHeader=0;

   while ( loc <= pstop ) {
	cout << "";

		// Check for Count Header to start decoding
		if((*loc)==0x14951495) {
			gotCntHeader=1;
			loc++;
			continue; // this was a BlkHeader, so make sure not to check for event in this word
		}
		// Check for Count Trailer and stop decoding
		if((*loc)==0x14950000) {
			gotCntHeader=0;
			break; // Done Decoding, Break out of the while loop
		}
		// gotCntHeader true? Store the Clock Count Word
		if(gotCntHeader) {
			*vCount = (*loc);
			fWordsSeen++;	//Not sure if this is actually needed...  -- REM -- 2018-02-01
		}

        loc++;
	}

	// Make sure we got block trailer
	//if(gotBlkHeader==1) cout << "VETROC: Warning! Event finished without BlockTrailer. Problem saving data?" << endl;

  return fWordsSeen;
}

}

ClassImp(Decoder::V1495Module)
