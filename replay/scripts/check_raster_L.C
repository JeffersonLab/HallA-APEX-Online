// compare raster pattern with a good run (3741)
// Shujie Li 2018.10


#include "headers/SQLanalysis.h"
void check_raster_L(Int_t runnum1, Int_t runnum2=3741){
	gStyle->SetOptStat(0);
	TChain *t1 = LoadOnline(runnum1);
	TChain *t2 = LoadOnline(runnum2);
	TCanvas *c1 = new TCanvas("c1","",800,600);
	c1->Divide(2,2);
	c1->cd(1);
 	TH1F   *rxnew = new TH1F("rxnew","raster raw current x",100,60000,95000); 
	TH1F   *rxgood = new TH1F("rxgood","",100,60000,95000);
	rxnew -> SetLineColor(kRed); 
	t1->Draw("Lrb.Raster.rawcur.x>>rxnew");
	t2->Draw("Lrb.Raster.rawcur.x>>rxgood","","same");
	auto legend = new TLegend(0.35,0.7,0.65,0.9);
	legend->AddEntry(rxgood,"run 3741");
	legend->AddEntry(rxnew,"this run");
	legend->Draw();

c1->cd(2);
 	TH1F   *rynew = new TH1F("rynew","raster raw current y",100,60000,90000); 
	TH1F   *rygood = new TH1F("rygood","",100,60000,90000); 
	rynew -> SetLineColor(kRed);
	t1->Draw("Lrb.Raster.rawcur.y>>rynew");
	t2->Draw("Lrb.Raster.rawcur.y>>rygood","","same");
	c1->cd(3);
	TH2F   *rgood = new TH2F("rgood","run 3741",100,60000,95000,100,60000,95000);
	t2->Draw("Lrb.Raster.rawcur.y:Lrb.Raster.rawcur.x>>rgood","","colz");
	rgood->SetXTitle("Lrb.Raster.rawcur.x");
	rgood->SetYTitle("Lrb.Raster.rawcur.y");

	c1->cd(4);
	TH2F   *rnew = new TH2F("rnew","this run",100,60000,95000,100,60000,95000);
	t1->Draw("Lrb.Raster.rawcur.y:Lrb.Raster.rawcur.x>>rnew","","colz");

	rnew->SetXTitle("Lrb.Raster.rawcur.x");
	rnew->SetYTitle("Lrb.Raster.rawcur.y");


}
