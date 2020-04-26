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

void plotEventDisplay_3Layer(string hv = "HV140V", string config = "Th4o5Tb3")
{
  // gStyle->SetPalette(kRainBow);
  gStyle->SetPalette(kBlackBody);
  gStyle->SetPaintTextFormat("1.0f");

  int mEventId;

  int mNumOfFstRawHits;
  int mNumOfIst1RawHits;
  int mNumOfIst2RawHits;
  int mNumOfIst3RawHits;

  int mNumOfFstClusters;
  int mNumOfIst1Clusters;
  int mNumOfIst2Clusters;
  int mNumOfIst3Clusters;

  int mNumOfHitTracks;
  int mNumOfClusterTracks;
  int mNumOfClusterTracks_2Layer;
  int mNumOfClusterTracks_3Layer;

  TH2F *h_mFstRawHitsDisplay      = new TH2F("h_mFstRawHitsDisplay","h_mFstRawHitsDisplay",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstRawPedsDisplay      = new TH2F("h_mFstRawPedsDisplay","h_mFstRawPedsDisplay",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstMaxTbDisplay        = new TH2F("h_mFstMaxTbDisplay","h_mFstMaxTbDisplay",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstRawHitsDisplay_bTh  = new TH2F("h_mFstRawHitsDisplay_bTh","h_mFstRawHitsDisplay_bTh",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstRawPedsDisplay_bTh  = new TH2F("h_mFstRawPedsDisplay_bTh","h_mFstRawPedsDisplay_bTh",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstMaxTbDisplay_bTh    = new TH2F("h_mFstMaxTbDisplay_bTh","h_mFstMaxTbDisplay_bTh",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);

  TH2F *h_mFstClustersDisplay     = new TH2F("h_mFstClustersDisplay","h_mFstClustersDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mHitTracksDisplay       = new TH2F("h_mHitTracksDisplay","h_mHitTracksDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mClusterTracksDisplay   = new TH2F("h_mClusterTracksDisplay","h_mClusterTracksDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TGraph *g_mFstClustersDisplay   = new TGraph();
  TGraph *g_mHitTracksDisplay     = new TGraph();
  TGraph *g_mClusterTracksDisplay = new TGraph();


  std::string inputfile = Form("../../output/configuration/FstQAStudy_%s_withPed_%s.root",hv.c_str(),config.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_EventDisplay = (TTree*)File_InPut->Get("mTree_EventDisplay");
  mTree_EventDisplay->SetBranchAddress("mEventId",&mEventId);
  mTree_EventDisplay->SetBranchAddress("mNumOfFstRawHits",&mNumOfFstRawHits);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst1RawHits",&mNumOfIst1RawHits);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst2RawHits",&mNumOfIst2RawHits);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst3RawHits",&mNumOfIst3RawHits);
  mTree_EventDisplay->SetBranchAddress("h_mFstRawHitsDisplay",&h_mFstRawHitsDisplay);
  mTree_EventDisplay->SetBranchAddress("h_mFstRawPedsDisplay",&h_mFstRawPedsDisplay);
  mTree_EventDisplay->SetBranchAddress("h_mFstMaxTbDisplay",&h_mFstMaxTbDisplay);
  mTree_EventDisplay->SetBranchAddress("h_mFstRawHitsDisplay_bTh",&h_mFstRawHitsDisplay_bTh);
  mTree_EventDisplay->SetBranchAddress("h_mFstRawPedsDisplay_bTh",&h_mFstRawPedsDisplay_bTh);
  mTree_EventDisplay->SetBranchAddress("h_mFstMaxTbDisplay_bTh",&h_mFstMaxTbDisplay_bTh);

  mTree_EventDisplay->SetBranchAddress("mNumOfFstClusters",&mNumOfFstClusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst1Clusters",&mNumOfIst1Clusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst2Clusters",&mNumOfIst2Clusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst3Clusters",&mNumOfIst3Clusters);
  mTree_EventDisplay->SetBranchAddress("h_mFstClustersDisplay",&h_mFstClustersDisplay);

  mTree_EventDisplay->SetBranchAddress("mNumOfHitTracks",&mNumOfHitTracks);
  mTree_EventDisplay->SetBranchAddress("h_mHitTracksDisplay",&h_mHitTracksDisplay);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks",&mNumOfClusterTracks);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks_2Layer",&mNumOfClusterTracks_2Layer);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks_3Layer",&mNumOfClusterTracks_3Layer);
  mTree_EventDisplay->SetBranchAddress("h_mClusterTracksDisplay",&h_mClusterTracksDisplay);

  mTree_EventDisplay->SetBranchAddress("g_mFstClustersDisplay",&g_mFstClustersDisplay);
  mTree_EventDisplay->SetBranchAddress("g_mHitTracksDisplay",&g_mHitTracksDisplay);
  mTree_EventDisplay->SetBranchAddress("g_mClusterTracksDisplay",&g_mClusterTracksDisplay);

  long NumOfEvents = (long)mTree_EventDisplay->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 1000;
  mTree_EventDisplay->GetEntry(0);

  const double rMaxFst = FST::rOuter + 4.0*FST::pitchR;
  const double rMinFst = FST::rOuter;
  const double phiMaxFst = 64.0*FST::pitchPhi;
  const double phiMinFst = 0.0;

  string outputname = "./figures/EventDisplay_3Layer.pdf";
  TCanvas *c_EventDisplay = new TCanvas("c_EventDisplay","c_EventDisplay",10,10,800,400);
  c_EventDisplay->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_EventDisplay->cd(i_pad+1)->SetLeftMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetRightMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetBottomMargin(0.15);
    c_EventDisplay->cd(i_pad+1)->SetTicks(1,1);
    c_EventDisplay->cd(i_pad+1)->SetGrid(0,0);
    c_EventDisplay->cd(i_pad+1)->SetLogz();
  }
  // TLegend *leg = new TLegend(0.4,0.6,0.85,0.8);
  // leg->SetBorderSize(0);
  // leg->SetFillColor(10);

  int numOfUsedEvent = 0;
  string output_start = "./figures/EventDisplay_3Layer.pdf[";
  c_EventDisplay->Print(output_start.c_str()); // open pdf file

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_EventDisplay->GetEntry(i_event);

    // cout << "mNumOfFstRawHits = " << mNumOfFstRawHits << ", h_mFstRawHitsDisplay->GetEntries() = " << h_mFstRawHitsDisplay->GetEntries() << endl;
    // cout << "mNumOfFstClusters = " << mNumOfFstClusters << ", h_mFstClustersDisplay->GetEntries() = " << h_mFstClustersDisplay->GetEntries() << endl;
    // cout << "mNumOfHitTracks = " << mNumOfHitTracks << ", h_mHitTracksDisplay->GetEntries() = " << h_mHitTracksDisplay->GetEntries() << endl;
    // cout << "mNumOfClusterTracks = " << mNumOfClusterTracks << ", h_mClusterTracksDisplay->GetEntries() = " << h_mClusterTracksDisplay->GetEntries() << endl;

    // if(mNumOfClusterTracks == 1 && mNumOfClusterTracks_2Layer == 1) // 2-Layer Tracking
    if(mNumOfClusterTracks == 1 && mNumOfClusterTracks_2Layer == 1 && mNumOfClusterTracks_3Layer == 1) // 3-Layer Tracking
    {
      c_EventDisplay->cd(1);
      string Title = Form("Event %d (Threshold 3.0)", mEventId);
      h_mFstRawHitsDisplay->SetTitle(Title.c_str());
      h_mFstRawHitsDisplay->SetStats(0);
      h_mFstRawHitsDisplay->GetXaxis()->SetTitle("R");
      h_mFstRawHitsDisplay->GetXaxis()->SetTitleSize(0.06);
      h_mFstRawHitsDisplay->GetXaxis()->SetRangeUser(FST::rMin,FST::rMax);
      h_mFstRawHitsDisplay->GetYaxis()->SetTitle("#phi");
      h_mFstRawHitsDisplay->GetYaxis()->SetRangeUser(phiMinFst-10.0*FST::pitchPhi,FST::phiMax+10.0*FST::pitchPhi);
      h_mFstRawHitsDisplay->GetYaxis()->SetTitleSize(0.06);
      h_mFstRawHitsDisplay->GetZaxis()->SetRangeUser(1.0,2000.0);
      h_mFstRawHitsDisplay->Draw("colz");
      h_mFstRawHitsDisplay->SetBarOffset(1.5);
      h_mFstRawHitsDisplay->SetMarkerColor(1);
      h_mFstRawHitsDisplay->Draw("TEXT Same");

      h_mFstRawPedsDisplay->SetMarkerColor(2);
      h_mFstRawPedsDisplay->SetBarOffset(-1.5);
      h_mFstRawPedsDisplay->Draw("TEXT Same");

      h_mFstMaxTbDisplay->SetMarkerColor(4);
      h_mFstMaxTbDisplay->SetBarOffset(-3.5);
      h_mFstMaxTbDisplay->Draw("TEXT Same");

      // plot track
      if(g_mFstClustersDisplay->GetN() > 0)
      {
	g_mFstClustersDisplay->SetMarkerStyle(25);
	g_mFstClustersDisplay->SetMarkerColor(1);
	g_mFstClustersDisplay->SetMarkerSize(1.0);
	g_mFstClustersDisplay->Draw("p Same");
      }
      g_mClusterTracksDisplay->SetMarkerStyle(5);
      g_mClusterTracksDisplay->SetMarkerColor(1);
      g_mClusterTracksDisplay->SetMarkerSize(1.0);
      g_mClusterTracksDisplay->Draw("p Same");

      // plot FST acceptance
      PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
      PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

      c_EventDisplay->cd(2);
      Title = Form("Event %d (Threshold: 2.0)", mEventId);
      h_mFstRawHitsDisplay_bTh->SetTitle(Title.c_str());
      h_mFstRawHitsDisplay_bTh->SetStats(0);
      h_mFstRawHitsDisplay_bTh->GetXaxis()->SetTitle("R");
      h_mFstRawHitsDisplay_bTh->GetXaxis()->SetTitleSize(0.06);
      h_mFstRawHitsDisplay_bTh->GetXaxis()->SetRangeUser(FST::rMin,FST::rMax);
      h_mFstRawHitsDisplay_bTh->GetYaxis()->SetTitle("#phi");
      h_mFstRawHitsDisplay_bTh->GetYaxis()->SetRangeUser(phiMinFst-10.0*FST::pitchPhi,FST::phiMax+10.0*FST::pitchPhi);
      h_mFstRawHitsDisplay_bTh->GetYaxis()->SetTitleSize(0.06);
      h_mFstRawHitsDisplay_bTh->GetZaxis()->SetRangeUser(1.0,2000.0);
      h_mFstRawHitsDisplay_bTh->Draw("colz");
      h_mFstRawHitsDisplay_bTh->SetBarOffset(1.5);
      h_mFstRawHitsDisplay_bTh->SetMarkerColor(1);
      h_mFstRawHitsDisplay_bTh->Draw("TEXT Same");

      h_mFstRawPedsDisplay_bTh->SetMarkerColor(2);
      h_mFstRawPedsDisplay_bTh->SetBarOffset(-1.5);
      h_mFstRawPedsDisplay_bTh->Draw("TEXT Same");

      h_mFstMaxTbDisplay_bTh->SetMarkerColor(4);
      h_mFstMaxTbDisplay_bTh->SetBarOffset(-3.5);
      h_mFstMaxTbDisplay_bTh->Draw("TEXT Same");

      // plot track
      if(g_mFstClustersDisplay->GetN() > 0)
      {
	g_mFstClustersDisplay->SetMarkerStyle(25);
	g_mFstClustersDisplay->SetMarkerColor(1);
	g_mFstClustersDisplay->SetMarkerSize(1.0);
	g_mFstClustersDisplay->Draw("p Same");
      }
      g_mClusterTracksDisplay->SetMarkerStyle(5);
      g_mClusterTracksDisplay->SetMarkerColor(1);
      g_mClusterTracksDisplay->SetMarkerSize(1.0);
      g_mClusterTracksDisplay->Draw("p Same");

      // plot FST acceptance
      PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
      PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

      c_EventDisplay->Update();
      c_EventDisplay->Print(outputname.c_str()); // print integrated efficiency

      numOfUsedEvent++;
    }
  }

  cout << "numOfUsedEvent = " << numOfUsedEvent << endl;

  string output_stop =  "./figures/EventDisplay_3Layer.pdf]";
  c_EventDisplay->Print(output_stop.c_str()); // close pdf file
}

