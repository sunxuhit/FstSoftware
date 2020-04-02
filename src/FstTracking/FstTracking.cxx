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
  bool isTracking_Hits = initTracking_Hits(); // initialize tracking with Hits
  bool isTree_OutPut = initTree();

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
  if(!isTree_OutPut) 
  {
    cout << "Failed to initialize TTree for FstEvent OutPut!" << endl;
    return -1;
  }

  initHitDisplay(); // initialize hit display

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

    clusterVec.clear(); // clear the container for clusters
    for(int i_cluster = 0; i_cluster < mFstEvent_InPut->getNumClusters(); ++i_cluster)
    { // get Clusters info
      FstCluster *fstCluster = mFstEvent_InPut->getCluster(i_cluster);
      clusterVec.push_back(fstCluster);
    }

    fillHitDisplay(rawHitVec); // fill Hits display
    fillTrackingQA_Hits(rawHitVec); // do tracking based on the first hit of each layer

    std::vector<FstTrack *> fstTrackVec_Hits = findTrack_Hits(rawHitVec);
    std::vector<FstTrack *> fstTrackVec_Clusters = findTrack_Clusters(clusterVec);

    // Setup OutPut FstEvent TTree
    mFstEvent_OutPut->clearRawHitsList(); // FstRawHit
    for(int i_hit = 0; i_hit < rawHitVec.size(); ++i_hit)
    {
      mFstRawHit = mFstEvent_OutPut->createRawHit();
      mFstRawHit->setLayer(rawHitVec[i_hit]->getLayer());
      mFstRawHit->setSensor(rawHitVec[i_hit]->getSensor());
      mFstRawHit->setColumn(rawHitVec[i_hit]->getColumn());
      mFstRawHit->setRow(rawHitVec[i_hit]->getRow());
      mFstRawHit->setCharge(rawHitVec[i_hit]->getCharge(rawHitVec[i_hit]->getMaxTb()),rawHitVec[i_hit]->getMaxTb());
      mFstRawHit->setMaxTb(rawHitVec[i_hit]->getMaxTb());
      mFstRawHit->setHitId(rawHitVec[i_hit]->getHitId());
      mFstRawHit->setDefaultTb(rawHitVec[i_hit]->getDefaultTb());
    }

    mFstEvent_OutPut->clearClustersList(); // FstCluster
    for(int i_cluster = 0; i_cluster < clusterVec.size(); ++i_cluster)
    {
      mFstCluster = mFstEvent_OutPut->createCluster();
      mFstCluster->setLayer(clusterVec[i_cluster]->getLayer());
      mFstCluster->setSensor(clusterVec[i_cluster]->getSensor());
      mFstCluster->setMeanColumn(clusterVec[i_cluster]->getMeanColumn());
      mFstCluster->setMeanRow(clusterVec[i_cluster]->getMeanRow());
      mFstCluster->setTotCharge(clusterVec[i_cluster]->getTotCharge());
      mFstCluster->setMaxTb(clusterVec[i_cluster]->getMaxTb());
      mFstCluster->setClusterType(clusterVec[i_cluster]->getClusterType());
      mFstCluster->setNRawHits(clusterVec[i_cluster]->getNRawHits());
      mFstCluster->setNRawHitsR(clusterVec[i_cluster]->getNRawHitsR());
      mFstCluster->setNRawHitsPhi(clusterVec[i_cluster]->getNRawHitsPhi());
      mFstCluster->setClusterId(i_cluster);

      for(int i_hit = 0; i_hit < FST::maxNHits; ++i_hit)
      {
	mFstCluster->setHitId(i_hit, clusterVec[i_cluster]->getHitId(i_hit));
      }
    }

    mFstEvent_OutPut->clearTracksList(); // FstTrack
    for(int i_track = 0; i_track < fstTrackVec_Hits.size(); ++i_track)
    { // get track from hits
      mFstTrack = mFstEvent_OutPut->createTrack();
      // mFstTrack = fstTrackVec_Hits[i_track];
      mFstTrack->setTrackId(fstTrackVec_Hits[i_track]->getTrackId());
      mFstTrack->setTrackType(fstTrackVec_Hits[i_track]->getTrackType());
      for(int i_layer = 0; i_layer < 4; ++i_layer)
      {
	mFstTrack->setId(i_layer, fstTrackVec_Hits[i_track]->getId(i_layer));
	mFstTrack->setPosition(i_layer, fstTrackVec_Hits[i_track]->getPosition(i_layer));
	mFstTrack->setProjection(i_layer, fstTrackVec_Hits[i_track]->getProjection(i_layer));
	mFstTrack->setPosOrig(i_layer, fstTrackVec_Hits[i_track]->getPosOrig(i_layer));
      }
    }
    for(int i_track = 0; i_track < fstTrackVec_Clusters.size(); ++i_track)
    { // get track from clusters
      mFstTrack = mFstEvent_OutPut->createTrack();
      // mFstTrack = fstTrackVec_Clusters[i_track];
      mFstTrack->setTrackId(fstTrackVec_Clusters[i_track]->getTrackId());
      mFstTrack->setTrackType(fstTrackVec_Clusters[i_track]->getTrackType());
      for(int i_layer = 0; i_layer < 4; ++i_layer)
      {
	mFstTrack->setId(i_layer, fstTrackVec_Clusters[i_track]->getId(i_layer));
	mFstTrack->setPosition(i_layer, fstTrackVec_Clusters[i_track]->getPosition(i_layer));
	mFstTrack->setProjection(i_layer, fstTrackVec_Clusters[i_track]->getProjection(i_layer));
	mFstTrack->setPosOrig(i_layer, fstTrackVec_Clusters[i_track]->getPosOrig(i_layer));
      }
    }

    mTree_FstEvent_OutPut->Fill();
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;


  return 1;
}

int FstTracking::Finish()
{
  cout << "FstTracking::Finish => " << endl;
  writeHitDisplay();
  writeTracking_Hits();
  writeTree();

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

//--------------Track QA with Hits---------------------
bool FstTracking::initTracking_Hits()
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

void FstTracking::writeTracking_Hits()
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

//--------------Find Track with Hits---------------------
std::vector<FstTrack *> FstTracking::findTrack_Hits(std::vector<FstRawHit *> rawHitVec_orig)
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

  const double z0_fst = FST::pitchLayer03;
  const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
  const double z2_ist = FST::pitchLayer23;
  const double z3_ist = 0.0;

  std::vector<FstTrack *> trackVec;
  trackVec.clear();
  int numOfTracks = 0;
  if(numOfHits[1] > 0 && numOfHits[3] > 0)
  { // only do tracking when at least 1 hit is found in fst & ist1 & ist3
    TVector3 pos_fst, pos_ist1, pos_ist2, pos_ist3;
    TVector3 pos_fst_orig, pos_ist1_orig, pos_ist2_orig, pos_ist3_orig;
    TVector3 proj_fst, proj_ist2;

    for(int i_ist1 = 0; i_ist1 < numOfHits[1]; ++i_ist1)
    {
      for(int i_ist3 = 0; i_ist3 < numOfHits[3]; ++i_ist3)
      {
	double x1_ist = rawHitVec[1][i_ist1]->getColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
	double y1_ist = (63-rawHitVec[1][i_ist1]->getRow())*FST::pitchRow + 0.5*FST::pitchRow;
	pos_ist1_orig.SetXYZ(x1_ist,y1_ist,z1_ist); // set original pos of hits on ist1

	double x1_corr = x1_ist*TMath::Cos(FST::phi_rot) + y1_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
	double y1_corr = y1_ist*TMath::Cos(FST::phi_rot) - x1_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
	pos_ist1.SetXYZ(x1_corr,y1_corr,z1_ist); // set aligned pos of hits on ist1

	double x3_ist = rawHitVec[3][i_ist3]->getColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
	double y3_ist = (63-rawHitVec[3][i_ist3]->getRow())*FST::pitchRow + 0.5*FST::pitchRow;
	pos_ist3_orig.SetXYZ(x3_ist,y3_ist,z3_ist); // set original pos of hits on ist3

	double x3_corr = x3_ist*TMath::Cos(FST::phi_rot) + y3_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
	double y3_corr = y3_ist*TMath::Cos(FST::phi_rot) - x3_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
	pos_ist3.SetXYZ(x3_corr,y3_corr,z3_ist); // set aligned pos of hits on ist3

	double x0_proj = x3_corr + (x1_corr-x3_corr)*z0_fst/z1_ist;
	double y0_proj = y3_corr + (y1_corr-y3_corr)*z0_fst/z1_ist;
	proj_fst.SetXYZ(x0_proj,y0_proj,z0_fst);

	double x2_proj = x3_corr + (x1_corr-x3_corr)*z2_ist/z1_ist;
	double y2_proj = y3_corr + (y1_corr-y3_corr)*z2_ist/z1_ist;
	proj_ist2.SetXYZ(x2_proj,y2_proj,z2_ist);

	if(numOfHits[0] > 0)
	{
	  for(int i_fst = 0; i_fst < numOfHits[0]; ++i_fst)
	  {
	    double r_fst = FST::rOuter + (rawHitVec[0][i_fst]->getColumn()-4)*FST::pitchR + 0.5*FST::pitchR;
	    double phi_fst = (63-rawHitVec[0][i_fst]->getRow())*FST::pitchPhi + 0.5*FST::pitchPhi;
	    double x0_fst = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
	    double y0_fst = r_fst*TMath::Sin(phi_fst); // y = r*sin(phi)
	    pos_fst.SetXYZ(x0_fst,y0_fst,z0_fst); // set aligned pos of hits on fst
	    pos_fst_orig.SetXYZ(x0_fst,y0_fst,z0_fst); // set original pos of hits on fst 

	    if(numOfHits[2] > 0)
	    {
	      for(int i_ist2 = 0; i_ist2 < numOfHits[2]; ++i_ist2)
	      {
		double x2_ist = rawHitVec[2][i_ist2]->getColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
		double y2_ist = (63-rawHitVec[2][i_ist2]->getRow())*FST::pitchRow + 0.5*FST::pitchRow;
		pos_ist2_orig.SetXYZ(x2_ist,y2_ist,z2_ist); // set original pos of hits on ist2

		double x2_corr = x2_ist*TMath::Cos(FST::phi_rot) + y2_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
		double y2_corr = y2_ist*TMath::Cos(FST::phi_rot) - x2_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
		pos_ist2.SetXYZ(x2_corr,y2_corr,z2_ist); // set aligned pos of hits on ist2

		FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
		fstTrack->Clear();
		fstTrack->setTrackId(numOfTracks);
		fstTrack->setTrackType(0);

		// set aligned position
		fstTrack->setId(0,rawHitVec[0][i_fst]->getHitId());
		fstTrack->setPosition(0,pos_fst);
		fstTrack->setId(1,rawHitVec[1][i_ist1]->getHitId());
		fstTrack->setPosition(1,pos_ist1);
		fstTrack->setId(2,rawHitVec[2][i_ist2]->getHitId());
		fstTrack->setPosition(2,pos_ist2);
		fstTrack->setId(3,rawHitVec[3][i_ist3]->getHitId());
		fstTrack->setPosition(3,pos_ist3);

		// set projection
		fstTrack->setProjection(0,proj_fst);
		fstTrack->setProjection(2,proj_ist2);

		// set original position
		fstTrack->setPosOrig(0,pos_fst_orig);
		fstTrack->setPosOrig(1,pos_ist1_orig);
		fstTrack->setPosOrig(2,pos_ist2_orig);
		fstTrack->setPosOrig(3,pos_ist3_orig);

		trackVec.push_back(fstTrack);

		// cout << "numOfTracks = " << numOfTracks << endl;
		// trackVec[numOfTracks]->Print();
		numOfTracks++;
	      }
	    }
	    else
	    {
	      FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
	      fstTrack->Clear();
	      fstTrack->setTrackId(numOfTracks);
	      fstTrack->setTrackType(0);

	      // set position
	      fstTrack->setId(0,rawHitVec[0][i_fst]->getHitId());
	      fstTrack->setPosition(0,pos_fst);
	      fstTrack->setId(1,rawHitVec[1][i_ist1]->getHitId());
	      fstTrack->setPosition(1,pos_ist1);
	      fstTrack->setId(3,rawHitVec[3][i_ist3]->getHitId());
	      fstTrack->setPosition(3,pos_ist3);

	      // set projection
	      fstTrack->setProjection(0,proj_fst);
	      fstTrack->setProjection(2,proj_ist2);

	      // set original position
	      fstTrack->setPosOrig(0,pos_fst_orig);
	      fstTrack->setPosOrig(1,pos_ist1_orig);
	      fstTrack->setPosOrig(3,pos_ist3_orig);

	      trackVec.push_back(fstTrack);

	      // cout << "numOfTracks = " << numOfTracks << endl;
	      // trackVec[numOfTracks]->Print();
	      numOfTracks++;
	    }
	  }
	}
	else
	{
	  if(numOfHits[2] > 0)
	  {
	    for(int i_ist2 = 0; i_ist2 < numOfHits[2]; ++i_ist2)
	    {
	      double x2_ist = rawHitVec[2][i_ist2]->getColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
	      double y2_ist = (63-rawHitVec[2][i_ist2]->getRow())*FST::pitchRow + 0.5*FST::pitchRow;
	      pos_ist2_orig.SetXYZ(x2_ist,y2_ist,z2_ist); // set original pos of hits on ist2

	      double x2_corr = x2_ist*TMath::Cos(FST::phi_rot) + y2_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
	      double y2_corr = y2_ist*TMath::Cos(FST::phi_rot) - x2_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
	      pos_ist2.SetXYZ(x2_corr,y2_corr,z2_ist); // set aligned pos of hits on ist2

	      FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
	      fstTrack->Clear();
	      fstTrack->setTrackId(numOfTracks);
	      fstTrack->setTrackType(0);

	      // set position
	      fstTrack->setId(1,rawHitVec[1][i_ist1]->getHitId());
	      fstTrack->setPosition(1,pos_ist1);
	      fstTrack->setId(2,rawHitVec[2][i_ist2]->getHitId());
	      fstTrack->setPosition(2,pos_ist2);
	      fstTrack->setId(3,rawHitVec[3][i_ist3]->getHitId());
	      fstTrack->setPosition(3,pos_ist3);

	      // set projection
	      fstTrack->setProjection(0,proj_fst);
	      fstTrack->setProjection(2,proj_ist2);

	      // set original position
	      fstTrack->setPosOrig(1,pos_ist1_orig);
	      fstTrack->setPosOrig(2,pos_ist2_orig);
	      fstTrack->setPosOrig(3,pos_ist3_orig);

	      trackVec.push_back(fstTrack);

	      // cout << "numOfTracks = " << numOfTracks << endl;
	      // trackVec[numOfTracks]->Print();
	      numOfTracks++;
	    }
	  }
	  else
	  {
	    FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
	    fstTrack->Clear();
	    fstTrack->setTrackId(numOfTracks);
	    fstTrack->setTrackType(0);

	    // set position
	    fstTrack->setId(1,rawHitVec[1][i_ist1]->getHitId());
	    fstTrack->setPosition(1,pos_ist1);
	    fstTrack->setId(3,rawHitVec[3][i_ist3]->getHitId());
	    fstTrack->setPosition(3,pos_ist3);

	    // set projection
	    fstTrack->setProjection(0,proj_fst);
	    fstTrack->setProjection(2,proj_ist2);

	    // set original position
	    fstTrack->setPosOrig(1,pos_ist1_orig);
	    fstTrack->setPosOrig(3,pos_ist3_orig);

	    trackVec.push_back(fstTrack);

	    // cout << "numOfTracks = " << numOfTracks << endl;
	    // trackVec[numOfTracks]->Print();
	    numOfTracks++;
	  }
	}
      }
    }
  }

  return trackVec;
}
//--------------Find Track with Hits---------------------

//--------------Find Track with Clusters---------------------
std::vector<FstTrack *> FstTracking::findTrack_Clusters(std::vector<FstCluster *> clusterVec_orig)
{
  int numOfClusters[4]; // 0 for fst | 1-3 for ist
  std::vector<FstCluster *> clusterVec[4];
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    numOfClusters[i_layer] = 0;
    clusterVec[i_layer].clear();
  }

  for(int i_cluster = 0; i_cluster < clusterVec_orig.size(); ++i_cluster)
  { // set temp ist cluster container for each layer
    int layer = clusterVec_orig[i_cluster]->getLayer();
    numOfClusters[layer]++;
    clusterVec[layer].push_back(clusterVec_orig[i_cluster]);
  }

  const double z0_fst = FST::pitchLayer03;
  const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
  const double z2_ist = FST::pitchLayer23;
  const double z3_ist = 0.0;

  std::vector<FstTrack *> trackVec;
  trackVec.clear();
  int numOfTracks = 100; // 100 => reconstructed from simple cluster 
  if(numOfClusters[1] > 0 && numOfClusters[3] > 0)
  { // only do tracking when at least 1 hit is found in fst & ist1 & ist3
    TVector3 pos_fst, pos_ist1, pos_ist2, pos_ist3;
    TVector3 pos_fst_orig, pos_ist1_orig, pos_ist2_orig, pos_ist3_orig;
    TVector3 proj_fst, proj_ist2;

    for(int i_ist1 = 0; i_ist1 < numOfClusters[1]; ++i_ist1)
    {
      for(int i_ist3 = 0; i_ist3 < numOfClusters[3]; ++i_ist3)
      {
	double x1_ist = clusterVec[1][i_ist1]->getMeanColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
	double y1_ist = (63-clusterVec[1][i_ist1]->getMeanRow())*FST::pitchRow + 0.5*FST::pitchRow;
	pos_ist1_orig.SetXYZ(x1_ist,y1_ist,z1_ist); // set original pos of hits on ist1

	double x1_corr = x1_ist*TMath::Cos(FST::phi_rot) + y1_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
	double y1_corr = y1_ist*TMath::Cos(FST::phi_rot) - x1_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
	pos_ist1.SetXYZ(x1_corr,y1_corr,z1_ist); // set aligned pos of hits on ist1

	double x3_ist = clusterVec[3][i_ist3]->getMeanColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
	double y3_ist = (63-clusterVec[3][i_ist3]->getMeanRow())*FST::pitchRow + 0.5*FST::pitchRow;
	pos_ist3_orig.SetXYZ(x3_ist,y3_ist,z3_ist); // set original pos of hits on ist3

	double x3_corr = x3_ist*TMath::Cos(FST::phi_rot) + y3_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
	double y3_corr = y3_ist*TMath::Cos(FST::phi_rot) - x3_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
	pos_ist3.SetXYZ(x3_corr,y3_corr,z3_ist); // set aligned pos of hits on ist3

	double x0_proj = x3_corr + (x1_corr-x3_corr)*z0_fst/z1_ist;
	double y0_proj = y3_corr + (y1_corr-y3_corr)*z0_fst/z1_ist;
	proj_fst.SetXYZ(x0_proj,y0_proj,z0_fst);

	double x2_proj = x3_corr + (x1_corr-x3_corr)*z2_ist/z1_ist;
	double y2_proj = y3_corr + (y1_corr-y3_corr)*z2_ist/z1_ist;
	proj_ist2.SetXYZ(x2_proj,y2_proj,z2_ist);

	if(numOfClusters[0] > 0)
	{
	  for(int i_fst = 0; i_fst < numOfClusters[0]; ++i_fst)
	  {
	    double r_fst = FST::rOuter + (clusterVec[0][i_fst]->getMeanColumn()-4)*FST::pitchR + 0.5*FST::pitchR;
	    double phi_fst = (63-clusterVec[0][i_fst]->getMeanRow())*FST::pitchPhi + 0.5*FST::pitchPhi;
	    double x0_fst = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
	    double y0_fst = r_fst*TMath::Sin(phi_fst); // y = r*sin(phi)
	    pos_fst.SetXYZ(x0_fst,y0_fst,z0_fst); // set aligned pos of hits on fst
	    pos_fst_orig.SetXYZ(x0_fst,y0_fst,z0_fst); // set original pos of hits on fst 

	    if(numOfClusters[2] > 0)
	    {
	      for(int i_ist2 = 0; i_ist2 < numOfClusters[2]; ++i_ist2)
	      {
		double x2_ist = clusterVec[2][i_ist2]->getMeanColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
		double y2_ist = (63-clusterVec[2][i_ist2]->getMeanRow())*FST::pitchRow + 0.5*FST::pitchRow;
		pos_ist2_orig.SetXYZ(x2_ist,y2_ist,z2_ist); // set original pos of hits on ist2

		double x2_corr = x2_ist*TMath::Cos(FST::phi_rot) + y2_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
		double y2_corr = y2_ist*TMath::Cos(FST::phi_rot) - x2_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
		pos_ist2.SetXYZ(x2_corr,y2_corr,z2_ist); // set aligned pos of hits on ist2

		FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
		fstTrack->Clear();
		fstTrack->setTrackId(numOfTracks);
		fstTrack->setTrackType(1);

		// set aligned position
		fstTrack->setId(0,clusterVec[0][i_fst]->getClusterId());
		fstTrack->setPosition(0,pos_fst);
		fstTrack->setId(1,clusterVec[1][i_ist1]->getClusterId());
		fstTrack->setPosition(1,pos_ist1);
		fstTrack->setId(2,clusterVec[2][i_ist2]->getClusterId());
		fstTrack->setPosition(2,pos_ist2);
		fstTrack->setId(3,clusterVec[3][i_ist3]->getClusterId());
		fstTrack->setPosition(3,pos_ist3);

		// set projection
		fstTrack->setProjection(0,proj_fst);
		fstTrack->setProjection(2,proj_ist2);

		// set original position
		fstTrack->setPosOrig(0,pos_fst_orig);
		fstTrack->setPosOrig(1,pos_ist1_orig);
		fstTrack->setPosOrig(2,pos_ist2_orig);
		fstTrack->setPosOrig(3,pos_ist3_orig);

		trackVec.push_back(fstTrack);

		// cout << "numOfTracks = " << numOfTracks << endl;
		// trackVec[numOfTracks]->Print();
		numOfTracks++;
	      }
	    }
	    else
	    {
	      FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
	      fstTrack->Clear();
	      fstTrack->setTrackId(numOfTracks);
	      fstTrack->setTrackType(1);

	      // set position
	      fstTrack->setId(0,clusterVec[0][i_fst]->getClusterId());
	      fstTrack->setPosition(0,pos_fst);
	      fstTrack->setId(1,clusterVec[1][i_ist1]->getClusterId());
	      fstTrack->setPosition(1,pos_ist1);
	      fstTrack->setId(3,clusterVec[3][i_ist3]->getClusterId());
	      fstTrack->setPosition(3,pos_ist3);

	      // set projection
	      fstTrack->setProjection(0,proj_fst);
	      fstTrack->setProjection(2,proj_ist2);

	      // set original position
	      fstTrack->setPosOrig(0,pos_fst_orig);
	      fstTrack->setPosOrig(1,pos_ist1_orig);
	      fstTrack->setPosOrig(3,pos_ist3_orig);

	      trackVec.push_back(fstTrack);

	      // cout << "numOfTracks = " << numOfTracks << endl;
	      // trackVec[numOfTracks]->Print();
	      numOfTracks++;
	    }
	  }
	}
	else
	{
	  if(numOfClusters[2] > 0)
	  {
	    for(int i_ist2 = 0; i_ist2 < numOfClusters[2]; ++i_ist2)
	    {
	      double x2_ist = clusterVec[2][i_ist2]->getMeanColumn()*FST::pitchColumn + 0.5*FST::pitchColumn;
	      double y2_ist = (63-clusterVec[2][i_ist2]->getMeanRow())*FST::pitchRow + 0.5*FST::pitchRow;
	      pos_ist2_orig.SetXYZ(x2_ist,y2_ist,z2_ist); // set original pos of hits on ist2

	      double x2_corr = x2_ist*TMath::Cos(FST::phi_rot) + y2_ist*TMath::Sin(FST::phi_rot) + FST::x_shift;
	      double y2_corr = y2_ist*TMath::Cos(FST::phi_rot) - x2_ist*TMath::Sin(FST::phi_rot) + FST::y_shift;
	      pos_ist2.SetXYZ(x2_corr,y2_corr,z2_ist); // set aligned pos of hits on ist2

	      FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
	      fstTrack->Clear();
	      fstTrack->setTrackId(numOfTracks);
	      fstTrack->setTrackType(1);

	      // set position
	      fstTrack->setId(1,clusterVec[1][i_ist1]->getClusterId());
	      fstTrack->setPosition(1,pos_ist1);
	      fstTrack->setId(2,clusterVec[2][i_ist2]->getClusterId());
	      fstTrack->setPosition(2,pos_ist2);
	      fstTrack->setId(3,clusterVec[3][i_ist3]->getClusterId());
	      fstTrack->setPosition(3,pos_ist3);

	      // set projection
	      fstTrack->setProjection(0,proj_fst);
	      fstTrack->setProjection(2,proj_ist2);

	      // set original position
	      fstTrack->setPosOrig(1,pos_ist1_orig);
	      fstTrack->setPosOrig(2,pos_ist2_orig);
	      fstTrack->setPosOrig(3,pos_ist3_orig);

	      trackVec.push_back(fstTrack);

	      // cout << "numOfTracks = " << numOfTracks << endl;
	      // trackVec[numOfTracks]->Print();
	      numOfTracks++;
	    }
	  }
	  else
	  {
	    FstTrack *fstTrack = new FstTrack(); // generate a FstTrack when both ist1 and ist3 have a hit
	    fstTrack->Clear();
	    fstTrack->setTrackId(numOfTracks);
	    fstTrack->setTrackType(1);

	    // set position
	    fstTrack->setId(1,clusterVec[1][i_ist1]->getClusterId());
	    fstTrack->setPosition(1,pos_ist1);
	    fstTrack->setId(3,clusterVec[3][i_ist3]->getClusterId());
	    fstTrack->setPosition(3,pos_ist3);

	    // set projection
	    fstTrack->setProjection(0,proj_fst);
	    fstTrack->setProjection(2,proj_ist2);

	    // set original position
	    fstTrack->setPosOrig(1,pos_ist1_orig);
	    fstTrack->setPosOrig(3,pos_ist3_orig);

	    trackVec.push_back(fstTrack);

	    // cout << "numOfTracks = " << numOfTracks << endl;
	    // trackVec[numOfTracks]->Print();
	    numOfTracks++;
	  }
	}
      }
    }
  }

  return trackVec;
}
//--------------Find Track with Clusters---------------------

//--------------Output TTree---------------------
bool FstTracking::initTree()
{
  mTree_FstEvent_OutPut = new TTree("mTree_FstEvent","Fst Hits and Clusters Info");
  mFstEvent_OutPut = new FstEvent();
  mTree_FstEvent_OutPut->Branch("FstEvent","FstEvent",&mFstEvent_OutPut);
  mTree_FstEvent_OutPut->SetAutoSave(50000000);

  return true;
}

void FstTracking::writeTree()
{
  mTree_FstEvent_OutPut->Write();
}
//--------------Output TTree---------------------

