//*-- Author :    Ole Hansen   07-Sep-00

//////////////////////////////////////////////////////////////////////////
//
// TriSpectrometer
//
// Abstract base class for a spectrometer.
//
// It implements a generic Reconstruct() method that should be
// useful for most situations. See specific description below.
//
// It also provides support for lists of different detector types
// (e.g. tracking/non-tracking/PID) for efficient processing.
//
//////////////////////////////////////////////////////////////////////////

#include "TriSpectrometer.h"
#include "THaParticleInfo.h"
#include "TriTrackingDetector.h"
#include "TriNonTrackingDetector.h"
#include "THaPidDetector.h"
#include "THaPIDinfo.h"
#include "TriTrack.h"
#include "TClass.h"
#include "TList.h"
#include "TMath.h"
#include "TList.h"
#include "VarDef.h"
#include <cmath>

#ifdef WITH_DEBUG
#include <iostream>
#endif

using namespace std;

//_____________________________________________________________________________
TriSpectrometer::TriSpectrometer( const char* name, const char* desc ) : 
  THaApparatus( name,desc ), fGoldenTrack(NULL), 
  fPID(kFALSE), fThetaGeo(0.0), fPhiGeo(0.0), fPcentral(1.0), fCollDist(0.0),
  fStagesDone(0), fListInit(kFALSE)
{
  // Constructor.
  // Protected. Can only be called by derived classes.

  fTracks     = new TClonesArray( "TriTrack",   kInitTrackMultiplicity );
  fTrackPID   = new TClonesArray( "THaPIDinfo", kInitTrackMultiplicity );
  fTrackingDetectors    = new TList;
  fNonTrackingDetectors = new TList;
  fPidDetectors         = new TObjArray;
  fPidParticles         = new TObjArray;

  Clear();
  DefinePidParticles();

  fProperties |= kNeedsRunDB;

  fTrkIfo.SetSpectrometer( this );
}

//_____________________________________________________________________________
TriSpectrometer::~TriSpectrometer()
{
  // Destructor. Delete the lists of specialized detectors.
  // FIXME: delete tracks, pid, vertices too?

  fPidParticles->Delete();   //delete all THaParticleInfo objects

  delete fPidParticles;          fPidParticles = 0;
  delete fPidDetectors;          fPidDetectors = 0;
  delete fNonTrackingDetectors;  fNonTrackingDetectors = 0;
  delete fTrackingDetectors;     fTrackingDetectors = 0;
  delete fTrackPID;              fTrackPID = 0;
  delete fTracks;                fTracks = 0;

  DefineVariables( kDelete );
}

//_____________________________________________________________________________
Int_t TriSpectrometer::AddDetector( THaDetector* pdet ,Bool_t quiet, Bool_t first)
{
  // Add a detector to the internal lists of spectrometer detectors.
  // This method is useful for quick testing of a new detector class that 
  // one doesn't want to include permanently in an Apparatus yet.
  // Duplicate detector names are not allowed.
  //
  // NOTE: The detector object must be allocated by the caller, but will be
  // deleted by the spectrometer. Do not delete detectors you've added
  // to an apparatus/spectrometer. Recommended: AddDetector( new MyDetector )

  if( !pdet || !pdet->IsA()->InheritsFrom("TriSpectrometerDetector")) {
    Error("AddDetector", "Detector is not a TriSpectrometerDetector. "
	  "Detector not added.");
    return -1;
  }
  TriSpectrometerDetector* sdet = 
    static_cast<TriSpectrometerDetector*>( pdet );

  Int_t status = THaApparatus::AddDetector( sdet );
  if( status != 0 ) return status;

  if( sdet->IsTracking() )
    fTrackingDetectors->Add( sdet );
  else
    fNonTrackingDetectors->Add( sdet );

  if( sdet->IsPid() )
    fPidDetectors->Add( sdet );

  return 0;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::AddPidParticle( const char* shortname, 
				       const char* name, 
				       Double_t mass, Int_t charge )
{
  // Add a particle type to the list of particles for which we want PID.
  // The "charge" argument is optional.
  
  fPidParticles->Add( new THaParticleInfo( shortname, name, mass, charge ));
  return 0;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::CalcPID()
{
  // Combine the PID information from all detectors into an overall PID
  // for each track.  The actual work is done in the THaPIDinfo class.
  // This is just a loop over all tracks.
  // Called by Reconstruct().

  TriTrack* theTrack;
  THaPIDinfo* pid;

  for( int i = 0; i < fTracks->GetLast()+1; i++ ) {
    if( (theTrack = static_cast<TriTrack*>( fTracks->At(i) ))) 
      if( (pid = theTrack->GetPIDinfo() )) {
	pid->CombinePID();
    }
  }    
  return 0;
}

//_____________________________________________________________________________
void TriSpectrometer::Clear( Option_t* opt )
{
  // Clear the spectrometer data for next event.

  THaApparatus::Clear(opt);
  // Clear the track array and also the track objects themselves since they
  // need to deallocate memory
  fTracks->Clear("C");
  TrkIfoClear();
  VertexClear();
  fGoldenTrack = NULL;
  fStagesDone = 0;
}

//_____________________________________________________________________________
void TriSpectrometer::DefinePidParticles()
{
  // Define the default set of PID particles:
  //  pion, kaon, proton

  fPidParticles->Delete();    //make sure array is empty

  AddPidParticle( "pi", "pion",   0.139, 0 );
  AddPidParticle( "k",  "kaon",   0.4936, 0 );
  AddPidParticle( "p",  "proton", 0.938, 1 );
}

//_____________________________________________________________________________
Int_t TriSpectrometer::DefineVariables( EMode mode )
{
  // Define/delete standard variables for a spectrometer (tracks etc.)
  // Can be overridden or extended by derived (actual) apparatuses

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  RVarDef vars[] = {
    { "tr.n",    "Number of tracks",             "GetNTracks()" },
    { "tr.x",    "Track x coordinate (m)",       "fTracks.TriTrack.fX" },
    { "tr.y",    "Track x coordinate (m)",       "fTracks.TriTrack.fY" },
    { "tr.th",   "Tangent of track theta angle", "fTracks.TriTrack.fTheta" },
    { "tr.ph",   "Tangent of track phi angle",   "fTracks.TriTrack.fPhi" },
    { "tr.p",    "Track momentum (GeV)",         "fTracks.TriTrack.fP" },
    { "tr.flag", "Track status flag",            "fTracks.TriTrack.fFlag" },
    { "tr.chi2", "Track's chi2 from hits",       "fTracks.TriTrack.fChi2" },
    { "tr.ndof", "Track's NDoF",                 "fTracks.TriTrack.fNDoF" },
    { "tr.d_x",  "Detector x coordinate (m)",    "fTracks.TriTrack.fDX" },
    { "tr.d_y",  "Detector y coordinate (m)",    "fTracks.TriTrack.fDY" },
    { "tr.d_th", "Detector tangent of theta",    "fTracks.TriTrack.fDTheta" },
    { "tr.d_ph", "Detector tangent of phi",      "fTracks.TriTrack.fDPhi" },
    { "tr.r_x",  "Rotated x coordinate (m)",     "fTracks.TriTrack.fRX" },
    { "tr.r_y",  "Rotated y coordinate (m)",     "fTracks.TriTrack.fRY" },
    { "tr.r_th", "Rotated tangent of theta",     "fTracks.TriTrack.fRTheta" },
    { "tr.r_ph", "Rotated tangent of phi",       "fTracks.TriTrack.fRPhi" },
    { "tr.tg_x", "Target x coordinate",          "fTracks.TriTrack.fTX"},
    { "tr.tg_y", "Target y coordinate",          "fTracks.TriTrack.fTY"},
    { "tr.tg_th", "Tangent of target theta angle", "fTracks.TriTrack.fTTheta"},
    { "tr.tg_ph", "Tangent of target phi angle",   "fTracks.TriTrack.fTPhi"},    
    { "tr.tg_dp", "Target delta",                "fTracks.TriTrack.fDp"},
    { "tr.px",    "Lab momentum x (GeV)",        "fTracks.TriTrack.GetLabPx()"},
    { "tr.py",    "Lab momentum y (GeV)",        "fTracks.TriTrack.GetLabPy()"},
    { "tr.pz",    "Lab momentum z (GeV)",        "fTracks.TriTrack.GetLabPz()"},
    { "tr.vx",    "Vertex x (m)",                "fTracks.TriTrack.GetVertexX()"},
    { "tr.vy",    "Vertex y (m)",                "fTracks.TriTrack.GetVertexY()"},
    { "tr.vz",    "Vertex z (m)",                "fTracks.TriTrack.GetVertexZ()"},
    { "tr.pathl", "Pathlength from tg to fp (m)","fTracks.TriTrack.GetPathLen()"},
    { "tr.time",  "Time of track@Ref Plane (s)", "fTracks.TriTrack.GetTime()"},
    { "tr.dtime", "uncer of time (s)",           "fTracks.TriTrack.GetdTime()"},
    { "tr.beta",  "Beta of track",               "fTracks.TriTrack.GetBeta()"},
    { "tr.dbeta", "uncertainty of beta",         "fTracks.TriTrack.GetdBeta()"},
    { "status",   "Bits of completed analysis stages", "fStagesDone" },
    { 0 }
  };

  return DefineVarsFromList( vars, mode );
}

//_____________________________________________________________________________
const TVector3& TriSpectrometer::GetVertex() const
{
  // Return vertex vector of the Golden Track.
  // Overrides standard method of THaVertexModule.

  return (fGoldenTrack) ? fGoldenTrack->GetVertex() : fVertex;
}

//_____________________________________________________________________________
Bool_t TriSpectrometer::HasVertex() const
{
  return (fGoldenTrack) ? fGoldenTrack->HasVertex() : kFALSE;
}

//_____________________________________________________________________________
void TriSpectrometer::ListInit()
{
  // Initialize lists of specialized detectors. 
  // Private function called by Reconstruct().

  fTrackingDetectors->Clear();
  fNonTrackingDetectors->Clear();
  fPidDetectors->Clear();

  TIter next(fDetectors);
  while( TriSpectrometerDetector* theDetector = 
	 static_cast<TriSpectrometerDetector*>( next() )) {

    if( theDetector->IsTracking() )
      fTrackingDetectors->Add( theDetector );
    else
      fNonTrackingDetectors->Add( theDetector );

    if( theDetector->IsPid() )
      fPidDetectors->Add( theDetector );
  }

  // Set up PIDinfo and vertex objects that can be associated with tracks

  UInt_t ndet  = GetNpidDetectors();
  UInt_t npart = GetNpidParticles();
  TClonesArray& pid  = *fTrackPID;

  for( int i = 0; i < kInitTrackMultiplicity; i++ ) {
    new( pid[i] )  THaPIDinfo( ndet, npart );
  }
  
  fListInit = kTRUE;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::CoarseTrack()
{
  // Coarse Tracking: First step of spectrometer analysis

  if( !fListInit )
    ListInit();

  // 1st step: Coarse tracking.  This should be quick and dirty.
  // Any tracks found are put in the fTrack array.
  TIter next( fTrackingDetectors );
  while( TriTrackingDetector* theTrackDetector =
	 static_cast<TriTrackingDetector*>( next() )) {
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "Call CoarseTrack() for " 
			<< theTrackDetector->GetName() << "... ";
#endif
    theTrackDetector->CoarseTrack( *fTracks );
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "done.\n";
#endif
  }

  fStagesDone |= kCoarseTrack;
  return 0;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::CoarseReconstruct()
{
  // 2nd step: Coarse processing.  Pass the coarse tracks to the remaining
  // detectors for any processing that can be done at this stage.
  // This may include clustering and preliminary PID.
  // PID information is tacked onto the tracks as a THaPIDinfo object.

  if( !IsDone(kCoarseTrack))
    CoarseTrack();

  TIter next( fNonTrackingDetectors );
  while( TriNonTrackingDetector* theNonTrackDetector =
	 static_cast<TriNonTrackingDetector*>( next() )) {
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "Call CoarseProcess() for " 
			<< theNonTrackDetector->GetName() << "... ";
#endif
    theNonTrackDetector->CoarseProcess( *fTracks );
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "done.\n";
#endif
  }

  fStagesDone |= kCoarseRecon;
  return 0;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::Track()
{
  // Fine tracking.  Compute the tracks with high precision.
  // If coarse tracking was done, this step should simply "refine" the
  // tracks found earlier, not add new tracks to fTrack.

  if( !IsDone(kCoarseRecon))
    CoarseReconstruct();

  TIter next( fTrackingDetectors );
  while( TriTrackingDetector* theTrackDetector =
	 static_cast<TriTrackingDetector*>( next() )) {
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "Call FineTrack() for " 
			<< theTrackDetector->GetName() << "... ";
#endif
    theTrackDetector->FineTrack( *fTracks );
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "done.\n";
#endif
  }

  // Reconstruct tracks to target/vertex
  // This usually also determines the track momentum
  FindVertices( *fTracks );

  fStagesDone |= kTracking;
  return 0;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::Reconstruct()
{
  // This method implements a fairly generic algorithm for spectrometer
  // reconstruction which should be useful for most situations.
  // Additional, equipment-specific processing can be done in
  // a derived class that calls this method first.
  //
  // The algorithm is as follows:
  //
  // For all tracking detectors:
  //   CoarseTrack()
  // For all non-tracking detectors:
  //   CoarseProcess()
  // For all tracking detectors:
  //   FineTrack()
  // Reconstruct tracks to target
  // For all non-tracking detectors:
  //   FineProcess()
  // Compute additional attributes of tracks (e.g. momentum, beta)
  // Find "Golden Track"
  // Combine all PID detectors to get overall PID for each track
  //

  // Do prior analysis stages if not done yet
  if( !IsDone(kTracking))
    Track();

  // Fine processing.  Pass the precise tracks to the
  // remaining detectors for any precision processing.
  // PID likelihoods should be calculated here.

  TIter next( fNonTrackingDetectors );
  while( TriNonTrackingDetector* theNonTrackDetector =
	 static_cast<TriNonTrackingDetector*>( next() )) {
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "Call FineProcess() for " 
			<< theNonTrackDetector->GetName() << "... ";
#endif
    theNonTrackDetector->FineProcess( *fTracks );
#ifdef WITH_DEBUG
    if( fDebug>1 ) cout << "done.\n";
#endif
  }

  // Compute additional track properties (e.g. beta)
  // Find "Golden Track" if appropriate.

  TrackCalc();

  // Compute combined PID

  if( fPID ) CalcPID();

  fStagesDone |= kReconstruct;
  return 0;
}

//_____________________________________________________________________________
void TriSpectrometer::TrackToLab( TriTrack& track, TVector3& pvect ) const
{
  // Convert TRANSPORT coordinates of 'track' to momentum vector 'pvect'
  // in the lab coordinate system (z = beam, y = up).
  // Uses the spectrometer angles from the database (loaded during Init())
  // for the transformation.
  // 
  // The track origin (vertex) is not calculated here because
  // doing so requires knowledge of beam positions and and angles.
  // Vertex calculations are done in a separate physics module.

  TransportToLab( track.GetP(), track.GetTTheta(), track.GetTPhi(), pvect );
}

//_____________________________________________________________________________
void TriSpectrometer::TransportToLab( Double_t p, Double_t th, Double_t ph,
				      TVector3& pvect ) const
{
  // Convert TRANSPORT vector to lab vector.
  // Inputs:
  //  p:  TRANSPORT momentum (absolute)
  //  th: Tangent of TRANSPORT theta
  //  ph: Tangent of TRANSPORT phi
  // Output:
  //  pvect: Vector in lab frame (z = beam, y = up) in same units as p.
  //
  // Note: Simple vector transformation can be done trivially by multiplying
  // with the appropriate rotation matrix, e.g.:
  //  TVector3 lab_vector = spect->GetToLabRot() * transport_vector;

  TVector3 v( th, ph, 1.0 );
  v *= p/TMath::Sqrt( 1.0+th*th+ph*ph );
  pvect = fToLabRot * v;
}

//_____________________________________________________________________________
void TriSpectrometer::LabToTransport( const TVector3& vertex, 
				      const TVector3& pvect,
				      TVector3& tvertex, Double_t* ray ) const
{
  // Convert lab coordinates to TRANSPORT coordinates in the spectrometer
  // coordinate system.
  // Inputs:
  //  vertex:  Reaction point in lab system
  //  pvect:   Momentum vector in lab
  // Outputs:
  //  tvertex: The vertex point in the TRANSPORT system, without any
  //           coordinate projections applied
  //  ray:     The TRANSPORT ray according to TRANSPORT conventions.
  //           This is an array of size 6 with elements x, tan(theta),
  //           y, tan(y), z, and delta.
  //           z is set to 0, and accordingly x and y are the TRANSPORT 
  //           coordinates in the z=0 plane. delta is computed with respect 
  //           to the present spectrometer's central momentum.
  //           Units are the same as of the input vectors.

  tvertex = fToTraRot * ( vertex - fPointingOffset );
  TVector3 pt = fToTraRot * pvect;
  if( pt.Z() != 0.0 ) {
    ray[1] = pt.X() / pt.Z();
    ray[3] = pt.Y() / pt.Z();
    // In the "ray", project the vertex to z=0
    ray[0] = tvertex.X() - tvertex.Z() * ray[1];
    ray[2] = tvertex.Y() - tvertex.Z() * ray[3];
  } else
    ray[0] = ray[1] = ray[2] = ray[3] = 0.0;
  
  ray[4] = 0.0;   // By definition for this ray, TRANSPORT z=0
  ray[5] = pt.Mag() / fPcentral - 1.0;
}

//_____________________________________________________________________________
Int_t TriSpectrometer::ReadRunDatabase( const TDatime& date )
{
  // Query the run database for parameters specific to this spectrometer
  // (central angles, momentum, offsets, drift, etc.)
  
  Int_t err = THaApparatus::ReadRunDatabase( date );
  if( err ) return err;

  FILE* file = OpenRunDBFile( date );
  if( !file ) return kFileError;

  static const Double_t degrad = TMath::Pi()/180.0;
  Double_t th = 0.0, ph = 0.0;
  Double_t off_x = 0.0, off_y = 0.0, off_z = 0.0;

  const DBRequest tags[] = { 
    { "theta",    &th }, 
    { "phi",      &ph, kDouble,0,1 },
    { "pcentral", &fPcentral},
    { "colldist", &fCollDist,kDouble,0,1 },
    { "off_x",    &off_x, kDouble,0,1},
    { "off_y",    &off_y, kDouble,0,1},
    { "off_z",    &off_z,kDouble,0,1 },
    { 0 }
  };
  err = LoadDB( file, date, tags, fPrefix );
  if( err ) {    
    if( err>0 )
      Error( Here("ReadRunDatabase()"), "Required key %s%s missing in the "
	     "run database.\nSpectrometer initialization failed.",
	     fPrefix, tags[err-1].name );
    fclose(file);
    return kInitError;
  }

  // Compute central angles in spherical coordinates and save trig. values
  // of angles for later use.
  // Note: negative theta corresponds to beam RIGHT.
  // phi should be close to zero unless this is a true out-of-plane spectrometer.
  fThetaGeo = degrad*th; fPhiGeo = degrad*ph;
  GeoToSph( fThetaGeo, fPhiGeo, fThetaSph, fPhiSph );
  fSinThGeo = sin( fThetaGeo ); fCosThGeo = cos( fThetaGeo );
  fSinPhGeo = sin( fPhiGeo );   fCosPhGeo = cos( fPhiGeo );
  Double_t st, ct, sp, cp;
  st = fSinThSph = sin( fThetaSph ); ct = fCosThSph = cos( fThetaSph );
  sp = fSinPhSph = sin( fPhiSph );   cp = fCosPhSph = cos( fPhiSph );

  // Compute the rotation from TRANSPORT to lab and vice versa.
  // If 'bend_down' is true, the spectrometer bends downwards.
  bool bend_down = false;
  Double_t norm = sqrt(ct*ct + st*st*cp*cp);
  TVector3 nx( st*st*sp*cp/norm, -norm, st*ct*sp/norm );
  TVector3 ny( ct/norm,          0.0,   -st*cp/norm   );
  TVector3 nz( st*cp,            st*sp, ct            );
  if( bend_down ) { nx *= -1.0; ny *= -1.0; }
#if ROOT_VERSION_CODE >= ROOT_VERSION(3,5,4)
  fToLabRot.SetToIdentity().RotateAxes( nx, ny, nz );
#else
  if( !fToLabRot.IsIdentity()) {
    TRotation tmp; //Identity
    fToLabRot = tmp;
  }
  fToLabRot.RotateAxes( nx, ny, nz );
#endif
  fToTraRot = fToLabRot.Inverse();
  fPointingOffset.SetXYZ( off_x, off_y, off_z );

  fclose(file);
  return kOK;
}

//_____________________________________________________________________________
ClassImp(TriSpectrometer)

