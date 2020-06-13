#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void calTrackClusterEfficiency_2Layer(string hv = "HV200V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.5, int numOfUsedTimeBins = 2, float nFstThresholdCut = 3.5)
{
  const double rMaxFst = FST::rOuter + 4.0*FST::pitchR;
  const double rMinFst = FST::rOuter;
  const double phiMaxFst = 64.0*FST::pitchPhi;
  const double phiMinFst = 0.0;

  const int nMatch = 8;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstTracking_%s_Th%1.1fTb%dPed%1.1f_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut,pedMode.c_str(),cmnMode.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());

  // simple cluster
  TH2F *h_mSimpleClustersTrackIstCounts_2Layer[nMatch];
  TH1F *h_mSimpleClustersTrackIstCountsR_2Layer[nMatch];
  TH1F *h_mSimpleClustersTrackIstCountsPhi_2Layer[nMatch];

  TH2F *h_mSimpleClustersTrackFstCounts_2Layer[nMatch];
  TH1F *h_mSimpleClustersTrackFstCountsR_2Layer[nMatch];
  TH1F *h_mSimpleClustersTrackFstCountsPhi_2Layer[nMatch];
  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackIstCounts_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackIstCounts_2Layer[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    // h_mSimpleClustersTrackIstCounts_2Layer[i_match]->GetXaxis()->SetRangeUser(rMinFst,rMaxFst);
    // h_mSimpleClustersTrackIstCounts_2Layer[i_match]->GetYaxis()->SetRangeUser(phiMinFst,phiMaxFst);
    h_mSimpleClustersTrackIstCounts_2Layer[i_match]->Sumw2();
    HistName = Form("h_mSimpleClustersTrackIstCountsR_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackIstCountsR_2Layer[i_match] = (TH1F*)h_mSimpleClustersTrackIstCounts_2Layer[i_match]->ProjectionX(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackIstCountsPhi_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match] = (TH1F*)h_mSimpleClustersTrackIstCounts_2Layer[i_match]->ProjectionY(HistName.c_str());

    HistName = Form("h_mSimpleClustersTrackFstCounts_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstCounts_2Layer[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    // h_mSimpleClustersTrackFstCounts_2Layer[i_match]->GetXaxis()->SetRangeUser(rMinFst,rMaxFst);
    // h_mSimpleClustersTrackFstCounts_2Layer[i_match]->GetYaxis()->SetRangeUser(phiMinFst,phiMaxFst);
    h_mSimpleClustersTrackFstCounts_2Layer[i_match]->Sumw2();
    HistName = Form("h_mSimpleClustersTrackFstCountsR_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstCountsR_2Layer[i_match] = (TH1F*)h_mSimpleClustersTrackFstCounts_2Layer[i_match]->ProjectionX(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstCountsPhi_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match] = (TH1F*)h_mSimpleClustersTrackFstCounts_2Layer[i_match]->ProjectionY(HistName.c_str());
  }

  TH2F *h_mSimpleClustersTrackFstEff_2Layer[nMatch];
  TH1F *h_mSimpleClustersTrackFstEffR_2Layer[nMatch];
  TH1F *h_mSimpleClustersTrackFstEffPhi_2Layer[nMatch];
  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackFstEff_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEff_2Layer[i_match] = (TH2F*)h_mSimpleClustersTrackIstCounts_2Layer[i_match]->Clone(HistName.c_str());
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->Reset();
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->Divide(h_mSimpleClustersTrackFstCounts_2Layer[i_match],h_mSimpleClustersTrackIstCounts_2Layer[i_match],1,1,"B");

    HistName = Form("h_mSimpleClustersTrackFstEffR_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffR_2Layer[i_match] = (TH1F*)h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->Clone(HistName.c_str());
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->Reset();
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->Divide(h_mSimpleClustersTrackFstCountsR_2Layer[i_match],h_mSimpleClustersTrackIstCountsR_2Layer[i_match],1,1,"B");

    HistName = Form("h_mSimpleClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match] = (TH1F*)h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match]->Clone(HistName.c_str());
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->Reset();
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->Divide(h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match],h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match],1,1,"B");
  }

  // scan cluster
  TH2F *h_mScanClustersTrackIstCounts_2Layer[nMatch];
  TH1F *h_mScanClustersTrackIstCountsR_2Layer[nMatch];
  TH1F *h_mScanClustersTrackIstCountsPhi_2Layer[nMatch];

  TH2F *h_mScanClustersTrackFstCounts_2Layer[nMatch];
  TH1F *h_mScanClustersTrackFstCountsR_2Layer[nMatch];
  TH1F *h_mScanClustersTrackFstCountsPhi_2Layer[nMatch];
  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    string HistName;
    HistName = Form("h_mScanClustersTrackIstCounts_2Layer_SF%d",i_match);
    h_mScanClustersTrackIstCounts_2Layer[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    // h_mScanClustersTrackIstCounts_2Layer[i_match]->GetXaxis()->SetRangeUser(rMinFst,rMaxFst);
    // h_mScanClustersTrackIstCounts_2Layer[i_match]->GetYaxis()->SetRangeUser(phiMinFst,phiMaxFst);
    h_mScanClustersTrackIstCounts_2Layer[i_match]->Sumw2();
    HistName = Form("h_mScanClustersTrackIstCountsR_2Layer_SF%d",i_match);
    h_mScanClustersTrackIstCountsR_2Layer[i_match] = (TH1F*)h_mScanClustersTrackIstCounts_2Layer[i_match]->ProjectionX(HistName.c_str());
    HistName = Form("h_mScanClustersTrackIstCountsPhi_2Layer_SF%d",i_match);
    h_mScanClustersTrackIstCountsPhi_2Layer[i_match] = (TH1F*)h_mScanClustersTrackIstCounts_2Layer[i_match]->ProjectionY(HistName.c_str());

    HistName = Form("h_mScanClustersTrackFstCounts_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstCounts_2Layer[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    // h_mScanClustersTrackFstCounts_2Layer[i_match]->GetXaxis()->SetRangeUser(rMinFst,rMaxFst);
    // h_mScanClustersTrackFstCounts_2Layer[i_match]->GetYaxis()->SetRangeUser(phiMinFst,phiMaxFst);
    h_mScanClustersTrackFstCounts_2Layer[i_match]->Sumw2();
    HistName = Form("h_mScanClustersTrackFstCountsR_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstCountsR_2Layer[i_match] = (TH1F*)h_mScanClustersTrackFstCounts_2Layer[i_match]->ProjectionX(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstCountsPhi_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstCountsPhi_2Layer[i_match] = (TH1F*)h_mScanClustersTrackFstCounts_2Layer[i_match]->ProjectionY(HistName.c_str());
  }

  TH2F *h_mScanClustersTrackFstEff_2Layer[nMatch];
  TH1F *h_mScanClustersTrackFstEffR_2Layer[nMatch];
  TH1F *h_mScanClustersTrackFstEffPhi_2Layer[nMatch];
  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    string HistName;
    HistName = Form("h_mScanClustersTrackFstEff_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEff_2Layer[i_match] = (TH2F*)h_mScanClustersTrackIstCounts_2Layer[i_match]->Clone(HistName.c_str());
    h_mScanClustersTrackFstEff_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mScanClustersTrackFstEff_2Layer[i_match]->Reset();
    h_mScanClustersTrackFstEff_2Layer[i_match]->Divide(h_mScanClustersTrackFstCounts_2Layer[i_match],h_mScanClustersTrackIstCounts_2Layer[i_match],1,1,"B");

    HistName = Form("h_mScanClustersTrackFstEffR_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffR_2Layer[i_match] = (TH1F*)h_mScanClustersTrackIstCountsR_2Layer[i_match]->Clone(HistName.c_str());
    h_mScanClustersTrackFstEffR_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mScanClustersTrackFstEffR_2Layer[i_match]->Reset();
    h_mScanClustersTrackFstEffR_2Layer[i_match]->Divide(h_mScanClustersTrackFstCountsR_2Layer[i_match],h_mScanClustersTrackIstCountsR_2Layer[i_match],1,1,"B");

    HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffPhi_2Layer[i_match] = (TH1F*)h_mScanClustersTrackIstCountsPhi_2Layer[i_match]->Clone(HistName.c_str());
    h_mScanClustersTrackFstEffPhi_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mScanClustersTrackFstEffPhi_2Layer[i_match]->Reset();
    h_mScanClustersTrackFstEffPhi_2Layer[i_match]->Divide(h_mScanClustersTrackFstCountsPhi_2Layer[i_match],h_mScanClustersTrackIstCountsPhi_2Layer[i_match],1,1,"B");
  }

  string outputname = "./figures/Efficiency_TrackCluster_2Layer.pdf";
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,900,900);
  c_play->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = "./figures/Efficiency_TrackCluster_2Layer.pdf[";
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    // projection position from IST
    c_play->cd(1);
    h_mSimpleClustersTrackIstCounts_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mSimpleClustersTrackIstCounts_2Layer[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mSimpleClustersTrackIstCounts_2Layer[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(2);
    h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.1*h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->GetMaximum());
    h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->Draw("HIST");
    PlotLine(rMinFst, rMinFst, 0.5, h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.5, h_mSimpleClustersTrackIstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(3);
    h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match]->GetXaxis()->SetTitle("phi_{proj} (mm)");
    h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match]->GetXaxis()->SetRangeUser(-0.05,phiMaxFst*1.2);
    h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match]->Draw("HIST");
    PlotLine(phiMinFst, phiMinFst, 0.5, h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.5, h_mSimpleClustersTrackIstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    // measured position from FST
    c_play->cd(4);
    h_mSimpleClustersTrackFstCounts_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mSimpleClustersTrackFstCounts_2Layer[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mSimpleClustersTrackFstCounts_2Layer[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(5);
    h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.1*h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->GetMaximum());
    h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->SetLineColor(1);
    h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->Draw("HIST");
    h_mScanClustersTrackFstCountsR_2Layer[i_match]->SetLineColor(2);
    h_mScanClustersTrackFstCountsR_2Layer[i_match]->Draw("HIST same");
    PlotLine(rMinFst, rMinFst, 0.5, h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.5, h_mSimpleClustersTrackFstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(6);
    h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match]->GetXaxis()->SetTitle("phi_{proj} (rad)");
    h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match]->GetXaxis()->SetRangeUser(-0.05,phiMaxFst*1.2);
    h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match]->SetLineColor(1);
    h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match]->Draw("HIST");
    h_mScanClustersTrackFstCountsPhi_2Layer[i_match]->SetLineColor(2);
    h_mScanClustersTrackFstCountsPhi_2Layer[i_match]->Draw("HIST same");
    PlotLine(phiMinFst, phiMinFst, 0.5, h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.5, h_mSimpleClustersTrackFstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(7);
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->SetStats(0);
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mSimpleClustersTrackFstEff_2Layer[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(8);
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->SetStats(0);
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.05);
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->SetLineColor(1);
    h_mSimpleClustersTrackFstEffR_2Layer[i_match]->Draw("pE");
    h_mScanClustersTrackFstEffR_2Layer[i_match]->SetLineColor(2);
    h_mScanClustersTrackFstEffR_2Layer[i_match]->Draw("pE same");
    PlotLine(rMinFst, rMinFst, 0.0, 1.05, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.0, 1.05, 1, 2, 2);
    PlotLine(rMinFst-FST::pitchR, rMaxFst+FST::pitchR, 0.90, 0.90, 4, 1, 2);
    PlotLine(rMinFst-FST::pitchR, rMaxFst+FST::pitchR, 0.95, 0.95, 4, 1, 2);

    c_play->cd(9);
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->SetStats(0);
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->GetXaxis()->SetTitle("phi_{proj} (rad)");
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->GetXaxis()->SetRangeUser(-0.05,phiMaxFst*1.2);
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.05);
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->SetLineColor(1);
    h_mSimpleClustersTrackFstEffPhi_2Layer[i_match]->Draw("pE");
    h_mScanClustersTrackFstEffPhi_2Layer[i_match]->SetLineColor(2);
    h_mScanClustersTrackFstEffPhi_2Layer[i_match]->Draw("pE same");
    PlotLine(phiMinFst, phiMinFst, 0.0, 1.05, 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.0, 1.05, 1, 2, 2);
    if(i_match == 0)
    {
      TLegend *leg = new TLegend(0.4,0.6,0.85,0.8);
      leg->SetBorderSize(0);
      leg->SetFillColor(10);
      leg->AddEntry(h_mSimpleClustersTrackFstEffPhi_2Layer[i_match],"Simple Cluster","L");
      leg->AddEntry(h_mScanClustersTrackFstEffPhi_2Layer[i_match],"Scan Cluster","L");
      leg->Draw("same");
    }

    c_play->Update();
    c_play->Print(outputname.c_str());
  }

  string output_stop = "./figures/Efficiency_TrackCluster_2Layer.pdf]";
  c_play->Print(output_stop.c_str()); // open pdf file
}
