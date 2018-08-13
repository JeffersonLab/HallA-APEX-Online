//--- Adapted from the Gmp equivalent written by Barak Schmookler & Thir Gautam  12-01-15
//--- by Reynier Cruz Torres for the Tritium experiments.
//--- Revision History: 
//    1) Barak Schmookler, August 2016: Change from loop 2 to loop 3. Loop
//       3 was used in Spring 2016.
//    2) Longwu Ou, October 2016: Now the target geometry is determined by a
//       tag "<fprefix>.config" in the run database.
//    3) Reynier Cruz Torres, May 2018: Adapted code for the specifics
//       of the Tritium gas targets.
//
//////////////////////////////////////////////////////////////////////////
//
// Tri_Track_Eloss
//
// Correct energy loss of a track. For the gas target the
// correction will be vertex and angle dependent. Also, all the material
// after the target and before the spectrometer vacuum is included.
//
// The user can set a target Z offset while instantiating this class.
// However, this offset has a different meaning in the case of the gas
// target and the other targets. We also allow the user to set the length
// of the target (default is 25 cm). Since the amount of air between the
// scattering chamber and the spectrometer entrance differs for the LHRS
// and the RHRS, this can be set by the user.
//
// It is important to note that we assume the track originates from 
// (x,y) = (0,0). Also, we currently only include the material between
// the target and the spectrometer vacuum when using the gas target.
//////////////////////////////////////////////////////////////////////////

#include "Tri_Track_Eloss.h"
#include "THaSpectrometer.h"
#include "THaTrack.h"
#include "THaTrackInfo.h"
#include "THaVertexModule.h"
#include "TMath.h"
#include "TVector3.h"
#include "Riostream.h"

using namespace std;

//_____________________________________________________________________________
Tri_Track_Eloss::Tri_Track_Eloss( const char* name, 
		const char* description,
		const char* input_tracks,
		const char* vertex,
		const Double_t t_length,
		const Double_t z_offset,
		const Double_t air_length,
		Double_t particle_mass,
		Int_t hadron_charge ) :
	Tri_ElossCorrection(name,description,input_tracks,particle_mass,
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
Tri_Track_Eloss::~Tri_Track_Eloss()
{
	// Destructor

	DefineVariables( kDelete );
}

//_____________________________________________________________________________
void Tri_Track_Eloss::CalcEloss( THaTrackInfo* trkifo )
{
	// Compute the expected energy loss for the track given in trkifo
	// as well as the vertex location.

	Double_t p0 = trkifo->GetP();
	Double_t pZ = trkifo->GetPz();
	Double_t beta = p0 / TMath::Sqrt(p0*p0 + fM*fM);
	Double_t Z = fVertexModule->GetVertex().Z();

	// -------------------------------------------------------------
	//Set Material Parameter

	//Aluminum
	// It's actually Al 7075 (rho = 2.81 g/cc) for the target,
	// and Al 2024-T3 (rho = 2.78 g/cc) for the scattering chamber
	// exit. But, just use 2.80 g/cc here...
	Double_t Z_Al   = 13.   ;
	Double_t A_Al   = 26.98 ;
	Double_t rho_Al = 2.80  ;

	//Air
	Double_t Z_Air   = 7.22000     ;
	Double_t A_Air   = 14.46343    ;
	Double_t rho_Air = 1.20480E-03 ;

	//Kapton
	Double_t Z_Kap   = 5.02564 ;
	Double_t A_Kap   = 9.80345 ;
	Double_t rho_Kap = 1.42000 ;

	// -------------------------------------------------------------
	// Pathlength through materials

	// Aluminum Scattering Chamber Exit Window
	Double_t l_Al2 = 4.064E-4 ; // (i.e. 0.016")

	// Air between Scattering Chamber and Spectrometer
	Double_t l_Air = fAirLength ;
	// 13.95" (RHRS), 10.62" (LHRS) for Spring 16
	// 14.79" (RHRS), 15.23" (LHRS) for Fall 16             

	// Kapton window at Spectrometer Entrance
	Double_t l_Kap = 3.048E-4 ; // (i.e. 0.012")

	// -------------------------------------------------------------
	// Doing the energy loss calculation
	// -----------------------------------
	// Looks a bit dumb to have the "if" statements here, but if for
	// any reason we switch to the mean energy loss calculation
	// the equation will be different for protons and neutrons.

	Double_t eloss_Al2(0), eloss_Air(0), eloss_Kap(0);
	if( fElectronMode ) {
		eloss_Al2 = MostProbEloss( fZ, beta, Z_Al , A_Al , rho_Al  , l_Al2 ); // Aluminum Scattering Chamber Exit Window
		eloss_Air = MostProbEloss( fZ, beta, Z_Air, A_Air, rho_Air , l_Air ); // Air between Scattering Chamber and HRS
		eloss_Kap = MostProbEloss( fZ, beta, Z_Kap, A_Kap, rho_Kap , l_Kap ); // Kapton window at Spectrometer Entrance
	}
	else{
		eloss_Al2 = MostProbEloss( fZ, beta, Z_Al , A_Al , rho_Al  , l_Al2 ); // Aluminum Scattering Chamber Exit Window
		eloss_Air = MostProbEloss( fZ, beta, Z_Air, A_Air, rho_Air , l_Air ); // Air between Scattering Chamber and HRS
		eloss_Kap = MostProbEloss( fZ, beta, Z_Kap, A_Kap, rho_Kap , l_Kap ); // Kapton window at Spectrometer Entrance
	}

	fEloss = eloss_Al2 + eloss_Air + eloss_Kap;

	// -------------------------------------------------------------
	// Check if target corresponds to one of the gas cells
	if(fZmed < 3. && fAmed < 4.){	

		Double_t eloss_gas(0), eloss_Al1(0);
		Double_t l_gas(0), l_Al1(0);

		//Set Target Geometry
		Double_t R; // diameters is 0.77 inch
		Double_t Tip_Al = 0.32*1.e-3; //Tip thickness(m)

		Double_t theta = TMath::ACos(pZ/p0); //Scattering Angle (rad)
		Double_t sin2 = TMath::Power(TMath::Sin(theta),2);

		// ---------------------------------------------------------------------------------------------
		// Thickness of the target side walls
		Double_t T_Al = 0; // Aluminum Wall Thickness (m) 

		if     ( fZmed>0.09 && fZmed<1.01 && fAmed>0.09 && fAmed<1.01 ) T_Al=(0.374+0.503)*1E-3/2.; // m - Hydrogen
		else if( fZmed>0.09 && fZmed<1.01 && fAmed>1.09 && fAmed<2.01 ) T_Al=(0.447+0.371)*1E-3/2.; // m - Deuterium
		else if( fZmed>0.09 && fZmed<1.01 && fAmed>2.09 && fAmed<3.01 ) T_Al=(0.435+0.447)*1E-3/2.; // m - Tritium
		else if( fZmed>1.09 && fZmed<2.01 && fAmed>2.09 && fAmed<3.01 ) T_Al=(0.487+0.478)*1E-3/2.; // m - Helium-3
		else T_Al=(0.374+0.503+0.447+0.371+0.435+0.447+0.487+0.478)*1E-3/8.; // m - Average of the numbers above

		// These numbers come from the following document:
		// https://logbooks.jlab.org/files/2018/01/3514205/TGT-RPT-17-007.pdf
		// for each target I average the "mid left" and "mid right" values
		// ---------------------------------------------------------------------------------------------

		R = 0.0065; // meters

		Double_t Z_cap = (fTLength/2. - R) + fZref;
		Double_t Z_rel = Z - Z_cap ;

		if( Z < Z_cap ){
			l_gas = ( R/TMath::Abs(TMath::Sin(theta)) );
			l_Al1 =( T_Al / TMath::Abs(TMath::Sin(theta)) );
		}
		else{
			double R1 = R;
			double R2 = R + Tip_Al;
			l_gas = TMath::Sqrt( (R1*R1) - (Z_rel*Z_rel*sin2) ) - (Z_rel*TMath::Abs(TMath::Cos(theta))) ;
			l_Al1 = TMath::Sqrt( (R2*R2) - (Z_rel*Z_rel*sin2) ) - TMath::Sqrt( (R1*R1) - (Z_rel*Z_rel*sin2) ) ; 
		}

		// --------------------------------------------------------------
		// Calculate energy loss with the parameters determined above

		if( fElectronMode ) {			
			eloss_gas = MostProbEloss( fZ, beta, fZmed, fAmed, fDensity, l_gas ); // Gas Target
			eloss_Al1 = MostProbEloss( fZ, beta, Z_Al , A_Al , rho_Al  , l_Al1 ); // Aluminum Target Wall 
		}
		else{
			eloss_gas = MostProbEloss( fZ, beta, fZmed, fAmed, fDensity, l_gas ); // Gas Target
			eloss_Al1 = MostProbEloss( fZ, beta, Z_Al , A_Al , rho_Al  , l_Al1 ); // Aluminum Target Wall 
		}

		// Calculate Total Eloss
		fPathlength = l_gas; // Set as pathlength through gas...not really important
		fEloss += eloss_gas + eloss_Al1;

	}

}

//_____________________________________________________________________________
void Tri_Track_Eloss::Clear( Option_t* opt )
{
	// Clear all event-by-event variables.

	Tri_ElossCorrection::Clear(opt);
	TrkIfoClear();
}

//_____________________________________________________________________________
THaAnalysisObject::EStatus Tri_Track_Eloss::Init( const TDatime& run_time )
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
	Tri_ElossCorrection::Init( run_time );

	return fStatus;
}

//_____________________________________________________________________________
Int_t Tri_Track_Eloss::DefineVariables( EMode mode )
{
	// Define/delete global variables.

	if( mode == kDefine && fIsSetup ) return kOK;
	Tri_ElossCorrection::DefineVariables( mode );

	return DefineVarsFromList( THaTrackingModule::GetRVarDef(), mode );
}

//_____________________________________________________________________________
Int_t Tri_Track_Eloss::Process( const THaEvData& )
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
		// pathlength is zero; while for the gas target, fZref means
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
