#include "./FstTracking.h"

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

using namespace std;

ClassImp(FstTracking)

//------------------------------------------
FstTracking::FstTracking() : mList("../../list/FST/FstPed_HV140.list"), mOutPutFile("./FstPed_HV140.root")
{
  cout << "FstTracking::FstTracking() -------- Constructor!  --------" << endl;
  mHome = getenv("HOME");
}

FstTracking::~FstTracking()
{
  cout << "FstTracking::~FstTracking() -------- Release Memory!  --------" << endl;
}
//------------------------------------------
int FstTracking::Init()
{
  cout << "FstTracking::Init => " << endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  bool isInPut = initChain(); // initialize input data/ped TChain;
  bool isTracking_Hits = initTrackingQA_Hits(); // initialize tracking with Hits
  initTracking_Hits();
  initTracking_Clusters();
  initEfficiency_Hits();
  initEfficiency_Clusters();

  if(!isInPut) 
  {
    cout << "Failed to initialize input data!" << endl;
    return -1;
  }
  if(!isTracking_Hits) 
  {
    cout << "Failed to initialize Hits based Tracking!" << endl;
    return -1;
  }

  initHitDisplay(); // initialize hit display
  initClusterDisplay(); // initialize hit display

  return 1;
}

int FstTracking::Make()
{
  cout << "FstTracking::Make => " << endl;

  long NumOfEvents = (long)mChainInPut->GetEntries();
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 100;
  mChainInPut->GetEntry(0);

  std::vector<FstRawHit *> rawHitVec;
  std::vector<FstCluster *> clusterVec;
  std::vector<FstTrack *> trackHitsVec;
  std::vector<FstTrack *> trackClustersVec;
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    rawHitVec.clear(); // clear the container for hits
    for(int i_hit = 0; i_hit < mFstEvent_InPut->getNumRawHits(); ++i_hit)
    { // get Hits info
      FstRawHit *fstRawHit = mFstEvent_InPut->getRawHit(i_hit);
      rawHitVec.push_back(fstRawHit);
    }
    fillHitDisplay(rawHitVec); // fill Hits display
    fillTrackingQA_Hits(rawHitVec); // do tracking based on the first hit of each layer

    clusterVec.clear(); // clear the container for clusters
    for(int i_cluster = 0; i_cluster < mFstEvent_InPut->getNumClusters(); ++i_cluster)
    { // get Clusters info
      FstCluster *fstCluster = mFstEvent_InPut->getCluster(i_cluster);
      clusterVec.push_back(fstCluster);
    }
    fillClusterDisplay(clusterVec); // fill Hits display

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
    calResolution_Hits(trackHitsVec);
    calResolution_Clusters(trackClustersVec);
    calEfficiency_Hits(trackHitsVec);
    calEfficiency_Clusters(trackClustersVec);

  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;


  return 1;
}

int FstTracking::Finish()
{
  cout << "FstTracking::Finish => " << endl;
  writeHitDisplay();
  writeClusterDisplay();
  writeTrackingQA_Hits();
  writeTracking_Hits();
  writeTracking_Clusters();
  writeEfficiency_Hits();
  writeEfficiency_Clusters();

  return 1;
}

// init Input TChain
bool FstTracking::initChain()
{
  cout << "FstTracking::initChain -> " << endl;

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

//--------------hit display---------------------
bool FstTracking::initHitDisplay()
{
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    std::string HistName = Form("h_mHitDisplay_Layer%d",i_layer);
    if(i_layer == 0) h_mHitDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
    else h_mHitDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::noRows,-0.5,FST::noRows-0.5);
    
    HistName = Form("h_mMaxTb_Layer%d",i_layer);
    h_mMaxTb[i_layer] = new TH1F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5); 
  }

  return true;
}

void FstTracking::fillHitDisplay(std::vector<FstRawHit *> rawHitVec_orig)
{
  for(int i_hit = 0; i_hit < rawHitVec_orig.size(); ++i_hit)
  {
    int layer = rawHitVec_orig[i_hit]->getLayer();
    h_mHitDisplay[layer]->Fill(rawHitVec_orig[i_hit]->getColumn(),rawHitVec_orig[i_hit]->getRow());
    h_mMaxTb[layer]->Fill(rawHitVec_orig[i_hit]->getMaxTb());
  }
}

void FstTracking::writeHitDisplay()
{
  cout << "FstTracking::writeHitDisplay => save Hits at each Layer!" << endl;
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mHitDisplay[i_layer]->Write();
    h_mMaxTb[i_layer]->Write();
  }
}
//--------------hit display---------------------

//--------------cluster display---------------------
bool FstTracking::initClusterDisplay()
{
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    std::string HistName = Form("h_mClusterDisplay_Layer%d",i_layer);
    if(i_layer == 0) h_mClusterDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
    else h_mClusterDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::noRows,-0.5,FST::noRows-0.5);
  }

  return true;
}

void FstTracking::fillClusterDisplay(std::vector<FstCluster *> clusterVec_orig)
{
  for(int i_cluster = 0; i_cluster < clusterVec_orig.size(); ++i_cluster)
  {
    int layer = clusterVec_orig[i_cluster]->getLayer();
    h_mClusterDisplay[layer]->Fill(clusterVec_orig[i_cluster]->getMeanColumn(),clusterVec_orig[i_cluster]->getMeanRow());
  }
}

void FstTracking::writeClusterDisplay()
{
  cout << "FstTracking::writeClusterDisplay => save Clusters at each Layer!" << endl;
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mClusterDisplay[i_layer]->Write();
  }

}
//--------------cluster display---------------------

//--------------Track QA with Hits---------------------
bool FstTracking::initTrackingQA_Hits()
{
  const string CorrNameXR[4] = {"ist1x_ist3x","ist1x_fstR","ist3x_fstR","ist1x_ist3x_fstR"};
  const string CorrNameYPhi[4] = {"ist1y_ist3y","ist1y_fstPhi","ist3y_fstPhi","ist1y_ist3y_fstPhi"};
  for(int i_corr = 0; i_corr < 4; ++i_corr)
  { // for QA
    string HistName = Form("h_mHitsCorrXR_%s",CorrNameXR[i_corr].c_str());
    if(i_corr == 0) h_mHitsCorrXR[i_corr] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::noColumns,-0.5,FST::noColumns-0.5);
    else h_mHitsCorrXR[i_corr] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::numRStrip,-0.5,FST::numRStrip-0.5);

    HistName = Form("h_mHitsCorrYPhi_%s",CorrNameYPhi[i_corr].c_str());
    if(i_corr == 0) h_mHitsCorrYPhi[i_corr] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noRows,-0.5,FST::noRows-0.5,FST::noRows,-0.5,FST::noRows-0.5);
    else h_mHitsCorrYPhi[i_corr] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noRows,-0.5,FST::noRows-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
  }

  // h_mXResidual_Hits_Before = new TH1F("h_mXResidual_Hits_Before","h_mXResidual_Hits_Before",100,-500.0,500.0);
  // h_mYResidual_Hits_Before = new TH1F("h_mYResidual_Hits_Before","h_mYResidual_Hits_Before",100,-50.0,50.0);
  // h_mXResidual_Hits = new TH1F("h_mXResidual_Hits","h_mXResidual_Hits",100,-100.0,100.0);
  // h_mYResidual_Hits = new TH1F("h_mYResidual_Hits","h_mYResidual_Hits",100,-50.0,50.0);
  // h_mRResidual_Hits = new TH1F("h_mRResidual_Hits","h_mRResidual_Hits",100,-50.0,50.0);
  // h_mPhiResidual_Hits = new TH1F("h_mPhiResidual_Hits","h_mPhiResidual_Hits",100,-5.0*FST::pitchPhi,5.0*FST::pitchPhi);

  h_mXResidual_Hits_Before = new TH1F("h_mXResidual_Hits_Before","h_mXResidual_Hits_Before",100,-500.0,500.0);
  h_mYResidual_Hits_Before = new TH1F("h_mYResidual_Hits_Before","h_mYResidual_Hits_Before",100,-50.0,50.0);
  h_mXResidual_Hits = new TH1F("h_mXResidual_Hits","h_mXResidual_Hits",100,-80.0,80.0);
  h_mYResidual_Hits = new TH1F("h_mYResidual_Hits","h_mYResidual_Hits",100,-16.0,16.0);
  h_mRResidual_Hits = new TH1F("h_mRResidual_Hits","h_mRResidual_Hits",100,-80.0,80.0);
  h_mPhiResidual_Hits = new TH1F("h_mPhiResidual_Hits","h_mPhiResidual_Hits",100,-0.05,0.05);

  return true;
}

void FstTracking::fillTrackingQA_Hits(std::vector<FstRawHit *> rawHitVec_orig)
{
  int numOfHits[4]; // 0 for fst | 1-3 for ist
  std::vector<FstRawHit *> rawHitVec[4];
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    numOfHits[i_layer] = 0;
    rawHitVec[i_layer].clear();
  }

  for(int i_hit = 0; i_hit < rawHitVec_orig.size(); ++i_hit)
  { // set temp ist hit container for each layer
    int layer = rawHitVec_orig[i_hit]->getLayer();
    numOfHits[layer]++;
    rawHitVec[layer].push_back(rawHitVec_orig[i_hit]);
  }

  if(numOfHits[0] > 0 && numOfHits[1] > 0 && numOfHits[3] > 0)
  { // only do tracking when at least 1 hit is found in fst & ist1 & ist3

    // QA Histograms
    h_mHitsCorrXR[0]->Fill(rawHitVec[1][0]->getColumn(),rawHitVec[3][0]->getColumn());
    h_mHitsCorrXR[1]->Fill(rawHitVec[1][0]->getColumn(),rawHitVec[0][0]->getColumn());
    h_mHitsCorrXR[2]->Fill(rawHitVec[3][0]->getColumn(),rawHitVec[0][0]->getColumn());
    h_mHitsCorrXR[3]->Fill(5.25/2*rawHitVec[1][0]->getColumn()-3.25/2*rawHitVec[3][0]->getColumn(),rawHitVec[0][0]->getColumn());

    h_mHitsCorrYPhi[0]->Fill(rawHitVec[1][0]->getRow(),rawHitVec[3][0]->getRow());
    h_mHitsCorrYPhi[1]->Fill(rawHitVec[1][0]->getRow(),rawHitVec[0][0]->getRow());
    h_mHitsCorrYPhi[2]->Fill(rawHitVec[3][0]->getRow(),rawHitVec[0][0]->getRow());
    h_mHitsCorrYPhi[3]->Fill(5.25/2*rawHitVec[1][0]->getRow()-3.25/2*rawHitVec[3][0]->getRow()+20,rawHitVec[0][0]->getRow());

    double r_fst = FST::rOuter + (rawHitVec[0][0]->getColumn()-4)*FST::pitchR + 0.5*FST::pitchR;
    double phi_fst = (63-rawHitVec[0][0]->getRow())*FST::pitchPhi + 0.5*FST::pitchPhi;
    double x0_fst = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
    double y0_fst = r_fst*TMath::Sin(phi_fst); // y = r*sin(phi)
    double z0_fst = FST::pitchLayer03;

    double x1_ist = rawHitVec[1][0]->getColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
    double y1_ist = (63-rawHitVec[1][0]->getRow())*FST::pitchRow + 0.5*FST::pitchRow;
    double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;

    double x3_ist = rawHitVec[3][0]->getColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
    double y3_ist = (63-rawHitVec[3][0]->getRow())*FST::pitchRow + 0.5*FST::pitchRow;
    double z3_ist = 0.0;

    double x0_proj = x3_ist + (x1_ist-x3_ist)*z0_fst/z1_ist;
    double y0_proj = y3_ist + (y1_ist-y3_ist)*z0_fst/z1_ist;

    double x0_corr = x0_proj*TMath::Cos(FST::phi_rot) + y0_proj*TMath::Sin(FST::phi_rot) + FST::x_shift;
    double y0_corr = y0_proj*TMath::Cos(FST::phi_rot) - x0_proj*TMath::Sin(FST::phi_rot) + FST::y_shift;
    /*
    double x1_corr = x1_ist*TMath::Cos(FST::phi_rot_ist1) + y1_ist*TMath::Sin(FST::phi_rot_ist1) + FST::x_shift;
    double y1_corr = y1_ist*TMath::Cos(FST::phi_rot_ist1) - x1_ist*TMath::Sin(FST::phi_rot_ist1) + FST::y_shift;

    double x3_corr = x3_ist*TMath::Cos(FST::phi_rot_ist3) + y3_ist*TMath::Sin(FST::phi_rot_ist3) + FST::x_shift;
    double y3_corr = y3_ist*TMath::Cos(FST::phi_rot_ist3) - x3_ist*TMath::Sin(FST::phi_rot_ist3) + FST::y_shift;

    double x0_corr = x3_corr + (x1_corr-x3_corr)*z0_fst/z1_ist;
    double y0_corr = y3_corr + (y1_corr-y3_corr)*z0_fst/z1_ist;
    */

    if(abs(rawHitVec[1][0]->getRow()-rawHitVec[3][0]->getRow()) < 17)
    {
      h_mXResidual_Hits_Before->Fill(x0_fst-x0_proj);
      h_mYResidual_Hits_Before->Fill(y0_fst-y0_proj);

      double xResidual = x0_fst-x0_corr;
      double yResidual = y0_fst-y0_corr;
      h_mXResidual_Hits->Fill(xResidual);
      h_mYResidual_Hits->Fill(yResidual);

      double r_corr = TMath::Sqrt(x0_corr*x0_corr+y0_corr*y0_corr);
      double phi_corr = TMath::ATan2(y0_corr,x0_corr);
      double rResidual = r_fst-r_corr;
      double phiResidual = phi_fst-phi_corr;
      h_mRResidual_Hits->Fill(rResidual);
      h_mPhiResidual_Hits->Fill(phiResidual);
    }
  }
}

void FstTracking::writeTrackingQA_Hits()
{
  for(int i_corr = 0; i_corr < 4; ++i_corr)
  {
    h_mHitsCorrXR[i_corr]->Write();
    h_mHitsCorrYPhi[i_corr]->Write();
  }
  h_mXResidual_Hits_Before->Write();
  h_mYResidual_Hits_Before->Write();
  h_mXResidual_Hits->Write();
  h_mYResidual_Hits->Write();
  h_mRResidual_Hits->Write();
  h_mPhiResidual_Hits->Write();
}
//--------------Track QA with Hits---------------------

//--------------Track Resolution with Hits---------------------
void FstTracking::initTracking_Hits()
{
  h_mTrackXRes_Hits   = new TH1F("h_mTrackXRes_Hits","h_mTrackXRes_Hits",100,-80.0,80.0);
  h_mTrackYRes_Hits   = new TH1F("h_mTrackYRes_Hits","h_mTrackYRes_Hits",100,-16.0,16.0);
  h_mTrackRRes_Hits   = new TH1F("h_mTrackRRes_Hits","h_mTrackRRes_Hits",100,-80.0,80.0);
  h_mTrackPhiRes_Hits = new TH1F("h_mTrackPhiRes_Hits","h_mTrackPhiRes_Hits",100,-0.05,0.05);
}

void FstTracking::calResolution_Hits(std::vector<FstTrack *> trackHitVec_orig)
{
  std::vector<FstTrack *> trackHitVec;
  trackHitVec.clear();
  for(int i_track = 0; i_track < trackHitVec_orig.size(); ++i_track)
  { // set temp track container for all tracks
    trackHitVec.push_back(trackHitVec_orig[i_track]);
  }


  // fill residual histograms
  for(int i_track = 0; i_track < trackHitVec.size(); ++i_track)
  {
    int id_fst = trackHitVec[i_track]->getId(0);
    TVector3 pos_fst  = trackHitVec[i_track]->getPosition(0);
    TVector3 proj_fst = trackHitVec[i_track]->getProjection(0);
    if(id_fst > 0)
    {
      // cout << "pos_fst.X = " << pos_fst.X() << ", pos_fst.Y = " << pos_fst.Y() << ", pos_fst.Z = " << pos_fst.Z() << endl;
      // cout << "proj_fst.X = " << proj_fst.X() << ", proj_fst.Y = " << proj_fst.Y() << ", proj_fst.Z = " << proj_fst.Z() << endl;
      // trackHitVec[i_track]->Print();
      double x0_fst = pos_fst.X();
      double y0_fst = pos_fst.Y();
      double x0_proj = proj_fst.X();
      double y0_proj = proj_fst.Y();

      double xResidual = x0_fst-x0_proj;
      double yResidual = y0_fst-y0_proj;
      h_mTrackXRes_Hits->Fill(xResidual);
      h_mTrackYRes_Hits->Fill(yResidual);

      double r_fst = TMath::Sqrt(x0_fst*x0_fst+y0_fst*y0_fst);
      double phi_fst = TMath::ATan2(y0_fst,x0_fst);
      double r_proj = TMath::Sqrt(x0_proj*x0_proj+y0_proj*y0_proj);
      double phi_proj = TMath::ATan2(y0_proj,x0_proj);
      double rResidual = r_fst-r_proj;
      double phiResidual = phi_fst-phi_proj;
      h_mTrackRRes_Hits->Fill(rResidual);
      h_mTrackPhiRes_Hits->Fill(phiResidual);
    }
  }
}

void FstTracking::writeTracking_Hits()
{
  h_mTrackXRes_Hits->Write();
  h_mTrackYRes_Hits->Write();
  h_mTrackRRes_Hits->Write();
  h_mTrackPhiRes_Hits->Write();
}
//--------------Track Resolution with Hits---------------------

//--------------Track Resolution with Clusters---------------------
void FstTracking::initTracking_Clusters()
{
  h_mTrackXRes_Clusters   = new TH1F("h_mTrackXRes_Clusters","h_mTrackXRes_Clusters",100,-80.0,80.0);
  h_mTrackYRes_Clusters   = new TH1F("h_mTrackYRes_Clusters","h_mTrackYRes_Clusters",100,-16.0,16.0);
  h_mTrackRRes_Clusters   = new TH1F("h_mTrackRRes_Clusters","h_mTrackRRes_Clusters",100,-80.0,80.0);
  h_mTrackPhiRes_Clusters = new TH1F("h_mTrackPhiRes_Clusters","h_mTrackPhiRes_Clusters",100,-0.05,0.05);
}

void FstTracking::calResolution_Clusters(std::vector<FstTrack *> trackClusterVec_orig)
{
  std::vector<FstTrack *> trackClusterVec;
  trackClusterVec.clear();
  for(int i_track = 0; i_track < trackClusterVec_orig.size(); ++i_track)
  { // set temp track container for all tracks
    trackClusterVec.push_back(trackClusterVec_orig[i_track]);
  }


  // fill residual histograms
  for(int i_track = 0; i_track < trackClusterVec.size(); ++i_track)
  {
    int id_fst = trackClusterVec[i_track]->getId(0);
    TVector3 pos_fst  = trackClusterVec[i_track]->getPosition(0);
    TVector3 proj_fst = trackClusterVec[i_track]->getProjection(0);
    if(id_fst > 0)
    {
      // cout << "pos_fst.X = " << pos_fst.X() << ", pos_fst.Y = " << pos_fst.Y() << ", pos_fst.Z = " << pos_fst.Z() << endl;
      // cout << "proj_fst.X = " << proj_fst.X() << ", proj_fst.Y = " << proj_fst.Y() << ", proj_fst.Z = " << proj_fst.Z() << endl;
      // trackClusterVec[i_track]->Print();
      double x0_fst = pos_fst.X();
      double y0_fst = pos_fst.Y();
      double x0_proj = proj_fst.X();
      double y0_proj = proj_fst.Y();

      double xResidual = x0_fst-x0_proj;
      double yResidual = y0_fst-y0_proj;
      h_mTrackXRes_Clusters->Fill(xResidual);
      h_mTrackYRes_Clusters->Fill(yResidual);

      double r_fst = TMath::Sqrt(x0_fst*x0_fst+y0_fst*y0_fst);
      double phi_fst = TMath::ATan2(y0_fst,x0_fst);
      double r_proj = TMath::Sqrt(x0_proj*x0_proj+y0_proj*y0_proj);
      double phi_proj = TMath::ATan2(y0_proj,x0_proj);
      double rResidual = r_fst-r_proj;
      double phiResidual = phi_fst-phi_proj;
      h_mTrackRRes_Clusters->Fill(rResidual);
      h_mTrackPhiRes_Clusters->Fill(phiResidual);
    }
  }
}

void FstTracking::writeTracking_Clusters()
{
  h_mTrackXRes_Clusters->Write();
  h_mTrackYRes_Clusters->Write();
  h_mTrackRRes_Clusters->Write();
  h_mTrackPhiRes_Clusters->Write();
}

//--------------Track Resolution with Clusters---------------------

//--------------Efficiency with Hits---------------------
void FstTracking::initEfficiency_Hits()
{
  h_mTrackHits_IST = new TH2F("h_mTrackHits_IST","h_mTrackHits_IST",4,FST::rOuter,FST::rOuter+4.0*FST::pitchR,64,0.0,64.0*FST::pitchPhi);
  h_mTrackHits_FST = new TH2F("h_mTrackHits_FST","h_mTrackHits_FST",4,FST::rOuter,FST::rOuter+4.0*FST::pitchR,64,0.0,64.0*FST::pitchPhi);
}

void FstTracking::calEfficiency_Hits(std::vector<FstTrack *> trackHitVec_orig)
{
  std::vector<FstTrack *> trackHitVec;
  trackHitVec.clear();
  for(int i_track = 0; i_track < trackHitVec_orig.size(); ++i_track)
  { // set temp ist hit container for all tracks
    trackHitVec.push_back(trackHitVec_orig[i_track]);
  }

  const double rMax = FST::rOuter + 4.0*FST::pitchR;
  const double rMin = FST::rOuter;
  const double phiMax = 64.0*FST::pitchPhi;
  const double phiMin = 0.0;

  // fill residual histograms
  for(int i_track = 0; i_track < trackHitVec.size(); ++i_track)
  {
    int id_fst = trackHitVec[i_track]->getId(0);
    TVector3 pos_fst  = trackHitVec[i_track]->getPosition(0);
    TVector3 proj_fst = trackHitVec[i_track]->getProjection(0);

    double r_proj = proj_fst.Perp();
    double phi_proj = proj_fst.Phi();

    if(r_proj >= rMin && r_proj <= rMax && phi_proj >= phiMin && phi_proj <= phiMax)
    { // used for efficiency only if the projected position is within FST acceptance
      h_mTrackHits_IST->Fill(r_proj,phi_proj);

      if(id_fst > 0)
      {
	double r_fst = pos_fst.Perp();
	double phi_fst = pos_fst.Phi();;
	h_mTrackHits_FST->Fill(r_proj,phi_proj);
      }
    }
  }

}

void FstTracking::writeEfficiency_Hits()
{
  h_mTrackHits_IST->Write();
  h_mTrackHits_FST->Write();
}
//--------------Efficiency with Hits---------------------

//--------------Efficiency with Clusters---------------------
void FstTracking::initEfficiency_Clusters()
{
  h_mTrackClusters_IST = new TH2F("h_mTrackClusters_IST","h_mTrackClusters_IST",4,FST::rOuter,FST::rOuter+4.0*FST::pitchR,64,0.0,64.0*FST::pitchPhi);
  h_mTrackClusters_FST = new TH2F("h_mTrackClusters_FST","h_mTrackClusters_FST",4,FST::rOuter,FST::rOuter+4.0*FST::pitchR,64,0.0,64.0*FST::pitchPhi);
}

void FstTracking::calEfficiency_Clusters(std::vector<FstTrack *> trackClusterVec_orig)
{
  std::vector<FstTrack *> trackClusterVec;
  trackClusterVec.clear();
  for(int i_track = 0; i_track < trackClusterVec_orig.size(); ++i_track)
  { // set temp ist hit container for all tracks
    trackClusterVec.push_back(trackClusterVec_orig[i_track]);
  }

  const double rMax = FST::rOuter + 4.0*FST::pitchR;
  const double rMin = FST::rOuter;
  const double phiMax = 64.0*FST::pitchPhi;
  const double phiMin = 0.0;

  // fill residual histograms
  for(int i_track = 0; i_track < trackClusterVec.size(); ++i_track)
  {
    int id_fst = trackClusterVec[i_track]->getId(0);
    TVector3 pos_fst  = trackClusterVec[i_track]->getPosition(0);
    TVector3 proj_fst = trackClusterVec[i_track]->getProjection(0);

    double r_proj = proj_fst.Perp();
    double phi_proj = proj_fst.Phi();

    if(r_proj >= rMin && r_proj <= rMax && phi_proj >= phiMin && phi_proj <= phiMax)
    { // used for efficiency only if the projected position is within FST acceptance
      h_mTrackClusters_IST->Fill(r_proj,phi_proj);

      if(id_fst > 0)
      {
	double r_fst = pos_fst.Perp();
	double phi_fst = pos_fst.Phi();;
	h_mTrackClusters_FST->Fill(r_proj,phi_proj);
      }
    }
  }

}


void FstTracking::writeEfficiency_Clusters()
{
  h_mTrackClusters_IST->Write();
  h_mTrackClusters_FST->Write();
}
//--------------Efficiency with Clusters---------------------
