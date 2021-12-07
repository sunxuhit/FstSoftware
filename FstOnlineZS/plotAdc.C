#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TString.h>

using namespace std;

void plotAdc(int runId = 22332030)
{
  int eventId[2000];
  float adc[2000];
  int count = 0;

  char inputfile[256]; 
  // read in pedestal file on fst01
  sprintf(inputfile, "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstInstallation/daqtest/adc_nonzs_%d.txt",runId);
  cout << "Read in adc: " << inputfile << endl;
  std::ifstream file_Adc ( inputfile );
  if ( !file_Adc.is_open() )
  {
    std::cout << "Abort. Fail to read in adc: " << inputfile << std::endl;
    return 0;
  }

  int eventIdTemp = -1; 
  float adcTemp = -1.0;
  cout << "Start to read in " << inputfile << endl;;
  while (file_Adc >> eventIdTemp >> adcTemp)
  {
    if(count < 2000)
    {
      eventId[count] = eventIdTemp;
      adc[count] = adcTemp;
      cout << "eventId = " << eventIdTemp << ", adc = " << adcTemp << endl;
      count++;
    }
  }
  file_Adc.close();

  TGraph *g_adc = new TGraph();

  TH1F *h_frame = new TH1F("h_frame","h_frame",2000,0.,2000.);
  for(int i_bin = 0; i_bin < 2000; ++i_bin)
  {
    h_frame->SetBinContent(i_bin+1, -100.0);
    h_frame->SetBinError(i_bin+1, 1.0);
  }
  h_frame->SetStats(0);
  h_frame->SetTitle(std::to_string(runId).c_str());
  h_frame->GetXaxis()->SetTitle("event Id");;
  // h_frame->GetXaxis()->SetNdivisions(-12);
  h_frame->GetYaxis()->SetTitle("ADC");
  h_frame->GetYaxis()->SetRangeUser(0,2500);

  for(int i_event = 0; i_event < count; ++i_event)
  {
    g_adc->SetPoint(i_event,eventId[i_event],adc[i_event]);
  }

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,600);
  c_play->cd()->SetLeftMargin(0.1);
  c_play->cd()->SetBottomMargin(0.15);
  c_play->cd()->SetTicks(1,1);
  c_play->cd()->SetGrid(1,0);
  h_frame->Draw("pE");
  g_adc->SetMarkerStyle(24);
  g_adc->SetMarkerColor(kGray+2);
  g_adc->Draw("p same");

  char figName[256];
  sprintf(figName, "figures/adc_%d.eps",runId);
  c_play->SaveAs(figName);
}
