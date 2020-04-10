#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile2D.h>
#include <TStyle.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotEventDisplay_Tree()
{
  gStyle->SetPalette(kRainBow);

  const double rMax = FST::rOuter + 5.0*FST::pitchR;
  const double rMin = FST::rOuter - 1.0*FST::pitchR;
  const double phiMax = 128.0*FST::pitchPhi;
  const double phiMin = -128.0*FST::pitchPhi;

  int mNumOfFstRawHits;
  int mNumOfFstClusters;
  int mNumOfHitTracks;
  int mNumOfClusterTracks;
  TH2F *h_mFstRawHitsDisplay = new TH2F("h_mFstRawHitsDisplay","h_mFstRawHitsDisplay",6,rMin,rMax,FST::numPhiSeg*2,phiMin,phiMax);
  TH2F *h_mFstClustersDisplay = new TH2F("h_mFstClustersDisplay","h_mFstClustersDisplay",60,rMin,rMax,FST::numPhiSeg*4,phiMin,phiMax);
  TH2F *h_mHitTracksDisplay = new TH2F("h_mHitTracksDisplay","h_mHitTracksDisplay",60,rMin,rMax,FST::numPhiSeg*4,phiMin,phiMax);
  TH2F *h_mClusterTracksDisplay = new TH2F("h_mClusterTracksDisplay","h_mClusterTracksDisplay",60,rMin,rMax,FST::numPhiSeg*4,phiMin,phiMax);

  // string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_woPed.root";
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_withPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_EventDisplay = (TTree*)File_InPut->Get("mTree_EventDisplay");
  mTree_EventDisplay->SetBranchAddress("mNumOfFstRawHits",&mNumOfFstRawHits);
  mTree_EventDisplay->SetBranchAddress("h_mFstRawHitsDisplay",&h_mFstRawHitsDisplay);
  mTree_EventDisplay->SetBranchAddress("mNumOfFstClusters",&mNumOfFstClusters);
  mTree_EventDisplay->SetBranchAddress("h_mFstClustersDisplay",&h_mFstClustersDisplay);
  mTree_EventDisplay->SetBranchAddress("mNumOfHitTracks",&mNumOfHitTracks);
  mTree_EventDisplay->SetBranchAddress("h_mHitTracksDisplay",&h_mHitTracksDisplay);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks",&mNumOfClusterTracks);
  mTree_EventDisplay->SetBranchAddress("h_mClusterTracksDisplay",&h_mClusterTracksDisplay);

  long NumOfEvents = (long)mTree_EventDisplay->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 500;
  mTree_EventDisplay->GetEntry(0);

  const double rMaxFst = FST::rOuter + 4.0*FST::pitchR;
  const double rMinFst = FST::rOuter;
  const double phiMaxFst = 64.0*FST::pitchPhi;
  const double phiMinFst = 0.0;

  string outputname = "./figures/EventDisplay_QA.pdf";
  TCanvas *c_EventDisplay = new TCanvas("c_EventDisplay","c_EventDisplay",10,10,900,900);
  c_EventDisplay->cd()->SetLeftMargin(0.15);
  c_EventDisplay->cd()->SetRightMargin(0.15);
  c_EventDisplay->cd()->SetBottomMargin(0.15);
  c_EventDisplay->cd()->SetTicks(1,1);
  c_EventDisplay->cd()->SetGrid(0,0);
  // TLegend *leg = new TLegend(0.4,0.6,0.85,0.8);
  // leg->SetBorderSize(0);
  // leg->SetFillColor(10);

  int numOfUsedEvent = 0;
  string output_start = "./figures/EventDisplay_QA.pdf[";
  c_EventDisplay->Print(output_start.c_str()); // open pdf file

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  // for(int i_event = 0; i_event < 100; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_EventDisplay->GetEntry(i_event);

    // cout << "mNumOfFstRawHits = " << mNumOfFstRawHits << ", h_mFstRawHitsDisplay->GetEntries() = " << h_mFstRawHitsDisplay->GetEntries() << endl;
    // cout << "mNumOfFstClusters = " << mNumOfFstClusters << ", h_mFstClustersDisplay->GetEntries() = " << h_mFstClustersDisplay->GetEntries() << endl;
    // cout << "mNumOfHitTracks = " << mNumOfHitTracks << ", h_mHitTracksDisplay->GetEntries() = " << h_mHitTracksDisplay->GetEntries() << endl;
    // cout << "mNumOfClusterTracks = " << mNumOfClusterTracks << ", h_mClusterTracksDisplay->GetEntries() = " << h_mClusterTracksDisplay->GetEntries() << endl;

    // if(mNumOfFstClusters > 1 && mNumOfHitTracks > 0)
    // if(mNumOfFstRawHits > 0 && mNumOfHitTracks > 0)
    // if(mNumOfFstRawHits > 0 && mNumOfHitTracks == 1)
    // if(mNumOfFstRawHits > 0)
    if(mNumOfHitTracks == 1)
    {
      h_mFstRawHitsDisplay->SetStats(0);
      h_mFstRawHitsDisplay->GetXaxis()->SetTitle("R");
      h_mFstRawHitsDisplay->GetXaxis()->SetTitleSize(0.06);
      h_mFstRawHitsDisplay->GetXaxis()->SetRangeUser(rMin,rMax);
      h_mFstRawHitsDisplay->GetYaxis()->SetTitle("#phi");
      // h_mFstRawHitsDisplay->GetYaxis()->SetRangeUser(phiMin/2.0,phiMax/2.0);
      h_mFstRawHitsDisplay->GetYaxis()->SetRangeUser(phiMinFst-10.0*FST::pitchPhi,phiMax/2.0+10.0*FST::pitchPhi);
      h_mFstRawHitsDisplay->GetYaxis()->SetTitleSize(0.06);
      h_mFstRawHitsDisplay->GetZaxis()->SetRangeUser(1.0,2000.0);
      h_mFstRawHitsDisplay->Draw("colz");
      h_mHitTracksDisplay->SetMarkerStyle(5);
      h_mHitTracksDisplay->SetMarkerColor(1);
      h_mHitTracksDisplay->SetMarkerSize(1.0);
      h_mHitTracksDisplay->Draw("p Same");

      h_mFstClustersDisplay->SetMarkerStyle(25);
      h_mFstClustersDisplay->SetMarkerColor(2);
      h_mFstClustersDisplay->SetMarkerSize(0.8);
      h_mFstClustersDisplay->Draw("p Same");
      // h_mClusterTracksDisplay->SetMarkerStyle(36);
      // h_mClusterTracksDisplay->SetMarkerColor(2);
      // h_mClusterTracksDisplay->SetMarkerSize(1.0);
      // h_mClusterTracksDisplay->Draw("p Same");
      PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
      PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);
      // leg->Clear();
      // leg->AddEntry(h_mFstRawHitsDisplay,"Hit","F");
      // leg->AddEntry(h_mFstClustersDisplay,"Cluster","P");
      // leg->AddEntry(h_mHitTracksDisplay,"Projected Track from Hits","P");
      // leg->Draw("same");

      c_EventDisplay->Update();
      c_EventDisplay->Print(outputname.c_str()); // print integrated efficiency
      numOfUsedEvent++;
    }
  }

  string output_stop =  "./figures/EventDisplay_QA.pdf]";
  c_EventDisplay->Print(output_stop.c_str()); // close pdf file

  cout << "numOfUsedEvent = " << numOfUsedEvent << endl;
}

