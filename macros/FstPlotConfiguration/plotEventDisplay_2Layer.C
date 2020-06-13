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

void plotBox(TGraph *g_track);

void plotEventDisplay_2Layer(string hv = "HV200V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.5, int numOfUsedTimeBins = 2, float nFstThresholdCut = 3.5)
{
  // gStyle->SetPalette(kRainBow);
  gStyle->SetPalette(kBlackBody);
  gStyle->SetPaintTextFormat("1.0f");

  int mEventId;

  int mNumOfFstRawHits;
  int mNumOfIst1RawHits;
  int mNumOfIst2RawHits;
  int mNumOfIst3RawHits;

  int mNumOfFstSimpleClusters;
  int mNumOfFstScanClusters;
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

  TH2F *h_mFstSimpleClustersDisplay     = new TH2F("h_mFstSimpleClustersDisplay","h_mFstSimpleClustersDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstScanClustersDisplay = new TH2F("h_mFstScanClustersDisplay","h_mFstScanClustersDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstScanClustersDisplay_RawHits = new TH2F("h_mFstScanClustersDisplay_RawHits","h_mFstScanClustersDisplay_RawHits",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstScanClustersDisplay_RawPeds = new TH2F("h_mFstScanClustersDisplay_RawPeds","h_mFstScanClustersDisplay_RawPeds",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mFstScanClustersDisplay_MaxTb   = new TH2F("h_mFstScanClustersDisplay_MaxTb","h_mFstScanClustersDisplay_MaxTb",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mHitTracksDisplay       = new TH2F("h_mHitTracksDisplay","h_mHitTracksDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TH2F *h_mClusterTracksDisplay   = new TH2F("h_mClusterTracksDisplay","h_mClusterTracksDisplay",60,FST::rMin,FST::rMax,FST::numPhiSeg*4,-2.0*FST::phiMax,2.0*FST::phiMax);
  TGraph *g_mFstSimpleClustersDisplay   = new TGraph();
  TGraph *g_mFstScanClustersDisplay = new TGraph();
  TGraph *g_mHitTracksDisplay     = new TGraph();
  TGraph *g_mClusterTracksDisplay = new TGraph();

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstQAStudy_%s_Th%1.1fTb%dPed%1.1f_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut,pedMode.c_str(),cmnMode.c_str());
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

  mTree_EventDisplay->SetBranchAddress("mNumOfFstSimpleClusters",&mNumOfFstSimpleClusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfFstScanClusters",&mNumOfFstScanClusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst1Clusters",&mNumOfIst1Clusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst2Clusters",&mNumOfIst2Clusters);
  mTree_EventDisplay->SetBranchAddress("mNumOfIst3Clusters",&mNumOfIst3Clusters);
  mTree_EventDisplay->SetBranchAddress("h_mFstSimpleClustersDisplay",&h_mFstSimpleClustersDisplay);
  mTree_EventDisplay->SetBranchAddress("h_mFstScanClustersDisplay",&h_mFstScanClustersDisplay);
  mTree_EventDisplay->SetBranchAddress("h_mFstScanClustersDisplay_RawHits",&h_mFstScanClustersDisplay_RawHits);
  mTree_EventDisplay->SetBranchAddress("h_mFstScanClustersDisplay_RawPeds",&h_mFstScanClustersDisplay_RawPeds);
  mTree_EventDisplay->SetBranchAddress("h_mFstScanClustersDisplay_MaxTb",&h_mFstScanClustersDisplay_MaxTb);

  mTree_EventDisplay->SetBranchAddress("mNumOfHitTracks",&mNumOfHitTracks);
  mTree_EventDisplay->SetBranchAddress("h_mHitTracksDisplay",&h_mHitTracksDisplay);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks",&mNumOfClusterTracks);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks_2Layer",&mNumOfClusterTracks_2Layer);
  mTree_EventDisplay->SetBranchAddress("mNumOfClusterTracks_3Layer",&mNumOfClusterTracks_3Layer);
  mTree_EventDisplay->SetBranchAddress("h_mClusterTracksDisplay",&h_mClusterTracksDisplay);

  mTree_EventDisplay->SetBranchAddress("g_mFstSimpleClustersDisplay",&g_mFstSimpleClustersDisplay);
  mTree_EventDisplay->SetBranchAddress("g_mFstScanClustersDisplay",&g_mFstScanClustersDisplay);
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

  string outputname = "./figures/EventDisplay_2Layer.pdf";
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
  string output_start = "./figures/EventDisplay_2Layer.pdf[";
  c_EventDisplay->Print(output_start.c_str()); // open pdf file

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_EventDisplay->GetEntry(i_event);

    // cout << "mNumOfFstRawHits = " << mNumOfFstRawHits << ", h_mFstRawHitsDisplay->GetEntries() = " << h_mFstRawHitsDisplay->GetEntries() << endl;
    // cout << "mNumOfFstSimpleClusters = " << mNumOfFstSimpleClusters << ", h_mFstSimpleClustersDisplay->GetEntries() = " << h_mFstSimpleClustersDisplay->GetEntries() << endl;
    // cout << "mNumOfHitTracks = " << mNumOfHitTracks << ", h_mHitTracksDisplay->GetEntries() = " << h_mHitTracksDisplay->GetEntries() << endl;
    // cout << "mNumOfClusterTracks = " << mNumOfClusterTracks << ", h_mClusterTracksDisplay->GetEntries() = " << h_mClusterTracksDisplay->GetEntries() << endl;

    if(mNumOfClusterTracks == 1 && mNumOfClusterTracks_2Layer == 1) // 2-Layer Tracking
    // if(mNumOfClusterTracks == 1 && mNumOfClusterTracks_2Layer == 1 && mNumOfClusterTracks_3Layer == 1) // 3-Layer Tracking
    {
      c_EventDisplay->cd(1);
#if 0
      // plot hits
      string Title = Form("Event %d (Threshold %1.1f)", mEventId, nFstHitsCut);
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
#endif

      // plot hits of cluster
      string Title = Form("Event %d (Threshold %1.1f)", mEventId, nFstHitsCut);
      h_mFstScanClustersDisplay_RawHits->SetTitle(Title.c_str());
      h_mFstScanClustersDisplay_RawHits->SetStats(0);
      h_mFstScanClustersDisplay_RawHits->GetXaxis()->SetTitle("R");
      h_mFstScanClustersDisplay_RawHits->GetXaxis()->SetTitleSize(0.06);
      h_mFstScanClustersDisplay_RawHits->GetXaxis()->SetRangeUser(FST::rMin,FST::rMax);
      h_mFstScanClustersDisplay_RawHits->GetYaxis()->SetTitle("#phi");
      h_mFstScanClustersDisplay_RawHits->GetYaxis()->SetRangeUser(phiMinFst-10.0*FST::pitchPhi,FST::phiMax+10.0*FST::pitchPhi);
      h_mFstScanClustersDisplay_RawHits->GetYaxis()->SetTitleSize(0.06);
      h_mFstScanClustersDisplay_RawHits->GetZaxis()->SetRangeUser(1.0,2000.0);
      h_mFstScanClustersDisplay_RawHits->Draw("colz");
      h_mFstScanClustersDisplay_RawHits->SetBarOffset(1.5);
      h_mFstScanClustersDisplay_RawHits->SetMarkerColor(1);
      h_mFstScanClustersDisplay_RawHits->Draw("TEXT Same");

      h_mFstScanClustersDisplay_RawPeds->SetMarkerColor(2);
      h_mFstScanClustersDisplay_RawPeds->SetBarOffset(-1.5);
      h_mFstScanClustersDisplay_RawPeds->Draw("TEXT Same");

      h_mFstScanClustersDisplay_MaxTb->SetMarkerColor(4);
      h_mFstScanClustersDisplay_MaxTb->SetBarOffset(-3.5);
      h_mFstScanClustersDisplay_MaxTb->Draw("TEXT Same");

      // plot clusters
      if(g_mFstSimpleClustersDisplay->GetN() > 0)
      {
	g_mFstSimpleClustersDisplay->SetMarkerStyle(25);
	g_mFstSimpleClustersDisplay->SetMarkerColor(1);
	g_mFstSimpleClustersDisplay->SetMarkerSize(1.2);
	g_mFstSimpleClustersDisplay->Draw("p Same");
      }
      if(g_mFstScanClustersDisplay->GetN() > 0)
      {
	g_mFstScanClustersDisplay->SetMarkerStyle(25);
	g_mFstScanClustersDisplay->SetMarkerColor(2);
	g_mFstScanClustersDisplay->SetMarkerSize(0.8);
	g_mFstScanClustersDisplay->Draw("p Same");
      }

      // plot tracks
      g_mClusterTracksDisplay->SetMarkerStyle(5);
      g_mClusterTracksDisplay->SetMarkerColor(1);
      g_mClusterTracksDisplay->SetMarkerSize(1.0);
      g_mClusterTracksDisplay->Draw("p Same");
      plotBox(g_mClusterTracksDisplay);

      // plot FST acceptance
      PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
      PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
      PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

      c_EventDisplay->cd(2);
      // plot hits
      Title = Form("Event %d (Threshold: %1.1f)", mEventId, nFstThresholdCut);
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

      // plot clusters
      if(g_mFstSimpleClustersDisplay->GetN() > 0)
      {
	g_mFstSimpleClustersDisplay->SetMarkerStyle(25);
	g_mFstSimpleClustersDisplay->SetMarkerColor(1);
	g_mFstSimpleClustersDisplay->SetMarkerSize(1.2);
	g_mFstSimpleClustersDisplay->Draw("p Same");
      }
      if(g_mFstScanClustersDisplay->GetN() > 0)
      {
	g_mFstScanClustersDisplay->SetMarkerStyle(25);
	g_mFstScanClustersDisplay->SetMarkerColor(2);
	g_mFstScanClustersDisplay->SetMarkerSize(0.8);
	g_mFstScanClustersDisplay->Draw("p Same");
      }

      // plot tracks
      g_mClusterTracksDisplay->SetMarkerStyle(5);
      g_mClusterTracksDisplay->SetMarkerColor(1);
      g_mClusterTracksDisplay->SetMarkerSize(1.0);
      g_mClusterTracksDisplay->Draw("p Same");
      plotBox(g_mClusterTracksDisplay);

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

  string output_stop =  "./figures/EventDisplay_2Layer.pdf]";
  c_EventDisplay->Print(output_stop.c_str()); // close pdf file
}

void plotBox(TGraph *g_track)
{
  double r_proj, phi_proj;
  g_track->GetPoint(0,r_proj,phi_proj); // only get r & phi info for first track
  const double phiMinFst = phi_proj-3.0*FST::pitchPhi;
  const double phiMaxFst = phi_proj+3.0*FST::pitchPhi;

  const double rMin_match1 = r_proj-1.0*FST::pitchR;
  const double rMax_match1 = r_proj+1.0*FST::pitchR;
  TBox *b_match1 = new TBox(rMin_match1,phiMinFst,rMax_match1,phiMaxFst);
  b_match1->SetFillColor(0);
  b_match1->SetFillColorAlpha(0,0.1);
  b_match1->SetLineStyle(1);
  b_match1->SetLineColor(1);
  b_match1->SetLineWidth(1);
  b_match1->Draw("l Same");

  const double rMin_match2 = r_proj-1.5*FST::pitchR;
  const double rMax_match2 = r_proj+1.5*FST::pitchR;
  TBox *b_match2 = new TBox(rMin_match2,phiMinFst,rMax_match2,phiMaxFst);
  b_match2->SetFillColor(0);
  b_match2->SetFillColorAlpha(0,0.1);
  b_match2->SetLineStyle(2);
  b_match2->SetLineColor(2);
  b_match2->SetLineWidth(1);
  b_match2->Draw("l Same");

  const double rMin_match3 = r_proj-2.0*FST::pitchR;
  const double rMax_match3 = r_proj+2.0*FST::pitchR;
  TBox *b_match3 = new TBox(rMin_match3,phiMinFst,rMax_match3,phiMaxFst);
  b_match3->SetFillColor(0);
  b_match3->SetFillColorAlpha(0,0.1);
  b_match3->SetLineStyle(3);
  b_match3->SetLineColor(4);
  b_match3->SetLineWidth(1);
  b_match3->Draw("l Same");
}
