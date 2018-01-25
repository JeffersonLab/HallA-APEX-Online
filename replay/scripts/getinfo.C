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
  const TString rootfilePath = "/volatile/halla/triton/Tritium_Rootfiles/";
  // const TString rootfilePath = "/volatile/halla/triton/shujie/replay/Rootfiles/";

  if(run==0){
	  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  	  cin >> irun;
  	  }
  else{irun=run;}
  
  if(irun==-1) return;

  TString fname = Form("%stritium_%d.root",rootfilePath.Data(),irun);
  
  TFile *file = new TFile(fname,"read");

    if (file->IsZombie()) {
       cout << "Error opening file" << endl;
       exit(-1);
    }

  Int_t j=0;
   
  TString temp = fname;
  while ( !gSystem->AccessPathName(temp.Data()) ) {
    cout << "Found ROOT file " << temp.Data()  << endl;   
    fname = temp;
    j++;
    temp = Form("%stritium_%d_%d.root",rootfilePath.Data(),irun,j);
    cout << "ROOT file " << Form("tritium_%d_%d.root",irun,j) << endl;
  }
  
  TTree *tree1=(TTree*)file->Get("T");
  TTree *tree2=(TTree*)file->Get("E");

  file = new TFile(fname,"read");
  THaRun *aRun = (THaRun*)file->Get("Run_Data");
  THaRunParameters *para=aRun->GetParameters();
  TArrayI ps = aRun->GetParameters()->GetPrescales();
  para->Print();

  Double_t p0, angle, pos,ebeam,clk,dnew,dnew_current, dnewr; 
  TString arm,targname="unknown";
   
  tree2 = (TTree*)file->Get("E");
  tree2->SetBranchAddress("HALLA_p",&ebeam);
  tree2->SetBranchAddress("haBDSPOS",&pos);
  tree1->Draw("evRightT5"); 
 
  if(irun<20000) {
    tree1->SetBranchAddress("evLeftLclock",&clk);
    tree1->SetBranchAddress("evLeftdnew",&dnew);
    tree1->SetBranchAddress("evLeftdnew_r",&dnewr);
    tree2->SetBranchAddress("HacL_alignAGL",&angle); 
    tree2->SetBranchAddress("HacL_D1_P0rb",&p0);
    arm="LHRS";

  }
  else {
    tree1->SetBranchAddress("evRightRclock",&clk);
    tree1->SetBranchAddress("evRightdnew",&dnew);
    tree2->SetBranchAddress("HacR_alignAGL",&angle);
    tree1->SetBranchAddress("evRightdnew_r",&dnewr);
    tree2->SetBranchAddress("HacR_D1_P0rb",&p0);
    arm="RHRS";
  }

  Double_t last=tree1->GetEntries();
  tree1->GetEntry(last-1);
  
  cout<<"---------------\n";
  cout<<"Events          : " << last<<endl;
  if (irun<20000){ cout<<"Time            : " << clk*1.0/103700/60<<" minutes"<<endl;}
  else{cout<<"Time            : " << clk*1.0/1024/60<<" minutes"<<endl;}
  cout<<"Charge          : " << dnew * 0.00033 << " C "<<endl;
  if (irun<20000){cout<<"Average Current : " <<(dnew * 0.00033)/(clk*1.0/103700) <<" uA"<<endl;}
  else{cout<<"Average Current : " <<(dnew * 0.00033)/(clk*1.0/1024) <<" uA"<<endl;}
  

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

  
  if(abs(pos)<50) targname="HOME";
  if(abs(pos-t2.pos)<50)          targname=t2.name;
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
  int trig;
  //Trigger dead time just for trigger 2 in the case of the LHRS and 5 in the case of the RHRS
  cout << tree1->GetMaximum("evRightT5") << endl;  
  if (irun < 20000){trig =2;}else{trig=5; }  
  TCut t_cut;
  if(irun<20000){t_cut = Form("DL.evtypebits&(1<<%i)",trig);}else{ t_cut = Form("DR.evtypebits&(1<<%i)",trig);}
  if(irun<20000){sprintf(rate,"evLeftT%i", trig);}else{sprintf(rate,"evRightT%i", trig);}
  icount[trig] = tree1->GetMaximum(rate);
  sprintf(hname[trig],"t%i",trig);
  cout<< rate << endl;
  if(irun<20000){ sprintf(hh,"DL.evtypebits>>%s", hname[trig]);}else{sprintf(hh,"DR.evtypebits>>%s", hname[trig]);}
  his[trig] =new TH1F (hname[trig], hname[trig], 100,0,1000000);
  tree1->Draw(hh,t_cut, "");
  daqcount[trig] = his[trig]->GetEntries();
  cout << daqcount[trig] <<endl;
  cout << icount[trig] << endl;
  if(ps[trig-1]>0){
     LT[trig] = 100.*ps[trig-1]*daqcount[trig]/icount[trig];
     DT[trig] = 100. - LT[trig];
   }
 



  cout<<"---------------\n";
  cout<< "Target name              = "<<targname<<endl;
  cout<< "Target Encoder Position  = "<<pos<<endl;
  cout<< "Beam Energy              = "<<ebeam<<" GeV"<<endl;
  cout<< arm.Data()<<" p0                  = "<<p0<<" GeV"<<endl;
   cout<< arm.Data()<<" angle               = "<<angle<<" degree"<<endl;
  cout<< Form("Dead time for Trigger %i = ", trig) << DT[trig]<<endl;
  
   int request;
   if(pos<=143940000){request=10;}
   	else{request=20;}
  
  Double_t curdnew=0;
  Int_t codnew=0;
  Double_t I_dnew=0;
  	
  for(Int_t i=0; i<last-1; i++){
    tree1->GetEntry(i);
    I_dnew=0.0003281*dnewr;
    if (I_dnew>(0.5)){curdnew+=I_dnew; codnew+=1; }
   }
  curdnew /=codnew;
  
  cout << "Average Current (uA): " << curdnew << endl; 
   // Append a log file with one line that contains the reqired info for the wiki run log
  TString line;

if(irun<20000){

   line ="<tr>";
   		line += TString::Format("<td> %d </td>",irun);//Run number[s]
   		line += TString::Format("<td>%s</td>",targname.Data());//Target
   		line += TString::Format("<td>%0.2f</td>",curdnew);//Current requested
  		line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033)/(clk*1.0/103700));//Average Current
   		line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033));//Charge
   		line += TString::Format("<td>PS1=%d PS2=%d PS3=%d PS8=%d </td>",ps[0],ps[1],ps[2],ps[7]);//Prescale
   		line += TString::Format("<td>%0.2f</td>",DT[2]);//Deadtime Trigger (2)
   		line += TString::Format("<td>%0.2f</td>",(clk*1.0/103700)/60);//Run time (minutes)  		   		   		   		
   		line += TString::Format("<td>%d</td>",daqcount[2]);//total counts Trigger 2 		
   		line += TString::Format("<td></td>");//Comments
		line += "</tr>";
   }else{
   line ="<tr>";
                line += TString::Format("<td> %d </td>",irun);//Run number[s]
                line += TString::Format("<td>%s</td>",targname.Data());//Target
                line += TString::Format("<td>%0.2f</td>",curdnew);//Current requested
                line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033)/(clk*1.0/1024));//Average Current
                line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033));//Charge
                line += TString::Format("<td>PS4=%d PS5=%d PS6=%d PS8=%d </td>",ps[3],ps[4],ps[5],ps[7]);//Prescale
                line += TString::Format("<td>%0.2f</td>",DT[5]);//Deadtime Trigger (2)
                line += TString::Format("<td>%0.2f</td>",(clk*1.0/1024)/60);//Run time (minutes)                                                              
                line += TString::Format("<td>%d</td>",daqcount[2]);//total counts Trigger 2             
                line += TString::Format("<td></td>");//Comments
                line += "</tr>";

    }

  std::fstream list;
  std::string line_file;
  list.open ("./wiki_runlist.txt", std::fstream::in);
  std::string stringrun;std::string::size_type sz;
  int inrun=0;
  int status=0;
  while (std::getline(list, line_file)){
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
    }else{
	cout <<"Run already added to the list for the wiki!! " <<endl; 
	}
   fclose(list2);

 exit(1);

 return;

}
