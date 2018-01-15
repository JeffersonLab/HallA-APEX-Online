////////////////////////////////////////////////////////////////////
//
//   TdcDataEvtHandler
//   author  Robert Michaels (rom@jlab.org), Jan 2017
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
//     gHaEvtHandlers->Add (new TdcDataEvtHandler("example1","for evtype 131"));
//
//   The data specified in the dataKeys array appear in Podd's global variables.
//   and can be sent to the ROOT output, and plotted or analyzed from the
//   ROOT Tree "T".
//   Example:  T->Draw("IPM1H04B.YPOS:fEvtHdr.fEvtNum")
//
/////////////////////////////////////////////////////////////////////

//#define WITH_DEBUG 0

#include "TdcDataEvtHandler.h"
#include "VETROCtdcModule.h"
#include "Caen1190Module.h"
#include "TstF1TDCModule.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "THaVarList.h"
#include <cstdlib>   // for atof
#include <iostream>
#include <sstream>
#include <cassert>

// why??
Int_t NTDCCHAN=192;
Int_t NCHAN_F1=32;
Int_t NCHAN_CAEN=32;

TString nameArm;

using namespace std;

static const Int_t MAXDATA=20000;

TdcDataEvtHandler::TdcDataEvtHandler(const char *name, const char* description)
  : THaEvtTypeHandler(name,description), dvars(0)
{
nameArm = name;
}

TdcDataEvtHandler::~TdcDataEvtHandler()
{
}

// GetData is a public method which other classes may use
Float_t TdcDataEvtHandler::GetData(const std::string& tag) const
{
  map<string, Float_t>::const_iterator elem = theDataMap.find(tag);
  if (elem == theDataMap.end())
    return kBig;

  return elem->second;
}



Int_t TdcDataEvtHandler::Analyze(THaEvData *evdata)
{

  Int_t ldebug=0;

  if (ldebug) cout << "Entering: TdcDataEvtHander Analyze"<<endl;;

  Decoder::VETROCtdcModule *vetroc = NULL;
  Decoder::TstF1TDCModule  *f1     = NULL;
  //Decoder::Caen1190Module *caen = NULL;

// Tentative to get crate/module number
//for(Int_t i=0; i< fDetMap->GetSize(); i++) {
  //THaDetMap::Module* d = fDetMap->GetModule( i );
  //THaDetMap::Module* d = fDetMap->GetModule( 0 );
  //cout << "MAPC: crate: " << d->crate << endl;
//}

  // FIXME: Not getting the crate/slot numbers automatically yet. Hardcoding them for now.
  if(nameArm=="LTDC") { // LEFT HRS (ROC31, slot 14 and 16):  
    //f1     = dynamic_cast <Decoder::TstF1TDCModule* > (evdata->GetModule(31,14));
    f1     = dynamic_cast <Decoder::TstF1TDCModule* > (evdata->GetModule(31,16));
  }
  else if(nameArm=="RTDC") { // RIGHT HRS:
  //if(f1==0) { // it was not the left arm, so try the right arm now
    vetroc = dynamic_cast <Decoder::VETROCtdcModule* > (evdata->GetModule(20,19));
    f1     = dynamic_cast <Decoder::TstF1TDCModule* > (evdata->GetModule(20,8));
    //cout << "Nf1 = " << f1->GetNumSlots() << endl;
    //f1     = dynamic_cast <Decoder::TstF1TDCModule* > (evdata->GetModule(20,10));
  }
  else cout << "Did not recognize name of TdcDataEvtHandler decoder. Please call it using RTDC or LTDC to decode F1 and VETROC tdcs." << endl;

//  if(vetroc!=0) cout << "vetroc pointer ?  "<<vetroc<<endl;

//cout << "MAPC: " << evdata->GetEvType();
//if(vetroc!=0) cout << " vetroc!" << endl;
//else cout << endl;


  // TestLab - for pre-calibration
  //vetroc = dynamic_cast <Decoder::VETROCtdcModule* > (evdata->GetModule(10,17));
  //f1     = dynamic_cast <Decoder::TstF1TDCModule* > (evdata->GetModule(10,20));
  //caen = dynamic_cast <Decoder::Caen1190Module* > (evdata->GetModule(10, 19));


if(ldebug) {
  if(vetroc==0) {
		cout << "ERROR: got not pointer for VETROC." << endl;
		cout << "  Did you update the ROC and slot numbers in the declaration of the Decoder::VETROCtdcModule (file TdcDataEvtHandler.cxx) ?" << endl;
  }
  if(f1==0) {
		cout << "ERROR: got not pointer for F1." << endl;
		cout << "  Did you update the ROC and slot numbers in the declaration of the Decoder::TstF1TDCModule (file TdcDataEvtHandler.cxx) ?" << endl;
  }
  cout << "vetroc pointer ?  "<<vetroc<<endl;
}

  assert( fStatus == kOK );  // should never get here if Init() failed
//MARCO
//  assert( dataKeys.size() == 0 || dvars != 0 ); // else logic error in Init()
  assert( dataKeys.size() == 0 ); // else logic error in Init()

  fDebug=0;
#ifdef WITH_DEBUG
  //  if( fDebug < 1 ) fDebug = 1; // force debug messages for now (testing)
#endif

  if ( !IsMyEvent(evdata->GetEvType()) )
    return -1;

  UInt_t evbuffer[MAXDATA];

  if (evdata->GetEvLength() >= MAXDATA)  {
    Error( Here("TdcDataEvtHandler::Analyze"), "need a bigger buffer!" );
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

// MARCO - VETROC
if(vetroc!=0) {
	for(Int_t i=0; i<NTDCCHAN; i++) {
	    nHits[i] = vetroc->GetNumHits(i,0);
		FirstHit[i] = vetroc->GetHit(i,0);
	}
	
	for(Int_t i=0; i<nchs; i++) {
	  AllHitNp[i].clear();
	  AllHitNp[i].resize(vetroc->GetNumHits(chN[i]));
	//cout << "--> ch=" << chN[i] << " - numHits=" << vetroc->GetNumHits(chN[i]) << endl;
	  for(Int_t j=0; j<vetroc->GetNumHits(chN[i]); j++) {
	    AllHitNp[i][j] = vetroc->GetHit(chN[i],j);
	  }

	  // FINE hit for calibration
	  AllHitNpFine[i].clear();
	  AllHitNpFine[i].resize(vetroc->GetNumHits(chN[i]));
	//cout << "--> ch=" << chN[i] << " - numHits=" << vetroc->GetNumHits(chN[i]) << endl;
	  for(Int_t j=0; j<vetroc->GetNumHits(chN[i]); j++) {
	    AllHitNpFine[i][j] = vetroc->GetFine(chN[i],j);
	  }
	}
}

// MARCO - F1
if(f1!=0) {

    // check for sizes for automatic identification of F1 slots - not working yet...
    /*
    if(totalF1channels != NCHAN_F1*f1->GetNumSlots()) {
//cout << "Reshaping..." << endl;
		totalF1channels = NCHAN_F1*f1->GetNumSlots();
		F1nHits.clear();
		F1nHits.resize(totalF1channels);
		F1FirstHit.clear();
		F1FirstHit.resize(totalF1channels);
	}*/
	totalF1channels = NCHAN_F1*2; // FIXME: hardcoded to read data from two modules
	F1nHits.clear();
	F1nHits.resize(totalF1channels);
	F1FirstHit.clear();
	F1FirstHit.resize(totalF1channels);

	for(Int_t i=0; i<totalF1channels; i++) {
	    F1nHits[i] = f1->GetNumHits(i);
		//F1FirstHit[i] = f1->GetHit(i,0);
		F1FirstHit[i] = f1->GetData(i,0);
	}
	
	for(Int_t i=0; i<F1nchs; i++) {
	  F1AllHitNp[i].clear();
	  F1AllHitNp[i].resize(f1->GetNumHits(F1chN[i]));
	//cout << "--> ch=" << chN[i] << " - numHits=" << vetroc->GetNumHits(chN[i]) << endl;
	  for(Int_t j=0; j<f1->GetNumHits(F1chN[i]); j++) {
	    F1AllHitNp[i][j] = f1->GetData(F1chN[i],j);
	  }
	}
}

// MARCO - CAEN
/*
if(caen!=0) {
	for(Int_t i=0; i<NCHAN_CAEN; i++) {
	    //CnHits[i] = caen->GetNumHits(i);
		CFirstHit[i] = caen->GetData(i,0);
	}
	
	for(Int_t i=0; i<Cnchs; i++) {
	  CAllHitNp[i].clear();
	  CAllHitNp[i].resize(2);
	  //CAllHitNp[i].resize(caen->GetNumHits(CchN[i]));
	  for(Int_t j=0; j<2; j++) {
	    CAllHitNp[i][j] = caen->GetData(CchN[i],j);
	  }
	}
}
*/

  return 1;
}


THaAnalysisObject::EStatus TdcDataEvtHandler::Init(const TDatime& date)
{

#ifdef WITH_DEBUG
  if( fDebug>1 )
    cout << "Hello !! We are initializing TdcDataEvtHandler !!   name =   "
	 <<fName<<endl;
#endif

  for (Int_t i=1; i<14; i++) eventtypes.push_back(i);  // what events to look for

  // WHICH CHANNEL TO EXPORT ALL HITS?
  Int_t chAnalyze[] = {126}; // VETROC
  Int_t F1chAnalyze[] = {33,14};//{34}; // F1
  //Int_t CchAnalyze[] = {0}; // CAEN

  // Number of channels selected
  // VETROC
  nchs = sizeof(chAnalyze)/sizeof(chAnalyze[0]);
  std::copy(&chAnalyze[0], &chAnalyze[nchs], back_inserter(chN));
  // F1
  F1nchs = sizeof(F1chAnalyze)/sizeof(F1chAnalyze[0]);
  std::copy(&F1chAnalyze[0], &F1chAnalyze[F1nchs], back_inserter(F1chN));
  // CAEN
  //Cnchs = sizeof(CchAnalyze)/sizeof(CchAnalyze[0]);
  //std::copy(&CchAnalyze[0], &CchAnalyze[Cnchs], back_inserter(CchN));

//cout << "-->chN:" << endl << " (size=" << nchs << ")" << endl;
//for(Int_t i=0; i<nchs; i++) cout << " [" << i << "]=" << chN[i] << endl;

  // data keys to look for in this fun example
  dataKeys.push_back("vfnhits");
  dataKeys.push_back("vfFirstHit");
  for(Int_t i=0; i<nchs; i++) dataKeys.push_back(Form("vfAllHits_%.3d",chN[i]));
  for(Int_t i=0; i<nchs; i++) dataKeys.push_back(Form("vfFine_%.3d",chN[i]));
  dataKeys.push_back("F1nhits");
  dataKeys.push_back("F1FirstHit");
  for(Int_t i=0; i<F1nchs; i++) dataKeys.push_back(Form("F1AllHits_%.3d",F1chN[i]));
  //dataKeys.push_back("Cnhit");
  //dataKeys.push_back("CFirstHit");
  //for(Int_t i=0; i<Cnchs; i++) dataKeys.push_back(Form("CAllHits_%.3d",CchN[i]));

  // initialize map elements to -1 (means not found yet)
  for (UInt_t i=0; i < dataKeys.size(); i++) {
    if(theDataMap.insert(make_pair(dataKeys[i],-1)).second == false) {
      Warning( Here("TdcDataEvtHandler::Init"), "Element %s already defined",
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
  // for nHits
  nHits.clear();
  nHits.resize(NTDCCHAN);
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "nHits", &nHits, kUIntV, 0);
  numEntries++;
  // for FirstHit
  FirstHit.clear();
  FirstHit.resize(NTDCCHAN);
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "FirstHit", &FirstHit, kIntV, 0);
  numEntries++;
  // for AllHitN
  AllHitNp.resize(NTDCCHAN);
  for(Int_t i=0; i<nchs; i++) {
    std::vector<UInt_t> tmp;
    AllHitNp.push_back(tmp);
	gHaVars->DefineByType(dataKeys[numEntries].c_str(), Form("Hits_ch%.3d",chN[i]), &AllHitNp[i], kUIntV, 0);
    numEntries++;
  }
  // for AllHitNFine
  AllHitNpFine.resize(NTDCCHAN);
  for(Int_t i=0; i<nchs; i++) {
    std::vector<UInt_t> tmp;
    AllHitNpFine.push_back(tmp);
  	gHaVars->DefineByType(dataKeys[numEntries].c_str(), Form("Fine_ch%.3d",chN[i]), &AllHitNpFine[i], kUIntV, 0);
    numEntries++;
  }


  // F1
  totalF1channels=NCHAN_F1;
  // for nHit
  F1nHits.clear();
  F1nHits.resize(NCHAN_F1);
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "F1nHits", &F1nHits, kUIntV, 0);
  numEntries++;
  // for FirstHit
  F1FirstHit.clear();
  F1FirstHit.resize(NCHAN_F1);
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "F1FirstHit", &F1FirstHit, kUIntV, 0);
  numEntries++;
  // for AllHitN
  F1AllHitNp.resize(NCHAN_F1);
  for(Int_t i=0; i<F1nchs; i++) {
    std::vector<UInt_t> tmp;
    F1AllHitNp.push_back(tmp);
	gHaVars->DefineByType(dataKeys[numEntries].c_str(), Form("F1Hits_ch%.3d",F1chN[i]), &F1AllHitNp[i], kUIntV, 0);
    numEntries++;
  }
  
  // CAEN
 /*
  // for nHit
  CnHits.clear();
  CnHits.resize(NCHAN_CAEN);
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "CnHits", &CnHits, kUIntV, 0);
  numEntries++;
  // for FirstHit
  CFirstHit.clear();
  CFirstHit.resize(NCHAN_CAEN);
  gHaVars->DefineByType(dataKeys[numEntries].c_str(), "CFirstHit", &CFirstHit, kUIntV, 0);
  numEntries++;
  // for AllHitN
  CAllHitNp.resize(NCHAN_CAEN);
  for(Int_t i=0; i<Cnchs; i++) {
    std::vector<UInt_t> tmp;
    CAllHitNp.push_back(tmp);
	gHaVars->DefineByType(dataKeys[numEntries].c_str(), Form("CHits_ch%.3d",CchN[i]), &CAllHitNp[i], kUIntV, 0);
    numEntries++;
  }
*/
  
  fStatus = kOK;
  return kOK;
}

ClassImp(TdcDataEvtHandler)
