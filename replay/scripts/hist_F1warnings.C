int hist_F1warnings(Int_t run) {

	TString fname = Form("t2root/Rootfiles/tritium_%d.root",run);
	//TString fname = Form("/adaqfs/home/a-onl/tritium/replay/Rootfiles_chafs/tritium_%d.root",run);

	TChain *T = new TChain("T","T");
	T->Add(fname);

	// Switch off all other branches to speed up things
	T->SetBranchStatus("*",0);
	// Switch on only those branches we use
	T->SetBranchStatus("F1Warnings",1);
	
	// Prepare to read branch
	Double_t f1w[64];
	T->SetBranchAddress("F1Warnings",f1w);

	// Prepare histogram
	TH1F *hHitOflw = new TH1F("hHitOflw",Form("run %d;F1 channel;counts",run),64,0,64);
	TH1F *hOutOflw = new TH1F("hOutOflw","",64,0,64);
	TH1F *hResLock = new TH1F("hResLock","",64,0,64);

	Int_t n=0; // number of events with warnings
	for(Int_t i=0; i<T->GetEntries(); i++) {
		T->GetEntry(i);
		
		Int_t nch = 0; // number of channels with warnings in the evt
		for(Int_t j=0; j<64; j++) {
			if(f1w[j] > 0) {
			nch++;
			// check for HitOverflow (1st bit is 1)
			if((((int)f1w[j])>>0)&1U) hHitOflw->Fill(j);
			// check for OutputOverflow (2nd bit is 1)
			if((((int)f1w[j])>>1)&1U) hOutOflw->Fill(j);
			// check for ResLock failure (3rd bit is 1)
			if((((int)f1w[j])>>2)&1U) hResLock->Fill(j);
			}
		}

		if(nch>0) n++; // count this event as having warning
	}

	// Fraction of events with warnings
	cout << "Run: " << run << endl;
	cout << "Events with warnings from the total of " << T->GetEntries() << ":" << endl;
	cout << "- " << hHitOflw->GetEntries() << " (" << (100.0*hHitOflw->GetEntries())/T->GetEntries() << "%) with Hit-FIFO overflow" << endl;
	cout << "- " << hOutOflw->GetEntries() << " (" << (100.0*hOutOflw->GetEntries())/T->GetEntries() << "%) with Output FIFO overflow" << endl;
	cout << "- " << hResLock->GetEntries() << " (" << (100.0*hResLock->GetEntries())/T->GetEntries() << "%) with Resolution lock failure" << endl;

	// Scale
	//hHitOflw->Scale(100.0/T->GetEntries());
	//hOutOflw->Scale(100.0/T->GetEntries());
	//hResLock->Scale(100.0/T->GetEntries());

	// Get maximum to adjust scale
	double max;
	max = TMath::Max((double)hHitOflw->GetMaximum(),(double)hOutOflw->GetMaximum());
	max = 1.2*TMath::Max((double)max,(double)hResLock->GetMaximum());

	TCanvas *c0 = new TCanvas("c0","c0");
	c0->SetTicks(1,1);
	gStyle->SetOptStat(0);
	hHitOflw->SetLineColor(4);
	hHitOflw->GetYaxis()->SetRangeUser(0,max);
	hHitOflw->Draw();
	hOutOflw->SetLineColor(3);
	hOutOflw->Draw("same");
	hResLock->SetLineColor(2);
	hResLock->Draw("same");
	c0->Update();
	TLatex l;
	l.DrawLatex(5,0.95*max,Form("#color[4]{Hit-FIFO overflow: %.2f%% of evts}",(100.0*hHitOflw->GetEntries())/T->GetEntries()));
	l.DrawLatex(5,0.9*max,Form("#color[3]{Output FIFO overflow: %.2f%% of evts}",(100.0*hOutOflw->GetEntries())/T->GetEntries()));
	l.DrawLatex(5,0.85*max,Form("#color[2]{Resolution lock failure: %.2f%% of evts}",(100.0*hResLock->GetEntries())/T->GetEntries()));
	c0->SaveAs(Form("F1warnings_%d.png",run));

	return 0;
}
