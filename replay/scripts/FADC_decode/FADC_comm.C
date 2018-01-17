#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <numeric>
#include <vector>
#include <time.h>
#include "Decoder.h"
#include "THaCodaFile.h"
#include "CodaDecoder.h"
#include "THaEvData.h"
#include "Module.h"
#include "Fadc250Module.h"
#include "evio.h"
#include "THaSlotData.h"
#include "TString.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TDirectory.h"
#include "TGraph.h"
#include "TVector.h"
#include "TMultiGraph.h"
#include "TRandom3.h"
#include "TCanvas.h"

//#define DEBUG
//#define WITH_DEBUG
#define SLOTMIN 1
#define NUMSLOTS 22
#define NADCCHAN 16
#define NUMRAWEVENTS 500
#define NPED 15

using namespace std;
using namespace Decoder;

TFile *hfile;
TDirectory *mode_dir, *slot_dir[NUMSLOTS], *chan_dir[NADCCHAN], *raw_samples_dir[NADCCHAN];
TH1I *h_pinteg[NUMSLOTS][NADCCHAN], *h_ptime[NUMSLOTS][NADCCHAN], *h_pped[NUMSLOTS][NADCCHAN], *h_ppeak[NUMSLOTS][NADCCHAN];
TH2I *h2_pinteg[NUMSLOTS], *h2_ptime[NUMSLOTS], *h2_pped[NUMSLOTS], *h2_ppeak[NUMSLOTS];
TGraph *g_psamp_event[NUMSLOTS][NADCCHAN][NUMRAWEVENTS];
TCanvas *c_psamp[NUMSLOTS][NADCCHAN];
TMultiGraph *mg_psamp[NUMSLOTS][NADCCHAN];
vector<uint32_t> raw_samples_vector[NUMSLOTS][NADCCHAN];
uint32_t raw_samp_index[NUMSLOTS][NADCCHAN];

static int fadc_mode_const;

/* parameters for pulse quality checking */
int win_size;
Int_t noverflow[NUMSLOTS][NADCCHAN],nunderflow[NUMSLOTS][NADCCHAN];//pulse quality checking
Int_t nbadped[NUMSLOTS][NADCCHAN]; //pedestal sum quality checking
Int_t nchanevents[NUMSLOTS][NADCCHAN];
Double_t poverflow[NUMSLOTS][NADCCHAN],punderflow[NUMSLOTS][NADCCHAN],pbadped[NUMSLOTS][NADCCHAN];

void GeneratePlots(Int_t mode, uint32_t islot, uint32_t chan) {
  // Mode Directory
  mode_dir = dynamic_cast <TDirectory*> (hfile->Get(Form("mode_%d_data", mode)));
  if(!mode_dir) {mode_dir = hfile->mkdir(Form("mode_%d_data", mode)); mode_dir->cd();}
  else hfile->cd(Form("/mode_%d_data", mode));
  // Slot directory
  slot_dir[islot] = dynamic_cast <TDirectory*> (mode_dir->Get(Form("slot_%d", islot)));
  if (!slot_dir[islot]) {slot_dir[islot] = mode_dir->mkdir(Form("slot_%d", islot)); slot_dir[islot]->cd();}
  else hfile->cd(Form("/mode_%d_data/slot_%d", mode, islot));
  if (mode != 1) {
    if (!h2_pinteg[islot]) h2_pinteg[islot] = new TH2I("h2_pinteg", Form("FADC Mode %d Pulse Integral Data Slot %d; Channel Number; FADC Units (Channels)", mode, islot), 16, -0.5, 15.5, 65000, 15000, 80000);
    if (!h2_ptime[islot]) h2_ptime[islot]   = new TH2I("h2_ptime", Form("FADC Mode %d Pulse Time Data Slot %d; Channel Number; FADC Units (Channels)", mode, islot), 16, -0.5, 15.5, 40096, 0, 40095);
    if (!h2_pped[islot]) h2_pped[islot]     = new TH2I("h2_pped", Form("FADC Mode %d Pulse Pedestal Data Slot %d; Channel Number; FADC Units (Channels)", mode, islot), 16, -0.5, 15.5, 4096, 0, 4095);
    if (!h2_ppeak[islot]) h2_ppeak[islot]   = new TH2I("h2_ppeak", Form("FADC Mode %d Pulse Peak Data Slot %d; Channel Number; FADC Units (Channels)", mode, islot), 16, -0.5, 15.5, 4096, 0, 4095);
  }
  // Channel directory
  chan_dir[chan] = dynamic_cast <TDirectory*> (slot_dir[islot]->Get(Form("chan_%d", chan)));
  if (!chan_dir[chan]) {chan_dir[chan] = slot_dir[islot]->mkdir(Form("chan_%d", chan)); chan_dir[chan]->cd();}
  else hfile->cd(Form("/mode_%d_data/slot_%d/chan_%d", mode, islot, chan));
  // Histos
  if (!h_pinteg[islot][chan]) h_pinteg[islot][chan] = new TH1I("h_pinteg", Form("FADC Mode %d Pulse Integral Data Slot %d Channel %d; FADC Units (Channels); Counts / 10 Channels", mode, islot, chan), 10000, 0, 210000);
  if (mode != 1) {
    if (!h_ptime[islot][chan]) h_ptime[islot][chan]  = new TH1I("h_ptime",  Form("FADC Mode %d Pulse Time Data Slot %d Channel %d; FADC Units (Channels); Counts / Channel", mode, islot, chan), 40096, 0, 40095);
    if (!h_pped[islot][chan])  h_pped[islot][chan]   = new TH1I("h_pped",   Form("FADC Mode %d Pulse Pedestal Data Slot %d Channel %d; FADC Units (Channels); Counts / Channel", mode, islot, chan), 4096, 0, 4095);
    if (!h_ppeak[islot][chan]) h_ppeak[islot][chan]  = new TH1I("h_ppeak",  Form("FADC Mode %d Pulse Peak Data Slot %d Channel %d; FADC Units (Channels); Counts / Channel", mode, islot, chan), 4096, 0, 4095);
  }
  // Graphs
  if (!mg_psamp[islot][chan] && ((mode == 1) || (mode == 8) || (mode == 10))) {
    mg_psamp[islot][chan] = new TMultiGraph("samples", "samples");
  }
  // Canvas'
  if (!c_psamp[islot][chan] && ((mode == 1) || (mode == 8) || (mode == 10))) {
    c_psamp[islot][chan] = new TCanvas(Form("c_psamp_slot_%d_chan_%d", islot, chan), Form("c_psamp_slot_%d_chan_%d", islot, chan), 1600, 1600);
    c_psamp[islot][chan]->Divide(5, 5);
  }
  // Raw samples directoy & graphs
  if ((mode == 1) || (mode == 8) || (mode == 10)) {   
    raw_samples_dir[chan] = dynamic_cast <TDirectory*> (chan_dir[chan]->Get("raw_samples"));
    if (!raw_samples_dir[chan]) {raw_samples_dir[chan] = chan_dir[chan]->mkdir("raw_samples"); raw_samples_dir[chan]->cd();}
    else hfile->cd(Form("/mode_%d_data/slot_%d/chan_%d/raw_samples", mode, islot, chan));
    for (uint32_t raw_index = 0; raw_index < NUMRAWEVENTS; raw_index++) {
      if (!g_psamp_event[islot][chan][raw_index]) {
	g_psamp_event[islot][chan][raw_index] = new TGraph();
	g_psamp_event[islot][chan][raw_index]->SetName(Form("g_psamp_event_%d", raw_index));
      }
    }
  }
}

Int_t SumVectorElements(vector<uint32_t> data_vector) {
  Int_t sum_of_elements = 0;
  sum_of_elements = accumulate(data_vector.begin(), data_vector.end(), 0);
  return sum_of_elements;
}

int main(int argc, char* argv[])
{

  Int_t runNumber = 0;
  Int_t maxEvent = 0;
  Int_t crateNumber = 0;
  TString spectrometer = "";
  TString runFile, rootFile;

  // Acquire run number, max number of events, spectrometer name, and crate number
  if (runNumber == 0) {
    cout << "\nEnter a Run Number (-1 to exit): ";
    cin >> runNumber;
    if (runNumber <= 0) {
      cerr << "...Invalid entry\n" << endl;; 
      return 1;
    }
  }
  if (maxEvent == 0) {
    cout << "\nEnter Number of Events to Analyze (-1 = All): ";
    cin >> maxEvent;
    if (maxEvent <= 0 && maxEvent != -1) {
      cerr << "...Invalid entry\n" << endl;;
      return 1;
    }
    if (maxEvent == -1)
      cout << "\nAll Events in Run " << runNumber << " Will be Analayzed" << endl;
  }
  if (spectrometer = "") {
    cout << "\nEnter the Spectrometer Name (LHRS or RHRS): ";
    cin >> spectrometer;
    if (spectrometer != "LHRS" && spectrometer != "RHRS") {
      cerr << "...Invalid entry\n"; 
      return 1;
    }
  }
  if (crateNumber == 0) {
    cout << "\nEnter the Crate Number to be Analyzed: ";
    cin >> crateNumber;
    if (crateNumber <= 0) {
      cerr << "...Invalid entry\n"; 
      return 1;
    }
  }

  // Create file name patterns
  if (spectrometer == "LHRS")
    runFile = Form("/adaq1/data1/triton_%d.dat.0", runNumber);
  if (spectrometer == "RHRS")
    runFile = Form("/adaq1/data1/triton_%d.dat.0", runNumber);
  rootFile = Form("/chafs1/work1/tritium/FADC_rootfiles/fadc_%d.root", runNumber);

  // Initialize raw samples index array
  memset(raw_samp_index, 0, sizeof(raw_samp_index));
  memset(noverflow, 0, sizeof(noverflow));
  memset(nunderflow, 0, sizeof(nunderflow));
  memset(nbadped, 0, sizeof(nbadped));
  memset(nchanevents, 0, sizeof(nchanevents));
  memset(poverflow, 0.0, sizeof(poverflow));
  memset(punderflow, 0.0, sizeof(punderflow));
  memset(pbadped, 0.0, sizeof(pbadped));

  win_size=0;
 
  // Initialize the analysis clock
  clock_t t;
  t = clock();

  // Define the data file to be analyzed
  TString filename(runFile);

  // Define the analysis debug output
  ofstream *debugfile = new ofstream;;
  debugfile->open ("FADCcom_main_debug.txt");
  
  // Initialize the CODA decoder
  THaCodaFile datafile(filename);
  THaEvData *evdata = new CodaDecoder();

  // Initialize the evdata debug output
  evdata->SetDebug(1);
  evdata->SetDebugFile(debugfile);

  // Initialize root and output
  //TROOT fadcana("tstfadcroot", "Hall C analysis");
  hfile = new TFile("fadc_data.root", "RECREATE", "fadc module data");
 
  // Set the number of event to be analyzed
  uint32_t iievent;
  if (maxEvent == -1) iievent = 1;
  else iievent = maxEvent;   

  // Loop over events
  for(uint32_t ievent = 0; ievent < iievent; ievent++) {
    // Read in data file
    int status = datafile.codaRead();
    if (status == S_SUCCESS){ 
      UInt_t *data = datafile.getEvBuffer();
      evdata->LoadEvent(data);
      // Loop over slots
      for(uint32_t islot = SLOTMIN; islot < NUMSLOTS; islot++) {
	// if (evdata->GetNumRaw(CRATE11, islot) != 0) {  // HMS Single arm setup
	if (evdata->GetNumRaw(crateNumber, islot) != 0) {
	  Fadc250Module *fadc = NULL;
	  fadc = dynamic_cast <Fadc250Module*> (evdata->GetModule(crateNumber, islot));   // HMS single arm setup
	  if (fadc != NULL) {
	    //fadc->CheckDecoderStatus();
	    // Loop over channels
	    for (uint32_t chan = 0; chan < NADCCHAN; chan++) {
	      // Initilize variables
	      Int_t  fadc_mode, num_fadc_events, num_fadc_samples;
	      Bool_t raw_mode; 
	      fadc_mode = num_fadc_events = num_fadc_samples = raw_mode = 0;
	      // Acquire the FADC mode
	      fadc_mode = fadc->GetFadcMode(); fadc_mode_const = fadc_mode;
	      if (debugfile) *debugfile << "Channel " << chan << " is in FADC Mode " << fadc_mode << endl;
	      raw_mode  = ((fadc_mode == 1) || (fadc_mode == 8) || (fadc_mode == 10));
	      
	      // Acquire the number of FADC events
	      num_fadc_events = fadc->GetNumFadcEvents(chan);
	      // If in raw mode, acquire the number of FADC samples
	      if (raw_mode) {
		num_fadc_samples = 0;
		num_fadc_samples = fadc->GetNumFadcSamples(chan, ievent);
                win_size = num_fadc_samples;  //window size 
	      }
	      if (num_fadc_events > 0) {
                //check pulse and pedestal quality
                int overflowbit=0, underflowbit=0, pedqbit=0;
                
		// Generate FADC plots
		GeneratePlots(fadc_mode, islot, chan);
	      	for (Int_t jevent = 0; jevent < num_fadc_events; jevent++) {
                  nchanevents[islot][chan]++; //total fadc events for each chan
		  // Debug output
		  if ((fadc_mode == 1 || fadc_mode == 8) && num_fadc_samples > 0) 
		    if (debugfile) *debugfile << "FADC EMULATED PI DATA = " << fadc->GetEmulatedPulseIntegralData(chan) << endl;
		  if (fadc_mode == 7 || fadc_mode == 8 || fadc_mode == 9 || fadc_mode == 10) {
		    if (fadc_mode != 8) {if (debugfile) *debugfile << "FADC PI DATA = " << fadc->GetPulseIntegralData(chan, jevent) << endl;}
		    if (debugfile) *debugfile << "FADC PT DATA = " << fadc->GetPulseTimeData(chan, jevent) << endl;
		    if (debugfile) *debugfile << "FADC PPED DATA = " << fadc->GetPulsePedestalData(chan, jevent) / NPED << endl;
		    if (debugfile) *debugfile << "FADC PPEAK DATA = " << fadc->GetPulsePeakData(chan, jevent) << endl;
		  }
                  //pulse and pedestal quality check
                  if(fadc_mode == 10 || fadc_mode == 9){
                      // overflowbit = fadc->GetPulseOverflow(chan,jevent);
                      // underflowbit = fadc->GetPulseUnderflow(chan,jevent);
                      // pedqbit = fadc->GetPedestalQuality(chan,jevent);

                       overflowbit = fadc->GetOverflowBit(chan,jevent);
                       underflowbit = fadc->GetUnderflowBit(chan,jevent);
                       pedqbit = fadc->GetPedestalQuality(chan,jevent);

                       if(overflowbit==1)noverflow[islot][chan]++;
                       if(underflowbit==1)nunderflow[islot][chan]++;
                       if(pedqbit==1)nbadped[islot][chan]++;

                  }//pulse and pedestal quality check

		  // Fill histos
	      	  if ((fadc_mode == 1 || fadc_mode == 8) && num_fadc_samples > 0) h_pinteg[islot][chan]->Fill(fadc->GetEmulatedPulseIntegralData(chan));
		  else if (fadc_mode == 7 || fadc_mode == 8 || fadc_mode == 9 || fadc_mode == 10) {
		    if (fadc_mode != 8) {h_pinteg[islot][chan]->Fill(fadc->GetPulseIntegralData(chan, jevent));}
		    if (fadc_mode == 8 || fadc_mode == 1) {h_pinteg[islot][chan]->Fill(fadc->GetEmulatedPulseIntegralData(chan));}
		    h_ptime[islot][chan]->Fill(fadc->GetPulseTimeData(chan, jevent));
		    h_pped[islot][chan]->Fill(fadc->GetPulsePedestalData(chan, jevent) / NPED);
		    h_ppeak[islot][chan]->Fill(fadc->GetPulsePeakData(chan, jevent));
		    // 2D Histos
		    if (fadc_mode != 8) {h2_pinteg[islot]->Fill(chan, fadc->GetPulseIntegralData(chan, jevent));}
		    if (fadc_mode == 8 || fadc_mode == 1) {h2_pinteg[islot]->Fill(chan, fadc->GetEmulatedPulseIntegralData(chan));}
		    h2_ptime[islot]->Fill(chan, fadc->GetPulseTimeData(chan, jevent));
		    h2_pped[islot]->Fill(chan, fadc->GetPulsePedestalData(chan, jevent) / NPED);
		    h2_ppeak[islot]->Fill(chan, fadc->GetPulsePeakData(chan, jevent));
		  }
		  // Raw sample events
	      	  if (raw_mode && num_fadc_samples > 0) {
		    // Debug output
	      	    if (debugfile) *debugfile << "NUM FADC SAMPLES = " << num_fadc_samples << endl;
		    // Acquire the raw samples vector and populate graphs
		    raw_samples_vector[islot][chan] = fadc->GetPulseSamplesVector(chan);
		    if (raw_samp_index[islot][chan] < NUMRAWEVENTS) {
		      for (Int_t sample_num = 0; sample_num < Int_t (raw_samples_vector[islot][chan].size()); sample_num++)
			g_psamp_event[islot][chan][raw_samp_index[islot][chan]]->SetPoint(sample_num, sample_num + 1, Int_t (raw_samples_vector[islot][chan][sample_num]));
		      mg_psamp[islot][chan]->Add(g_psamp_event[islot][chan][raw_samp_index[islot][chan]], "ACP");
		      raw_samp_index[islot][chan] += 1;
		    }  // NUMRAWEVENTS condition
		  }  // Raw mode condition
	      	}  //  FADC event loop
	      }  // Number of FADC events condition
	    }  //FADC channel loop
	  }  // FADC module found condition
	  else 
	    if (debugfile) *debugfile << "FADC MODULE NOT FOUND!!!" << endl;
	}  // Number raw words condition
      }  // Slot loop
    }  // CODA file read status condition
    if (ievent % 1000 == 0)
      cout << ievent << " events have been processed" << endl;
    if (maxEvent == -1) iievent++;
    if (status == EOF) break;
  }  // Event loop 

  // Populate waveform graphs and multigraphs and write to root file
  TRandom3 *rand = new TRandom3();
  for(uint32_t islot = SLOTMIN; islot < NUMSLOTS; islot++) {
    for (uint32_t chan = 0; chan < NADCCHAN; chan++) {
       //calculate pulse and pedestal quality results
      if(nchanevents[islot][chan]!=0){
         poverflow[islot][chan] = (1.0*noverflow[islot][chan])/nchanevents[islot][chan];
         punderflow[islot][chan] = (1.0*nunderflow[islot][chan])/nchanevents[islot][chan];
         pbadped[islot][chan] = (1.0*nbadped[islot][chan])/nchanevents[islot][chan];       
       }
      for( uint32_t raw_index = 0; raw_index < NUMRAWEVENTS; raw_index++) {
      // Raw sample plots 
  	if (g_psamp_event[islot][chan][raw_index] != NULL) {
  	  UInt_t rand_int = 1 + rand->Integer(9);  
  	  hfile->cd(Form("/mode_%d_data/slot_%d/chan_%d/raw_samples", fadc_mode_const, islot, chan));
  	  c_psamp[islot][chan]->cd(raw_index + 1);
  	  g_psamp_event[islot][chan][raw_index]->Draw("ACP");
  	  g_psamp_event[islot][chan][raw_index]->SetTitle(Form("FADC Mode %d Pulse Peak Data Slot %d Channel %d Event %d", fadc_mode_const, islot, chan, raw_index));
  	  g_psamp_event[islot][chan][raw_index]->GetXaxis()->SetTitle("Sample Number");
  	  g_psamp_event[islot][chan][raw_index]->GetYaxis()->SetTitle("Sample Value");
  	  g_psamp_event[islot][chan][raw_index]->SetLineColor(rand_int);
  	  g_psamp_event[islot][chan][raw_index]->SetMarkerColor(rand_int);
  	  g_psamp_event[islot][chan][raw_index]->SetMarkerStyle(20);
  	  g_psamp_event[islot][chan][raw_index]->SetDrawOption("ACP");
  	}  // Graph condition
      }  // Raw event loop
      // Write the canvas to file
      if (c_psamp[islot][chan] != NULL) c_psamp[islot][chan]->Write();
      // Write the multigraphs to file
      if (mg_psamp[islot][chan] != NULL) {
        hfile->cd(Form("/mode_%d_data/slot_%d/chan_%d", fadc_mode_const, islot, chan));
        mg_psamp[islot][chan]->Draw("ACP");
        mg_psamp[islot][chan]->SetTitle(Form("%d Raw Events of FADC Mode %d Sample Data Slot %d Channel %d; Sample Number; Sample Value", raw_samp_index[islot][chan], fadc_mode_const, islot, chan));
        mg_psamp[islot][chan]->Write(); 
      }  // Mulitgraph condition
    }  // Channel loop
  }  // Slot loop

  //output pulse and pedestal quality check results
  int slotmin,slotmax;
  if(spectrometer=="LHRS") {slotmin=6;slotmax=10;}
  if(spectrometer=="RHRS") {slotmin=13;slotmax=17;}
  ofstream qualityfile;
  qualityfile.open(Form("results/fadc_quality_%d.txt",runNumber));
  for(int islot=slotmin;islot<slotmax;islot++)
  {  
       qualityfile<<" Slot "<<islot<<endl;
       qualityfile<<setiosflags(ios::left) << setw(12)<<"chan  "<<setw(7)<<"1"<<setw(7)<<"2"<<setw(7)<<"3"<<setw(7)<<"4"<<
                  setw(7)<<"5"<<setw(7)<<"6"<<setw(7)<<"7"<<setw(7)<<"8"<<setw(7)<<"9"<<setw(7)<<"10"<<setw(7)<<"11"<<
                  setw(7)<<"12"<<setw(7)<<"13"<<setw(7)<<"14"<<setw(7)<<"15"<<setw(7)<<"16"<<endl;
       qualityfile<<setiosflags(ios::left) << setw(12)<<"overflow  ";
       for(int chan=0;chan<NADCCHAN;chan++){
          qualityfile<<setiosflags(ios::left) <<setw(7)<<setiosflags(ios::fixed) << setprecision(4)<<poverflow[islot][chan];
        }
       qualityfile<<endl;
       qualityfile<<setiosflags(ios::left) << setw(12)<<"underflow  ";
       for(int chan=0;chan<NADCCHAN;chan++)
        {  
          qualityfile<<setiosflags(ios::left) <<setw(7)<<setiosflags(ios::fixed) << setprecision(4)<<punderflow[islot][chan];
        }
       qualityfile<<endl;
       qualityfile<<setiosflags(ios::left) << setw(12)<<"badped  ";
       for(int chan=0;chan<NADCCHAN;chan++){
          qualityfile<<setiosflags(ios::left) <<setw(7)<<setiosflags(ios::fixed) << setprecision(4)<<pbadped[islot][chan];
        }
       qualityfile<<endl;

  }
 
  cout << "***************************************" << endl;
  cout << iievent - 1 << " events were processed" << endl;
  cout << "***************************************" << endl;
  
  // Write and clode the data file
  hfile->Write();
  hfile->Close();
  TString copy = "cp fadc_data.root "+rootFile;
  system(copy);

  // Calculate the analysis rate
  t = clock() - t;
  printf ("The analysis took %.1f seconds \n", ((float) t) / CLOCKS_PER_SEC);
  printf ("The analysis event rate is %.1f Hz \n", (iievent - 1) / (((float) t) / CLOCKS_PER_SEC));

}  // main()
