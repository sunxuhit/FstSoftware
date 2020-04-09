#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile2D.h>
#include <TStyle.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotEventDisplay()
{
  gStyle->SetPalette(kRainBow);
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mEventDisplay[100];
  TH2F *h_mEventProjection[100];
  for(int i_event = 0; i_event < 100; ++i_event)
  {
    string HistName;
    HistName = Form("h_mEventDisplay_%d",i_event);
    h_mEventDisplay[i_event] = (TH2F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mEventProjection_%d",i_event);
    h_mEventProjection[i_event] = (TH2F*)File_InPut->Get(HistName.c_str());
    // cout << "i_event = " << i_event << ", numoftracks = " << h_mEventProjection[i_event]->GetEntries() << endl;
  }

  const double rMax = FST::rOuter + 4.0*FST::pitchR;
  const double rMin = FST::rOuter;
  const double phiMax = 64.0*FST::pitchPhi;
  const double phiMin = 0.0;

  string outputname = "./figures/EventDisplay_QA.pdf";
  TCanvas *c_EventDisplay = new TCanvas("c_EventDisplay","c_EventDisplay",10,10,900,900);
  c_EventDisplay->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_EventDisplay->cd(i_pad+1)->SetLeftMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetBottomMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetTicks(1,1);
    c_EventDisplay->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = "./figures/EventDisplay_QA.pdf[";
  c_EventDisplay->Print(output_start.c_str()); // open pdf file

  int numOfUsedEvent = 0;
  while(numOfUsedEvent < 99)
  {
    for(int i_pad = 0; i_pad < 9; ++i_pad)
    {
      c_EventDisplay->cd(i_pad+1);
      h_mEventDisplay[numOfUsedEvent]->SetStats(0);
      h_mEventDisplay[numOfUsedEvent]->GetXaxis()->SetTitle("R_strip");
      h_mEventDisplay[numOfUsedEvent]->GetXaxis()->SetTitleSize(0.06);
      h_mEventDisplay[numOfUsedEvent]->GetYaxis()->SetTitle("#phi");
      h_mEventDisplay[numOfUsedEvent]->GetYaxis()->SetTitleSize(0.06);
      h_mEventDisplay[numOfUsedEvent]->GetZaxis()->SetRangeUser(1.0,2000.0);
      h_mEventDisplay[numOfUsedEvent]->Draw("colz");
      // h_mEventDisplay[numOfUsedEvent]->Draw("TEXT Same");
      h_mEventProjection[numOfUsedEvent]->Draw("col Same");
      PlotLine(rMin, rMax, phiMin, phiMin, 1, 2, 2);
      PlotLine(rMin, rMax, phiMax, phiMax, 1, 2, 2);
      PlotLine(rMin, rMin, phiMin, phiMax, 1, 2, 2);
      PlotLine(rMax, rMax, phiMin, phiMax, 1, 2, 2);
      numOfUsedEvent++;
    }
    c_EventDisplay->Update();
    c_EventDisplay->Print(outputname.c_str()); // print integrated efficiency
  }

  string output_stop =  "./figures/EventDisplay_QA.pdf]";
  c_EventDisplay->Print(output_stop.c_str()); // close pdf file

}

