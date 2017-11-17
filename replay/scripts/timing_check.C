// Bane-> Timing plots for scintilators and trigger

void timing_check(Int_t flag, TString arm, TString side,Int_t full=1, Int_t options=0, Int_t trig =1){

	
	//flag 1 -> diff in s2coinc and s2 r/l)
	//flag 2 -> diff in S0coinc and S0 A/B)
	//flag 3 -> diff cher sum TDC and S2coinc)
	//flag 4 -> diff cher sum TDC and s0coinc)
	
	gStyle->SetOptStat(0);
	TTree *T =(TTree*)gDirectory->Get("T");	
	
	trig=3;	

	if(full!=1){int ttt =1;}
		 
	switch (flag){
	
		case 1: {
			TH1F *h1 = new TH1F("h1",Form(" Difference in S2 Coinc. and the total s2%s tdc.",side.Data()),3000,-500,2500);
			TH1F *h1_1 = new TH1F("h1_1",Form(" Difference in S2 Coinc. and the total s2%s tdc .",side.Data() ) ,3000 ,-500,2500);
			T->Draw(Form("D%s.tS2coinc-%s.s2.%st>>h1",arm.Data(),arm.Data(),side.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),3));
			h1->SetLineColor(2);h1->SetFillStyle(3001);h1->SetFillColor(2);
			h1->GetXaxis()->SetTitle("TDC channel diff.");
			T->Draw(Form("D%s.tS2coinc-%s.s2.%st>>h1_1",arm.Data(),arm.Data(),side.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),1),"same");
			h1_1->SetLineColor(4);
			
			TLegend *key = new TLegend(0.7,0.7,0.9,0.9);
				key->AddEntry(h1,"Trigger 3", "l");
				key->AddEntry(h1_1,"Trigger 1", "l");
				key->Draw("same");

			break;
				}
		case 2: {
			TH1F *h2 = new TH1F("h2",Form(" Difference in S0 Coinc. and s0%s tdc for trig. %d.",side.Data(),trig),3000,-500,2500);
			T->Draw(Form("D%s.tS0coinc-%s.s0.%st[0]>>h2",arm.Data(),arm.Data(),side.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
			h2->GetXaxis()->SetTitle("TDC channel diff.");
			break;
				}
		case 3: {
			TH1F *h3 = new TH1F("h3",Form(" Difference in Cherkov sum and S2coinc tdc for trig. %d.",trig),4000,-2000,2000);
			T->Draw(Form("D%s.tGCSum-D%s.tS2coinc>>h3",arm.Data(),arm.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
			h3->GetXaxis()->SetTitle("TDC channel diff.");
			break;
				}
		case 4: {
			TH1F *h4 = new TH1F("h4",Form(" Difference in Cherkov sum and S0coinc tdc for trig. %d.",trig),4000,-2000,2000);
			T->Draw(Form("D%s.tGCSum-D%s.tS0coinc>>h4",arm.Data(),arm.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
						h4->GetXaxis()->SetTitle("TDC channel diff.");
			break;
				}								
				
	//	default: {break;}
		}

}
