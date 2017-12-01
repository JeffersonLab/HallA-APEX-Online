#ifndef ROOT_TriVDCUVTrack
#define ROOT_TriVDCUVTrack

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriVDCUVTrack                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaCluster.h"
#include "TVector3.h"

class TriVDCCluster;
class TriVDCUVPlane;
class THaTrack;

class TriVDCUVTrack : public THaCluster {

public:
  TriVDCUVTrack() :
    fUClust(NULL), fVClust(NULL), fUVPlane(NULL), fTrack(NULL), fPartner(NULL),
    fX(0.0), fY(0.0), fTheta(0.0), fPhi(0.0) {}

  virtual ~TriVDCUVTrack() {}

  void CalcDetCoords();

  // Get and Set Functions  
  TriVDCCluster* GetUCluster() const { return fUClust; }
  TriVDCCluster* GetVCluster() const { return fVClust; }
  TriVDCUVPlane* GetUVPlane()  const { return fUVPlane; }
  TriVDCUVTrack* GetPartner()  const { return fPartner; }
  Double_t       GetX()        const { return fX; }
  Double_t       GetY()        const { return fY; }
  Double_t       GetTheta()    const { return fTheta; }
  Double_t       GetPhi()      const { return fPhi; } 

  void CalcChisquare(Double_t &chi2, Int_t &nhits) const;

  void SetUCluster( TriVDCCluster* clust)  { fUClust = clust;}
  void SetVCluster( TriVDCCluster* clust)  { fVClust = clust;}
  void SetUVPlane( TriVDCUVPlane* plane)   { fUVPlane = plane;}
  void SetTrack( THaTrack* track)          { fTrack = track; }
  void SetPartner( TriVDCUVTrack* partner) { fPartner = partner;}

  void SetX( Double_t x )                  { fX = x;}
  void SetY( Double_t y )                  { fY = y;}
  void SetTheta( Double_t theta )          { fTheta = theta;}
  void SetPhi( Double_t phi )              { fPhi = phi;} 
  void Set( Double_t x, Double_t y, Double_t theta, Double_t phi )
  { fX = x; fY = y; fTheta = theta; fPhi = phi; }
  void Set( Double_t x, Double_t y, Double_t theta, Double_t phi,
	    const TVector3& offset );


protected:
  TriVDCCluster* fUClust;       // Cluster in the U plane
  TriVDCCluster* fVClust;       // Cluster in the V plane
  TriVDCUVPlane* fUVPlane;      // UV plane that own's this track
  THaTrack*      fTrack;        // Track this UV Track is associated with
  TriVDCUVTrack* fPartner;      // UV track associated with this one in 
                                //  the other UV plane
  // Track Info (Detector cs - X,Y in m; theta, phi in tan(angle) )
  Double_t fX;     // X position of track in U wire-plane 
  Double_t fY;     // Y position of track in U wire-plane 
  Double_t fTheta; // Angle between z-axis and projection of track into xz plane
  Double_t fPhi;   // Angle between z-axis and projection of track into yz plane
  
private:
  // Hide copy ctor and op=
  TriVDCUVTrack( const TriVDCUVTrack& );
  TriVDCUVTrack& operator=( const TriVDCUVTrack& );

  ClassDef(TriVDCUVTrack,0)             // VDCUVTrack class
};

//-------------------- inlines -------------------------------------------------
inline
void TriVDCUVTrack::Set( Double_t x, Double_t y, Double_t theta, Double_t phi,
			 const TVector3& offset )
{
  // Set coordinates for this track. Also set absolute position vector.

  Set( x, y, theta, phi );
  fCenter.SetXYZ( x, y, 0.0 );
  fCenter += offset;
}

////////////////////////////////////////////////////////////////////////////////

#endif
