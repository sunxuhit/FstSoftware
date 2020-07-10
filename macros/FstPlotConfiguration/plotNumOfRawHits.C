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

void plotNumOfRawHits(string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstQAStudy_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mNumFstRawHitsDisplay = (TH1F*)File_InPut->Get("h_mNumFstRawHitsDisplay");
  TH1F *h_mNumFstSimpleClustersDisplay = (TH1F*)File_InPut->Get("h_mNumFstSimpleClustersDisplay");
  TH1F *h_mNumFstScanClustersDisplay = (TH1F*)File_InPut->Get("h_mNumFstScanClustersDisplay");

  TCanvas *c_EventDisplay = new TCanvas("c_EventDisplay","c_EventDisplay",10,10,800,400);
  c_EventDisplay->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_EventDisplay->cd(i_pad+1)->SetLeftMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetRightMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetBottomMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetTicks(1,1);
    c_EventDisplay->cd(i_pad+1)->SetGrid(0,0);
    c_EventDisplay->cd(i_pad+1)->SetLogy();
  }

  c_EventDisplay->cd(1);
  h_mNumFstRawHitsDisplay->SetTitle("Number of Raw Hits");
  // h_mNumFstRawHitsDisplay->SetStats(0);
  h_mNumFstRawHitsDisplay->GetXaxis()->SetTitle("# Raw Hits");
  h_mNumFstRawHitsDisplay->GetXaxis()->SetTitleSize(0.06);
  h_mNumFstRawHitsDisplay->GetXaxis()->SetRangeUser(-0.5,44.5);
  h_mNumFstRawHitsDisplay->Draw();

  c_EventDisplay->cd(2);
  h_mNumFstSimpleClustersDisplay->SetTitle("Number of Clusters");
  // h_mNumFstSimpleClustersDisplay->SetStats(0);
  h_mNumFstSimpleClustersDisplay->GetXaxis()->SetTitle("# Clusters");
  h_mNumFstSimpleClustersDisplay->GetXaxis()->SetTitleSize(0.06);
  h_mNumFstSimpleClustersDisplay->GetXaxis()->SetRangeUser(-0.5,44.5);
  h_mNumFstSimpleClustersDisplay->SetLineColor(1);
  h_mNumFstSimpleClustersDisplay->Draw();
  h_mNumFstScanClustersDisplay->SetLineColor(2);
  h_mNumFstScanClustersDisplay->Draw("same");

  TLegend *leg = new TLegend(0.4,0.6,0.85,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(h_mNumFstSimpleClustersDisplay,"Simple Cluster","L");
  leg->AddEntry(h_mNumFstScanClustersDisplay,"Scan Cluster","L");
  leg->Draw("same");

  // c_EventDisplay->SaveAs("./figures/NumOfRawHits.eps");
  // c_EventDisplay->SaveAs("./figures/NumOfRawHits.png");
  c_EventDisplay->SaveAs("./figures/NumOfRawHits.pdf");
}

