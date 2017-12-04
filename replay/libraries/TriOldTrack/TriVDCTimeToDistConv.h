#ifndef ROOT_TriVDCTimeToDistConv
#define ROOT_TriVDCTimeToDistConv

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriVDCTimeToDistConv                                                      //
//                                                                           //
// Base class for algorithms for converting TDC time into perpendicular      //
// drift distance                                                            //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"

class TriVDCTimeToDistConv : public TObject {

public:
  TriVDCTimeToDistConv() {}
  virtual ~TriVDCTimeToDistConv();

  virtual Double_t ConvertTimeToDist(Double_t time, Double_t tanTheta,
				     Double_t *ddist=0) = 0;
private:

  TriVDCTimeToDistConv( const TriVDCTimeToDistConv& );
  TriVDCTimeToDistConv& operator=( const TriVDCTimeToDistConv& );

  ClassDef(TriVDCTimeToDistConv,0)             // VDCTimeToDistConv class
};

////////////////////////////////////////////////////////////////////////////////

#endif
