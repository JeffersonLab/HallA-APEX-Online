///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// GmpShowerBlock                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include "GmpShowerBlock.h"

const Double_t GmpShowerBlock::kBig = 1.e38;

GmpShowerBlock& GmpShowerBlock::operator=(const GmpShowerBlock& rhs)
{
    TObject::operator=(rhs);
    if ( this != &rhs ) {
        fXpos = rhs.fXpos;
        fYpos = rhs.fYpos;
        fEnergy = rhs.fEnergy;
        fRow = rhs.fRow;
        fColumn = rhs.fColumn;
        fNumber = rhs.fNumber;
    }
    return *this;
}

Int_t GmpShowerBlock::Compare(const TObject* obj) const
{
    if ( !obj || IsA() != obj->IsA() ) return -1;

    const GmpShowerBlock* block = static_cast<const GmpShowerBlock*>(obj);
    if (fEnergy > block->GetEnergy()) return -1;
    if (fEnergy < block->GetEnergy()) return 1;
    return 0;
}

ClassImp(GmpShowerBlock)

