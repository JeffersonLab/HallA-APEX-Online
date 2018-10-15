//beta distribution
//Tritium (e,e'p) SRC experiment
//Reynier Cruz Torres, Apr 2018
void beta_dist(Int_t flag, TString drawoption){
	gStyle->SetOptStat(0);
	TTree *tree = (TTree*)gDirectory->Get("T");

	// ---------------------------------------------------------------------------------
	// LHRS
	if(flag ==1){
		TH1F *h = new TH1F("h","#beta;#beta",100,-1.5,2.5);
		h->GetYaxis()->CenterTitle();	h->GetXaxis()->CenterTitle();
		tree->Draw("L.tr.beta[0]>>h","L.tr.n==1",drawoption);
	}
	// ---------------------------------------------------------------------------------
	// RHRS
	else if(flag ==2){
		TH1F *h1 = new TH1F("h1","#beta;#beta",100,-1.5,2.5);
		h1->GetYaxis()->CenterTitle();	h1->GetXaxis()->CenterTitle();
		tree->Draw("R.tr.beta[0]>>h1","R.tr.n==1",drawoption);
	}
	// ---------------------------------------------------------------------------------
}
