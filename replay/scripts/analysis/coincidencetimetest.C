//Check if Signal is inside Gate
//Barak Schmookler, Feb 2016
void coincidencetimetest(Int_t flag=1, Char_t *drawoption){

  gStyle->SetOptStat(0);

  TTree *tree = (TTree*)gDirectory->Get("T");

 	if (flag == 1) {
    TH1F *ht10 = new TH1F("ht10","S0l_LEFT - S0l_RIGHT retimed",200,-500,2000);
    ht10->GetXaxis()->SetTitle("");ht10->GetXaxis()->CenterTitle();
//     ht10->GetYaxis()->SetTitle("S2L6 TDC RHRS retimed for LHRS");ht10->GetYaxis()->CenterTitle();
    ht10->SetMarkerStyle(3);ht10->SetMarkerSize(0.75);

    tree->Draw("L.s0.lt[0]-R.s0.lt[0]+DR.Retiming_L[0]>>ht10","((DR.evtypebits>>1)&1) && L.s0.la_p>50 && R.s0.la_p>50",drawoption);
	}
	if (flag == 2) {
    TH1F *ht11 = new TH1F("ht11","Retiming L",2000,-400,1600);
    ht11->GetXaxis()->SetTitle("Retiming Signal from L");ht11->GetXaxis()->CenterTitle();


    tree->Draw("DR.Retiming_L[0]>>ht11","((DR.evtypebits>>1)&1)",drawoption);
	}
	if (flag == 3) {
    TH1F *ht12 = new TH1F("ht12","Diff S0l_LEFT - S0l_RIGHT",200,-500,1500);
    ht12->GetXaxis()->SetTitle("");ht12->GetXaxis()->CenterTitle();


    tree->Draw("L.s0.lt[0]-R.s0.lt[0]>>ht12","((DR.evtypebits>>1)&1) && L.s0.la_p>50 && R.s0.la_p>50",drawoption);
	}

}
