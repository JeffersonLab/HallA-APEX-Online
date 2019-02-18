void PlotCer()
{

  TDirectory * dir = (TDirectory *) gFile->Get("mode_10_data");
  dir->cd();
  dir = (TDirectory *) dir->Get("slot_15");
  dir->cd();

  TCanvas * c  = new TCanvas;
  c->Divide(2,5);
  for (int i = 0 ; i< 10 ; i++)
    {
      c->cd(i+1);
      ((TDirectory *)gDirectory->Get(Form("chan_%d",i+2)))->Get("samples")->Draw("AX");
    }

}
