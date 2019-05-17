// the goal of this script is to histogram the number of count in an array of detector tdc
// eg: if you have 10 tdc histos for each pmt of your  cerenkov  (one histo per pmt) 
//make one 1d histo of the # of hit in each pmt versus the cerenkov mirror number
void compact_count(TString vHist, Int_t first, Int_t last,TString drawoption)
{
  TH1D *hnew;
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
	  name=vHist+Form("%d-%d_counts",first,last);
// 	  std::cout<<" nx, xmin,xmax, ny, ymin,ymax="
// 		   <<nx<<", "<<xmin<<", "<<xmax<<", "
// 		   <<ny<<", "<<ymin<<", "<<ymax<<"\n";
	    
	  hnew= new TH1D(name,name,nx,xmin,xmax);
	  hnew->GetXaxis()->SetTitle("Channel number");
	  hnew->GetYaxis()->SetTitle("Counts");

	}
      for(Int_t iy=1;iy<ny+1;iy++)
	{
	  Double_t validcount=0; // for example in the case of the 1875 tdc valid counts are larger than 0 and smaller than 4092
// 	  for(int jbin=1;jbin<hh->GetNbinsX();jbin++)
// 	    {
// 	      Double_t lmax=hh->GetBinLowEdge(jbin);
// 	      Double_t lmin=hh->GetBinLowEdge(jbin)+hh->GetBinWidth(jbin);
// 	      if(lmin>0&&lmax<4092)
// 		validcount+=hh->GetBinContent(jbin);
// 	    }
	  validcount=hh->GetEntries();
	  for(int jbin=1;jbin<hh->GetNbinsX();jbin++)
	    {
	      validcount-=hh->GetBinContent(jbin);
	      Double_t lmin=hh->GetBinLowEdge(jbin)+hh->GetBinWidth(jbin);
	      if(lmin>0)
		{
		  jbin=hh->GetNbinsX()+10;
		}
	    }
	      	     
	  hnew->SetBinContent(i-first+1, validcount);
	}
    }
	    
  hnew->SetMinimum(0);
  hnew->Draw(drawoption);

  return;
    }
