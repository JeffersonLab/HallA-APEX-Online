void PlotSlot(int slot)
{

  TDirectory * dir = (TDirectory *) gFile->Get("mode_10_data");
  dir->cd();
  dir = (TDirectory *) dir->Get(Form("slot_%d",slot));
  dir->cd();

  TCanvas * c  = new TCanvas;
  c->Divide(2,8);
  for (int i = 0 ; i< 16 ; i++)
    {
      c->cd(i+1);
      ((TDirectory *)gDirectory->Get(Form("chan_%d",i)))->Get("samples")->Draw("AX");
    }

}
