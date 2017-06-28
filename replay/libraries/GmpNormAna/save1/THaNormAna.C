//*-- Author :  R. Michaels, Mar 2014
//
//////////////////////////////////////////////////////////////////////////
//
// GmpNormAna
//
// Code to summarize the information needed 
// to normalize an experiment and obtain a 
// cross section, including
//
// 1. Prescale factors
// 2. Calibration of charge monitor
// 3. Integrated Charge of a run
// 4. Livetime by trigger type, by helicity, and 
//    an approximate average livetime.
// 5. Number of triggers from bit pattern.
//
// This class requires scalers to be enabled
// in the analyzer.  For scalers, see 
// http://hallaweb.jlab.org/equipment/daq/THaScaler.html
//
// At present the run summary is just a printout,
// eventually one could write to a database.
// This needs some work (hopefully a student will do it).
// In addition, some global variables are
// registered for possible Tree analysis, e.g.
// cutting on helicity-correlated deadtime.
//
// How to use this class:
// 
// 1. Do something like this in the analysis script:
//      GmpNormAna* norm = 
//        new GmpNormAna("N","Normalization Analysis");
//      gHaPhysics->Add(norm);
// 
// 2. Add variables N.* to the output tree.
//
// 3. GmpNormAna will produce an end-of-run printout.
//
//
//////////////////////////////////////////////////////////////////////////

//#define WITH_DEBUG 1

#include "GmpNormAna.h"
#include "THaVarList.h"
#include "THaVar.h"
#include "THaGlobals.h"
#include "THaAnalyzer.h"
#include "THaScalerGroup.h"
#include "THaString.h"
#include "TList.h"
#include "THaEvData.h"
#include "TDatime.h"
#include "THaScaler.h"
#include "TH1.h"
#include "TH2.h"
#include "VarDef.h"
#include <fstream>
#include <iostream>

using namespace std;
using THaString::CmpNoCase;

//_____________________________________________________________________________
GmpNormAna::GmpNormAna( const char* name, const char* descript ) :
  THaPhysicsModule( name, descript )
{
  fSetPrescale = kFALSE;
  fHelEnable = kFALSE;
  myscaler = 0;
  normdata = new BNormData();
  // There are 12 triggers
  nhit = new Int_t[12];
  // There are 6 hits possible on a TDC channel
  tdcdata = new Int_t[6*12];
  alive = 0;
  hpos_alive = 0;
  hneg_alive = 0;
  norm_scaler = new Double_t[fgNumRoc * fgNumChan];   
  norm_plus = new Double_t[fgNumRoc * fgNumChan];   
  norm_minus = new Double_t[fgNumRoc * fgNumChan];   
  for (int i = 0; i < fgNumRoc*fgNumChan; i++) {
      norm_scaler[i] = 0; norm_plus[i] = 0; norm_minus[i] = 0;
  }
  cout << "HELLO !!!!!   GmpNormAna .... "<<endl;
  exit(0);
}

//_____________________________________________________________________________
GmpNormAna::~GmpNormAna()
{

  delete normdata;
  delete [] nhit;
  delete [] tdcdata;
  delete [] norm_scaler;
  delete [] norm_plus;
  delete [] norm_minus;
  for (vector<BRocScaler*>::iterator ir = fRocScaler.begin();
       ir != fRocScaler.end(); ir++) delete *ir;
  SetupRawData( NULL, kDelete ); 

}

//_____________________________________________________________________________
Int_t GmpNormAna::SetupRawData( const TDatime* /* run_time */, EMode mode )
{
  Int_t retval = 0;

  RVarDef vars[] = {
    { "evtypebits", "event type bit pattern",      "evtypebits" },  
    // This first part is based on event type 140 and is "older" code.
    { "bcm",   "bcm ",      "bcm" },
    { "alive",   "approx average livetime from scaler", "alive" },
    { "dlive",   "differential livetime from scaler ",  "dlive" },  
    { "hpos_alive", "helicity positive livetime", "hpos_alive" },  
    { "hneg_alive", "helicity negative livetime", "hneg_alive" },  


    { 0 }
  };

  if( mode != kDefine || !fIsSetup )
    retval = DefineVarsFromList( vars, mode );

  fIsSetup = ( mode == kDefine );
  
  return retval;
}

//_____________________________________________________________________________
Int_t GmpNormAna::End( THaRunBase* ) 
{

  PrintSummary();
  WriteHist();
  return 0;
}


//_____________________________________________________________________________
Int_t GmpNormAna::PrintSummary() const
{
// Print the summary.  Eventually want this to go to a
// database or a file.

  if (myscaler == 0) {
    cout << "Sorry, no scalers.  Therefore GmpNormAna ";
    cout << "failed to work."<<endl;
    return 0;
  }

  cout << "Trigger      Prescale      Num evt     Num in      Livetime"<<endl;
  cout << "---           Factor        CODA file   Scalers"<<endl;

  Int_t nhel=1;
  if (fHelEnable) nhel=3; 
  for (int ihel = 0; ihel < nhel; ihel++) {
    Int_t jhel = normdata->InvHel(ihel);  // jhel = 0, -1, +1
    if (fHelEnable) {
      if (jhel==0) {
        cout << "\nIrrespective of helicity "<<endl;
      } else {
        cout << "\nFor Helicity =  "<<jhel<<endl<<endl;
      }
    }
    for (int itrig = 0; itrig < 12; itrig++) {
// Again be careful about indices.  E.g. GetTrig starts at itrig=1.
      cout << "  "<<itrig+1<<"      "<<normdata->GetPrescale(itrig)<<"      "<<normdata->GetTrigCount(itrig,jhel)<<"       "<<myscaler->GetTrig(jhel,itrig+1)<<"       "<<normdata->GetLive(itrig,jhel)<<endl;

    }
    cout << "\nNumber of events = "<<normdata->GetEvCount(jhel)<<endl;
    cout << "Run-averaged livetime from scalers = ";
    cout << normdata->GetAvgLive(jhel)<<endl;
    cout << "Correction for non-synch of scalers = ";
    cout << normdata->GetCorrfact(jhel)<<endl;
  }

// This repeats what is in THaScaler::PrintSummary()
  printf("\n ------------ GmpNormAna Scaler Summary   ---------------- \n");
  Double_t clockrate = 1024;
  Double_t time_sec = myscaler->GetPulser("clock")/clockrate;
  printf("Time of run = %f sec \n",time_sec);

  return 1;
}

//_____________________________________________________________________________
  void GmpNormAna::WriteHist()
{
  for (UInt_t i = 0; i < hist.size(); i++) hist[i]->Write();
}

//_____________________________________________________________________________
  void GmpNormAna::BookHist()
{
// TDCs used for trigger latch pattern.  Used to tune cuts
// that define "simultaneity" of triggers.  Optionally fill
// histograms for BCM calibration (scalers versus EPICS).
  
  Int_t nbin = 100;
  Float_t xlo = 0;
  Float_t xhi = 4000;
  char cname[50],ctitle[50];
  for (int i = 0; i < 12; i++) {
    sprintf(cname,"Tlatch%d",i+1);
    sprintf(ctitle,"Trigger Latch TDC %d",i+1);
    hist.push_back(new TH1F(cname,ctitle,nbin,xlo,xhi));
  }

}


//_____________________________________________________________________________
THaAnalysisObject::EStatus GmpNormAna::Init( const TDatime& run_time ) 
{

  fStatus = kNotinit;
  MakePrefix();

  cout << "GmpNormAna:: Init !   the  name =  "<<fName<<endl<<flush;

  // Grab the scalers.
  // Of course these scalers are the event type 140 scaler data 
  THaAnalyzer* theAnalyzer = THaAnalyzer::GetInstance();
  TList* scalerList = theAnalyzer->GetScalers();
  TIter next(scalerList);
  while( THaScalerGroup* tscalgrp = static_cast<THaScalerGroup*>( next() )) {
    THaScaler *scaler = tscalgrp->GetScalerObj();
    string mybank("Left");
    if (CmpNoCase(mybank,scaler->GetName()) == 0) {
         myscaler = scaler;   // event type 140 data
         break;
    }
  }

  if (myscaler) {
     cout << "NormAna:  Found the scalers"<<endl;
  } else {
     cout << "NormAna:  Did NOT find the scalers"<<endl;
  }

  return fStatus = static_cast<EStatus>( SetupRawData( &run_time ) );


}



//_____________________________________________________________________________
Int_t GmpNormAna::Process(const THaEvData& evdata)
{
// Process the data:  Get prescale factors, get trigger
// latch pattern.  Process scalers.  All sorted by helicity
// if helicity was enabled.

  Int_t i, j;

  //  cout << "GmpNormAna:  process the data ..."<<endl<<flush;

// helicity is -1,0,1.  here, 0 == irrespective of helicity.
// But if helicity disabled its always 0.
//  fHelEnable = evdata.HelicityEnabled();
//  Int_t helicity = evdata.GetHelicity();
  fHelEnable = kFALSE;
  Int_t helicity = 0;

  if (evdata.IsPhysicsTrigger()) normdata->EvCount(helicity);

// If you have ~1000 events you probably have seen 
// prescale event; ok, this might not work for filesplitting
// if prescale event only goes into first file (sigh).
  
  if (!fSetPrescale && evdata.GetEvNum() > 1000) {
    fSetPrescale = kTRUE;
// CAREFUL, must GetPrescaleFactor(J) where J = 1,2,3 .. 
// (index starts at 1).  normdata starts at 0.
//    cout << "Prescale factors.  Origin = "<<evdata.GetOrigPS()<<endl;
    for (i = 0; i < 8; i++) {
         cout << "  trig= "<<i+1<<"   ps= "<<evdata.GetPrescaleFactor(i+1)<<endl;
         normdata->SetPrescale(i,evdata.GetPrescaleFactor(i+1));
    }
    for (i = 8; i < 12; i++) normdata->SetPrescale(i,1);
  }

#ifdef TRY1
  // Example to process the scalers if event type 140
  // came here, which it does not.  See also the TRY1
  // comments elsewhere.
  //  if (evdata.GetEvType() == 140) {
  //     if (myscaler) myscaler->LoadData(evdata);
  //  }
#endif


// TDC for trigger latch pattern.  The crate,slot,startchan 
// might change with experiments (I hope not).

//  int crate = 3;
//  int slot = 5;
//  int startchan = 64;
  int crate = 4;
  int slot = 13;
  int startchan = 0;

  int ldebug = 0;

  for (i = 0; i < 12; i++) {
    for (j = 0; j < 6; j++) tdcdata[j*12+i] = 0;
    nhit[i] = evdata.GetNumHits(crate, slot, startchan+i);
    if (ldebug) cout << "Latch for trig "<<i+1<<"   Nhit "<<nhit[i]<<endl;
    if (nhit[i] > 6) nhit[i] = 6;  
    for (j = 0; j < nhit[i]; j++) {
      Int_t tdat = evdata.GetData(crate,slot,startchan+i,j);
      if (ldebug) cout << "Raw TDC "<<tdat<<endl;
      tdcdata[j*12+i] = tdat;
      hist[i]->Fill(tdat);
    }        
  }
     
  TrigBits(helicity);

  LiveTime();

  if (fDEBUG) Print();


  return 0;
}


//_____________________________________________________________________________
void GmpNormAna::Print( Option_t* ) const
{
  // Print for purpose of debugging.

   normdata->Print();

   PrintSummary();
}




void GmpNormAna::TrigBits( Int_t helicity ) {
// Figure out which triggers got a hit.  These are 
// multihit TDCs, so we need to sort out which hit 
// we want to take by applying cuts.

  return;    // we'll need this eventually but its turned off now.

//  CODE turned off for now
#ifdef WHEN_READY

  int itrig,jhit,localtrig[12];

// These cuts can be decided by looking at the histograms.
// They importantly define "simultaneous" triggers, and 
// may therefore introduce a (small) systematic error

  static const Int_t cutlo = 100;
  static const Int_t cuthi = 1300;

  evtypebits = 0;
  memset(localtrig,0,12*sizeof(int));
  
  for (itrig = 0; itrig < 12; itrig++) {
    if (fDEBUG) {
           cout << "TDC for trig "<<itrig;
           cout << "   nhit = "<<nhit[itrig]<<endl;
    }
    for (jhit = 0; jhit < nhit[itrig]; jhit++) {
      Int_t tdat = tdcdata[12*jhit + itrig];
// Count at most once per event (don't double count)
// That's the purpose of localtrig
      if (fDEBUG) {
        cout << "    TDC dat for hit ";
        cout << jhit<<"   "<<tdat<<endl;
      }
      if (tdat > cutlo && tdat < cuthi && localtrig[itrig]==0) {
        localtrig[itrig] = 1;
	normdata->TrigCount(itrig,helicity);
        evtypebits |= (2<<itrig);
        if (fDEBUG) {
          cout << "        trigger bit "<< itrig;
          cout << "   helicity "<< helicity << "  count = ";
          cout << normdata->GetTrigCount(itrig,helicity)<<endl;
	}
      }
    }
  }

  if (fDEBUG) {
     cout << "  trig bit pattern "<<evtypebits<<endl;
  }

#endif 
//  CODE turned off for now

}


//_____________________________________________________________________________

void GmpNormAna::LiveTime() {

// Compute the livetime by trigger type, by
// helicity, and averaged from scalers-only-info.

   if (myscaler == 0) return;

  Int_t itrig, ihel, nhel, ok1;
  Double_t t5corr, t7corr, tcorr;
  Double_t totaltrig, livetime, avglive;
  Double_t numtrig, tsaccept, corrfact;  
  Double_t trate, ttrigrate;
  Double_t t5rate_corr;

  ok1=0;
  nhel = 1;  
  if (fHelEnable) nhel = 3;

  for (ihel = 0; ihel < nhel; ihel++) { 
    Int_t helicity = 0;
    if (ihel==1) helicity = -1;
    if (ihel==2) helicity = 1;

// CAREFUL ABOUT INDEX conventions
// THaScaler::GetTrig starts at itrig=1
// normdata indices all start at 0.

    t5corr = 0;
    t5rate_corr = 0;
    if (normdata->GetPrescale(4) != 0) {
      t5corr = 
          (Double_t)myscaler->GetTrig(helicity,5)/
                      normdata->GetPrescale(4);
      t5rate_corr = 
          (Double_t)myscaler->GetTrigRate(helicity,5)/
                      normdata->GetPrescale(4);
    }
    t7corr = 0;
    if (normdata->GetPrescale(6) != 0) {
      t7corr = 
          (Double_t)myscaler->GetTrig(helicity,7)/
                      normdata->GetPrescale(6);
    }

    tsaccept = 0;
    totaltrig = 0;
    corrfact = 0;
    ttrigrate = 0;
    ok1 = 0;

    for (itrig = 0; itrig < 12; itrig++) {

      numtrig = (Double_t)myscaler->GetTrig(helicity,itrig+1);
      trate = myscaler->GetTrigRate(helicity,itrig+1);
      //      cout << "trate "<<trate<<endl;

// Livetime by trigger type and by helicity:
// These are exact and physically meaningful, in
// contrast to "average livetime" below.

      if (normdata->GetPrescale(itrig) > 0 && numtrig > 0) {
        livetime = normdata->GetTrigCount(itrig,helicity)*
             normdata->GetPrescale(itrig)/numtrig;
        tsaccept = 
          (Double_t)myscaler->GetNormData(helicity,"TS-accept");

// Correction for the non-synch of scalers and events.
// If correction too far from 1 --> failure to estimate.
// At a normal end-of-run, corrfact should be 1.0

        if (tsaccept > 0) corrfact = 
   	   normdata->GetEvCount(helicity)/tsaccept;
        if (corrfact > 0.5 && corrfact < 2.0) {
          ok1=1;
          livetime = livetime/corrfact;
        } else {
          livetime = -1;  // failure
	}
        normdata->SetLive(itrig, livetime, helicity); 

// Average livetime using only scaler information
// Assumptions, valid for e94107, are:
// Every T5 is a T1 and T3.  Every T7 is a T5 and T6.
// There are other correlations which are ignored,
// hence this is only approximate.

        tcorr = 0; 
        if (itrig == 0 || itrig == 2) tcorr = t5corr;
        if (itrig == 4 || itrig == 5) tcorr = t7corr;
        totaltrig = totaltrig + 
           (numtrig-tcorr)/normdata->GetPrescale(itrig);
      }

      avglive = 0;
      if (totaltrig > 0) avglive = tsaccept/totaltrig;
      normdata->SetAvgLive(avglive,corrfact,helicity);

// Differential DT for helicity==0.  Ignores the "corrfact"
      if (helicity==0 && 
        normdata->GetPrescale(itrig) > 0 && trate > 0) {
        tcorr = 0; 
        if (itrig == 0 || itrig == 2) tcorr = t5rate_corr;
        ttrigrate = ttrigrate +
 	   (trate-tcorr)/normdata->GetPrescale(itrig);
	//        cout << "ttrigrate "<<itrig<<"  "<<trate<<"  "<<tcorr<<"   "<<ttrigrate<<endl;
      }


    }

    if (helicity == 0) {   
     dlive = -1;
     if (ttrigrate > 0) dlive = myscaler->GetNormRate(0,"TS-accept")/ttrigrate;
     //     cout << "TS-accept rate  "<<myscaler->GetNormRate(0,"TS-accept")<<"   dlive "<<dlive<<endl;
    }

  }

  alive = normdata->GetAvgLive();
  hpos_alive = normdata->GetAvgLive(1);
  hneg_alive = normdata->GetAvgLive(-1);

}


//_____________________________________________________________________________
ClassImp(GmpNormAna)

