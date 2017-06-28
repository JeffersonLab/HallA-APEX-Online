//*-- Author :    Barak Schmookler & Thir Gautam  12-01-15

//////////////////////////////////////////////////////////////////////////
//
// Gmp_Track_Eloss
//
// Correct energy loss of a track. For the liquid hydrogen target the
// correction will be vertex and angle dependent. Also, all the material
// after the target and before the spectrometer vacuum is included.
//
// The user can set a target Z offset while instantiating this class.
// However, this offset has a different meaning in the case of the hydrogen 
// target and the other targets. We also allow the user to set the length
// of the target (default is 15 cm). Since the amount of air between the
// scattering chamber and the spectrometer entrance differs for the LHRS
// and the RHRS, this can be set by the user.
//
// It is important to note that we assume the track originates from 
// (x,y) = (0,0). Also, we currently only include the material between
// the target and the spectrometer vacuum when using the liquid hydrogen
// target.
//
// We also want to briefly discuss what is currently implemented for the 
// track in SIMC. The energy of the track passing through the spectrometer
// is calculated using the vertex and track angle. However, the track energy
// at the reaction point is reconstructed assuming it originated from the
// center of the target (i.e. Z=0)
//////////////////////////////////////////////////////////////////////////

#include "Gmp_Track_Eloss.h"
#include "THaSpectrometer.h"
#include "THaTrack.h"
#include "THaTrackInfo.h"
#include "THaVertexModule.h"
#include "TMath.h"
#include "TVector3.h"
#include "Riostream.h"

using namespace std;

//_____________________________________________________________________________
Gmp_Track_Eloss::Gmp_Track_Eloss( const char* name, 
				  const char* description,
				  const char* input_tracks,
				  const char* vertex,
				  const Double_t t_length,
				  const Double_t z_offset,
				  const Double_t air_length,
				  Double_t particle_mass,
				  Int_t hadron_charge ) :
  THaElossCorrection(name,description,input_tracks,particle_mass,
		     hadron_charge), 
  fTLength(t_length), 
  fAirLength(air_length),
  fTrackModule(NULL)
{
  // Normal constructor.

  // Sets fExtPathMode = kTRUE, which allows the vertex 
  // module to be initialized. Also sets zref and the
  // scale factor.
  Double_t scale = 1.0 ; //Set scale factor to 1
  SetPathlength(vertex,z_offset,scale);

  Clear();
}

//_____________________________________________________________________________
Gmp_Track_Eloss::~Gmp_Track_Eloss()
{
  // Destructor

  DefineVariables( kDelete );
}

//_____________________________________________________________________________
void Gmp_Track_Eloss::CalcEloss( THaTrackInfo* trkifo )
{
  // Compute the expected energy loss for the track given in trkifo
  // as well as the vertex location.
  //

  Double_t p0 = trkifo->GetP();
  Double_t pZ = trkifo->GetPz();
  Double_t Z = fVertexModule->GetVertex().Z();
  Double_t beta = p0 / TMath::Sqrt(p0*p0 + fM*fM);

  //Set Material Parameter

  //Aluminum
  Double_t Z_Al = 13. ;
  Double_t A_Al = 26.98 ;
  Double_t rho_Al = 2.70 ;
  
  //Air
  Double_t Z_Air = 7.22000 ;
  Double_t A_Air = 14.46343 ;
  Double_t rho_Air = 1.20480E-03 ;

  //Kapton
  Double_t Z_Kap = 5.02564 ;
  Double_t A_Kap = 9.80345 ;
  Double_t rho_Kap = 1.42000 ;

  if( fElectronMode ) {
    
    if(fZmed < 1.01 && fAmed < 1.01){

      Double_t eloss_H2(0), eloss_Al1(0), eloss_Al2(0), eloss_Air(0), eloss_Kap(0) ;
      Double_t l_H2(0), l_Al1(0), l_Al2(0),l_Air,l_Kap(0) ;

      //Set Target Geometry
      Double_t theta = TMath::ACos(pZ/p0); //Scattering Angle (rad)
      Double_t sin2 = TMath::Power(TMath::Sin(theta),2);
      Double_t T_Al = 1.73E-4 ; // Aluminum Wall Thickness (m) 
      Double_t R = 0.0381 ; //1.5 inch
      Double_t R1 = R ; // Inner Radius for End-Cap
      Double_t R2 = R + 1.32E-4 ; // Outer Radius for End-Cap

      Double_t Z_cap = (fTLength/2. - R) + fZref;
      Double_t Z_rel = Z - Z_cap ;

      // Case 1
      if(Z_rel < 0 && (TMath::Abs(Z_rel * TMath::Tan(theta))) >= R){
	//cout << "Doing Case 1!!!"<<endl;

	l_H2 = ( R/TMath::Abs(TMath::Sin(theta)) );
	l_Al1 =( T_Al / TMath::Abs(TMath::Sin(theta)) ); 
      }
      // Case 2
      else if(Z_rel >= 0){
	//cout << "Doing Case 2!!!"<<endl;

	l_H2 = TMath::Sqrt( (R1*R1) - (Z_rel*Z_rel*sin2) ) - (Z_rel*TMath::Abs(TMath::Cos(theta))) ;
        l_Al1 = TMath::Sqrt( (R2*R2) - (Z_rel*Z_rel*sin2) ) - TMath::Sqrt( (R1*R1) - (Z_rel*Z_rel*sin2) ) ;
      }
      // Case 3
      else if(Z_rel < 0 && (TMath::Abs(Z_rel * TMath::Tan(theta))) < R){
	//cout << "Doing Case 3!!!"<<endl;
	
	l_H2 = TMath::Sqrt( (R1*R1) - (TMath::Abs(Z_rel*Z_rel)*sin2) ) + (TMath::Abs(Z_rel)*TMath::Abs(TMath::Cos(theta))) ;
	l_Al1 = TMath::Sqrt( (R2*R2) - (TMath::Abs(Z_rel*Z_rel)*sin2) ) - TMath::Sqrt( (R1*R1) - (TMath::Abs(Z_rel*Z_rel)*sin2) ) ;
      } 
      
      // Hydrogen Target
      eloss_H2 = ElossElectron( beta, fZmed, fAmed, 
				fDensity, l_H2 );

      // Aluminum Target Wall 
      eloss_Al1 = ElossElectron( beta, Z_Al, A_Al, 
				 rho_Al, l_Al1 );
      
      // Aluminum Scattering Chamber Exit Window
      l_Al2 = 4.064E-4 ; // 16 mil (i.e. 0.0016")
      
      eloss_Al2 = ElossElectron( beta, Z_Al, A_Al, 
				 rho_Al, l_Al2 );
      
      // Air between Scattering Chamber and Spectrometer
      l_Air = fAirLength ; // 13.33" (RHRS), 10.62" (LHRS)
      eloss_Air = ElossElectron( beta, Z_Air, A_Air, 
				rho_Air, l_Air );
      
      // Kapton window at Spectrometer Entrance
      l_Kap = 3.048E-4 ; // 12 mil (i.e. 0.0012")

      eloss_Kap = ElossElectron( beta, Z_Kap, A_Kap, 
				 rho_Kap, l_Kap );

      // Calculate Total Eloss
      fPathlength = l_H2; // Set as pathlength through hydrogen...not really important
      fEloss = eloss_H2 + eloss_Al1 + eloss_Al2 + eloss_Air + eloss_Kap ;

    } else{
      fEloss = ElossElectron( beta, fZmed, fAmed, 
			      fDensity, fPathlength );
    }
  } else {
    fEloss = ElossHadron( fZ, beta, fZmed, fAmed, 
			  fDensity, fPathlength );
  }
}

//_____________________________________________________________________________
void Gmp_Track_Eloss::Clear( Option_t* opt )
{
  // Clear all event-by-event variables.
  
  THaElossCorrection::Clear(opt);
  TrkIfoClear();
}

//_____________________________________________________________________________
THaAnalysisObject::EStatus Gmp_Track_Eloss::Init( const TDatime& run_time )
{
  // Initialize the module.
  // Locate the input tracking module named in fInputName and save
  // pointer to it.

  static const char* const here = "Init()";

  // Find the input tracking module
  fTrackModule = dynamic_cast<THaTrackingModule*>
    ( FindModule( fInputName.Data(), "THaTrackingModule"));
  if( !fTrackModule )
    return fStatus;

  // Get the parent spectrometer apparatus from the input module  
  THaSpectrometer* spectro = fTrackModule->GetTrackInfo()->GetSpectrometer();
  if( !spectro ) {
    Error( Here(here), "Oops. Input tracking module has no pointer "
	   "to a spectrometer?!?" );
    return fStatus = kInitError;
  }
  // Needed for initialization of dependent modules in a chain
  fTrkIfo.SetSpectrometer( spectro );

  // Standard initialization. Calls this object's DefineVariables() and
  // reads meterial properties from the run database.
  THaElossCorrection::Init( run_time );

  return fStatus;
}

//_____________________________________________________________________________
Int_t Gmp_Track_Eloss::DefineVariables( EMode mode )
{
  // Define/delete global variables.

  if( mode == kDefine && fIsSetup ) return kOK;
  THaElossCorrection::DefineVariables( mode );

  return DefineVarsFromList( THaTrackingModule::GetRVarDef(), mode );
}

//_____________________________________________________________________________
Int_t Gmp_Track_Eloss::Process( const THaEvData& )
{
  // Calculate corrections and adjust the track parameters.
  
  if( !IsOK() ) return -1;
  
  THaTrackInfo* trkifo = fTrackModule->GetTrackInfo();
  if( !trkifo->IsOK() ) return 2;

  // Copy the input track info
  fTrkIfo = *trkifo;
  
  // Compute the correction
  Double_t p_out = fTrkIfo.GetP(); 
  if( p_out <= 0.0 ) return 4; //oops
  Double_t E_out = TMath::Sqrt(p_out*p_out + fM*fM);
  
  if( !fTestMode ) {
    // calculate pathlength for this event if we have a vertex module
    // For electron tracks, this is only used for solid target. It may
    // not be very useful because there is no angle dependence.
    // Also note that fZref here means the vertex value where the
    // pathlength is zero; while for the hydrogen target, fZref means
    // how much the target is shifted along Z
    if( fExtPathMode ) {
      if( !fVertexModule->HasVertex() )
	return 1;
      fPathlength = 
	TMath::Abs(fVertexModule->GetVertex().Z() - fZref) * fScale;
    }
    //update fEloss
    CalcEloss(trkifo); 
  }
  Double_t p_in = TMath::Sqrt(p_out*p_out + fEloss*fEloss + 2.0*E_out*fEloss);
  
  // Apply the correction
  fTrkIfo.SetP(p_in);

  fDataValid = true;
  return 0;
}
