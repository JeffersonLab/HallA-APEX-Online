////////////////////////////////////////////////////////////////////
//
//   TdcDataEvtHandler
//   author  Robert Michaels (rom@jlab.org), Jan 2017
//
//   This Version: ClockCountEvtHandler for custom V1495 Clock Counter Readout
//   R. Evan McClellan (randallm@jlab.org), 2018-01-29
//
//   Example of an Event Type Handler.
//   This class handles events of the type specified in the vector eventtypes.
//   It was tested on some EPICS data in hall A, and so it is a primitive
//   EPICS event type handler.
//
//   The idea would be to copy this and modify it for your purposes.
//
//   To use as a plugin with your own modifications, you can do this in
//   your analysis script
//     gHaEvtHandlers->Add (new ClockCountEvtHandler("example1","for evtype 131"));
//
//   The data specified in the dataKeys array appear in Podd's global variables.
//   and can be sent to the ROOT output, and plotted or analyzed from the
//   ROOT Tree "T".
//   Example:  T->Draw("IPM1H04B.YPOS:fEvtHdr.fEvtNum")
//
/////////////////////////////////////////////////////////////////////

//#define WITH_DEBUG 0

#include "ClockCountEvtHandler.h"
#include "V1495Module.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "THaVarList.h"
#include <cstdlib>   // for atof
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

static const Int_t MAXDATA=20000;

ClockCountEvtHandler::ClockCountEvtHandler(const char *name, const char* description)
  : THaEvtTypeHandler(name,description), dvars(0)
{
nameArm1495 = name;
}

ClockCountEvtHandler::~ClockCountEvtHandler()
{
}

// GetData is a public method which other classes may use
// 	I have no idea what this method is doing, so I will leave it for now -- REM -- 2018-02-01
Float_t ClockCountEvtHandler::GetData(const std::string& tag) const
{
  map<string, Float_t>::const_iterator elem = theDataMap.find(tag);
  if (elem == theDataMap.end())
    return kBig;

  return elem->second;
}

Int_t ClockCountEvtHandler::Analyze(THaEvData *evdata)
{

  Int_t ldebug=0;

  if (ldebug) cout << "Entering: ClockCountEvtHander Analyze"<<endl;;

  Decoder::V1495Module *V1495 = NULL;


  // FIXME: Not getting the crate/slot numbers automatically yet. Hardcoding them for now.
  if(nameArm1495=="LV1495") { // LEFT HRS (ROC31, slot 18):  
    V1495 = dynamic_cast <Decoder::V1495Module* > (evdata->GetModule(31,18));	//FIXME: Need to get the actual slots for V1495s
  }
  else if(nameArm1495=="RV1495") { // RIGHT HRS (ROC20, slot 4):
    V1495 = dynamic_cast <Decoder::V1495Module* > (evdata->GetModule(20,4));	//FIXME: Need to get the actual slots for V1495s
  }
  else cout << "Did not recognize name of ClockCountEvtHandler decoder. Please call it using RV1495 or LV1495 to decode Clock Counter." << endl;

if(ldebug) {
  if(V1495==0) {
		cout << "ERROR: got not pointer for V1495." << endl;
		cout << "  Did you update the ROC and slot numbers in the declaration of the Decoder::V1495Module (file ClockCountEvtHandler.cxx) ?" << endl;
  }
}

  assert( fStatus == kOK );  // should never get here if Init() failed
  assert( dataKeys.size() == 0 ); // else logic error in Init()

  fDebug=0;
#ifdef WITH_DEBUG
  //  if( fDebug < 1 ) fDebug = 1; // force debug messages for now (testing)
#endif

  if ( !IsMyEvent(evdata->GetEvType()) )
    return -1;

  UInt_t evbuffer[MAXDATA];

  if (evdata->GetEvLength() >= MAXDATA)  {
    Error( Here("ClockCountEvtHandler::Analyze"), "need a bigger buffer!" );
    return 1;
  }

// Copy the buffer.  If the events are infrequent this causes no harm.
  for (Int_t i = 0; i < evdata->GetEvLength(); i++)
	 evbuffer[i] = evdata->GetRawData(i);

  char* cbuff = (char*)evbuffer;
  size_t len = sizeof(int)*(evbuffer[0]+1);
#ifdef WITH_DEBUG
  if (fDebug>1) cout << "Evt Handler Data, len = "<<len<<endl;
#endif
  if( len<16 )
    return 0;
  // The first 16 bytes of the buffer are the event header
  len -= 16;
  cbuff += 16;

  // Copy data to internal string buffer
  string buf( cbuff, len );

  // The first line is the time stamp
  string date;
  istringstream ib(buf);

  string line;
  while( getline(ib,line) ) {
#ifdef WITH_DEBUG
    if(fDebug) cout << "data line : "<<line<<endl;
#endif
    istringstream il(line);
    string wtag, wval, sunit;
    il >> wtag;
    if( wtag.empty() || wtag[0] == 0 ) continue;
    istringstream::pos_type spos = il.tellg();
    il >> wval >> sunit;
    Double_t dval;
    istringstream iv(wval);
    if( !(iv >> dval) ) {
      string::size_type lpos = line.find_first_not_of(" \t",spos);
      wval = ( lpos != string::npos ) ? line.substr(lpos) : "";
      dval = 0;
      sunit.clear();
    }
#ifdef WITH_DEBUG
    if(fDebug) cout << "wtag = "<<wtag<<"   wval = "<<wval
		      << "   dval = "<<dval<<"   sunit = "<<sunit<<endl;
#endif
    if (theDataMap.find(wtag) != theDataMap.end())
	 theDataMap[wtag] = (Float_t)atof(wval.c_str());

  }

  // Fill global variables
#ifdef WITH_DEBUG
  if (fDebug) cout << "---------------------------------------------"<<endl<<endl;
#endif

  for (UInt_t i=0; i < dataKeys.size(); i++) {
    //dvars[i] = GetData(dataKeys[i]);


#ifdef WITH_DEBUG
    if (fDebug)
      cout << "GetData ::  key " << i << "   " << dataKeys[i]
	   << "   data = " << GetData(dataKeys[i]) 
	   << endl;
#endif
  }

if(V1495!=0){
	V1495PrevCount = V1495ClockCount;
	V1495ClockCount = 0;
	if (fDebug) cout << "Before V1495->GetCount()     V1495ClockCount = " << hex << V1495ClockCount << dec << endl;
	V1495ClockCount = V1495->GetCount();
	if (fDebug) cout << "After V1495->GetCount()      V1495ClockCount = " << hex << V1495ClockCount << dec << endl;
	V1495ClockInterval = 0;
	V1495ClockInterval = V1495ClockCount - V1495PrevCount;
}

  return 1;
}


THaAnalysisObject::EStatus ClockCountEvtHandler::Init(const TDatime& date)
{

#ifdef WITH_DEBUG
  if( fDebug>1 )
    cout << "Hello !! We are initializing ClockCountEvtHandler !!   name =   "
	 <<fName<<endl;
#endif

  for (Int_t i=1; i<14; i++) eventtypes.push_back(i);  // what events to look for

  // data keys to look for in this fun example
  dataKeys.push_back(nameArm1495 + ".ClockCount");
  dataKeys.push_back(nameArm1495 + ".ClockInterval");

  // initialize map elements to -1 (means not found yet)
  for (UInt_t i=0; i < dataKeys.size(); i++) {
    if(theDataMap.insert(make_pair(dataKeys[i],-1)).second == false) {
      Warning( Here("ClockCountEvtHandler::Init"), "Element %s already defined",
	       dataKeys[i].c_str() );
    }
  }

  // After adding the dataKeys to the global variables (using lines below)
  // one can obtain them in the ROOT output using lines like this in the
  // output definition file.  like, T->Draw("IPM1H04B.YPOS:fEvtHdr.fEvtNum")
  // (Careful:  variables with ":" in the name don't plot well, i.e. T->Draw()
  // has a problem with the ":".  Also arithmetic characters.)
  //    variable hac_bcm_average
  //    variable IPM1H04B.YPOS

  UInt_t Nvars = dataKeys.size();
  dvars = new Double_t[Nvars];  // dvars is a member of this class


  Int_t numEntries = 0;
  // for Clock Count
  V1495ClockCount = 0;
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "ClockCount", &V1495ClockCount, kUInt, 0);
  numEntries++;
  // for Clock Interval
  V1495ClockInterval = 0;
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "ClockInterval", &V1495ClockInterval, kUInt, 0);
  numEntries++;
  
  fStatus = kOK;
  return kOK;
}

ClassImp(ClockCountEvtHandler)
