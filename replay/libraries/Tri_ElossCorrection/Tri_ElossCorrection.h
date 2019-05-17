#ifndef ROOT_Tri_ElossCorrection
#define ROOT_Tri_ElossCorrection

//////////////////////////////////////////////////////////////////////////
//
// Tri_ElossCorrection
//
//////////////////////////////////////////////////////////////////////////

#include "THaPhysicsModule.h"
#include "TString.h"

class THaVertexModule;

class Tri_ElossCorrection : public THaPhysicsModule {
  
public:
  virtual ~Tri_ElossCorrection();
  
  virtual void      Clear( Option_t* opt="" );
  virtual EStatus   Init( const TDatime& run_time );

  Double_t          GetMass()       const { return fM; }
  Double_t          GetEloss()      const { return fEloss; }

          void      SetInputModule( const char* name );
          void      SetMass( Double_t m /* GeV/c^2 */ );
          void      SetTestMode( Bool_t enable=kTRUE,
				 Double_t eloss_value=0.0 /* GeV */ );
          void      SetMedium( Double_t Z, Double_t A,
			       Double_t density  /* g/cm^3 */ );
          void      SetPathlength( Double_t pathlength /* m */ );
          void      SetPathlength( const char* vertex_module,
				   Double_t z_ref /* m */, Double_t scale = 1.0 );

  static  Double_t  ElossElectron( Double_t beta, Double_t z_med,
				   Double_t a_med, 
				   Double_t d_med /* g/cm^3 */, 
				   Double_t pathlength /* m */ );
  static  Double_t  ElossHadron( Int_t Z_hadron, Double_t beta, 
				 Double_t z_med, Double_t a_med, 
				 Double_t d_med /* g/cm^3 */, 
				 Double_t pathlength /* m */ );
  static  Double_t  MostProbEloss( Int_t Z_part, Double_t beta,
                		   Double_t z_med, Double_t a_med,
                		   Double_t d_med /* g/cm^3 */,
               			   Double_t pathlength /* m */);

protected:

  // Event-by-event data  
  Double_t           fEloss;       // Energy loss correction (GeV)

  // Parameters
  Double_t           fM;           // Mass of particle (GeV/c^2)
  Int_t              fZ;           // Charge (Z) of particle (only used for hadrons)

  Double_t           fZmed;        // Effective Z of medium 
  Double_t           fAmed;        // Effective A of medium
  Double_t           fDensity;     // Density of medium (g/cm^3)
  Double_t           fPathlength;  // Pathlength through medium (m)
  Double_t           fZref;        // Reference z-pos for variable pathlength calc (m)
  Double_t           fScale;       // Scale factor for variable pathlength calc

  Bool_t             fTestMode;    // If true, use fixed value for fEloss
  Bool_t             fElectronMode;// Particle is electron or hadron
  Bool_t             fExtPathMode; // If true, obtain pathlength from vertex module
  TString            fInputName;   // Name of input module
  TString            fVertexName;  // Name of vertex module for var pathlength, if any
  THaVertexModule*   fVertexModule;// Pointer to vertex module

  // Setup functions
  virtual Int_t DefineVariables( EMode mode = kDefine );
  virtual Int_t ReadRunDatabase( const TDatime& date );

  Tri_ElossCorrection( const char* name, const char* description,
		      const char* input = "", 
		      Double_t particle_mass = 0.511e-3 /* GeV/c^2 */,
		      Int_t hadron_charge = 1 );

private:
  // Energy loss library functions
  static Double_t ExEnerg( Double_t z_med, Double_t d_med );
  static void     HaDensi( Double_t z_med, Double_t d_med,
			   Double_t& X0, Double_t& X1, Double_t& M );

  ClassDef(Tri_ElossCorrection,0)   //Track energy loss correction module
};

#endif
