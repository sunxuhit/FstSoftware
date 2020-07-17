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
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotSignalQA(string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5, int sensorId = 1)
{
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.25); gStyle->SetStatH(0.55);

  TH1F *h_mMeanSignalHits_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mMeanNoiseHits_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mMeanSNRatioHits_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mMeanSignalClusters_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      string HistName;
      HistName = Form("h_mMeanSignalHits_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(sensorId > 0) HistName = Form("h_mMeanSignalHits_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mMeanSignalHits_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
      HistName = Form("h_mMeanNoiseHits_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(sensorId > 0) HistName = Form("h_mMeanNoiseHits_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mMeanNoiseHits_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
      HistName = Form("h_mMeanSNRatioHits_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(sensorId > 0) HistName = Form("h_mMeanSNRatioHits_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mMeanSNRatioHits_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
      HistName = Form("h_mMeanSignalClusters_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(sensorId > 0) HistName = Form("h_mMeanSignalClusters_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mMeanSignalClusters_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
    }
  }

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstQAStudy_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TProfile2D *p_mPedMap_FST = (TProfile2D*)File_InPut->Get("p_mPedMap_FST");;
  TProfile2D *p_mSigMap_FST = (TProfile2D*)File_InPut->Get("p_mSigMap_FST");

  TH1F *h_mFstHitsMaxTb[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstHitsSignal[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstHitsNoise[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstHitsSNRatio[FST::mFstNumSensorsPerModule];

  TH1F *h_mFstHitsMaxTb_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstHitsSignal_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstHitsNoise_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstHitsSNRatio_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];

  TH1F *h_mFstHitsSignal_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins]; // time bin differential
  TH1F *h_mFstHitsNoise_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];
  TH1F *h_mFstHitsSNRatio_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];

  //clusters
  TH1F *h_mFstSimpleClustersSignal[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstSimpleClustersSignal_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstSimpleClustersMaxTb_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstSimpleClustersSignal_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];

  TH1F *h_mFstScanClustersSignal[FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersSignal_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstScanClustersMaxTb_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mFstScanClustersSignal_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    string HistName;
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
    HistName = Form("h_mFstScanClustersSignal_Sensor%d",i_sensor);
    h_mFstScanClustersSignal[i_sensor] = (TH1F*)File_InPut->Get(HistName.c_str());

    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      HistName = Form("h_mFstHitsMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsMaxTb_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstSimpleClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstSimpleClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstSimpleClustersMaxTb_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstScanClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstScanClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstScanClustersMaxTb_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsSignal_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsNoise_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsSNRatio_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstSimpleClustersSignalSensor%d__Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstSimpleClustersSignalSensor%d__Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstSimpleClustersSignal_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mFstScanClustersSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstScanClustersSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstScanClustersSignal_Rstrip[i_sensor][i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
      for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
      {
	HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstHitsSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
	h_mMeanSignalHits_Rstrip[i_sensor][i_rstrip]->SetBinContent(i_tb+1,h_mFstHitsSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->GetMean()); // mean

	HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstHitsNoise_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
	h_mMeanNoiseHits_Rstrip[i_sensor][i_rstrip]->SetBinContent(i_tb+1,h_mFstHitsNoise_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->GetMean()); // mean

	HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstHitsSNRatio_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
	h_mMeanSNRatioHits_Rstrip[i_sensor][i_rstrip]->SetBinContent(i_tb+1,h_mFstHitsSNRatio_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->GetMean()); // mean

	HistName = Form("h_mFstSimpleClustersSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstSimpleClustersSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstSimpleClustersSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
	h_mMeanSignalClusters_Rstrip[i_sensor][i_rstrip]->SetBinContent(i_tb+1,h_mFstSimpleClustersSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->GetMean());
      }
    }
  }

  string outputname = Form("./figures/SignalQA_Sensor%d.pdf",sensorId);

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,800,1200);
  c_Signal->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_Signal->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Signal->cd(i_pad+1)->SetRightMargin(0.15);
    c_Signal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Signal->cd(i_pad+1)->SetTicks(1,1);
    c_Signal->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("./figures/SignalQA_Sensor%d.pdf[",sensorId);
  c_Signal->Print(output_start.c_str()); // open pdf file

  c_Signal->cd(1);
  p_mPedMap_FST->SetTitle("Pedestal");
  p_mPedMap_FST->SetStats(0);
  p_mPedMap_FST->GetXaxis()->SetTitle("R");
  p_mPedMap_FST->GetYaxis()->SetTitle("#phi");
  p_mPedMap_FST->GetXaxis()->SetTitleSize(0.06);
  p_mPedMap_FST->GetYaxis()->SetTitleSize(0.06);
  p_mPedMap_FST->Draw("colz");

  c_Signal->cd(2);
  p_mSigMap_FST->SetTitle("Signal");
  p_mSigMap_FST->SetStats(0);
  p_mSigMap_FST->GetXaxis()->SetTitle("R");
  p_mSigMap_FST->GetYaxis()->SetTitle("#phi");
  p_mSigMap_FST->GetXaxis()->SetTitleSize(0.06);
  p_mSigMap_FST->GetYaxis()->SetTitleSize(0.06);
  p_mSigMap_FST->Draw("colz");

  c_Signal->cd(3);
  h_mFstHitsSignal[sensorId]->SetTitle("Signal of Hits");
  h_mFstHitsSignal[sensorId]->GetXaxis()->SetTitle("ADC");
  h_mFstHitsSignal[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstHitsSignal[sensorId]->Draw("HIST");

  c_Signal->cd(4);
  h_mFstSimpleClustersSignal[sensorId]->SetTitle("Signal of Clusters");
  h_mFstSimpleClustersSignal[sensorId]->GetXaxis()->SetTitle("ADC");
  h_mFstSimpleClustersSignal[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstSimpleClustersSignal[sensorId]->SetLineColor(1);
  h_mFstSimpleClustersSignal[sensorId]->Draw("HIST");
  h_mFstScanClustersSignal[sensorId]->SetLineColor(2);
  h_mFstScanClustersSignal[sensorId]->Draw("HIST same");

  TLegend *leg = new TLegend(0.4,0.65,0.65,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(h_mFstSimpleClustersSignal[sensorId],"Simple Cluster","L");
  leg->AddEntry(h_mFstScanClustersSignal[sensorId],"Scan Cluster","L");
  leg->Draw("same");

  c_Signal->cd(5);
  h_mFstHitsNoise[sensorId]->SetTitle("Noise");
  h_mFstHitsNoise[sensorId]->GetXaxis()->SetTitle("ADC");
  h_mFstHitsNoise[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstHitsNoise[sensorId]->Draw("HIST");

  c_Signal->cd(6);
  h_mFstHitsSNRatio[sensorId]->SetTitle("Signal/Noise of Hits");
  h_mFstHitsSNRatio[sensorId]->GetXaxis()->SetTitle("Signal/Noise");
  h_mFstHitsSNRatio[sensorId]->GetXaxis()->SetTitleSize(0.06);
  h_mFstHitsSNRatio[sensorId]->Draw("HIST");

  c_Signal->Update();
  c_Signal->Print(outputname.c_str());

  // ADC & Time Bin
  c_Signal->cd(1);
  {
    TF1 *f_landau_FST[FST::mFstNumRstripPerSensor];
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->SetTitle("FST Hits");
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(0.0,0.6*h_mFstHitsSignal[sensorId]->GetMaximum());
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      string FuncName = Form("f_landau_FST_%d",i_rstrip);
      f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
      f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
      h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
      f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
      f_landau_FST[i_rstrip]->SetLineStyle(2);
      f_landau_FST[i_rstrip]->SetLineWidth(2);

      if(i_rstrip == 0) h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->Draw("same");
      // f_landau_FST[i_rstrip]->Draw("l same");

      // string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mFstHitsSignal_Rstrip[sensorId][i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mFstHitsSignal_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(2);
  {
    TF1 *f_landau_FST[FST::mFstNumRstripPerSensor];
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->SetTitle("FST Clusters");
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(0.0,0.6*h_mFstSimpleClustersSignal[sensorId]->GetMaximum());
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      string FuncName = Form("f_landau_FST_%d",i_rstrip);
      f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
      f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
      h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
      f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
      f_landau_FST[i_rstrip]->SetLineStyle(2);
      f_landau_FST[i_rstrip]->SetLineWidth(2);

      if(i_rstrip == 0) h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->Draw("same");
      // f_landau_FST[i_rstrip]->Draw("l same");

      // string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mFstSimpleClustersSignal_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(3);
  {
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->SetTitle("FST Noise");
      h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(0.0,0.6*h_mFstHitsNoise[sensorId]->GetMaximum());
      h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mFstHitsNoise_Rstrip[sensorId][i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mFstHitsNoise_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(4);
  {
    TLegend *leg_FST = new TLegend(0.50,0.6,0.90,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->SetTitle("FST Signal/Noise");
      h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Signal/Noise");
      h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(0.0,0.6*h_mFstHitsSNRatio[sensorId]->GetMaximum());
      h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mFstHitsSNRatio_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(5);
  {
    TLegend *leg_FST = new TLegend(0.55,0.6,0.8,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->SetTitle("Max Time Bin Hits");
      h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Maxt Time Bin");
      h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mFstHitsMaxTb_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(6);
  {
    TLegend *leg_FST = new TLegend(0.55,0.6,0.8,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->SetTitle("Max Time Bin Clusters");
      h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Maxt Time Bin");
      h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mFstSimpleClustersMaxTb_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->Update();
  c_Signal->Print(outputname.c_str());

  //--------------------------------------------------
  TCanvas *c_MeanSignal = new TCanvas("c_MeanSignal","c_MeanSignal",10,10,800,800);
  c_MeanSignal->Divide(1,4);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_MeanSignal->cd(i_pad+1)->SetLeftMargin(0.1);
    c_MeanSignal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_MeanSignal->cd(i_pad+1)->SetTicks(1,1);
    c_MeanSignal->cd(i_pad+1)->SetGrid(0,0);
  }

  c_MeanSignal->cd(1);
  {
    TLegend *leg_FST = new TLegend(0.7,0.6,0.9,0.9);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->SetTitle("");
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitle("<ADC>_{Hits}");
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleSize(0.10);
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(0.5*h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetMinimum(),1.5*h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetMaximum());
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mMeanSignalHits_Rstrip[sensorId][i_rstrip]->Draw("same");

      string LegName = Form("Sensor %d & R_strip %d",sensorId,i_rstrip);
      leg_FST->AddEntry(h_mMeanSignalHits_Rstrip[sensorId][i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_MeanSignal->cd(2);
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->SetTitle("");
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitle("<ADC>_{Clusters}");
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleSize(0.08);
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(0.5*h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetMinimum(),1.5*h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetMaximum());
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mMeanSignalClusters_Rstrip[sensorId][i_rstrip]->Draw("same");
    }
  }

  c_MeanSignal->cd(3);
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->SetTitle("");
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitle("Noise <ADC>_{Hits}");
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleSize(0.08);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(5.0,25.0);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mMeanNoiseHits_Rstrip[sensorId][i_rstrip]->Draw("same");
    }
  }

  c_MeanSignal->cd(4);
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->SetTitle("");
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->SetStats(0);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitle("<Signal/Noise>_{Hits}");
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleSize(0.08);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetRangeUser(5.0,35.0);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->Draw();
      else h_mMeanSNRatioHits_Rstrip[sensorId][i_rstrip]->Draw("same");
    }
  }

  c_MeanSignal->Update();
  c_MeanSignal->Print(outputname.c_str());
  //--------------------------------------------------

  //--------------------------------------------------
  TCanvas *c_Signal_TimeBin = new TCanvas("c_Signal_TimeBin","c_Signal_TimeBin",10,10,1800,800);
  c_Signal_TimeBin->Divide(9,4);
  for(int i_pad = 0; i_pad < 36; ++i_pad)
  {
    c_Signal_TimeBin->cd(i_pad+1)->SetLeftMargin(0.1);
    c_Signal_TimeBin->cd(i_pad+1)->SetBottomMargin(0.1);
    c_Signal_TimeBin->cd(i_pad+1)->SetTicks(1,1);
    c_Signal_TimeBin->cd(i_pad+1)->SetGrid(0,0);
  }

  // Signal
  for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mFstHitsSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mFstHitsSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mFstHitsSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mFstHitsSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->Draw();
      if(i_rstrip == 0 && i_tb == 0) plotTopLegend((char*)"Hits",0.3,0.6,0.15,1,0.0,42,1,1);
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Signal_TimeBin->Update();
      c_Signal_TimeBin->Print(outputname.c_str());
    }
  }

  // Noise
  for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mFstHitsNoise_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mFstHitsNoise_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mFstHitsNoise_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetXaxis()->SetRangeUser(0.0,40.0);
      h_mFstHitsNoise_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mFstHitsNoise_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->Draw();
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Signal_TimeBin->Update();
      c_Signal_TimeBin->Print(outputname.c_str());
    }
  }

  // Signal/Noise
  for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mFstHitsSNRatio_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mFstHitsSNRatio_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mFstHitsSNRatio_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mFstHitsSNRatio_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->Draw();
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Signal_TimeBin->Update();
      c_Signal_TimeBin->Print(outputname.c_str());
    }
  }

  // Signal of Clusters
  for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mFstSimpleClustersSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mFstSimpleClustersSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mFstSimpleClustersSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mFstSimpleClustersSignal_Rstrip_TimeBin[sensorId][i_rstrip][i_tb]->Draw();
      if(i_rstrip == 0 && i_tb == 0) plotTopLegend((char*)"Clusters",0.3,0.6,0.15,1,0.0,42,1,1);
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Signal_TimeBin->Update();
      c_Signal_TimeBin->Print(outputname.c_str());
    }
  }
  //--------------------------------------------------

  // c_Signal->SaveAs("./figures/SignalQA.pdf");
  string output_stop = Form("./figures/SignalQA_Sensor%d.pdf]",sensorId);
  c_Signal->Print(output_stop.c_str()); // open pdf file
}

