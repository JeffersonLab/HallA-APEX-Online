//--- Author :    Barak Schmookler & Thir Gautam  12-01-15
//--- Revision History: 
//    1) Barak Schmookler, August 2016: Change from loop 2 to loop 3. Loop
//       3 was used in Spring 2016 run.
//    2) Longwu Ou, October 2016: Now the target geometry is determined by a
//       tag "<fprefix>.config" in the run database.
//
//////////////////////////////////////////////////////////////////////////
//
// Gmp_Beam_Eloss
//
// Correct energy loss of the beam by an amount that depends on reaction
// vertex. For the hydrogen target, the beam passes through an entrance window
// of thickness 0.147 (0.175) mm (Al 7075).
//
// Since a call to THaElossCorrection::SetPathlength(...) sets fExtPathMode = kTRUE,
// the "pathlength" will not be read from the database. So, we allow the user to set 
// the target z offset when instantiating this class.
// (N.B. If a target of length "x" is centered at z = 0., the offset should be
// set to "-x/2.")
//
// We don't make take the beam angle into account when calculating the energy 
// loss.
//
// We also want to briefly discuss what is currently implemented for the beam
// in SIMC... The real beam energy is calculated taking into account the reaction
// vertex. As of some point in 2016, the beam energy is also reconstructed with a
// reaction vertex dependence. (Previously, it corrected for the energy loss 
// assuming the beam travels to the center of the target.)
//
//////////////////////////////////////////////////////////////////////////

#include "Gmp_Beam_Eloss.h"
#include "THaBeam.h"
#include "THaVertexModule.h"
#include "TMath.h"
#include "TVector3.h"
#include "Riostream.h"

using namespace std;

//_____________________________________________________________________________
Gmp_Beam_Eloss::Gmp_Beam_Eloss( const char* name, const char* description,
				const char* input_beam, const char* vertex,
				const Double_t z_offset ) :
  THaElossCorrection(name,description,input_beam), fBeamModule(NULL)
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
Gmp_Beam_Eloss::~Gmp_Beam_Eloss()
{
  // Destructor

  DefineVariables( kDelete );
}

//_____________________________________________________________________________
void Gmp_Beam_Eloss::CalcEloss( THaBeamInfo* beamifo )
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
    
    // Check if LH2 Target
    if(fZmed < 1.01 && fAmed < 1.01){

      //cout << "Doing Electron w/ Hydrogen Target" <<endl;
      
      Double_t eloss_Al(0),eloss_H2(0);
      
      //Set Aluminum 7075 Parameters
      Double_t Z_Al = 13. ;
      Double_t A_Al = 26.98 ; //(amu)
      Double_t rho_Al = 2.81 ; //for Al 7075 

      Double_t l_Al = 0;

      switch (fConfig.Atoi()) {
	      case 1:
		      l_Al = 0.175E-3 ;
		      break;

	      case 2:
		      l_Al = 0.147E-3 ;
		      break;
	      default:
		      l_Al = 0.175E-3 ;
      }
      
      // Aluminum Entrance Window
      eloss_Al = ElossElectron( beta, Z_Al, A_Al, 
				rho_Al, l_Al );
      
      // Hydrogen Target
      eloss_H2 = ElossElectron( beta, fZmed, fAmed, 
				fDensity, fPathlength );
      
      fEloss = eloss_Al + eloss_H2 ;
      
    } else {
      fEloss = ElossElectron( beta, fZmed, fAmed, 
			      fDensity, fPathlength );
    }
    
  } else {
    fEloss = ElossHadron( fZ, beta, fZmed, fAmed, 
			  fDensity, fPathlength );
  }
}

//_____________________________________________________________________________
void Gmp_Beam_Eloss::Clear( Option_t* opt )
{
  // Clear all event-by-event variables.
  
  THaElossCorrection::Clear(opt);
  BeamIfoClear();
}

//_____________________________________________________________________________
THaAnalysisObject::EStatus Gmp_Beam_Eloss::Init( const TDatime& run_time )
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
  THaElossCorrection::Init( run_time );

#ifdef WITH_DEBUG
  if (fDebug>0) {
	  switch (fConfig.Atoi()) {
		  case 1:
			  Info( Here(here), "Using the loop 2 configuration of LH2 target"
			        "in Spring 2015 Gmp run." );
			  break;
		  case 2:
			  Info( Here(here), "Using the loop 3 configuration of LH2 target"
			        "in Spring 2016 Gmp run." );
			  break;
		  default:
			  Info( Here(here), "Using the loop 2 configuration of LH2 target"
			        "in Fall 2016 Gmp run." );
	  }
  }
#endif

  return fStatus;
}

//_____________________________________________________________________________
Int_t Gmp_Beam_Eloss::DefineVariables( EMode mode )
{
  // Define/delete global variables.

  if( mode == kDefine && fIsSetup ) return kOK;
  THaElossCorrection::DefineVariables( mode );

  // Define the variables for the beam info subobject
  return DefineVarsFromList( THaBeamModule::GetRVarDef(), mode );
}

//_____________________________________________________________________________
Int_t Gmp_Beam_Eloss::Process( const THaEvData& )
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
ClassImp(Gmp_Beam_Eloss)

