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

void plotNoiseTemp()
{
  const int defTimeBin = 0;

  const int numOfTemp = 19; // Mod03
  string temp[numOfTemp] = {"Ped","Daq","NoCooling7","NoCooling8","NoCooling1","NoCooling2","NoCooling3","NoCooling4","NoCooling5","NoCooling6","Cooling1","Cooling2","Cooling3","Cooling4","Cooling5","Cooling6","Cooling7","Cooling8","Cooling9"};
  int time[numOfTemp] = {-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,90,120,130};

  TFile *File_InPut[numOfTemp];
  TH1F *h_mPedSigma[numOfTemp][FST::numRStrip][FST::numTBins];
  TH1F *h_mCMNSigma[numOfTemp][FST::numRStrip][FST::numTBins];
  TH1F *h_mRanSigma[numOfTemp][FST::numRStrip][FST::numTBins];

  // read in noise histograms
  for(int i_temp = 0; i_temp < numOfTemp; ++i_temp)
  {
    string input_Sensor = Form("../../output/noise/Cooling/Fst%sNoise_Mod04_HV0V.root",temp[i_temp].c_str());
    File_InPut[i_temp] = TFile::Open(input_Sensor.c_str());
    for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
    {
      for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
      {
	std::string HistName;
	HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
	h_mPedSigma[i_temp][i_rstrip][i_tb] = (TH1F*)File_InPut[i_temp]->Get(HistName.c_str());
	HistName = Form("h_mCMNSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
	h_mCMNSigma[i_temp][i_rstrip][i_tb] = (TH1F*)File_InPut[i_temp]->Get(HistName.c_str());
	HistName = Form("h_mRanSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
	h_mRanSigma[i_temp][i_rstrip][i_tb] = (TH1F*)File_InPut[i_temp]->Get(HistName.c_str());
      }
    }
  }

  // calculate mean noise
  TGraphAsymmErrors *g_mMeanPedSigma[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanCMNSigma[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanRanSigma[FST::mFstNumSensorsPerModule];
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    string GraphName;

    GraphName = Form("g_mMeanPedSigma_Sensor%d",i_sensor);
    g_mMeanPedSigma[i_sensor] = new TGraphAsymmErrors();
    g_mMeanPedSigma[i_sensor]->SetName(GraphName.c_str());

    GraphName = Form("g_mMeanCMNSigma_Sensor%d",i_sensor);
    g_mMeanCMNSigma[i_sensor] = new TGraphAsymmErrors();
    g_mMeanCMNSigma[i_sensor]->SetName(GraphName.c_str());

    GraphName = Form("g_mMeanRanSigma_Sensor%d",i_sensor);
    g_mMeanRanSigma[i_sensor] = new TGraphAsymmErrors();
    g_mMeanRanSigma[i_sensor]->SetName(GraphName.c_str());
  }

  for(int i_temp = 0; i_temp < numOfTemp; ++i_temp)
  {
    for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
    {
      int pedCounts = 0;
      double meanPedSigam = 0.0;
      int cmnCounts = 0;
      double meanCMNSigam = 0.0;
      int ranCounts = 0;
      double meanRanSigam = 0.0;
      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
      {
	if(i_sensor == 0)
	{
	  for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	  { // calculate mean total and random noise
	    if(h_mPedSigma[i_temp][i_rstrip][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanPedSigam += h_mPedSigma[i_temp][i_rstrip][defTimeBin]->GetBinContent(i_ch+1);
	      pedCounts++;
	    }
	    if(h_mCMNSigma[i_temp][i_rstrip][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanCMNSigam += h_mCMNSigma[i_temp][i_rstrip][defTimeBin]->GetBinContent(i_ch+1);
	      cmnCounts++;
	    }
	    if(h_mRanSigma[i_temp][i_rstrip][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanRanSigam += h_mRanSigma[i_temp][i_rstrip][defTimeBin]->GetBinContent(i_ch+1);
	      ranCounts++;
	    }
	  }
	}
	if(i_sensor == 1)
	{
	  for(int i_ch = 0; i_ch < 64; ++i_ch)
	  { // calculate mean total and random noise
	    if(h_mPedSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanPedSigam += h_mPedSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinContent(i_ch+1);
	      pedCounts++;
	    }
	    if(h_mCMNSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanCMNSigam += h_mCMNSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinContent(i_ch+1);
	      cmnCounts++;
	    }
	    if(h_mRanSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanRanSigam += h_mRanSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinContent(i_ch+1);
	      ranCounts++;
	    }
	  }
	}
	if(i_sensor == 2)
	{
	  for(int i_ch = 64; i_ch < 128; ++i_ch)
	  { // calculate mean total and random noise
	    if(h_mPedSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanPedSigam += h_mPedSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinContent(i_ch+1);
	      pedCounts++;
	    }
	    if(h_mCMNSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanCMNSigam += h_mCMNSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinContent(i_ch+1);
	      cmnCounts++;
	    }
	    if(h_mRanSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinError(i_ch+1) > 0)
	    {
	      meanRanSigam += h_mRanSigma[i_temp][i_rstrip+4][defTimeBin]->GetBinContent(i_ch+1);
	      ranCounts++;
	    }
	  }
	}
      }
      // cout << "i_sensor = " << i_sensor << ", pedCounts = " << pedCounts << ", ranCounts = " << ranCounts << endl;
      g_mMeanPedSigma[i_sensor]->SetPoint(i_temp,time[i_temp],meanPedSigam/pedCounts);
      g_mMeanCMNSigma[i_sensor]->SetPoint(i_temp,time[i_temp],meanCMNSigam/cmnCounts);
      g_mMeanRanSigma[i_sensor]->SetPoint(i_temp,time[i_temp],meanRanSigam/ranCounts);
    }
  }

  // copy Ped reference point
  TGraphAsymmErrors *g_mMeanPedSigmaLocal[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanCMNSigmaLocal[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanRanSigmaLocal[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanPedSigmaDaq[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanCMNSigmaDaq[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanRanSigmaDaq[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanPedSigmaRO[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanCMNSigmaRO[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanRanSigmaRO[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanPedSigmaROCooling[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanCMNSigmaROCooling[FST::mFstNumSensorsPerModule];
  TGraphAsymmErrors *g_mMeanRanSigmaROCooling[FST::mFstNumSensorsPerModule];
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    g_mMeanPedSigmaLocal[i_sensor] = new TGraphAsymmErrors();
    g_mMeanPedSigmaLocal[i_sensor]->SetPoint(0,time[0],g_mMeanPedSigma[i_sensor]->GetPointY(0));
    g_mMeanPedSigmaDaq[i_sensor] = new TGraphAsymmErrors();
    g_mMeanPedSigmaDaq[i_sensor]->SetPoint(0,time[1],g_mMeanPedSigma[i_sensor]->GetPointY(1));
    g_mMeanPedSigmaRO[i_sensor] = new TGraphAsymmErrors();
    g_mMeanPedSigmaRO[i_sensor]->SetPoint(0,time[2],g_mMeanPedSigma[i_sensor]->GetPointY(2));
    g_mMeanPedSigmaROCooling[i_sensor] = new TGraphAsymmErrors();
    g_mMeanPedSigmaROCooling[i_sensor]->SetPoint(0,time[3],g_mMeanPedSigma[i_sensor]->GetPointY(3));

    g_mMeanCMNSigmaLocal[i_sensor] = new TGraphAsymmErrors();
    g_mMeanCMNSigmaLocal[i_sensor]->SetPoint(0,time[0],g_mMeanCMNSigma[i_sensor]->GetPointY(0));
    g_mMeanCMNSigmaDaq[i_sensor] = new TGraphAsymmErrors();
    g_mMeanCMNSigmaDaq[i_sensor]->SetPoint(0,time[1],g_mMeanCMNSigma[i_sensor]->GetPointY(1));
    g_mMeanCMNSigmaRO[i_sensor] = new TGraphAsymmErrors();
    g_mMeanCMNSigmaRO[i_sensor]->SetPoint(0,time[2],g_mMeanCMNSigma[i_sensor]->GetPointY(2));
    g_mMeanCMNSigmaROCooling[i_sensor] = new TGraphAsymmErrors();
    g_mMeanCMNSigmaROCooling[i_sensor]->SetPoint(0,time[3],g_mMeanCMNSigma[i_sensor]->GetPointY(3));

    g_mMeanRanSigmaLocal[i_sensor] = new TGraphAsymmErrors();
    g_mMeanRanSigmaLocal[i_sensor]->SetPoint(0,time[0],g_mMeanRanSigma[i_sensor]->GetPointY(0));
    g_mMeanRanSigmaDaq[i_sensor] = new TGraphAsymmErrors();
    g_mMeanRanSigmaDaq[i_sensor]->SetPoint(0,time[1],g_mMeanRanSigma[i_sensor]->GetPointY(1));
    g_mMeanRanSigmaRO[i_sensor] = new TGraphAsymmErrors();
    g_mMeanRanSigmaRO[i_sensor]->SetPoint(0,time[2],g_mMeanRanSigma[i_sensor]->GetPointY(2));
    g_mMeanRanSigmaROCooling[i_sensor] = new TGraphAsymmErrors();
    g_mMeanRanSigmaROCooling[i_sensor]->SetPoint(0,time[3],g_mMeanRanSigma[i_sensor]->GetPointY(3));
  }

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_point = 0; i_point < 8; ++i_point)
    {
      g_mMeanPedSigma[i_sensor]->RemovePoint(0); // remove points with bad DAQ
      g_mMeanCMNSigma[i_sensor]->RemovePoint(0);
      g_mMeanRanSigma[i_sensor]->RemovePoint(0);
    }
  }

  TH1F *h_play = new TH1F("h_play","h_play",200,-29.5,170.5);
  for(int i_bin = 0; i_bin < 200; ++i_bin)
  {
    h_play->SetBinContent(i_bin+1,-10.0);
    h_play->SetBinError(i_bin+1,1.0);
  }
  h_play->SetStats(0);
  h_play->GetXaxis()->SetRangeUser(-19.5,140.5);
  h_play->GetXaxis()->SetTitle("Time (mins)");
  h_play->GetXaxis()->SetTitleSize(0.06);
  h_play->GetXaxis()->SetNdivisions(505);
  h_play->GetXaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetRangeUser(-1.5,50.0);
  h_play->GetYaxis()->SetTitle("ADC");
  h_play->GetYaxis()->SetNdivisions(505);
  h_play->GetYaxis()->SetTitleSize(0.06);
  h_play->GetYaxis()->SetLabelSize(0.04);

  const int markerColor[3] = {kGray+2, 2, 4};
  const int markerStyleLocal[3] = {21, 25, 25};
  const int markerStyleDaq[3] = {22, 26, 26};
  const int markerStyleRO[3] = {29, 30, 30};
  const int markerStyleROCooling[3] = {34, 28, 28};
  const int markerStyle[3] = {20, 24, 24};

  TCanvas *c_CMNNoise = new TCanvas("c_CMNNoise","c_CMNNoise",10,10,1200,400);
  c_CMNNoise->cd()->SetLeftMargin(0.05);
  c_CMNNoise->cd()->SetBottomMargin(0.15);
  c_CMNNoise->cd()->SetTicks(1,1);
  c_CMNNoise->cd()->SetGrid(0,0);
  string title = "Common Mode Noise vs. Time (Temperature)";
  h_play->SetTitle(title.c_str());
  h_play->GetXaxis()->SetRangeUser(-19.5,140.5);
  h_play->GetXaxis()->SetNdivisions(520);
  h_play->GetYaxis()->SetTitle("Common Mode Noise (ADC)");
  h_play->GetYaxis()->SetTitleOffset(0.3);
  h_play->GetYaxis()->SetRangeUser(10,40);
  h_play->DrawCopy("hE");
  // PlotLine(-10.5, 140.5,0.0,0.0,1,1,2);
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    g_mMeanCMNSigmaLocal[i_sensor]->SetMarkerStyle(markerStyleLocal[i_sensor]);
    g_mMeanCMNSigmaLocal[i_sensor]->SetMarkerSize(2.0);
    g_mMeanCMNSigmaLocal[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanCMNSigmaLocal[i_sensor]->Draw("P same");

    g_mMeanCMNSigmaDaq[i_sensor]->SetMarkerStyle(markerStyleDaq[i_sensor]);
    g_mMeanCMNSigmaDaq[i_sensor]->SetMarkerSize(2.0);
    g_mMeanCMNSigmaDaq[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanCMNSigmaDaq[i_sensor]->Draw("P same");

    g_mMeanCMNSigmaRO[i_sensor]->SetMarkerStyle(markerStyleRO[i_sensor]);
    g_mMeanCMNSigmaRO[i_sensor]->SetMarkerSize(2.0);
    g_mMeanCMNSigmaRO[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanCMNSigmaRO[i_sensor]->Draw("P same");

    g_mMeanCMNSigmaROCooling[i_sensor]->SetMarkerStyle(markerStyleROCooling[i_sensor]);
    g_mMeanCMNSigmaROCooling[i_sensor]->SetMarkerSize(2.0);
    g_mMeanCMNSigmaROCooling[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanCMNSigmaROCooling[i_sensor]->Draw("P same");

    g_mMeanCMNSigma[i_sensor]->SetMarkerStyle(markerStyle[i_sensor]);
    g_mMeanCMNSigma[i_sensor]->SetMarkerSize(2.0);
    g_mMeanCMNSigma[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanCMNSigma[i_sensor]->Draw("P same");
  }
  plotTopLegend((char*)"reference",-7.0,12,0.06,1,0.0,42,0,1);
  PlotLine(10.0,10.0,10,40,2,2,2);
  PlotLine(32.0,32.0,10,40,2,2,2);
  plotTopLegend((char*)"w/o cooling",13.0,12,0.06,2,0.0,42,0,1);
  PlotLine(33.0,33.0,10,40,4,2,2);
  PlotLine(132.5,132.5,10,40,4,2,2);
  plotTopLegend((char*)"with cooling",50.0,12,0.06,4,0.0,42,0,1);
  c_CMNNoise->SaveAs("./figures/Cooling/CommonModeNoiseTemp.eps");

  TCanvas *c_RanNoise = new TCanvas("c_RanNoise","c_RanNoise",10,10,1200,400);
  c_RanNoise->cd()->SetLeftMargin(0.05);
  c_RanNoise->cd()->SetBottomMargin(0.15);
  c_RanNoise->cd()->SetTicks(1,1);
  c_RanNoise->cd()->SetGrid(0,0);
  title = "Random Noise vs. Time (Temperature)";
  h_play->SetTitle(title.c_str());
  h_play->GetXaxis()->SetRangeUser(-19.5,140.5);
  h_play->GetXaxis()->SetNdivisions(520);
  h_play->GetYaxis()->SetTitle("Random Noise (ADC)");
  h_play->GetYaxis()->SetTitleOffset(0.3);
  h_play->GetYaxis()->SetRangeUser(30,47);
  h_play->DrawCopy("hE");
  // PlotLine(-10.5, 140.5,0.0,0.0,1,1,2);
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    g_mMeanRanSigmaLocal[i_sensor]->SetMarkerStyle(markerStyleLocal[i_sensor]);
    g_mMeanRanSigmaLocal[i_sensor]->SetMarkerSize(2.0);
    g_mMeanRanSigmaLocal[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanRanSigmaLocal[i_sensor]->Draw("P same");

    g_mMeanRanSigmaDaq[i_sensor]->SetMarkerStyle(markerStyleDaq[i_sensor]);
    g_mMeanRanSigmaDaq[i_sensor]->SetMarkerSize(2.0);
    g_mMeanRanSigmaDaq[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanRanSigmaDaq[i_sensor]->Draw("P same");

    g_mMeanRanSigmaRO[i_sensor]->SetMarkerStyle(markerStyleRO[i_sensor]);
    g_mMeanRanSigmaRO[i_sensor]->SetMarkerSize(2.0);
    g_mMeanRanSigmaRO[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanRanSigmaRO[i_sensor]->Draw("P same");

    g_mMeanRanSigmaROCooling[i_sensor]->SetMarkerStyle(markerStyleROCooling[i_sensor]);
    g_mMeanRanSigmaROCooling[i_sensor]->SetMarkerSize(2.0);
    g_mMeanRanSigmaROCooling[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanRanSigmaROCooling[i_sensor]->Draw("P same");

    g_mMeanRanSigma[i_sensor]->SetMarkerStyle(markerStyle[i_sensor]);
    g_mMeanRanSigma[i_sensor]->SetMarkerSize(2.0);
    g_mMeanRanSigma[i_sensor]->SetMarkerColor(markerColor[i_sensor]);
    g_mMeanRanSigma[i_sensor]->Draw("P same");
  }
  plotTopLegend((char*)"reference",-7.0,31,0.06,1,0.0,42,0,1);
  PlotLine(10.0,10.0,30,47,2,2,2);
  PlotLine(32.0,32.0,30,47,2,2,2);
  plotTopLegend((char*)"w/o cooling",13.0,31,0.06,2,0.0,42,0,1);
  PlotLine(33.0,33.0,30,47,4,2,2);
  PlotLine(132.5,132.5,30,47,4,2,2);
  plotTopLegend((char*)"with cooling",50.0,31,0.06,4,0.0,42,0,1);

  TLegend *leg = new TLegend(0.10,0.65,0.90,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetMargin(0.1);
  leg->SetNColumns(5);
  leg->AddEntry(g_mMeanCMNSigmaLocal[0],"Local Test","P");
  leg->AddEntry(g_mMeanCMNSigmaDaq[0],"DAQ Test","P");
  leg->AddEntry(g_mMeanCMNSigmaRO[0],"RO Test (w/o coolant)","P");
  leg->AddEntry(g_mMeanCMNSigmaROCooling[0],"RO Test (with coolant)","P");
  leg->AddEntry(g_mMeanCMNSigma[0],"Cooling Test (Sensor0)","P");

  leg->AddEntry(g_mMeanCMNSigmaLocal[1],"Local Test","P");
  leg->AddEntry(g_mMeanCMNSigmaDaq[1],"DAQ Test","P");
  leg->AddEntry(g_mMeanCMNSigmaRO[1],"RO Test (w/o coolant)","P");
  leg->AddEntry(g_mMeanCMNSigmaROCooling[1],"RO Test (with coolant)","P");
  leg->AddEntry(g_mMeanCMNSigma[1],"Cooling Test (Sensor1)","P");

  leg->AddEntry(g_mMeanCMNSigmaLocal[2],"Local Test","P");
  leg->AddEntry(g_mMeanCMNSigmaDaq[2],"DAQ Test","P");
  leg->AddEntry(g_mMeanCMNSigmaRO[2],"RO Test (w/o coolant)","P");
  leg->AddEntry(g_mMeanCMNSigmaROCooling[2],"RO Test (with coolant)","P");
  leg->AddEntry(g_mMeanCMNSigma[2],"Cooling Test (Sensor2)","P");
  leg->Draw("same");

  c_RanNoise->SaveAs("./figures/Cooling/RandomNoiseTemp.eps");
}
