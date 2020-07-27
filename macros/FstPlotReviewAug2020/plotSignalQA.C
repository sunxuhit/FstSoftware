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
#include "../FstPlotConfiguration/draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotSignalQA(string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5, int sensorId = 2)
{
  gStyle->SetStatX(0.90); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.35); gStyle->SetStatH(0.55);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstQAStudy_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TProfile2D *p_mFstPedMap[FST::mFstNumSensorsPerModule];
  TProfile2D *p_mFstSigMap[FST::mFstNumSensorsPerModule];

  TH1F *h_mFstHitsMaxTb[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstHitsSignal[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstHitsNoise[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstHitsSNRatio[FST::mFstNumSensorsPerModule];

  //clusters
  TH1F *h_mFstSimpleClustersSignal[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstSimpleClustersNoise[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstSimpleClustersSNRatio[FST::mFstNumSensorsPerModule];

  TH1F *h_mFstScanClustersSignal[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersNoise[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersSNRatio[FST::mFstNumSensorsPerModule];

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    string HistName;
    HistName = Form("p_mFstPedMap_Sensor%d",i_sensor);
    p_mFstPedMap[i_sensor] = (TProfile2D*)File_InPut->Get(HistName.c_str());
    HistName = Form("p_mFstSigMap_Sensor%d",i_sensor);
    p_mFstSigMap[i_sensor] = (TProfile2D*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstHitsMaxTb_Sensor%d",i_sensor);
    h_mFstHitsMaxTb[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstHitsSignal_Sensor%d",i_sensor);
    h_mFstHitsSignal[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstHitsNoise_Sensor%d",i_sensor);
    h_mFstHitsNoise[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstHitsSNRatio_Sensor%d",i_sensor);
    h_mFstHitsSNRatio[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());

    HistName = Form("h_mFstSimpleClustersSignal_Sensor%d",i_sensor);
    h_mFstSimpleClustersSignal[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstSimpleClustersNoise_Sensor%d",i_sensor);
    h_mFstSimpleClustersNoise[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstSimpleClustersSNRatio_Sensor%d",i_sensor);
    h_mFstSimpleClustersSNRatio[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());

    HistName = Form("h_mFstScanClustersSignal_Sensor%d",i_sensor);
    h_mFstScanClustersSignal[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstScanClustersNoise_Sensor%d",i_sensor);
    h_mFstScanClustersNoise[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mFstScanClustersSNRatio_Sensor%d",i_sensor);
    h_mFstScanClustersSNRatio[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());
  }

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,1200,400);
  c_Signal->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_Signal->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Signal->cd(i_pad+1)->SetRightMargin(0.15);
    c_Signal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Signal->cd(i_pad+1)->SetTicks(1,1);
    c_Signal->cd(i_pad+1)->SetGrid(0,0);
  }

  /*
  c_Signal->cd(1);
  p_mFstPedMap[sensorId]->SetTitle("Pedestal");
  p_mFstPedMap[sensorId]->SetStats(0);
  p_mFstPedMap[sensorId]->GetXaxis()->SetTitle("R");
  p_mFstPedMap[sensorId]->GetYaxis()->SetTitle("#phi");
  p_mFstPedMap[sensorId]->GetXaxis()->SetTitleSize(0.06);
  p_mFstPedMap[sensorId]->GetYaxis()->SetTitleSize(0.06);
  p_mFstPedMap[sensorId]->Draw("colz");

  c_Signal->cd(2);
  p_mFstSigMap[sensorId]->SetTitle("Signal");
  p_mFstSigMap[sensorId]->SetStats(0);
  p_mFstSigMap[sensorId]->GetXaxis()->SetTitle("R");
  p_mFstSigMap[sensorId]->GetYaxis()->SetTitle("#phi");
  p_mFstSigMap[sensorId]->GetXaxis()->SetTitleSize(0.06);
  p_mFstSigMap[sensorId]->GetYaxis()->SetTitleSize(0.06);
  p_mFstSigMap[sensorId]->Draw("colz");
  */

  // c_Signal->cd(1);
  // h_mFstHitsSignal[sensorId]->SetTitle("Signal of Seeds");
  // h_mFstHitsSignal[sensorId]->GetXaxis()->SetNdivisions(505);
  // h_mFstHitsSignal[sensorId]->GetXaxis()->SetTitleSize(0.06);
  // h_mFstHitsSignal[sensorId]->GetXaxis()->SetTitle("ADC");
  // h_mFstHitsSignal[sensorId]->GetYaxis()->SetNdivisions(505);
  // h_mFstHitsSignal[sensorId]->GetYaxis()->SetTitleSize(0.06);
  // h_mFstHitsSignal[sensorId]->GetYaxis()->SetTitle("Counts");
  // h_mFstHitsSignal[sensorId]->SetLineColor(1);
  // h_mFstHitsSignal[sensorId]->Draw("HIST");


  c_Signal->cd(1);
  h_mFstHitsNoise[sensorId]->SetTitle("Single Channel Noise");
  h_mFstHitsNoise[sensorId]->GetXaxis()->SetNdivisions(505);
  h_mFstHitsNoise[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstHitsNoise[sensorId]->GetXaxis()->SetTitleSize(0.04);
  h_mFstHitsNoise[sensorId]->GetYaxis()->SetLabelSize(0.04);
  h_mFstHitsNoise[sensorId]->GetXaxis()->SetTitle("Single Channel Noise (ADC)");
  h_mFstHitsNoise[sensorId]->GetYaxis()->SetNdivisions(505);
  h_mFstHitsNoise[sensorId]->GetYaxis()->SetTitleSize(0.06);
  h_mFstHitsNoise[sensorId]->GetYaxis()->SetTitleOffset(1.1);
  h_mFstHitsNoise[sensorId]->GetYaxis()->SetLabelSize(0.04);
  h_mFstHitsNoise[sensorId]->GetYaxis()->SetTitle("Counts");
  h_mFstHitsNoise[sensorId]->SetLineColor(1);
  h_mFstHitsNoise[sensorId]->Draw("HIST");
  // h_mFstSimpleClustersNoise[sensorId]->SetTitle("Clusters Noise");
  // h_mFstSimpleClustersNoise[sensorId]->GetXaxis()->SetTitle("ADC");
  // h_mFstSimpleClustersNoise[sensorId]->GetXaxis()->SetTitleSize(0.06);
  // h_mFstSimpleClustersNoise[sensorId]->SetLineColor(1);
  // h_mFstSimpleClustersNoise[sensorId]->Draw("HIST");
  // h_mFstScanClustersNoise[sensorId]->SetLineColor(2);
  // h_mFstScanClustersNoise[sensorId]->Draw("HIST same");

  c_Signal->cd(2);
  h_mFstScanClustersSignal[sensorId]->SetTitle("Signal of Clusters");
  h_mFstScanClustersSignal[sensorId]->GetXaxis()->SetNdivisions(505);
  h_mFstScanClustersSignal[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstScanClustersSignal[sensorId]->GetXaxis()->SetLabelSize(0.04);
  h_mFstScanClustersSignal[sensorId]->GetXaxis()->SetTitle("Cluster Signal (ADC)");
  h_mFstScanClustersSignal[sensorId]->GetYaxis()->SetNdivisions(505);
  h_mFstScanClustersSignal[sensorId]->GetYaxis()->SetTitleSize(0.06);
  h_mFstScanClustersSignal[sensorId]->GetYaxis()->SetLabelSize(0.04);
  h_mFstScanClustersSignal[sensorId]->GetYaxis()->SetTitle("Counts");
  h_mFstScanClustersSignal[sensorId]->SetLineColor(1);
  h_mFstScanClustersSignal[sensorId]->Draw("HIST");
  // h_mFstSimplClustersSignal[sensorId]->SetLineColor(2);
  // h_mFstSimplClustersSignal[sensorId]->Draw("HIST same");

  c_Signal->cd(3);
  h_mFstScanClustersSNRatio[sensorId]->SetTitle("Signal/Noise of Clusters");
  h_mFstScanClustersSNRatio[sensorId]->GetXaxis()->SetNdivisions(505);
  h_mFstScanClustersSNRatio[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstScanClustersSNRatio[sensorId]->GetXaxis()->SetLabelSize(0.04);
  h_mFstScanClustersSNRatio[sensorId]->GetXaxis()->SetTitle("Signal/Noise");
  h_mFstScanClustersSNRatio[sensorId]->GetYaxis()->SetNdivisions(505);
  h_mFstScanClustersSNRatio[sensorId]->GetYaxis()->SetTitleSize(0.06);
  h_mFstScanClustersSNRatio[sensorId]->GetYaxis()->SetLabelSize(0.04);
  h_mFstScanClustersSNRatio[sensorId]->GetYaxis()->SetTitle("Counts");
  h_mFstScanClustersSNRatio[sensorId]->SetLineColor(1);
  h_mFstScanClustersSNRatio[sensorId]->Draw("HIST");
  // h_mFstSimpleClustersSNRatio[sensorId]->SetLineColor(2);
  // h_mFstSimpleClustersSNRatio[sensorId]->Draw("HIST same");

  string FigName = Form("./figures/c_SignalSensor%d.eps",sensorId);
  c_Signal->SaveAs(FigName.c_str());
}

