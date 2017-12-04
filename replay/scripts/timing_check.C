// Bane-> Timing plots for scintilators and trigger

void timing_check(Int_t flag, TString arm, TString side, Int_t options=0, Int_t trig =1,Int_t full=1){

	
	//flag 1 -> diff in s2coinc and s2 r/l)
	//flag 2 -> diff in S0coinc and S0 A/B)
	//flag 3 -> diff cher sum TDC and S2coinc)
	//flag 4 -> diff cher sum TDC and s0coinc)
	
	gStyle->SetOptStat(0);
	TTree *T =(TTree*)gDirectory->Get("T");	
	


	if(full!=1){int ttt =1;}
		 
	switch (flag){
	
		case 1: {
			TH1F *h1 = new TH1F("h1",Form(" Difference in S2 Coinc. and the total s2%s tdc.",side.Data()),1500,-500,2500);
			TH1F *h1_1 = new TH1F("h1_1",Form(" Difference in S2 Coinc. and the total s2%s tdc .",side.Data() ) ,1500 ,-500,2500);
			T->Draw(Form("D%s.tS2coinc-%s.s2.%st>>h1",arm.Data(),arm.Data(),side.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
			h1->SetLineColor(2);//h1->SetFillStyle(3001);h1->SetFillColor(2);
			h1->GetXaxis()->SetTitle("TDC channel diff.");
			T->Draw(Form("D%s.tS2coinc-%s.s2.%st>>h1_1",arm.Data(),arm.Data(),side.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig-2),"same");
			h1_1->SetLineColor(4);
			
			TLegend *key = new TLegend(0.7,0.7,0.9,0.9);
				key->AddEntry(h1,Form("Trigger %d",trig), "l");
				key->AddEntry(h1_1,Form("Trigger %d",trig-2), "l");
				key->Draw("same");

			break;
				}
		case 2: {
			TH1F *h2 = new TH1F("h2",Form(" Difference in S0 Coinc. and s0%s tdc for trig. %d.",side.Data(),trig),2500,-2500,2500);
			T->Draw(Form("D%s.tS0coinc-%s.s0.%st[0]>>h2",arm.Data(),arm.Data(),side.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
			h2->GetXaxis()->SetTitle("TDC channel diff.");
			break;
				}
		case 3: {
			TH1F *h3 = new TH1F("h3",Form(" Difference in Cherkov sum and S2coinc tdc for trig. %d.",trig),2000,-4000,4000);
			T->Draw(Form("D%s.tGCSum-D%s.tS2coinc>>h3",arm.Data(),arm.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
			h3->GetXaxis()->SetTitle("TDC channel diff.");
			break;
				}
		case 4: {
			TH1F *h4 = new TH1F("h4",Form(" Difference in Cherkov sum and S0coinc tdc for trig. %d.",trig),2000,-4000,4000);
			T->Draw(Form("D%s.tGCSum-D%s.tS0coinc>>h4",arm.Data(),arm.Data()),Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig));
						h4->GetXaxis()->SetTitle("TDC channel diff.");
			break;
				}								
		case 5: {
		TH1F *s0A;TH1F *s0B;
		  if(arm=='L'){
			 s0A= new TH1F("s0A",Form("S0A and S0B for s2 pmt %d", options),400,1400,1800);
			 s0B= new TH1F("s0B",Form("S0B for s2 pmt %d", options),400,1400,1800);
			 }
		  else{	
			 s0A= new TH1F("s0A",Form("S0A and S0B for s2 pmt %d", options),400,1600,2000);
			 s0B= new TH1F("s0B",Form("S0B for s2 pmt %d", options),400,1600,2000);
			 }
			TCut pmt = Form("%s.s2.lt[%d]>=100&&%s.s2.rt[%d]>=100",arm.Data(),options,arm.Data(),options);	

			TCut oneT = Form("%s.tr.n==1",arm.Data());
			s0B->SetLineColor(2);
			s0A->SetFillStyle(3001);
			T->Draw(Form("%s.s0.lt[0]>>s0A",arm.Data()),pmt&&oneT&&Form("%s.s0.lt[0]>10",arm.Data())&&Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig),"");
			T->Draw(Form("%s.s0.rt[0]>>s0B",arm.Data()),pmt&&oneT&&Form("%s.s0.rt[0]>10",arm.Data())&&Form("D%s.evtypebits&(1<<%d)",arm.Data(),trig),"same");
			TLegend *key5 = new TLegend(0.1,0.7,0.3,0.9);
				key5->AddEntry(s0A,"s0.lt", "l");
				key5->AddEntry(s0B,"s0.rt", "l");
				key5->Draw("same");
			double mean1 = s0A->GetMean();	double mean2 = s0B->GetMean();
			double diff = mean1 - mean2;
				
			double max1 = s0A->GetBinContent(s0A->GetMaximumBin());
			double max2 = s0B->GetBinContent(s0B->GetMaximumBin());	
			double max = max1; if(max1<max2){max=max2;}
			TLatex *tex = new TLatex( s0A->GetBinCenter(1)+2,max*0.50,Form("Peak seperation \n %0.1f ",diff));
			tex->Draw("same");
			
			if(s0A->GetEntries()<=3||s0B->GetEntries()<=3){
				TImage *img = TImage::Open("/adaqfs/home/a-onl/tritium/replay/onlineGUI64/awe.jpg");
					//img->Draw();
					}	
			
			break;
				}	
			
			
	//	default: {break;}
		}

}
