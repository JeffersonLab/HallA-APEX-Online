#define T_cxx
#include "T.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"

Int_t iIndex=0;
Int_t PreGolden();

void T::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L T.C
//      Root > T t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch


   if (fChain == 0) return;
   Int_t nNumblPre[150],nNumblShower[150],iEven=0;
   Int_t ampPre[150],ampShower[150];
   Int_t nentries = Int_t(fChain->GetEntriesFast());
   Float_t dpp2,pmom2,pmom0,pmom1;

   FILE* tempfile = fopen("electrSampR","w+");

   for(Int_t nTemp=0;nTemp<150;nTemp++)ampPre[nTemp] = 0;
   for(Int_t nTemp=0;nTemp<150;nTemp++)ampShower[nTemp] = 0;   

//   cout << "Please input the particle momenta (Mev)";
 //  cin >> pmom0;

   Int_t nbytes = 0, nb = 0,nRunNum=0;
   for (Int_t jentry=0; jentry<nentries;jentry++) {
      iIndex++;
      Int_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
     
   //    nRunNum = Run_Data->GetNumber();
      nRunNum = 3847;
      if(iIndex==1)fprintf(tempfile,"%d\n",nRunNum);
      if (Cut2(ientry) > 0) {
 
	//       pmom1 = 1000.*2.505*(1+R_gold_dp) ;	iEven++;
       pmom1 =1000.0*R_gold_p ;	iEven++;
	if(!(iEven%10000))
	  //	cout<<"the momentum value"<<pmom1<<endl;
	  cout<<"Event Procceding ..."<<iEven++<<endl;

	Int_t nMultShower=R_sh_mult,nMultPreShower=R_ps_mult;
        //R_sh/ps_mult is the total number of blocks in shower or preshower fired by the main cluster
     
       for(Int_t nPreShower=0;nPreShower<nMultPreShower;nPreShower++) {
	   Int_t nBlockPreShower = R_ps_nblk[nPreShower];//R_ps_nblk[j],j=0,R_ps_mult-1, is the numbers of block fired by the main cluster
	   nNumblPre[nPreShower] = nBlockPreShower;
           ampPre[nPreShower]    = R_ps_a_p[nBlockPreShower];           
	}
        for(Int_t nShower=0;nShower<nMultShower;nShower++) {
	   Int_t nBlockShower    = R_sh_nblk[nShower];
	   nNumblShower[nShower] = nBlockShower;
           ampShower[nShower]    = R_sh_a_p[nBlockShower];           	  
	}
        fprintf(tempfile,"%f\n",pmom1);
	fprintf(tempfile,"%d,",nMultPreShower);
	for(Int_t nTmp=0;nTmp<nMultPreShower-1;nTmp++)fprintf(tempfile,"%d,%d,",nNumblPre[nTmp],ampPre[nTmp]);
        fprintf(tempfile,"%d,%d\n",nNumblPre[nMultPreShower-1],ampPre[nMultPreShower-1]);        

	fprintf(tempfile,"%d,",nMultShower);
	for(Int_t nTmp=0;nTmp<nMultShower-1;nTmp++)fprintf(tempfile,"%d,%d,",nNumblShower[nTmp],ampShower[nTmp]); 
        fprintf(tempfile,"%d,%d\n",nNumblShower[nMultShower-1],ampShower[nMultShower-1]);        
      }
      if (Cut2(ientry) < 0) continue;
      
   }
   fclose(tempfile);
   printf("iEven = %d\n",iEven);
}

Int_t T::Cut2(Int_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.

//  One Cluster is reconstructed by Shower & PreShower
//   if(R_sh_nclust==1&&R_ps_nclust==1)
     
//   evtype == 1 or 2
//     if(g_evtyp == 1) 

     //One track is reconstructed by VDC
       if(R_vdc_u1_nclust==1&&R_vdc_v1_nclust==1&&R_vdc_u2_nclust==1&&R_vdc_v2_nclust==1)
  {
       //One track is reconstructed by S1 & S2
       //  if(R_s1_nthit==1&&R_s2_nthit==1&&R_s1_nlahit==1&&R_s1_nrahit==1&&R_s2_nlahit==1&&R_s2_nrahit==1){

         //Corrected sum amp in Cherenkov is less than 3000 but more than 2500
	 if(R_cer_asum_c>1000&&R_cer_asum_c<20000){                 

         //The momentum of the track cut
	   if(R_tr_n==1&&fabs(R_gold_th)<0.5&&fabs(R_gold_ph)<0.5&&fabs(R_gold_dp)<1.){

         //The Golden Track should be coincident to the main cluster in (Pre)Shower
	     // if(fabs(R_ps_x-R_ps_trx)<0.25&&fabs(R_ps_y-R_ps_try)<0.25){
	     // if(fabs(R_sh_y-R_sh_try)<0.25&&fabs(R_sh_x-R_sh_trx)<0.25){
		 // Cut the edge
		 Int_t nMultShower=R_sh_mult,nMultPreShower=R_ps_mult;
		 for(Int_t nShower=0;nShower<nMultShower;nShower++) {
		   Int_t nBlockShower    = R_sh_nblk[nShower];

		   // 	 	   if(nBlockShower > 61 ||nBlockShower <17) return -1;
		   // 		   if(nBlockShower == 30 || nBlockShower == 31 || nBlockShower == 32 || nBlockShower == 46|| nBlockShower == 47|| nBlockShower == 48 
		   //     ) return -1;
		 
//  	 	   if(nBlockShower > 62 ||nBlockShower <16) return -1;
// 		   if(nBlockShower == 31 || nBlockShower == 47 || nBlockShower == 65) return -1;
		   if(nBlockShower == 65) return -1;  //Fix the noisy channel, Z. Ye 06/08/2011
		 }
		 
		 return 1;
		 //  }
		 // }
	   }
         }
     }
     
     
     return -1;    
}
