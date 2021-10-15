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

void plotNoiseTriggerRates(string mod = "Mod04", string hv = "HV70V")
{
  const int defTimeBin = 0;

  string inputDef = Form("../../output/noise/%s/HVScan/BNL/FstPedNoise_%s_%s.root",mod.c_str(),mod.c_str(),hv.c_str());
  TFile *File_Default = TFile::Open(inputDef.c_str());
  TH1F *h_mPedSigmaDef[FST::numRStrip][FST::numTBins];
  TH1F *h_mRanSigmaDef[FST::numRStrip][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      std::string HistName;
      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedSigmaDef[i_rstrip][i_tb] = (TH1F*)File_Default->Get(HistName.c_str());
      HistName = Form("h_mRanSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mRanSigmaDef[i_rstrip][i_tb] = (TH1F*)File_Default->Get(HistName.c_str());
    }
  }

  string input10Hz = Form("../../output/noise/%s/HVScan/BNL/FstPedNoise_%s_%s_10Hz.root",mod.c_str(),mod.c_str(),hv.c_str());
  TFile *File_10Hz = TFile::Open(input10Hz.c_str());
  TH1F *h_mPedSigma10Hz[FST::numRStrip][FST::numTBins];
  TH1F *h_mRanSigma10Hz[FST::numRStrip][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      std::string HistName;
      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedSigma10Hz[i_rstrip][i_tb] = (TH1F*)File_10Hz->Get(HistName.c_str());
      HistName = Form("h_mRanSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mRanSigma10Hz[i_rstrip][i_tb] = (TH1F*)File_10Hz->Get(HistName.c_str());
    }
  }

  string input1kHz = Form("../../output/noise/%s/HVScan/BNL/FstPedNoise_%s_%s_1kHz.root",mod.c_str(),mod.c_str(),hv.c_str());
  TFile *File_1kHz = TFile::Open(input1kHz.c_str());
  TH1F *h_mPedSigma1kHz[FST::numRStrip][FST::numTBins];
  TH1F *h_mRanSigma1kHz[FST::numRStrip][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      std::string HistName;
      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedSigma1kHz[i_rstrip][i_tb] = (TH1F*)File_1kHz->Get(HistName.c_str());
      HistName = Form("h_mRanSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mRanSigma1kHz[i_rstrip][i_tb] = (TH1F*)File_1kHz->Get(HistName.c_str());
    }
  }

  string input10Hz_confYu = Form("../../output/noise/%s/HVScan/BNL/FstPedNoise_%s_%s_10Hz_confYu.root",mod.c_str(),mod.c_str(),hv.c_str());
  TFile *File_10Hz_confYu = TFile::Open(input10Hz_confYu.c_str());
  TH1F *h_mPedSigma10Hz_confYu[FST::numRStrip][FST::numTBins];
  TH1F *h_mRanSigma10Hz_confYu[FST::numRStrip][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      std::string HistName;
      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedSigma10Hz_confYu[i_rstrip][i_tb] = (TH1F*)File_10Hz_confYu->Get(HistName.c_str());
      HistName = Form("h_mRanSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mRanSigma10Hz_confYu[i_rstrip][i_tb] = (TH1F*)File_10Hz_confYu->Get(HistName.c_str());
    }
  }

  TCanvas *c_PedSigma = new TCanvas("c_PedSigma","c_PedSigma",10,10,1600,800);
  c_PedSigma->Divide(4,2);
  for(int i_pad = 0; i_pad < 8; ++i_pad)
  {
    c_PedSigma->cd(i_pad+1)->SetLeftMargin(0.15);
    c_PedSigma->cd(i_pad+1)->SetBottomMargin(0.15);
    c_PedSigma->cd(i_pad+1)->SetTicks(1,1);
    c_PedSigma->cd(i_pad+1)->SetGrid(0,0);
  }

  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    c_PedSigma->cd(i_rstrip+1);
    string title = Form("Total Noise: R-Strip %d",i_rstrip);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->SetTitle(title.c_str());
    h_mPedSigmaDef[i_rstrip][defTimeBin]->SetStats(0);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetXaxis()->SetTitle("Channel");
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetXaxis()->SetTitleSize(0.06);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetXaxis()->SetLabelSize(0.06);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetRangeUser(-1.5,50.0);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetTitle("ADC");
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetTitleSize(0.06);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetLabelSize(0.06);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->SetMarkerStyle(24);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->SetMarkerColor(1);
    h_mPedSigmaDef[i_rstrip][defTimeBin]->Draw("p");
    h_mPedSigma1kHz[i_rstrip][defTimeBin]->SetMarkerStyle(25);
    h_mPedSigma1kHz[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    h_mPedSigma1kHz[i_rstrip][defTimeBin]->SetMarkerColor(2);
    h_mPedSigma1kHz[i_rstrip][defTimeBin]->Draw("p same");
    h_mPedSigma10Hz[i_rstrip][defTimeBin]->SetMarkerStyle(26);
    h_mPedSigma10Hz[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    h_mPedSigma10Hz[i_rstrip][defTimeBin]->SetMarkerColor(4);
    h_mPedSigma10Hz[i_rstrip][defTimeBin]->Draw("p same");
    // h_mPedSigma10Hz_confYu[i_rstrip][defTimeBin]->SetMarkerStyle(32);
    // h_mPedSigma10Hz_confYu[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    // h_mPedSigma10Hz_confYu[i_rstrip][defTimeBin]->SetMarkerColor(7);
    // h_mPedSigma10Hz_confYu[i_rstrip][defTimeBin]->Draw("p same");
    PlotLine(-0.5, 127.5,0.0,0.0,1,1,2);
    TLegend *leg = new TLegend(0.15,0.65,0.85,0.85);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry(h_mPedSigma10Hz[i_rstrip][defTimeBin],"10Hz","P");
    leg->AddEntry(h_mPedSigmaDef[i_rstrip][defTimeBin],"100Hz","P");
    leg->AddEntry(h_mPedSigma1kHz[i_rstrip][defTimeBin],"1kHz","P");
    leg->Draw("same");
  }
  string FigName_Ped = Form("./figures/%s/c_PedSigmaTriggerRates_%s_%s.eps",mod.c_str(),mod.c_str(),hv.c_str());
  c_PedSigma->SaveAs(FigName_Ped.c_str());

  TCanvas *c_RanSigma = new TCanvas("c_RanSigma","c_RanSigma",10,10,1600,800);
  c_RanSigma->Divide(4,2);
  for(int i_pad = 0; i_pad < 8; ++i_pad)
  {
    c_RanSigma->cd(i_pad+1)->SetLeftMargin(0.15);
    c_RanSigma->cd(i_pad+1)->SetBottomMargin(0.15);
    c_RanSigma->cd(i_pad+1)->SetTicks(1,1);
    c_RanSigma->cd(i_pad+1)->SetGrid(0,0);
  }

  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    c_RanSigma->cd(i_rstrip+1);
    string title = Form("Random Noise: R-Strip %d",i_rstrip);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->SetTitle(title.c_str());
    h_mRanSigmaDef[i_rstrip][defTimeBin]->SetStats(0);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetXaxis()->SetTitle("Channel");
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetXaxis()->SetTitleSize(0.06);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetXaxis()->SetLabelSize(0.06);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetRangeUser(-1.5,50.0);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetTitle("ADC");
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetTitleSize(0.06);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->GetYaxis()->SetLabelSize(0.06);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->SetMarkerStyle(24);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->SetMarkerColor(1);
    h_mRanSigmaDef[i_rstrip][defTimeBin]->Draw("p");
    h_mRanSigma1kHz[i_rstrip][defTimeBin]->SetMarkerStyle(25);
    h_mRanSigma1kHz[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    h_mRanSigma1kHz[i_rstrip][defTimeBin]->SetMarkerColor(2);
    h_mRanSigma1kHz[i_rstrip][defTimeBin]->Draw("p same");
    h_mRanSigma10Hz[i_rstrip][defTimeBin]->SetMarkerStyle(26);
    h_mRanSigma10Hz[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    h_mRanSigma10Hz[i_rstrip][defTimeBin]->SetMarkerColor(4);
    h_mRanSigma10Hz[i_rstrip][defTimeBin]->Draw("p same");
    // h_mRanSigma10Hz_confYu[i_rstrip][defTimeBin]->SetMarkerStyle(32);
    // h_mRanSigma10Hz_confYu[i_rstrip][defTimeBin]->SetMarkerSize(1.0);
    // h_mRanSigma10Hz_confYu[i_rstrip][defTimeBin]->SetMarkerColor(7);
    // h_mRanSigma10Hz_confYu[i_rstrip][defTimeBin]->Draw("p same");
    PlotLine(-0.5, 127.5,0.0,0.0,1,1,2);
    TLegend *leg = new TLegend(0.15,0.65,0.85,0.85);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetMargin(0.1);
    leg->AddEntry(h_mRanSigma10Hz[i_rstrip][defTimeBin],"10Hz","P");
    leg->AddEntry(h_mRanSigmaDef[i_rstrip][defTimeBin],"100Hz","P");
    leg->AddEntry(h_mRanSigma1kHz[i_rstrip][defTimeBin],"1kHz","P");
    leg->Draw("same");
  }
  string FigName_Ran = Form("./figures/%s/c_RanSigmaTriggerRates_%s_%s.eps",mod.c_str(),mod.c_str(),hv.c_str());
  c_RanSigma->SaveAs(FigName_Ran.c_str());
}
