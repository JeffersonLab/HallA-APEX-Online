///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaFpp                                                                    //
//                                                                           //
// Focal Plane Polarimeter.                                                  //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaFpp.h"
#include "THaTrack.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "THaFppHit.h"
#include "THaFppPlane.h"
//#include "THaDBFile.h"
//#include "THaDB.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TList.h"
#include "VarDef.h"
#include "VarType.h"
#include "TDatime.h"
#include "TROOT.h"
#include "THaString.h"
#include "TError.h"
#include <map>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
extern "C" {
  void trk4_(Int_t&,Int_t& , Int_t*, Double_t*, Int_t&, Double_t*,Double_t*,
             Double_t*,Double_t*,Int_t*);
  void trackit_(Int_t&, Double_t*, Double_t*, Double_t*, Int_t*, Double_t*,
              Int_t*, Double_t&, Double_t&,Double_t&, Double_t*);
  void aligncorr_(Double_t&,Double_t&,Double_t&,Double_t&,
              Double_t&,Double_t&,Double_t&,Double_t&,Double_t*,Double_t*);
  void fpplast_(Double_t*,Double_t*,Short_t*,Double_t*,Int_t&,Int_t&,Int_t&);
}

//_____________________________________________________________________________
THaFpp::THaFpp( const char* name, const char* description,
			   THaApparatus* apparatus ) :
THaNonTrackingDetector(name,description,apparatus)
{
	// Constructor
  //fDebug=3;
	// Create array of fpp planes
	fPlane[0]   = new THaFppPlane("v1", "Front v1 plane", this);
	fPlane[1]   = new THaFppPlane("v2", "Front v2 plane", this);
	fPlane[2]   = new THaFppPlane("v3", "Front v3 plane", this);
	fPlane[3]   = new THaFppPlane("u1", "Front u1 plane", this);
	fPlane[4]   = new THaFppPlane("u2", "Front u2 plane", this);
	fPlane[5]   = new THaFppPlane("u3", "Front u3 plane", this);
	fPlane[6]   = new THaFppPlane("v4", "Front v4 plane", this);
	fPlane[7]   = new THaFppPlane("v5", "Front v5 plane", this);
	fPlane[8]   = new THaFppPlane("v6", "Front v6 plane", this);
	fPlane[9]   = new THaFppPlane("u4", "Front u4 plane", this);
	fPlane[10]   = new THaFppPlane("u5", "Front u5 plane", this);
	fPlane[11]   = new THaFppPlane("u6", "Front u6 plane", this);
	fPlane[12]   = new THaFppPlane("u7", "Rear u1 plane", this);
	fPlane[13]   = new THaFppPlane("u8", "Rear u2 plane", this);
	fPlane[14]   = new THaFppPlane("v7", "Rear v1 plane", this);
	fPlane[15]   = new THaFppPlane("v8", "Rear v2 plane", this);
	fPlane[16]   = new THaFppPlane("x1", "Rear x1 plane", this);
	fPlane[17]   = new THaFppPlane("x2", "Rear x2 plane", this);
	fPlane[18]   = new THaFppPlane("u9", "Rear u3 plane", this);
	fPlane[19]   = new THaFppPlane("u10", "Rear u4 plane", this);
	fPlane[20]   = new THaFppPlane("u11", "Rear u5 plane", this);
	fPlane[21]   = new THaFppPlane("v9", "Rear v3 plane", this);
	fPlane[22]   = new THaFppPlane("v10", "Rear v4 plane", this);
	fPlane[23]   = new THaFppPlane("v11", "Rear v5 plane", this);


	if( !fPlane[0] || !fPlane[1] || !fPlane[2] || !fPlane[3] || !fPlane[4] || !fPlane[5] || 
		!fPlane[6] || !fPlane[7] || !fPlane[8] || !fPlane[9] || !fPlane[10] || !fPlane[11] ||
		!fPlane[12] || !fPlane[13] || !fPlane[14] || !fPlane[15] || !fPlane[16] || !fPlane[17] || 
		!fPlane[18] || !fPlane[19] || !fPlane[20] || !fPlane[21] || !fPlane[22] || !fPlane[23] ||
		fPlane[0]->IsZombie() || fPlane[1]->IsZombie() ||fPlane[2]->IsZombie() || fPlane[3]->IsZombie() ||
		fPlane[4]->IsZombie() || fPlane[5]->IsZombie() ||fPlane[6]->IsZombie() || fPlane[7]->IsZombie() ||
		fPlane[8]->IsZombie() || fPlane[9]->IsZombie() ||fPlane[10]->IsZombie() || fPlane[11]->IsZombie() ||
		fPlane[12]->IsZombie() || fPlane[13]->IsZombie() ||fPlane[14]->IsZombie() || fPlane[15]->IsZombie() ||
		fPlane[16]->IsZombie() || fPlane[17]->IsZombie() ||fPlane[18]->IsZombie() || fPlane[19]->IsZombie() ||
		fPlane[20]->IsZombie() || fPlane[21]->IsZombie() ||fPlane[22]->IsZombie() || fPlane[23]->IsZombie() ) {
			Error( Here("THaFpp()"), "Failed to create subdetectors." );
			MakeZombie();
	}
}

//_____________________________________________________________________________
THaAnalysisObject::EStatus THaFpp::Init( const TDatime& date )
{
	// Initialize Fpp. Calls standard Init(), then initializes subdetectors.

	if( IsZombie() || !fPlane[0] || !fPlane[1] || !fPlane[2] || !fPlane[3] || !fPlane[4] || !fPlane[5] || 
		!fPlane[6] || !fPlane[7] || !fPlane[8] || !fPlane[9] || !fPlane[10] || !fPlane[11] ||
		!fPlane[12] || !fPlane[13] || !fPlane[14] || !fPlane[15] || !fPlane[16] || !fPlane[17] || 
		!fPlane[18] || !fPlane[19] || !fPlane[20] || !fPlane[21] || !fPlane[22] || !fPlane[23] )
		return fStatus = kInitError;

	EStatus status;
	if( (status = THaNonTrackingDetector::Init( date )) ||
		(status = fPlane[0]->Init( date )) ||
		(status = fPlane[1]->Init( date )) ||
		(status = fPlane[2]->Init( date )) ||
		(status = fPlane[3]->Init( date )) ||
		(status = fPlane[4]->Init( date )) ||
		(status = fPlane[5]->Init( date )) ||
		(status = fPlane[6]->Init( date )) ||
		(status = fPlane[7]->Init( date )) ||
		(status = fPlane[8]->Init( date )) ||
		(status = fPlane[9]->Init( date )) ||
		(status = fPlane[10]->Init( date )) ||
		(status = fPlane[11]->Init( date )) ||
		(status = fPlane[12]->Init( date )) ||
		(status = fPlane[13]->Init( date )) ||
		(status = fPlane[14]->Init( date )) ||
		(status = fPlane[15]->Init( date )) ||
		(status = fPlane[16]->Init( date )) ||
		(status = fPlane[17]->Init( date )) ||
		(status = fPlane[18]->Init( date )) ||
		(status = fPlane[19]->Init( date )) ||
		(status = fPlane[20]->Init( date )) ||
		(status = fPlane[21]->Init( date )) ||
		(status = fPlane[22]->Init( date )) ||
		(status = fPlane[23]->Init( date )) )
		return fStatus = status;

	return fStatus = kOK;
}
//_____________________________________________________________________________
Int_t THaFpp::End( THaRunBase* run)
{
  // Default End() for an apparatus: 
  fpplast_(&fTHits[0][0],&fTSum[0][0],&fTHist[0][0][0],&fT0[0][0]
                 ,calib,calib_opt,fDebug);

  return 0;
}
//_____________________________________________________________________________
Int_t THaFpp::ReadDatabase( const TDatime& date )
{
	// Read Fpp database

	// Read this detector's parameters from the database file 'fi'.
	// This function is called by THaDetectorBase::Init() once at the
	// beginning of the analysis.
	// 'date' contains the date/time of the run being analyzed.

  //	THaDBFile *dbf = new THaDBFile("db_L.fpp.dat","detmap.config");
//	TDatime now; // defaults to today's date and time
	static const char* const here = "ReadDatabase()";

	FILE* file = OpenFile( date );
	if( !file ) return kFileError;

	//read geometry for the fpp
	Int_t status = kInitError;

	vector<Double_t>* fpplist= new vector<Double_t>;
	vector<Double_t>* carblist= new vector<Double_t>;

	vector<Double_t>& fpplist_v= *fpplist;
	vector<Double_t>& carblist_v= *carblist;

	vector<Double_t>* toff = new vector<Double_t>;
	vector<Double_t>* tadd = new vector<Double_t>;
	vector<Double_t>* m1front = new vector<Double_t>;
	vector<Double_t>* m1rear = new vector<Double_t>;
	vector<Double_t>* icut = new vector<Double_t>;
	vector<Double_t>* DrftDst = new vector<Double_t>;

	vector<Double_t>& toff_v = *toff;
	vector<Double_t>& tadd_v = *tadd;
	vector<Double_t>& m1front_v = *m1front;
	vector<Double_t>& m1rear_v = *m1rear;
	vector<Double_t>& icut_v = *icut;
	vector<Double_t>& DrftDst_v = *DrftDst;

	fpplist->reserve(9);
	carblist->reserve(7);
	toff->reserve(40*FPP_NELEM);
	tadd->reserve(40*FPP_NELEM);
	m1front->reserve(4*6);
	m1rear->reserve(4*6);
	icut->reserve(9*35*FPP_NELEM);
	DrftDst->reserve(7*FPP_NELEM);


	const DBRequest listOpts[] ={
	  { "alignment", &align, kInt, 0, 1},
	  { "align_opt",&align_opt, kInt, 0, 1},
	  { "calibration",&calib, kInt, 0, 1},
	  { "calib_option",&calib_opt, kInt, 0, 1},
	  { "ana_front" , &ana_front, kInt, 0, 1},
	  { "ana_rear" , &ana_rear, kInt, 0, 1},
	  { "demux_stats", &pulser_fpp, kInt, 0, 1},
	  { "ev_display", &evdisplay, kInt, 0, 1},
	  { "mkjtrack", &mkjtrack, kInt, 0, 1},
	  { "geometry", fpplist, kDoubleV ,9 },
	  { "carbgeometry", carblist, kDoubleV ,7 },
	  { "TOffset", toff, kDoubleV, 40*FPP_NELEM},
	  { "TAdd", tadd, kDoubleV, 40*FPP_NELEM},
	  { "FrontAlign", m1front, kDoubleV, 4*6,kDouble},
	  { "RearAlign", m1rear, kDoubleV, 4*6,kDouble},
	  { "DemuxCuts", icut, kDoubleV, 9*35*FPP_NELEM},
	  { "DrftDistParam", DrftDst, kDoubleV, 7*FPP_NELEM},
	  { 0 }
	};

	//	cout<<"defined request!"<<endl;
// 	const DBRequest allgeom[] ={
// 	  {"geometry", fpplist, kDoubleV, 9 },
// 	  {"carbgeometry", carblist, kDoubleV, 7 },
// 	  {0}
// 	};
	
	Int_t nelem1;

	Int_t err0= LoadDB( file, date, listOpts, GetPrefix() );
	fclose(file);
	//	cout<<GetPrefix()<<endl;

	//	cout<<align<<" "<<align_opt<<" "<<calib<<" "<<calib_opt<<" ana_front:  "<<ana_front <<endl;
	//	cout<<"geometry:"<<fpplist_v[0]<<" "<<fpplist_v[1]<<" "<<fpplist_v[8]<<endl;
	//	cout<<"carblist:"<<carblist_v[0]<<" "<<carblist_v[1]<<" "<<carblist_v[2]<<endl;
	//	cout<<"toff:"<<toff_v[0]<<" "<<toff_v[1]<<" "<<toff_v[959]<<endl;
	//	cout<<"tadd:"<<tadd_v[0]<<" "<<tadd_v[959]<<endl;
	//cout<<"drftdst:"<<DrftDst_v[0]<<" "<<DrftDst_v[7]<<endl;
	      	//cout<<"DemuxCuts:"<<icut_v[0]<<" "<<icut_v[2]<<" "<<icut_v[9]<<endl;
		//cout<<"FrontAlign: "<<m1front_v[0]<<" "<<m1front_v[23]<<endl;
		//cout<<"ReadAlign: "<<m1rear_v[0]<<" "<<m1rear_v[23]<<endl;
      	//Int_t err_geom = LoadDB(file, date, allgeom, GetPrefix() );
	
	if( !err0 ){
	  status= kOK;
	}
	
	if( status==kOK ){
	  if( fpplist->size()!=9 ){
	    Error( Here(here), "Error reading fpp options and geometry from db_L.fpp.dat." 
		   "Fix database");
	    status = kInitError;
	  } else{
	    
	    fX = fpplist_v[0];
	    fY = fpplist_v[1];
	    fZ = fpplist_v[2];
	    fAngle = fpplist_v[3];
	    fdX = fpplist_v[4];
	    fdY = fpplist_v[5];
	    fdZ = fpplist_v[6];
	    fZZ = fpplist_v[7];
	    fCarbonX = carblist_v[0];   
	    fCarbonY = carblist_v[1];
	    fCarbonZ = carblist_v[2];
	    fCarbondX = carblist_v[3]; 
	    fCarbondY = carblist_v[4];
	    fCarbondZ = carblist_v[5]; 
	    fCarbZZ = carblist_v[6];
	  }
	}
	nelem1 = static_cast<Int_t>(fpplist_v[8]);

	// Reinitialization only possible for same basic configuration 
	//cout<<"fIsInit:"<<fIsInit<<endl;

	if( fIsInit && nelem1 != FPP_NELEM ) {
		Error( Here(here), "Cannot re-initalize with different number of planes. "
			"(was: %d, date: %d). Detector not re-initialized.", FPP_NELEM, nelem1 );
		status= kInitError;
	}

	// calculate important quantities from geometry (& conversion to cm)
        if(!fIsInit){
	  fZcarbonmin = fCarbonZ*100. - fCarbondZ *100.; 
	  fZcarbonmax = fCarbonZ*100. + fCarbondZ *100.;
	  fZcarbon = (fZcarbonmin + fZcarbonmax) / 2.;
	  
	  for (Int_t m = 0; m < FPP_NELEM; m++)
	    {
	      for (Int_t n=0; n < 40; n++)
		{
		  fTHits[n][m] = 0.;
		  fTSum[n][m] = 0.;
		  fT0[n][m] = 0.;
		  fTOff[n][m] = 0.;
		  for(Int_t k=0; k < 80;k++){
		    fTHist[k][n][m]=0;
		  }
		}
	    }
	  Int_t get_calib = 0;
	  if(calib == 1) {
	    if(calib_opt == 1 || calib_opt == 2) {get_calib = 1;}
	  }
	  else {get_calib = 1;}
	  
	  // Chamber position and rotation offsets used in GEANT     
	  /* fXofff[0] = fXoff[0]*100.;
	     fYofff[0] = fYoff[0)*100.;
	     fThofff[0] = fThoff[0]*degrad;
	     fPhofff[0] = fPhoff[0]*degrad;
	     fPsofff[0] = fPsoff[0]*degrad;
	     fXofff[1] = fXoff[0]*100.;
	     fYofff[1] = fYoff[0]*100.;
	     fThofff[1] = fThoff[0]*degrad;
	     fPhofff[1] = fPhoff[0]*degrad;
	     fPsofff[1] = fPsoff[0]*degrad;
	     fXofff[2] = fXoff[6]*100.;
	     fYofff[2] = fYoff[6]*100.;
	     fThofff[2] = fThoff[6]*degrad;
	     fPhofff[2] = fPhoff[6]*degrad;
	     fPsofff[2] = fPsoff[6]*degrad;
	     fXofff[3] = fXoff[12]*100.;
	     fYofff[3] = fYoff[12]*100.;
	     fThofff[3] = fThoff[12]*degrad;
	     fPhofff[3] = fPhoff[12]*degrad;
	     fPsofff[3] = fPsoff[12]*degrad;
	     fXofff[4] = fXoff[18]*100.;
	     fYofff[4] = fYoff[18]*100.;
	     fThofff[4] = fThoff[18]*degrad;
	     fPhofff[4] = fPhoff[18]*degrad;
	     fPsofff[4] = fPsoff[18]*degrad; 

	// Define the inverse Euler rotation for (thofff,phiofff,psiofff) used in GEANT

	rotmat[0][0]=TMath::Cos(psofff[0])*TMath::Cos(thofff[0])+dsin(psofff[0])*TMath::Sin(thofff[0])*TMath::Sin(phofff[0]);
	rotmat[1][0]=-TMath::Cos(phofff[0])*TMath::Sin(thofff[0]);
	rotmat[2][0]=-TMath::Sin(psofff[0])*TMath::Cos(thofff[0])+TMath::Cos(psofff[0])*TMath::Sin(thofff[0])*TMath::Cos(phofff[0]);
	rotmat[0][1]=TMath::Cos(psofff[0])*TMath::Sin(thofff[0])-TMath::Sin(psofff[0])*TMath::Cos(thofff[0])*TMath::Sin(phofff[0]);
	rotmat[1][1]=TMath::Cos(phofff)*TMath::Cos(thofff);
	rotmat[2][1]=-TMath::Sin(psofff[0])*TMath::Sin(thofff[0])-TMath::Cos(psofff[0])*TMath::Cos(thofff[0])*TMath::Sin(phofff[0]);
	rotmat[0][2]=TMath::Sin(psofff[0])*TMath::Cos(phofff[0]);
	rotmat[1][2]=TMath::Sin(phofff[0]);
	rotmat[2][2]=TMath::Cos(psofff[0])*TMath::Cos(phofff[0]);*/

	//Apply corrections to crude calibration parameters

	  Int_t t = 0;
	  if(get_calib == 1){
	    for (Int_t m = 0; m < FPP_NELEM; m++){
	      for (Int_t n = 0; n < 40; n++){
		fTOff[n][m] = toff_v[t];
		//cout<<"toff_v:"<<toff_v[t]<<endl;
		//cout<<"fTOff:"<<fTOff[n][m]<<endl;
		//if ( m==1 ) printf(" m = %d  n = %d off  = %f \n",m,n,fTOff[n][m]);
		t++;
	      }
	    }
	    if(calib != 1)
	      {
		t = 0;
		for (Int_t m = 0; m < FPP_NELEM; m++){
		  for (Int_t n = 0; n < 40; n++){
		    fTAdd[n][m] = tadd_v[t];
		    fTOff[n][m] = fTOff[n][m] - fTAdd[n][m];
		    if(fTOff[n][m] == 0.) {fTOff[n][m] = 775.0;}
		    t++;
		  }
		}
	      }
	  }
	  for (Int_t m = 0; m < 4; m++){
	    for (Int_t n = 0; n < 6; n++){
	      fM1Front[m][n] = 0.;
	      fM1Rear[m][n] = 0.;
	    }
	  }
	  
	  //read front and rear alignment coefficient matrices
	  if (align == 0 && calib !=1){
	    t = 0;
	    for (Int_t m = 0; m < 4; m++){
	      for (Int_t n = 0; n < 6; n++){
		fM1Front[n][m] = m1front_v[t];
		fM1Rear[n][m] = m1rear_v[t];
		t++;
	      }
	    }
	  }
	  //read demultiplexing cuts 
	  t = 0;
	  for (Int_t m=0;m<FPP_NELEM;m++){
	    for (Int_t n=0;n<FPP_MAXHIT;n++){
	      //if ( m==23 ) printf(" m = %d  n = %d demux = ",m,n);
	      for (Int_t r=0;r<9;r++){
		fDemuxCut[m][n][r] = icut_v[t];
		// if ( m==23 ) printf(" %f ",fDemuxCut[m][n][r]);
		t++;
	      }
	      //if ( m==23 ) printf(" \n ");
	      for (Int_t s =0; s < 8;s++){ 
		fPHits[m][n][s] = 0.;
		fPSum[m][n][s] = 0.;
		fPCentr[m][n][s] = (fDemuxCut[m][n][s] + fDemuxCut[m][n][s+1])/2.;
		//fPWidth[m][n][s] = 5.;
	      }
	    }
	  }
	  
	  
	  //read drift distance parameters
	  
	  t = 0;
	  for (Int_t m = 0; m < FPP_NELEM; m++){	  
	    for (Int_t n=0; n < 7; n++){	     
	      fDrftDstPara[m][n] = DrftDst_v[t];
	      //cout<<"fDrftDstPara[]:"<<fDrftDstPara[m][n]<<endl;
	      //cout<<"DrftDst_v:"<<DrftDst_v[t]<<endl;
	      t++;
	    }
	  }
        }

	delete fpplist; fpplist = 0;
	delete carblist; carblist = 0;
	delete toff; toff = 0;
	delete tadd; tadd = 0;
	delete m1front; m1front = 0;
	delete m1rear; m1rear = 0;
	delete icut; icut = 0;
	delete DrftDst; DrftDst = 0;
	return status;


}

//_____________________________________________________________________________
THaFpp::~THaFpp()
{
	// Destructor. Remove variables 
        // from global list and delete subdetectors

        if( fIsSetup )
         RemoveVariables();
	for(Int_t i = 0; i < FPP_NELEM; i++){
		delete fPlane[i];
	}
}

//_____________________________________________________________________________
Int_t THaFpp::DefineVariables( EMode mode)
{
       if( mode == kDefine && fIsSetup ) return kOK;
       fIsSetup = ( mode == kDefine );
       RVarDef vars[] = {
         {"THits", "Total number of hits per event", "fTNHits"},
         {"TGHits", "Total number of Good hits per event", "fTNGHits"},
         {"ntrackfu","# of tracks for front U ch.","ntrackfu"},
         {"ntrackfv","# of tracks for front V ch.","ntrackfv"},
         {"ntrackrv","# of tracks for rear V ch.","ntrackfv"},
         {"ntrackru","# of tracks for rear U ch.","ntrackru"},
         {"conetest","+1 if passed -1 if failed conetest","conetest"},
         {"coneteste","E. Brash new Conetest, +1 pass, -1 fail","coneteste"},
         {"x_fa",    "x at z=0 from FPP front chambers", "x_fa"},
         {"y_fa",    "y at z=0 from FPP front chambers", "y_fa"},
         {"x_fb",    "x at z=.350 from FPP front chambers", "x_fb"},
         {"y_fb",    "y at z=.350 from FPP front chambers", "y_fb"},
         {"th_f",    "theta from FPP front chambers"      , "th_f"},
         {"ph_f",    " phi from FPP front chambers "      , "ph_f"},
         {"x_r",     "x at z=.350 from FPP rear chambers" , "x_r"},
         {"y_r",     "y at z=.350 from FPP rear chambers" , "y_r"},
         {"th_r",    "theta from FPP rear chambers"       , "th_r"},
         {"ph_r",    "phi from FPP rear chambers"         , "ph_r"},
         {"x_x",     "x at z=.350 from FPP X-Straws"      , "x_x"},
         {"x_diff_f","diff in X (VDC vs. FPP Front Chs.)" , "x_diff_f"},
         {"y_diff_f","diff in Y (VDC vs. FPP Front Chs.)" , "y_diff_f"},
         {"x_diff_r","diff in X (FPP Front vs. FPP Rear Chs.)", "x_diff_r"},
         {"y_diff_r","diff in Y (FPP Front vs. FPP Rear Chs.)", "y_diff_r"},
         {"x_diff_x","diff in X (FPP Front vs. FPP X straws)", "x_diff_x"},
         {"th_diff_f","diff in Theta (VDC vs. FPP Front Chs.)", "th_diff_f"},
         {"ph_diff_f","diff in Phi (VDC vs. FPP Front Chs.)",  "ph_diff_f"},
         {"th_diff_r","diff in Theta (FPP Front vs. FPP Rear Chs.)","th_diff_r"},
         {"ph_diff_r","diff in Phi (FPP Front vs. FPP Rear Chs.)","ph_diff_r"},
         {"th_sc",    "Theta scattered"                    , "th_sc"},
         {"ph_sc",    "Phi scattered"                      , "ph_sc"},
         {"th_az",    "Theta scattered (polar coordinates)", "th_az"},
         {"ph_az",    "Phi scattered (polar coordinates)"  , "ph_az"},
         {"zclose",   "Z of closest approach of front/rear tracks", "zclose"},
         {"sclose",   "distance of closest approach of f/r tracks", "sclose"},
         {"chisqfu",  "chisq for front chamber u track"           , "chisqfu"},
         {"chisqfv",  "chisq for front chamber v track"           , "chisqfv"},
         {"chisqru",  "chisq for rear  chamber u track"           , "chisqru"},
         {"chisqrv",  "chisq for rear  chamber v track"           , "chisqrv"},
        {0}
       };


       return DefineVarsFromList(vars,mode);
}

//_____________________________________________________________________________
Int_t THaFpp::Decode( const THaEvData& evdata )
{
	// Decode data from Fpp planes

	Clear();
	for(Int_t i=0; i < FPP_NELEM; i++){
		fPlane[i]->Decode(evdata);
	}
	return 0;
}

//_____________________________________________________________________________
Int_t THaFpp::CoarseProcess( TClonesArray& tracks )
{
      if(!fIsInit){
        fWspacingr = fPlane[12]->GetWSpac()*100.;
	fWspacingf = fPlane[0]->GetWSpac()*100.;
	for (Int_t m = 0; m < FPP_NELEM; m++){
	  //cout<<fPlane[m]->GetZ()<<" "<<fPlane[m]->GetZZ()<<" "<<fZ<<" "; 
		fZfpp[m] = ( fPlane[m]->GetZ() + fPlane[m]->GetZZ()  + fZ )*100.;
		//cout<<fZfpp[m]<<endl;
		fUVXzero[m] = fPlane[m]->GetUVX0()*100.;
		fZmid[m] = fPlane[m]->GetXoff()*100.;
		fUVX_first[m] = fPlane[m]->GetYoff()*100.;
		fUVrot[m] = fPlane[m]->GetPhoff();
		fUVno[m] = fPlane[m]->GetWAngle();
		fZUrot[m] = fPlane[m]->GetThoff();
		fZVrot[m] = fPlane[m]->GetPsoff();
		fWSpacing[m] = fPlane[m]->GetWSpac()*100.;
		fZshift[m] = fPlane[m]->GetdZ()*100.;
		fUVX_shift[m] = 0.;
                if (m == 1 || m == 4 || m == 7 || m == 10 || m == 19) {
                   fUVX_shift[m] = - fWSpacing[m]/2.;
                }
                if ( m == 13 || m == 15 || m == 17 || m == 22) {
                   fUVX_shift[m] = fWSpacing[m]/2.;
                }
                if ( m == 20 ) {
                   fUVX_shift[m] = - fWSpacing[m];
                }
                if ( m == 23 ) {
                   fUVX_shift[m] =  fWSpacing[m];
                }
	}
        fIsInit=true;
      }

     if(fDebug > 0){
         cout << "**** Geometry from database for FPP ****" << endl;
         cout << " Plane     Zpos (cm)    UVXzero (cm) " << endl;
         cout.setf(ios::fixed);
         cout.setf(ios::showpoint);
         cout.precision(7);
         for(Int_t i=0; i < 24; i++){
            cout << i+1  << " " << fZfpp[i] << " " << fUVXzero[i] << endl;
         }
         cout.precision(14);
         cout << "*** Other Pars from db for FPP ***" << endl;
         cout << "Wire Spacings are  " << setprecision(14) << fWspacingf << " " << fWspacingr << endl;
         cout << "Carbon at " << setprecision(14) << fZcarbon << " " << fZcarbonmin << " " << fZcarbonmax << endl;
         cout.unsetf (ios::fixed); 
      }
	return 0;
}

//_____________________________________________________________________________
Int_t THaFpp::FineProcess( TClonesArray& tracks )
{
        Int_t ntrack;	       //# of tracks
        Int_t itrk[5];         //# of tracks found at each tracking interation
        Int_t icode;           //signal to main code of tracking problem 0=o.k.
        Int_t itrcode;         //tracking code 
        Int_t ntr[4],ntrack_front,ntrack_rear,ntrack_tot,prev_straw;
        Double_t track[25][40];       //track information
        Double_t zplane,str,ddist,uvx,a,b,c,d,ee,ff,gg,hh,iii,ztr,yw;
        Double_t  xhat1r,yhat1r,zhat1r;        
        Double_t jjj,dd2by2,dd2,dd,resres,fac,residual;
        Double_t t1,t2,t3,t4,t5,t6,a0, b1, b2, c1, c2, c3, lambda1, lambda2,fg;
        Double_t xfaold,xrold,yfaold,yrold,xfbold,yfbold,tfold,trold,pfold,prold;
        Double_t psif,psir,xhatr,yhatr,zhatr;
        Double_t thetasc,phisc,psisc,thetafd,phifd,x_sc,y_sc,z_sc,psi_sc,r_sc;

        Double_t   zone[25],xone[25],dxone[25],dxfone[25];
        Int_t      uone[25],lrfit[25];
        Double_t   vat0,uat0,vchi, vslope ,uchi, uslope;
        Int_t      tr_num;


        Double_t   usave[6],zusave[6],dusave[6];
        Double_t   us,vs,zs1,vs1,vs3,us2,us3,zs2,psi_tr,zs;
        Int_t      uplsave[6],vplsave[6],npl,npl_skip;
        Double_t   vsave[6],zvsave[6],dvsave[6];

        Double_t   ave1,ave2,zave1,zave2,npt1,npt2,ang_cr,int_cr;
        Double_t   chis[25];

        Double_t   xatz0f,yatz0f,xatz0r,yatz0r,xatz0x;
        Double_t   xtr[24],ytr[24],uvxtr[24],z[24];
        Double_t   uvx_low,uvx_hi,strw_rad,uvx0;
        Int_t      straw_pred[24],nstraws,itrgood[24];
        Double_t   tthetaf,tphif,tthetaa,tphia,thinitd,phiinitd;
        Double_t   tthetar,tphir,theta_sc,phi_sc,theta_az,phi_az;
        Double_t   xdiff_front,ydiff_front,xdiff_rear,ydiff_rear,xdiff_x;
        Double_t   theta_difff,phi_difff,theta_diffr,phi_diffr;
        Double_t   term1,term2,term3,term4,term5,term6;
        Double_t   zclose_old, sclose_old;
        Int_t      iwiregroup,igroup;
        Double_t   letime,tetime,tdiff;
        Double_t   diff=0,rstraw,drdist;
        Int_t      imark,index=0,itime;
        Double_t   hits[24][35][5];
/*        Double_t   front_old[4], rear_old[4];
        Double_t   front_new[4], rear_new[4];
        Double_t   zfront = 100.,zrear = 360.;*/

	Double_t xinit, yinit, phiinit, thinit, dtime;

        Int_t wiregrp, istraw, ngoodhit[25];

	Int_t iss_front[8]   = {1,3,2,4,6,8,5,7};
	Int_t iss_rear_v3[8] = {1,3,2,4,6,8,5,7};
	Int_t iss_rear_u3[8] = {8,6,7,5,3,1,4,2};
	Int_t iss_rear_x3[8] = {1,3,2,4,6,8,5,7};
	Int_t iss_rear_u4[8] = {8,6,7,5,3,1,4,2};
	Int_t iss_rear_v4[8] = {8,6,7,5,3,1,4,2};

// New conetest variables
        Double_t xf3,xf4,yf3,yf4, xr3,xr4,yr3,yr4;
        Double_t dcone3,dcone4, zback,ttheta;
        Double_t r1x3,r2x3,r1y3,r2y3,r1x4,r2x4,r1y4,r2y4;

        Double_t xpt3[8],ypt3[8],xpt4[8],ypt4[8];
        Int_t oddnodes3,oddnodes4, j;
        Bool_t inside3,inside4;

        Double_t xpoly3[6] = {-110.0,-87.0,110.0,110.0,87.0,-110.0};
        Double_t ypoly3[6] = {33.0,55.0,55.0,-33.0,-55.0,-55.0};
        Int_t nsides3 = 6;

        Double_t xpoly4[8] = {-140.0,-72.0,104.0,140.0,140.0,72.0,-104.0,-140.0};
        Double_t ypoly4[8] = {2.0,70.0,70.0,38.0,-2.0,-70.0,-70.0,-38.0};
        Int_t nsides4 = 8;

	Int_t n_track = tracks.GetLast()+1;
	if(n_track >= 1){
		THaTrack* theTrack = static_cast<THaTrack*>( tracks[0] );
		xinit = theTrack->GetX()*100.;
		yinit = theTrack->GetY()*100.;
		phiinit = theTrack->GetTheta();
		thinit = theTrack->GetPhi();
	}
	else {
		xinit = kBig;
		yinit = kBig;
		phiinit = kBig;
		thinit = kBig;
	}
	if (fDebug > 0) {
		cout << "Calc vdc quantites" << "\n" << " x = " << xinit << " y = "
			<< yinit << " dydz = " << thinit << " dxdz = " << phiinit << "\n";
	}

	thinit = atan(thinit);
	phiinit = atan(phiinit); 

	// Initialize Hit array
	for(Int_t k = 0; k < 5; k++){
		for(Int_t j = 0; j< FPP_MAXHIT; j++){
			for(Int_t i=0; i<FPP_NELEM; i++) {hits[i][j][k]=0.0;}
		}
	}    
	// Initialize Track array
	for(Int_t i=0; i < 40; i++){
		for(Int_t j=0; j<25; j++) {track[j][i]=0.0;}
	}
	for(Int_t i=0; i < 25; i++){
		ngoodhit[i] = 0;
	}

	//Loop over all hits, demultiplex wiregroups into straws and
	//determine good hits based on drift distance

	fTNHits = 0;
        fTNGHits = 0;
	for(Int_t i=0; i <  FPP_NELEM; i++){
		fNHits[i] = fPlane[i]->GetNHits();
		prev_straw = -1;
		for(Int_t j=0;j < fNHits[i];j++){
                        fTNHits++;
			THaFppHit* hit = fPlane[i]->GetHit(j);
			wiregrp = hit->GetWire();
			iwiregroup = wiregrp -1;
			letime= hit->GetLEtime()/2.0; 
			tetime= hit->GetTEtime()/2.0;
			tdiff=letime-tetime;
			hit->SetWidth(tdiff);
			imark = 0;
			for(Int_t k = 0; k < 9; k++){
				if(tdiff > fDemuxCut[i][iwiregroup][k]) {imark++;}
			}
			if(imark > 0 && imark <= 8){
				if(i >= 0 && i <= 11)   {diff=iss_front[imark-1];}
				if(i >= 12 && i <= 13)  {diff=iss_rear_u3[imark-1];}
				if(i >= 18 && i <= 20)  {diff=iss_rear_u4[imark-1];}
				if(i >= 14 && i <= 15)  {diff=iss_rear_v3[imark-1];}
				if(i >= 21 && i <= 23)  {diff=iss_rear_v4[imark-1];}
				if(i >= 16 && i <= 17)  {diff=iss_rear_x3[imark-1];}
			}
			else {
				if(imark > 8) {
					imark=0;
					if(i >= 0 && i <= 11)   {diff=iss_front[7];}
					if(i >= 12 && i <= 13)  {diff=iss_rear_u3[7];}
					if(i >= 18 && i <= 20)  {diff=iss_rear_u4[7];}
					if(i >= 14 && i <= 15)  {diff=iss_rear_v3[7];}
					if(i >= 21 && i <= 23)  {diff=iss_rear_v4[7];}
					if(i >= 16 && i <= 17)  {diff=iss_rear_x3[7];}
				}
				else {
					if(i >= 0 && i <= 11)   {diff=iss_front[0];}
					if(i >= 12 && i <= 13)  {diff=iss_rear_u3[0];}
					if(i >= 18 && i <= 20)  {diff=iss_rear_u4[0];}
					if(i >= 14 && i <= 15)  {diff=iss_rear_v3[0];}
					if(i >= 21 && i <= 23)  {diff=iss_rear_v4[0];}
					if(i >= 16 && i <= 17)  {diff=iss_rear_x3[0];}
				}
			}
			rstraw=8.*(wiregrp -1)+diff;
			istraw = static_cast<Int_t>(rstraw);
			hit->SetStraw(rstraw);

                        //Calculate drift distance to wire
                        dtime = fTOff[iwiregroup][i] - letime;
			//cout<<"dtime: "<<dtime<<" "<<iwiregroup<<" "<<i<<"TOff: "<<fTOff[iwiregroup][i]<<endl;

                        if((dtime > 0.0) && (dtime <= 200.0)){
                                drdist=0.;
                                if (dtime >= 150) {
                                  drdist = .521;
                                } else {
                                for(Int_t j = 0; j < 7; j++){
                                        drdist = drdist + fDrftDstPara[i][j]*pow(dtime,j);
                                }
				//cout<<"drdist(1):"<<drdist<<endl;
                                if(drdist > 0.522) drdist=0.522;
                                if (drdist <  0.) drdist = 0.;
                                }
                        }
                        else if ( (dtime <= 0.) && (dtime > -10.) ) {drdist = 0.0;}
                        else {drdist=1.0;}

			//cout<<"drdist: "<<drdist<<endl;
			
                        hit->SetDrftDist(drdist);
			if((letime >= 300.) && (letime <= 1400.)){
				fTHits[iwiregroup][i] = fTHits[iwiregroup][i]+1.0;
				fTSum[iwiregroup][i] = fTSum[iwiregroup][i]+ letime*1.0;
			}
                        if((tdiff > 15.) && (tdiff < 110.)){
			  for(Int_t k = 0; k < 8; k++){
				if((calib >= 1) && (calib <= 8)) {
					if(i >= 0 && i <= 11) {if(calib == iss_front[k]){index=k;}}
					if(i >= 12 && i <= 13) {if(calib == iss_rear_u3[k]){index=k;}}
					if(i >= 18 && i <= 20) {if(calib == iss_rear_u4[k]){index=k;}}
					if(i >= 14 && i <= 15) {if(calib == iss_rear_v3[k]){index=k;}}
					if(i >= 21 && i <= 23) {if(calib == iss_rear_v4[k]){index=k;}}					
                                        if(i >= 16 && i <= 17) {if(calib == iss_rear_x3[k]){index=k;}}
				}
				else{
					if(imark >= 1 && imark <= 8) {index=imark-1;}
					else {index=0;}
				}
			  }
                          if(fDebug > 0){cout << index+1<<" ";}
                        
			// Try to decide if it was a reasonable hit or some cross talk - If
			// it is within 15 channels of the "standard" value, then include it.
			// If it is not then do not include it.  15 channels is probably ok
			// because of the ordering in the demuxing.  We will never have cross
			// talk in the adjacent peak - it will always be about 2 peaks away. 

			        if(tdiff >= fDemuxCut[i][iwiregroup][index]-10.0 && 
			           tdiff <= fDemuxCut[i][iwiregroup][index]+20.0) {
			           fPHits[i][iwiregroup][index]= fPHits[i][iwiregroup][index] + 1.0;
			           fPSum[i][iwiregroup][index]= fPSum[i][iwiregroup][index] + tdiff;
			        }
                         }
		
			itime = Int_t(fTOff[iwiregroup][i] - letime) + 40;
			if(itime > 0 && itime <= 80) {
			   fTHist[itime-1][iwiregroup][i]++;
			}
			if((drdist >= 0.0 && drdist <= 0.522) || pulser_fpp == 1) {
				if (imark != 0) {
					if(pulser_fpp == 1) {
						fTNGHits++;
						new( (*fPlane[i]->fGHits)[ngoodhit[i]++] ) 
                                                      THaFppHit( wiregrp, letime, tetime);
						THaFppHit* goodhit = fPlane[i]->GetGHit(ngoodhit[i]);
						goodhit->SetLETime(fTOff[iwiregroup][i] - letime*1.);
						goodhit->SetWidth(tdiff);
						goodhit->SetStraw(rstraw);
						prev_straw = istraw;
						goodhit->SetDrftDist(drdist);
						hits[i][ngoodhit[i]-1][0] = rstraw; //straw number within plane
						hits[i][ngoodhit[i]-1][1] = drdist; //drift distance in cm
						hits[i][ngoodhit[i]-1][4] = static_cast<Double_t>(i+1); //plane number
					}
					else {
						fTNGHits++;
						if ( ngoodhit[i] >=  1 && prev_straw == istraw){       
							ngoodhit[i]--;
						} //if multi-hit in straw want last hit
						new( (*fPlane[i]->fGHits)[ngoodhit[i]++] ) 
                                                      THaFppHit( wiregrp, letime, tetime);
						THaFppHit* goodhit = fPlane[i]->GetGHit(ngoodhit[i]-1);
						goodhit->SetLETime(fTOff[iwiregroup][i] - letime*1.);
						goodhit->SetWidth(tdiff);
						goodhit->SetStraw(rstraw);
						prev_straw = istraw;
						goodhit->SetDrftDist(drdist);		       
						hits[i][ngoodhit[i]-1][0] = rstraw; //straw number within plane
						hits[i][ngoodhit[i]-1][1] = drdist; //drift distance in cm
						hits[i][ngoodhit[i]-1][4] = static_cast<Double_t>(i+1); //plane number
					}
				}
			}
		}// end loop over hits
	}//end loop over fpp planes

	if (fDebug > 0) {
		cout << "Raw hits" << "\n"; 
		cout << "plane  #hit    WG  straw  ttdc  ltdc" << "\n";
		for(Int_t i = 0; i < FPP_NELEM; i++ ){
			for(Int_t j=0; j < fPlane[i]->GetNHits(); j++){
				THaFppHit* hit = fPlane[i]->GetHit(j);
				cout << i+1 << " " << j+1 << " " << hit->GetWire() << " " << hit->GetStraw() 
					<< " " << hit->GetTEtime()/2.0 << " " << hit->GetLEtime()/2.0 << "\n";                
			}
		}
		cout << "Good hits" << "\n"; 
		cout << "plane  #hit     WG  straw  letediff  drdist" << "\n";
		for(Int_t i=0; i<FPP_NELEM; i++){
                       if (ngoodhit[i] != fPlane[i]->GetNGHits()){
                            cout << "nhits .ne. nghits" << endl;
                        } 
			for(Int_t j=0; j<fPlane[i]->GetNGHits(); j++){
				THaFppHit* hit = fPlane[i]->GetGHit(j);
				cout << i+1 << " " << j+1 << " " << hit->GetWire() << " " << hit->GetStraw() 
					<< " " << hit->GetWidth() << " " << hit->GetDrftDist() << "\n";
			}
		}
	}
	//cout<<"mkjtrack: "<<mkjtrack<<endl;
	// Now calculate the z and y (or x) position of the straw knowing what
	// plane it is and what straw it is within the plane. EJB method
	if (!mkjtrack){
		for(Int_t i = 0; i < 12; i++){
			for(Int_t j = 0; j < fPlane[i]->GetNGHits(); j++){
				hits[i][j][2]=fUVXzero[i] + fWspacingf*(hits[i][j][0]-1);
				hits[i][j][3]=fZfpp[i];
				//cout<<"fZfpp[i]:"<<fZfpp[i]<<endl;
			}
		}
		for(Int_t i=12; i < FPP_NELEM; i++){ 
			for(Int_t j=0; j< fPlane[i]->GetNGHits(); j++){
				hits[i][j][2]=fUVXzero[i] + fWspacingr*(hits[i][j][0]-1.);
				hits[i][j][3]=fZfpp[i];
                        }
		}
	}

	// KW First find rough u,v positions for each wire plane using wire positions
	// only. This can be used to correct for chamber mis-alignments

	if (mkjtrack) {
		for(Int_t i = 0; i < FPP_NELEM; i++){ 
			for(Int_t j=0; j < fPlane[i]->GetNGHits(); j++){
				hits[i][j][2]=fUVX_first[i]+fUVX_shift[i]+ fWSpacing[i]*(hits[i][j][0]-1);
				hits[i][j][3]=fZmid[i]+fZshift[i];
			}
		}
	}

	// Call FORTRAN tracking routines (trk4) sending it the hits array and the 
	// number of good hits
	//cout<<"pulser_fpp"<<pulser_fpp<<endl;
	icode=0;
	if (pulser_fpp) {
		icode=1;
		for(Int_t i = 0; i < 5; i++) {
			itrk[i]=0;
		}
	}
	else { 
		if(ana_front) {
			itrcode = 1;
			itrk[itrcode-1]=0;
			trk4_(fDebug,itrcode,ngoodhit,&hits[0][0][0],ntrack,&track[0][0],usave,zusave,dusave,uplsave);
			ntr[itrcode-1] = ntrack;
		
			if (ntrack >= 1) {itrk[itrcode - 1] = 1;}
			if (ntrack == 0) {
				icode = 1;
				chisqfu = kBig;
			}
			else {chisqfu = track[0][11];}
			itrcode = 2;
			itrk[itrcode - 1] = 0;
			trk4_(fDebug,itrcode,ngoodhit,&hits[0][0][0],ntrack,&track[0][0],vsave,zvsave,dvsave,vplsave);
			ntr[itrcode-1] = ntrack;
			if (ntrack >= 1) {itrk[itrcode -1] = 1;}
			if (ntrack == 0) {
				icode=1;
				chisqfv = kBig;
			}
			else {chisqfv = track[0][10];}

			if (itrk[0] == 1 && itrk[1] == 1 &&  mkjtrack) {
				if (fDebug > 0) {cout << "Correct front u track" << "\n";}
				ave1 = 0.;
				ave2 = 0.;
				zave1 = 0.;
				zave2 = 0.;
				npt1 = 0.;
				npt2 = 0.;
				for(Int_t i=0; i < Int_t(track[0][14]); i++) {
					npl = vplsave[i];
					if ( npl <= 6) {
						ave1 = ave1 + vsave[i];
						zave1 = zave1 + zvsave[i];
						npt1 = npt1 + 1;
					}
					else {
						ave2 = ave2 + vsave[i];
						zave2 = zave2 + zvsave[i];
						npt2 = npt2 + 1;
					}
				}
				ave1 = ave1/npt1;
				zave1 = zave1/npt1;
				ave2 = ave2/npt2;
				zave2 = zave2/npt2;
				if ( (zave2-zave1) != 0. ) {
					ang_cr = (ave2-ave1)/(zave2-zave1);
				}
				else {ang_cr = 0.;}
				int_cr = ave1 - ang_cr*zave1;
				if (fDebug > 0) {
					cout << " crude calc " << "\n" <<  int_cr << " " << ang_cr
						<< " " << track[0][2] << " " << track[0][3] << "\n";
					cout << " vs   us  zs  vs1  zs1   us2  zs2   us3    psi_tr " << "\n";
				}
				for(Int_t i=0; i < Int_t(track[0][15]); i++) {
					npl = uplsave[i];
					//        vs = track[0][2] +  track[0][3]*zusave[i];
					vs = int_cr +  ang_cr*zusave[i];
					us = usave[i];  
					psi_tr = atan(tan(fZUrot[npl-1]) * cos(fZVrot[npl-1]));
					zs = fZshift[npl-1];
					vs1= cos(fZVrot[npl-1])*vs + sin(fZVrot[npl-1])*zs; 
					zs1= cos(fZVrot[npl-1])*zs - sin(fZVrot[npl-1])*vs;
					us2= cos(psi_tr)*us - sin(psi_tr)*zs1;
					zs2= cos(psi_tr)*zs1 + sin(psi_tr)*us;
					us3= cos(fUVrot[npl-1])*us2 - sin(fUVrot[npl-1])*vs1;
					if (fDebug > 0) {
						cout << vs << " " << us << " " << zs << " " 
                                                     << vs1 << " " << zs1 << " " << us2 << " " 
                                                     << zs2 << " " << us3 << " " << psi_tr << "\n";
					}
					zone[i] = zs2 + fZmid[npl-1];
					xone[i] = us3;
					dxone[i] = dusave[i];
					uone[i] = 1;
					dxfone[i] = 0.;
				}
				if (fDebug > 0) {
					cout << " plane  u  du   z " << "\n";  
					for(Int_t i=0; i < Int_t(track[0][15]); i++) {
						cout << uplsave[i] << " " << xone[i] << " " << dxone[i] 
						<< " " << zone[i] << "\n";
					}
				}
				tr_num = Int_t(track[0][15]);
				trackit_(tr_num,zone,xone,dxone,uone,dxfone,lrfit,uat0,uslope,uchi,chis);
				if (fDebug > 0) { 
					cout << tr_num << " " << uat0 << " " << uslope << " " << uchi << "\n";     
					cout << " Correct front v track " << "\n";
				}
				ave1 = 0.;
				ave2 = 0.;
				zave1 = 0.;
				zave2 = 0.;
				npt1 = 0.;
				npt2 = 0.;
				for(Int_t i=0; i < Int_t(track[0][15]);i++){
					npl = uplsave[i];                 
					if ( npl <= 6) {
						ave1 = ave1 + usave[i];
						zave1 = zave1 + zusave[i];
						npt1 = npt1 + 1;
					}
					else {
						ave2 = ave2 + usave[i];
						zave2 = zave2 + zusave[i];
						npt2 = npt2 + 1;
					}
				}
				ave1 = ave1/npt1;
				zave1 = zave1/npt1;
				ave2 = ave2/npt2;
				zave2 = zave2/npt2;
				if ( (zave2-zave1) != 0. ) {
					ang_cr = (ave2-ave1)/(zave2-zave1);
				}
				else {ang_cr = 0;}
				int_cr = ave1 - ang_cr*zave1;
				if (fDebug > 0) {
					cout << " crude calc " << "\n" << int_cr << " " << ang_cr << " " 
						<< track[0][0] << " " << track[0][1] << "\n";
					cout << " vs   us  zs  vs1  zs1  us2  zs2   vs3    psi_tr " << "\n"; 
				}
				for(Int_t i=0; i < Int_t(track[0][14]); i++) {
					npl = vplsave[i];                 
					//        us = track[0][0] +  track[0][1]*zvsave[i];
					us = int_cr +  ang_cr*zvsave[i];
					vs = vsave[i]; 
					psi_tr = atan(tan(fZUrot[npl-1])*cos(fZVrot[npl-1]));
					zs =  fZshift[npl-1];
					vs1 = cos(fZVrot[npl-1])*vs + sin(fZVrot[npl-1])*zs;
					zs1 = cos(fZVrot[npl-1])*zs - sin(fZVrot[npl-1])*vs;
					us2 = cos(psi_tr)*us - sin(psi_tr)*zs1;
					zs2 = cos(psi_tr)*zs1 + sin(psi_tr)*us;
					vs3 = sin(fUVrot[npl-1])*us2 + cos(fUVrot[npl-1])*vs1;
					if (fDebug > 0) {
						cout <<  vs << " " << us << " " << zs << " " << vs1 << " " << zs1 << " " 
							<< us2 << " " << zs2 << " " << vs3 << " " << psi_tr << "\n";
					}
					zone[i] = zs2 + fZmid[npl-1];
					xone[i] = -sin(fUVno[npl-1])*us2 + cos(fUVno[npl-1])*vs3;
					dxone[i] = dvsave[i];
					uone[i] = 1;
					dxfone[i] = 0.;
				}
				if (fDebug > 0) {
					cout <<  " plane  v  dv   z " << "\n";  
					for(Int_t i=0; i < Int_t(track[0][14]);i++){
				              cout << vplsave[i] << " " << xone[i] << " " 
                                                   << dxone[i] << " " << zone[i] << "\n";
					}
				}
				tr_num = Int_t(track[0][14]);
				trackit_(tr_num,zone,xone,dxone,uone,dxfone,lrfit,vat0,vslope,vchi,chis);
				if (fDebug > 0) {
					cout << tr_num << " " << vat0 << " " << vslope << " " << vchi << "\n";  
				}
				track[0][0] = uat0;
				track[0][1] = uslope;
				track[0][11] = uchi;
				chisqfu = track[11][0];
				track[0][2] = vat0;
				track[0][3] = vslope;
				track[0][10] = vchi;
				chisqfv = track[0][10];
			}
		}
		else {
			itrk[0]=0;
			itrk[1]=0;
		}
		if(ana_rear) {
			itrcode = 3;
			itrk[itrcode-1]=0;
			trk4_(fDebug,itrcode,ngoodhit,&hits[0][0][0],ntrack,&track[0][0],usave,zusave,dusave,uplsave);
			ntr[itrcode-1] = ntrack;
			if (ntrack >= 1) {itrk[itrcode-1]=1;}
			if(ntrack == 0) {
				icode=1;
				chisqru = kBig;
			}
			else {chisqru = track[0][13];}
			itrcode = 4;
			itrk[itrcode-1]=0;
			trk4_(fDebug,itrcode,ngoodhit,&hits[0][0][0],ntrack,&track[0][0],vsave,zvsave,dvsave,vplsave);
			if (ntrack >= 1) {itrk[itrcode-1]=1;}
			if(ntrack == 0) {
				icode=1;
				chisqrv = kBig;
			}
			else {chisqrv = track[0][12];}
			if (itrk[2]==1 && itrk[3]==1 && mkjtrack ) {
				if (fDebug > 0) {cout << " Correct rear u track " << "\n";}
				ave1 = 0.;
				ave2 = 0.;
				zave1 = 0.;
				zave2 = 0.;
				npt1 = 0.;
				npt2 = 0.;
				for( Int_t i=0; i < Int_t(track[0][16]); i++) {
					npl = vplsave[i];
					if ( npl <= 16) {
						ave1 = ave1 + vsave[i];
						zave1 = zave1 + zvsave[i];
						npt1 = npt1 + 1;
					}
					else {
						ave2 = ave2 + vsave[i];
						zave2 = zave2 + zvsave[i];
						npt2 = npt2 + 1;
					}
				} 
				ave1 = ave1/npt1;
				zave1 = zave1/npt1;
				ave2 = ave2/npt2;
				zave2 = zave2/npt2;
				if ( (zave2-zave1) != 0. ) {ang_cr = (ave2-ave1)/(zave2-zave1);}
				else {ang_cr = 0.;}
				int_cr = ave1 - ang_cr*zave1;
				if (fDebug > 0) {
					cout << " crude calc " << "\n" << ave1 << " " << zave1 << " " << ave2 
                                             << " " << zave2 << "\n"
					     << int_cr << " " << ang_cr << " " << track[0][6] << " " << track[0][7] 
                                             << "\n"
					     <<  " vs   us  zs  vs1  zs1  us2  zs2   us3    psi_tr " << "\n";
				}
				for (Int_t i=0; i < Int_t(track[0][17]); i++) {
					npl = uplsave[i];
					if ( npl == 13 || npl == 14) {npl_skip = 2;}
					else {npl_skip = 3;}
					//             vs = track[0][6] +  track[0][7]*zusave[i];
					vs = int_cr +  ang_cr*zusave[i];
					us = usave[i];  
					psi_tr = atan(tan(fZUrot[npl-1]) * cos(fZVrot[npl-1]));
					zs = fZshift[npl-1];
					vs1= cos(fZVrot[npl-1])*vs + sin(fZVrot[npl-1])*zs; 
					zs1= cos(fZVrot[npl-1])*zs - sin(fZVrot[npl-1])*vs; 
					us2= cos(psi_tr)*us - sin(psi_tr)*zs1;
					zs2= cos(psi_tr)*zs1 + sin(psi_tr)*us;
					us3= cos(fUVrot[npl-1])*us2 - sin(fUVrot[npl-1])*vs1;
					if (fDebug > 0) {
						cout << vs << " " << us << " " << vs1 << " " << " " 
							<< us2 << " " << zs2 << " " << us3 << " " 
                                                        << fZmid[npl-1] << "\n";
					}
					zone[i] = zs2 + fZmid[npl-1];
					xone[i] = us3;
					dxone[i] = dusave[i];
					uone[i] = 1;
					dxfone[i] = 0.;
				}
				if (fDebug > 0) {
					cout << " plane  u  du   z " << "\n";  
					for( Int_t i=0; i < Int_t(track[0][17]);i++) {
						cout << uplsave[i] << " " << xone[i] << " " 
                                                     << dxone[i] << " " << zone[i] << "\n";
					}
				}
				tr_num = Int_t(track[0][17]);
				trackit_(tr_num,zone,xone,dxone,uone,dxfone,lrfit,uat0,uslope,uchi,chis);
				if (fDebug > 0) { 
					cout << tr_num << " " << uat0 << " " << uslope << " " << uchi << "\n";     
					cout << " Correct rear v track " << "\n" << " plane  v  dv   z " <<"\n";  
				}
				ave1 = 0.;
				ave2 = 0.;
				zave1 = 0.;
				zave2 = 0.;
				npt1 = 0.;
				npt2 = 0.;
				for(Int_t i=0 ; i < Int_t(track[0][17]);i++) {
					npl = uplsave[i];
					if ( npl <= 16) {
						ave1 = ave1 + usave[i];
						zave1 = zave1 + zusave[i];
						npt1 = npt1 + 1;
					}
					else {
						ave2 = ave2 + usave[i];
						zave2 = zave2 + zusave[i];
						npt2 = npt2 + 1;
					}
				}
				ave1 = ave1/npt1;
				zave1 = zave1/npt1;
				ave2 = ave2/npt2;
				zave2 = zave2/npt2;
				if ( (zave2-zave1) != 0 ){ang_cr = (ave2-ave1)/(zave2-zave1);}
				else {ang_cr = 0;}
				int_cr = ave1 - ang_cr*zave1;
				if (fDebug > 0) {
					cout << " crude calc " << "\n"  
						<< ave1<< " " << zave1 << " " << ave2 << " " << zave2 << "\n"
						<< int_cr << " " << ang_cr << " " << track[0][4] << " " << track[0][5] 
                                                << "\n"
						<< " vs   us  zs  vs1  zs1  us2  zs2   vs3    psi_tr " << "\n";
				}
				for(Int_t i=0; i < Int_t(track[0][16]);i++) {
					npl = vplsave[i];
					if ( npl == 15 || npl == 16) {npl_skip = 2;}
					else {npl_skip = 3;}
					//             us = track[0][4] +  track[0][5]*zvsave[i];
					us = int_cr +  ang_cr*zvsave[i];
					vs = vsave[i]; 
					psi_tr = atan(tan(fZUrot[npl-1]) * cos(fZVrot[npl-1])); 
					zs =  fZshift[npl-1];
					vs1 = cos(fZVrot[npl-1])*vs + sin(fZVrot[npl-1])*zs;
					zs1 = cos(fZVrot[npl-1])*zs - sin(fZVrot[npl-1])*vs;
					us2 = cos(psi_tr)*us - sin(psi_tr)*zs1;
					zs2 = cos(psi_tr)*zs1 + sin(psi_tr)*us;
					vs3 = sin(fUVrot[npl-1])*us2 + cos(fUVrot[npl-1])*vs1;
					if (fDebug > 0) {
				          cout << vs << " " << us << " " << zs << " " << vs1 << " " 
					       << zs1 << " " << us2 << " " << zs2 << " " << vs3 << " " << fZmid[npl-1] 
                                               << "\n";
					}
					zone[i] = zs2 + fZmid[npl-1];
					xone[i] = -sin(fUVno[npl-1])*us2 + cos(fUVno[npl-1])*vs3;
					dxone[i] = dvsave[i];
					uone[i] = 1;
					dxfone[i] = 0.;
				}
				if (fDebug > 0) {
					for( Int_t i=0;i < Int_t(track[0][16]);i++) {
						cout << vplsave[i] << " " << xone[i] << " " 
                                                     << dxone[i] << " " << zone[i] << "\n";
					}
				}
				tr_num = Int_t(track[0][16]);
				trackit_(tr_num,zone,xone,dxone,uone,dxfone,lrfit,vat0,vslope,vchi,chis);
				if (fDebug > 0) {
					cout << tr_num << " " << vat0 << " " << vslope << " " << vchi << "\n";     
				}
				track[0][6] = vat0;
				track[0][7] = vslope;
				track[0][12] = vchi;
				chisqrv = track[0][12];
				track[0][4] = uat0;
				track[0][5] = uslope;
				track[0][13] = uchi;
				chisqru = track[0][13];
			}
		}
		else {
			itrk[2]=0;
			itrk[3]=0;
			itrk[4]=0;
		}
	}
        if (fDebug > 0) {
          cout << "# Tracks for Front U  Front V  Rear U  Rear V" << endl;
          cout << "              -------  ------    ------  ------" << endl;
          cout << "		" << itrk[0] << "         " << itrk[1] << "         " 
               << itrk[2] << "         " <<itrk[3] <<"          " << endl;
          cout << "		" << "chisq " << "     " << track[0][11] << "     " 
               << track[0][10] << "     " << track[0][13] << "     " <<track[0][12] << endl;
        }
        ntrackfu = itrk[0];
        ntrackfv = itrk[1];
        ntrackru = itrk[2];
        ntrackrv = itrk[3];
      
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
//c     Write out data for event display purposes !!!!!!!! ccccccccccccc
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
//      if(fDebug > 0) {
//         for(Int_t ipl=0; ipl < 24; ipl++){
//            cout << ipl+1 << " " << min(5,ngoodhit[ipl]) << " ";
//             for(Int jj=0; jj < min(5,ngoodhit[ipl]; jj++){
//                cout << hits[ipl][jj][0] << " ";
//               }
//               cout << endl;
//         }
//         for(Int_t jj=0; jj < 4; jj++){
//            if(itrk[jj] > 0) {
//               cout << itrk[jj] << " ";
//               for(Int_t i=0; i < min(itrk[jj],25); i++){
//                  cout << track[ii][2*jj] << " " track[ii][2*jj];
//            }
//            cout << endl;
//            else {
//               cout << itrk[jj] << endl;
//            }
//         }
//         if ( hits(19,0,0) >= 120 && hits(19,0,0) <= 127){
//           fpp_plot_tracks_(ngoodhit,&hits[0][0][0],track[0][0],fZfpp,fUVX_zero);
//         }
//      }
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
      ntrack_front = itrk[0] + itrk[1];
      ntrack_rear = itrk[2] + itrk[3];
      ntrack_tot = ntrack_front + ntrack_rear;

//     mkj alignment
      if (align_opt == 10 && xinit != kBig) {
         cout << xinit << " " << yinit << " " << tan(phiinit) << " " << tan(thinit) << "\n";
              for(Int_t i=0; i < 24; i++){cout << hits[i][0][0] << " ";}
              cout << endl;
              for(Int_t i=0; i < 24; i++){cout << hits[i][0][1] << " ";}
              cout << endl;
      }
      if (align_opt == 20 && xinit != kBig) {
         cout << track[0][0] << " " << track[0][2] << " " << track[0][1] 
              << " " << track[0][3] << "\n";
              for(Int_t i=0; i < 24; i++){cout << hits[i][0][0] << " ";}
              cout << endl;
              for(Int_t i=0; i < 24; i++){cout << hits[i][0][1] << " ";}
              cout << endl;
      }
      
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
   
      if( (ntrack_front == 2 && align_opt == 0 && align) 
          || (ntrack_tot == 4 && align_opt == 1 && align) 
          || ( ntrack_tot == 4  && !align & ana_front && ana_rear) 
          || ( ntrack_front == 2  && !align & ana_front && !ana_rear)
          || ( ntrack_rear == 2 && !align & ana_rear && !ana_front) ){

          //Position Calculations !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          //Calculate x and y at z_transport=0 for both front and rear 
          //     chambers.
           xatz0f=(track[0][0]-track[0][2])/1.414213562;
           yatz0f=(track[0][0]+track[0][2])/1.414213562;
           xatz0r=(track[0][4]-track[0][6])/1.414213562;
           yatz0r=(track[0][4]+track[0][6])/1.414213562;
           xatz0x=track[0][8];

           if(fDebug > 0){
             cout << " U and V " << endl;
             cout << "Actual x and y are  " << xinit << " " << yinit << endl;
             cout << "Calculated x and y front are  " << xatz0f << " " << yatz0f << endl;
             cout << "Calculated x and y rear are  " << xatz0r << " " << yatz0r << endl;
           }

           // Angle Calculations !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
           // First, calculate front and rear angle tangents from track array
           // Note that we only do this for one track at the moment.
           // This will be modified later.

           fg = 180.00000/3.141592653589793;
           tthetaf = (track[0][1]+track[0][3])/1.414213562;
           tphif = (track[0][1]-track[0][3])/1.414213562;
           tthetar = (track[0][5]+track[0][7])/1.414213562;
           tphir = (track[0][5]-track[0][7])/1.414213562;
           thetafd = atan(tthetaf)*fg;
           phifd = atan(tphif)*fg;

           tthetaa = tan(thinit);
           tphia = tan(phiinit);
           thinitd = thinit*fg;
           phiinitd = phiinit*fg;

           if(fDebug > 0) {
              cout << "Actual angle tangents are  " << tthetaa << " " << tphia << endl;
              cout << "Calculated front angle tangents are " << tthetaf << " " << tphif << endl;
              cout << "Calculated rear angle tangents are " << tthetar << " " << tphir << endl;
           }
     
// Perform alignment corrections, but first, remember old x,y,theta,phi
// for residual calculations.

           xfaold=xatz0f;
           yfaold=yatz0f;
           xfbold=tphif*350.0+xatz0f;
           yfbold=tthetaf*350.0+yatz0f;
           xrold=xatz0r;
           yrold=yatz0r;
           tfold=tthetaf;
           pfold=tphif;
           trold=tthetar;
           prold=tphir;
	   //cout<<xatz0f<<endl;

// Call the routine which calculates the new x,y,theta, and phi based
// on the alignment correction parameters

           if (!mkjtrack){
// SD >>> NEW ROUTINE:
            aligncorr_(xatz0f,yatz0f,tthetaf,tphif,
                       xatz0r,yatz0r,tthetar,tphir,&fM1Front[0][0],&fM1Rear[0][0]);
	    //cout<<xatz0f<<endl;
	    //cout<<"--------"<<endl;
  
      /*       front_old[0] = xatz0f + zfront * tphif;
             front_old[1] = yatz0f + zfront * tthetaf;
             front_old[2] = atan(tthetaf)*fg;
             front_old[3] = atan(tphif)*fg;

             rear_old[0] = xatz0r + zrear * tphir;
             rear_old[1] = yatz0r + zrear * tthetar;
             rear_old[2] = atan(tthetar)*fg;
             rear_old[3] = atan(tphir)*fg;

             for(Int_t i = 0; i < 4; i++){
                 front_new[i] = front_old[i]+ fM1Front[5][i]
                               + fM1Front[0][i] * front_old[0] 
                               + fM1Front[1][i] * front_old[1]
                               + fM1Front[2][i] * pow(front_old[0],2)
                               + fM1Front[3][i] * pow(front_old[1],2)
                               + fM1Front[4][i] * front_old[0]*front_old[1];
                 rear_old[i] = rear_old[i] + fM1Front[5][i];
             }

//     align 'vdc-aligned rear track' with second set
//     of alignment coefficients
  
             for(Int_t i = 0; i < 4; i++) {
                  rear_new[i] = rear_old[i] + fM1Rear[5][i]
                               + fM1Rear[0][i] * rear_old[0] 
                               + fM1Rear[1][i] * rear_old[1]
                               + fM1Rear[2][i] * pow(rear_old[0],2)
                               + fM1Rear[3][i] * pow(rear_old[1],2)
                               + fM1Rear[4][i] * rear_old[0]*rear_old[1];
             }

             tthetaf = tan(front_new[2]/fg);
             tphif = tan(front_new[3]/fg);
             xatz0f = front_new[0] - zfront * tphif;
             yatz0f = front_new[1] - zfront * tthetaf;
                           
             tthetar = tan(rear_new[2]/fg);
             tphir = tan(rear_new[3]/fg);
             xatz0r = rear_new[0] - zrear * tphir;
             yatz0r = rear_new[1] - zrear * tthetar; */
           }
	   //cout<<xatz0r<<" "<<yatz0r<<endl;
           xatz0r=tphir*350.0+xatz0r;
           yatz0r=tthetar*350.0+yatz0r;
	   //cout<<xatz0r<<" "<<yatz0r<<endl;
           if(fDebug > 0){
              cout << "Corrected front angle tangents are  " << tthetaf << " " << tphif << endl;
              cout << "Corrected rear angle tangents are  " << tthetar << " " << tphir << endl;
           }

// Now, calculate differences compared to VDC tracks
// Modification of this portion by EJB - December 14, 1996.
// Want to compare front and rear fpp chambers to each other not at
// the vdc z=0 plane but at a plane midway between the front and rear
// chambers, which should be at about z=350 cm.

           xdiff_front=xinit-xatz0f;
           ydiff_front=yinit-yatz0f;

           x_fa=xatz0f;
           y_fa=yatz0f;

// Now, calculate differences between front and rear angles and VDC angles
// Note that these are differences in the angle TANGENTS.

           theta_difff=tthetaf-tthetaa;
           phi_difff=tphif-tphia;
           theta_diffr=tthetar-tthetaf;
           phi_diffr=tphir-tphif;

           th_f=fg*atan(tthetaf);
           ph_f=fg*atan(tphif);
           th_r=fg*atan(tthetar);
           ph_r=fg*atan(tphir);

           xatz0f=tphif*350.0+xatz0f;
           yatz0f=tthetaf*350.0+yatz0f;

           xdiff_rear=xatz0f-xatz0r;
           ydiff_rear=yatz0f-yatz0r;
           xdiff_x=xatz0f-xatz0x;

           x_fb=xatz0f;
           y_fb=yatz0f;
           x_r=xatz0r;
           y_r=yatz0r;
           x_x=xatz0x;

           x_diff_f=xdiff_front;
           y_diff_f=ydiff_front;
           x_diff_r=xdiff_rear;
           y_diff_r=ydiff_rear;
           x_diff_x=xdiff_x;
           th_diff_f=fg*atan(theta_difff);
           ph_diff_f=fg*atan(phi_difff);
           th_diff_r=fg*atan(theta_diffr);
           ph_diff_r=fg*atan(phi_diffr);

// At this point, we have differences , so lets call the
// routine which fills the sum arrays for alignment coefficient
// calculations.
          /*if (!mkjtrack) then
              call fpp_align_sums(xdiff_front,ydiff_front,xdiff_rear,
              ydiff_rear,thinitd,phiinitd,theta_difff,
              phi_difff,theta_diffr,phi_diffr)
            endif*/

// Now calculate scattering angles in carbon analyzer - Note that
// alignment corrections have been made at this point, so we are
// working with CORRECTED angles and positions.
// February 12, 1996 - modified to test another scattering angle
// determination method - by EJB.

           psif=atan(tphif*cos(atan(tthetaf)));
           psir=atan(tphir*cos(atan(tthetar)));
           xhatr=sin(psir);
           yhatr=cos(psir)*sin(atan(tthetar));
           zhatr=cos(psir)*cos(atan(tthetar));

           xhat1r=xhatr*cos(psif)-
               yhatr*sin(atan(tthetaf))*sin(psif)-
               zhatr*cos(atan(tthetaf))*sin(psif);
           yhat1r=
               yhatr*cos(atan(tthetaf))-
               zhatr*sin(atan(tthetaf));
           zhat1r=xhatr*sin(psif)+
               yhatr*sin(atan(tthetaf))*cos(psif)+
               zhatr*cos(atan(tthetaf))*cos(psif);

           psisc=asin(xhat1r);
           if(cos(psisc) != 0.){ 
              thetasc=asin(yhat1r/cos(psisc));
           }
           else {
              thetasc = -1000.;
              cout << "Zero problem with psisc" << endl; 
           }
           if(cos(thetasc) != 0.){ 
              phisc=atan(tan(psisc)/cos(thetasc));
           }
           else {
              phisc = -1000.;
              cout << "Zero problem with thetasc" << endl; 
           }
         
           if (fDebug > 0){
              if(abs(thetasc) >= 1.0 && abs(phisc) >= 1.0){
                 cout << endl;
                 cout << "Scattered angles  are  " << fg*thetasc << " " << fg*phisc
                      << " " << fg*psisc << endl;
                 cout << "Actual initial angles are  " << thinitd << " " << phiinitd << endl;
                 cout << "Front angles are  " << fg*atan(tthetaf)
                      << " " << fg*atan(tphif) << " " << fg*psif << endl;
                 cout << "Rear angles are  " << fg*atan(tthetar)
                      << fg*atan(tphir) << " " << fg*psir << endl;
                 cout << "Actual x and y are  " << xinit << " " << yinit << endl;
                 cout << "Calculated x and y front are  " << xatz0f << " " << yatz0f << endl;
                 cout << "Calculated x and y rear are  " << xatz0r << " " << yatz0r << endl;
              }
           }

           theta_sc=thetasc;
           phi_sc=phisc;

           th_sc=fg*theta_sc;
           ph_sc=fg*phi_sc;
     
           xatz0f=xatz0f-350.0*tphif;
           yatz0f=yatz0f-350.0*tthetaf;
           xatz0r=xatz0r-350.0*tphir;
           yatz0r=yatz0r-350.0*tthetar;

           if(abs(theta_sc) <= 1.0 && abs(phi_sc) <= 1.0){
              term1=(xatz0r-xatz0f)*(tphir-tphif);
              term2=(yatz0r-yatz0f)*(tthetar-tthetaf);
              term3=pow((tphir-tphif),2);
              term4=pow((tthetar-tthetaf),2);
              if((term3+term4) != 0) {
                 zclose_old=-(term1+term2)/(term3+term4);
              }
              else {
                 zclose_old=kBig;
              }
              term5=(xatz0r-xatz0f+(tphir-tphif)*zclose_old);
              term6=(yatz0r-yatz0f+(tthetar-tthetaf)*zclose_old);
              sclose_old=sqrt(pow(term5,2)+pow(term6,2));
           }
           else {
              zclose_old=0.;
              sclose_old=0.;
           }

// The correct method to determine sclose and zclose
// (see R.D. Ransome, Ph.D. thesis, Appendix A)

           a0 = pow((xatz0r-xatz0f),2) + pow((yatz0r-yatz0f),2);
           b1 = pow(tphif,2) + pow(tthetaf,2) + 1.0;
           b2 = pow(tphir,2) + pow(tthetar,2) + 1.0;
           c1 = (xatz0r-xatz0f)*tphif + (yatz0r-yatz0f)*tthetaf;
           c2 = (xatz0r-xatz0f)*tphir + (yatz0r-yatz0f)*tthetar;
           c3 = tphif*tphir + tthetaf*tthetar + 1.0;
           lambda1 =  (c1*b2 - c2*c3) / (b1*b2 - c3*c3);
           lambda2 =  (c1*c3 - c2*b1) / (b1*b2 - c3*c3);
           zclose = 0.5 * (lambda1 + lambda2);
           sclose = sqrt(a0 + pow(lambda1,2) * b1 + pow(lambda2,2) * b2
            +2.0*(-lambda1*c1 + lambda2*c2 - lambda1*lambda2*c3));

// Another method of changing to spherical coordinates

           psi_sc=atan(tan(phi_sc)*cos(theta_sc));
           x_sc=sin(psi_sc);
           y_sc=cos(psi_sc)*sin(theta_sc);
           z_sc=cos(psi_sc)*cos(theta_sc);
           r_sc=sqrt(pow(x_sc,2)+pow(y_sc,2));
           if(z_sc != 0. && y_sc != 0.) {
              theta_az=atan(r_sc/z_sc);
              phi_az=atan(x_sc/y_sc);
           }
           else {
              theta_az = -1000.;
              phi_az = -1000.;
              cout << "Zero problem with zsc or ysc" << endl;
           }
           theta_az=theta_az*fg;
           phi_az=phi_az*fg;
           if(y_sc < 0.0) {phi_az=phi_az+180.0;}
           if(y_sc >= 0.0 && x_sc < 0.0) {phi_az=phi_az+360.0;}

           theta_sc=theta_sc*fg;
           phi_sc=phi_sc*fg;

           if (fDebug > 0) {
              cout << "Calculated azimuthal angles are  " << theta_az << " " << phi_az << endl;
              cout << "Vertex coordinates zclose sclose =  " << zclose << " " << sclose << endl;
           }
/*
//   KW conetest
// mkj change conetest z_fpp[14] is rear ch3 RU1 (replaced 408)
//                     z_fpp[20] is rear ch4 RU3 (replaced 448)

           xf3=tphif*fZfpp[14]+xatz0f;
           yf3=tthetaf*fZfpp[14]+yatz0f;
           xr3=tphir*fZfpp[14]+xatz0r;
           yr3=tthetar*fZfpp[14]+yatz0r;
           xf4=tphif*fZfpp[20]+xatz0f;
           yf4=tthetaf*fZfpp[20]+yatz0f;
           xr4=tphir*fZfpp[20]+xatz0r;
           yr4=tthetar*fZfpp[20]+yatz0r;
           if (mkjtrack) {
             xf3=tphif*(fZmid[14]+fZshift[14])+xatz0f;
             yf3=tthetaf*(fZmid[14]+fZshift[14])+yatz0f;
             xr3=tphir*(fZmid[14]+fZshift[14])+xatz0r;
             yr3=tthetar*(fZmid[14]+fZshift[14])+yatz0r;
             xf4=tphif*(fZmid[20]+fZshift[20])+xatz0f;
             yf4=tthetaf*(fZmid[20]+fZshift[20])+yatz0f;
             xr4=tphir*(fZmid[20]+fZshift[20])+xatz0r;
             yr4=tthetar*(fZmid[20]+fZshift[20])+yatz0r;
           }
           dcone3=sqrt(pow((xf3-xr3),2)+pow((yf3-yr3),2));
           dcone4=sqrt(pow((xf4-xr4),2)+pow((yf4-yr4),2));

           if ( (xf3+dcone3) < 134. && (xf3+dcone3) > -134. &&
                (xf4+dcone4) < 149. && (xf4+dcone4) > -149. &&
                (xf3-dcone3) < 134. && (xf3-dcone3) > -134. &&
                (xf4-dcone4) < 149. && (xf4-dcone4) > -149. &&
                (yf3+dcone3) < 62.  && (yf3+dcone3) > -62.  && 
                (yf3-dcone3) < 62.  && (yf3-dcone3) > -62.  &&
                (yf4+dcone4) < 72.  && (yf4+dcone4) > -72.  && 
                (yf4-dcone4) < 72.  && (yf4-dcone4) > -72) {
                conetest = 1;
           }
           else {
             conetest = -1;
           }*/
//?? planes (15 and 21) or (17 and 23) ???
//   KW conetest

         xf3=tphif*fZfpp[17]+xatz0f;
         yf3=tthetaf*fZfpp[17]+yatz0f;
         xr3=tphir*fZfpp[17]+xatz0r;
         yr3=tthetar*fZfpp[17]+yatz0r;
         xf4=tphif*fZfpp[23]+xatz0f;
         yf4=tthetaf*fZfpp[23]+yatz0f;
         xr4=tphir*fZfpp[23]+xatz0r;
         yr4=tthetar*fZfpp[23]+yatz0r;

         dcone3=sqrt(pow((xf3-xr3),2)+pow((yf3-yr3),2));
         dcone4=sqrt(pow((xf4-xr4),2)+pow((yf4-yr4),2));

      ttheta=tan(theta_az*3.14159265/180.0);
      zback=fZfpp[17];
      r1x3 = (zback-zclose)*(tphif + (ttheta-tphif)/(1.0+ttheta*tphif));
      r2x3 = (zback-zclose)*((ttheta+tphif)/(1.0-ttheta*tphif) - tphif);
      r1y3 = (zback-zclose)*(tthetaf +(ttheta-tthetaf)/(1.0+ttheta*tthetaf));
      r2y3 = (zback-zclose)*((ttheta+tthetaf)/(1.0-ttheta*tthetaf)- tthetaf);

      zback=fZfpp[23];
      r1x4 = (zback-zclose)*(tphif + (ttheta-tphif)/(1.0+ttheta*tphif));
      r2x4 = (zback-zclose)*((ttheta+tphif)/(1.0-ttheta*tphif) - tphif);
      r1y4 = (zback-zclose)*(tthetaf +(ttheta-tthetaf)/(1.0+ttheta*tthetaf));
      r2y4 = (zback-zclose)*((ttheta+tthetaf)/(1.0-ttheta*tthetaf)- tthetaf);

       inside3=true;
       inside4=true;
       xpt3[0]=xf3+r1x3;
       xpt3[1]=xf3+r1x3/sqrt(2.);
       xpt3[2]=xf3;
       xpt3[3]=xf3-r2x3/sqrt(2.);
       xpt3[4]=xf3-r2x3;
       xpt3[5]=xf3-r2x3/sqrt(2.);
       xpt3[6]=xf3;
       xpt3[7]=xf3+r1y3/sqrt(2.);
       ypt3[0]=yf3;
       ypt3[1]=yf3+r1y3/sqrt(2.);
       ypt3[2]=yf3+r1y3;
       ypt3[3]=yf3+r1y3/sqrt(2.);
       ypt3[4]=yf3;
       ypt3[5]=yf3-r2y3/sqrt(2.);
       ypt3[6]=yf3-r2y3;
       ypt3[7]=yf3-r2y3/sqrt(2.);

       xpt4[0]=xf4+r1x4;
       xpt4[1]=xf4+r1x4/sqrt(2.);
       xpt4[2]=xf4;
       xpt4[3]=xf4-r2x4/sqrt(2.);
       xpt4[4]=xf4-r2x4;
       xpt4[5]=xf4-r2x4/sqrt(2.);
       xpt4[6]=xf4;
       xpt4[7]=xf4+r1y4/sqrt(2.);
       ypt4[0]=yf4;
       ypt4[1]=yf4+r1y4/sqrt(2.);
       ypt4[2]=yf4+r1y4;
       ypt4[3]=yf4+r1y4/sqrt(2.);
       ypt4[4]=yf4;
       ypt4[5]=yf4-r2y4/sqrt(2.);
       ypt4[6]=yf4-r2y4;
       ypt4[7]=yf4-r2y4/sqrt(2.);

       for(Int_t k=0; k < 8; k++){
          j=1;
          oddnodes3=-1;
                                                                               
          for(Int_t i=0; i < nsides3; i++){
            j=j+1;
            if(j == (nsides3+1)) {j=1;}
            if(((ypoly3[i] < ypt3[k])&&(ypoly3[j-1]>=ypt3[k]))||
             ((ypoly3[j-1] < ypt3[k])&&(ypoly3[i] >= ypt3[k]))){
                  if((xpoly3[i]+((ypt3[k]-ypoly3[i])/(ypoly3[j-1]-ypoly3[i])*
                    (xpoly3[j-1]-xpoly3[i]))) < xpt3[k]){
                          oddnodes3= -1*oddnodes3;
                  }
            }
          }
                                                                                
          if(oddnodes3 == -1) {
             inside3=false;
          }

          j=1;
          oddnodes4=-1;

          for(Int_t i=0; i < nsides4; i++){
             j=j+1;
             if(j == (nsides4+1)){j=1;}
             if(((ypoly4[i] < ypt4[k])&&(ypoly4[j-1]>=ypt4[k]))||
               ((ypoly4[j-1] < ypt4[k])&&(ypoly4[i]>=ypt4[k]))) {
                  if((xpoly4[i]+((ypt4[k]-ypoly4[i])/(ypoly4[j-1]-ypoly4[i])*
                     (xpoly4[j-1]-xpoly4[i]))) < xpt4[k]){
                     oddnodes4=-1*oddnodes4;
                  }
              }
          }
                                                                                
        if(oddnodes4 == -1){
	  inside4=false;
        }
     }


// old numbers were +/- 134 x +/- 62 for chamber 3
// and +/- 149 x +/- 72 for chamber 4
// decided to put a 10cm "border" around the outside, to be conservative

    if (inside3 && inside4){
        coneteste = 1;
    }
    else {
        coneteste = -1;
    }
         if (   (xf3+dcone3) < 134. && (xf3+dcone3) > -134. 
             && (xf4+dcone4) < 149. && (xf4+dcone4) > -149.
             && (xf3-dcone3) < 134. && (xf3-dcone3) > -134.
             && (xf4-dcone4) < 149. && (xf4-dcone4) > -149.
             && (yf3+dcone3) < 62.  && (yf3+dcone3) > -62.  
             && (yf3-dcone3) < 62.  && (yf3-dcone3) > -62.
             && (yf4+dcone4) < 72.  && (yf4+dcone4) > -72. 
             && (yf4-dcone4) < 72.  && (yf4-dcone4) > -72){
             conetest = 1;
         }
         else {
            conetest = -1;
         }

	if((coneteste!=conetest)&&fDebug>0) {
           cout << "Conetest EJB: " << coneteste << endl;
	   cout << "Conetest KW: " << conetest << endl;
	}

// Residual and Efficiency Calculations !!!!!!!!!!!!

// Note that all calculations are done with the raw angle and 
// position values before alignment corrections.
	int kkgroup[25]={0,1,1,1,2,2,2,1,1,1,2,2,2,2,2,1,1,3,3,2,2,2,1,1,1};
         if (!mkjtrack){
           for(Int_t k =0; k < 24; k++) {
              Int_t kk = k+1;
	      /*            if(kk == 1 || kk == 2 || kk == 3) {igroup=1;}
              if(kk == 4 || kk == 5 || kk == 6) {igroup=2;}
              if(kk == 7 || kk == 8 || kk == 9) {igroup=1;}
              if(kk == 10 || kk == 11 || kk == 12) {igroup=2;}
              if(kk == 13 || kk == 14) {igroup=2;}
              if(kk == 15 || kk == 16) {igroup=1;}
              if(kk == 17 || kk == 18) {igroup=3;}
              if(kk == 19 || kk == 20 || kk == 21) {igroup=2;}
              if(kk == 22 || kk == 23 || kk == 24) {igroup=1;}
              else{igroup = -1;}*/
              igroup=kkgroup[kk];
              z[k]=fZfpp[k];
	      //cout<<"group: "<<igroup<<endl;
              if(kk <= 12) {
		xtr[k]=xfaold+z[k]*pfold;
                ytr[k]=yfaold+z[k]*tfold;
		//cout<<k<<" xtr "<<xtr[k]<<" ytr:"<<ytr[k]<<endl;
		strw_rad=fWspacingf/2.0;
		if(igroup == 1){
                    uvxtr[k]=(ytr[k]-xtr[k])/1.414213562;
		}
                 else {
                    uvxtr[k]=(ytr[k]+xtr[k])/1.414213562;
		 }
              }
              else {
		xtr[k]=xrold+(z[k])*prold;
                ytr[k]=yrold+(z[k])*trold;
		strw_rad=fWspacingr/2.0;
                 if(igroup == 1) {
                    uvxtr[k]=(ytr[k]-xtr[k])/1.414213562;
                 }
                 else if(igroup == 2){
                    uvxtr[k]=(ytr[k]+xtr[k])/1.414213562;
                 }
                 else {
                    uvxtr[k]=xtr[k];
                 }
              }
	      
              straw_pred[k]=0;
              nstraws=fPlane[k]->GetNWire();
              uvx0=fPlane[k]->GetUVX0()*100.;
	      for(Int_t ii=1; ii < nstraws + 1; ii++) {
                 uvx_low=uvx0+2.*(strw_rad)*(ii-1.)-0.522; 
                 uvx_hi=uvx0+2.*(strw_rad)*(ii-1.)+0.522; 
                 if(uvxtr[k] >= uvx_low && uvxtr[k] <= uvx_hi) {
                    straw_pred[k]=ii;
                 }
              }
            
              itrgood[k]=0;
              for(Int_t ii=0; ii < ngoodhit[k]; ii++) {
		str=hits[k][ii][0];
		if(str == straw_pred[k]) {itrgood[k]=1;}
              }

              if(straw_pred[k]== 0) {itrgood[k]=2;}
              
              fPlane[k]->Setitrgd(itrgood[k]);
              fPlane[k]->Setstrwgd(straw_pred[k]);
              fPlane[k]->Setstrwbd(straw_pred[k]);
            
              for(Int_t ii=0; ii < ngoodhit[k]; ii++){
                 ddist=hits[k][ii][1];
                 drdist=ddist;
                 str=hits[k][ii][0];
                 zplane=hits[k][ii][3];
                 uvx=hits[k][ii][2];
                 if(igroup == 1) {
                    fac=1.0;
                    if(kk <= 12) {
                       a=pfold;
                       b=sqrt(2.)*uvx+xfaold;
                       c=tfold;
                       d=yfaold;
                    }
                    else {
                       a=prold;
                       b=sqrt(2.)*uvx+xrold;
                       c=trold;
                       d=yrold;
                    }
                    ee=zplane;
                 }
                 if(igroup == 2){
                    fac=1.0;
                    if(kk <= 12) {
                       a=-1.0*pfold;
                       b=sqrt(2.)*uvx-xfaold;
                       c=tfold;
                       d=yfaold;
                    }
                    else {
                       a=-1.0*prold;
                       b=sqrt(2.)*uvx-xrold;
                       c=trold;
                       d=yrold;
                    }
                    ee=zplane;
                 }
                 if(igroup == 3) {
                    fac=0.0;
                    a=prold;
                    b=-1.0*uvx+xrold;
                    c=trold;
                    d=yrold;
                    ee=zplane;
                 }
                 else {
                    fac = kBig;
                    a = kBig;
                    b = kBig;
                    c = kBig;
                    d = kBig;
                    ee = kBig;
                 }

                 if(kk == 27) {
                    cout << "****** Initial Residual Stats *******" << endl;
                    cout << ddist << " " << str << " " << zplane << " " << uvx << endl;
                    cout << tphia << " " << xinit << " " << tthetaa << " " << yinit << endl;
                    cout << xfaold << " " << yfaold << " " << xrold << " " << yrold << endl;
                    cout << a << " " << b << " " << c << " " << d << " " << ee << endl;
                    cout << "*************************************" << endl;
                 }
                 ff=-1.0*fac*b-1.0*d;
                 gg=(1.0+pow(a,2)+pow(c,2))/2.;
                 hh=a*b+c*d-ee;
                 iii=-1.0*fac*a-1.0*c;
                 jjj=(pow(b,2)+pow(d,2)+pow(ee,2))/2.;
 
// 1st iteration

                 ztr=zplane;
                 yw=-1.0*(ff+iii*ztr)/(1.0+pow(fac,2));
                 if(kk == 27) {cout << "1st pass - ywire = " << yw << endl;}
                 ztr=-0.5*(hh+iii*yw)/gg;
                 if(kk == 27) {cout << "1st pass - ztr = " << ztr << endl;}
 
// 2nd iteration

                 yw=-1.0*(ff+iii*ztr)/(1.0+pow(fac,2));
                 if(kk == 27) {cout << "2nd pass - ywire = " << yw << endl;}
                 ztr=-0.5*(hh+iii*yw)/gg;
                 if(kk == 27) {cout << "2nd pass - ztr = " << ztr << endl;}
     
// Now calculate distance of closest approach between wire hit
// and track.

                 t1=0.5*(1.0+pow(fac,2))*pow(yw,2);
                 t2=ff*yw;
                 t3=gg*pow(ztr,2);
                 t4=hh*ztr;
                 t5=iii*yw*ztr;
                 t6=jjj;
                 dd2by2=t1+t2+t3+t4+t5+t6;
                 dd2=dd2by2*2.;
                 if(kk == 27) {cout << "Distance squared = " << dd2 << endl;}
                 if(dd2 >= 0.) {
                    dd=sqrt(dd2);
                    resres=ddist-dd;
                 }
                 else {
                    dd=0.;
                    resres=0.49;
                 }
                 residual=resres;
                 if(kk == 27) {cout << "P D1 D2 Res= " << kk << " " << ddist << " " 
                              << dd << " " << resres << endl;}
                 THaFppHit* hit = fPlane[k]->GetGHit(ii);
                 hit->SetResidual(residual);
              }//end loop over hits
           }//end loop over planes
         }//if !mkjtrack

         th_az=theta_az;
         ph_az=phi_az;

         if(evdisplay && xinit != kBig) {
            ofstream evdsp("fpp_evdsp");
 //         evdsp << nr_interrupts << endl;
            evdsp << theta_az << " " << phi_az << " " << zclose << " " << sclose << endl;
            evdsp << (yinit-xinit)/1.4142 << " " << (yinit+xinit)/1.4142 << " "
                  << (thinit-phiinit)/1.4142 << " " << (phiinit+thinit)/1.4142 << endl;
            evdsp << ntr[0] << " " << ntr[1] << " " << ntr[2] << " " << ntr[3] << endl;
            for(Int_t i = 0; i < ntr[0]; i++){
               evdsp << track[i][0] << " " << track[i][1] << " " << track[i][11] << " ";
            } 
            cout << endl;
            for(Int_t i = 0; i < ntr[1]; i++) {
               evdsp << track[i][2] << " " << track[i][3] << " " << track[i][10] << " ";
            }
            cout << endl;
            for(Int_t i = 0; i < ntr[2]; i++) {
               evdsp << track[i][4] << " " << track[i][5] << " " << track[i][13] << " ";
            }
            cout << endl;
            for(Int_t i = 0; i < ntr[3]; i++) {
               evdsp << track[i][6] << " " << track[i][7] << " " << track[i][12] << " ";
            }
            cout << endl;
            for(Int_t i=0; i < 24; i++) {
               evdsp << ngoodhit[i] << " ";
            }
            cout << endl;
            for(Int_t j = 0; j < 24; j++){
              for(Int_t i = 0; i < ngoodhit[j]; i++){
                 evdsp << hits[j][i][1] << " " << hits[j][i][2] << " " << hits[j][i][3] << " ";
              }
              cout << endl;
            }
            evdsp.close();
         }
         if (fDebug > 0) {
            if ( zclose > 435. && zclose < 441. && theta_az > 4.5 && theta_az < 4.9 ) {
               ofstream debug("fpp_debug");
               debug << " Raw hits " << endl; 
               debug << "plane  #hit    WG  straw  ttdc  ltdc" << endl;
               for(Int_t j=0; j < 24; j++) {
                  for(Int_t i = 0; i < fPlane[j]->GetNHits(); i++) {
                      THaFppHit* hit = fPlane[j]->GetHit(i);
                      debug << j+1 << " " << i+1 << " " << hit->GetWire() << " "
                            << hit->GetStraw() << " " << hit->GetTEtime()/2.0 << " " 
                            << hit->GetLEtime()/2.0 << endl;
                  }
               }
               debug << " Good hits " << endl; 
               debug << "plane  #hit     WG  straw  letediff  drdist" << endl;
               for(Int_t jj=0; jj < 24; jj++){	
                  if (ngoodhit[jj] != fPlane[jj]->GetNGHits()){
                      debug << "nhits .ne. nghits" << endl;
                  } 
                  for(Int_t ii=0; ii < ngoodhit[jj]; ii++) {
                      THaFppHit* hit = fPlane[jj]->GetGHit(ii);
                      debug << jj+1 << " " << ii+1 << " " << hit->GetWire() << " " 
                            << hit->GetStraw() << " " << hit->GetWidth() << " "
                            << hit->GetDrftDist() << endl;
                  }
               }
            }
          }//end if debug 
      }//end if good tracking

      else {
//     cout << "Bad Tracking " << ntothits << endl;
       conetest=1000;
       x_fa=kBig;
       y_fa=kBig;
       x_fb=kBig;
       y_fb=kBig;
       x_r=kBig;
       y_r=kBig;
       x_x=kBig;
     
       th_f=kBig;
       ph_f=kBig;
       th_r=kBig;
       ph_r=kBig;
     
       x_diff_f=kBig;
       y_diff_f=kBig;
       x_diff_r=kBig;
       y_diff_r=kBig;
       x_diff_x=kBig;
       th_diff_f=kBig;
       ph_diff_f=kBig;
       th_diff_r=kBig;
       ph_diff_r=kBig;
     
       th_sc=kBig;
       ph_sc=kBig;
     
       for(Int_t k=0; k < 24; k++){
          fPlane[k]->Setitrgd(10000);
          fPlane[k]->Setstrwgd(10000);
          fPlane[k]->Setstrwbd(10000);
          for(Int_t ii=0; ii < ngoodhit[k]; ii++) {
             THaFppHit* hit = fPlane[k]->GetGHit(ii);
             hit->SetResidual(kBig);
          }
       }
     
       th_az=kBig;
       ph_az=kBig;
       zclose=kBig;
       sclose=kBig;
   
    }

    return 0;
  
}
//_____________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
ClassImp(THaFpp)

