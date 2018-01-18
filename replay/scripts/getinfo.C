//======================================
// get run info from rootfile -shujie
// !! Run with analyzer NOT root!!
//======================================

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h> 

using namespace std;
struct target
{
  Double_t pos;
  TString name;
};



void getinfo(Int_t run=0){

  cout<<"Please run this code with ANALYZER instead of ROOT"<<endl;
  Int_t irun;
  const TString rootfilePath = "/chafs1/work1/tritium/Rootfiles/";
  // const TString rootfilePath = "/volatile/halla/triton/shujie/replay/Rootfiles/";

  if(run==0){
	  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  	  cin >> irun;
  	  }
  else{irun=run;}
  
  if(irun==-1) return;

	TString file_name = TString::Format("%stritium_%d.root",rootfilePath.Data(),irun);
	TString basename = TString::Format("%stritium_%d",rootfilePath.Data(),irun);
	TString rootfile = basename + ".root";
    TChain* T = new TChain("T");
    Long_t jk=0;
    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
        T->Add(rootfile.Data());
        cout << "ROOT file " << run<< "_"<< jk << " added to TChain." << endl; jk++;
        rootfile = basename + "_" + jk + ".root"; 
       // if(i>10){break;}
        }


   TTree *tree1;TTree *tree2;
   TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),irun),"read");
    tree1 = (TTree*)file->Get("T");
    tree2 = (TTree*)file->Get("E");
  if(file->IsZombie()){
    cout<<" this rootfile doest not exist: "<<endl;
    cout<<"Please try again with a new run. "<<endl;
    return;
  }
  
  
  THaRun *aRun = (THaRun*)file->Get("Run_Data");
  THaRunParameters *para=aRun->GetParameters();
  TArrayI ps = aRun->GetParameters()->GetPrescales();
  para->Print();


  Double_t p0, angle,pos, pos2,ebeam,clk,dnew,dnew_current; 
  TString arm,targname="unknown";
  
 // TTree *tree1=(TTree*)file->Get("T");
 // TTree *tree2=(TTree*)file->Get("E");
  
  //tree2 = (TTree*)file->Get("E");
  tree2->SetBranchAddress("HALLA_p",&ebeam);
  tree2->SetBranchAddress("haBDSPOS.VAL",&pos2);
  tree2->SetBranchAddress("haBDSPOS",&pos);
  
  
  
  
  
  if(run<20000) {
    //tree1->SetBranchAddress("evLeftLclock",&clk);
    T->SetBranchAddress("evLeftLclock",&clk);
    T->SetBranchAddress("evLeftdnew",&dnew);
    tree2->SetBranchAddress("HacL_alignAGL",&angle); 
    tree2->SetBranchAddress("HacL_D1_P0rb",&p0);

    arm="LHRS";

  }
  else {
    tree1->SetBranchAddress("evRightRclock",&clk);
    tree1->SetBranchAddress("evRightdnew",&dnew);
    tree2->SetBranchAddress("hacR_alignAGL",&angle);
    tree2->SetBranchAddress("HacR_D1_P0rb",&p0);
    arm="RHRS";
  }

  Double_t last=tree1->GetEntries();
  tree1->GetEntry(last-1);
  T->GetEntry(T->GetEntries()-1);
  
  cout<<"---------------\n";
  cout<<"Events          : " << last<<" " <<T->GetEntries()<<endl;
  cout<<"Time            : " << clk*1.0/103700/60<<" minutes"<<endl;
  cout<<"Charge          : " << dnew * 0.00033 << " C "<<endl;
  cout<<"Average Current : " <<(dnew * 0.00033)/(clk*1.0/103700) <<" uA"<<endl;
  

  Int_t mm=tree2->GetEntries();
  tree2->GetEntry(mm-1);
  

  target t2={33106235,"Tritium"};
  target d2={29367355,"Deuterium"};
  target h={25610043,"Hydrogen"};
  target he3={21875520,"Helium3"};
  target empty={18119995,"Empty Cell"};
  target dummy={15175217,"Dummy"};
  target optics={14394929,"Multifoils"};
  target hole={13108119,"Carbon Hole"};
  target raster={12444209,"Raster Target"};
  target al={11728945,"Thick Aluminum"};
  target single={11013681,"Single Carbon Foil"};
  target ti={10298417,"Titanium"};
  target beo={9583153,"BeO"};


  if(abs(pos)<= 0.000000001 ) {targname =" Unkown - Please double check logbook ";}
  else if(abs(pos)<50) targname="HOME";
  else if(abs(pos-t2.pos)<50)          targname=t2.name;
  else if(abs(pos-d2.pos)<50)     targname=d2.name;
  else if(abs(pos-h.pos)<50)      targname=h.name;
  else if(abs(pos-he3.pos)<50)    targname=he3.name;
  else if(abs(pos-empty.pos)<50)  targname=empty.name;
  else if(abs(pos-dummy.pos)<50)  targname=dummy.name;
  else if(abs(pos-optics.pos)<50) targname=optics.name;
  else if(abs(pos-hole.pos)<50)   targname=hole.name;
  else if(abs(pos-raster.pos)<50) targname=raster.name;
  else if(abs(pos-al.pos)<50)     targname=al.name;
  else if(abs(pos-single.pos)<50) targname=single.name;
  else if(abs(pos-ti.pos)<50)     targname=ti.name;
  else if(abs(pos-beo.pos)<50)    targname=beo.name;
 

  char *rate = new char[500];
  char *clkrate = new char[50];
  char hname[10][50];
  char *hh = new char[50];
  Double_t LT[10], DT[10];
  int icount[10];
  int daqcount[10];
  TH1F *his[10];
  double PS[8];
   
     for (int k=0; k<8; k++){
    PS[k]= ps[k];
    //cout<< " Prescaler = : "<< PS[k]<<endl;
  }
  //cout<<"Run # "<<"   "<<irun<<endl; 
  //cout<<"     |  Trigger |   Prescale |  Recorded Trigger |  Raw Trigger |   Deadtime (%)\n";
    for (int i=1; i<4; i++){
      TCut t_cut = Form("DL.evtypebits&(1<<%i)",i);
      sprintf(rate,"evLeftT%i", i);
      icount[i] = tree1->GetMaximum(rate);
      sprintf(hname[i],"t%i",i);
      sprintf(hh,"DL.evtypebits>>%s", hname[i]);
      his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
      tree1->Draw(hh,t_cut, "goff");
      daqcount[i] = his[i]->GetEntries();
      if(PS[i-1]>0){
	LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
	DT[i] = 100. - LT[i];
	//cout<<setw(12)<<i<<setw(12)<<PS[i-1]<<setw(18)<<daqcount[i]<<setw(18)<<icount[i]<<setw(18)<<DT[i]<<endl;
      }
    }


  cout<<"---------------\n";
  cout<< "Target name              = "<<targname<<endl;
  cout<< "Target Encoder Position  = "<<pos<<endl;
  cout<< "Beam Energy              = "<<ebeam<<" GeV"<<endl;
  cout<< arm.Data()<<" p0                  = "<<p0<<" GeV"<<endl;
   cout<< arm.Data()<<" angle               = "<<angle<<" degree"<<endl;
  cout<< "Dead time for Trigger 1 = "<<DT[1]<<endl;
  cout<< "Dead time for Trigger 2 = "<<DT[2]<<endl;
  cout<< "Counts for Trigger 2    = "<<daqcount[2]<<endl;
   
   int request;
   if(pos<=143940000){request=10;}
   	else{request=20;}
   	
     // Append a log file with one line that contains the reqired info for the wiki run log
   TString line ="<tr>";
   		line += TString::Format("<td> %d </td>",irun);//Run number[s]
   		line += TString::Format("<td>%s</td>",targname.Data());//Target
   		line += TString::Format("<td>%d</td>",request);//Current requested
  		line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033)/(clk*1.0/103700));//Average Current
   		line += TString::Format("<td>%0.2f</td>",dnew * 0.00033);//Charge
   		line += TString::Format("<td>PS1=%d PS2=%d PS3=%d PS8=%d </td>",ps[0],ps[1],ps[2],ps[7]);//Prescale
   		line += TString::Format("<td>%0.2f</td>",DT[2]);//Deadtime Trigger (2)
   		line += TString::Format("<td>%0.2f</td>",(clk*1.0/103700)/60);//Run time (minutes)  		   		   		   		
   		line += TString::Format("<td>%d</td>",daqcount[2]);//total counts Trigger 2 		
   		line += TString::Format("<td></td>");//Comments
		line += "</tr>";


	std::fstream list;
	std::string line_file;
  list.open ("./wiki_runlist.txt", std::fstream::in);
	std::string stringrun;std::string::size_type sz;
  	int inrun=0;
  	int status=0;
	while (std::getline(list, line_file))
		{

			for(int jj =9; jj<30;jj++){
				if(line_file[jj]== ' '){break;}
				stringrun+=line_file[jj];
			}	

		inrun=stoi(stringrun,&sz);
		size_t leng = stringrun.length();	
			//if(leng == 0){ status=2;}
			if(inrun==irun){status=1; }

			stringrun="";
		}
	list.close();
	
	FILE* list2;
	 list2 =fopen ("./wiki_runlist.txt","a");
	if(status==0){
		fprintf(list2,"%s \n",line.Data());	
		
		}
	else{

		cout <<"Run already added to the list for the wiki!! " <<endl; 
		}
	fclose(list2);

 //exit(1);

 return;

}
