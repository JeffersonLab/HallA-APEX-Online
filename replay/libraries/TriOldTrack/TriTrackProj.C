//////////////////////////////////////////////////////////////////////////
//
// TriTrackProj
//    - reconstructed information for a track hit in a timing detector
//////////////////////////////////////////////////////////////////////////

#include "TriTrackProj.h"

const Double_t TriTrackProj::kBig = 1.e38;

TriTrackProj::TriTrackProj( Double_t x, Double_t y,
			    Double_t pathl,
			    Double_t dx, Int_t ch,
			    TriNonTrackingDetector *parent):
  fX(x), fY(y), fPathl(pathl), fdX(dx), fChannel(ch),
  fParent(parent) { }

TriTrackProj::~TriTrackProj() { }

ClassImp(TriTrackProj)

