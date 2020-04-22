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

void plotNoiseQA(string hv = "HV140V", string config = "Th4o5Tb3")
{
  string inputfile = Form("../../output/FstClusters_%s_withPed_%s.root",hv.c_str(),config.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mPedMean_FST[4][FST::numTBins]; // for RStrip
  TH1F *h_mPedSigma_FST[4][FST::numTBins];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      string HistName;
      HistName = Form("h_mPedMean_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedMean_FST[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());

      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_rstrip,i_tb);
      h_mPedSigma_FST[i_rstrip][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
    }
  }

  string outputname = "./figures/NoiseQA.pdf";

  TCanvas *c_Noise = new TCanvas("c_Noise","c_Noise",10,10,1800,800);
  c_Noise->Divide(9,4);
  for(int i_pad = 0; i_pad < 36; ++i_pad)
  {
    c_Noise->cd(i_pad+1)->SetLeftMargin(0.1);
    // c_Noise->cd(i_pad+1)->SetRightMargin(0.15);
    c_Noise->cd(i_pad+1)->SetBottomMargin(0.1);
    c_Noise->cd(i_pad+1)->SetTicks(1,1);
    c_Noise->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = "./figures/NoiseQA.pdf[";
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

  string output_stop = "./figures/NoiseQA.pdf]";
  c_Noise->Print(output_stop.c_str()); // open pdf file
}

