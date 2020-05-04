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

void plotNoiseQA(string hv = "HV140V", string mode = "Ped")
{
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.35); gStyle->SetStatH(0.35);

  string inputfile = Form("../../output/noise/Fst%sNoise_%s.root",mode.c_str(),hv.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH1F *h_mPedMean_FST[4][FST::numTBins]; // for RStrip
  TH1F *h_mPedSigma_FST[4][FST::numTBins];
  TH1F *h_mCMNSigma_FST[4][FST::numTBins];
  TH1F *h_mDiffSigma_FST[4][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      string HistName;
      HistName = Form("h_mPedMean_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedMean_FST[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());

      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedSigma_FST[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());

      HistName = Form("h_mCMNSigma_FST_RStrip%d_TimeBin%d",i_rstrip+4,i_tb);
      h_mCMNSigma_FST[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());

      HistName = Form("h_mDiffSigma_FST_RStrip%d_TimeBin%d",i_rstrip+4,i_tb);
      h_mDiffSigma_FST[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
    }
  }

  string outputname = Form("./figures/%sNoiseQA.pdf",mode.c_str());

  TCanvas *c_Noise = new TCanvas("c_Noise","c_Noise",10,10,1800,800);
  c_Noise->Divide(9,4);
  for(int i_pad = 0; i_pad < 36; ++i_pad)
  {
    c_Noise->cd(i_pad+1)->SetLeftMargin(0.1);
    // c_Noise->cd(i_pad+1)->SetRightMargin(0.15);
    c_Noise->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Noise->cd(i_pad+1)->SetTicks(1,1);
    c_Noise->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("./figures/%sNoiseQA.pdf[",mode.c_str());
  c_Noise->Print(output_start.c_str()); // open pdf file

  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Noise->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mPedMean_FST[i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mPedMean_FST[i_rstrip][i_tb]->SetStats(0);
      h_mPedMean_FST[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
      h_mPedMean_FST[i_rstrip][i_tb]->GetYaxis()->SetRangeUser(200,700);
      h_mPedMean_FST[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mPedMean_FST[i_rstrip][i_tb]->Draw();
      PlotLine(31.5, 31.5,200,700,2,1,2);
    }
  }
  c_Noise->Update();
  c_Noise->Print(outputname.c_str());

  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Noise->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mPedSigma_FST[i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mPedSigma_FST[i_rstrip][i_tb]->SetStats(0);
      h_mPedSigma_FST[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
      h_mPedSigma_FST[i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0,40);
      h_mPedSigma_FST[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mPedSigma_FST[i_rstrip][i_tb]->Draw();
      PlotLine(31.5, 31.5,0,40,2,1,2);
    }
  }
  c_Noise->Update();
  c_Noise->Print(outputname.c_str());

  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Noise->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mCMNSigma_FST[i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mCMNSigma_FST[i_rstrip][i_tb]->SetStats(0);
      h_mCMNSigma_FST[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
      h_mCMNSigma_FST[i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0,40);
      h_mCMNSigma_FST[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mCMNSigma_FST[i_rstrip][i_tb]->Draw();
      PlotLine(31.5, 31.5,0,40,2,1,2);
    }
  }
  c_Noise->Update();
  c_Noise->Print(outputname.c_str());

  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      int i_pad = i_rstrip*9 + i_tb;
      c_Noise->cd(i_pad+1);
      string title = Form("RStrip%d & TB%d",i_rstrip,i_tb);
      h_mDiffSigma_FST[i_rstrip][i_tb]->SetTitle(title.c_str());
      h_mDiffSigma_FST[i_rstrip][i_tb]->SetStats(0);
      h_mDiffSigma_FST[i_rstrip][i_tb]->GetXaxis()->SetLabelSize(0.06);
      h_mDiffSigma_FST[i_rstrip][i_tb]->GetYaxis()->SetRangeUser(0,40);
      h_mDiffSigma_FST[i_rstrip][i_tb]->GetYaxis()->SetLabelSize(0.06);
      h_mDiffSigma_FST[i_rstrip][i_tb]->Draw();
      PlotLine(31.5, 31.5,0,40,2,1,2);
    }
  }
  c_Noise->Update();
  c_Noise->Print(outputname.c_str());

  TGraph *g_mPedSigma[FST::numTBins];
  TGraph *g_mCMNSigma_FST[FST::numTBins];
  TGraph *g_mDiffSigma_FST[FST::numTBins];

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    std::string gName;
    gName = Form("g_mPedSigma_Layer0_TimeBin%d",i_tb);
    g_mPedSigma[i_tb] = (TGraph*)File_InPut->Get(gName.c_str());

    gName = Form("g_mNoiseCMN_FST_TimeBin%d",i_tb);
    g_mCMNSigma_FST[i_tb] = (TGraph*)File_InPut->Get(gName.c_str());

    gName = Form("g_mNoiseDiff_FST_TimeBin%d",i_tb);
    g_mDiffSigma_FST[i_tb] = (TGraph*)File_InPut->Get(gName.c_str());
  }

  TH1F *h_play = new TH1F("h_play","h_play",2000,-0.5,1999.5);
  for(int i_bin = 0; i_bin < 2000; ++i_bin)
  {
    h_play->SetBinContent(i_bin+1,-1000.0);
    h_play->SetBinError(i_bin+1,1.0);
  }
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("ch");
  h_play->GetXaxis()->SetTitleSize(0.06);
  h_play->GetXaxis()->SetRangeUser(4*128-50,6*128+50);
  h_play->GetYaxis()->SetTitle("noise adc");
  h_play->GetYaxis()->SetTitleSize(0.06);
  h_play->GetYaxis()->SetRangeUser(0.0,50);

  TCanvas *c_NoiseSum = new TCanvas("c_NoiseSum","c_NoiseSum",10,10,900,900);
  c_NoiseSum->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_NoiseSum->cd(i_pad+1)->SetLeftMargin(0.15);
    c_NoiseSum->cd(i_pad+1)->SetBottomMargin(0.15);
    c_NoiseSum->cd(i_pad+1)->SetTicks(1,1);
    c_NoiseSum->cd(i_pad+1)->SetGrid(0,0);
  }

  TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    c_NoiseSum->cd(i_tb+1);
    string title = Form("Noise @ TimeBin%d",i_tb);
    h_play->SetTitle(title.c_str());
    h_play->DrawCopy("pE");

    g_mPedSigma[i_tb]->SetMarkerStyle(20);
    g_mPedSigma[i_tb]->SetMarkerColor(kGray+1);
    g_mPedSigma[i_tb]->SetMarkerSize(1.0);
    g_mPedSigma[i_tb]->Draw("P same");

    // g_mCMNSigma_FST[i_tb]->SetMarkerStyle(24);
    // g_mCMNSigma_FST[i_tb]->SetMarkerColor(2);
    // g_mCMNSigma_FST[i_tb]->SetMarkerSize(1.0);
    // g_mCMNSigma_FST[i_tb]->Draw("P same");
    //
    // g_mDiffSigma_FST[i_tb]->SetMarkerStyle(24);
    // g_mDiffSigma_FST[i_tb]->SetMarkerColor(4);
    // g_mDiffSigma_FST[i_tb]->SetMarkerSize(1.0);
    // g_mDiffSigma_FST[i_tb]->Draw("P same");
    if(i_tb == 0)
    {
      leg->AddEntry(g_mPedSigma[i_tb],"Total Noise","P");
      // leg->AddEntry(g_mCMNSigma_FST[i_tb],"CMN","P");
      // leg->AddEntry(g_mDiffSigma_FST[i_tb],"Differential Noise","P");
      leg->Draw("same");
    }
  }
  c_NoiseSum->Update();
  c_NoiseSum->Print(outputname.c_str());

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    c_NoiseSum->cd(i_tb+1);
    string title = Form("Noise @ TimeBin%d",i_tb);
    h_play->SetTitle(title.c_str());
    h_play->DrawCopy("pE");

    g_mPedSigma[i_tb]->SetMarkerStyle(20);
    g_mPedSigma[i_tb]->SetMarkerColor(kGray+1);
    g_mPedSigma[i_tb]->SetMarkerSize(1.0);
    g_mPedSigma[i_tb]->Draw("P same");

    g_mCMNSigma_FST[i_tb]->SetMarkerStyle(24);
    g_mCMNSigma_FST[i_tb]->SetMarkerColor(2);
    g_mCMNSigma_FST[i_tb]->SetMarkerSize(1.0);
    g_mCMNSigma_FST[i_tb]->Draw("P same");

    // g_mDiffSigma_FST[i_tb]->SetMarkerStyle(24);
    // g_mDiffSigma_FST[i_tb]->SetMarkerColor(4);
    // g_mDiffSigma_FST[i_tb]->SetMarkerSize(1.0);
    // g_mDiffSigma_FST[i_tb]->Draw("P same");
    if(i_tb == 0)
    {
      // leg->AddEntry(g_mPedSigma[i_tb],"Total Noise","P");
      leg->AddEntry(g_mCMNSigma_FST[i_tb],"CMN","P");
      // leg->AddEntry(g_mDiffSigma_FST[i_tb],"Differential Noise","P");
      leg->Draw("same");
    }
  }
  c_NoiseSum->Update();
  c_NoiseSum->Print(outputname.c_str());

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    c_NoiseSum->cd(i_tb+1);
    string title = Form("Noise @ TimeBin%d",i_tb);
    h_play->SetTitle(title.c_str());
    h_play->DrawCopy("pE");

    g_mPedSigma[i_tb]->SetMarkerStyle(20);
    g_mPedSigma[i_tb]->SetMarkerColor(kGray+1);
    g_mPedSigma[i_tb]->SetMarkerSize(1.0);
    g_mPedSigma[i_tb]->Draw("P same");

    g_mCMNSigma_FST[i_tb]->SetMarkerStyle(24);
    g_mCMNSigma_FST[i_tb]->SetMarkerColor(2);
    g_mCMNSigma_FST[i_tb]->SetMarkerSize(1.0);
    g_mCMNSigma_FST[i_tb]->Draw("P same");

    g_mDiffSigma_FST[i_tb]->SetMarkerStyle(24);
    g_mDiffSigma_FST[i_tb]->SetMarkerColor(4);
    g_mDiffSigma_FST[i_tb]->SetMarkerSize(1.0);
    g_mDiffSigma_FST[i_tb]->Draw("P same");
    if(i_tb == 0)
    {
      // leg->AddEntry(g_mPedSigma[i_tb],"Total Noise","P");
      // leg->AddEntry(g_mCMNSigma_FST[i_tb],"CMN","P");
      leg->AddEntry(g_mDiffSigma_FST[i_tb],"Differential Noise","P");
      leg->Draw("same");
    }
  }
  c_NoiseSum->Update();
  c_NoiseSum->Print(outputname.c_str());

  TH1F *h_ratio[FST::numTBins];
  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    string HistName = Form("h_ratio_%d",i_tb);
    h_ratio[i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),2000,-0.5,1999.5);
    for(int i_bin = 0; i_bin < 2000; ++i_bin)
    {
      double x_PedSigma, y_PedSigma;
      g_mPedSigma[i_tb]->GetPoint(i_bin,x_PedSigma,y_PedSigma);
      double x_DiffSigma, y_DiffSigma;
      g_mDiffSigma_FST[i_tb]->GetPoint(i_bin,x_DiffSigma,y_DiffSigma);
      if(y_PedSigma > 0)
      {
	h_ratio[i_tb]->SetBinContent(i_bin+1,y_DiffSigma/y_PedSigma);
      }
    }
  }

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    c_NoiseSum->cd(i_tb+1);
    string title = Form("Differetial Noise @ TimeBin%d",i_tb);
    h_ratio[i_tb]->SetTitle(title.c_str());
    h_ratio[i_tb]->SetStats(0);
    h_ratio[i_tb]->GetXaxis()->SetRangeUser(4*128-50,6*128+50);
    h_ratio[i_tb]->GetXaxis()->SetTitle("ch");
    h_ratio[i_tb]->GetXaxis()->SetTitleSize(0.06);
    h_ratio[i_tb]->GetYaxis()->SetTitle("Noise_{diff}/Noise_{total}");
    h_ratio[i_tb]->GetYaxis()->SetTitleSize(0.06);
    h_ratio[i_tb]->Draw("h");
  }
  c_NoiseSum->Update();
  c_NoiseSum->Print(outputname.c_str());

  string output_stop = Form("./figures/%sNoiseQA.pdf]",mode.c_str());
  c_Noise->Print(output_stop.c_str()); // open pdf file
}

