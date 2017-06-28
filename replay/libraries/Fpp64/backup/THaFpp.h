#ifndef ROOT_THaFpp
#define ROOT_THaFpp

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaFpp                                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaNonTrackingDetector.h"
#include "TMath.h"
#include <vector>
class TClonesArray;
class THaFppPlane;

#define FPP_NELEM 24
#define FPP_MAXHIT 35

//const Double_t degrad = TMath::Pi()/180.0;

class THaFpp : public THaNonTrackingDetector {

 friend class THaFppPlane;

public:
  THaFpp( const char* name, const char* description = "",
	  THaApparatus* a = NULL );

  virtual ~THaFpp();

  virtual Int_t Decode( const THaEvData& );
  virtual Int_t CoarseProcess( TClonesArray& tracks );
  virtual Int_t FineProcess( TClonesArray& tracks );
  virtual EStatus Init( const TDatime& date );
  virtual Int_t End(THaRunBase* run);
  //  virtual void Print(const Option_t* opt) const;

protected:
        THaFppPlane *fPlane[FPP_NELEM];    // Fpp planes
        Int_t fTNHits;
        Int_t fTNGHits;

        //Options: calibration, alignment or analysis run
	Int_t align;
        Int_t align_opt;
	Int_t calib;
	Int_t calib_opt;
	Int_t ana_front;
	Int_t ana_rear;
	Int_t pulser_fpp;
        Int_t evdisplay;
        Int_t mkjtrack;

	// Geometry for Fpp detector and carbon analyzer
	Double_t    fX;
	Double_t    fY;
	Double_t    fZ;
	Double_t    fdX;
	Double_t    fdY;
	Double_t    fdZ;
	Double_t    fAngle;
	Double_t    fZZ;
	Double_t    fCarbonX;   //Origin of carbon analyzer
	Double_t    fCarbonY;
	Double_t    fCarbonZ;
	Double_t    fCarbondX; //Size of carbon analyzer,  in 1/2 widths
	Double_t    fCarbondY;
	Double_t    fCarbondZ; 
	Double_t    fCarbZZ;  
 
	//Calibration quantities
        Double_t    fTOff[40][FPP_NELEM];
	Double_t    fTAdd[40][FPP_NELEM];
	Float_t     fDrftDstPara[FPP_NELEM][7]; // Drift distribution parameters
	Double_t    fM1Front[6][4];
	Double_t    fM1Rear[6][4];
	Float_t     fDemuxCut[FPP_NELEM][FPP_MAXHIT][9];
	Double_t    fPHits[FPP_NELEM][FPP_MAXHIT][8]; 
	Double_t    fPSum[FPP_NELEM][FPP_MAXHIT][8];
	Double_t    fPCentr[FPP_NELEM][FPP_MAXHIT][8]; 
	Double_t    fPWidth[FPP_NELEM][FPP_MAXHIT][8];   

	//Important Calculated quantities
	Double_t    fZcarbonmin;
	Double_t    fZcarbonmax;
	Double_t    fZcarbon;
        Double_t    fWSpacing[FPP_NELEM];
	Double_t    fWspacingr; 
	Double_t    fWspacingf; 
	Double_t    fZfpp[FPP_NELEM];
	Double_t    fZmid[FPP_NELEM];
        Double_t    fUVX_first[FPP_NELEM];
	Double_t    fUVXzero[FPP_NELEM];
        Double_t    fZshift[FPP_NELEM];
        Double_t    fUVX_shift[FPP_NELEM];
	Double_t    fTHits[40][FPP_NELEM];
	Double_t    fTSum[40][FPP_NELEM];
        Short_t     fTHist[80][40][FPP_NELEM];
	Double_t    fT0[40][FPP_NELEM];
	Double_t    fUVrot[FPP_NELEM]; 
	Double_t    fZUrot[FPP_NELEM];
	Double_t    fZVrot[FPP_NELEM];
	Double_t    fUVno[FPP_NELEM];

/*	Double_t    fXofff[5];
	Double_t    fYofff[5];
	Double_t    fThofff[5];
	Double_t    fPhofff[5];
	Double_t    fPsofff[5];*/

      
        Double_t       fHits[5][FPP_MAXHIT][FPP_NELEM];
        Double_t       fTrack[40][25];
        Int_t       fNGHits[FPP_NELEM];
        Int_t       fNHits[25];  

        Int_t          ntrackfu;       //# of tracks for front U ch.
        Int_t          ntrackfv;       //# of tracks for front V ch.
        Int_t          ntrackrv;       //# of tracks for rear V ch.
        Int_t          ntrackru;       //# of tracks for rear U ch.
        Int_t          conetest;        // +1 if passed, -1 if failed conetest
        Int_t          coneteste;      // Ed Brash's new conetest
        Int_t          coneteste_frvdc;      // conetest front vs vdc
        Int_t          coneteste_cfrvdc;      // conetest crude front vs vdc
	Double_t       x_fa;           // x at z=0 from FPP front chambers
        Double_t       y_fa;           // y at z=0 from FPP front chambers
        Double_t       x_fb;           // x at z=.350 from FPP front chambers
        Double_t       y_fb;           // y at z=.350 from FPP front chambers
        Double_t       th_f;           // theta from FPP front chambers
        Double_t       ph_f;           // phi from FPP front chambers
        Double_t       x_r;            // x at z=.350 from FPP rear chambers
        Double_t       y_r;            // y at z=.350 from FPP rear chambers
        Double_t       th_r;           // theta from FPP rear chambers
        Double_t       ph_r;           // phi from FPP rear chambers
        Double_t       x_x;            // x at z=.350 from FPP X-Straws
        Double_t       x_diff_f;       // diff in X (VDC vs. FPP Front Chs.)
        Double_t       y_diff_f;       // diff in Y (VDC vs. FPP Front Chs.)
        Double_t       x_diff_r;       // diff in X (FPP Front vs. FPP Rear Chs.)
        Double_t       y_diff_r;       // diff in Y (FPP Front vs. FPP Rear Chs.)
        Double_t       x_diff_x;       // diff in X (FPP Front vs. FPP X straws)
        Double_t       th_diff_f;      // diff in Theta (VDC vs. FPP Front Chs.)
        Double_t       ph_diff_f;      // diff in Phi (VDC vs. FPP Front Chs.)
        Double_t       th_diff_r;      // diff in Theta (FPP Front vs. FPP Rear Chs.)
        Double_t       ph_diff_r;      // diff in Phi (FPP Front vs. FPP Rear Chs.)
        Double_t       th_sc;          // Theta scattered
        Double_t       ph_sc;          // Phi scattered
        Double_t       th_az;          // Theta scattered (polar coordinates)
        Double_t       ph_az;          // Phi scattered (polar coordinates)
        Double_t       th_az_frvdc;          // Theta scattered (polar coordinates) front vs vdc
        Double_t       ph_az_frvdc;          // Phi scattered (polar coordinates) front vs vdc
        Double_t       th_az_cfrvdc;          // Theta scattered (polar coordinates) crude front vs vdc
        Double_t       ph_az_cfrvdc;          // Phi scattered (polar coordinates) crude front vs vdc
        Double_t       zclose;         // Z of closest approach of front/rear tracks
        Double_t       sclose;         // distance of closest approach of f/r tracks
        Double_t       zclose_frvdc;         // Z of closest approach of  front vs vdc tracks
        Double_t       sclose_frvdc;         // distance of closest approach of front vs vdc tracks
        Double_t       zclose_cfrvdc;         // Z of closest approach of  crude front vs vdc tracks
        Double_t       sclose_cfrvdc;         // distance of closest approach of crude front vs vdc tracks
        Double_t       chisqfu;        // chisq for front chamber u track 
        Double_t       chisqfv;        // chisq for front chamber v track 
        Double_t       chisqru;        // chisq for rear  chamber u track 
        Double_t       chisqrv;        // chisq for rear  chamber v track 

        Int_t ReadDatabase( const TDatime& date );
        virtual Int_t DefineVariables( EMode mode = kDefine);
        virtual void  Clear( Option_t* opt="" )  {}

  ClassDef(THaFpp,0)             // Fpp class
}; 

////////////////////////////////////////////////////////////////////////////////

#endif
