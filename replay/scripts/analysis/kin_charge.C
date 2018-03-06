#include "../TRI_Main.h" //Used for th RunNoChain to Tchain all of the TTrees together
#include "../TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together

/*This script will calculate the total amount of charge calulcated from the bcms.
This code is called by Kin_charge_ratio
The value returned will be the total charge recorded by the BCM dnew calibrated by the BCM class.
There is currently a 2.5 uA beam trip cut made. 

Limitations/ issues
The code is limitted to one arm at a time. :(
*/

Double_t kin_charge( TString filename="",Int_t cuts_bool=1){

	//input the name opf the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "/adaqfs/home/a-onl/tritium_work/Runlist/" + filename;
    ifstream file(filename.Data());
    if(!file.good()){cout << filename.Data() << "does not exist! "<<endl; exit(1);}
    TString content;
    TString Target,Kin,Run_String;
    const char* kin;
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin);
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
   ///////////////////////////////////////////////////
   ///Call tri_tools functions and combine root files
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
	TChain* T =(TChain*) gGetTree(RunNoChain, "T");
   //////////////////////////////////////////////////
	TString ARM,Arm,arm;

	if(RunNoChain[0]<90000){ARM="Left";
							Arm ="L";arm="l";}
						else{ARM="Right";
							 Arm="R";arm="r";}
/////////////////////////////////////////////////////////
	
	Double_t Total_Charge=1; //This will be the total amount of charge collected from the runs in Run_String;

//Total number of events to loop through.	
	Int_t Total_entries = T->GetEntries();
//Values used in charge calculations
	Double_t dnew_ch_ev=0, unew_ch_ev=0;
	Double_t dnew_ch_total=0, unew_ch_total=0;
	Double_t dnew_cur_ev=0, unew_cur_ev=0;
	Double_t isrenewed=0; //Did the scaler record:


//Needed pointer for values in the TChain
	T->ResetBranchAddresses();
	T->SetBranchAddress(Form("%sBCM.charge_dnew",ARM.Data()),&dnew_ch_ev);
	T->SetBranchAddress(Form("%sBCM.charge_unew",ARM.Data()),&unew_ch_ev);
	T->SetBranchAddress(Form("%sBCM.current_dnew",ARM.Data()),&dnew_cur_ev);
	T->SetBranchAddress(Form("%sBCM.current_unew",ARM.Data()),&dnew_cur_ev);
	T->SetBranchAddress(Form("%sBCM.isrenewed",ARM.Data()),   &isrenewed);
	    
	Double_t dnew_u_ch_ev=0, dnew_u_ch_ev_old=0,dnew_u_ch_total=0;
    T->SetBranchAddress(Form("ev%sdnew",ARM.Data()),&dnew_u_ch_ev);
	
		double gain, offset; 
		if(arm=="R"){ gain = 0.0003353; offset =0;}
			else {  gain = 0.0003358; offset =0;}        
    
	
//for loop to calculate the total charge;
	for(Int_t i=0;i<Total_entries;i++){
		T->GetEntry(i);
		//is T-event a new scaler event and if cuts are turned on makes a beam trip cut forcing at least 0.5 uAs
		if(isrenewed==1 && dnew_cur_ev >= cuts_bool*5.5){ 
			dnew_ch_total+=dnew_ch_ev;
			unew_ch_total+=unew_ch_ev;
			}//End of renewed if
			if(dnew_u_ch_ev>=dnew_u_ch_ev_old){
			dnew_u_ch_total+=(dnew_u_ch_ev-dnew_u_ch_ev_old);}

			dnew_u_ch_ev_old=dnew_u_ch_ev;
			dnew_ch_ev=0, unew_ch_ev=0; dnew_u_ch_ev=0;//reset;
	
		if(i/250000==i/250000.0){
			double per_done = (i/(Total_entries*1.0))*100;
			cout << per_done <<"\tpercent complete! "<<endl;
		}
	}
// End of for loop of events in the tchain.    
    
    //cout <<"Dnew :" << dnew_ch_total << "|  Unew  :"<< unew_ch_total <<endl;
    Total_Charge=dnew_ch_total;
    

//	T->SetBranchAddress(Form("%sdnew_r",ARM.Data()),&dnew_cur_ev);


  //  T->GetEntry(Total_entries-1);
    cout << "Charge without cuts from ev dnew : " << dnew_u_ch_total*gain<<" uC"<<endl;
    cout << "Charge with cuts from BCM class   : " << Total_Charge<<" uC" <<endl;  
   	
    
 
return Total_Charge;
}



