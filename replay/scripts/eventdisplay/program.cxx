#include <iostream>
#include <string>
#include <sstream>

#include "TCanvas.h"
#include "TPad.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TEntryList.h"
#include "TCut.h"
#include "TRint.h"

#include "eventdisplay.h"

TFile* GetROOTFile();
TString GetCuts();
void EventDisplay(TFile*, const char*);

int main(int argc, char** argv) {
	TRint* myapp = new TRint("RootSession",&argc,argv,NULL,0);
	TFile* rootfile = GetROOTFile();
	if (rootfile) {
		EventDisplay(rootfile,GetCuts().Data());
	}
	myapp->Run();
	return 0;
}
