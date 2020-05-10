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

void plotSignalQA(string hv = "HV140V", bool isSavePed = true, bool isApplyCMNCorr = false, float nFstHitsCut = 4.5, int numOfUsedTimeBins = 3)
{
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.25); gStyle->SetStatH(0.55);

  TH1F *h_mMeanSignalHits_Rstrip[4];
  TH1F *h_mMeanNoiseHits_Rstrip[4];
  TH1F *h_mMeanSNRatioHits_Rstrip[4];
  TH1F *h_mMeanSignalClusters_Rstrip[4];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    string HistName;
    HistName = Form("h_mMeanSignalHits_Rstrip%d",i_rstrip);
    h_mMeanSignalHits_Rstrip[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
    HistName = Form("h_mMeanNoiseHits_Rstrip%d",i_rstrip);
    h_mMeanNoiseHits_Rstrip[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
    HistName = Form("h_mMeanSNRatioHits_Rstrip%d",i_rstrip);
    h_mMeanSNRatioHits_Rstrip[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
    HistName = Form("h_mMeanSignalClusters_Rstrip%d",i_rstrip);
    h_mMeanSignalClusters_Rstrip[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),FST::numTBins,-0.5,FST::numTBins-0.5);
  }

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstQAStudy_%s_Th%1.1fTb%d_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,pedMode.c_str(),cmnMode.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TProfile2D *p_mPedMap_FST = (TProfile2D*)File_InPut->Get("p_mPedMap_FST");;
  TProfile2D *p_mSigMap_FST = (TProfile2D*)File_InPut->Get("p_mSigMap_FST");
  TH1F *h_mSignalHits_FST = (TH1F*)File_InPut->Get("h_mSignalHits_FST");
  TH1F *h_mNoiseHits_FST = (TH1F*)File_InPut->Get("h_mNoiseHits_FST");
  TH1F *h_mSNRatioHits_FST = (TH1F*)File_InPut->Get("h_mSNRatioHits_FST");
  TH1F *h_mSignalClusters_FST = (TH1F*)File_InPut->Get("h_mSignalClusters_FST");

  TH1F *h_mMaxTbHits_Rstrip[4];
  TH1F *h_mMaxTbClusters_Rstrip[4];
  TH1F *h_mSignalHits_Rstrip[4];
  TH1F *h_mNoiseHits_Rstrip[4];
  TH1F *h_mSNRatioHits_Rstrip[4];
  TH1F *h_mSignalClusters_Rstrip[4];
  TH1F *h_mSignalHits_Rstrip_TimeBin[4][FST::numTBins];
  TH1F *h_mNoiseHits_Rstrip_TimeBin[4][FST::numTBins];
  TH1F *h_mSNRatioHits_Rstrip_TimeBin[4][FST::numTBins];
  TH1F *h_mSignalClusters_Rstrip_TimeBin[4][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    string HistName;
    HistName = Form("h_mMaxTbHits_Rstrip%d",i_rstrip);
    h_mMaxTbHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mMaxTbClusters_Rstrip%d",i_rstrip);
    h_mMaxTbClusters_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSignalHits_Rstrip%d",i_rstrip);
    h_mSignalHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mNoiseHits_Rstrip%d",i_rstrip);
    h_mNoiseHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSNRatioHits_Rstrip%d",i_rstrip);
    h_mSNRatioHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSignalClusters_Rstrip%d",i_rstrip);
    h_mSignalClusters_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      HistName = Form("h_mSignalHits_Rstrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
      h_mMeanSignalHits_Rstrip[i_rstrip]->SetBinContent(i_tb+1,h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetMean());
      HistName = Form("h_mNoiseHits_Rstrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
      h_mMeanNoiseHits_Rstrip[i_rstrip]->SetBinContent(i_tb+1,h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetMean());
      HistName = Form("h_mSNRatioHits_Rstrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->SetBinContent(i_tb+1,h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetMean());
      HistName = Form("h_mSignalClusters_Rstrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
      h_mMeanSignalClusters_Rstrip[i_rstrip]->SetBinContent(i_tb+1,h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb]->GetMean());
    }
  }


  string outputname = "./figures/SignalQA.pdf";

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

  string output_start = "./figures/SignalQA.pdf[";
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
  h_mSignalHits_FST->SetTitle("Signal of Hits");
  h_mSignalHits_FST->GetXaxis()->SetTitle("ADC");
  h_mSignalHits_FST->GetXaxis()->SetTitleSize(0.06);
  h_mSignalHits_FST->Draw("HIST");

  c_Signal->cd(4);
  h_mSignalClusters_FST->SetTitle("Signal of Clusters");
  h_mSignalClusters_FST->GetXaxis()->SetTitle("ADC");
  h_mSignalClusters_FST->GetXaxis()->SetTitleSize(0.06);
  h_mSignalClusters_FST->Draw("HIST");

  c_Signal->cd(5);
  h_mNoiseHits_FST->SetTitle("Noise");
  h_mNoiseHits_FST->GetXaxis()->SetTitle("ADC");
  h_mNoiseHits_FST->GetXaxis()->SetTitleSize(0.06);
  h_mNoiseHits_FST->Draw("HIST");

  c_Signal->cd(6);
  h_mSNRatioHits_FST->SetTitle("Signal/Noise of Hits");
  h_mSNRatioHits_FST->GetXaxis()->SetTitle("Signal/Noise");
  h_mSNRatioHits_FST->GetXaxis()->SetTitleSize(0.06);
  h_mSNRatioHits_FST->Draw("HIST");

  c_Signal->Update();
  c_Signal->Print(outputname.c_str());

  // ADC & Time Bin
  c_Signal->cd(1);
  {
    TF1 *f_landau_FST[4];
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mSignalHits_Rstrip[i_rstrip]->SetStats(0);
      h_mSignalHits_Rstrip[i_rstrip]->SetTitle("FST Hits");
      h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mSignalHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      string FuncName = Form("f_landau_FST_%d",i_rstrip);
      f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
      f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
      h_mSignalHits_Rstrip[i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
      f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
      f_landau_FST[i_rstrip]->SetLineStyle(2);
      f_landau_FST[i_rstrip]->SetLineWidth(2);

      if(i_rstrip == 0) h_mSignalHits_Rstrip[i_rstrip]->Draw();
      else h_mSignalHits_Rstrip[i_rstrip]->Draw("same");
      // f_landau_FST[i_rstrip]->Draw("l same");

      // string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mSignalHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mSignalHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(2);
  {
    TF1 *f_landau_FST[4];
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mSignalClusters_Rstrip[i_rstrip]->SetStats(0);
      h_mSignalClusters_Rstrip[i_rstrip]->SetTitle("FST Clusters");
      h_mSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mSignalClusters_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      string FuncName = Form("f_landau_FST_%d",i_rstrip);
      f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
      f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
      h_mSignalClusters_Rstrip[i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
      f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
      f_landau_FST[i_rstrip]->SetLineStyle(2);
      f_landau_FST[i_rstrip]->SetLineWidth(2);

      if(i_rstrip == 0) h_mSignalClusters_Rstrip[i_rstrip]->Draw();
      else h_mSignalClusters_Rstrip[i_rstrip]->Draw("same");
      // f_landau_FST[i_rstrip]->Draw("l same");

      // string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mSignalClusters_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mSignalClusters_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(3);
  {
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mNoiseHits_Rstrip[i_rstrip]->SetStats(0);
      h_mNoiseHits_Rstrip[i_rstrip]->SetTitle("FST Noise");
      h_mNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mNoiseHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mNoiseHits_Rstrip[i_rstrip]->Draw();
      else h_mNoiseHits_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mNoiseHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mNoiseHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(4);
  {
    TLegend *leg_FST = new TLegend(0.50,0.6,0.90,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mSNRatioHits_Rstrip[i_rstrip]->SetStats(0);
      h_mSNRatioHits_Rstrip[i_rstrip]->SetTitle("FST Signal/Noise");
      h_mSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Signal/Noise");
      h_mSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mSNRatioHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mSNRatioHits_Rstrip[i_rstrip]->Draw();
      else h_mSNRatioHits_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mSNRatioHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mSNRatioHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(5);
  {
    TLegend *leg_FST = new TLegend(0.55,0.6,0.8,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mMaxTbHits_Rstrip[i_rstrip]->SetStats(0);
      h_mMaxTbHits_Rstrip[i_rstrip]->SetTitle("Max Time Bin Hits");
      h_mMaxTbHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Maxt Time Bin");
      h_mMaxTbHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMaxTbHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMaxTbHits_Rstrip[i_rstrip]->Draw();
      else h_mMaxTbHits_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mMaxTbHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mMaxTbHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Signal->cd(6);
  {
    TLegend *leg_FST = new TLegend(0.55,0.6,0.8,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mMaxTbClusters_Rstrip[i_rstrip]->SetStats(0);
      h_mMaxTbClusters_Rstrip[i_rstrip]->SetTitle("Max Time Bin Clusters");
      h_mMaxTbClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Maxt Time Bin");
      h_mMaxTbClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMaxTbClusters_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMaxTbClusters_Rstrip[i_rstrip]->Draw();
      else h_mMaxTbClusters_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mMaxTbClusters_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mMaxTbClusters_Rstrip[i_rstrip],LegName.c_str(),"L");
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
    TLegend *leg_FST = new TLegend(0.7,0.6,0.8,0.9);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mMeanSignalHits_Rstrip[i_rstrip]->SetTitle("");
      h_mMeanSignalHits_Rstrip[i_rstrip]->SetStats(0);
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetTitle("<ADC>_{Hits}");
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleSize(0.10);
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetRangeUser(0.5*h_mMeanSignalHits_Rstrip[i_rstrip]->GetMinimum(),1.5*h_mMeanSignalHits_Rstrip[i_rstrip]->GetMaximum());
      h_mMeanSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanSignalHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanSignalHits_Rstrip[i_rstrip]->Draw();
      else h_mMeanSignalHits_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d",i_rstrip);
      leg_FST->AddEntry(h_mMeanSignalHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_MeanSignal->cd(2);
  {
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mMeanSignalClusters_Rstrip[i_rstrip]->SetTitle("");
      h_mMeanSignalClusters_Rstrip[i_rstrip]->SetStats(0);
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetYaxis()->SetTitle("<ADC>_{Clusters}");
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetYaxis()->SetTitleSize(0.08);
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetYaxis()->SetRangeUser(0.5*h_mMeanSignalClusters_Rstrip[i_rstrip]->GetMinimum(),1.5*h_mMeanSignalClusters_Rstrip[i_rstrip]->GetMaximum());
      h_mMeanSignalClusters_Rstrip[i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanSignalClusters_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanSignalClusters_Rstrip[i_rstrip]->Draw();
      else h_mMeanSignalClusters_Rstrip[i_rstrip]->Draw("same");
    }
  }

  c_MeanSignal->cd(3);
  {
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mMeanNoiseHits_Rstrip[i_rstrip]->SetTitle("");
      h_mMeanNoiseHits_Rstrip[i_rstrip]->SetStats(0);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetYaxis()->SetTitle("Noise <ADC>_{Hits}");
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleSize(0.08);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetYaxis()->SetRangeUser(5.0,25.0);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanNoiseHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanNoiseHits_Rstrip[i_rstrip]->Draw();
      else h_mMeanNoiseHits_Rstrip[i_rstrip]->Draw("same");
    }
  }

  c_MeanSignal->cd(4);
  {
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->SetTitle("");
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->SetStats(0);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Time Bin");
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetLabelSize(0.06);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetYaxis()->SetTitle("<Signal/Noise>_{Hits}");
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleSize(0.08);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleOffset(0.5);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetYaxis()->SetRangeUser(5.0,35.0);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->GetYaxis()->SetLabelSize(0.08);
      h_mMeanSNRatioHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mMeanSNRatioHits_Rstrip[i_rstrip]->Draw();
      else h_mMeanSNRatioHits_Rstrip[i_rstrip]->Draw("same");
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
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb]->SetTitle(title.c_str());
      // h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb]->SetStats(0);
      h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mSignalHits_Rstrip_TimeBin[i_rstrip][i_tb]->Draw();
      if(i_rstrip == 0 && i_tb == 0) plotTopLegend((char*)"Hits",0.3,0.6,0.15,1,0.0,42,1,1);
    }
  }
  c_Signal_TimeBin->Update();
  c_Signal_TimeBin->Print(outputname.c_str());

  // Noise
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->SetTitle(title.c_str());
      // h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->SetStats(0);
      h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetXaxis()->SetRangeUser(0.0,40.0);
      h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mNoiseHits_Rstrip_TimeBin[i_rstrip][i_tb]->Draw();
    }
  }
  c_Signal_TimeBin->Update();
  c_Signal_TimeBin->Print(outputname.c_str());

  // Signal/Noise
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb]->SetTitle(title.c_str());
      // h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb]->SetStats(0);
      h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mSNRatioHits_Rstrip_TimeBin[i_rstrip][i_tb]->Draw();
    }
  }
  c_Signal_TimeBin->Update();
  c_Signal_TimeBin->Print(outputname.c_str());

  // Signal of Clusters
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Signal_TimeBin->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb]->SetTitle(title.c_str());
      // h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb]->SetStats(0);
      h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.04);
      h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mSignalClusters_Rstrip_TimeBin[i_rstrip][i_tb]->Draw();
      if(i_rstrip == 0 && i_tb == 0) plotTopLegend((char*)"Clusters",0.3,0.6,0.15,1,0.0,42,1,1);
    }
  }
  c_Signal_TimeBin->Update();
  c_Signal_TimeBin->Print(outputname.c_str());
  //--------------------------------------------------

  // c_Signal->SaveAs("./figures/SignalQA.pdf");
  string output_stop = "./figures/SignalQA.pdf]";
  c_Signal->Print(output_stop.c_str()); // open pdf file
}

