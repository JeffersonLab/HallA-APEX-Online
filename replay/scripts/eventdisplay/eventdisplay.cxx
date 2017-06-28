#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include "TBox.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLine.h"
#include "TText.h"
#include "TVector3.h"
#include "TString.h"
#include "TEntryList.h"

#include "def.h"
#include "eventdisplay.h"

//--------------------------------Container-----------------------------------------

void EvDisplay::Add(VDisplay* det) {
	detectors.push_back(det);
}

void EvDisplay::Process(Long64_t event) {
    if (!fCanvas) CreateCanvas();
    for (UInt_t i=0; i<detectors.size(); i++) {
	detectors[i]->draw_event(event);
    }
    fCanvas->Update();
}

void EvDisplay::CreateCanvas(Int_t x1, Int_t y1, Int_t width, Int_t height) {

    if (fCanvas) delete fCanvas;

    fCanvas = new TCanvas("evdisplay","Event display",x1,y1,width,height);
    fCanvas->Draw();

    Int_t ndet = GetNumOfDets();
    Double_t dw = 1./ndet;
    Double_t w0 = 0.;

    for (Int_t i=0; i<GetNumOfDets(); i++, w0+=dw) {
	TPad* pad = new TPad("","",w0,0.0,w0+dw,1.0);
	pad->Draw();
	detectors[i]->SetPad(pad);
    }
}

void EvDisplay::Init(TTree* T) const {
    VDisplay::SetTree(T);
    for (UInt_t i=0; i<detectors.size(); i++) {
        detectors[i]->InitTree();
    }
}

//--------------------------------ABC-----------------------------------------

Double_t VDisplay::ntrack = 0;
TTree* VDisplay::tree = 0;
TString VDisplay::arm = "";

void VDisplay::SetTree(TTree* T) {
    if (tree == T) return;
    tree = T;
    if (tree) {
	//if (T->GetListOfBranches()->FindObject("L.tr.n")) arm = "L";
	//if (T->GetListOfBranches()->FindObject("R.tr.n")) arm = "R";
	if (T->GetBranch("L.tr.n")) arm = "L";
	if (T->GetBranch("R.tr.n")) arm = "R";
        tree->SetBranchAddress((arm+".tr.n").Data(),&ntrack);
        VDCdisplay::Reset();
    }
}

//--------------------------------VDC-----------------------------------------

Bool_t VDCdisplay::IsInit = kFALSE;
const UInt_t VDCdisplay::NoOfWires = 368;
const UInt_t VDCdisplay::MaxNoOfHits = 50;
const UInt_t VDCdisplay::MaxNoOfTrks = 10;
Double_t* VDCdisplay::projx = new Double_t[MaxNoOfTrks];
Double_t* VDCdisplay::projy = new Double_t[MaxNoOfTrks];
Double_t* VDCdisplay::dxdz = new Double_t[MaxNoOfTrks];
Double_t* VDCdisplay::dydz = new Double_t[MaxNoOfTrks];

void VDCdisplay::AInit() {

    tree->SetBranchAddress((arm+".tr.d_x").Data(),projx);
    tree->SetBranchAddress((arm+".tr.d_y").Data(),projy);
    tree->SetBranchAddress((arm+".tr.d_th").Data(),dxdz);
    tree->SetBranchAddress((arm+".tr.d_ph").Data(),dydz);

    IsInit = kTRUE;
}

VDCdisplay::VDCdisplay(Int_t no) : wspace(6.00e-3), width(0.288), length(2.118), dz(0.3348), scale(500) {

    number = (no+1)%2+1;
    wspace*=scale;
    width*=scale;
    length*=scale;

    uwires = new Double_t[MaxNoOfHits];
    nuwires = 0;
    vwires = new Double_t[MaxNoOfHits];
    nvwires = 0;
    //ntrack = 0;

    v0 = new TVector3(dz*(number-1),0.,dz*(number-1));

    if (tree) InitTree();
}

VDCdisplay::~VDCdisplay() {
    delete [] uwires;
    delete [] vwires;
    if (projx) { delete [] projx; projx = 0; }
    if (projy) { delete [] projy; projy = 0; }
    if (dxdz)  { delete [] dxdz;  dxdz  = 0; }
    if (dydz)  { delete [] dydz;  dydz  = 0; }
    delete v0;
}

void VDCdisplay::draw_outline() const {

    if (!pad) {
        std::cerr << "TPad not set!" << std::endl;
        return;
    }

    //pad->Draw();

    Double_t halfPadX = width*5./8.;
    Double_t halfPadY = length*5./8.;
    pad->Range(-halfPadX,-halfPadY,halfPadX,halfPadY);
    pad->cd();
    pad->Clear();

    TBox* vdc = new TBox(-width/2.,-length/2.,width/2.,length/2.);
    vdc->SetFillColor(kGreen-8);
    vdc->SetFillStyle(3001);
    vdc->Draw();
}	

void VDCdisplay::draw_wire(VDCdisplay::vdcPlane plane, UInt_t wire) const {

    if (wire >= NoOfWires || !(plane==uplane || plane==vplane)) return;

    UInt_t nLongWire = static_cast<Int_t>((length-width)/wspace)+1;
    UInt_t low = (NoOfWires-nLongWire)/2;
    UInt_t high = low+nLongWire-1;

    Double_t x1, x2, y1, y2;

    TLine* activewire = new TLine;

    if (plane==uplane) {
        if (wire<low) {
            y1 = length/2.;
            x2 = width/2.;
            y2 = length/2.-width+(low-wire)*wspace;
            x1 = -width/2.+(low-wire)*wspace;
        } else if (wire>high) {
            y2 = -length/2.;
            x1 = -width/2.;
            y1 = width-length/2.-(wire-high)*wspace;
            x2 = width/2.-(wire-high)*wspace;
        } else {
            x1 = -width/2.;
            x2 = width/2.;
            y1 = length/2.-(wire-low)*wspace;
            y2 = -length/2.+(high-wire)*wspace;
        }
        activewire->SetLineColor(kRed);
    } else {       // plane==v
        if (wire<low) {
            x1 = -width/2.;
            y2 = length/2.;
            x2 = width/2.-(low-wire)*wspace;
            y1 = length/2.-width+(low-wire)*wspace; 
        } else if (wire>high) {
            y1 = -length/2.;
            x2 = width/2.;
            y2 = width-length/2.-(wire-high)*wspace;
            x1 = -width/2.+(wire-high)*wspace;
        } else {
            x1 = -width/2.;
            x2 = width/2.;
            y1 = -length/2.+(high-wire)*wspace;
            y2 = length/2.-(wire-low)*wspace;
        }
        activewire->SetLineColor(kBlue);
    }

    y1*=-1;
    y2*=-1;

    activewire->SetX1(x1);
    activewire->SetY1(y1);
    activewire->SetX2(x2);
    activewire->SetY2(y2);

    pad->cd();
    activewire->Draw();

}

void VDCdisplay::InitTree() {

    if (!tree) {
        std::cerr << "TTree not set!" << std::endl;
    } else {

        if (!IsInit) AInit();

        TString upl, vpl, nupl, nvpl;
        upl.Form((arm+".vdc.u%d.wire").Data(),number);
        nupl.Form("Ndata.%s.vdc.u%d.wire",arm.Data(),number);
        vpl.Form((arm+".vdc.v%d.wire").Data(),number);
        nvpl.Form("Ndata.%s.vdc.v%d.wire",arm.Data(),number);

        tree->SetBranchAddress(upl.Data(),uwires);
        tree->SetBranchAddress(nupl.Data(),&nuwires);
        tree->SetBranchAddress(vpl.Data(),vwires);
        tree->SetBranchAddress(nvpl.Data(),&nvwires);
        //tree->SetBranchAddress("L.tr.n",&ntrack);	
        tree->SetBranchAddress((arm+".tr.d_x").Data(),projx);	
        tree->SetBranchAddress((arm+".tr.d_y").Data(),projy);	
        tree->SetBranchAddress((arm+".tr.d_th").Data(),dxdz);	
        tree->SetBranchAddress((arm+".tr.d_ph").Data(),dydz);	
    }

}

void VDCdisplay::draw_event(Long64_t event) {

    if (!tree) return;

    pad->cd();
    //clear();
    pad->Clear();
    draw_outline();

    Long64_t nevent = tree->GetEntries();	
    if (event>=nevent) return;

    tree->GetEntry(event);
    //std::cout << "ntrack = " << ntrack << std::endl;

    //std::cout << "nuwires = " << nuwires << std::endl;
    for (Int_t i=0; i<nuwires; i++) {
        draw_wire(uplane,(Int_t)uwires[i]);
    }

    //std::cout << "nvwires = " << nvwires << std::endl;
    for (Int_t i=0; i<nvwires; i++) {
        draw_wire(vplane,(Int_t)vwires[i]);
    }

    //std::cout << "ntrack = " << ntrack << std::endl;
    for (Int_t i=0; i<ntrack; i++) {
        Double_t xx = projx[i]+dxdz[i]*v0->Z(), yy = projy[i]+dydz[i]*v0->Z();
        draw_proj(xx-v0->X(),yy-v0->Y());

        //std::cout << "Track " << i << ": " 
            //<< "x= " << projx[i] << ", y= " << projy[i] << std::endl;
    }
}

void VDCdisplay::draw_proj(Double_t x, Double_t y) {  // x, y are the coordinates of projection in the local VDC coordinate system 

    pad->cd();

    x*=scale;
    y*=scale;

    //TText* hit = new TText(-y,x,"O");
    TText* hit = new TText(-y,-x,"O");
    hit->SetTextAlign(22);
    hit->SetTextColor(kGreen+2);
    hit->SetTextSize(0.05);

    hit->Draw();
}
/*
void VDCdisplay::clear() {

    if (IsInit) IsInit = kFLASE;
    pad->Clear();

    nuwires = 0;
    nvwires = 0;
    for (UInt_t i=0; i<MaxNoOfHits; i++) {
        uwires[i] = vwires[i] = -1;
    }

    //ntrack = 0;
    for (UInt_t i=0; i<MaxNoOfTrks; i++) {
        projx[i] = projy[i] = -1.e38;
        dxdz[i] = dydz[i] = -1.e38;
    }

}
*/
//----------------------------------------------------------------------------



//--------------------------------S2m-----------------------------------------

const UInt_t S2mdisplay::NoOfPads=16;
const UInt_t S2mdisplay::MaxNoOfTrks=10;

S2mdisplay::S2mdisplay() : halfwidth(0.216), halfheight(1.118), scale(500) {

    halfwidth*=scale;
    halfheight*=scale;

    fTHitLeft = new Double_t[NoOfPads];
    fTHitRight = new Double_t[NoOfPads];
    projx = new Double_t[MaxNoOfTrks];
    projy = new Double_t[MaxNoOfTrks];
    //ntrack = 0;

    //v0 = new TVector3(-0.11,0.0,3.1415);
    if (tree) InitTree();
}

S2mdisplay::~S2mdisplay() {
    delete [] fTHitLeft;
    delete [] fTHitRight;
    delete [] projx;
    delete [] projy;
    //delete v0;
}

void S2mdisplay::draw_outline() const {

    if (!pad) {
        std::cerr << "TPad not set!" << std::endl;
        return;
    }

    //pad->Draw();

    Double_t halfPadX = halfwidth*2*5./8.;
    Double_t halfPadY = halfheight*2*5./8.;
    pad->Range(-halfPadX,-halfPadY,halfPadX,halfPadY);
    pad->cd();
    pad->Clear();

    TBox* s2m = new TBox(-halfwidth,-halfheight,halfwidth,halfheight);
    s2m->SetFillColor(kGreen-8);
    s2m->SetFillStyle(3001);
    s2m->Draw();
    /*
       TBox* s2m = new TBox[2*NoOfPads];
       Double_t padheight = halfheight*2./NoOfPads;
       for (UInt_t i=0; i<NoOfPads; i++) {
       s2m[i] = new TBox(-halfwidth,-halfheight+i*padheight,0.0,-halfheight+(i+1)*padheight);
       s2m[i]->SetFillColor(38);
       s2m[i]->SetFillStyle(3001);
       s2m[i]->Draw();
       }
       for (UInt_t i=0; i<NoOfPads; i++) {
       s2m[i+NoOfPads] = new TBox(0.0,-halfheight+i*padheight,halfwidth,-halfheight+(i+1)*padheight);
       s2m[i+NoOfPads]->SetFillColor(38);
       s2m[i+NoOfPads]->SetFillStyle(3001);
       s2m[i+NoOfPads]->Draw();
       }
       */
    Double_t padheight = halfheight*2./NoOfPads;
    for (UInt_t i=1; i<NoOfPads; i++) {
        TLine* hline = new TLine(-halfwidth,-halfheight+i*padheight,halfwidth,-halfheight+i*padheight);
        hline->Draw();
    }

    TLine* vline = new TLine(0.0,-halfheight,0.0,halfheight);
    vline->SetLineStyle(2);
    vline->Draw();
}	

void S2mdisplay::draw_pad(TString side, UInt_t pad) const {

    if (pad >= NoOfPads) return;

    side.ToLower();

    Double_t padheight = halfheight*2./NoOfPads;
    if (side=="left" || side=="l") {
        TBox* hitpad = new TBox(-halfwidth,halfheight-(pad+1)*padheight,0.0,halfheight-pad*padheight);
        hitpad->SetFillColor(2);
        hitpad->SetFillStyle(3001);
        hitpad->Draw("l");
    } else if (side=="right" || side=="r") {
        TBox* hitpad = new TBox(0.0,halfheight-(pad+1)*padheight,halfwidth,halfheight-pad*padheight);
        hitpad->SetFillColor(2);
        hitpad->SetFillStyle(3001);
        hitpad->Draw("l");
    }
}

void S2mdisplay::InitTree() {

    if (!tree) {
        std::cerr << "TTree not set!" << std::endl;
    } else {
        tree->SetBranchAddress((arm+".s2.lt").Data(),fTHitLeft);
        tree->SetBranchAddress((arm+".s2.rt").Data(),fTHitRight);
        //tree->SetBranchAddress("L.tr.n",&ntrack);	
        tree->SetBranchAddress((arm+".s2.trx").Data(),projx);	
        tree->SetBranchAddress((arm+".s2.try").Data(),projy);	
    }
}

void S2mdisplay::draw_event(Long64_t event) {

    if (!tree) return;

    pad->cd();
    //clear();
    pad->Clear();
    draw_outline();

    Long64_t nevent = tree->GetEntries();	
    if (event>=nevent) return;

    tree->GetEntry(event);

    for (UInt_t i=0; i<NoOfPads; i++) {
        if (fTHitLeft[i]>0) draw_pad("left",i);
        if (fTHitRight[i]>0) draw_pad("right",i);
    }

    for (Int_t i=0; i<ntrack; i++) {
        draw_proj(projx[i],projy[i]);
    }
    //std::cout << ntrack << std::endl;
    //std::cout << projx[0] << std::endl;
    //std::cout << projy[0] << std::endl;
}

void S2mdisplay::draw_proj(Double_t x, Double_t y) {  // x, y are the coordinates of projection in the local S2m coordinate system 

    pad->cd();

    x*=scale;
    y*=scale;

    TText* hit = new TText(-y,-x,"O");
    hit->SetTextAlign(22);
    hit->SetTextColor(kGreen+2);
    hit->SetTextSize(0.05);

    hit->Draw();
}
/*
void S2mdisplay::clear() {

    pad->Clear();

    for (UInt_t i=0; i<NoOfPads; i++) {
        fTHitLeft[i] = fTHitRight[i] = -1;
    }

    //ntrack = 0;
    for (UInt_t i=0; i<MaxNoOfTrks; i++) {
        projx[i] = projy[i] = -1.e38;
    }
}
*/
//----------------------------------------------------------------------------



//--------------------------------FPP-----------------------------------------

const UInt_t FPPdisplay::NoOfStraws = 160;
const UInt_t FPPdisplay::MaxNoOfHits = 50;
const UInt_t FPPdisplay::MaxNoOfTrks = 10;

FPPdisplay::FPPdisplay() : wspace(0.0155), width(0.600), height(2.088), dz(0.0095), scale(500) {

    wspace*=scale;
    width*=scale;
    height*=scale;

    u1straws = new Double_t[MaxNoOfHits];
    nu1straws = 0;
    u2straws = new Double_t[MaxNoOfHits];
    nu2straws = 0;
    u3straws = new Double_t[MaxNoOfHits];
    nu3straws = 0;
    v1straws = new Double_t[MaxNoOfHits];
    nv1straws = 0;
    v2straws = new Double_t[MaxNoOfHits];
    nv2straws = 0;
    v3straws = new Double_t[MaxNoOfHits];
    nv3straws = 0;
    projx = new Double_t[MaxNoOfTrks];
    projy = new Double_t[MaxNoOfTrks];
    //ntrack = 0;

    //v0 = new TVector3(dz*(number-1),0.,dz*(number-1));
    if (tree) InitTree();
}

FPPdisplay::~FPPdisplay() {
    delete [] u1straws;
    delete [] u2straws;
    delete [] u3straws;
    delete [] v1straws;
    delete [] v2straws;
    delete [] v3straws;
    delete [] projx;
    delete [] projy;
    //delete v0;
}

void FPPdisplay::draw_outline() const {

    if (!pad) {
        std::cerr << "TPad not set!" << std::endl;
        return;
    }

    //pad->Draw();

    Double_t halfPadX = width*5./8.;
    Double_t halfPadY = height*5./8.;
    pad->Range(-halfPadX,-halfPadY,halfPadX,halfPadY);
    pad->cd();
    pad->Clear();

    TBox* fpp = new TBox(-width/2.,-height/2.,width/2.,height/2.);
    fpp->SetFillColor(kGreen-8);
    fpp->SetFillStyle(3001);
    fpp->Draw();
}	

void FPPdisplay::draw_straw(FPPdisplay::fppPlane plane, UInt_t straw) const {

    UInt_t straw_number = NoOfStraws+plane/3*16;

    if (straw >= straw_number) return;

    //UInt_t nLongStraw = static_cast<int>((height-width)/wspace)+1;
    UInt_t nLongStraw = static_cast<Int_t>((height-width)/wspace);
    UInt_t low = (straw_number-nLongStraw)/2;
    UInt_t high = low+nLongStraw-1;

    Double_t x1, x2, y1, y2;

    TLine* activestraw = new TLine;

    if (plane==v1 || plane==v2 || plane==v3) {
        if (straw<low) {
            y1 = -height/2.;
            x2 = width/2.;
            x1 = -width/2.+(low-straw-0.5)*wspace;
            y2 = -height/2.+width/2.-x1;
            if (plane==v1 || plane==v3) {
                x1-=0.5*wspace;
                y2+=0.5*wspace;
            }
        } else if (straw>high) {
            y2 = height/2.;
            x1 = -width/2.;
            x2 = width/2.-(straw-high-0.5)*wspace;
            y1 = height/2.-width/2.-x2;
            if (plane==v1 || plane==v3) {
                x2-=0.5*wspace;
                y1+=0.5*wspace;
            }
        } else {
            x1 = -width/2.;
            x2 = width/2.;
            y1 = -height/2.+(straw-low+0.5)*wspace;
            y2 = height/2.-(high-straw+0.5)*wspace;
            if (plane==v1 || plane==v3) {
                y1+=0.5*wspace;
                y2+=0.5*wspace;
            }
        }
        activestraw->SetLineColor(kRed);
    } else if (plane==u1 || plane==u2 || plane==u3) {  
        if (straw<low) {
            y1 = height/2.;
            x2 = width/2.;
            x1 = -width/2.+(low-straw-0.5)*wspace;
            y2 = height/2.-width/2.+x1; 
            if (plane==u1 || plane==u3) {
                x1-=0.5*wspace;
                y2-=0.5*wspace;
            }

        } else if (straw>high) {
            x1 = -width/2.;
            y2 = -height/2.;
            x2 = width/2.-(straw-high-0.5)*wspace;
            y1 = -height/2.+width/2.+x2;
            if (plane==u1 || plane==u3) {
                y1-=0.5*wspace;
                x2-=0.5*wspace;
            }
        } else {
            x1 = -width/2.;
            x2 = width/2.;
            y1 = height/2.-(straw-low+0.5)*wspace;
            y2 = -height/2.+(high-straw+0.5)*wspace;
            if (plane==u1 || plane==u3) {
                y1-=0.5*wspace;
                y2-=0.5*wspace;
            }
        }
        activestraw->SetLineColor(kBlue);
    } else return;

    activestraw->SetX1(x1);
    activestraw->SetY1(y1);
    activestraw->SetX2(x2);
    activestraw->SetY2(y2);

    pad->cd();
    activestraw->Draw();

}

void FPPdisplay::InitTree() {

    if (!tree) {
        std::cerr << "TTree not set!" << std::endl;
    } else {
        TString upl, vpl, nupl, nvpl;
        nupl.Form("Ndata.L.vdc.u%d.wire",number);
        vpl.Form("L.vdc.v%d.wire",number);
        nvpl.Form("Ndata.L.vdc.v%d.wire",number);

        tree->SetBranchAddress((arm+".fpp.u1.straw_c").Data(),u1straws);
        tree->SetBranchAddress(Form("Ndata.%s.fpp.u1.straw_c",arm.Data()),&nu1straws);
        tree->SetBranchAddress((arm+".fpp.u2.straw_c").Data(),u2straws);
        tree->SetBranchAddress(Form("Ndata.%s.fpp.u2.straw_c",arm.Data()),&nu2straws);
        tree->SetBranchAddress((arm+".fpp.u3.straw_c").Data(),u3straws);
        tree->SetBranchAddress(Form("Ndata.%s.fpp.u3.straw_c",arm.Data()),&nu3straws);
        tree->SetBranchAddress((arm+".fpp.v1.straw_c").Data(),v1straws);
        tree->SetBranchAddress(Form("Ndata.%s.fpp.v1.straw_c",arm.Data()),&nv1straws);
        tree->SetBranchAddress((arm+".fpp.v2.straw_c").Data(),v2straws);
        tree->SetBranchAddress(Form("Ndata.%s.fpp.v2.straw_c",arm.Data()),&nv2straws);
        tree->SetBranchAddress((arm+".fpp.v3.straw_c").Data(),v3straws);
        tree->SetBranchAddress(Form("Ndata.%s.fpp.v3.straw_c",arm.Data()),&nv3straws);
        //tree->SetBranchAddress("L.tr.n",&ntrack);	
        tree->SetBranchAddress((arm+".fpp.trx").Data(),projx);	
        tree->SetBranchAddress((arm+".fpp.try").Data(),projy);	
    }
}

void FPPdisplay::draw_event(Long64_t event) {

    if(!tree) return;

    pad->cd();
    //clear();
    pad->Clear();
    draw_outline();

    Long64_t nevent = tree->GetEntries();	
    if (event>=nevent) return;

    tree->GetEntry(event);

    for (Int_t i=0; i<nu1straws; i++) {
        draw_straw(u1,(Int_t)u1straws[i]);
    }
    for (Int_t i=0; i<nu2straws; i++) {
        draw_straw(u2,(Int_t)u2straws[i]);
    }
    for (Int_t i=0; i<nu3straws; i++) {
        draw_straw(u3,(Int_t)u3straws[i]);
    }

    for (Int_t i=0; i<nv1straws; i++) {
        draw_straw(v1,(Int_t)v1straws[i]);
    }
    for (Int_t i=0; i<nv2straws; i++) {
        draw_straw(v2,(Int_t)v2straws[i]);
    }
    for (Int_t i=0; i<nv3straws; i++) {
        draw_straw(v3,(Int_t)v3straws[i]);
    }

    for (Int_t i=0; i<ntrack; i++) {
        draw_proj(projx[i],projy[i]);
    }
}

void FPPdisplay::draw_proj(Double_t x, Double_t y) {  // x, y are the coordinates of projection in the local VDC coordinate system 

    pad->cd();

    x*=scale;
    y*=scale;

    TText* hit = new TText(-y,-x,"O");
    hit->SetTextAlign(22);
    hit->SetTextColor(kGreen+2);
    hit->SetTextSize(0.05);

    hit->Draw();
}
/*
void FPPdisplay::clear() {

    pad->Clear();

    nu1straws = nu2straws = nu3straws = 0;
    nv1straws = nv2straws = nv3straws = 0; 
    for (UInt_t i=0; i<MaxNoOfHits; i++) {
        u1straws[i] = u2straws[i] = u3straws[i] = -1;
        v1straws[i] = v2straws[i] = v3straws[i] = -1;
    }

    //ntrack = 0;
    for (UInt_t i=0; i<MaxNoOfTrks; i++) {
        projx[i] = projy[i] = -1.e38;
    }
}
*/
//----------------------------------------------------------------------------

//----------------------------Calorimeter-------------------------------------

const UInt_t Calodisplay::MaxNoOfClusts=10;
const UInt_t Calodisplay::MaxNoOfTrks=10;

Calodisplay::Calodisplay(const Char_t* name, UInt_t row, UInt_t column, Double_t deltaX, Double_t deltaY) : 
    fname(name), fNRows(row), fNColumns(column), dx(deltaX),
    dy(deltaY), scale(500) {

        dx*=scale;
        dy*=scale;

        nclust = 0;
        nhitblock = 0;
        mult = new Double_t[MaxNoOfClusts];
        nblk = new Double_t[fNRows*fNColumns];
        projx = new Double_t[MaxNoOfTrks];
        projy = new Double_t[MaxNoOfTrks];
        //ntrack = 0;

        //v0 = new TVector3(-0.11,0.0,3.1415);
        if (tree) InitTree();
    }

Calodisplay::~Calodisplay() {
    delete [] mult;
    delete [] nblk;
    delete [] projx;
    delete [] projy;
    //delete v0;
}

void Calodisplay::draw_outline() const {

    if (!pad) {
        std::cerr << "TPad not set!" << std::endl;
        return;
    }

    //pad->Draw();

    Double_t Dx = TMath::Abs(dx);
    Double_t Dy = TMath::Abs(dy);

    Double_t width=Dy*fNColumns;
    Double_t height=Dx*fNRows;
    Double_t halfPadX = width*5./8.;
    Double_t halfPadY = height*5./8.;
    pad->Range(-halfPadX,-halfPadY,halfPadX,halfPadY);
    pad->cd();
    pad->Clear();

    TBox* calo = new TBox(-width/2.,-height/2.,width/2.,height/2.);
    calo->SetFillColor(kGreen-8);
    calo->SetFillStyle(3001);
    calo->Draw();

    for (UInt_t i=1; i<fNRows; i++) {
        TLine* hline = new TLine(-width/2.,-height/2.+i*Dx,width/2.,-height/2.+i*Dx);
        hline->SetLineWidth(2);
        hline->Draw();
    }

    for (UInt_t i=1; i<fNColumns; i++) {
        TLine* vline = new TLine(-width/2.+i*Dy,-height/2.,-width/2.+i*Dy,height/2.);
        vline->SetLineWidth(2);
        vline->Draw();
    }
}	

void Calodisplay::draw_block(UInt_t block, Color_t color) const {

    if (block >= fNRows*fNColumns) return;

    UInt_t col = block/fNRows;
    UInt_t row = block%fNRows;
    draw_block(row,col,color);
}

void Calodisplay::draw_block(UInt_t row, UInt_t col, Color_t color) const {

    if (row>=fNRows || col>=fNColumns) return;

    Double_t width=TMath::Abs(dy)*fNColumns;
    Double_t height=TMath::Abs(dx)*fNRows;
    Double_t y1 = TMath::Sign(height/2.,dx)-row*dx;
    Double_t y2 = TMath::Sign(height/2.,dx)-(row+1)*dx;
    Double_t x1 = TMath::Sign(width/2.,dy)-col*dy;
    Double_t x2 = TMath::Sign(width/2.,dy)-(col+1)*dy;

    TBox* hitblock = new TBox(x1,y1,x2,y2);
    hitblock->SetFillColor(color);
    hitblock->SetFillStyle(3001);
    hitblock->Draw("l");
}

void Calodisplay::InitTree() {

    if (!tree) {
        std::cerr << "TTree not set!" << std::endl;
    } else {
        TString str;
        //str.Form("L.%s.nclust",fname);
        str = arm + "." + fname + ".nclust";
        tree->SetBranchAddress(str,&nclust);
        //str.Form("L.%s.mult",fname);
        str = arm + "." + fname + ".mult";
        tree->SetBranchAddress(str,mult);
        //str.Form("L.%s.nhitblock",fname);
        str = arm + "." + fname + ".nhitblock";
        tree->SetBranchAddress(str,&nhitblock);
        //str.Form("L.%s.nblk",fname);
        str = arm + "." + fname + ".nblk";
        tree->SetBranchAddress(str,nblk);
        //str.Form("L.%s.trx",fname);
        str = arm + "." + fname + ".trx";
        tree->SetBranchAddress(str,projx);
        //str.Form("L.%s.try",fname);
        str = arm + "." + fname + ".try";
        tree->SetBranchAddress(str,projy);
        //tree->SetBranchAddress("L.tr.n",&ntrack);	
    }
}

void Calodisplay::draw_event(Long64_t event) {

    if (!tree) return;

    pad->cd();
    //clear();
    pad->Clear();
    draw_outline();

    Long64_t nevent = tree->GetEntries();	
    if (event>=nevent) return;

    tree->GetEntry(event);

    Int_t count=0;
    for (UInt_t i=0; i<nclust; i++) {
        for (UInt_t j=0; j<mult[i]; j++) {
            if (j==0) {
                if (i==0) draw_block(nblk[count++], kRed); // Main block of main cluster is in red
                else draw_block(nblk[count++], kBlue); // Other block of main cluster is in gray
            }
            else draw_block(nblk[count++], 16);
        }
    }
    //assert(count==(Int_t)nhitblock);

    for (Int_t i=0; i<ntrack; i++) {
        draw_proj(projx[i],projy[i]);
    }
}

void Calodisplay::draw_proj(Double_t x, Double_t y) {  // x, y are the coordinates of projection in the local Calorimeter coordinate system 

    pad->cd();

    x*=scale;
    y*=scale;

    TText* hit = new TText(-y,-x,"O");
    hit->SetTextAlign(22);
    hit->SetTextColor(kGreen+2);
    hit->SetTextSize(0.05);

    hit->Draw();
}
/*
void Calodisplay::clear() {

    pad->Clear();

    nclust = nhitblock = 0;
    for (UInt_t i=0; i<MaxNoOfClusts; i++) mult[i] = 0;
    for (UInt_t i=0; i<fNRows*fNColumns; i++) nblk[i] = -1;

    //ntrack = 0;
    for (UInt_t i=0; i<MaxNoOfTrks; i++) projx[i] = projy[i] = -1.e38;
}
*/

//---------------------------Other routines-------------------------------------------------
TFile* GetROOTFile() {

	int runno;
	TString filename;
	TFile* file = 0;

	std::string line;
	std::istringstream buffer;
	std::cout << "Which run to display (run no): " << std::endl;
	while (std::getline(std::cin,line)) {
		buffer.clear();
		buffer.str(line);
		if (!(buffer >> runno)) {
			std::cerr << "Invalid input." << std::endl;
			std::cerr << "Input another run number or -1 to exit: " << std::endl;
			continue;
		} else {
			if (runno<0) {
				std::cout << "Program terminating." << std::endl;
				return 0;
			} else {
				filename = ROOTfilePATH;
				if (runno<20000) filename.Append(Form("left_gmp_%d.root",runno));
				else 		 filename.Append(Form("right_gmp_%d.root",runno));
				file = new TFile(filename);
				if (file->IsZombie()) {
					std::cerr << "Cannot open this ROOT file." << std::endl
						<< "Input another run number or -1 to exit: " << std::endl;
				} else break;
			}
		}
	}
	
	return file;
}

TString GetCuts() {
	TString cut = "";
	std::string line;
	std::cout << "Please specify the cuts to select events (press enter to skip any cuts): " << std::endl;
	std::getline(std::cin,line);
	cut += line.c_str();
	return cut;
}

void EventDisplay(TFile* file, const char* displaycut="") {

	if (!file) {
		std::cerr << "ROOT file is not specified in EventDisplay." << std::endl;
		std::cerr << "Program terminating." << std::endl;
		return;
	}

	TString rootfilename = file->GetName();
	TString prefix( rootfilename(rootfilename.Last('/')+1,rootfilename.Last('.')-rootfilename.Last('/')-1) );

	TTree* T = (TTree*)file->Get("T");

	TCut cut = displaycut;
	TEntryList* eList= new TEntryList("elist","Event list for display");
	T->Draw(">>elist",cut,"entrylist");
	eList->Print();

	//T->SetEntryList(eList);
	//std::cout << T->GetEntries() << std::endl;
	//std::cout << eList->GetN() << std::endl;
	//std::cout << eList->Next() << std::endl;

	//std::cout << eList->GetEntry(10) << std::endl;

	//if (eList->Contains(9435)) std::cout << "Yes!" << std::endl;
	//else std::cout << "No!" << std::endl;

	//std::cout << eList->Next() << std::endl;

	EvDisplay* evDisplay = new EvDisplay;
	evDisplay->Init(T);

	evDisplay->Add( new VDCdisplay(1) );
	evDisplay->Add( new VDCdisplay(2) );
	evDisplay->Add( new FPPdisplay );
	evDisplay->Add( new S2mdisplay );
	if (T->GetBranch("L.tr.n")) {
	    evDisplay->Add( new Calodisplay("prl1",17,2,-0.15,-0.325) );
	    evDisplay->Add( new Calodisplay("prl2",17,2,-0.15,-0.325) );
	}
	if (T->GetBranch("R.tr.n")) {
	    evDisplay->Add( new Calodisplay("ps",24,2,0.10,0.35) );
	    evDisplay->Add( new Calodisplay("sh",15,5,0.15,0.15) );
	}

	Long64_t nevents = T->GetEntries();
	Long64_t event;


	std::cout << "Please enter an event number in the tree or hit enter to go to next event, enter -1 to exit: ";

	std::string line;
	std::istringstream buffer;
	while (std::getline(std::cin,line)) {
		//if (line.length()==0) event = eList->Next();
		if (line.empty()) event = eList->Next();
		else {
			buffer.clear();
			buffer.str(line);
			Long64_t testnumber;
			if (!(buffer >> testnumber)) {
				std::cerr << "Invalid input." << std::endl;
				std::cerr << "Please enter an event number in the tree or hit enter to go to next event, enter -1 to exit: ";
				continue;
			} else {
				if (testnumber < 0) break;
				if (testnumber >= nevents) {
					std::cerr << "Event number is too big." << std::endl;
					std::cerr << "Please enter an event number in the tree or hit enter to go to next event, enter -1 to exit: " ;
					continue;
				}
				if (!eList->Contains(testnumber)) {
					std::cerr << "Event number does not pass the cut." << std::endl;
					std::cerr << "Please enter an event number in the tree or hit enter to go to next event, enter -1 to exit: " ;
					continue;
				}
				event = testnumber;
			}
		}
		std::cout << "Now display event " << event << std::endl;
		//vdc1->draw_event(event);
		//vdc2->draw_event(event);
		//fpp->draw_event(event);
		//s2m->draw_event(event);
		//calo->draw_event(event);
		//VDisplay::Process(event);
		evDisplay->Process(event);

		std::cout << "Save the plot? (default = no) " << std::endl;
		std::getline(std::cin,line);
		TString str = line.c_str();
		str.ToLower();
		while (!str.IsNull() && !(str=="yes" || str=="y" || str=="no" || str=="n")) {
			std::cerr << "Invalid input." << std::endl;
			std::cerr << "Save the plot? (default = no) " << std::endl;
			std::getline(std::cin,line);
			str = line.c_str();
			str.ToLower();
		}
		if (str=="yes" || str=="y") {
			std::ostringstream plotfilename;
			//filename << "plots/" << runno << "_" << event << ".svg";
			//plotfilename << "plots/" << prefix << "_" << event << ".svg";
			plotfilename << PathOfPlots << "/" << prefix << "_" << event << ".svg";
			evDisplay->SavePlot(plotfilename.str().c_str());
		}
		std::cout << "Please enter an event number in the tree or hit enter to go to next event, enter -1 to exit: ";
	}

	delete evDisplay;
}
