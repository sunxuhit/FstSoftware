#include "./FstQAStudy.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include <TFile.h>
#include <TChain.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TMath.h>
#include <TVector3.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TGraph.h>

using namespace std;

ClassImp(FstQAStudy)

//------------------------------------------
FstQAStudy::FstQAStudy() : mList("../../list/FST/FstPed_HV140.list"), mOutPutFile("./FstPed_HV140.root"), mApplyCMNCorr(true)
{
  cout << "FstQAStudy::FstQAStudy() -------- Constructor!  --------" << endl;
  mHome = getenv("HOME");
}

FstQAStudy::~FstQAStudy()
{
  cout << "FstQAStudy::~FstQAStudy() -------- Release Memory!  --------" << endl;
}
//------------------------------------------
int FstQAStudy::Init()
{
  cout << "FstQAStudy::Init => " << endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  bool isInPut = initChain(); // initialize input data/ped TChain;
  initCounts();
  initAdc_Hits();
  initAdc_Clusters();
  initClusterSize();
  initClusterSize_TrackClusters();
  initSignalQA();
  initEventDisplay_TrackClusters();

  if(!isInPut) 
  {
    cout << "Failed to initialize input data!" << endl;
    return -1;
  }

  return 1;
}

int FstQAStudy::Make()
{
  cout << "FstQAStudy::Make => " << endl;

  long NumOfEvents = (long)mChainInPut->GetEntries();
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 1000;
  mChainInPut->GetEntry(0);

  int numOfUsedEvent = 0;
  std::vector<FstRawHit *> rawHitVec_orig;
  std::vector<FstCluster *> clusterVec_orig;
  std::vector<FstTrack *> trackHitsVec_orig;
  std::vector<FstTrack *> trackClustersVec_orig;
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    fillCounts(mFstEvent_InPut);

    rawHitVec_orig.clear(); // clear the container for hits
    for(int i_hit = 0; i_hit < mFstEvent_InPut->getNumRawHits(); ++i_hit)
    { // get Hits info
      FstRawHit *fstRawHit = mFstEvent_InPut->getRawHit(i_hit);
      rawHitVec_orig.push_back(fstRawHit);
    }
    fillAdc_Hits(rawHitVec_orig);

    clusterVec_orig.clear(); // clear the container for clusters
    for(int i_cluster = 0; i_cluster < mFstEvent_InPut->getNumClusters(); ++i_cluster)
    { // get Clusters info
      FstCluster *fstCluster = mFstEvent_InPut->getCluster(i_cluster);
      clusterVec_orig.push_back(fstCluster);
    }
    fillAdc_Clusters(clusterVec_orig);
    fillClusterSize(clusterVec_orig);

    trackHitsVec_orig.clear(); // clear the container for clusters
    trackClustersVec_orig.clear(); // clear the container for clusters
    for(int i_track = 0; i_track < mFstEvent_InPut->getNumTracks(); ++i_track)
    { // get Tracks info
      FstTrack *fstTrack = mFstEvent_InPut->getTrack(i_track);
      if(fstTrack->getTrackType() == 0) // track reconstructed with hits
      {
	trackHitsVec_orig.push_back(fstTrack);
      }
      if(fstTrack->getTrackType() == 1) // track reconstructed with clusters
      {
	trackClustersVec_orig.push_back(fstTrack);
      }
    }
    fillClusterSize_TrackClusters(mFstEvent_InPut);
    fillSignalQA(mFstEvent_InPut);
    fillEventDisplay_TrackClusters(mFstEvent_InPut);
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 1;
}

int FstQAStudy::Finish()
{
  cout << "FstQAStudy::Finish => " << endl;
  writeCounts();
  writeAdc_Hits();
  writeAdc_Cluster();
  writeClusterSize();
  writeClusterSize_TrackClusters();
  writeSignalQA();
  writeEventDisplay_TrackClusters();

  return 1;
}

// init Input TChain
bool FstQAStudy::initChain()
{
  cout << "FstQAStudy::initChain -> " << endl;

  mChainInPut = new TChain("mTree_FstEvent");

  if (!mList.empty())   // if input file is ok
  {
    cout << "Open input probability file list" << endl;
    ifstream in(mList.c_str());  // input stream
    if(in)
    {
      cout << "input file probability list is ok" << endl;
      char str[255];       // char array for each file name
      Long64_t entries_save = 0;
      while(in)
      {
	in.getline(str,255);  // take the lines of the file list
	if(str[0] != 0)
	{
	  string addfile;
	  addfile = str;
	  mChainInPut->AddFile(addfile.c_str(),-1,"mTree_FstEvent");
	  Long64_t file_entries = mChainInPut->GetEntries();
	  cout << "File added to data chain: " << addfile.c_str() << " with " << (file_entries-entries_save) << " entries" << endl;
	  entries_save = file_entries;
	}
      }
    }
    else
    {
      cout << "WARNING: input probability file input is problemtic" << endl;
      return false;
    }
  }

  mFstEvent_InPut = new FstEvent();
  mChainInPut->SetBranchAddress("FstEvent",&mFstEvent_InPut);

  long NumOfEvents = (long)mChainInPut->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;

  return true;
}
// init Input TChain

//--------------QA---------------------
void FstQAStudy::initCounts()
{
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    std::string HistName = Form("h_mCounts_Hits_Layer%d",i_layer);
    h_mCounts_Hits[i_layer] = new TH1F(HistName.c_str(),HistName.c_str(),15,-0.5,14.5);
    HistName = Form("h_mCounts_Clusters_Layer%d",i_layer);
    h_mCounts_Clusters[i_layer] = new TH1F(HistName.c_str(),HistName.c_str(),15,-0.5,14.5);
    HistName = Form("h_mCounts_Corr_Layer%d",i_layer);
    h_mCounts_Corr[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),15,-0.5,14.5,15,-0.5,14.5);
    HistName = Form("h_mCounts_RPhi_Layer%d",i_layer);
    h_mCounts_RPhi[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5,10,-0.5,9.5);
  }
}

void FstQAStudy::fillCounts(FstEvent *fstEvent)
{
  int numOfHits[4] = {0,0,0,0};
  for(int i_hit = 0; i_hit < fstEvent->getNumRawHits(); ++i_hit)
  { // get Hits info
    FstRawHit *fstRawHit = fstEvent->getRawHit(i_hit);
    int layer = fstRawHit->getLayer();
    numOfHits[layer]++;
  }
  int numOfClusters[4] = {0,0,0,0};
  for(int i_cluster = 0; i_cluster < fstEvent->getNumClusters(); ++i_cluster)
  { // get Clusters info
    FstCluster *fstCluster = fstEvent->getCluster(i_cluster);
    int layer = fstCluster->getLayer();
    numOfClusters[layer]++;
    h_mCounts_RPhi[layer]->Fill(fstCluster->getNRawHitsR(),fstCluster->getNRawHitsPhi());
  }

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mCounts_Hits[i_layer]->Fill(numOfHits[i_layer]);
    h_mCounts_Clusters[i_layer]->Fill(numOfClusters[i_layer]);
    h_mCounts_Corr[i_layer]->Fill(numOfHits[i_layer],numOfClusters[i_layer]);
  }
}

void FstQAStudy::writeCounts()
{
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mCounts_Hits[i_layer]->Write();
    h_mCounts_Clusters[i_layer]->Write();
    h_mCounts_Corr[i_layer]->Write();
    h_mCounts_RPhi[i_layer]->Write();
  }
}
//--------------QA---------------------

//--------------ADC---------------------
void FstQAStudy::initAdc_Hits()
{
  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    std::string HistName; 
    HistName = Form("h_mAdcFst_Hits_RStrip%d",i_rstrip);
    h_mAdcFst_Hits[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
    HistName = Form("h_mAdcIst_Hits_Layer%d",i_rstrip);
    h_mAdcIst_Hits[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
  }
}

void FstQAStudy::fillAdc_Hits(std::vector<FstRawHit *> rawHitVec_orig)
{
  std::vector<FstRawHit *> rawHitVec;
  rawHitVec.clear(); // clear the container for hits
  for(int i_hit = 0; i_hit < rawHitVec_orig.size(); ++i_hit)
  {
    rawHitVec.push_back(rawHitVec_orig[i_hit]);
  }

  for(int i_hit = 0; i_hit < rawHitVec.size(); ++i_hit)
  { // get Hits info
    int layer = rawHitVec[i_hit]->getLayer();
    int maxTb = rawHitVec[i_hit]->getMaxTb();
    double adc = rawHitVec[i_hit]->getCharge(maxTb);
    if(rawHitVec[i_hit]->getIsHit())
    {
      h_mAdcIst_Hits[layer]->Fill(adc);

      if(layer == 0)
      { // FST
	int column = rawHitVec[i_hit]->getColumn(); // R strip
	h_mAdcFst_Hits[column]->Fill(adc);
      }
    }
  }
}

void FstQAStudy::writeAdc_Hits()
{
  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
  {
    h_mAdcFst_Hits[i_rstrip]->Write();
    h_mAdcIst_Hits[i_rstrip]->Write();
  }
}

void FstQAStudy::initAdc_Clusters()
{
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    std::string HistName; 
    HistName = Form("h_mAdcFst_Clusters_RStrip%d",i_rstrip);
    h_mAdcFst_Clusters[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
    HistName = Form("h_mAdcIst_Clusters_Layer%d",i_rstrip);
    h_mAdcIst_Clusters[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
  }

  p_mAdcFst_Column = new TProfile2D("p_mAdcFst_Column","p_mAdcFst_Column",FST::numRStrip*5.0,0.0,FST::numRStrip,FST::numRStrip,-0.5,FST::numRStrip-0.5);
  p_mAdcFst_Row = new TProfile2D("p_mAdcFst_Row","p_mAdcFst_Row",FST::numPhiSeg*2.0,0.0,FST::numPhiSeg,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);

  // h_mPositionR_Clusters = new TH2F("h_mPositionR_Clusters","h_mPositionR_Clusters",FST::numRStrip*10.0,FST::rOuter,FST::rOuter+4*FST::pitchR,0.5*FST::numRStrip,FST::rOuter,FST::rOuter+4*FST::pitchR);
  // h_mPositionPhi_Clusters = new TH2F("h_mPositionPhi_Clusters","h_mPositionPhi_Clusters",FST::numPhiSeg,-0.5*FST::numPhiSeg*FST::pitchPhi,0.5*FST::numPhiSeg*FST::pitchPhi,0.5*FST::numPhiSeg,0.0,0.5*FST::numPhiSeg*FST::pitchPhi);
}

void FstQAStudy::fillAdc_Clusters(std::vector<FstCluster *> clusterVec_orig)
{
  std::vector<FstCluster *> clusterVec;
  clusterVec.clear(); // clear the container for hits
  for(int i_cluster = 0; i_cluster < clusterVec_orig.size(); ++i_cluster)
  {
    clusterVec.push_back(clusterVec_orig[i_cluster]);
  }

  for(int i_cluster = 0; i_cluster < clusterVec.size(); ++i_cluster)
  { // get Hits info
    int layer = clusterVec[i_cluster]->getLayer();
    double adc = clusterVec[i_cluster]->getTotCharge();
    h_mAdcIst_Clusters[layer]->Fill(adc);

    if(layer == 0)
    { // FST
      double meanColumn = clusterVec[i_cluster]->getMeanColumn(); // R strip
      int column = 0;
      if(meanColumn > 3.5 && meanColumn <= 4.5) column = 0;
      if(meanColumn > 4.5 && meanColumn <= 5.5) column = 1;
      if(meanColumn > 5.5 && meanColumn <= 6.5) column = 2;
      if(meanColumn > 6.5 && meanColumn <= 7.5) column = 3;
      h_mAdcFst_Clusters[column]->Fill(adc);
    }
  }

  for(int i_cluster = 0; i_cluster < clusterVec.size(); ++i_cluster)
  { // get Hits info
    int layer = clusterVec[i_cluster]->getLayer();
    const int nHits = clusterVec[i_cluster]->getNRawHits();
    if(layer == 0 && nHits > 1) // only study cluster with at least 2 hits
    {
      double meanColumn = clusterVec[i_cluster]->getMeanColumn();
      double meanRow = clusterVec[i_cluster]->getMeanRow();
      for(int i_clusterHit = 0; i_clusterHit < nHits; ++i_clusterHit)
      {
	FstRawHit *fstRawHit = clusterVec[i_cluster]->getRawHit(i_clusterHit);
	int column = fstRawHit->getColumn(); // R strip
	int row = fstRawHit->getRow(); // phi segmentation
	int maxTb = fstRawHit->getMaxTb();
	double adc = fstRawHit->getCharge(maxTb);

	p_mAdcFst_Column->Fill(meanColumn,column,adc);
	p_mAdcFst_Row->Fill(meanRow,row,adc);
      }
    }
  }
}

void FstQAStudy::writeAdc_Cluster()
{
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    h_mAdcFst_Clusters[i_rstrip]->Write();
    h_mAdcIst_Clusters[i_rstrip]->Write();
  }

  p_mAdcFst_Column->Write();
  p_mAdcFst_Row->Write();
}
//--------------ADC---------------------

//--------------Cluster Size---------------------
void FstQAStudy::initClusterSize()
{
  p_mNHitsR_meanColumn   = new TProfile("p_mNHitsR_meanColumn","p_mNHitsR_meanColumn",FST::numRStrip*5.0,-0.5,FST::numRStrip-0.5);
  p_mNHitsR_meanRow      = new TProfile("p_mNHitsR_meanRow","p_mNHitsR_meanRow",FST::numPhiSeg*2.0,-0.5,FST::numPhiSeg-0.5);
  p_mNHitsPhi_meanColumn = new TProfile("p_mNHitsPhi_meanColumn","p_mNHitsPhi_meanColumn",FST::numRStrip*5.0,-0.5,FST::numRStrip-0.5);
  p_mNHitsPhi_meanRow    = new TProfile("p_mNHitsPhi_meanRow","p_mNHitsPhi_meanRow",FST::numPhiSeg*2.0,-0.5,FST::numPhiSeg-0.5);
  p_mTbDiffR             = new TProfile("p_mTbDiffR","p_mTbDiffR",FST::numRStrip*5.0,-0.5,FST::numRStrip-0.5);
  p_mTbDiffPhi           = new TProfile("p_mTbDiffPhi","p_mTbDiffPhi",FST::numPhiSeg*2.0,-0.5,FST::numPhiSeg-0.5);
}

void FstQAStudy::fillClusterSize(std::vector<FstCluster *> clusterVec_orig)
{
  std::vector<FstCluster *> clusterVec;
  clusterVec.clear(); // clear the container for hits
  for(int i_cluster = 0; i_cluster < clusterVec_orig.size(); ++i_cluster)
  {
    clusterVec.push_back(clusterVec_orig[i_cluster]);
  }

  for(int i_cluster = 0; i_cluster < clusterVec.size(); ++i_cluster)
  { // get Hits info
    int layer = clusterVec[i_cluster]->getLayer();
    const int nHits = clusterVec[i_cluster]->getNRawHits();
    if(layer == 0 && nHits > 1) // only study cluster with at least 2 hits
    {
      double meanColumn = clusterVec[i_cluster]->getMeanColumn();
      double meanRow = clusterVec[i_cluster]->getMeanRow();
      double nHitsR = clusterVec[i_cluster]->getNRawHitsR();
      double nHitsPhi = clusterVec[i_cluster]->getNRawHitsPhi();
      p_mNHitsR_meanColumn->Fill(meanColumn,nHitsR);
      p_mNHitsR_meanRow->Fill(meanRow,nHitsR);
      p_mNHitsPhi_meanColumn->Fill(meanColumn,nHitsPhi);
      p_mNHitsPhi_meanRow->Fill(meanRow,nHitsPhi);

      int TimeBin[nHits];
      double charge[nHits];

      for(int i_clusterHit = 0; i_clusterHit < nHits; ++i_clusterHit)
      {
	FstRawHit *fstRawHit = clusterVec[i_cluster]->getRawHit(i_clusterHit);
	int maxTb = fstRawHit->getMaxTb();
	double adc = fstRawHit->getCharge(maxTb);

	TimeBin[i_clusterHit] = maxTb;
	charge[i_clusterHit] = adc;
      }

      double maxCharge = charge[0];
      int maxHitIt = 0;
      for (int i_hit = 1; i_hit < nHits; ++i_hit) 
      {
	if ( charge[i_hit] > maxCharge) 
	{
	  maxCharge = charge[i_hit];
	  maxHitIt = i_hit;
	}
      }

      for(int i_hit = 0; i_hit < nHits; ++i_hit)
      {
	if(i_hit != maxHitIt)
	{
	  p_mTbDiffR->Fill(meanColumn,TimeBin[maxHitIt]-TimeBin[i_hit]);
	  p_mTbDiffPhi->Fill(meanRow,TimeBin[maxHitIt]-TimeBin[i_hit]);
	}
      }
    }
  }
}

void FstQAStudy::writeClusterSize()
{
  p_mNHitsR_meanColumn->Write();
  p_mNHitsR_meanRow->Write();
  p_mNHitsPhi_meanColumn->Write();
  p_mNHitsPhi_meanRow->Write();
  p_mTbDiffR->Write();
  p_mTbDiffPhi->Write();
}
//--------------Cluster Size---------------------

//--------------Cluster Size with Track---------------------
void FstQAStudy::initClusterSize_TrackClusters()
{
  for(int i_match = 0; i_match < 4; ++i_match)
  {
    string ProName;
    ProName = Form("p_mNHitsR_rP_SF%d",i_match*10);
    p_mNHitsR_rP[i_match] = new TProfile(ProName.c_str(),ProName.c_str(),24,FST::rMin[0],FST::rMax[0]);
    ProName = Form("p_mNHitsR_phiP_SF%d",i_match*10);
    p_mNHitsR_phiP[i_match] = new TProfile(ProName.c_str(),ProName.c_str(),FST::numPhiSeg*2,-2.0*FST::phiMax[0],2.0*FST::phiMax[0]);
    ProName = Form("p_mNHitsPhi_rP_SF%d",i_match*10);
    p_mNHitsPhi_rP[i_match] = new TProfile(ProName.c_str(),ProName.c_str(),24,FST::rMin[0],FST::rMax[0]);
    ProName = Form("p_mNHitsPhi_phiP_SF%d",i_match*10);
    p_mNHitsPhi_phiP[i_match] = new TProfile(ProName.c_str(),ProName.c_str(),FST::numPhiSeg*2,-2.0*FST::phiMax[0],2.0*FST::phiMax[0]);
  }
}

void FstQAStudy::fillClusterSize_TrackClusters(FstEvent *fstEvent)
{
  std::vector<FstTrack *> trackClustersVec;
  trackClustersVec.clear(); // clear the container for clusters
  for(int i_track = 0; i_track < fstEvent->getNumTracks(); ++i_track)
  { // get Tracks info
    FstTrack *fstTrack = fstEvent->getTrack(i_track);
    if(fstTrack->getTrackType() == 1) // track reconstructed with clusters
    {
      trackClustersVec.push_back(fstTrack);
    }
  }

  std::vector<FstCluster *> fstClusterVec;
  fstClusterVec.clear(); // clear the container for clusters
  for(int i_cluster = 0; i_cluster < fstEvent->getNumClusters(); ++i_cluster)
  { // get Clusters info
    FstCluster *fstCluster = fstEvent->getCluster(i_cluster);
    if(fstCluster->getLayer() == 0)
    {
      fstClusterVec.push_back(fstCluster);
    }
  }

  int numOfFstClusters = fstEvent->getNumFstClusters_Simple();
  if(numOfFstClusters > 0)
  {
    for(int i_cluster = 0; i_cluster < numOfFstClusters; ++i_cluster)
    {
      int nHits = fstClusterVec[i_cluster]->getNRawHits();
      if(nHits > 1)
      {
	int nHitsR = fstClusterVec[i_cluster]->getNRawHitsR();
	int nHitsPhi = fstClusterVec[i_cluster]->getNRawHitsPhi();
	double r_fst = fstClusterVec[i_cluster]->getMeanX(); // r for fst
	double phi_fst = fstClusterVec[i_cluster]->getMeanY(); // phi for fst

	for(int i_track = 0; i_track < trackClustersVec.size(); ++i_track)
	{
	  TVector3 proj_fst = trackClustersVec[i_track]->getProjection(0);
	  double r_proj = proj_fst.Perp();
	  double phi_proj = proj_fst.Phi();

	  for(int i_match = 0; i_match < 4; ++i_match)
	  {
	    if(i_match == 0)
	    {
	      p_mNHitsR_rP[i_match]->Fill(r_proj,nHitsR);
	      p_mNHitsR_phiP[i_match]->Fill(phi_proj,nHitsR);
	      p_mNHitsPhi_rP[i_match]->Fill(r_proj,nHitsPhi);
	      p_mNHitsPhi_phiP[i_match]->Fill(phi_proj,nHitsPhi);
	    }
	    if( i_match > 0 && abs(r_fst-r_proj) <= (i_match+0.5)*FST::pitchR && abs(phi_fst-phi_proj) <= (i_match*10+0.5)*FST::pitchPhi)
	      // if( i_match > 0 && abs(r_fst-r_proj) <= (i_match+0.5)*FST::pitchR)
	    {
	      p_mNHitsR_rP[i_match]->Fill(r_proj,nHitsR);
	      p_mNHitsR_phiP[i_match]->Fill(phi_proj,nHitsR);
	      p_mNHitsPhi_rP[i_match]->Fill(r_proj,nHitsPhi);
	      p_mNHitsPhi_phiP[i_match]->Fill(phi_proj,nHitsPhi);
	    }
	  }
	}
      }
    }
  }
}

void FstQAStudy::writeClusterSize_TrackClusters()
{
  for(int i_match = 0; i_match < 4; ++i_match)
  {
    p_mNHitsR_rP[i_match]->Write();
    p_mNHitsR_phiP[i_match]->Write();
    p_mNHitsPhi_rP[i_match]->Write();
    p_mNHitsPhi_phiP[i_match]->Write();
  }
}
//--------------Cluster Size with Track---------------------

//--------------Signal QA---------------------
void FstQAStudy::initSignalQA()
{
  p_mPedMap_FST = new TProfile2D("p_mPedMap_FST","p_mPedMap_FST",FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
  p_mSigMap_FST = new TProfile2D("p_mSigMap_FST","p_mSigMap_FST",FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    string HistName;
    HistName = Form("h_mFstHitsMaxTb_Sensor%d",i_sensor);
    h_mFstHitsMaxTb[i_sensor] = new TH1F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5);
    HistName = Form("h_mFstHitsSignal_Sensor%d",i_sensor);
    h_mFstHitsSignal[i_sensor] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
    HistName = Form("h_mFstHitsNoise_Sensor%d",i_sensor);
    h_mFstHitsNoise[i_sensor] = new TH1F(HistName.c_str(),HistName.c_str(),100,-0.5,99.5);
    HistName = Form("h_mFstHitsSNRatio_Sensor%d",i_sensor);
    h_mFstHitsSNRatio[i_sensor] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,99.5);
    HistName = Form("h_mFstSimpleClustersSignal_Sensor%d",i_sensor);
    h_mFstSimpleClustersSignal[i_sensor] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
    HistName = Form("h_mFstScanClustersSignal_Sensor%d",i_sensor);
    h_mFstScanClustersSignal[i_sensor] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);

    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      HistName = Form("h_mFstHitsMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsMaxTb_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5);
      HistName = Form("h_mFstSimpleClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstSimpleClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstSimpleClustersMaxTb_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5);
      HistName = Form("h_mFstScanClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstScanClustersMaxTb_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstScanClustersMaxTb_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5);
      HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsSignal_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
      HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsNoise_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),100,-0.5,99.5);
      HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstHitsSNRatio_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,99.5);
      HistName = Form("h_mFstSimpleClustersSignalSensor%d__Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstSimpleClustersSignalSensor%d__Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstSimpleClustersSignal_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
      HistName = Form("h_mFstScanClustersSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip);
      if(i_sensor > 0) HistName = Form("h_mFstScanClustersSignal_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mFstScanClustersSignal_Rstrip[i_sensor][i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
      for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
      {
	HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstHitsSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstHitsSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
	HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstHitsNoise_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstHitsNoise_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),100,-0.5,99.5);
	HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstHitsSNRatio_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstHitsSNRatio_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,99.5);
	HistName = Form("h_mFstSimpleClustersSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstSimpleClustersSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstSimpleClustersSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
	HistName = Form("h_mFstScanClustersSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip,i_tb);
	if(i_sensor > 0) HistName = Form("h_mFstScanClustersSignal_Sensor%d_Rstrip%d_TimeBin%d",i_sensor,i_rstrip+4,i_tb);
	h_mFstScanClustersSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
      }
    }
  }
}

void FstQAStudy::fillSignalQA(FstEvent *fstEvent)
{
  std::vector<FstRawHit *> fstRawHitVec;
  fstRawHitVec.clear(); // clear the container for hits
  for(int i_hit = 0; i_hit < fstEvent->getNumRawHits(); ++i_hit)
  { // get Hits info
    FstRawHit *fstRawHit = fstEvent->getRawHit(i_hit);
    if(fstRawHit->getLayer() == 0)
    {
      fstRawHitVec.push_back(fstRawHit);
    }
  }
  int numOfFstHits = fstEvent->getNumFstRawHits();

  if(numOfFstHits > 0)
  {
    for(int i_hit = 0; i_hit < numOfFstHits; ++i_hit)
    {
      const int sensorId  = fstRawHitVec[i_hit]->getSensor();
      const int apv       = fstRawHitVec[i_hit]->getAPV();
      const int channel   = fstRawHitVec[i_hit]->getChannel();
      int column          = fstRawHitVec[i_hit]->getColumn(); // RStrip
      const int row       = fstRawHitVec[i_hit]->getRow(); // phi segmentation
      const int maxTb     = fstRawHitVec[i_hit]->getMaxTb();
      const double ped    = fstRawHitVec[i_hit]->getPedMean(maxTb); // pedMean
      const double signal = fstRawHitVec[i_hit]->getCharge(maxTb); // adc - pedMean - cmn
      double noise        = fstRawHitVec[i_hit]->getPedStdDev(maxTb); // pedStdDev
      if(mApplyCMNCorr) noise  = fstRawHitVec[i_hit]->getRanStdDev(maxTb); // pedStdDev
      p_mPedMap_FST->Fill(column,row,ped);
      p_mSigMap_FST->Fill(column,row,signal);
      if(fstRawHitVec[i_hit]->getIsHit())
      {
	h_mFstHitsMaxTb[sensorId]->Fill(maxTb);
	h_mFstHitsSignal[sensorId]->Fill(signal);
	h_mFstHitsNoise[sensorId]->Fill(noise);
	h_mFstHitsSNRatio[sensorId]->Fill(signal/noise);

	if(sensorId > 0) column = column-4;

	h_mFstHitsMaxTb_Rstrip[sensorId][column]->Fill(maxTb);
	h_mFstHitsSignal_Rstrip[sensorId][column]->Fill(signal);
	h_mFstHitsNoise_Rstrip[sensorId][column]->Fill(noise);
	h_mFstHitsSNRatio_Rstrip[sensorId][column]->Fill(signal/noise);

	h_mFstHitsSignal_Rstrip_TimeBin[sensorId][column][maxTb]->Fill(signal);
	h_mFstHitsNoise_Rstrip_TimeBin[sensorId][column][maxTb]->Fill(noise);
	h_mFstHitsSNRatio_Rstrip_TimeBin[sensorId][column][maxTb]->Fill(signal/noise);
      }
    }
  }

  std::vector<FstCluster *> fstClusterVec;
  fstClusterVec.clear(); // clear the container for clusters
  for(int i_cluster = 0; i_cluster < fstEvent->getNumClusters(); ++i_cluster)
  { // get Clusters info
    FstCluster *fstCluster = fstEvent->getCluster(i_cluster);
    if(fstCluster->getLayer() == 0)
    {
      fstClusterVec.push_back(fstCluster);
    }
  }
  int numOfFstClusters = fstClusterVec.size();
  if(numOfFstClusters > 0)
  {
    for(int i_cluster = 0; i_cluster < numOfFstClusters; ++i_cluster)
    {
      int sensorId = fstClusterVec[i_cluster]->getSensor();
      double meanColumn = fstClusterVec[i_cluster]->getMeanColumn();
      double meanRow = fstClusterVec[i_cluster]->getMeanRow();
      double signal = fstClusterVec[i_cluster]->getTotCharge();
      double maxTb = fstClusterVec[i_cluster]->getMaxTb();
      if(fstClusterVec[i_cluster]->getClusterType() == 1 && fstClusterVec[i_cluster]->getIsSeed())
      { // Simple Clusters
	h_mFstSimpleClustersSignal[sensorId]->Fill(signal);

	int column = -1;
	if(meanColumn > -0.5 && meanColumn <= 0.5) column = 0;
	if(meanColumn >  0.5 && meanColumn <= 1.5) column = 1;
	if(meanColumn >  1.5 && meanColumn <= 2.5) column = 2;
	if(meanColumn >  2.5 && meanColumn <= 3.5) column = 3;
	if(meanColumn >  3.5 && meanColumn <= 4.5) column = 0;
	if(meanColumn >  4.5 && meanColumn <= 5.5) column = 1;
	if(meanColumn >  5.5 && meanColumn <= 6.5) column = 2;
	if(meanColumn >  6.5 && meanColumn <= 7.5) column = 3;
	if(column > -1)
	{
	  h_mFstSimpleClustersMaxTb_Rstrip[sensorId][column]->Fill(maxTb);
	  h_mFstSimpleClustersSignal_Rstrip[sensorId][column]->Fill(signal);
	  h_mFstSimpleClustersSignal_Rstrip_TimeBin[sensorId][column][(int)maxTb]->Fill(signal);
	}
      }
      if(fstClusterVec[i_cluster]->getClusterType() == 2 && fstClusterVec[i_cluster]->getIsSeed())
      { // Scan Clusters
	h_mFstScanClustersSignal[sensorId]->Fill(signal);

	int column = -1;
	if(meanColumn > -0.5 && meanColumn <= 0.5) column = 0;
	if(meanColumn >  0.5 && meanColumn <= 1.5) column = 1;
	if(meanColumn >  1.5 && meanColumn <= 2.5) column = 2;
	if(meanColumn >  2.5 && meanColumn <= 3.5) column = 3;
	if(meanColumn >  3.5 && meanColumn <= 4.5) column = 0;
	if(meanColumn >  4.5 && meanColumn <= 5.5) column = 1;
	if(meanColumn >  5.5 && meanColumn <= 6.5) column = 2;
	if(meanColumn >  6.5 && meanColumn <= 7.5) column = 3;
	if(column > -1)
	{
	  h_mFstScanClustersMaxTb_Rstrip[sensorId][column]->Fill(maxTb);
	  h_mFstScanClustersSignal_Rstrip[sensorId][column]->Fill(signal);
	  h_mFstScanClustersSignal_Rstrip_TimeBin[sensorId][column][(int)maxTb]->Fill(signal);
	}
      }
    }
  }
}

void FstQAStudy::writeSignalQA()
{
  p_mPedMap_FST->Write();
  p_mSigMap_FST->Write();

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    h_mFstHitsMaxTb[i_sensor]->Write();
    h_mFstHitsSignal[i_sensor]->Write();
    h_mFstHitsNoise[i_sensor]->Write();
    h_mFstHitsSNRatio[i_sensor]->Write();
    h_mFstSimpleClustersSignal[i_sensor]->Write();
    h_mFstScanClustersSignal[i_sensor]->Write();

    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      h_mFstHitsMaxTb_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstSimpleClustersMaxTb_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstScanClustersMaxTb_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstHitsSignal_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstHitsNoise_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstHitsSNRatio_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstSimpleClustersSignal_Rstrip[i_sensor][i_rstrip]->Write();
      h_mFstScanClustersSignal_Rstrip[i_sensor][i_rstrip]->Write();
      for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
      {
	h_mFstHitsSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->Write();
	h_mFstHitsNoise_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->Write();
	h_mFstHitsSNRatio_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->Write();
	h_mFstSimpleClustersSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->Write();
	h_mFstScanClustersSignal_Rstrip_TimeBin[i_sensor][i_rstrip][i_tb]->Write();
      }
    }
  }
}
//--------------Signal QA---------------------

//--------------Event Display---------------------
void FstQAStudy::initEventDisplay_TrackClusters()
{
  const int numRDisplay = 10;
  const double rMinDisplay = FST::rInner - 1.0*FST::pitchR;
  const double rMaxDisplay = FST::rOuter + 5.0*FST::pitchR;
  const int numPhiDisplay = 2*FST::numPhiSeg;
  const double phiMinDisplay = -TMath::Pi()*30.0/180.0 - FST::gapPhi;
  const double phiMaxDisplay =  TMath::Pi()*30.0/180.0 + FST::gapPhi;
  h_mFstRawHitsDisplay              = new TH2F("h_mFstRawHitsDisplay","h_mFstRawHitsDisplay",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstRawPedsDisplay              = new TH2F("h_mFstRawPedsDisplay","h_mFstRawPedsDisplay",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstMaxTbDisplay                = new TH2F("h_mFstMaxTbDisplay","h_mFstMaxTbDisplay",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstRawHitsDisplay_bTh          = new TH2F("h_mFstRawHitsDisplay_bTh","h_mFstRawHitsDisplay_bTh",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstRawPedsDisplay_bTh          = new TH2F("h_mFstRawPedsDisplay_bTh","h_mFstRawPedsDisplay_bTh",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstMaxTbDisplay_bTh            = new TH2F("h_mFstMaxTbDisplay_bTh","h_mFstMaxTbDisplay_bTh",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstSimpleClustersDisplay       = new TH2F("h_mFstSimpleClustersDisplay","h_mFstSimpleClustersDisplay",10*numRDisplay,rMinDisplay,rMaxDisplay,2*numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstScanClustersDisplay         = new TH2F("h_mFstScanClustersDisplay","h_mFstScanClustersDisplay",10*numRDisplay,rMinDisplay,rMaxDisplay,2*numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstScanClustersDisplay_RawHits = new TH2F("h_mFstScanClustersDisplay_RawHits","h_mFstScanClustersDisplay_RawHits",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstScanClustersDisplay_RawPeds = new TH2F("h_mFstScanClustersDisplay_RawPeds","h_mFstScanClustersDisplay_RawPeds",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mFstScanClustersDisplay_MaxTb   = new TH2F("h_mFstScanClustersDisplay_MaxTb","h_mFstScanClustersDisplay_MaxTb",numRDisplay,rMinDisplay,rMaxDisplay,numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mHitTracksDisplay               = new TH2F("h_mHitTracksDisplay","h_mHitTracksDisplay",10*numRDisplay,rMinDisplay,rMaxDisplay,2*numPhiDisplay,phiMinDisplay,phiMaxDisplay);
  h_mClusterTracksDisplay           = new TH2F("h_mClusterTracksDisplay","h_mClusterTracksDisplay",10*numRDisplay,rMinDisplay,rMaxDisplay,2*numPhiDisplay,phiMinDisplay,phiMaxDisplay);

  g_mFstSimpleClustersDisplay = new TGraph();
  g_mFstSimpleClustersDisplay->SetName("g_mFstSimpleClustersDisplay");
  g_mFstScanClustersDisplay = new TGraph();
  g_mFstScanClustersDisplay->SetName("g_mFstScanClustersDisplay");
  g_mHitTracksDisplay = new TGraph();
  g_mHitTracksDisplay->SetName("g_mHitTracksDisplay");
  g_mClusterTracksDisplay = new TGraph();
  g_mClusterTracksDisplay->SetName("g_mClusterTracksDisplay");

  mTree_EventDisplay = new TTree("mTree_EventDisplay","Fst Hits Clusters Tracks Display");
  mTree_EventDisplay->Branch("mEventId",&mEventId,"mEventId/I");
  mTree_EventDisplay->Branch("mNumOfFstRawHits",&mNumOfFstRawHits,"mNumOfFstRawHits/I");
  mTree_EventDisplay->Branch("mNumOfIst1RawHits",&mNumOfIst1RawHits,"mNumOfIst1RawHits/I");
  mTree_EventDisplay->Branch("mNumOfIst2RawHits",&mNumOfIst2RawHits,"mNumOfIst2RawHits/I");
  mTree_EventDisplay->Branch("mNumOfIst3RawHits",&mNumOfIst3RawHits,"mNumOfIst3RawHits/I");
  mTree_EventDisplay->Branch("h_mFstRawHitsDisplay","TH2F",&h_mFstRawHitsDisplay);
  mTree_EventDisplay->Branch("h_mFstRawPedsDisplay","TH2F",&h_mFstRawPedsDisplay);
  mTree_EventDisplay->Branch("h_mFstMaxTbDisplay","TH2F",&h_mFstMaxTbDisplay);
  mTree_EventDisplay->Branch("h_mFstRawHitsDisplay_bTh","TH2F",&h_mFstRawHitsDisplay_bTh); // hit map below threshold
  mTree_EventDisplay->Branch("h_mFstRawPedsDisplay_bTh","TH2F",&h_mFstRawPedsDisplay_bTh);
  mTree_EventDisplay->Branch("h_mFstMaxTbDisplay_bTh","TH2F",&h_mFstMaxTbDisplay_bTh);

  mTree_EventDisplay->Branch("mNumOfFstSimpleClusters",&mNumOfFstSimpleClusters,"mNumOfFstSimpleClusters/I");
  mTree_EventDisplay->Branch("mNumOfFstScanClusters",&mNumOfFstScanClusters,"mNumOfFstScanClusters/I");
  mTree_EventDisplay->Branch("mNumOfIst1Clusters",&mNumOfIst1Clusters,"mNumOfIst1Clusters/I");
  mTree_EventDisplay->Branch("mNumOfIst2Clusters",&mNumOfIst2Clusters,"mNumOfIst2Clusters/I");
  mTree_EventDisplay->Branch("mNumOfIst3Clusters",&mNumOfIst3Clusters,"mNumOfIst3Clusters/I");
  mTree_EventDisplay->Branch("h_mFstSimpleClustersDisplay","TH2F",&h_mFstSimpleClustersDisplay);
  mTree_EventDisplay->Branch("h_mFstScanClustersDisplay","TH2F",&h_mFstScanClustersDisplay);
  mTree_EventDisplay->Branch("h_mFstScanClustersDisplay_RawHits","TH2F",&h_mFstScanClustersDisplay_RawHits);
  mTree_EventDisplay->Branch("h_mFstScanClustersDisplay_RawPeds","TH2F",&h_mFstScanClustersDisplay_RawPeds);
  mTree_EventDisplay->Branch("h_mFstScanClustersDisplay_MaxTb","TH2F",&h_mFstScanClustersDisplay_MaxTb);

  mTree_EventDisplay->Branch("mNumOfHitTracks",&mNumOfHitTracks,"mNumOfHitTracks/I");
  mTree_EventDisplay->Branch("h_mHitTracksDisplay","TH2F",&h_mHitTracksDisplay);
  mTree_EventDisplay->Branch("mNumOfClusterTracks",&mNumOfClusterTracks,"mNumOfClusterTracks/I");
  mTree_EventDisplay->Branch("mNumOfClusterTracks_2Layer",&mNumOfClusterTracks_2Layer,"mNumOfClusterTracks_2Layer/I");
  mTree_EventDisplay->Branch("mNumOfClusterTracks_3Layer",&mNumOfClusterTracks_3Layer,"mNumOfClusterTracks_3Layer/I");
  mTree_EventDisplay->Branch("h_mClusterTracksDisplay","TH2F",&h_mClusterTracksDisplay);

  mTree_EventDisplay->Branch("g_mFstSimpleClustersDisplay","TGraph",&g_mFstSimpleClustersDisplay);
  mTree_EventDisplay->Branch("g_mFstScanClustersDisplay","TGraph",&g_mFstScanClustersDisplay);
  mTree_EventDisplay->Branch("g_mHitTracksDisplay","TGraph",&g_mHitTracksDisplay);
  mTree_EventDisplay->Branch("g_mClusterTracksDisplay","TGraph",&g_mClusterTracksDisplay);

  mTree_EventDisplay->SetAutoSave(50000000);

  h_mNumFstRawHitsDisplay = new TH1F("h_mNumFstRawHitsDisplay","h_mNumFstRawHitsDisplay",100,-0.5,99.5);
  h_mNumFstSimpleClustersDisplay = new TH1F("h_mNumFstSimpleClustersDisplay","h_mNumFstSimpleClustersDisplay",100,-0.5,99.5);
  h_mNumFstScanClustersDisplay = new TH1F("h_mNumFstScanClustersDisplay","h_mNumFstScanClustersDisplay",100,-0.5,99.5);
}

void FstQAStudy::clearEventDisplay_TrackClusters()
{
  mEventId = -1;
  mNumOfFstRawHits = 0;
  mNumOfIst1RawHits = 0;
  mNumOfIst2RawHits = 0;
  mNumOfIst3RawHits = 0;

  mNumOfFstSimpleClusters = 0;
  mNumOfFstScanClusters = 0;
  mNumOfIst1Clusters = 0;
  mNumOfIst2Clusters = 0;
  mNumOfIst3Clusters = 0;

  mNumOfHitTracks = 0;
  mNumOfClusterTracks = 0;
  mNumOfClusterTracks_2Layer = 0;
  mNumOfClusterTracks_3Layer = 0;

  h_mFstRawHitsDisplay->Reset();
  h_mFstRawPedsDisplay->Reset();
  h_mFstMaxTbDisplay->Reset();
  h_mFstRawHitsDisplay_bTh->Reset();
  h_mFstRawPedsDisplay_bTh->Reset();
  h_mFstMaxTbDisplay_bTh->Reset();

  h_mFstSimpleClustersDisplay->Reset();
  h_mFstScanClustersDisplay->Reset();
  h_mFstScanClustersDisplay_RawHits->Reset();
  h_mFstScanClustersDisplay_RawPeds->Reset();
  h_mFstScanClustersDisplay_MaxTb->Reset();

  h_mHitTracksDisplay->Reset();
  h_mClusterTracksDisplay->Reset();

  g_mFstSimpleClustersDisplay->Set(-1);
  g_mFstScanClustersDisplay->Set(-1);
  g_mHitTracksDisplay->Set(-1);
  g_mClusterTracksDisplay->Set(-1);
}

void FstQAStudy::fillEventDisplay_TrackClusters(FstEvent *fstEvent)
{
  this->clearEventDisplay_TrackClusters();

  mEventId = fstEvent->getEventId();
  // cout << "mEventId = " << mEventId << endl;

  for(int i_hit = 0; i_hit < fstEvent->getNumRawHits(); ++i_hit)
  { // fill Hits Display
    FstRawHit *fstRawHit = fstEvent->getRawHit(i_hit);
    if(fstRawHit->getLayer() == 0) // FST
    {
      double r_fst = fstRawHit->getPosX(); // r for fst
      double phi_fst = fstRawHit->getPosY(); // phi for fst
      int maxTb = fstRawHit->getMaxTb();
      double adc = fstRawHit->getCharge(maxTb); // adc - pedMean
      double ped = fstRawHit->getPedStdDev(maxTb); // pedStdDev
      if(mApplyCMNCorr) ped = fstRawHit->getRanStdDev(maxTb); // ranStdDev

      if( fstRawHit->getIsHit() )
      { // above threshold
	mNumOfFstRawHits++;
	h_mFstRawHitsDisplay->Fill(r_fst,phi_fst,adc);
	h_mFstRawPedsDisplay->Fill(r_fst,phi_fst,ped);
	h_mFstMaxTbDisplay->Fill(r_fst,phi_fst,maxTb);
      }
      h_mFstRawHitsDisplay_bTh->Fill(r_fst,phi_fst,adc);
      h_mFstRawPedsDisplay_bTh->Fill(r_fst,phi_fst,ped);
      h_mFstMaxTbDisplay_bTh->Fill(r_fst,phi_fst,maxTb);
    }
    if(fstRawHit->getLayer() == 1) mNumOfIst1RawHits++;
    if(fstRawHit->getLayer() == 2) mNumOfIst2RawHits++;
    if(fstRawHit->getLayer() == 3) mNumOfIst3RawHits++;
  }
  // cout << "mNumOfFstRawHits = " << mNumOfFstRawHits << ", fstEvent->getNumRawHits = " << fstEvent->getNumFstClusters() << endl;
  h_mNumFstRawHitsDisplay->Fill(mNumOfFstRawHits);

  for(int i_cluster = 0; i_cluster < fstEvent->getNumClusters(); ++i_cluster)
  { // fill Clusters Display
    FstCluster *fstCluster = fstEvent->getCluster(i_cluster);
    if(fstCluster->getLayer() == 0 && fstCluster->getClusterType() == 1) // FST Simple Cluster
    {
      if( fstCluster->getIsSeed() ) // select cluste with seed
      {
	mNumOfFstSimpleClusters++;
	double r_fst = fstCluster->getMeanX(); // r for fst
	double phi_fst = fstCluster->getMeanY(); // phi for fst
	double adc = fstCluster->getTotCharge();
	h_mFstSimpleClustersDisplay->Fill(r_fst,phi_fst,adc);
	g_mFstSimpleClustersDisplay->SetPoint(mNumOfFstSimpleClusters-1,r_fst,phi_fst);
      }
    }
    if(fstCluster->getLayer() == 1 && fstCluster->getClusterType() == 1) mNumOfIst1Clusters++;
    if(fstCluster->getLayer() == 2 && fstCluster->getClusterType() == 1) mNumOfIst2Clusters++;
    if(fstCluster->getLayer() == 3 && fstCluster->getClusterType() == 1) mNumOfIst3Clusters++;

    if(fstCluster->getLayer() == 0 && fstCluster->getClusterType() == 2) // FST Scan Cluster
    {
      if( fstCluster->getIsSeed() ) // select cluste with seed
      {
	mNumOfFstScanClusters++;
	double r_fst = fstCluster->getMeanX(); // r for fst
	double phi_fst = fstCluster->getMeanY(); // phi for fst
	double adc = fstCluster->getTotCharge();
	h_mFstScanClustersDisplay->Fill(r_fst,phi_fst,adc);
	g_mFstScanClustersDisplay->SetPoint(mNumOfFstScanClusters-1,r_fst,phi_fst);

	for(int i_hit = 0; i_hit < fstCluster->getNumRawHits(); ++i_hit)
	{ // fill hits display for clusters with hit
	  FstRawHit *fstRawHit = fstCluster->getRawHit(i_hit);
	  double r_hit = fstRawHit->getPosX(); // r for fst
	  double phi_hit = fstRawHit->getPosY(); // phi for fst
	  int maxTb_hit = fstRawHit->getMaxTb();
	  double adc_hit = fstRawHit->getCharge(maxTb_hit); // adc - pedMean
	  double ped_hit = fstRawHit->getPedStdDev(maxTb_hit); // pedStdDev
	  if(mApplyCMNCorr) ped_hit = fstRawHit->getRanStdDev(maxTb_hit); // ranStdDev

	  h_mFstScanClustersDisplay_RawHits->Fill(r_hit,phi_hit,adc_hit);
	  h_mFstScanClustersDisplay_RawPeds->Fill(r_hit,phi_hit,ped_hit);
	  h_mFstScanClustersDisplay_MaxTb->Fill(r_hit,phi_hit,maxTb_hit);
	}
      }
    }
  }
  // cout << "mNumOfFstSimpleClusters = " << mNumOfFstSimpleClusters << ", fstEvent->getNumClusters = " << fstEvent->getNumFstClusters() << endl;
  h_mNumFstSimpleClustersDisplay->Fill(mNumOfFstSimpleClusters);
  h_mNumFstScanClustersDisplay->Fill(mNumOfFstScanClusters);

  for(int i_track = 0; i_track < mFstEvent_InPut->getNumTracks(); ++i_track)
  { // fill Tracks Display
    FstTrack *fstTrack = mFstEvent_InPut->getTrack(i_track);

    TVector3 pos_ist1 = fstTrack->getPosOrig(1);
    double y1_ist = pos_ist1.Y(); // original hit postion on IST1
    TVector3 pos_ist3 = fstTrack->getPosOrig(3);
    double y3_ist = pos_ist3.Y(); // original hit postion on IST3

    TVector3 proj_fst = fstTrack->getProjection(0);
    double r_proj = proj_fst.X();
    double phi_proj = proj_fst.Y();

    TVector3 proj_ist2 = fstTrack->getProjection(2);
    double x2_proj = proj_ist2.X(); // get aligned projected position w.r.t. IST2
    double y2_proj = proj_ist2.Y(); // x & y for ist2

    if(fstTrack->getTrackType() == 0) // track reconstructed with hits
    {
      mNumOfHitTracks++;
      h_mHitTracksDisplay->Fill(r_proj,phi_proj);
      g_mHitTracksDisplay->SetPoint(mNumOfHitTracks-1,r_proj,phi_proj);
    }
    if(fstTrack->getTrackType() == 1) // track reconstructed with clusters
    {
      mNumOfClusterTracks++;
      h_mClusterTracksDisplay->Fill(r_proj,phi_proj);
      g_mClusterTracksDisplay->SetPoint(mNumOfClusterTracks-1,r_proj,phi_proj);

      if( abs(y1_ist-y3_ist) < 17.0*FST::pitchRow )
      {
	if(r_proj >= FST::rInner-1.0*FST::pitchR && r_proj <= FST::rOuter+5.0*FST::pitchR && phi_proj >= -TMath::Pi()*15.0/180.0-10*FST::pitchPhi && phi_proj <= TMath::Pi()*15.0/180.0+10*FST::pitchPhi)
	{ // save only when in the area of interest
	  mNumOfClusterTracks_2Layer++; // satisfied 2-Layer tracking

	  for(int i_cluster = 0; i_cluster < fstEvent->getNumClusters(); ++i_cluster)
	  { // find IST2 matching
	    FstCluster *fstCluster = fstEvent->getCluster(i_cluster);
	    if(fstCluster->getLayer() == 2) // IST2
	    {
	      double x2_ist = fstCluster->getMeanX(); // x for ist2
	      double y2_ist = fstCluster->getMeanY(); // y for ist2
	      if( x2_proj >= 20.0*FST::pitchColumn && x2_proj <= 24.0*FST::pitchColumn )
	      {
		if( abs(x2_ist-x2_proj) < 6.0 && abs(y2_ist-y2_proj) < 0.6 )
		{ // IST2 matching cut
		  mNumOfClusterTracks_3Layer++;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  mTree_EventDisplay->Fill();
}

void FstQAStudy::writeEventDisplay_TrackClusters()
{
  mTree_EventDisplay->Write();
  h_mNumFstRawHitsDisplay->Write();
  h_mNumFstSimpleClustersDisplay->Write();
  h_mNumFstScanClustersDisplay->Write();
}
//--------------Event Display---------------------
