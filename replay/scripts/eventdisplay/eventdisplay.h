#ifndef EVENTDISPLAY_H_
#define EVENTDISPLAY_H_

#include <vector>
#include "Rtypes.h"

class TTree;
class TPad;
class TVector3;
class TString;
class TCanvas;

class VDisplay 
{
    friend class EvDisplay;
    public:
        VDisplay() : pad(0) {} // constructor
        virtual ~VDisplay() {}
        virtual void draw_event(Long64_t) = 0;
	virtual void SetPad(TPad* cpad) { pad = cpad; }
        static void SetTree(TTree* T); // Set the tree to be analyzed
    protected:
	TPad* pad;
        static Double_t ntrack;
        static TTree* tree;
        static TString arm;
        virtual void InitTree() = 0;        // Init the branch address of the tree
                                            // Required for the inherited classes
                                            // Called by ctor or SetTree() static method

};

class VDCdisplay : public VDisplay {

public:
	VDCdisplay(Int_t no=1);             // no=1 for bottom VDC and no=2 for top VDC
	virtual ~VDCdisplay();
	virtual void draw_event(Long64_t);
	//void SetTree(TTree* t) { tree = t; }

	enum vdcPlane {uplane, vplane};

        static void Reset() { IsInit = kFALSE; }

private:
	static const UInt_t NoOfWires;
	static const UInt_t MaxNoOfHits;
	static const UInt_t MaxNoOfTrks;

        static Double_t* projx;
	static Double_t* projy;
	static Double_t* dxdz;
	static Double_t* dydz;

	Int_t number;
	Double_t wspace;
	Double_t width;
	Double_t length;
	Double_t dz;
	Double_t scale;

	Double_t* uwires;
	Int_t nuwires;
	Double_t* vwires;
	Int_t nvwires;
	//Double_t ntrack;
	//TTree* tree;
	TVector3* v0;
        //Bool_t flag;

        static Bool_t IsInit;
        static void AInit();

	virtual void InitTree();
	void draw_wire(vdcPlane, UInt_t) const;
	void draw_proj(Double_t, Double_t);
	virtual void draw_outline() const;
        //virtual void clear();
};

class S2mdisplay : public VDisplay {
public:
	S2mdisplay();
	virtual ~S2mdisplay();
	virtual void draw_event(Long64_t);
	//void SetTree(TTree* t) { tree = t; }
private:
	static const UInt_t NoOfPads;
	static const UInt_t MaxNoOfTrks;
	Double_t halfwidth;
	Double_t halfheight;
	Double_t scale;

	Double_t* fTHitLeft;
	Double_t* fTHitRight;
	Double_t* projx;
	Double_t* projy;
	//Double_t ntrack;
	//TTree* tree;
	//TVector3* v0;

	//Bool_t IsInit;

	virtual void InitTree();
	void draw_pad(TString, UInt_t) const;
	void draw_proj(Double_t, Double_t);
	virtual void draw_outline() const;
        //virtual void clear();
};

class FPPdisplay : public VDisplay {

public:
	FPPdisplay();
	virtual ~FPPdisplay();
	virtual void draw_event(Long64_t);
	//void SetTree(TTree* t) { tree = t; }

	enum fppPlane {v1,v2,v3,u1,u2,u3};

private:
	static const UInt_t NoOfStraws;
	static const UInt_t MaxNoOfHits;
	static const UInt_t MaxNoOfTrks;
	Int_t number;
	Double_t wspace;
	Double_t width;
	Double_t height;
	Double_t dz;
	Double_t scale;

	Double_t* u1straws;
	Int_t nu1straws;
	Double_t* u2straws;
	Int_t nu2straws;
	Double_t* u3straws;
	Int_t nu3straws;
	Double_t* v1straws;
	Int_t nv1straws;
	Double_t* v2straws;
	Int_t nv2straws;
	Double_t* v3straws;
	Int_t nv3straws;
	Double_t* projx;
	Double_t* projy;
	//Double_t ntrack;
	//TTree* tree;
	//TVector3* v0;

	//Bool_t IsInit;

	virtual void InitTree();
	void draw_straw(fppPlane, UInt_t) const;
	void draw_proj(Double_t, Double_t);
	virtual void draw_outline() const;
        //virtual void clear();
};

class Calodisplay : public VDisplay {
public:
	Calodisplay(const Char_t*, UInt_t, UInt_t, Double_t, Double_t);
	virtual ~Calodisplay();
	virtual void draw_event(Long64_t);
	void SetTree(TTree* t) { tree = t; }
private:
	static const UInt_t MaxNoOfClusts;
	static const UInt_t MaxNoOfTrks;
	const Char_t* fname;
	UInt_t fNRows;
	UInt_t fNColumns;
	Double_t dx;       // Here x and y refers to the spectrometer cs
	Double_t dy;	   // The signs of dx and dy are related to the position of block 1
	Double_t scale;

	Double_t nclust;
	Double_t nhitblock;
	Double_t* mult;
	Double_t* nblk;
	Double_t* projx;
	Double_t* projy;
	//Double_t ntrack;
	//TTree* tree;
	//TVector3* v0;

	//Bool_t IsInit;

	virtual void InitTree();
	void draw_block(UInt_t, Color_t) const;
	void draw_block(UInt_t, UInt_t, Color_t) const;
	void draw_proj(Double_t, Double_t);
	virtual void draw_outline() const;
        //virtual void clear();
};

class EvDisplay
{
    public:
	EvDisplay() : fCanvas(0) {}
	virtual ~EvDisplay() {
            for (UInt_t i=0; i<detectors.size(); i++) {
                delete detectors[i];
		detectors[i] = 0;
            }
	    if (fCanvas) {
	        delete fCanvas;
	        fCanvas = 0;
	    }
	}
	void Add(VDisplay*);
	void Process(Long64_t);
	void CreateCanvas(Int_t x1=0, Int_t y1=0, Int_t width=1260, Int_t height=562);
        Int_t GetNumOfDets() const { return detectors.size(); }
	void Init(TTree*) const;
	void SavePlot(const char* filename, Option_t* option="") const {
	    fCanvas->SaveAs(filename,option);
	}
    private:
	TCanvas* fCanvas;
	std::vector<VDisplay*> detectors;

};

#endif
