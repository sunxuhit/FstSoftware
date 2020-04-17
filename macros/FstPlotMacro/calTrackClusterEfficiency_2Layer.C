#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void calTrackClusterEfficiency_2Layer()
{
  const double rMaxFst = FST::rOuter + 4.0*FST::pitchR;
  const double rMinFst = FST::rOuter;
  const double phiMaxFst = 64.0*FST::pitchPhi;
  const double phiMinFst = 0.0;

  // bool isSavePed = true;
  bool isSavePed = false;
  std::string hv = "HV140V";
  std::string inputfile;
  if(isSavePed) inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstTracking_" + hv + "_withPed.root";
  if(!isSavePed) inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstTracking_" + hv + "_woPed.root";

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mTrackClusters_IST[4];
  TH1F *h_mClustersR_IST[4];
  TH1F *h_mClustersPhi_IST[4];

  TH2F *h_mTrackClusters_FST[4];
  TH1F *h_mClustersR_FST[4];
  TH1F *h_mClustersPhi_FST[4];
  for(int i_match = 0; i_match < 4; ++i_match)
  {
    string HistName;
    HistName = Form("h_mTrackClusters_IST_2Layer_SF%d",i_match);
    h_mTrackClusters_IST[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    // h_mTrackClusters_IST[i_match]->GetXaxis()->SetRangeUser(rMinFst,rMaxFst);
    // h_mTrackClusters_IST[i_match]->GetYaxis()->SetRangeUser(phiMinFst,phiMaxFst);
    h_mTrackClusters_IST[i_match]->Sumw2();
    HistName = Form("h_mClustersR_IST_2Layer_SF%d",i_match);
    h_mClustersR_IST[i_match] = (TH1F*)h_mTrackClusters_IST[i_match]->ProjectionX(HistName.c_str());
    HistName = Form("h_mClustersPhi_IST_2Layer_SF%d",i_match);
    h_mClustersPhi_IST[i_match] = (TH1F*)h_mTrackClusters_IST[i_match]->ProjectionY(HistName.c_str());

    HistName = Form("h_mTrackClusters_FST_2Layer_SF%d",i_match);
    h_mTrackClusters_FST[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    // h_mTrackClusters_FST[i_match]->GetXaxis()->SetRangeUser(rMinFst,rMaxFst);
    // h_mTrackClusters_FST[i_match]->GetYaxis()->SetRangeUser(phiMinFst,phiMaxFst);
    h_mTrackClusters_FST[i_match]->Sumw2();
    HistName = Form("h_mClustersR_FST_2Layer_SF%d",i_match);
    h_mClustersR_FST[i_match] = (TH1F*)h_mTrackClusters_FST[i_match]->ProjectionX(HistName.c_str());
    HistName = Form("h_mClustersPhi_FST_2Layer_SF%d",i_match);
    h_mClustersPhi_FST[i_match] = (TH1F*)h_mTrackClusters_FST[i_match]->ProjectionY(HistName.c_str());
  }

  TH2F *h_mEfficiency[4];
  TH1F *h_mEffR[4];
  TH1F *h_mEffPhi[4];
  for(int i_match = 0; i_match < 4; ++i_match)
  {
    string HistName;
    HistName = Form("h_mEfficiency_2Layer_SF%d",i_match);
    h_mEfficiency[i_match] = (TH2F*)h_mTrackClusters_IST[i_match]->Clone(HistName.c_str());
    h_mEfficiency[i_match]->SetTitle(HistName.c_str());
    h_mEfficiency[i_match]->Reset();
    h_mEfficiency[i_match]->Divide(h_mTrackClusters_FST[i_match],h_mTrackClusters_IST[i_match],1,1,"B");

    HistName = Form("h_mEffR_2Layer_SF%d",i_match);
    h_mEffR[i_match] = (TH1F*)h_mClustersR_IST[i_match]->Clone(HistName.c_str());
    h_mEffR[i_match]->SetTitle(HistName.c_str());
    h_mEffR[i_match]->Reset();
    h_mEffR[i_match]->Divide(h_mClustersR_FST[i_match],h_mClustersR_IST[i_match],1,1,"B");

    HistName = Form("h_mEffPhi_2Layer_SF%d",i_match);
    h_mEffPhi[i_match] = (TH1F*)h_mClustersPhi_IST[i_match]->Clone(HistName.c_str());
    h_mEffPhi[i_match]->SetTitle(HistName.c_str());
    h_mEffPhi[i_match]->Reset();
    h_mEffPhi[i_match]->Divide(h_mClustersPhi_FST[i_match],h_mClustersPhi_IST[i_match],1,1,"B");
  }

  string outputname;
  if(isSavePed) outputname = Form("./figures/Efficiency_TrackCluster_2Layer_%s_withPed.pdf",hv.c_str());
  if(!isSavePed) outputname = Form("./figures/Efficiency_TrackCluster_2Layer_%s_woPed.pdf",hv.c_str());
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,900,900);
  c_play->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start;
  if(isSavePed) output_start = Form("./figures/Efficiency_TrackCluster_2Layer_%s_withPed.pdf[",hv.c_str());
  if(!isSavePed) output_start = Form("./figures/Efficiency_TrackCluster_2Layer_%s_woPed.pdf[",hv.c_str());
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_match = 0; i_match < 4; ++i_match)
  {
    // projection position from IST
    c_play->cd(1);
    h_mTrackClusters_IST[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mTrackClusters_IST[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mTrackClusters_IST[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(2);
    h_mClustersR_IST[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mClustersR_IST[i_match]->GetYaxis()->SetRangeUser(0.0,1.1*h_mClustersR_IST[i_match]->GetMaximum());
    h_mClustersR_IST[i_match]->Draw("HIST");
    PlotLine(rMinFst, rMinFst, 0.5, h_mClustersR_IST[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.5, h_mClustersR_IST[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(3);
    h_mClustersPhi_IST[i_match]->GetXaxis()->SetTitle("phi_{proj} (mm)");
    h_mClustersPhi_IST[i_match]->GetXaxis()->SetRangeUser(-0.05,phiMaxFst*1.2);
    h_mClustersPhi_IST[i_match]->Draw("HIST");
    PlotLine(phiMinFst, phiMinFst, 0.5, h_mClustersPhi_IST[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.5, h_mClustersPhi_IST[i_match]->GetMaximum(), 1, 2, 2);

    // measured position from FST
    c_play->cd(4);
    h_mTrackClusters_FST[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mTrackClusters_FST[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mTrackClusters_FST[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(5);
    h_mClustersR_FST[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mClustersR_FST[i_match]->GetYaxis()->SetRangeUser(0.0,1.1*h_mClustersR_FST[i_match]->GetMaximum());
    h_mClustersR_FST[i_match]->Draw("HIST");
    PlotLine(rMinFst, rMinFst, 0.5, h_mClustersR_FST[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.5, h_mClustersR_FST[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(6);
    h_mClustersPhi_FST[i_match]->GetXaxis()->SetTitle("phi_{proj} (rad)");
    h_mClustersPhi_FST[i_match]->GetXaxis()->SetRangeUser(-0.05,phiMaxFst*1.2);
    h_mClustersPhi_FST[i_match]->Draw("HIST");
    PlotLine(phiMinFst, phiMinFst, 0.5, h_mClustersPhi_FST[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.5, h_mClustersPhi_FST[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(7);
    h_mEfficiency[i_match]->SetStats(0);
    h_mEfficiency[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mEfficiency[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mEfficiency[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(8);
    h_mEffR[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mEffR[i_match]->GetYaxis()->SetRangeUser(0.0,1.0);
    h_mEffR[i_match]->Draw("pE");
    PlotLine(rMinFst, rMinFst, 0.0, 1.0, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.0, 1.0, 1, 2, 2);

    c_play->cd(9);
    h_mEffPhi[i_match]->GetXaxis()->SetTitle("phi_{proj} (rad)");
    h_mEffPhi[i_match]->GetXaxis()->SetRangeUser(-0.05,phiMaxFst*1.2);
    h_mEffPhi[i_match]->GetYaxis()->SetRangeUser(0.0,1.0);
    h_mEffPhi[i_match]->Draw("pE");
    PlotLine(phiMinFst, phiMinFst, 0.0, 1.0, 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.0, 1.0, 1, 2, 2);

    c_play->Update();
    c_play->Print(outputname.c_str());
  }

  string output_stop;
  if(isSavePed) output_stop = Form("./figures/Efficiency_TrackCluster_2Layer_%s_withPed.pdf]",hv.c_str());
  if(!isSavePed) output_stop = Form("./figures/Efficiency_TrackCluster_2Layer_%s_woPed.pdf]",hv.c_str());
  c_play->Print(output_stop.c_str()); // open pdf file
}
