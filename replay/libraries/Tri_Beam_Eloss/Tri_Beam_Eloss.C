//--- Adapted from the Gmp equivalent written by Barak Schmookler & Thir Gautam  12-01-15
//--- by Reynier Cruz Torres for the Tritium experiments.
//--- Revision History: 
//    1) Barak Schmookler, August 2016: Change from loop 2 to loop 3. Loop
//       3 was used in Spring 2016 run.
//    2) Longwu Ou, October 2016: Now the target geometry is determined by a
//       tag "<fprefix>.config" in the run database.
//    3) Reynier Cruz Torres, May 2018: Adapted code for the specifics
//       of the Tritium gas targets.
//
//////////////////////////////////////////////////////////////////////////
//
// Tri_Beam_Eloss
//
// Correct energy loss of the beam by an amount that depends on reaction
// vertex. For the gas target, the beam passes through an entrance window (Al 7075).
//
// Since a call to Tri_ElossCorrection::SetPathlength(...) sets fExtPathMode = kTRUE,
// the "pathlength" will not be read from the database. So, we allow the user to set 
// the target z offset when instantiating this class.
// (N.B. If a target of length "x" is centered at z = 0., the offset should be
// set to "-x/2.")
//
// We don't make take the beam angle into account when calculating the energy 
// loss.
//////////////////////////////////////////////////////////////////////////

#include "Tri_Beam_Eloss.h"
#include "THaBeam.h"
#include "THaVertexModule.h"
#include "TMath.h"
#include "TVector3.h"
#include "Riostream.h"

using namespace std;

//_____________________________________________________________________________
Tri_Beam_Eloss::Tri_Beam_Eloss( const char* name, const char* description,
		const char* input_beam, const char* vertex,
		const Double_t z_offset ) :
	Tri_ElossCorrection(name,description,input_beam), fBeamModule(NULL)
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
Tri_Beam_Eloss::~Tri_Beam_Eloss()
{
	// Destructor

	DefineVariables( kDelete );
}

//_____________________________________________________________________________
void Tri_Beam_Eloss::CalcEloss( THaBeamInfo* beamifo )
{
	// Compute the mean energy loss for the beam given in beamifo.
	//
	// Currently, we use a very simple algorithm that computes
	// the energy loss based on a fixed material thickness.
	// Only the beta dependence of the energy loss is used,
	// but there are no corrections for angle, pathlength etc.
	//
	// May be overridden by derived classes as necessary.

	Double_t p0 = beamifo->GetP();
	Double_t beta = p0 / TMath::Sqrt(p0*p0 + fM*fM);
	if( fElectronMode ) {

		// Check if target corresponds to one of the gas cells
		if(fZmed < 3. && fAmed < 4.){	

			Double_t eloss_Al(0),eloss_gas(0);

			// Set Aluminum 7075 Parameters
			Double_t Z_Al   = 13.   ;
			Double_t A_Al   = 26.98 ; //(amu)
			Double_t rho_Al = 2.81  ; //for Al 7075 

			// ---------------------------------------------------------------------------------------------
			// Thickness of the target entrance
			Double_t l_Al = 0;

			if     ( fZmed>0.09 && fZmed<1.01 && fAmed>0.09 && fAmed<1.01 ) l_Al=0.311E-3; // m - Hydrogen
			else if( fZmed>0.09 && fZmed<1.01 && fAmed>1.09 && fAmed<2.01 ) l_Al=0.215E-3; // m - Deuterium
			else if( fZmed>0.09 && fZmed<1.01 && fAmed>2.09 && fAmed<3.01 ) l_Al=0.253E-3; // m - Tritium
			else if( fZmed>1.09 && fZmed<2.01 && fAmed>2.09 && fAmed<3.01 ) l_Al=0.203E-3; // m - Helium-3
			else l_Al=(0.311E-3+0.215E-3+0.253E-3+0.203E-3)/4.; // m - Average of the numbers above

			// These numbers come from the following document:
			// https://logbooks.jlab.org/files/2018/01/3514205/TGT-RPT-17-007.pdf
			// ---------------------------------------------------------------------------------------------

			eloss_Al  = MostProbEloss( fZ, beta, Z_Al , A_Al , rho_Al  , l_Al        ); // Aluminum Entrance Window
			eloss_gas = MostProbEloss( fZ, beta, fZmed, fAmed, fDensity, fPathlength ); // Gas Target

			fEloss = eloss_Al + eloss_gas ;

		} else {
			fEloss = MostProbEloss( fZ, beta, fZmed, fAmed, fDensity, fPathlength );
		}

	}
	else {
		fEloss = MostProbEloss( fZ, beta, fZmed, fAmed, fDensity, fPathlength );
	}

	// ----------------------------------------------------------------------------	
	// Account for the energy loss in the Beryllium window upstream of the targets
	// 05-30-2018 RCT
	Double_t eloss_Be(0);

	// Setting up Beryllium parameters
	Double_t Z_Be   = 4.       ;
        Double_t A_Be   = 9.0122   ; //(amu)
        Double_t rho_Be = 1.848    ; //g/cc
	Double_t l_Be   = 0.2003E-3; // m
	
	eloss_Be = MostProbEloss( fZ, beta, Z_Be, A_Be, rho_Be, l_Be ); // Beryllium Window Energy Loss
	fEloss += eloss_Be;
	// ----------------------------------------------------------------------------
}

//_____________________________________________________________________________
void Tri_Beam_Eloss::Clear( Option_t* opt )
{
	// Clear all event-by-event variables.
	Tri_ElossCorrection::Clear(opt);
	BeamIfoClear();
}

//_____________________________________________________________________________
THaAnalysisObject::EStatus Tri_Beam_Eloss::Init( const TDatime& run_time )
{
	// Initialize the module.
	// Locate the input beam module named in fInputName and save pointer to it. 
	// Extract mass and charge of the beam particles from the input.

	static const char* const here = "Init()";

	// Find the input beam module
	fBeamModule = dynamic_cast<THaBeamModule*>
		( FindModule( fInputName.Data(), "THaBeamModule"));
	if( !fBeamModule )
		return fStatus;

	// Get the parent beam apparatus from the input module
	// NB: FindModule() above already checked initialization
	THaBeamInfo* beamifo = fBeamModule->GetBeamInfo();
	THaBeam* beam = beamifo->GetBeam();
	if( !beam ) {
		Error( Here(here), "Oops. Input beam module has no pointer to "
				"a beam apparatus?!?" );
		return fStatus = kInitError;  
	}
	// Needed for initialization of dependent modules in a chain
	fBeamIfo.SetBeam(beam);

	// Get beam particle properties from the input module. 
	// Overrides anything set by SetMass()
	SetMass( beamifo->GetM() );
	fZ = TMath::Abs(beamifo->GetQ());

	// Standard initialization. Calls this object's DefineVariables().
	// Reads material parameters from the run database
	Tri_ElossCorrection::Init( run_time );

	return fStatus;
}

//_____________________________________________________________________________
Int_t Tri_Beam_Eloss::DefineVariables( EMode mode )
{
	// Define/delete global variables.

	if( mode == kDefine && fIsSetup ) return kOK;
	Tri_ElossCorrection::DefineVariables( mode );

	// Define the variables for the beam info subobject
	return DefineVarsFromList( THaBeamModule::GetRVarDef(), mode );
}

//_____________________________________________________________________________
Int_t Tri_Beam_Eloss::Process( const THaEvData& )
{
	// Calculate corrections and adjust the track parameters.

	if( !IsOK() ) return -1;

	THaBeamInfo* beamifo = fBeamModule->GetBeamInfo();
	if( !beamifo->IsOK() ) return 2;

	// Copy the input track info
	fBeamIfo = *beamifo;

	// Compute the correction. Remember that we want to compute the
	// beam energy at the reaction vertex, so we need to subtract the
	// energy loss from the input energy.
	Double_t p_in = fBeamIfo.GetP(); 
	if( p_in <= 0.0 ) return 4; //oops
	Double_t E_in = TMath::Sqrt(p_in*p_in + fM*fM);
	if( !fTestMode ) {
		// calculate pathlength for this event if we have a vertex module
		if( fExtPathMode ) {
			if( !fVertexModule->HasVertex() )
				return 1;
			fPathlength = 
				TMath::Abs(fVertexModule->GetVertex().Z() - fZref) * fScale;
		}
		//update fEloss
		CalcEloss(beamifo);
	}
	Double_t p_out = TMath::Sqrt(p_in*p_in + fEloss*fEloss - 2.0*E_in*fEloss);

	// Apply the corrected momentum to our beam info
	fBeamIfo.SetP(p_out);

	fDataValid = true;
	return 0;
}

//_____________________________________________________________________________
ClassImp(Tri_Beam_Eloss)

