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

void plotNumOfRawHits(string hv = "HV140V", string config = "Th4o5Tb3")
{
  string inputfile = Form("../../output/configuration/FstQAStudy_%s_withPed_%s.root",hv.c_str(),config.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mNumFstRawHitsDisplay = (TH1F*)File_InPut->Get("h_mNumFstRawHitsDisplay");
  TH1F *h_mNumFstClustersDisplay = (TH1F*)File_InPut->Get("h_mNumFstClustersDisplay");

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
  h_mNumFstClustersDisplay->SetTitle("Number of Clusters");
  // h_mNumFstClustersDisplay->SetStats(0);
  h_mNumFstClustersDisplay->GetXaxis()->SetTitle("# Clusters");
  h_mNumFstClustersDisplay->GetXaxis()->SetTitleSize(0.06);
  h_mNumFstClustersDisplay->GetXaxis()->SetRangeUser(-0.5,44.5);
  h_mNumFstClustersDisplay->Draw();

  // c_EventDisplay->SaveAs("./figures/NumOfRawHits.eps");
  // c_EventDisplay->SaveAs("./figures/NumOfRawHits.png");
  c_EventDisplay->SaveAs("./figures/NumOfRawHits.pdf");
}

