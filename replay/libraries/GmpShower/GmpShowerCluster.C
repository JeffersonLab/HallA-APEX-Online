///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// GmpShowerCluster                                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include "GmpShowerCluster.h"

const Double_t GmpShowerCluster::kBig = 1.e38;

void GmpShowerCluster::CalcCluster() {
    if (fBlks->GetLast()+1==0) return;

    fBlks->Sort();
    const GmpShowerBlock* mainblk = static_cast<const GmpShowerBlock*>(fBlks->At(0));
    fMainBlkNo = mainblk->GetNumber();

    Double_t xx=0, yy=0, ee=0;
    for (int i=0; i<fBlks->GetLast()+1; i++) {
        const GmpShowerBlock* showerblk = static_cast<const GmpShowerBlock*>(fBlks->At(i));
        ee+=showerblk->GetEnergy();
        xx+=showerblk->GetEnergy()*showerblk->GetX();
        yy+=showerblk->GetEnergy()*showerblk->GetY();
    }
    SetE(ee);
    SetCtrX(xx/ee);
    SetCtrY(yy/ee);
}

Int_t GmpShowerCluster::Compare(const TObject* obj) const
{
    if ( !obj || IsA() != obj->IsA() ) return -1;

    const GmpShowerCluster* cluster = static_cast<const GmpShowerCluster*>(obj);
    if ( fE > cluster->GetE() ) return -1;
    if ( fE < cluster->GetE() ) return 1;
    return 0;
}

ClassImp(GmpShowerCluster)

