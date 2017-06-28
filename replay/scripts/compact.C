// the goal of this script is to histograms of the same kind into a compact form.
// eg: if you have 10 histo of the amplitude of the cerenkov adc signal 
//- one for each mirror, 
//make one 2d histo of the adc signal versus the cerenkov mirror number
void compact(TString vHist, Int_t first, Int_t last,TString drawoption)
{
  TH2D *hnew;
  TString name;

  for(Int_t i=first;i<last+1;i++)
    {
      Int_t nx,ny;
      Double_t xmin,xmax;
      Double_t ymin,ymax;
      name=vHist+Form("%d",i);
//       cout<<"name="<<name<<endl;
      TH1D *hh = (TH1D*)gDirectory->Get(name);
      if(i==first)
	{
	  nx=last-first+1;
	  xmin=first-0.5;
	  xmax=last+0.5;
	  ny=hh->GetNbinsX();
	  ymin=hh->GetBinLowEdge(1);
	  ymax=hh->GetBinLowEdge(ny)+hh->GetBinWidth(ny);
	  name=vHist+Form("%d-%d",first,last);
// 	  std::cout<<" nx, xmin,xmax, ny, ymin,ymax="
// 		   <<nx<<", "<<xmin<<", "<<xmax<<", "
// 		   <<ny<<", "<<ymin<<", "<<ymax<<"\n";
	    
	  hnew= new TH2D(name,name,nx,xmin,xmax,ny,ymin,ymax);
	  hnew->GetXaxis()->SetTitle("Channel number");
	  hnew->GetYaxis()->SetTitle(vHist);

	}
      for(Int_t iy=1;iy<ny+1;iy++)
	hnew->SetBinContent(i-first+1,iy,hh->GetBinContent(iy));
    }
	    
  hnew->Draw(drawoption);
}
