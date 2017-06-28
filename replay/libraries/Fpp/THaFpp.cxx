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
#include "THaTrackProj.h"
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

//_____________________________________________________________________________
THaFpp::THaFpp( const char* name, const char* description,
			   THaApparatus* apparatus ) :
THaNonTrackingDetector(name,description,apparatus)
{
	// Constructor
  //fDebug=3;

  fTrackProj = new TClonesArray( "THaTrackProj", 5 );
 
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
  //  fpplast_(&fTHits[0][0],&fTSum[0][0],&fTHist[0][0][0],&fT0[0][0]
  //                 ,calib,calib_opt,fDebug);

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

	//K.Allada
	// Define Detector X,Y,Z coord (m)
	// Define rotation angle 
	Float_t kx,ky,kz;
	Float_t angle;
	const Double_t degrad = TMath::Pi()/180.0;

	kx = 0;
	ky = 0;
	kz = 1.9; // meters
	angle = 45; //degrees
	fOrigin.SetXYZ( kx, ky, kz );
	DefineAxes(angle*degrad);

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
         {"trx",    "x-position of track in det plane",  "fTrackProj.THaTrackProj.fX" },
	 {"try",    "y-position of track in det plane",  "fTrackProj.THaTrackProj.fY" },
  	 {"trpath", "TRCS pathlen of track to det plane","fTrackProj.THaTrackProj.fPathl" },
         {"THits", "Total number of hits per event", "fTNHits"},
         {"TGHits", "Total number of Good hits per event", "fTNGHits"},
         {"ntrackfu","# of tracks for front U ch.","ntrackfu"},
         {"ntrackfv","# of tracks for front V ch.","ntrackfv"},
         {"ntrackrv","# of tracks for rear V ch.","ntrackfv"},
         {"ntrackru","# of tracks for rear U ch.","ntrackru"},
         {"conetest","+1 if passed -1 if failed conetest","conetest"},
         {"coneteste","E. Brash new Conetest, +1 pass, -1 fail","coneteste"},
         {"x_fa",    "x at z=100 from FPP front chambers", "x_fa"},
         {"y_fa",    "y at z=100 from FPP front chambers", "y_fa"},
         {"x_fb",    "x at z=350 from FPP front chambers", "x_fb"},
         {"y_fb",    "y at z=350 from FPP front chambers", "y_fb"},
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
         {"th_az_frvdc",    "Theta scattered (polar coordinates) vdc/front tracks", "th_az_frvdc"},
         {"ph_az_frvdc",    "Phi scattered (polar coordinates) vdc/front tracks "  , "ph_az_frvdc"},
         {"zclose_frvdc",   "Z of closest approach of vdc/front tracks", "zclose_frvdc"},
         {"sclose_frvdc",   "distance of closest approach of vdc/front tracks", "sclose_frvdc"},
         {"coneteste_frvdc",   "conetest of vdc/front tracks", "coneteste_frvdc"},
         {"th_az_cfrvdc",    "Theta scattered (polar coordinates) vdc/crude front tracks", "th_az_cfrvdc"},
         {"ph_az_cfrvdc",    "Phi scattered (polar coordinates) vdc/crude front tracks "  , "ph_az_cfrvdc"},
         {"zclose_cfrvdc",   "Z of closest approach of vdc/crude front tracks", "zclose_cfrvdc"},
         {"sclose_cfrvdc",   "distance of closest approach of vdc/crude front tracks", "sclose_cfrvdc"},
         {"coneteste_cfrvdc",   "conetest of vdc/crude front tracks", "coneteste_cfrvdc"},
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
  //--added by K. Allada 9th Jan 2014 -----------------------------
  //  Reconstruct coordinates of where a particle track crosses
  //  the Fpp plane, and copy the point into the fTrackProj array.
  
  //  Calculation of coordinates of particle track cross point with Fpp
  //  plane in the detector coordinate system. For this, parameters of track 
  //  reconstructed in THaVDC::CrudeTrack() are used.

  int n_track = tracks.GetLast()+1;   // Number of reconstructed tracks

  for ( int i=0; i<n_track; i++ ) {
    THaTrack* theTrack = static_cast<THaTrack*>( tracks[i] );
    Double_t pathl=kBig, xc=kBig, yc=kBig;
    Double_t dx=0.; // unused
    Int_t pad=-1;   // unused
    
    CalcTrackIntercept(theTrack, pathl, xc, yc);
    // if it hit or not, store the information (defaults if no hit)
    new ( (*fTrackProj)[i] ) THaTrackProj(xc,yc,pathl,dx,pad,this);
  }
  //  --------------------------------------------------------------

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
        Double_t  xhat1f,yhat1f,zhat1f;        
        Double_t jjj,dd2by2,dd2,dd,resres,fac,residual;
        Double_t t1,t2,t3,t4,t5,t6,a0, b1, b2, c1, c2, c3, lambda1, lambda2,fg;
        Double_t xfaold,xrold,yfaold,yrold,xfbold,yfbold,tfold,trold,pfold,prold;
        Double_t psif,psir,xhatr,yhatr,zhatr;
	Double_t psivdc,xhatf,yhatf,zhatf;
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
        Double_t   vang_cr,vint_cr,uang_cr,uint_cr;
        Double_t   chis[25];

        Double_t   xatz0f,yatz0f,xatz0r,yatz0r,xatz0x;
	Double_t    temp_x,temp_y,temp_t,temp_p;
        Double_t   xatz0f_cr,yatz0f_cr;
        Double_t   xtr[24],ytr[24],uvxtr[24],z[24];
        Double_t   uvx_low,uvx_hi,strw_rad,uvx0;
        Int_t      straw_pred[24],nstraws,itrgood[24];
        Double_t   tthetaf,tphif,tthetaa,tphia,thinitd,phiinitd;
        Double_t   tthetaf_cr,tphif_cr;
        Double_t   tthetar,tphir,theta_sc,phi_sc,theta_az,phi_az;
	Double_t   theta_sc_frvdc,phi_sc_frvdc,theta_az_frvdc,phi_az_frvdc;
        Double_t   theta_sc_cfrvdc,phi_sc_cfrvdc,theta_az_cfrvdc,phi_az_cfrvdc;
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
        Double_t xpt1[8],ypt1[8],xpt2[8],ypt2[8];
        Int_t oddnodes3,oddnodes4, j;
        Bool_t inside3,inside4;
        Int_t oddnodes1,oddnodes2;
        Bool_t inside1,inside2;

	Double_t xpoly1[4] = {-104.5,104.5,104.5,-104.5};
        Double_t ypoly1[4] = {30.0,30.,-30,-30.};
        Int_t nsides1 = 4;

        Double_t xpoly2[4] = {-104.5,104.5,104.5,-104.5};
        Double_t ypoly2[4] = {30.0,30.,-30,-30.};
        Int_t nsides2 = 4;

        Double_t xpoly3[6] = {-110.0,-87.0,110.0,110.0,87.0,-110.0};
        Double_t ypoly3[6] = {33.0,55.0,55.0,-33.0,-55.0,-55.0};
        Int_t nsides3 = 6;

        Double_t xpoly4[8] = {-140.0,-72.0,104.0,140.0,140.0,72.0,-104.0,-140.0};
        Double_t ypoly4[8] = {2.0,70.0,70.0,38.0,-2.0,-70.0,-70.0,-38.0};
        Int_t nsides4 = 8;

	Int_t n_track = tracks.GetLast()+1; // Number of reconstructed tracks

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
		     << yinit << " dydz = " << thinit << " dxdz = " << phiinit << " n_track =  " << n_track << "\n";
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
	//	for(Int_t i=0; i <  FPP_NELEM; i++){
	for(Int_t i=0; i <  6; i++){
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
			//cout<<"dtime: "<<dtime<<" "<<iwiregroup<<" "<<i<<" TOff: "<<fTOff[iwiregroup][i]<<endl;

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
			  //  cout << "dtime: " << dtime << " drift dist: " << drdist << endl; 
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
			      // cout << i << " " << j << " " << fTNGHits << endl;
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

	//------ Added by K.A Jan 9th 2014 ------
	// track projection on the detector
	fTrackProj->Clear();
	for ( int i=0; i<n_track; i++ ) {
	  THaTrack* theTrack = static_cast<THaTrack*>( tracks[i] );
	  Double_t pathl=kBig, xc=kBig, yc=kBig;
	  Double_t dx=0.; // unused
	  Int_t pad=-1;   // unused
	  
	  CalcTrackIntercept(theTrack, pathl, xc, yc);
	  //	  cout << "xc: " << xc << " yc: " << yc << " pathl: " << pathl << " n_track =  " << n_track << endl;
 	  // if it hit or not, store the information (defaults if no hit)
 	  new ( (*fTrackProj)[i] ) THaTrackProj(xc,yc,pathl,dx,pad,this);
	}
	// ----------------------------------------------	

      return 0;
      
}
//_____________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
ClassImp(THaFpp)

