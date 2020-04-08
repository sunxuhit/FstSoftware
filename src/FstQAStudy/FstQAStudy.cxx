#include "./FstQAStudy.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include <TFile.h>
#include <TChain.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TVector3.h>
#include <TProfile2D.h>

using namespace std;

ClassImp(FstQAStudy)

//------------------------------------------
FstQAStudy::FstQAStudy() : mList("../../list/FST/FstPed_HV140.list"), mOutPutFile("./FstPed_HV140.root")
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
  initAdcFst();

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
  NumOfEvents = 1000;
  mChainInPut->GetEntry(0);

  std::vector<FstRawHit *> rawHitVec;
  std::vector<FstCluster *> clusterVec;
  std::vector<FstTrack *> trackHitsVec;
  std::vector<FstTrack *> trackClustersVec;
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    fillCounts(mFstEvent_InPut);
    fillAdcFst(mFstEvent_InPut);

    rawHitVec.clear(); // clear the container for hits
    for(int i_hit = 0; i_hit < mFstEvent_InPut->getNumRawHits(); ++i_hit)
    { // get Hits info
      FstRawHit *fstRawHit = mFstEvent_InPut->getRawHit(i_hit);
      rawHitVec.push_back(fstRawHit);
    }

    clusterVec.clear(); // clear the container for clusters
    for(int i_cluster = 0; i_cluster < mFstEvent_InPut->getNumClusters(); ++i_cluster)
    { // get Clusters info
      FstCluster *fstCluster = mFstEvent_InPut->getCluster(i_cluster);
      clusterVec.push_back(fstCluster);
      int nHits = fstCluster->getNRawHitsR();
      if(nHits > 1)
      {
	cout << "nHits = " << nHits << endl;
	fstCluster->Print();
	// for(int i_hit = 0; i_hit < nHits; ++i_hit)
	// {
	//   cout << "hitId = " << fstCluster->getHitId(i_hit) << endl;
	//   FstRawHit *fstRawHit_Cluster = fstCluster->getRawHit(i_hit);
	//   fstRawHit_Cluster->Print();
	// }
      }
    }

    trackHitsVec.clear(); // clear the container for clusters
    trackClustersVec.clear(); // clear the container for clusters
    for(int i_track = 0; i_track < mFstEvent_InPut->getNumTracks(); ++i_track)
    { // get Tracks info
      FstTrack *fstTrack = mFstEvent_InPut->getTrack(i_track);
      if(fstTrack->getTrackType() == 0) // track reconstructed with hits
      {
	trackHitsVec.push_back(fstTrack);
      }
      if(fstTrack->getTrackType() == 1) // track reconstructed with clusters
      {
	trackClustersVec.push_back(fstTrack);
      }
    }
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 1;
}

int FstQAStudy::Finish()
{
  cout << "FstQAStudy::Finish => " << endl;
  writeCounts();
  writeAdcFst();

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
void FstQAStudy::initAdcFst()
{
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    std::string HistName = Form("h_mAdcFst_Hits_RStrip%d",i_rstrip);
    h_mAdcFst_Hits[i_rstrip] = new TH1F(HistName.c_str(),HistName.c_str(),200,-0.5,1999.5);
  }
  p_mAdcFst_Hits = new TProfile2D("p_mAdcFst_Hits","p_mAdcFst_Hits",FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
  p_mAdcFst_Column = new TProfile2D("p_mAdcFst_Column","p_mAdcFst_Column",FST::numRStrip*5.0,0.0,FST::numRStrip,FST::numRStrip,-0.5,FST::numRStrip-0.5);
  p_mAdcFst_Row = new TProfile2D("p_mAdcFst_Row","p_mAdcFst_Row",FST::numPhiSeg*2.0,0.0,FST::numPhiSeg,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
  p_mMaxTbFst_Column = new TProfile2D("p_mMaxTbFst_Column","p_mMaxTbFst_Column",FST::numRStrip*5.0,0.0,FST::numRStrip,FST::numRStrip,-0.5,FST::numRStrip-0.5);
  p_mMaxTbFst_Row = new TProfile2D("p_mMaxTbFst_Row","p_mMaxTbFst_Row",FST::numPhiSeg*2.0,0.0,FST::numPhiSeg,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);

  h_mTbDiff_Clusters = new TH1F("h_mTbDiff_Clusters","h_mTbDiff_Clusters",21,-10.5,10.5);
  h_mPositionR_Clusters = new TH2F("h_mPositionR_Clusters","h_mPositionR_Clusters",FST::numRStrip*10.0,FST::rOuter,FST::rOuter+4*FST::pitchR,0.5*FST::numRStrip,FST::rOuter,FST::rOuter+4*FST::pitchR);
  h_mPositionPhi_Clusters = new TH2F("h_mPositionPhi_Clusters","h_mPositionPhi_Clusters",FST::numPhiSeg,-0.5*FST::numPhiSeg*FST::pitchPhi,0.5*FST::numPhiSeg*FST::pitchPhi,0.5*FST::numPhiSeg,0.0,0.5*FST::numPhiSeg*FST::pitchPhi);
}

void FstQAStudy::fillAdcFst(FstEvent *fstEvent)
{
  std::vector<FstRawHit *> rawHitVec;
  rawHitVec.clear(); // clear the container for hits

  for(int i_hit = 0; i_hit < fstEvent->getNumRawHits(); ++i_hit)
  { // get Hits info
    FstRawHit *fstRawHit = fstEvent->getRawHit(i_hit);
    int layer = fstRawHit->getLayer();
    if(layer == 0)
    {
      rawHitVec.push_back(fstRawHit);
      int column = fstRawHit->getColumn(); // R strip
      int row = fstRawHit->getRow(); // R strip
      int maxTb = fstRawHit->getMaxTb();
      double adc = fstRawHit->getCharge(maxTb);
      h_mAdcFst_Hits[column-4]->Fill(adc);
      p_mAdcFst_Hits->Fill(column,row,adc);
    }
  }

  for(int i_cluster = 0; i_cluster < fstEvent->getNumClusters(); ++i_cluster)
  { // get Hits info
    FstCluster *fstCluster = fstEvent->getCluster(i_cluster);
    int layer = fstCluster->getLayer();
    const int nHits = fstCluster->getNRawHits();
    if(layer == 0 && nHits > 1) // only study cluster with at least 2 hits
    {
      double meanColumn = fstCluster->getMeanColumn();
      double meanRow = fstCluster->getMeanRow();
      int TimeBin[nHits];
      double charge[nHits];
      int numOfHits = 0;
      /*
      for(int i_clusterHit = 0; i_clusterHit < fstCluster->getNRawHits(); ++i_clusterHit)
      {
	int cHitId = fstCluster->getHitId(i_clusterHit);
	for(int i_hit = 0; i_hit < rawHitVec.size(); ++i_hit)
	{
	  int hitId = rawHitVec[i_hit]->getHitId();
	  if(cHitId == hitId)
	  {
	    int column = rawHitVec[i_hit]->getColumn(); // R strip
	    int row = rawHitVec[i_hit]->getRow(); // phi segmentation
	    int maxTb = rawHitVec[i_hit]->getMaxTb();
	    double adc = rawHitVec[i_hit]->getCharge(maxTb);
	    p_mAdcFst_Column->Fill(meanColumn,column,adc);
	    p_mAdcFst_Row->Fill(meanRow,row,adc);
	    p_mMaxTbFst_Column->Fill(meanColumn,column,maxTb);
	    p_mMaxTbFst_Row->Fill(meanRow,row,maxTb);
	    TimeBin[numOfHits] = maxTb;
	    charge[numOfHits] = adc;
	  }
	}
	numOfHits++;
      }
      */

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
      // cout << "maxHitIt = " << maxHitIt << ", charge[maxHitIt] = " << charge[maxHitIt] << ", maxCharge = " << maxCharge << endl;

      for(int i_hit = 0; i_hit < nHits; ++i_hit)
      {
	if(i_hit != maxHitIt)
	{
	  h_mTbDiff_Clusters->Fill(TimeBin[maxHitIt]-TimeBin[i_hit]);
	}
      }
    }
  }

  for(int i_track = 0; i_track < mFstEvent_InPut->getNumTracks(); ++i_track)
  { // get Tracks info
    FstTrack *fstTrack = mFstEvent_InPut->getTrack(i_track);
    if(fstTrack->getTrackType() == 1) // track reconstructed with clusters
    {

      TVector3 v0_fst = fstTrack->getPosition(0); // get fst read out position
      double r0_fst = v0_fst.Perp();
      double phi0_fst = v0_fst.Phi();

      TVector3 p0_fst = fstTrack->getProjection(0); // get projected position
      double r0_proj = p0_fst.Perp();
      double phi0_proj = p0_fst.Phi();

      h_mPositionR_Clusters->Fill(r0_proj,r0_fst);
      h_mPositionPhi_Clusters->Fill(phi0_proj,phi0_fst);
    }
  }
}

void FstQAStudy::writeAdcFst()
{
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    h_mAdcFst_Hits[i_rstrip]->Write();
  }
  p_mAdcFst_Hits->Write();
  p_mAdcFst_Column->Write();
  p_mAdcFst_Row->Write();
  p_mMaxTbFst_Column->Write();
  p_mMaxTbFst_Row->Write();

  h_mTbDiff_Clusters->Write();
  h_mPositionR_Clusters->Write();
  h_mPositionPhi_Clusters->Write();
}
//--------------QA---------------------

