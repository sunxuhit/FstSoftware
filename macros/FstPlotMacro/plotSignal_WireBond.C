#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile2D.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotSignal_WireBond(string mod = "Mod04", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetOptStat("RMe");
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.25); gStyle->SetStatH(0.20);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  const int numOfHV = 2;
  string hv[numOfHV] = {"HV70V","HV140V"};
  
  TH1F *FstScanClustersSignal[2][numOfHV][3];
  TH1F *FstScanClustersNoise[2][numOfHV][3];
  TH1F *FstScanClustersSNRatio[2][numOfHV][3];
  TH1F *FstScanClustersCMN[2][numOfHV][3];
  
  for(int i_config = 0; i_config < 2; ++i_config)
  {
    for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
    {
      string config;
      if(i_config == 0) config = "configuration";
      if(i_config == 1) config = "configuration_noenc_wb"; //change
      string inputfile = Form("../../output/%s/FstQAStudy_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",config.c_str(),mod.c_str(),hv[i_hv].c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

      TFile *File_InPut = TFile::Open(inputfile.c_str());

      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        string HistName;
        HistName = Form("h_mFstScanClustersSignal_Sensor%d",i_sensor);
        FstScanClustersSignal[i_config][i_hv][i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersNoise_Sensor%d",i_sensor);
        FstScanClustersNoise[i_config][i_hv][i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersSNRatio_Sensor%d",i_sensor);
        FstScanClustersSNRatio[i_config][i_hv][i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersCMN_Sensor%d",i_sensor);
        FstScanClustersCMN[i_config][i_hv][i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
      }  
    }
  }

  string outputname = Form("./figures/wirebond/Signal_%s_WireBondStudy_JustExtraWireBonds.pdf",mod.c_str());
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,300*numOfHV,2400);
  c_play->Divide(numOfHV,8);
  for(int i_pad = 0; i_pad < numOfHV*8; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetRightMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("%s[",outputname.c_str());
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    c_play->cd();
    string TitleName = Form("Sensor %d",i_sensor);
    TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
    canvas_title->SetBorderSize(1);
    canvas_title->Draw();
    for(int i_config = 0; i_config < 2; ++i_config)
    {
      string config;
      if(i_config == 0) config = "w/o Encapsulation";
      if(i_config == 1) config = "w/ Encapsulation";
      for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
      {
        c_play->cd(1+i_hv+numOfHV*i_config);
        string subtitle = Form("Signal Scan Clusters %s %s",hv[i_hv].c_str(),config.c_str());
        FstScanClustersSignal[i_config][i_hv][i_sensor]->SetTitle(subtitle.c_str());
        FstScanClustersSignal[i_config][i_hv][i_sensor]->GetXaxis()->SetTitle("ADC");
        FstScanClustersSignal[i_config][i_hv][i_sensor]->GetXaxis()->SetTitleSize(0.06);
        FstScanClustersSignal[i_config][i_hv][i_sensor]->SetLineColor(1);
        FstScanClustersSignal[i_config][i_hv][i_sensor]->Draw("HIST");
        
        c_play->cd(1+2*numOfHV+i_hv+numOfHV*i_config);
        subtitle = Form("Noise Scan Clusters %s %s",hv[i_hv].c_str(),config.c_str());
        FstScanClustersNoise[i_config][i_hv][i_sensor]->SetTitle(subtitle.c_str());
        FstScanClustersNoise[i_config][i_hv][i_sensor]->GetXaxis()->SetTitle("Noise ADC");
        FstScanClustersNoise[i_config][i_hv][i_sensor]->GetXaxis()->SetTitleSize(0.06);
        FstScanClustersNoise[i_config][i_hv][i_sensor]->SetLineColor(1);
        FstScanClustersNoise[i_config][i_hv][i_sensor]->Draw("HIST");

        c_play->cd(1+4*numOfHV+i_hv+numOfHV*i_config);
        subtitle = Form("S/N Scan Clusters %s %s",hv[i_hv].c_str(),config.c_str());
        FstScanClustersSNRatio[i_config][i_hv][i_sensor]->SetTitle(subtitle.c_str());
        FstScanClustersSNRatio[i_config][i_hv][i_sensor]->GetXaxis()->SetTitle("Signal/Noise");
        FstScanClustersSNRatio[i_config][i_hv][i_sensor]->GetXaxis()->SetTitleSize(0.06);
        FstScanClustersSNRatio[i_config][i_hv][i_sensor]->SetLineColor(1);
        FstScanClustersSNRatio[i_config][i_hv][i_sensor]->Draw("HIST");
        
         c_play->cd(1+6*numOfHV+i_hv+numOfHV*i_config);
        subtitle = Form("CMN Scan Clusters %s %s",hv[i_hv].c_str(),config.c_str());
        FstScanClustersCMN[i_config][i_hv][i_sensor]->SetTitle(subtitle.c_str());
        FstScanClustersCMN[i_config][i_hv][i_sensor]->GetXaxis()->SetTitle("CMN ADC");
        FstScanClustersCMN[i_config][i_hv][i_sensor]->GetXaxis()->SetTitleSize(0.06);
        FstScanClustersCMN[i_config][i_hv][i_sensor]->SetLineColor(1);
        FstScanClustersCMN[i_config][i_hv][i_sensor]->Draw("HIST");
      }
    }
    c_play->Update();
    c_play->Print(outputname.c_str());
  }

  string output_stop = Form("%s]",outputname.c_str());
  c_play->Print(output_stop.c_str()); // close pdf file
}
