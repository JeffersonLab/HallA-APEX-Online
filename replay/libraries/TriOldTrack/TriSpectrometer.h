#ifndef ROOT_TriSpectrometer
#define ROOT_TriSpectrometer

//////////////////////////////////////////////////////////////////////////
//
// TriSpectrometer
//
//////////////////////////////////////////////////////////////////////////

#include "THaApparatus.h"
#include "THaVertexModule.h"
#include "TriTrackingModule.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TRotation.h"

class THaParticleInfo;
class THaPidDetector;
class TriTrack;
class TList;
class THaCut;

class TriSpectrometer : public THaApparatus, public TriTrackingModule,
			public THaVertexModule {
  
public:
  virtual ~TriSpectrometer();
  
  // Main functions
  virtual void             Clear( Option_t* opt="");
  virtual Int_t            CoarseTrack();
  virtual Int_t            CoarseReconstruct();
  virtual Int_t            Track();
  virtual Int_t            Reconstruct();
  virtual Int_t            CalcPID();
  virtual Int_t            FindVertices( TClonesArray& tracks ) = 0;
  virtual Int_t            TrackCalc() = 0;

  // Auxiliary functions
  virtual Int_t            AddDetector( THaDetector* det ,Bool_t quiet, Bool_t first);
  virtual Int_t            AddPidParticle( const char* shortname, 
					   const char* name,
					   Double_t mass, Int_t charge = 0 );
  virtual void             DefinePidParticles();
  virtual Int_t            DefineVariables( EMode mode = kDefine );
          TriTrack*        GetGoldenTrack() const { return fGoldenTrack; }
          Int_t            GetNpidParticles() const;
          Int_t            GetNpidDetectors() const;
  const   THaParticleInfo* GetPidParticleInfo( Int_t i ) const;
  const   THaPidDetector*  GetPidDetector( Int_t i ) const;
          Int_t            GetNTracks()  const { return fTracks->GetLast()+1; }
          TClonesArray*    GetTracks()   const { return fTracks; }
          TClonesArray*    GetTrackPID() const { return fTrackPID; }
  virtual const TVector3&  GetVertex()   const;
  virtual Bool_t           HasVertex() const;

          Bool_t           IsDone( UInt_t stage ) const;
          Bool_t           IsPID() const       { return fPID; }
          void             SetGoldenTrack( TriTrack* t ) { fGoldenTrack = t; }
          void             SetPID( Bool_t b = kTRUE )    { fPID = b; }

  // The following is specific to small-acceptance pointing spectrometers
  // using spectrometer-specific coordinates such as TRANSPORT
  const   TRotation&       GetToLabRot() const { return fToLabRot; }
  const   TRotation&       GetToTraRot() const { return fToTraRot; }
  const   TVector3&        GetPointingOffset() const { return fPointingOffset; }
          Double_t         GetThetaGeo() const { return fThetaGeo; }
          Double_t         GetPhiGeo()   const { return fPhiGeo; }
          Double_t         GetThetaSph() const { return fThetaSph; }
          Double_t         GetPhiSph()   const { return fPhiSph; }
          Double_t         GetPcentral() const { return fPcentral; }
          Double_t         GetCollDist() const { return fCollDist; }

  virtual void             TrackToLab( TriTrack& track, TVector3& pvect ) const;
  virtual void             TransportToLab( Double_t p, Double_t th, 
					   Double_t ph, TVector3& pvect ) const;
  virtual void             LabToTransport( const TVector3& vertex, 
					   const TVector3& pvect, 
					   TVector3& tvertex, 
					   Double_t* ray ) const;
          void             LabToTransport( const TVector3& vertex, 
					   const TVector3& pvect, 
					   Double_t* ray ) const;
  enum EStagesDone {
    kCoarseTrack = BIT(0),
    kCoarseRecon = BIT(1),
    kTracking    = BIT(2),
    kReconstruct = BIT(3)
  };

protected:

  static const Int_t kInitTrackMultiplicity = 5;

  TClonesArray*   fTracks;                //Tracks 
  TClonesArray*   fTrackPID;              //PID info for the tracks
  TList*          fTrackingDetectors;     //Tracking detectors
  TList*          fNonTrackingDetectors;  //Non-tracking detectors
  TObjArray*      fPidDetectors;          //PID detectors
  TObjArray*      fPidParticles;          //Particles for which we want PID
  TriTrack*       fGoldenTrack;           //Golden track within fTracks
  Bool_t          fPID;                   //PID enabled

  // The following is specific to small-acceptance pointing spectrometers
  TRotation       fToLabRot;              //Rotation matrix from TRANSPORT to lab
  TRotation       fToTraRot;              //Rotation matrix from lab to TRANSPORT
  TVector3        fPointingOffset;        //Optical point in lab coordinate system
  Double_t        fThetaGeo;              //In-plane geographic central angle (rad)
  Double_t        fPhiGeo;                //Out-of-plane geographic central angle (rad)
  Double_t        fThetaSph, fPhiSph;     //Central angles in spherical coords. (rad)
  Double_t        fSinThGeo, fCosThGeo;   //Sine and cosine of central angles
  Double_t        fSinPhGeo, fCosPhGeo;   // in geographical coordinates
  Double_t        fSinThSph, fCosThSph;   //Sine and cosine of central angles in 
  Double_t        fSinPhSph, fCosPhSph;   // spherical coordinates
  Double_t        fPcentral;              //Central momentum (GeV)
  Double_t        fCollDist;              //Distance from collimator to target center (m)

  UInt_t          fStagesDone;            //Bitfield of completed analysis stages

  // only derived classes can construct me
  TriSpectrometer( const char* name, const char* description );

  virtual Int_t   ReadRunDatabase( const TDatime& date );

private:
  Bool_t          fListInit;      //Detector lists initialized

  void            ListInit();     //Initializes lists of specialized detectors

  ClassDef(TriSpectrometer,0)     //A generic spectrometer
};


//---------------- inlines ----------------------------------------------------
inline Int_t TriSpectrometer::GetNpidParticles() const
{
  return fPidParticles->GetLast()+1;
}

//_____________________________________________________________________________
inline Int_t TriSpectrometer::GetNpidDetectors() const
{
  return fPidDetectors->GetLast()+1;
}

//_____________________________________________________________________________
inline const THaParticleInfo* TriSpectrometer::GetPidParticleInfo( Int_t i ) 
  const
{
  return (const THaParticleInfo*) fPidParticles->At(i);
}

//_____________________________________________________________________________
inline const THaPidDetector* TriSpectrometer::GetPidDetector( Int_t i ) const
{
  return (const THaPidDetector*) fPidDetectors->At(i);
}

//_____________________________________________________________________________
inline 
Bool_t TriSpectrometer::IsDone( UInt_t stage ) const
{
  return ((fStagesDone & stage) != 0);
}

//_____________________________________________________________________________
inline
void TriSpectrometer::LabToTransport( const TVector3& vertex,
				      const TVector3& pvect,
				      Double_t* ray ) const
{
  TVector3 dummy;
  LabToTransport( vertex, pvect, dummy, ray );
}

#endif

