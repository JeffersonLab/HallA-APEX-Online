#ifndef ROOT_GmpShowerCluster
#define ROOT_GmpShowerCluster

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// GmpShowerCluster                                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "TObject.h"
#include "TClonesArray.h"
#include "GmpShowerBlock.h"

class GmpShowerCluster : public TObject {
    public:
        GmpShowerCluster() : fMainBlkNo(-1), fCenterX(kBig), fCenterY(kBig), fE(kBig) {
            fBlks = new TClonesArray("GmpShowerBlock",16);
        }
        //GmpShowerCluster(const GmpShowerCluster& cluster);
        //GmpShowerCluster(const GmpShowerCluster* cluster) { return GmpShowerCluster(*cluster); }
        //GmpShowerCluster& operator=(const GmpShowerCluster&);
        virtual ~GmpShowerCluster() {
            fBlks->Clear();
            delete fBlks;
            fBlks = 0;
        }
        Double_t GetE() const { return fE; }
        Double_t GetCtrX() const { return fCenterX; }
        Double_t GetCtrY() const { return fCenterY; }
        Int_t GetMult() const { return fBlks->GetLast()+1; }
        Int_t GetMainBlk() const { return fMainBlkNo; }
        TClonesArray* GetBlocks() const { return fBlks; }
        void AddBlock(const GmpShowerBlock* hit) {
            new ((*fBlks)[fBlks->GetLast()+1]) GmpShowerBlock(hit);
        }
        void CalcCluster();
        Bool_t IsSortable() const { return kTRUE; }
        virtual Int_t Compare(const TObject*) const;


    protected:

        static const Double_t kBig;
        TClonesArray* fBlks;
        Int_t fMainBlkNo;
        Double_t fCenterX;
        Double_t fCenterY;
        Double_t fE;
        void SetE(Double_t e) { fE=e; }
        void SetCtrX(Double_t x) { fCenterX=x; }
        void SetCtrY(Double_t y) { fCenterY=y; }

        ClassDef(GmpShowerCluster,0)
};

#endif
