//======================================
// get run info from rootfile -shujie
// !! Run with analyzer NOT root!!
//======================================

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

  TString fname = Form("%stritium_%d.root",rootfilePath.Data(),irun);
  
  TFile *file = new TFile(fname,"read");

  Int_t j=0;
  // 
  TString temp = fname;
  while ( !gSystem->AccessPathName(temp.Data()) ) {
    cout << "Found ROOT file " << temp.Data()  << endl;   
    fname = temp;
    j++;
    temp = Form("%stritium_%d_%d.root",rootfilePath.Data(),irun,j);
  }
  
  file = new TFile(fname,"read");
  THaRun *aRun = (THaRun*)file->Get("Run_Data");
  THaRunParameters *para=aRun->GetParameters();
  para->Print();


  Double_t p0, angle, pos,ebeam,clk,dnew; 
  TString arm,targname="unknown";
  
  TTree *tree1=(TTree*)file->Get("T");
  TTree *tree2=(TTree*)file->Get("E");
  
  //tree2 = (TTree*)file->Get("E");
  tree2->SetBranchAddress("HALLA_p",&ebeam);
  tree2->SetBranchAddress("haBDSPOS",&pos);
  
  if(run<20000) {
    tree1->SetBranchAddress("evLeftLclock",&clk);
    tree1->SetBranchAddress("evLeftdnew",&dnew);
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
  
  cout<<"---------------\n";
  cout<<"Events      : " << last<<endl;
  cout<<"Time        : " << clk*1.0/103700/60<<" minutes"<<endl;
  cout<<"Charge      : " << dnew * 0.00033 << " C "<<endl;
  

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
 


  cout<<"---------------\n";
  cout<< "Target name              = "<<targname<<endl;
  cout<< "Target Encoder Position  = "<<pos<<endl;
  cout<< "Beam Energy              = "<<ebeam<<" GeV"<<endl;
  cout<< arm.Data()<<" p0                  = "<<p0<<" GeV"<<endl;
   cout<< arm.Data()<<" angle               = "<<angle<<" degree"<<endl;
  return;
}
