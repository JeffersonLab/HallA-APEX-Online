//Check rate: xbj , Q2, and tg_y after cuts
//Shujie Li,Dec 2017
void rate_check_R(Int_t flag, TString drawoption=""){

  gStyle->SetOptStat(0);
  
  
  TTree *tree = (TTree*)gDirectory->Get("T");
 //Set the cut for data 
   const double dp_cut = 0.035;
   const double th_cut = 0.035;
   const double ph_cut = 0.02; 
  //const double y_cut = 10;//0.02;
  

  TCut track = "R.tr.n ==1";
  TCut trigger = "((DR.evtypebits>>5)&1)";
  TCut pid = Form("R.cer.asum_c>2000 && ((R.ps.e+R.sh.e)/(R.tr.p[0]*1000)>0.7)");
   TCut acc = Form("abs(R.tr.tg_dp)<%f && abs(R.tr.tg_th)<%f && abs(R.tr.tg_ph)<%f ", dp_cut, th_cut, ph_cut); 
  TCut data_cut =  track + acc+ trigger+pid ;
  TCut y_cut = "abs(R.tr.vz)<0.05"; 
  if(flag==1){
    
    TH1F *ht1 = new TH1F("ht1","xbj w/ acc and tgy cuts",1000,0,5);
    ht1->GetXaxis()->SetTitle("xbj");ht1->GetXaxis()->CenterTitle();
    ht1->GetYaxis()->SetTitle("good events counts");ht1->GetYaxis()->CenterTitle();
    
    
    tree->Draw("EKR.x_bj>>ht1",data_cut+y_cut,drawoption);
  }
 if(flag==2){
    
    TH1F *ht2 = new TH1F("ht2","Q2 w/ acc and tgy cuts",1000,0,2);
    ht2->GetXaxis()->SetTitle("Q2");ht2->GetXaxis()->CenterTitle();
    ht2->GetYaxis()->SetTitle("good events counts");ht2->GetYaxis()->CenterTitle();
    
    
    tree->Draw("EKR.Q2>>ht2",data_cut+y_cut,drawoption);
  }
 if(flag==3){
    
   TH1F *ht3 = new TH1F("ht3","ytar w/ acc cuts",1000,-0.1,0.1);
    ht3->GetXaxis()->SetTitle("y_target (m)");ht3->GetXaxis()->CenterTitle();
    ht3->GetYaxis()->SetTitle("good events counts");ht3->GetYaxis()->CenterTitle();
         TH1F *htt3 = new TH1F("htt3","ytarget after cuts",1000,-0.1,0.1);
       
    tree->Draw("R.tr.tg_y>>ht3","",drawoption);
    tree->Draw("R.tr.tg_y>>htt3",data_cut+y_cut+trigger,"same");
    htt3->SetLineColor(kRed);
    htt3->Draw("same");

   TLatex *t = new TLatex();
    t->SetTextColor(kRed);
    t->SetTextSize(0.04);
    t->SetTextAlign(12);
    int i1=ht3->Integral();
    int i2=htt3->Integral();
    gPad->SetLogy();
    t->DrawLatexNDC(0.2,0.85,Form("good events counts: %d/%d",i2,i1));
 
  }

  if(flag==4){
    
    TH2F *ht4 = new TH2F("ht4","Q2 v.s. xbj (w/ acc, tgy cuts)",1000,0,1,1000,0,5);
    ht4->GetXaxis()->SetTitle("x_bj");ht4->GetXaxis()->CenterTitle();
    ht4->GetYaxis()->SetTitle("Q2");ht4->GetYaxis()->CenterTitle();
    //ht3->SetMarkerStyle(3);ht3->SetMarkerSize(0.75);

    tree->Draw("EKR.Q2:EKR.x_bj>>ht4",data_cut+y_cut,drawoption);
    
  }
}
