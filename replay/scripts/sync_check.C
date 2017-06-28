#include <iostream>
#include "TGraph.h"

void sync_check(int );

using namespace std;

void sync_check(int flag)
{
  gStyle->SetOptStat(0);

  TTree *t = (TTree*)gDirectory->Get("T");
  if (flag == 1)
  {
    t->Draw("DL.evtyperoc3[0]:DL.evtyperoc4[0]","","");
    ((TGraph*)gPad->GetPrimitive("Graph"))->SetMarkerStyle(21);
    gPad->Update();
  }

  if (flag == 2)
  {
    t->Draw("DL.evtyperoc4[0]:DL.evtyperoc5[0]","","");
    ((TGraph*)gPad->GetPrimitive("Graph"))->SetMarkerStyle(21);
    gPad->Update();
  }
  if (flag == 3)
  {
    t->Draw("DL.evtyperoc5[0]:DL.evtypetsroc[0]","","");
    ((TGraph*)gPad->GetPrimitive("Graph"))->SetMarkerStyle(21);
    gPad->Update();
  }
/*
  if (flag == 4)
  {
    t->Draw("DL.evtyperoc3:DL.evtypetsroc","","*");
  }

  if (flag == 5)
  {
    t->Draw("DL.evtyperoc4:DL.evtypetsroc","","*");
  }
  if (flag == 6)
  {
    t->Draw("DL.evtyperoc5:DL.evtypetsroc","","*");
  }

  if (flag == 4)
  {
    t->Draw("DL.vxroc3:DL.vxroc4");
  }

  if (flag == 5)
  {
    t->Draw("DL.vxroc4:DL.vxroc5");
  }

  if (flag == 6)
  {
    t->Draw("DL.vxroc3:DL.vxroc5");
  }
*/

  if (flag == 4)
  {
    t->Draw("(DL.evtroc3[0]-DL.evtroc4[0]):Entry$");
  }

  if (flag == 5)
  {
    t->Draw("(DL.evtroc4[0]-DL.evtroc5[0]):Entry$");
  }

  if (flag == 6)
  {
    t->Draw("(DL.evtroc5[0]-DL.evttsroc[0]):Entry$");
  }
/*
  if (flag == 10)
  {
    t->Draw("DL.syncroc3:DL.syncroc4");
//    t->Draw("DL.syncroc3/DL.syncroc4:Entry$","","*");
  }

  if (flag == 11)
  {
    t->Draw("DL.syncroc4:DL.syncroc5");
//    t->Draw("DL.syncroc4/DL.syncroc5:Entry$","","*");
  }

  if (flag == 12)
  {
    t->Draw("DL.syncroc3:DL.syncroc5");
//    t->Draw("DL.syncroc3/DL.syncroc5:Entry$","","*");
  }
*/

}
