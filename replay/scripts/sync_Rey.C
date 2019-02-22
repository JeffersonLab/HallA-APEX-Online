//Synchronization plots
//Reynier Cruz Torres, Feb 2017
void sync_Rey(Int_t flag){
	gStyle->SetOptStat(0);

	TTree *tree = (TTree*)gDirectory->Get("T");


	TString input;
	if     (flag==2){input = "DR.syncroc1:DR.syncroc2"  ;}
	else if(flag==3){input = "DR.syncroc1:DR.syncroc3"  ;}
	else if(flag==4){input = "DR.syncroc1:DR.syncroc4"  ;}
	else if(flag==5){input = "DR.syncroc1:DR.syncroc5"  ;}
	else if(flag==6){input = "DR.syncroc1:DR.syncroc6"  ;}
	else if(flag==7){input = "DR.syncroc1:DR.syncroc7"  ;}
	else if(flag==8){input = "DR.syncroc1:DR.syncroc8"  ;}
	else if(flag==9){input = "DR.syncroc1:DR.syncroc9"  ;}

	T->Draw(input,"");

}
