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

void plotSensorDataNoiseQA_WireBond(string module = "Mod04", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.35); gStyle->SetStatH(0.35);
  
  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  TH1F *h_mPedSigma_FST[2][FST::numRStrip][FST::numTBins];
  TH1F *h_mCMNSigma_FST[2][FST::numRStrip][FST::numTBins];
  TH1F *h_mRanSigma_FST[2][FST::numRStrip][FST::numTBins];

  for(int i_config = 0; i_config < 2; ++i_config)
  {
    string config;
    if(i_config == 0) config = "clusters";
    if(i_config == 1) config = "clusters_wb";
    string inputfile = Form("../../output/%s/FstClusters_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_AllData.root",config.c_str(),module.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

    TFile *File_InPut = TFile::Open(inputfile.c_str());

    for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
    {
      for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
      {
        string HistName;
        HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());

        HistName = Form("h_mCMNSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());

        HistName = Form("h_mRanSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
      }
    }
  }
  string outputname = Form("./figures/wirebond/DataNoiseQA_%s_%s.pdf",module.c_str(),hv.c_str());

  TCanvas *c_Noise = new TCanvas("c_Noise","c_Noise",10,10,3600,1600);
  c_Noise->Divide(9,4);
  for(int i_pad = 0; i_pad < 36; ++i_pad)
  {
    c_Noise->cd(i_pad+1)->SetLeftMargin(0.1);
    // c_Noise->cd(i_pad+1)->SetRightMargin(0.15);
    c_Noise->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Noise->cd(i_pad+1)->SetTicks(1,1);
    c_Noise->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("./figures/wirebond/DataNoiseQA_%s_%s.pdf[",module.c_str(),hv.c_str());
  c_Noise->Print(output_start.c_str()); // open pdf file

  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Noise->cd(i_pad+1);
      for(int i_config = 0; i_config < 2; i_config++)
      {
        string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->SetTitle(title.c_str());
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->SetStats(0);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetRangeUser(-0.5,127.5);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0.0,40.0);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetTitle("channel");
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0,40);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
        h_mPedSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetTitle("ADC");
        if(i_config == 0)
        {
          h_mPedSigma_FST[i_config][i_rstrip][i_tb]->SetLineColor(1);
          h_mPedSigma_FST[i_config][i_rstrip][i_tb]->Draw();
        }
        else
        {
          h_mPedSigma_FST[i_config][i_rstrip][i_tb]->SetLineColor(2);
          h_mPedSigma_FST[i_config][i_rstrip][i_tb]->Draw("same");
        }
        PlotLine(31.5, 31.5,0,40,2,1,2);
        PlotLine(63.5, 63.5,0,40,2,1,2);
        PlotLine(95.5, 95.5,0,40,2,1,2);
        if(i_config == 0 && (i_rstrip == 0 || i_rstrip == 4) && i_tb == 0)
        {
	  TLegend *leg = new TLegend(0.2,0.6,0.8,0.9);
	  leg->SetBorderSize(0);
	  leg->SetFillColor(10);
	  leg->AddEntry(h_mPedSigma_FST[0][i_rstrip][i_tb],"w/o Encapsulation","L");
	  leg->AddEntry(h_mPedSigma_FST[1][i_rstrip][i_tb],"w/ Encapsulation","L");
	  leg->Draw("same");
        }
      }
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Noise->cd();
      string TitleName = "Total Noise";
      TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
      canvas_title->SetBorderSize(1);
      canvas_title->Draw();
      c_Noise->Update();
      c_Noise->Print(outputname.c_str());
    }
  }

  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Noise->cd(i_pad+1);
      for(int i_config = 0; i_config < 2; i_config++)
      {
        string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->SetTitle(title.c_str());
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->SetStats(0);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetRangeUser(-0.5,127.5);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0.0,40.0);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetTitle("channel");
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0,40);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
        h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetTitle("ADC");
        if(i_config == 0)
        {
          h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->SetLineColor(1);
          h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->Draw();
        }
        else
        {
          h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->SetLineColor(2);
          h_mCMNSigma_FST[i_config][i_rstrip][i_tb]->Draw("same");
        }
        PlotLine(31.5, 31.5,0,40,2,1,2);
        PlotLine(63.5, 63.5,0,40,2,1,2);
        PlotLine(95.5, 95.5,0,40,2,1,2);
        if(i_config == 0 && (i_rstrip == 0 || i_rstrip == 4) && i_tb == 0)
        {
	  TLegend *leg = new TLegend(0.2,0.6,0.8,0.9);
	  leg->SetBorderSize(0);
	  leg->SetFillColor(10);
	  leg->AddEntry(h_mCMNSigma_FST[0][i_rstrip][i_tb],"w/o Encapsulation","L");
	  leg->AddEntry(h_mCMNSigma_FST[1][i_rstrip][i_tb],"w/ Encapsulation","L");
	  leg->Draw("same");
        }
      }
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Noise->cd();
      string TitleName = "Common Mode Noise";
      TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
      canvas_title->SetBorderSize(1);
      canvas_title->Draw();
      c_Noise->Update();
      c_Noise->Print(outputname.c_str());
    }
  }

  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      if(i_rstrip > 3) i_pad = (i_rstrip-4)*9 + i_tb;
      c_Noise->cd(i_pad+1);
      for(int i_config = 0; i_config < 2; i_config++)
      {
        string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->SetTitle(title.c_str());
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->SetStats(0);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetRangeUser(-0.5,127.5);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->SetMinimum(0.0);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->SetMaximum(40.0);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->GetXaxis()->SetTitle("channel");
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0,40);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
        h_mRanSigma_FST[i_config][i_rstrip][i_tb]->GetYaxis()->SetTitle("ADC");
        if(i_config == 0)
        {
          h_mRanSigma_FST[i_config][i_rstrip][i_tb]->SetLineColor(1);
          h_mRanSigma_FST[i_config][i_rstrip][i_tb]->Draw();
        }
        else
        {
          h_mRanSigma_FST[i_config][i_rstrip][i_tb]->SetLineColor(2);
          h_mRanSigma_FST[i_config][i_rstrip][i_tb]->Draw("same");
        }
        PlotLine(31.5, 31.5,0,40,2,1,2);
        PlotLine(63.5, 63.5,0,40,2,1,2);
        PlotLine(95.5, 95.5,0,40,2,1,2);
        if(i_config == 0 && (i_rstrip == 0 || i_rstrip == 4) && i_tb == 0)
        {
	  TLegend *leg = new TLegend(0.2,0.6,0.8,0.9);
	  leg->SetBorderSize(0);
	  leg->SetFillColor(10);
	  leg->AddEntry(h_mRanSigma_FST[0][i_rstrip][i_tb],"w/o Encapsulation","L");
	  leg->AddEntry(h_mRanSigma_FST[1][i_rstrip][i_tb],"w/ Encapsulation","L");
	  leg->Draw("same");
        }
      }
    }
    if(i_rstrip == 3 || i_rstrip == FST::numRStrip-1)
    {
      c_Noise->cd();
      string TitleName = "Random Noise";
      TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
      canvas_title->SetBorderSize(1);
      canvas_title->Draw();
      c_Noise->Update();
      c_Noise->Print(outputname.c_str());
    }
  }

  string output_stop = Form("./figures/wirebond/DataNoiseQA_%s_%s.pdf]",module.c_str(),hv.c_str());
  c_Noise->Print(output_stop.c_str()); // open pdf file
}

