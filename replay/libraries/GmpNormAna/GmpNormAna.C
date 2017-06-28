//*-- Author :  R. Michaels, Apr 2014
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

#include "GmpNormAna.h"
#include "THaVarList.h"
#include "THaVar.h"
#include "THaGlobals.h"
#include "THaAnalyzer.h"
#include "THaScalerGroup.h"
//#include "THaString.h"
#include "TList.h"
#include "THaEvData.h"
#include "TDatime.h"
#include "THaScaler.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "VarDef.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
//using THaString::CmpNoCase;

//_____________________________________________________________________________
GmpNormAna::GmpNormAna( const char* name, const char* descript ) :
  THaPhysicsModule( name, descript )
{
  fSetPrescale = kFALSE;
  fHelEnable = kFALSE;
  myscaler = 0;
  fSpectr = descript;
  normdata = new BNormData();
  // There are NTRIG triggers
  nhit = new Int_t[NTRIG];
  // There are MAXHIT  hits possible on a TDC channel
  tdcdata = new Int_t[MAXHIT*NTRIG];
  t1live = 0;
  t2live = 0;
  t3live = 0;
  t4live = 0;
  t5live = 0;
  t6live = 0;
  t7live = 0;
  t8live = 0;
  TSLive1 = 0;
  TSLive2 = 0;
  TSdtime = 1;
  //isrenewed = 0;
  herecnt=0;
  crate = slot = startchan = -1;
  //ev140_scaler = new Double_t[fgNumRoc * fgNumChan];   
  //roc_scaler = new Double_t[fgNumRoc * fgNumChan];   
  if (fDebug) {
       outputFile = new ofstream("normana.txt");
  } 
 //for (int i = 0; i < fgNumRoc*fgNumChan; i++) {
 //     ev140_scaler[i] = 0;
 //     roc_scaler[i] = 0;
 // }
  //BookHist();

  bcm_u1=0; bcm_d1=0; bcm_d3=0; bcm_d10=0; bcm_unew=0; bcm_dnew=0;
  charge_u1=0; charge_d1=0; charge_d3=0; charge_d10=0; charge_unew=0; charge_dnew=0;

}

//_____________________________________________________________________________
GmpNormAna::~GmpNormAna()
{

  //WriteHist();

  //RemoveVariables();

  delete normdata;
  delete [] nhit;
  delete [] tdcdata;
  //delete [] ev140_scaler;
  //delete [] roc_scaler;
  //for (vector<BRocScaler*>::iterator ir = fRocScaler.begin();
  //     ir != fRocScaler.end(); ir++) delete *ir;
  //SetupRawData( NULL, kDelete ); 
  DefineVariables( kDelete ); 


}

//_____________________________________________________________________________
//Int_t GmpNormAna::SetupRawData( const TDatime* /* run_time */, EMode mode )
Int_t GmpNormAna::DefineVariables( EMode mode )
{
  
  if ( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

//  Int_t retval = 0;

  RVarDef vars[] = {
    { "evtypebits", "event type bit pattern",      "evtypebits" },  
    // This first part is based on event type 140 and is "older" code.
    //{ "bcm",   "bcm ",      "bcm" },
    { "t1live",    "t1 live time",                           "t1live" },
    { "t2live",    "t2 live time",                           "t2live" },
    { "t3live",    "t3 live time",                           "t3live" },
    { "t4live",    "t4 live time",                           "t4live" },
    { "t5live",    "t5 live time",                           "t5live" },
    { "t6live",    "t6 live time",                           "t6live" },
    { "t7live",    "t7 live time",                           "t7live" },
    { "t8live",    "t8 live time",                           "t8live" },
    { "TSdtime",   "Trigger Supervisor scalers deadtime",    "TSdtime" },
    //{ "dlive",   "differential livetime from scaler ",  "dlive" },  
    { "current_u1",    "Beam current from bcm u1",           "bcm_u1" },  
    { "current_u3",    "Beam current from bcm u3",           "bcm_u3" },  
    { "current_u10",   "Beam current from bcm u10",          "bcm_u10" },  
    { "current_d1",    "Beam current from bcm d1",           "bcm_d1" },  
    { "current_d3",    "Beam current from bcm d3",           "bcm_d3" },  
    { "current_d10",   "Beam current from bcm d10",          "bcm_d10" },  
    { "current_unew",  "Beam current from bcm unew",         "bcm_unew" },  
    { "current_dnew",  "Beam current from bcm dnew",         "bcm_dnew" },  
    { "charge_u1",     "Integrated charge from bcm u1",      "charge_u1" },  
    { "charge_u3",     "Integrated charge from bcm u3",      "charge_u3" },  
    { "charge_u10",    "Integrated charge from bcm u10",     "charge_u10" },  
    { "charge_d1",     "Integrated charge from bcm d1",      "charge_d1" },  
    { "charge_d3",     "Integrated charge from bcm d3",      "charge_d3" },  
    { "charge_d10",    "Integrated charge from bcm d10",     "charge_d10" },  
    { "charge_unew",   "Integrated charge from bcm unew",    "charge_unew" },  
    { "charge_dnew",   "Integrated charge from bcm dnew",    "charge_dnew" },  
    //{ "isrenewed",     "Scaler reading updated or not",      "isrenewed"},
    { "isrenewed",     "Scaler reading updated or not",      "myscaler.IsRenewed()"},
    { 0 }
  };


#ifdef WITH_DEBUG
  if (mode == kDefine)  cout << "setting up RVarDef in Normana "<<endl;
#endif

  
  return DefineVarsFromList( vars, mode );
}

//_____________________________________________________________________________
Int_t GmpNormAna::End( THaRunBase* ) 
{

  PrintSummary();
  WriteHist();
  if (fDebug) outputFile->close();
  return 0;
}


//_____________________________________________________________________________
Int_t GmpNormAna::PrintSummary() const
{
// Print the summary.  Eventually want this to go to a
// database or a file.

  cout << endl << "Summary of Normalization Analysis "<<endl;

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
    for (int itrig = 0; itrig < 8; itrig++) {
// Again be careful about indices.  E.g. GetTrig starts at itrig=1.
      cout << "  "<<itrig+1<<"      "<<normdata->GetPrescale(itrig)<<"      "<<normdata->GetTrigCount(itrig,jhel)<<"       "<<myscaler->GetTrig(jhel,itrig+1)<<"       "<<normdata->GetLive(itrig,jhel)<<endl;

    }
    cout << "\nNumber of events = "<<normdata->GetEvCount(jhel)<<endl;
//    cout << "Run-averaged livetime from scalers = ";
//    cout << normdata->GetAvgLive(jhel)<<endl;
//    cout << "Correction for non-synch of scalers = ";
//    cout << normdata->GetCorrfact(jhel)<<endl;
  }

// This repeats what is in THaScaler::PrintSummary()
  printf("\n ------------ GmpNormAna Scaler Summary   ---------------- \n");
  //Double_t clockrate = 1024;
  Double_t clockrate = myscaler->GetClockRate();
  Double_t time_sec = myscaler->GetPulser("clock")/clockrate;
  if (time_sec == 0) {
     cout << "THaScaler: WARNING:  Time of run = ZERO (\?\?)\n"<<endl;
  } else {
     Double_t time_min = time_sec/60.;
     //printf("Time of run = %f sec \n",time_sec);
     printf("Time of run = %7.2f min \n",time_min);
     //printf("Triggers:     1 = %d    2 = %d    3 = %d   4 = %d    5 = %d\n",
     //      myscaler->GetTrig(1),myscaler->GetTrig(2),myscaler->GetTrig(3),myscaler->GetTrig(4),myscaler->GetTrig(5));
     printf("Accepted triggers:   %d \n",myscaler->GetNormData(0,"TS-accept"));
//     printf("Accepted triggers by helicity:    (-) = %d    (+) = %d\n",
//           myscaler->GetNormData(-1,"TS-accept"),myscaler->GetNormData(1,"TS-accept"));
  }

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
// that define "simultaneity" of triggers.  
  
  Int_t nbin = 100;
  Float_t xlo = 0;
  Float_t xhi = 4000;
  char cname[50],ctitle[50];
  for (int i = 0; i < NTRIG; i++) {
    sprintf(cname,"Tlatch%d_%s",i+1,GetTitle());
    sprintf(ctitle,"Trigger Latch TDC %d",i+1);
    hist.push_back(new TH1F(cname,ctitle,nbin,xlo,xhi));
  }

}


//_____________________________________________________________________________
THaAnalysisObject::EStatus GmpNormAna::Init( const TDatime& run_time ) 
{

//  fStatus = kNotinit;
//  MakePrefix();
//  BookHist();

  //if (fDEBUG) *outputFile << "GmpNormAna:: Init !   the  name =  "<<fName<<"    "<<fSpectr<<endl<<flush;
  if (fDebug) *outputFile << "GmpNormAna:: Init !   the  name =  "<<fName<<"    "<<fSpectr<<endl<<flush;

  // Grab the scalers.
  // Of course these scalers are the event type 140 scaler data 
  THaAnalyzer* theAnalyzer = THaAnalyzer::GetInstance();
  TList* scalerList = theAnalyzer->GetScalers();
  TIter next(scalerList);
  while( THaScalerGroup* tscalgrp = static_cast<THaScalerGroup*>( next() )) {
    THaScaler *scaler = tscalgrp->GetScalerObj();
    //if (CmpNoCase(fSpectr,scaler->GetName()) == 0) {
    if (fSpectr.CompareTo(scaler->GetName(),TString::kIgnoreCase) == 0) {
         myscaler = scaler;   // event type 140 data // Can I use scaler data in the event stream here?
         break;
    }
  }

  if (myscaler) {
      if (fDebug) *outputFile << "GmpNormAna:  Found the scalers"<<endl;
  } else {
      if (fDebug) *outputFile << "GmpNormAna:  Did NOT find the scalers"<<endl;
  }

  if (fDebug) *outputFile << "fSpectr   "<<fSpectr<<endl;

//  ReadDatabase( run_time );
  THaPhysicsModule::Init( run_time );
  BookHist();

  return fStatus;


}



//_____________________________________________________________________________
Int_t GmpNormAna::Process(const THaEvData& evdata)
{
// Process the data:  Get prescale factors, get trigger
// latch pattern.  Process scalers.  All sorted by helicity
// if helicity was enabled.

  //isrenewed = 0;

  Int_t i, j;

// helicity is -1,0,1.  here, 0 == irrespective of helicity.
// But if helicity disabled its always 0.

//  fHelEnable = evdata.HelicityEnabled();
//  Int_t helicity = evdata.GetHelicity();
  fHelEnable = kFALSE;
  Int_t helicity = 0;

  if (evdata.IsPhysicsTrigger()) {
      normdata->EvCount(helicity);
      //itrig = evdata.GetEvType();
      //normdata->TrigCount(itrig-1,helicity);  // Want to replace this with trigger latch -- later
  }

// If you have ~1000 events you probably have seen 
// prescale event; ok, this might not work for filesplitting
// if prescale event only goes into first file (sigh).

// In Gmp data, prescale event is right after prestart event,
// i.e., before any physics event  
//  if (!fSetPrescale && evdata.GetEvNum() > 1000) {
  if (!fSetPrescale) {

// CAREFUL, must GetPrescaleFactor(J) where J = 1,2,3 .. 
// (index starts at 1).  normdata starts at 0.
    if (fDebug) *outputFile << "Prescale factors.  Origin = "<<evdata.GetOrigPS()<<endl;
    for (i = 0; i < 8; i++) {
         if (fDebug) *outputFile << "     trig= "<<i+1<<"   ps= "<<evdata.GetPrescaleFactor(i+1)<<endl;
         normdata->SetPrescale(i,evdata.GetPrescaleFactor(i+1));
    }
    for (i = 8; i < NTRIG; i++) normdata->SetPrescale(i,1);
    fSetPrescale = kTRUE;

  }

// TDC for trigger latch pattern.  The crate,slot,startchan 
// might change with experiments (I hope not).
//  It's either in roc5 (L-HRS) or roc6 (R-HRS)

   int index;


   for (i=0; i < NTRIG; i++) {
       for (j=0; j < MAXHIT; j++) {
           tdcdata[j*NTRIG+i] = 0;
       }
   }

   //   for (i = 0; i < 8; i++) {   // only 8 pre-scalable triggers 
   for (i = 0; i < NTRIG; i++) {

       nhit[i] = evdata.GetNumHits(crate, slot, startchan+i);
       if (nhit[i] > MAXHIT) nhit[i] = MAXHIT;

       for (j = 0; j < nhit[i]; j++) {
           Int_t tdat = evdata.GetData(crate,slot,startchan+i,j);
           index = j*NTRIG+i;
           //if (fDEBUG) *outputFile << "Indices "<<i<<"   "<<j<<"   "<<i+1<<"   "<<index<<"      Raw TDC "<<tdat<<endl;
           if (fDebug) *outputFile << "Indices "<<i<<"   "<<j<<"   "<<i+1<<"   "<<index<<"      Raw TDC "<<tdat<<endl;
           if (index >= 0 && index < MAXHIT*NTRIG)  tdcdata[index] = tdat;
           hist[i]->Fill(tdat);
       }

   }


   TrigBits(helicity);

   if (myscaler) {
       if (myscaler->IsRenewed() && fSetPrescale)  LiveTime();  
       if (myscaler->IsRenewed()) {
           //isrenewed = 1;
           CalcCharge();
       }
   }

  if ( evdata.GetRocLength(scaroc)>32) {
     for (i = 0; i < evdata.GetRocLength(scaroc); i++) {
       Int_t sdat = evdata.GetRawData(scaroc,i);
       //if(fDebug) *outputFile << "scaroc  on arm "<<fSpectr<<"   "<<i<<"   "<<sdat<<"   = 0x"<<hex<<sdat<<dec<<endl;
       //cout << "scaroc  on arm "<<fSpectr<<"   "<<i<<"   "<<sdat<<"   = 0x"<<hex<<sdat<<dec<<endl;
       //if (sdat==header1) {
       //  for (j=0; j<fgNumChan; j++) {
       //         roc_scaler[j]=evdata.GetRawData(scaroc,i+j+1);
       //         if (fDebug) *outputFile << "Roc_scaler "<<j<<"   = 0x"<<hex<<roc_scaler[j]<<dec<<endl;
       //  }
       //}
       if (sdat==(Int_t)header) {  // TS DT scalers
         TSLive1=evdata.GetRawData(scaroc,i+20);
         TSLive2=evdata.GetRawData(scaroc,i+21);
	 for (j=1; j<24; j++) {
	   if (fDebug) *outputFile << "TS DT scalers   "<<evdata.GetRawData(scaroc,i+j)<<endl;
	   //cout << "TS DT scalers   " << TSLive1 << " " << TSLive2 << endl;
	   }
         break;
       }
     }
  }

   //TSdtime=1.;
   if (TSLive1 != 0 && TSLive2 != 0) {
       Double_t x1 = 1.*TSLive1;
       Double_t x2 = 1.*TSLive2;
       TSdtime = 1.*(1.0- (x1/x2));
   }
   if (fDebug) *outputFile << "Trigger Supervisor deadtime = "<<TSLive1<<"   "<<TSLive2<<"  "<<TSdtime<<endl;
   //cout << "Trigger Supervisor deadtime = "<<TSLive1<<"   "<<TSLive2<<"  "<<TSdtime<<endl;

   if (fDebug) Print();
/*
  if ( evdata.GetEvType()==100) {
     cout << "**********Last scaler reading event found**********." << endl;
     for (i = 0; i < evdata.GetRocLength(scaroc); i++) {
       Int_t sdat = evdata.GetRawData(scaroc,i);
       //if(fDebug) *outputFile << "scaroc  on arm "<<fSpectr<<"   "<<i<<"   "<<sdat<<"   = 0x"<<hex<<sdat<<dec<<endl;
       //cout << "scaroc  on arm "<<fSpectr<<"   "<<i<<"   "<<sdat<<"   = 0x"<<hex<<sdat<<dec<<endl;
       if (sdat==header1) {
	 for (j=0; j<fgNumChan; j++) {
                roc_scaler[j]=evdata.GetRawData(scaroc,i+j+1);
                if (fDebug) *outputFile << "Roc_scaler "<<j<<"   = 0x"<<hex<<roc_scaler[j]<<dec<<endl;
                cout << "Roc_scaler "<<j<<"   = 0x"<<hex<<roc_scaler[j]<<dec<<endl;
	 }
       }
     }
  }
*/
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

  int itrig,jhit,localtrig[NTRIG];

// These cuts can be decided by looking at the histograms.
// They importantly define "simultaneous" triggers, and 
// may therefore introduce a (small) systematic error

  static const Int_t cutlo = 500;
  static const Int_t cuthi = 1700;

  evtypebits = 0;
  memset(localtrig,0,NTRIG*sizeof(int));
  
  for (itrig = 0; itrig < NTRIG; itrig++) {   
    //if (fDEBUG && nhit[itrig] > 0) {
    if (fDebug && nhit[itrig] > 0) {
           *outputFile << "TDC for trig "<<itrig+1;
           *outputFile << "   nhit = "<<nhit[itrig]<<endl;
    }
    for (jhit = 0; jhit < nhit[itrig]; jhit++) {
      Int_t tdat = tdcdata[NTRIG*jhit + itrig];
// Count at most once per event (don't double count)
// That's the purpose of localtrig
      //if (fDEBUG) {
      if (fDebug) {
          *outputFile << "    TDC dat for hit ";
          *outputFile << jhit<<"   "<<"  index  "<<NTRIG*jhit + itrig<<"     data = "<<tdat<<endl;
      }
      if (tdat > cutlo && tdat < cuthi && localtrig[itrig]==0) {
          localtrig[itrig] = 1;
    	  normdata->TrigCount(itrig,helicity);
          evtypebits |= (2<<itrig);  
          //if (fDEBUG) {
          if (fDebug) {
                *outputFile << "        trigger bit "<< itrig;
                *outputFile << "   helicity "<< helicity << "  count = ";
                *outputFile << normdata->GetTrigCount(itrig,helicity)<<endl;
	  }
      }
    }
  }

  //if (fDEBUG) {
  if (fDebug) {
           *outputFile << "  trig bit pattern "<<evtypebits<<endl;
  }

}


//_____________________________________________________________________________

void GmpNormAna::LiveTime() {

// Compute the livetime by trigger type and by helicity.

  //int fDEBUG=0;

   if (myscaler == 0) return;
/*
   Int_t isrenewed=0;

 // Check if scalers have changed.
   for (Int_t i=0; i<8; i++) {
     ev140_scaler[i] = myscaler->GetTrig(0,i+1);
     if (ev140_scaler[i] != ev140_scaler[i+8]) isrenewed=1;
   }
 
   if ( !isrenewed) return;  // probably no new scaler data

   herecnt++;

   //if (fDEBUG) *outputFile << "here count "<<herecnt<<endl;
   if (fDebug) *outputFile << "here count "<<herecnt<<endl;


   for (Int_t i=0; i<8; i++) {
     ev140_scaler[i+8] = ev140_scaler[i];
   }
*/

   Int_t itrig, ihel, nhel, numtrig;
   Double_t trate, livetime;


  nhel = 1;  
  if (fHelEnable) nhel = 3;

  for (ihel = 0; ihel < nhel; ihel++) { 
    Int_t helicity = 0;
    if (ihel==1) helicity = -1;
    if (ihel==2) helicity = 1;

// CAREFUL ABOUT INDEX conventions
// THaScaler::GetTrig starts at itrig=1
// normdata indices all start at 0.

    for (itrig = 0; itrig < NTRIG; itrig++) {

      numtrig = (Double_t)myscaler->GetTrig(helicity,itrig+1);
      trate = myscaler->GetTrigRate(helicity,itrig+1);
      //if (fDEBUG) *outputFile << "trig= "<<itrig+1<<"   numtrig "<<numtrig<<"    rate =  "<<trate<<endl;
      if (fDebug) *outputFile << "trig= "<<itrig+1<<"   numtrig "<<numtrig<<"    rate =  "<<trate<<endl;

// Livetime by trigger type and by helicity:

      if (normdata->GetPrescale(itrig) > 0 && numtrig > 0) {
           livetime = normdata->GetTrigCount(itrig,helicity)*
                normdata->GetPrescale(itrig)/numtrig;
            normdata->SetLive(itrig, livetime, helicity); 

            //if (fDEBUG) *outputFile << "Prescale "<<normdata->GetPrescale(itrig)<<"    "<<normdata->GetTrigCount(itrig,helicity)<<"    livetime =   "<<livetime<<endl;
            if (fDebug) *outputFile << "Prescale "<<normdata->GetPrescale(itrig)<<"    "<<normdata->GetTrigCount(itrig,helicity)<<"    livetime =   "<<livetime<<endl;



      }

    }

  }

   t1live = normdata->GetLive(0,0);
   t2live = normdata->GetLive(1,0);
   t3live = normdata->GetLive(2,0);
   t4live = normdata->GetLive(3,0);
   t5live = normdata->GetLive(4,0);
   t6live = normdata->GetLive(5,0);
   t7live = normdata->GetLive(6,0);
   t8live = normdata->GetLive(7,0);
   if (fDebug) {
       *outputFile  << "the  t1live = "<<t1live<<endl;
       *outputFile  << "the  t2live = "<<t2live<<endl;
       *outputFile  << "the  t3live = "<<t3live<<endl;
       *outputFile  << "the  t4live = "<<t4live<<endl;
       *outputFile  << "the  t5live = "<<t5live<<endl;
       *outputFile  << "the  t6live = "<<t6live<<endl;
       *outputFile  << "the  t7live = "<<t7live<<endl;
       *outputFile  << "the  t8live = "<<t8live<<endl;
   }

}

//_____________________________________________________________________________

void GmpNormAna::CalcCharge() {

   if (myscaler == 0) return;

   Double_t clockrate = myscaler->GetClockRate();
   Double_t time_sec = myscaler->GetPulser("clock")/clockrate;

   // Calculate beam current
   //bcm_u1 = myscaler->GetBcmRate("bcm_u1")*gain_u1 + off_u1;
   bcm_u1 = myscaler->GetScalerRate("bcm_u1",0)*gain_u1 + off_u1;
   //bcm_u3 = myscaler->GetBcmRate("bcm_u3")*gain_u3 + off_u3;
   bcm_u3 = myscaler->GetScalerRate("bcm_u3",0)*gain_u3 + off_u3;
   //bcm_u10 = myscaler->GetBcmRate("bcm_u10")*gain_u10 + off_u10;
   bcm_u10 = myscaler->GetScalerRate("bcm_u10",0)*gain_u10 + off_u10;
   //bcm_d1 = myscaler->GetBcmRate("bcm_d1")*gain_d1 + off_d1;
   bcm_d1 = myscaler->GetScalerRate("bcm_d1",0)*gain_d1 + off_d1;
   //bcm_d3 = myscaler->GetBcmRate("bcm_d3")*gain_d3 + off_d3;
   bcm_d3 = myscaler->GetScalerRate("bcm_d3",0)*gain_d3 + off_d3;
   //bcm_d10 = myscaler->GetBcmRate("bcm_d10")*gain_d10 + off_d10;
   bcm_d10 = myscaler->GetScalerRate("bcm_d10",0)*gain_d10 + off_d10;
   //bcm_unew = myscaler->GetBcmRate("bcm_unew")*gain_unew + off_unew;
   bcm_unew = myscaler->GetScalerRate("bcm_unew",0)*gain_unew + off_unew;
   //bcm_dnew = myscaler->GetBcmRate("bcm_dnew")*gain_dnew + off_dnew;
   bcm_dnew = myscaler->GetScalerRate("bcm_dnew",0)*gain_dnew + off_dnew;

   // Calculate beam charge
   //charge_u1 = myscaler->GetBcm("bcm_u1")*gain_u1 + off_u1*time_sec;
   charge_u1 = myscaler->GetScaler("bcm_u1",0)*gain_u1 + off_u1*time_sec;
   //charge_u3 = myscaler->GetBcm("bcm_u3")*gain_u3 + off_u3*time_sec;
   charge_u3 = myscaler->GetScaler("bcm_u3",0)*gain_u3 + off_u3*time_sec;
   //charge_u10 = myscaler->GetBcm("bcm_u10")*gain_u10 + off_u10*time_sec;
   charge_u10 = myscaler->GetScaler("bcm_u10",0)*gain_u10 + off_u10*time_sec;
   //charge_d1 = myscaler->GetBcm("bcm_d1")*gain_d1 + off_d1*time_sec;
   charge_d1 = myscaler->GetScaler("bcm_d1",0)*gain_d1 + off_d1*time_sec;
   //charge_d3 = myscaler->GetBcm("bcm_d3")*gain_d3 + off_d3*time_sec;
   charge_d3 = myscaler->GetScaler("bcm_d3",0)*gain_d3 + off_d3*time_sec;
   //charge_d10 = myscaler->GetBcm("bcm_d10")*gain_d10 + off_d10*time_sec;
   charge_d10 = myscaler->GetScaler("bcm_d10",0)*gain_d10 + off_d10*time_sec;
   //charge_unew = myscaler->GetBcm("bcm_unew")*gain_unew + off_unew*time_sec;
   charge_unew = myscaler->GetScaler("bcm_unew",0)*gain_unew + off_unew*time_sec;
   //charge_dnew = myscaler->GetBcm("bcm_dnew")*gain_dnew + off_dnew*time_sec;
   charge_dnew = myscaler->GetScaler("bcm_dnew",0)*gain_dnew + off_dnew*time_sec;

}

//_____________________________________________________________________________

void GmpNormAna::MakePrefix() {

    TString prefix = GetTitle();
    prefix.ToLower();
    prefix.Prepend(".");
    prefix.Prepend(GetName());
    prefix += ".";

    delete [] fPrefix;
    fPrefix = new char[ prefix.Length()+1 ];
    strcpy( fPrefix, prefix.Data() );
}

//_____________________________________________________________________________

Int_t GmpNormAna::ReadDatabase( const TDatime& date) {

    static const char* const here = "ReadDatabase()";

    FILE* file = OpenFile( date );
    if (!file) return kFileError;

    std::string hexheader;

    const DBRequest req[] = {
        { "u1.gain",      &gain_u1,   kDouble, 0 },
        { "u1.offset",    &off_u1,    kDouble, 0 },
        { "u3.gain",      &gain_u3,   kDouble, 0 },
        { "u3.offset",    &off_u3,    kDouble, 0 },
        { "u10.gain",     &gain_u10,  kDouble, 0 },
        { "u10.offset",   &off_u10,   kDouble, 0 },
        { "d1.gain",      &gain_d1,   kDouble, 0 },
        { "d1.offset",    &off_d1,    kDouble, 0 },
        { "d3.gain",      &gain_d3,   kDouble, 0 },
        { "d3.offset",    &off_d3,    kDouble, 0 },
        { "d10.gain",     &gain_d10,  kDouble, 0 },
        { "d10.offset",   &off_d10,   kDouble, 0 },
        { "unew.gain",    &gain_unew, kDouble, 0 },
        { "unew.offset",  &off_unew,  kDouble, 0 },
        { "dnew.gain",    &gain_dnew, kDouble, 0 },
        { "dnew.offset",  &off_dnew,  kDouble, 0 },
        { "crate",        &crate,     kUInt,   0 },
        { "slot",         &slot,      kUInt,   0 },
        { "startchan",    &startchan, kUInt,   0 },
        { "roc",          &scaroc,    kUInt,   0 },
        { "header",       &hexheader, kString, 0 },
        { 0 }
    };

    TString scaler = myscaler->GetName();
    const char* prefix = 0;
    if (scaler.Contains("Left",TString::kIgnoreCase)) {
        prefix = "Left.";
    }
    if (scaler.Contains("Right",TString::kIgnoreCase)) {
        prefix = "Right.";
    }

    Int_t err = LoadDB( file, date, req, prefix);

    fclose(file);

    if (err) {
        Error( Here(here), "Error reading relevant data from db_%sdat. Fix database.", GetDBFileName() );
        return kInitError;
    }

    std::istringstream buffer(hexheader);
    buffer >> hex >> header;

    if (fDebug) {
        *outputFile << "GmpNormAna::ReadDatabase: "<< endl; 
        *outputFile << "u1 gain: " << gain_u1 << ", u1 offset: " << off_u1 << endl; 
        *outputFile << "d1 gain: " << gain_d1 << ", d1 offset: " << off_d1 << endl; 
        *outputFile << "d3 gain: " << gain_d3 << ", d3 offset: " << off_d3 << endl; 
        *outputFile << "d10 gain: " << gain_d10 << ", d10 offset: " << off_d10 << endl; 
        *outputFile << "unew gain: " << gain_unew << ", unew offset: " << off_unew << endl; 
        *outputFile << "dnew gain: " << gain_dnew << ", dnew offset: " << off_dnew << endl; 
        *outputFile << "crate: " << crate << ", slot: " << slot << ", start channel: "
                    << startchan << endl; 
        *outputFile << "roc: " << scaroc << ", header: 0x" << hex << header << dec << endl; 
    }

    cout << "GmpNormAna::ReadDatabase: "<< endl; 
    cout << "u1 gain: " << gain_u1 << ", u1 offset: " << off_u1 << endl; 
    cout << "d1 gain: " << gain_d1 << ", d1 offset: " << off_d1 << endl; 
    cout << "d3 gain: " << gain_d3 << ", d3 offset: " << off_d3 << endl; 
    cout << "d10 gain: " << gain_d10 << ", d10 offset: " << off_d10 << endl; 
    cout << "unew gain: " << gain_unew << ", unew offset: " << off_unew << endl; 
    cout << "dnew gain: " << gain_dnew << ", dnew offset: " << off_dnew << endl; 
    cout << "crate: " << crate << ", slot: " << slot << ", start channel: "
         << startchan << endl; 
    cout << "roc: " << scaroc << ", header: 0x" << hex << header << dec << endl; 

    return kOK;
}

//_____________________________________________________________________________
ClassImp(GmpNormAna)

