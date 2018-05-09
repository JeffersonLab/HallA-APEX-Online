#include <sstream> 
#include <string>
#include <cmath>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <TSystem.h> 
#include <TGraph.h>
#include <TString.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1F.h>
#include <TChain.h>
#include <TTree.h>
#include <TF1.h>
#include <TChain.h>
#include <TLine.h>
#include <TMath.h>
#include <TNtuple.h>
#include <stdlib.h>     //for using the function sleep

using namespace std;

 /////////////////////////////////////////////////////////
  //                                                     //
  // This section identifies the harp and run to analyze //
  //                                                     //
  /////////////////////////////////////////////////////////

/* In order update this code, you must change the date of the Harp scan and the time stamp of each position change, and the run numbers for the harp scans, including the acclerator run number . This is currently limited to running five runs for two harps.

prev used
{"15:22:28","15:29:59","15:55:20","16:35:33","17:11:23"}, //1H04A
{"15:25:14","15:32:24","15:34:22","15:58:10","16:37:48"} };  //1H04B


*/
void get_harp_pos(){

TCanvas *C1[2][5];

  int harp;
  int run;
  int ct = 0;//Counter


  //Day of harp runs in hall A
   char date[256] = {"05032018"};

//output array;
	double results[5][10];
//CODA run number for harp scans
	int run_number[5]  ={3033,3034,3035,3036,3038};
	int logbook_num[5] ={3461814,3461814,3461814,3461814,3461814};	

//output file named with date of harpscans
	ofstream out_file;
	char Hresults[256];
	sprintf(Hresults,"harp_results_%s.txt",date);
	out_file.open(Hresults,ios::out);

  //Harp names for the Hall A beamline (in hall, not arc)
  char harp_name[2][256] = {"IHA1H04A","IHA1H04B"};

  //Time of each harp run {in order: (2,-2),(-2,-2),(-2,2),(2,2),(0,0)}
	char TimeStamp[2][5][256] = {{"21:44:49","22:24:58","22:29:26","22:44:03","22:54:11"},{"21:48:03","22:17:50","22:31:29","22:37:58","22:57:05"}};			

  /////////////////////////////////////////////////////////
  //                                                     //
  //    Get data from run file and fit with gaussians    //
  //                                                     //
  /////////////////////////////////////////////////////////

///Input data
  	double tmp1[10000], tmp2[10000], tmp3[10000];


//The second number might need to be adjusted depending on the number of harp scans
	TF1 *fit_u[2][5];
	TF1 *fit_x[2][5];
	TF1 *fit_v[2][5];

	int r =0;
//Loop through both harps, and all scan locations
	for(harp=0; harp<2;harp++){
		for(run=0;run<5;run++){r++;//C->cd(r);

		//run=1;harp=1;
			ifstream myfile;
	  		char file_name[256];
			

  	//Open file name based on answers above
	  		sprintf(file_name,"HarpScans/%s.%s_%s",harp_name[harp],date,TimeStamp[harp][run]);
	  		myfile.open(file_name,ios::in);
	  		string line;
			cout << endl;	
			if(!myfile.good()){printf( "%s WIll not open \n",file_name);return;}
			cout<<endl;

///////////////////////////////////////////////////////////////////////////////////////////
//Read in the input file from the harp scan
  	//fill tmp1, tmp2, tmp3 with raw data (in order of: index, encoder value, signal)
			int q=0;
			ct=0;
  			while (!myfile.eof()) {
    			getline(myfile,line);
				//if(q<100){cout<<line<<endl;}
   				if (line[0]=='#'){ct += 0;}  //ie do nothing
    			else {myfile >> tmp1[ct] >> tmp2[ct] >> tmp3[ct];ct++;
    			//tmp2[ct]=tmp2[ct]*2.805 + 0.01667;
    				}
				q++; }
			myfile.close();
///////////////////////////////////////////////////////////////////////////////////////////

  	//create graph for plotting, fitting
  			TGraph *gr = new TGraph(ct-1,tmp2,tmp3);
  			gr->SetMarkerStyle(20);	
  	//find position of maximum signal for each wire
  	//limits based loosely on encoder range for wires, but may need to be adjusted in future
 	 		double max1=0, max2=0, max3=0;
  			double pos1=0, pos2=0, pos3=0;
	//Very basic ped. avg of first few signals				
			double ped=tmp3[0];int pc=1;
			for(int i =0;i<5;i++){if(tmp3[i+1] < ped*1.5/pc){ped+=tmp3[i+1];pc++;}}
			ped=ped/pc;
			//cout <<ped<<endl;

	//Limiting the postions of x,u,v
			double harplimits[2][6]={{0.0,35,35,60,54,85},{0.0,35,35,55,54,85}};

			

			//for(int m=0;m<10;m++){	cout << tmp1[m] << "  "<< tmp2[m] << "  " <<tmp3[m]<<endl;}
			
///////////////////////////////////////////////////////////////////////////////////////
			int location[4]={0};
	//For loop to find the max value of each wire!	
  			for (int i=0; i<ct; i++){
   				//u wire
    			if (tmp2[i]<harplimits[harp][1]){
      				if (tmp3[i]>max1){max1 = tmp3[i];pos1 = tmp2[i];location[1]=tmp1[i];}}

   				//v wire
     if (tmp2[i]>=harplimits[harp][2] && tmp2[i]<=harplimits[harp][3]&&tmp2[i] >=pos1){
    				if (tmp3[i]>max2){max2 = tmp3[i];pos2 = tmp2[i];location[2]=tmp1[i];}}

    			//x wire
     if (tmp2[i]>=harplimits[harp][4]&&tmp2[i] >pos2&&tmp2[i]<=80){
			       if (tmp3[i]>max3){max3 = tmp3[i];pos3 = tmp2[i];location[3]=tmp1[i];}}
			}
/////////////////////////////////////////////////////////////////////////////////////////////			
			
//			cout <<endl<<endl;
//			cout << pos1 <<"  " << pos2<< " " <<pos3 <<endl;
//			cout << location[1] <<"     " << location[2]<< "    " <<location[3] <<endl;

//Fill an array to seperate the wires.
			double X_wire[200],U_wire[200],V_wire[200],X_cur[200],U_cur[200],V_cur[200];
			int i=0;
			for(int H=location[1]-100;H<location[1]+100;H++){
					U_wire[i]=tmp2[H];U_cur[i]=tmp3[H];	i++;}i=0;
			for(int H=location[2]-100;H<location[2]+100;H++){
					V_wire[i]=tmp2[H];V_cur[i]=tmp3[H];i++;}i=0;
			for(int H=location[3]-100;H<location[3]+100;H++){
					X_wire[i]=tmp2[H];X_cur[i]=tmp3[H];i++;}
			char* AB;
			if(harp==0){AB="A";}else{AB="B";}
			C1[harp][run] = new TCanvas(Form("C%d%d",harp,run),Form("Run %d %s",run_number[run], AB),0,0,1000,900);
			C1[harp][run]->Divide(1,4);
						
  			TGraph *Uu = new TGraph(200,U_wire,U_cur);
			C1[harp][run]->cd(1);Uu->Draw();
fit_u[harp][run] = new TF1(Form("fit_u%d%d",harp,run),"[0]+gaus(1)",U_wire[0],U_wire[199]);
			fit_u[harp][run] ->SetParLimits(0,ped-0.1,ped+0.1);
			fit_u[harp][run] ->SetParLimits(1,ped-0.1,max1);
			fit_u[harp][run] ->SetParLimits(2,pos1-0.25,pos1+0.25);
			fit_u[harp][run] ->SetParLimits(3,0.1,3);
			fit_u[harp][run] ->SetParameters(ped,max1,pos1,2);
			Uu->Fit(Form("fit_u%d%d",harp,run),"Q","",pos1-1,pos1+1);
			//cout <<endl << abs(pos1-fit_u[harp][run]->GetParameter(2))<<endl;
			

  			TGraph *Vv = new TGraph(200,V_wire,V_cur);
			C1[harp][run]->cd(2);Vv->Draw();
fit_v[harp][run] = new TF1(Form("fit_v%d%d",harp,run),"[0]+gaus(1)",V_wire[0],V_wire[199]);
			fit_v[harp][run] ->SetParLimits(0,ped-0.1,ped+0.1);
			fit_v[harp][run] ->SetParLimits(1,ped-0.1,max2);
			fit_v[harp][run] ->SetParLimits(2,pos2-0.25,pos2+0.25);
			fit_v[harp][run] ->SetParLimits(3,0.1,3);
			fit_v[harp][run] ->SetParameters(ped,max2,pos2,2);
			Vv->Fit(Form("fit_v%d%d",harp,run),"Q","",pos2-1,pos2+1);
			//cout <<endl << abs(pos2-fit_v[harp][run]->GetParameter(2))<<endl;

  			TGraph *Xx = new TGraph(200,X_wire,X_cur);
			C1[harp][run]->cd(3);Xx->Draw();
fit_x[harp][run] = new TF1(Form("fit_x%d%d",harp,run),"[0]+gaus(1)",X_wire[0],X_wire[199]);
			fit_x[harp][run] ->SetParLimits(0,ped-0.1,ped+0.1);
			fit_x[harp][run] ->SetParLimits(1,0,max3);
			fit_x[harp][run] ->SetParLimits(2,pos3-0.15,pos3+0.15);
			fit_x[harp][run] ->SetParLimits(3,0.1,3);
			fit_x[harp][run] ->SetParameters(ped,max3,pos3-0.001,2);
			Xx->Fit(Form("fit_x%d%d",harp,run),"Q","",pos3-1,pos3+1);
			//cout <<endl << abs(pos3-fit_x[harp][run]->GetParameter(2))<<endl;			
	
		
  //draw each wire function in window
  			fit_u[harp][run]->SetLineColor(3);
  			fit_v[harp][run]->SetLineColor(4);
  			fit_x[harp][run]->SetLineColor(6);
  			fit_u[harp][run]->SetLineWidth(3);
  			fit_v[harp][run]->SetLineWidth(3);
  			fit_x[harp][run]->SetLineWidth(3);

			C1[harp][run]->cd(4);
			gr->Draw();
  			fit_u[harp][run]->Draw("same");
  			fit_v[harp][run]->Draw("same");
  			fit_x[harp][run]->Draw("same");
  			fit_u[harp][run]->Draw("same");

  /////////////////////////////////////////////////////////
  //                                                     //
  // Analyze fit results to extract x & y beam positions //
  //                                                     //
  /////////////////////////////////////////////////////////

  //get encoder position of each wire from fit
  			double u,v,x;
 			u = fit_u[harp][run]->GetParameter(2);
  			v = fit_v[harp][run]->GetParameter(2);
 			x = fit_x[harp][run]->GetParameter(2);

  //draw line to indicate wire position on plot
  			TLine *l1  = new TLine(u,0.02,u,0.1);  l1->SetLineColor(3);  l1->Draw();
  			TLine *l2  = new TLine(v,0.02,v,0.1);  l2->SetLineColor(4);  l2->Draw();
  			TLine *l3  = new TLine(x,0.02,x,0.1);  l3->SetLineColor(6);  l3->Draw();



TCanvas *D = new TCanvas(Form("D%d%d",harp,run),"Harp Scan",0,0,1100,900);
			gr->Draw();
  			fit_u[harp][run]->Draw("same");
  			fit_v[harp][run]->Draw("same");
  			fit_x[harp][run]->Draw("same");
  			fit_u[harp][run]->Draw("same");
			l3->Draw();
			l2->Draw();
			l1->Draw();
			gr->SetTitle(Form("Harp Scan run % d",run));
			gr->GetXaxis()->SetTitle("Harp encoder position");
			gr->GetYaxis()->SetTitle("Signal Strength");
			gr->GetYaxis()->SetTitleOffset(1.4);


  //Harp properties taken from CEBAF Element Database (CED) online
  //distance of each harp wire to beam center (in x-direction)
  			double harp_pos[3];  //mm
  			if (harp == 0){//1H04A
    			harp_pos[0] = 23.956;
   				harp_pos[1] = 49.608;
    			harp_pos[2] = 72.339;
  				}
  			else if (harp == 1){//1H04B
    			harp_pos[0] = 23.899;
   				harp_pos[1] = 49.520;
    			harp_pos[2] = 72.276;
  				}
  //Angle of the harp
			double harp_ang = 0;  //deg
  //Angle of the wires (u,v,x) relative to harp
		  	double wire_ang[3] = {45, 135, -90};  //deg
  //Conversion factor from encoder position to mm
			double enc_conv = 1; //0.0006104; 2.805 + 0.01667

  //calculate x_pos (in mm) of each wire when it hits the center of the beam
//   cout << u-harp_pos[0]<<" "<< v-harp_pos[1]<< " "<<x-harp_pos[2]<<endl;
  
  			double u_pos, v_pos, x_pos;
  			u_pos = (u*enc_conv-harp_pos[0]) * cos((harp_ang-wire_ang[0]+90)*TMath::Pi()/180);
  			v_pos = (v*enc_conv-harp_pos[1]) * cos((harp_ang-wire_ang[1]+90)*TMath::Pi()/180);
  			x_pos = (x*enc_conv-harp_pos[2]) * cos((harp_ang-wire_ang[2]+90)*TMath::Pi()/180);

			x_pos=-1*x_pos; //switch to halla cords.

  //get the width (sigma) in mm of beam on each wire
  			double u_sigma, v_sigma, x_sigma;
  			u_sigma = fabs(fit_u[harp][run]->GetParameter(3))*enc_conv;
  			v_sigma = fabs(fit_v[harp][run]->GetParameter(3))*enc_conv;
  			x_sigma = fabs(fit_x[harp][run]->GetParameter(3))*enc_conv;

  //use the u,v wires to calculate the y position of the beam
  //the x position comes directly from the x wire
  			double y_pos, y_sigma;
  			y_pos = 1.0/sqrt(2.0)*(v_pos-u_pos);
  			y_sigma = 1.0/sqrt(2.0)*(sqrt(u_sigma*u_sigma+v_sigma*v_sigma));


// Need to get the correct value for these coeifcents. 

		//if(harp==0){x_pos = x_pos*1.0072+1.98418;y_pos=y_pos*0.963489-1.83665;}
	//	if(harp==1){x_pos = x_pos*0.993559+1.47372;y_pos=y_pos*0.966529-0.707752;}
			//y_pos = y_pos+2.0;
  //print out the results
/*			cout <<"File " <<file_name << " of run number "<< run_number[run] <<endl;
			cout <<" is displayed on canvas C"<<harp<<run<<"  with positions!"<<endl; 
  			printf("Position X = %0.3f mm,  Sigma X = %0.3f mm \n", x_pos, x_sigma);
  			printf("Position Y = %0.3f mm,  Sigma Y = %0.3f mm \n", y_pos, y_sigma);	
	*/
	
					
			if(harp==0){results[run][0]=run_number[run];
						results[run][1]=logbook_num[run];
						results[run][2]=x_pos;
						results[run][3]=x_sigma;
						results[run][4]=y_pos;
						results[run][5]=y_sigma;}
			if(harp==1){results[run][6]=x_pos;
						results[run][7]=x_sigma;
						results[run][8]=y_pos;
						results[run][9]=y_sigma;}

//If you would like to few the fits comment the command below!
			delete C1[harp][run];
	
			} //run loop
		}			//harp loop	

	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){out_file<<setw(10)<< setprecision(10)<<results[i][j]<<" ";}
		for(int j=2;j<10;j++){
		out_file<<setw(10)<< setprecision(4)<<results[i][j]<<" ";}out_file<<endl;}
	
		for(int i=0;i<5;i++){
			cout<<run_number[i] <<" " << results[i][2]<<" " << results[i][4]<<" " << results[i][6] <<" " << results[i][8] <<endl;}
	
	out_file.close();
} 
