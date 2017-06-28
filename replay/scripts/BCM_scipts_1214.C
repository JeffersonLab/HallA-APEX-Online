#include <TStyle.h>
#include<TObject.h>
void unser_new_co(int run)
{
 gStyle->SetOptFit(1111);
  //Open RootFile
 TFile * f = new TFile(Form("/adaqfs/home/a-onl/gmp/thir/replay/rootfiles_thir/left_gmp_%d.root",run));
 //TFile * f = new TFile(Form(,run));
  TTree * t = (TTree*)f->Get("T");
  //  unser_new_co.C
 
 
 // //Define Variable
 
//   Double_t        hac_bcm_average;

//    Double_t        hac_bcm_dvm1_read;
//    Double_t        hac_bcm_dvm2_read;
//    Double_t        hac_bcm_average;
//    Double_t        hac_unser_current;
//    Double_t        hac_bcm_dvm1_current;
//    Double_t        hac_bcm_dvm2_current;
//    Double_t        IPM1H04A_XPOS;
//    Double_t        IPM1H04A_YPOS;
//    Double_t        IPM1H04B_XPOS;
//    Double_t        IPM1H04B_YPOS;
//    Double_t        IBC0L02Current;
//    Double_t        IBC3H00CRCUR4;
//    Double_t        HA_K224_Current;
//    Double_t        hac_unser_read;
//    Double_t        left_clkcount;
//    Double_t        left_clkcountr;
//    Double_t        left_t1c;
//    Double_t        left_t2c;
//    Double_t        left_t3c;
//    Double_t        left_t4c;
//    Double_t        left_t5c;
//    Double_t        left_t6c;
//    Double_t        left_t7c;
//    Double_t        left_t8c;
//    Double_t        left_l1ac;
//    Double_t        left_t1r;
//    Double_t        left_t2r;
//    Double_t        left_t3r;
//    Double_t        left_t4r;
//    Double_t        left_t5r;
//    Double_t        left_t6r;
//    Double_t        left_t7r;
//    Double_t        left_t8r;
//    Double_t        left_l1ar;
//    Double_t        left_unser_c;
//    Double_t        left_bcm_u1c;
//    Double_t        left_bcm_d1c;
//    Double_t        left_bcm_d3c;
//    Double_t        left_bcm_d10c;
   Double_t        left_unser_r;
    Double_t        left_bcm_u1r;
    Double_t        left_bcm_d1r;
//    Double_t        left_bcm_d3r;
//    Double_t        left_bcm_d10r;
//    //   ThaEvent        Event_Branch;
//    ULong64_t         fEvtHdr_fEvtTime;
   UInt_t          fEvtHdr_fEvtNum;
//    Int_t           fEvtHdr_fEvtType;
//    Int_t           fEvtHdr_fEvtLen;
//    Int_t           fEvtHdr_fHelicity;
//    Int_t           fEvtHdr_fTargetPol;
//    Int_t           fEvtHdr_fRun;




  


// //Tree Branches
//    T->SetBranchAddress("hac_bcm_average",&hac_bcm_average);
//    T->SetBranchAddress("hac_bcm_dvm1_read",&hac_bcm_dvm1_read);
//    T->SetBranchAddress("hac_bcm_dvm2_read",&hac_bcm_dvm2_read);
//    T->SetBranchAddress("hac_bcm_average",&hac_bcm_average);
//    T->SetBranchAddress("hac_unser_current",&hac_unser_current);
//    T->SetBranchAddress("hac_bcm_dvm1_current",&hac_bcm_dvm1_current);
//    T->SetBranchAddress("hac_bcm_dvm2_current",&hac_bcm_dvm2_current);
//    T->SetBranchAddress("IBC0L02Current",&IBC0L02Current);
//    T->SetBranchAddress("IBC3H00CRCUR4",&IBC3H00CRCUR4);
//    T->SetBranchAddress("HA_K224_Current",&HA_K224_Current);
//    T->SetBranchAddress("hac_unser_read",&hac_unser_read);
//    T->SetBranchAddress("IPM1H04A.XPOS",&IPM1H04A_XPOS);
//    T->SetBranchAddress("IPM1H04A.YPOS",&IPM1H04A_YPOS);
//    T->SetBranchAddress("IPM1H04B.XPOS",&IPM1H04B_XPOS);
//    T->SetBranchAddress("IPM1H04B.YPOS",&IPM1H04B_YPOS);
//    T->SetBranchAddress("left_clkcount",&left_clkcount);
//    T->SetBranchAddress("left_clkcountr",&left_clkcountr);
//    T->SetBranchAddress("left_t1c",&left_t1c);
//    T->SetBranchAddress("left_t2c",&left_t2c);
//    T->SetBranchAddress("left_t3c",&left_t3c);
//    T->SetBranchAddress("left_t4c",&left_t4c);
//    T->SetBranchAddress("left_t5c",&left_t5c);
//    T->SetBranchAddress("left_t6c",&left_t6c);
//    T->SetBranchAddress("left_t7c",&left_t7c);
//    T->SetBranchAddress("left_t8c",&left_t8c);
//    T->SetBranchAddress("left_l1ac",&left_l1ac);
//    T->SetBranchAddress("left_t1r",&left_t1r);
//    T->SetBranchAddress("left_t2r",&left_t2r);
//    T->SetBranchAddress("left_t3r",&left_t3r);
//    T->SetBranchAddress("left_t4r",&left_t4r);
//    T->SetBranchAddress("left_t5r",&left_t5r);
//    T->SetBranchAddress("left_t6r",&left_t6r);
//    T->SetBranchAddress("left_t7r",&left_t7r);
//    T->SetBranchAddress("left_t8r",&left_t8r);
//    T->SetBranchAddress("left_l1ar",&left_l1ar);
//    T->SetBranchAddress("left_unser_c",&left_unser_c);
//    T->SetBranchAddress("left_bcm_u1c",&left_bcm_u1c);
//    T->SetBranchAddress("left_bcm_d1c",&left_bcm_d1c);
//    T->SetBranchAddress("left_bcm_d3c",&left_bcm_d3c);
//    T->SetBranchAddress("left_bcm_d10c",&left_bcm_d10c);
   T->SetBranchAddress("left_unser_r",&left_unser_r);
    T->SetBranchAddress("left_bcm_u1r",&left_bcm_u1r);
    T->SetBranchAddress("left_bcm_d1r",&left_bcm_d1r);
//    T->SetBranchAddress("left_bcm_d3r",&left_bcm_d3r);
//    T->SetBranchAddress("left_bcm_d10r",&left_bcm_d10r);
//    //T->SetBranchAddress("Event_Branch",&Event_Branch);
//    T->SetBranchAddress("fEvtHdr.fEvtTime",&fEvtHdr_fEvtTime);
 T->SetBranchAddress("fEvtHdr.fEvtNum",&fEvtHdr_fEvtNum);
//   T->SetBranchAddress("fEvtHdr.fEvtType",&fEvtHdr_fEvtType);
//    T->SetBranchAddress("fEvtHdr.fEvtLen",&fEvtHdr_fEvtLen);
//    T->SetBranchAddress("fEvtHdr.fHelicity",&fEvtHdr_fHelicity);
// //Tree Branches
//    T->SetBranchAddress("hac_bcm_average",&hac_bcm_average);
//    T->SetBranchAddress("hac_bcm_dvm1_read",&hac_bcm_dvm1_read);
//    T->SetBranchAddress("hac_bcm_dvm2_read",&hac_bcm_dvm2_read);
//    T->SetBranchAddress("hac_bcm_average",&hac_bcm_average);
//    T->SetBranchAddress("hac_unser_current",&hac_unser_current);
//    T->SetBranchAddress("hac_bcm_dvm1_current",&hac_bcm_dvm1_current);
//    T->SetBranchAddress("hac_bcm_dvm2_current",&hac_bcm_dvm2_current);
//    T->SetBranchAddress("IBC0L02Current",&IBC0L02Current);
//    T->SetBranchAddress("IBC3H00CRCUR4",&IBC3H00CRCUR4);
//    T->SetBranchAddress("HA_K224_Current",&HA_K224_Current);
//    T->SetBranchAddress("hac_unser_read",&hac_unser_read);
//    T->SetBranchAddress("IPM1H04A.XPOS",&IPM1H04A_XPOS);
//    T->SetBranchAddress("IPM1H04A.YPOS",&IPM1H04A_YPOS);
//    T->SetBranchAddress("IPM1H04B.XPOS",&IPM1H04B_XPOS);
//    T->SetBranchAddress("IPM1H04B.YPOS",&IPM1H04B_YPOS);
//    T->SetBranchAddress("left_clkcount",&left_clkcount);
//    T->SetBranchAddress("left_clkcountr",&left_clkcountr);
//    T->SetBranchAddress("left_t1c",&left_t1c);
//    T->SetBranchAddress("left_t2c",&left_t2c);
//    T->SetBranchAddress("left_t3c",&left_t3c);
//    T->SetBranchAddress("left_t4c",&left_t4c);
//    T->SetBranchAddress("left_t5c",&left_t5c);
//    T->SetBranchAddress("left_t6c",&left_t6c);
//    T->SetBranchAddress("left_t7c",&left_t7c);
//    T->SetBranchAddress("left_t8c",&left_t8c);
//    T->SetBranchAddress("left_l1ac",&left_l1ac);
//    T->SetBranchAddress("left_t1r",&left_t1r);
//    T->SetBranchAddress("left_t2r",&left_t2r);
//    T->SetBranchAddress("left_t3r",&left_t3r);
//    T->SetBranchAddress("left_t4r",&left_t4r);
//    T->SetBranchAddress("left_t5r",&left_t5r);
//    T->SetBranchAddress("left_t6r",&left_t6r);
//    T->SetBranchAddress("left_t7r",&left_t7r);
//    T->SetBranchAddress("left_t8r",&left_t8r);
//    T->SetBranchAddress("left_l1ar",&left_l1ar);//    T->SetBranchAddress("fEvtHdr.fTargetPol",&fEvtHdr_fTargetPol);
//    T->SetBranchAddress("fEvtHdr.fRun",&fEvtHdr_fRun);


// FILE *left_unser = fopen(Form("left_unser_r_%d.dat",run),"w");// open file and write into it("w")
// Int_t nev;
// nev=t->GetEntries();
// TH2F *h2 = new TH2F("h2","left_unser_r:scaler_event_number", 100,0,20000,100,0,45000);
 // for(Int_t i=0;i<nev;i++)
 //   {
    //   t->GetEntry(i);
 //  fprintf(left_unser," %10.5f     \n",left_unser_r);

//       fprintf(left_unser," %10.5f     %8.5f      %10.5f \n",left_unser_r,left_bcm_u1r,left_bcm_d1r);
//       h2->Fill(fEvtHdr_fEvtNum,left_unser_r);
 //  }

 //  fclose(left_unser);}
  
 // Double_t mean[20]={0.};
 // Double_t S_D[20]={0.};
 
//   Double_t ex[20]={0.};
// for (Int_t i=0; i<20; i++)   ex[i]= 2/sqrt(3*60); 



 cout<<"/////////////////////////////////////////////////////////"<< endl;
  cout<<"////////////////////////////////////////////////////////"<< endl;  
    Double_t mean1= 0;
    for(Int_t i=0;i<950;i++)
      {
        t->GetEntry(i);
        mean1= mean1+left_unser_r;
   
      }
    mean1=mean1/950.;
    //  cout<<"mean1=" <<mean1<<endl;


      Double_t  S_D1  = 0;
    for(Int_t i=0;i<950;i++)
      {
	t->GetEntry(i);
	S_D1 =S_D1+(left_unser_r-mean1)**2;  
      }
S_D1  =sqrt(S_D1/(950.));
  S_D1  =S_D1/sqrt(950.);
  //     cout<<"S_D1=" <<S_D1<<endl;

  cout<<"/////////////////////////////////////////////////////////"<< endl;
  cout<<"////////////////////////////////////////////////////////"<< endl; 



Double_t mean2 = 0;
  for(Int_t i=1000;i<1900;i++)
    {     
        t->GetEntry(i);
        mean2 = mean2+left_unser_r;
    }
  mean2 = mean2/900.;
  // cout<<"mean2=" <<mean2<<endl;


Double_t  S_D2 = 0;
for(Int_t i=1000;i<1900;i++)
    {
     t->GetEntry(i);
     S_D2 =S_D2+(left_unser_r-mean2)**2;  
    }
 S_D2 =sqrt(S_D2/(900.));
 S_D2 =S_D2/sqrt(900.);
 //cout<<"S_D2=" <<S_D2<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;


 Double_t mean3 = 0;
  for(Int_t i=1950;i<2900;i++)
    {     
        t->GetEntry(i);
        mean3 = mean3+left_unser_r;
    }
  mean3 = mean3/950.;
  // cout<<"mean3=" <<mean3<<endl;


Double_t  S_D3 = 0;
for(Int_t i=1950;i<2900;i++)
    {
     t->GetEntry(i);
     S_D3 =S_D3+(left_unser_r-mean3)**2;  
    }
 S_D3 =sqrt(S_D3/(950.));
 S_D3 =S_D3/sqrt(950.);
 //cout<<"S_D3=" <<S_D3<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;



 Double_t mean4 = 0;
  for(Int_t i=2950;i<3850;i++)
    {     
        t->GetEntry(i);
        mean4 = mean4+left_unser_r;
    }
  mean4 = mean4/890.;
  // cout<<"mean4=" <<mean4<<endl;


Double_t  S_D4 = 0;
for(Int_t i=2950;i<3850;i++)
    {
     t->GetEntry(i);
     S_D4 =S_D4+(left_unser_r-mean4)**2;  
    }
 S_D4 =sqrt(S_D4/(890.));
 S_D4 =S_D4/sqrt(890.);
 // cout<<"S_D4=" <<S_D4<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

   

 Double_t mean5 = 0;
  for(Int_t i=3850;i<4800;i++)
    {     
        t->GetEntry(i);
        mean5 = mean5+left_unser_r;
    }
  mean5 = mean5/950.;
  //cout<<"mean5=" <<mean5<<endl;


Double_t  S_D5 = 0;
for(Int_t i=3850;i<4800;i++)
    {
     t->GetEntry(i);
     S_D5 =S_D5+(left_unser_r-mean5)**2;  
    }
 S_D5 =sqrt(S_D5/(950.));
 S_D5 =S_D5/sqrt(950.);
 // cout<<"S_D5=" <<S_D5<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean6 = 0;
  for(Int_t i=4850;i<5650;i++)
    {     
        t->GetEntry(i);
        mean6 = mean6+left_unser_r;
    }
  mean6 = mean6/800.;
  // cout<<"mean6=" <<mean6<<endl;


Double_t  S_D6= 0;
for(Int_t i=4850;i<5650;i++)
    {
     t->GetEntry(i);
     S_D6 =S_D6+(left_unser_r-mean6)**2;  
    }
 S_D6 =sqrt(S_D6/(800.));
 S_D6 =S_D6/sqrt(800.);
 // cout<<"S_D6=" <<S_D6<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean7 = 0;
  for(Int_t i=5700;i<6600;i++)
    {     
        t->GetEntry(i);
        mean7 = mean7+left_unser_r;
    }
  mean7 = mean7/900.;
  //cout<<"mean7=" <<mean7<<endl;


Double_t  S_D7= 0;
for(Int_t i=5700;i<6600;i++)
    {
     t->GetEntry(i);
     S_D7 =S_D7+(left_unser_r-mean7)**2;  
    }
 S_D7 =sqrt(S_D7/(900.));
 S_D7 =S_D7/sqrt(900.);
 // cout<<"S_D7=" <<S_D7<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean8 = 0;
  for(Int_t i=6700;i<7550;i++)
    {     
        t->GetEntry(i);
        mean8 = mean8+left_unser_r;
    }
  mean8 = mean8/850.;
  //cout<<"mean8=" <<mean8<<endl;


Double_t  S_D8= 0;
for(Int_t i=6700;i<7550;i++)
    {
     t->GetEntry(i);
     S_D8 =S_D8+(left_unser_r-mean8)**2;  
    }
 S_D8 =sqrt(S_D8/(850.));
 S_D8 =S_D8/sqrt(850.);
 // cout<<"S_D8=" <<S_D8<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;



Double_t mean9 = 0;
  for(Int_t i=7600;i<8450;i++)
    {     
        t->GetEntry(i);
        mean9 = mean9+left_unser_r;
    }
  mean9 = mean9/850.;
  //cout<<"mean9=" <<mean9<<endl;


Double_t  S_D9= 0;
for(Int_t i=7600;i<8450;i++)
    {
     t->GetEntry(i);
     S_D9 =S_D9+(left_unser_r-mean9)**2;  
    }
 S_D9 =sqrt(S_D9/(850.));
 S_D9 =S_D9/sqrt(850.);
 //cout<<"S_D9=" <<S_D9<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;


Double_t mean10 = 0;
  for(Int_t i=8500;i<9450;i++)
    {     
        t->GetEntry(i);
        mean10 = mean10+left_unser_r;
    }
  mean10 = mean10/950.;
  //cout<<"mean10=" <<mean10<<endl;


Double_t  S_D10= 0;
for(Int_t i=8500;i<9450;i++)
    {
     t->GetEntry(i);
     S_D10 =S_D10+(left_unser_r-mean10)**2;  
    }
 S_D10 =sqrt(S_D10/(950.));
 S_D10 =S_D10/sqrt(950.);
 // cout<<"S_D10=" <<S_D10<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;


Double_t mean11 = 0;
  for(Int_t i=9500;i<10400;i++)
    {     
        t->GetEntry(i);
        mean11 = mean11+left_unser_r;
    }
  mean11 = mean11/900.;
  // cout<<"mean11=" <<mean11<<endl;


Double_t  S_D11= 0;
for(Int_t i=9500;i<10400;i++)
    {
     t->GetEntry(i);
     S_D11 =S_D11+(left_unser_r-mean11)**2;  
    }
 S_D11 =sqrt(S_D11/(900.));
 S_D11 =S_D11/sqrt(900.);
 // cout<<"S_D11=" <<S_D11<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean12 = 0;
  for(Int_t i=10450;i<11350;i++)
    {     
        t->GetEntry(i);
        mean12 = mean12+left_unser_r;
    }
  mean12 = mean12/900.;
  //cout<<"mean12=" <<mean12<<endl;


Double_t  S_D12= 0;
for(Int_t i=10450;i<11350;i++)
    {
     t->GetEntry(i);
     S_D12 =S_D12+(left_unser_r-mean12)**2;  
    }
 S_D12 =sqrt(S_D12/(901.));
 S_D12 =S_D12/sqrt(901.);
 // cout<<"S_D12=" <<S_D12<<endl;
  
cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean13 = 0;
  for(Int_t i=11400;i<12300;i++)
    {     
        t->GetEntry(i);
        mean13 = mean13+left_unser_r;
    }
  mean13 = mean13/900.;
  //cout<<"mean13=" <<mean13<<endl;


Double_t  S_D13= 0;
for(Int_t i=11400;i<12300;i++)
    {
     t->GetEntry(i);
     S_D13 =S_D13+(left_unser_r-mean13)**2;  
    }
 S_D13 =sqrt(S_D13/(900.));
 S_D13 =S_D13/sqrt(900.);
 // cout<<"S_D13=" <<S_D13<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;


Double_t mean14 = 0;
  for(Int_t i=12350;i<13250;i++)
    {     
        t->GetEntry(i);
        mean14 = mean14+left_unser_r;
    }
  mean14 = mean14/900.;
  // cout<<"mean14=" <<mean14<<endl;


Double_t  S_D14= 0;
for(Int_t i=12350;i<13250;i++)
    {
     t->GetEntry(i);
     S_D14 =S_D14+(left_unser_r-mean14)**2;  
    }
 S_D14 =sqrt(S_D14/(900.));
 S_D14 =S_D14/sqrt(900.);
 //  cout<<"S_D14=" <<S_D14<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;


Double_t mean15 = 0;
  for(Int_t i=13250;i<14200;i++)
    {     
        t->GetEntry(i);
        mean15 = mean15+left_unser_r;
    }
  mean15 = mean15/950.;
  //cout<<"mean15=" <<mean15<<endl;


Double_t  S_D15= 0;
for(Int_t i=13250;i<14200;i++)
    {
     t->GetEntry(i);
     S_D15 =S_D15+(left_unser_r-mean15)**2;  
    }
 S_D15 =sqrt(S_D15/(950.));
 S_D15 =S_D15/sqrt(950.);
 // cout<<"S_D15=" <<S_D15<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;


Double_t mean16 = 0;
  for(Int_t i=14250;i<15200;i++)
    {     
        t->GetEntry(i);
        mean16 = mean16+left_unser_r;
    }
  mean16 = mean16/950.;
  //cout<<"mean16=" <<mean16<<endl;


Double_t  S_D16= 0;
for(Int_t i=14250;i<15200;i++)
    {
     t->GetEntry(i);
     S_D16 =S_D16+(left_unser_r-mean16)**2;  
    }
 S_D16 =sqrt(S_D16/(950.));
 S_D16 =S_D16/sqrt(950.);
 // cout<<"S_D16=" <<S_D16<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean17 = 0;
  for(Int_t i=15200;i<16150;i++)
    {     
        t->GetEntry(i);
        mean17 = mean17+left_unser_r;
    }
  mean17 = mean17/950.;
  // cout<<"mean17=" <<mean17<<endl;


Double_t  S_D17= 0;
for(Int_t i=15200;i<16150;i++)
    {
     t->GetEntry(i);
     S_D17 =S_D17+(left_unser_r-mean17)**2;  
    }
 S_D17 =sqrt(S_D17/(950.));
 S_D17 =S_D17/sqrt(950.);
 // cout<<"S_D17=" <<S_D17<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

Double_t mean18 = 0;
  for(Int_t i=16200;i<17050;i++)
    {     
        t->GetEntry(i);
        mean18 = mean18+left_unser_r;
    }
  mean18 = mean18/850.;
  //cout<<"mean18=" <<mean18<<endl;


Double_t  S_D18= 0;
for(Int_t i=16200;i<17050;i++)
    {
     t->GetEntry(i);
     S_D18 =S_D18+(left_unser_r-mean18)**2;  
    }
 S_D18 =sqrt(S_D18/(850.));
 S_D18 =S_D18/sqrt(850.);
 // cout<<"S_D18=" <<S_D18<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;
Double_t mean19 = 0;
  for(Int_t i=17100;i<18050;i++)
    {     
        t->GetEntry(i);
        mean19 = mean19+left_unser_r;
    }
  mean19 = mean19/950.;
  //cout<<"mean19=" <<mean19<<endl;


Double_t  S_D19= 0;
for(Int_t i=17100;i<18050;i++)
    {
     t->GetEntry(i);
     S_D19 =S_D19+(left_unser_r-mean19)**2;  
    }
 S_D19 =sqrt(S_D19/(950.));
 S_D19 =S_D19/sqrt(950.);
 //cout<<"S_D19=" <<S_D19<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;
Double_t mean20 = 0;
  for(Int_t i=18100;i<19000;i++)
    {     
        t->GetEntry(i);
        mean20 = mean20+left_unser_r;
    }
  mean20 = mean20/900.;
  //cout<<"mean20=" <<mean20<<endl;


Double_t  S_D20= 0;
for(Int_t i=18100;i<19000;i++)
    {
     t->GetEntry(i);
     S_D20 =S_D20+(left_unser_r-mean20)**2;  
    }
 S_D20 =sqrt(S_D20/(900.));
 S_D20 =S_D20/sqrt(900.);
 //cout<<"S_D20=" <<S_D20<<endl;

cout<<"///////////////////////////////////////////////////////////////////"<< endl;
cout<<"///////////////////////////////////////////////////////////////////"<< endl;

//fit
 TCanvas *c1 = new TCanvas("c1");
 c1->Divide(2,2);
 c1->cd(1);
 Double_t current[]= {0,12.5,0,25,0,50,0,75,0,100,0,12.5,0,25,0,50,0,75,0,100};
 Double_t mean[] = {mean1,mean2,mean3,mean4,mean5,mean6,mean7,mean8,mean9,mean10,mean11,mean12,mean13,mean14,mean15,mean16,mean17,mean18,mean19,mean20};
 Double_t p[]= {S_D1,S_D2,S_D3,S_D4,S_D5,S_D6,S_D7,S_D8,S_D9,S_D10,S_D11,S_D12,S_D13,S_D14,S_D15,S_D16,S_D17,S_D18,S_D19,S_D20};
 Double_t ey[20];
 for (Int_t i=0; i<20; i++)   ey[i]= 2/sqrt(3*60);

 TGraphErrors* gr = new TGraphErrors(20,mean,current,p,ey);
 gr->Draw("AP");
 gr->Fit("pol1","ww");
 gr->SetTitle("Unser Calibration");
 gr->GetXaxis()->SetTitle("Frequency(Hz)");
 gr->GetYaxis()->SetTitle("Current(#muA)");


 //Residual 
 Double_t m[20];
 Double_t y[20];
 for(Int_t j=0;j<20;j++)
   {
     m[j]=0.00275*mean[j]-21.31;
     y[j] = current[j]-m[j];
	  
   }
 TGraphErrors* gr1 = new TGraphErrors(20,current,y,ey,ey);
 c1->cd(2);
 line = new TLine(-8,0,109,0);
 line->SetLineWidth(2);
 line->SetLineColor(3);
 gr1->SetTitle("Unser Calibration");
 gr1->GetYaxis()->SetTitle("Residual(#muA)");
 gr1->GetXaxis()->SetTitle("Current(#muA)");
 gr1->Draw("AP");
 line->Draw("same");

//Residual time 
 Double_t time[20]={3,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,30};
 Double_t ec[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 TGraphErrors* gr3 = new TGraphErrors(20,time,y,ec,ey);
 c1->cd(3);
 line3 = new TLine(0,0,60,0);
 line3->SetLineWidth(2);
 line3->SetLineColor(3);
 gr3->SetTitle("Unser Calibration");
 gr3->GetYaxis()->SetTitle("Residual(#muA)");
 gr3->GetXaxis()->SetTitle("Scalar event peroid(sec)");
 gr3->Draw("AP");
 line3->Draw("same");

 //%residual
 Double_t y1[20];
 Double_t ey1[20];
 Double_t et[20];

 for(Int_t k=0; k<20;k++)
   if( current[k]>0)  	
     {
    
       et[k]= 2/sqrt(3*60);
       y1[k]=y[k]/m[k]*100;
       ey1[k]=et[k]/current[k]*100;
     }
 TGraphErrors* gr2 = new TGraphErrors(20,current,y1,et,ey1);
 c1->cd(4);
 line1 = new TLine(-2.2,0,109,0);
 line1->SetLineWidth(2);
 line1->SetLineColor(3);
 gr2->SetTitle("Unser Calibration");
 gr2->GetXaxis()->SetTitle("Current(#muA)");
 gr2->GetYaxis()->SetTitle("Residual(%)");
 gr2->Draw("AP");
 line1->Draw("same") ;
 //  TH1F *h1 = new TH1F("h1","left_unser_r",10,0,45000);

 //h2->Fill(left_unser_r,fEvtHdr.fEvtNum);

 //  h1->Fill(left_unser_c);
 // TCanvas *c1 = new TCanvas("c1");
 //h2->Draw();    

}
 
