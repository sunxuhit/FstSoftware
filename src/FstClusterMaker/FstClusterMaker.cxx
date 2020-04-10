#include "./FstClusterMaker.h"
#include "../FstUtil/FstRawHit.h"
#include "../FstUtil/FstCluster.h"
#include "../FstUtil/FstTrack.h"
#include "../FstUtil/FstEvent.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TVector3.h>

using namespace std;

ClassImp(FstClusterMaker)

//------------------------------------------

FstClusterMaker::FstClusterMaker() : mList("../../list/FST/FstPed_HV70.list"), mOutPutFile("./FstPed_HV70.root"), mSavePed(false)
{
  cout << "FstClusterMaker::FstClusterMaker() -------- Constructor!  --------" << endl;
  mHome = getenv("HOME");
}

FstClusterMaker::~FstClusterMaker()
{
  cout << "FstClusterMaker::~FstClusterMaker() -------- Release Memory!  --------" << endl;
}

//------------------------------------------
int FstClusterMaker::Init()
{
  cout << "FstClusterMaker::Init => " << endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  bool isInPut = initChain(); // initialize input data/ped TChain;
  bool isPed = initPedestal(); // initialize pedestal array;
  bool isSig = initSignal(); // initialize signal array;
  bool isTree = initTree();

  if(!isInPut) 
  {
    cout << "Failed to initialize input data!" << endl;
    return -1;
  }
  if(!isPed) 
  {
    cout << "Failed to initialize pedestals!" << endl;
    return -1;
  }
  if(!isSig) 
  {
    cout << "Failed to initialize signal!" << endl;
    return -1;
  }
  if(!isTree) 
  {
    cout << "Failed to initialize TTree for FST & IST Hits & Clusters!" << endl;
    return -1;
  }

  initHitDisplay(); // initialize hit display

  return 1;
}

bool FstClusterMaker::initChain()
{
  cout << "FstClusterMaker::initChain -> " << endl;

  mChainInPut = new TChain("tree_Hits");

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
	  mChainInPut->AddFile(addfile.c_str(),-1,"tree_Hits");
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

  mChainInPut->SetBranchAddress("evt_rdo",evt_rdo);
  mChainInPut->SetBranchAddress("evt_arm",evt_arm);
  mChainInPut->SetBranchAddress("evt_port",evt_port);
  mChainInPut->SetBranchAddress("evt_apv",evt_apv);
  mChainInPut->SetBranchAddress("hit_ch",hit_ch);
  mChainInPut->SetBranchAddress("hit_tb",hit_tb);
  mChainInPut->SetBranchAddress("hit_adc",hit_adc);

  long NumOfEvents = (long)mChainInPut->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;

  return true;
}

bool FstClusterMaker::initSignal()
{
  cout << "FstClusterMaker::initSignal -> " << endl;
  return clearSignal();
}

//------------------------------------------
bool FstClusterMaker::clearSignal()
{
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	    mRawSig[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	  }
	}
      }
    }
  }

  return true;
}

//------------------------------------------
int FstClusterMaker::Make()
{
  cout << "FstClusterMaker::Make => " << endl;

  bool isPed = calPedestal(); // calculate pedestals with first 1000 events
  if( !isPed ) 
  {
    cout << "No Pedestal!!!" << endl;
    return -1;
  }

  long NumOfEvents = (long)mChainInPut->GetEntries();
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 10000;
  mChainInPut->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);
    clearSignal();

    // calculate ped corrected signal for each ch & time bin
    // ped is calcualte from pedTimeBin = 0
    for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < FST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
	{
	  int rdo  = evt_rdo[i_arm][i_port][i_apv];
	  int arm  = evt_arm[i_arm][i_port][i_apv];
	  int port = evt_port[i_arm][i_port][i_apv];
	  int apv  = evt_apv[i_arm][i_port][i_apv];

	  // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
	  bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < FST::numAPVs ) ) ;
	  bool bAPV = isBadAPV(arm,port,apv);
	  if(pass && !bAPV)
	  {
	    for(int i_ro = 0; i_ro < FST::numROChannels; ++i_ro)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ro]; // time bin
	      int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
	      int adc = hit_adc[i_arm][i_port][i_apv][i_ro];

	      mSigPedCorr[i_arm][i_port][i_apv][ch][tb] = adc-mPed[i_arm][i_port][i_apv][ch][tb]; // adc - ped
	      mRawSig[i_arm][i_port][i_apv][ch][tb] = adc;
	    }
	  }
	}
      }
    }

    // find Hits
    std::vector<FstRawHit *> rawHitsVec_temp;
    rawHitsVec_temp.clear();
    int numOfHits = 0;
    for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < FST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
	{
	  for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	  {
	    if( // some hit quality cuts => 1st one is questionable | 2nd made sure ch shows reasonable noise
		( mSigPedCorr[i_arm][i_port][i_apv][i_ch][0] < mSigPedCorr[i_arm][i_port][i_apv][i_ch][3] ) &&
		( mPedStdDev[i_arm][i_port][i_apv][i_ch][FST::pedTimeBin] > FST::MinNoise)  
	      )
	    {
	      double maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][0]; // init with 1st tb
	      int maxTB = 0;
	      double preADC = maxADC;
	      bool isHit = false;
	      float nHitsCut = FST::nIstHitsCut; // 6.0 for IST
	      if(i_arm == 1 && i_port == 1) nHitsCut = FST::nFstHitsCut; // 4.5 for FST
	      for(int i_tb = 1; i_tb < FST::numTBins-1; ++i_tb)
	      { // only if 3 consequetive timebins of a ch exceed the threshold cut is considered as a hit
		if( 
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb-1] > nHitsCut*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb-1]) &&
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > nHitsCut*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]) &&
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > nHitsCut*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb+1])
		  ) 
		{
		  isHit = true; // set isHit to true if 3 consequetive time bins exceed the threshold
		  if(mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > preADC)
		  { // find time bin with max adc for 0-FST::numTBins-2
		    maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb];
		    maxTB = i_tb;
		    preADC = maxADC;
		  }
		  if(i_tb == FST::numTBins-2 && mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > preADC)
		  { // check if last time bin has the max ADC
		    maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1];
		    maxTB = i_tb+1;
		  }
		}
	      }

	      bool isPed = false;
	      if( !isHit && mSavePed)
	      {
		maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][0]; // init with 1st tb
		maxTB = 0;
		preADC = maxADC;
		// float nPedsCut = FST::nIstPedsCut; // 3.0 for IST
		float nPedsCut = FST::nIstHitsCut; // 6.0 for IST => only save hits for IST
		if(i_arm == 1 && i_port == 1) nPedsCut = FST::nFstPedsCut; // 3.0 for FST
		for(int i_tb = 1; i_tb < FST::numTBins-1; ++i_tb)
		{ // only if 3 consequetive timebins of a ch exceed the threshold cut is considered as a hit
		  if( 
		      ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb-1] > nPedsCut*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb-1]) &&
		      ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > nPedsCut*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]) &&
		      ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > nPedsCut*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb+1])
		    ) 
		  {
		    isPed = true; // set isHit to true if 3 consequetive time bins exceed the threshold
		    if(mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > preADC)
		    { // find time bin with max adc for 0-FST::numTBins-2
		      maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb];
		      maxTB = i_tb;
		      preADC = maxADC;
		    }
		    if(i_tb == FST::numTBins-2 && mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > preADC)
		    { // check if last time bin has the max ADC
		      maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1];
		      maxTB = i_tb+1;
		    }
		  }
		}
	      }


	      if( (isHit || isPed) && numOfHits < FST::maxNHits)
	      { // set Hit info
		FstRawHit *fstRawHit = new FstRawHit();
		fstRawHit->Clear();
		fstRawHit->setLayer(getLayer(i_arm,i_port));
		fstRawHit->setSensor(getSensor(i_arm,i_port,i_apv));
		fstRawHit->setAPV(i_apv);
		fstRawHit->setChannel(i_ch);
		fstRawHit->setColumn(getColumn(i_arm,i_port,i_apv,i_ch));
		fstRawHit->setRow(getRow(i_arm,i_port,i_apv,i_ch));
		fstRawHit->setPosX(getPosX(i_arm,i_port,i_apv,i_ch));
		fstRawHit->setPosY(getPosY(i_arm,i_port,i_apv,i_ch));
		for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
		{
		  fstRawHit->setPedMean(mPed[i_arm][i_port][i_apv][i_ch][i_tb], i_tb);
		  fstRawHit->setPedStdDev(mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb], i_tb);
		  fstRawHit->setPedRMS(mPedRMS[i_arm][i_port][i_apv][i_ch][i_tb], i_tb);
		  fstRawHit->setCharge(mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb], i_tb);
		}
		// fstRawHit->setCharge(maxADC, maxTB);
		fstRawHit->setMaxTb(maxTB);
		fstRawHit->setHitId(numOfHits);
		fstRawHit->setDefaultTb(FST::defaultTimeBin);
		fstRawHit->setIsHit(isHit && !isPed);
		rawHitsVec_temp.push_back(fstRawHit);

		numOfHits++;
	      }
	    }
	  }
	}
      }
    }

    if(numOfHits > 0 && numOfHits <= FST::maxNHitsPerEvent) // maximum hits to expect per event is 10
    {
      fillHitDisplay(rawHitsVec_temp); // fill hit display

      // set up FstRawHit
      mFstEvent->clearRawHitsList();
      int nHits = rawHitsVec_temp.size();
      int numOfFstHits = 0;
      for(int i_hit = 0; i_hit < nHits; ++i_hit)
      {
	mFstRawHit = mFstEvent->createRawHit();
	mFstRawHit->setLayer(rawHitsVec_temp[i_hit]->getLayer());
	mFstRawHit->setSensor(rawHitsVec_temp[i_hit]->getSensor());
	mFstRawHit->setAPV(rawHitsVec_temp[i_hit]->getAPV());
	mFstRawHit->setChannel(rawHitsVec_temp[i_hit]->getChannel());
	mFstRawHit->setColumn(rawHitsVec_temp[i_hit]->getColumn());
	mFstRawHit->setRow(rawHitsVec_temp[i_hit]->getRow());
	mFstRawHit->setPosX(rawHitsVec_temp[i_hit]->getPosX());
	mFstRawHit->setPosY(rawHitsVec_temp[i_hit]->getPosY());
	for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	{
	  mFstRawHit->setPedMean(rawHitsVec_temp[i_hit]->getPedMean(i_tb), i_tb);
	  mFstRawHit->setPedStdDev(rawHitsVec_temp[i_hit]->getPedStdDev(i_tb), i_tb);
	  mFstRawHit->setPedRMS(rawHitsVec_temp[i_hit]->getPedRMS(i_tb), i_tb);
	  mFstRawHit->setCharge(rawHitsVec_temp[i_hit]->getCharge(i_tb), i_tb);
	}
	// mFstRawHit->setCharge(rawHitsVec_temp[i_hit]->getCharge(rawHitsVec_temp[i_hit]->getMaxTb()),rawHitsVec_temp[i_hit]->getMaxTb());
	mFstRawHit->setMaxTb(rawHitsVec_temp[i_hit]->getMaxTb());
	mFstRawHit->setHitId(rawHitsVec_temp[i_hit]->getHitId());
	mFstRawHit->setDefaultTb(rawHitsVec_temp[i_hit]->getDefaultTb());
	mFstRawHit->setIsHit(rawHitsVec_temp[i_hit]->getIsHit());
	if(mFstRawHit->getLayer() == 0) numOfFstHits++;
      }
      mFstEvent->setNumFstRawHits(numOfFstHits);

      // set up FstCluster
      mFstEvent->clearClustersList();
      std::vector<FstCluster *> cluster_simple = findCluster_Simple(rawHitsVec_temp);
      int nClusters = cluster_simple.size();
      int numOfFstClusters = 0;
      for(int i_cluster = 0; i_cluster < nClusters; ++i_cluster)
      {
	cluster_simple[i_cluster]->setClusterId(i_cluster);

	mFstCluster = mFstEvent->createCluster();
	mFstCluster->setLayer(cluster_simple[i_cluster]->getLayer());
	mFstCluster->setSensor(cluster_simple[i_cluster]->getSensor());
	mFstCluster->setMeanColumn(cluster_simple[i_cluster]->getMeanColumn());
	mFstCluster->setMeanRow(cluster_simple[i_cluster]->getMeanRow());
	mFstCluster->setTotCharge(cluster_simple[i_cluster]->getTotCharge());
	mFstCluster->setMaxTb(cluster_simple[i_cluster]->getMaxTb());
	mFstCluster->setClusterType(cluster_simple[i_cluster]->getClusterType());
	mFstCluster->setNRawHits(cluster_simple[i_cluster]->getNRawHits());
	mFstCluster->setNRawHitsR(cluster_simple[i_cluster]->getNRawHitsR());
	mFstCluster->setNRawHitsPhi(cluster_simple[i_cluster]->getNRawHitsPhi());
	mFstCluster->setClusterId(cluster_simple[i_cluster]->getClusterId());
	
	std::vector<FstRawHit *> rawHitsVec = cluster_simple[i_cluster]->getRawHitVec(); // get raw hits vec from cluster finder
	mFstCluster->clearRawHitsList(); // set up FstRawHit in the FstCluster
	for(int i_hit = 0; i_hit < rawHitsVec.size(); ++i_hit)
	{
	  // mFstCluster->setHitId(i_hit, rawHitsVec[i_hit]->getHitId());
	  mFstClusteredRawHit = mFstCluster->createRawHit();
	  mFstClusteredRawHit->setLayer(rawHitsVec[i_hit]->getLayer());
	  mFstClusteredRawHit->setSensor(rawHitsVec[i_hit]->getSensor());
	  mFstClusteredRawHit->setAPV(rawHitsVec[i_hit]->getAPV());
	  mFstClusteredRawHit->setChannel(rawHitsVec[i_hit]->getChannel());
	  mFstClusteredRawHit->setColumn(rawHitsVec[i_hit]->getColumn());
	  mFstClusteredRawHit->setRow(rawHitsVec[i_hit]->getRow());
	  mFstClusteredRawHit->setPosX(rawHitsVec[i_hit]->getPosX());
	  mFstClusteredRawHit->setPosY(rawHitsVec[i_hit]->getPosY());
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    mFstClusteredRawHit->setPedMean(rawHitsVec[i_hit]->getPedMean(i_tb), i_tb);
	    mFstClusteredRawHit->setPedStdDev(rawHitsVec[i_hit]->getPedStdDev(i_tb), i_tb);
	    mFstClusteredRawHit->setPedRMS(rawHitsVec[i_hit]->getPedRMS(i_tb), i_tb);
	    mFstClusteredRawHit->setCharge(rawHitsVec[i_hit]->getCharge(i_tb), i_tb);
	  }
	  // mFstRawHit->setCharge(rawHitsVec_temp[i_hit]->getCharge(rawHitsVec_temp[i_hit]->getMaxTb()),rawHitsVec_temp[i_hit]->getMaxTb());
	  mFstClusteredRawHit->setMaxTb(rawHitsVec[i_hit]->getMaxTb());
	  mFstClusteredRawHit->setHitId(rawHitsVec[i_hit]->getHitId());
	  mFstClusteredRawHit->setDefaultTb(rawHitsVec[i_hit]->getDefaultTb());
	  mFstClusteredRawHit->setIsHit(rawHitsVec[i_hit]->getIsHit());
	}
	if(mFstCluster->getLayer() == 0) numOfFstClusters++;
      }
      mFstEvent->setNumFstClusters(numOfFstClusters);

      // set up FstTrack
      mFstEvent->clearTracksList(); // FstTrack
      std::vector<FstTrack *> fstTrackVec_Hits = findTrack_Hits(rawHitsVec_temp); // find tracks with Hits
      for(int i_track = 0; i_track < fstTrackVec_Hits.size(); ++i_track)
      { // get track from hits
	// if(mFstEvent->getNumTracks() > 30) continue; // only save first 30 tracks | will drop the event with more than 10 trakcs anyway
	mFstTrack = mFstEvent->createTrack();
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
      std::vector<FstTrack *> fstTrackVec_Clusters = findTrack_Clusters(cluster_simple); // find tracks with Clusters
      for(int i_track = 0; i_track < fstTrackVec_Clusters.size(); ++i_track)
      { // get track from clusters
	mFstTrack = mFstEvent->createTrack();
	// if(mFstEvent->getNumTracks() > 30) continue; // only save first 30 tracks | will drop the event with more than 10 trakcs anyway
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

      mTree_FstEvent->Fill();
    }
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 1;
}

int FstClusterMaker::Finish()
{
  cout << "FstClusterMaker::Finish => " << endl;
  writePedestal();
  writeHitDisplay();
  writeTree();

  return 1;
}

//--------------pedestal---------------------
bool FstClusterMaker::clearPedestal()
{
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    mPed[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	    mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	    mPedRMS[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	  }
	}
      }
    }
  }

  return true;
}

bool FstClusterMaker::initPedestal()
{
  cout << "FstClusterMaker::initPedestal -> " << endl;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      std::string gName = Form("g_mPedMean_Layer%d_TimeBin%d",i_layer,i_tb);
      g_mPedMean[i_layer][i_tb] = new TGraph();
      g_mPedMean[i_layer][i_tb]->SetName(gName.c_str());

      gName = Form("g_mPedSigma_Layer%d_TimeBin%d",i_layer,i_tb);
      g_mPedSigma[i_layer][i_tb] = new TGraph();
      g_mPedSigma[i_layer][i_tb]->SetName(gName.c_str());

      std::string HistName = Form("h_mPedDisplay_Layer%d_TimeBin%d",i_layer,i_tb);
      if(i_layer == 0) h_mPedDisplay[i_layer][i_tb] = new TH2F(HistName.c_str(),HistName.c_str(),FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
      else h_mPedDisplay[i_layer][i_tb] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::noRows,-0.5,FST::noRows-0.5);
    }
  }

  return clearPedestal();
}

bool FstClusterMaker::calPedestal()
{
  cout << "FstClusterMaker::calPedestal -> " << endl;
  cout << " Only use first 1000 event for Pedstal Calculation!" << endl;

  int NumOfEvents = (int)mChainInPut->GetEntries();
  if(NumOfEvents > 1000) NumOfEvents = 1000;
  // const int NumOfEvents = 1000;
  mChainInPut->GetEntry(0);

  //  Calculate a rolling average and standard deviation
  int counters[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins];
  double sumValues[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins];
  double sumValuesSquared[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins];

  //--------------------------------------------------------
  // 1st loop to get a rough estimate of ped (including Hits)
  cout << "=====>First Pedestal Pass...." << endl;
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    counters[i_arm][i_port][i_apv][i_ch][i_tb] = 0;
	    sumValues[i_arm][i_port][i_apv][i_ch][i_tb] = 0.0;
	    sumValuesSquared[i_arm][i_port][i_apv][i_ch][i_tb] = 0.0;
	  }
	}
      }
    }
  }

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  { 
    // if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < FST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
	{
	  int rdo  = evt_rdo[i_arm][i_port][i_apv];
	  int arm  = evt_arm[i_arm][i_port][i_apv];
	  int port = evt_port[i_arm][i_port][i_apv];
	  int apv  = evt_apv[i_arm][i_port][i_apv];

	  // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
	  bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < FST::numAPVs ) ) ;
	  bool bAPV = isBadAPV(arm,port,apv);
	  if(pass && !bAPV)
	  {
	    for(int i_ro = 0; i_ro < FST::numROChannels; ++i_ro)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ro]; // time bin
	      // if(tb == FST::pedTimeBin)
	      // {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ro];
		sumValues[arm][port][apv][ch][tb] += adc;
		sumValuesSquared[arm][port][apv][ch][tb] += adc * adc;
		counters[arm][port][apv][ch][tb]++;
	      // }
	    }
	  }
	}
      }
    }
  }

  // rough estimate of ped (including Hits)
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    if(counters[i_arm][i_port][i_apv][i_ch][i_tb] > 0) // eject bad channels
	    {
	      mPed[i_arm][i_port][i_apv][i_ch][i_tb] = sumValues[i_arm][i_port][i_apv][i_ch][i_tb]/counters[i_arm][i_port][i_apv][i_ch][i_tb];
	      mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch][i_tb]-(double)counters[i_arm][i_port][i_apv][i_ch][i_tb]*mPed[i_arm][i_port][i_apv][i_ch][i_tb]*mPed[i_arm][i_port][i_apv][i_ch][i_tb])/(double)(counters[i_arm][i_port][i_apv][i_ch][i_tb]-1));
	      mPedRMS[i_arm][i_port][i_apv][i_ch][i_tb] = sqrt(mPed[i_arm][i_port][i_apv][i_ch][i_tb]*mPed[i_arm][i_port][i_apv][i_ch][i_tb]+mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]); // rms^2 = mean^2+std^2
	    }
	  }
	}
      }
    }
  }
  //--------------------------------------------------------

  //--------------------------------------------------------
  // 2nd loop to find ped excluding Hits
  cout << "=====>Second Pedestal Pass...." << endl;
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    counters[i_arm][i_port][i_apv][i_ch][i_tb] = 0;
	    sumValues[i_arm][i_port][i_apv][i_ch][i_tb] = 0.0;
	    sumValuesSquared[i_arm][i_port][i_apv][i_ch][i_tb] = 0.0;
	  }
	}
      }
    }
  }
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  { 
    // if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < FST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
	{
	  int rdo  = evt_rdo[i_arm][i_port][i_apv];
	  int arm  = evt_arm[i_arm][i_port][i_apv];
	  int port = evt_port[i_arm][i_port][i_apv];
	  int apv  = evt_apv[i_arm][i_port][i_apv];

	  // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
	  bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < FST::numAPVs ) ) ;
	  bool bAPV = isBadAPV(arm,port,apv);
	  if(pass && !bAPV)
	  {
	    for(int i_ro = 0; i_ro < FST::numROChannels; ++i_ro)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ro]; // time bin
	      // if(tb == FST::pedTimeBin)
	      // {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ro];
		float nPedsCut = FST::nIstPedsCut; // 3.0 for IST
		if(i_arm == 1 && i_port == 1) nPedsCut = FST::nFstPedsCut; // 3.0 for FST
		if ( (adc < mPed[arm][port][apv][ch][tb]+nPedsCut*mPedStdDev[arm][port][apv][ch][tb]) && ( adc >= 0 && adc < 4096) )
		{ // only adc belew ped+3sigma are considered for 2nd loop
		  sumValues[arm][port][apv][ch][tb] += adc;
		  sumValuesSquared[arm][port][apv][ch][tb] += adc * adc;
		  counters[arm][port][apv][ch][tb]++;
		}
	      // }
	    }
	  }
	}
      }
    }
  }
  // calculate ped excluding Hits
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    if(counters[i_arm][i_port][i_apv][i_ch][i_tb] > 0) // eject bad channels
	    {
	      mPed[i_arm][i_port][i_apv][i_ch][i_tb] = sumValues[i_arm][i_port][i_apv][i_ch][i_tb]/counters[i_arm][i_port][i_apv][i_ch][i_tb];
	      mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch][i_tb]-(double)counters[i_arm][i_port][i_apv][i_ch][i_tb]*mPed[i_arm][i_port][i_apv][i_ch][i_tb]*mPed[i_arm][i_port][i_apv][i_ch][i_tb])/(double)(counters[i_arm][i_port][i_apv][i_ch][i_tb]-1));
	      mPedRMS[i_arm][i_port][i_apv][i_ch][i_tb] = sqrt(mPed[i_arm][i_port][i_apv][i_ch][i_tb]*mPed[i_arm][i_port][i_apv][i_ch][i_tb]+mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]*mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]); // rms^2 = mean^2+std^2
	    }

	    int layer = this->getLayer(i_arm,i_port);
	    int col = this->getColumn(i_arm,i_port,i_apv,i_ch);
	    int row = this->getRow(i_arm,i_port,i_apv,i_ch);

	    g_mPedMean[layer][i_tb]->SetPoint(i_apv*FST::numChannels+i_ch,i_apv*FST::numChannels+i_ch,mPed[i_arm][i_port][i_apv][i_ch][i_tb]);
	    g_mPedSigma[layer][i_tb]->SetPoint(i_apv*FST::numChannels+i_ch,i_apv*FST::numChannels+i_ch,mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]);
	    h_mPedDisplay[layer][i_tb]->Fill(col,row,mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]);
	  }
	}
      }
    }
  }
  //--------------------------------------------------------

  return true;
}

void FstClusterMaker::writePedestal()
{
  cout << "FstClusterMaker::writePedestal => save ped mean & sigma!" << endl;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      g_mPedMean[i_layer][i_tb]->Write();
      g_mPedSigma[i_layer][i_tb]->Write();
      h_mPedDisplay[i_layer][i_tb]->Write();
    }
  }
}
//--------------pedestal---------------------

//--------------hit display---------------------
bool FstClusterMaker::initHitDisplay()
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

void FstClusterMaker::fillHitDisplay(std::vector<FstRawHit *> rawHitsVec)
{
  if(rawHitsVec.size() < FST::maxNHits)
  {
    for(int i_hit = 0; i_hit < rawHitsVec.size(); ++i_hit)
    {
      int layer = rawHitsVec[i_hit]->getLayer();
      h_mHitDisplay[layer]->Fill(rawHitsVec[i_hit]->getColumn(),rawHitsVec[i_hit]->getRow());
      h_mMaxTb[layer]->Fill(rawHitsVec[i_hit]->getMaxTb());
    }
  }
}

void FstClusterMaker::writeHitDisplay()
{
  cout << "FstClusterMaker::writeHitDisplay => save Hits at each Layer!" << endl;
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mHitDisplay[i_layer]->Write();
    h_mMaxTb[i_layer]->Write();
  }
}
//--------------hit display---------------------

//--------------cluster with Simple Algorithm---------------------
std::vector<FstCluster *> FstClusterMaker::findCluster_Simple(std::vector<FstRawHit *> rawHitsVec_orig)
{
  double meanRow = 0., meanColumn = 0.;
  double totAdc = 0.;
  int nRawHits = 1, nRawHitsR = 1, nRawHitsPhi = 1;

  int numOfHits = rawHitsVec_orig.size();
  std::vector<FstRawHit *> rawHitsVec;
  rawHitsVec.clear();
  rawHitsVec.reserve(rawHitsVec_orig.size());
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  { // set temp ist hit container
    rawHitsVec.push_back(rawHitsVec_orig[i_hit]);
  }

  // find cluster
  std::vector<FstCluster *> clustersVec_Simple;
  clustersVec_Simple.clear();
  if(rawHitsVec.size() > 0)
  {
    // set 1st cluster to 1st hit
    std::vector<FstRawHit *>::iterator rawHitsIt = rawHitsVec.begin();

    FstCluster *fstCluster_tmp = new FstCluster();
    fstCluster_tmp->Clear();
    fstCluster_tmp->setLayer((*rawHitsIt)->getLayer());
    fstCluster_tmp->setSensor((*rawHitsIt)->getSensor());
    fstCluster_tmp->setMeanColumn((*rawHitsIt)->getColumn());
    fstCluster_tmp->setMeanRow((*rawHitsIt)->getRow());
    fstCluster_tmp->setTotCharge((*rawHitsIt)->getCharge((*rawHitsIt)->getMaxTb()));
    fstCluster_tmp->setMaxTb((*rawHitsIt)->getMaxTb());
    fstCluster_tmp->setClusterType(1);
    fstCluster_tmp->setNRawHits(nRawHits);
    fstCluster_tmp->setNRawHitsR(nRawHitsR);
    fstCluster_tmp->setNRawHitsPhi(nRawHitsPhi);
    fstCluster_tmp->addRawHit((*rawHitsIt)); // save hits for ith cluster

    clustersVec_Simple.push_back(fstCluster_tmp);

    rawHitsVec.erase(rawHitsIt); // remove 1st hit from ist hit container

    if( rawHitsVec.size() !=0 )
    {
      double weight, tempSumAdc;
      std::vector<FstCluster *>::iterator clusterIt = clustersVec_Simple.begin(); // get the begin of cluster container

      while( clusterIt != clustersVec_Simple.end() && !rawHitsVec.empty() )
      { //loop the existed clusters vector constainer
	std::vector<FstRawHit *> clusteredHitsVec = (*clusterIt)->getRawHitVec(); // get hits stored in current cluster
	for(std::vector<FstRawHit *>::iterator clusteredHitsIt = clusteredHitsVec.begin(); clusteredHitsIt != clusteredHitsVec.end(); clusteredHitsIt++) 
	{ // loop over the hit belong to ith cluster
	  rawHitsIt = rawHitsVec.begin(); // get hit from the rest of rawHitsVec

	  while( rawHitsIt != rawHitsVec.end() ) 
	  { // loop over the rest of hits in rawHitsVec
	    if( ( ((*rawHitsIt)->getLayer() == (*clusteredHitsIt)->getLayer()) && ((*rawHitsIt)->getSensor() == (*clusteredHitsIt)->getSensor()) && ((*rawHitsIt)->getRow() == (*clusteredHitsIt)->getRow()) && ( ((*rawHitsIt)->getColumn() == (*clusteredHitsIt)->getColumn() + 1) || ((*rawHitsIt)->getColumn() == (*clusteredHitsIt)->getColumn() - 1) )) ||
		( ((*rawHitsIt)->getLayer() == (*clusteredHitsIt)->getLayer()) && ((*rawHitsIt)->getSensor() == (*clusteredHitsIt)->getSensor()) && ((*rawHitsIt)->getColumn() == (*clusteredHitsIt)->getColumn()) && ( ((*rawHitsIt)->getRow() == (*clusteredHitsIt)->getRow() + 1) || ((*rawHitsIt)->getRow() == (*clusteredHitsIt)->getRow() - 1) ))   ) 
	    {
	      nRawHits = (*clusterIt)->getNRawHits() + 1;
	      if( (*rawHitsIt)->getRow() == (*clusteredHitsIt)->getRow() ) nRawHitsR = (*clusterIt)->getNRawHitsR() + 1; //same phi
	      if( (*rawHitsIt)->getColumn() == (*clusteredHitsIt)->getColumn() ) nRawHitsPhi = (*clusterIt)->getNRawHitsPhi() + 1; //same R

	      int maxTb_temp = (*rawHitsIt)->getMaxTb();
	      double currentAdc = (*rawHitsIt)->getCharge(maxTb_temp);
	      tempSumAdc = (*clusterIt)->getTotCharge() + currentAdc;
	      weight = currentAdc/tempSumAdc;

	      int layer_temp        = (*clusterIt)->getLayer();
	      int sensor_temp        = (*clusterIt)->getSensor();
	      double meanColumn_temp = (1.0 - weight) * (*clusterIt)->getMeanColumn() + weight * (*rawHitsIt)->getColumn();
	      double meanRow_temp    = (1.0 - weight) * (*clusterIt)->getMeanRow()    + weight * (*rawHitsIt)->getRow();
	      double totAdc_temp     = tempSumAdc;

	      (*clusterIt)->setLayer(layer_temp);
	      (*clusterIt)->setSensor(sensor_temp);
	      (*clusterIt)->setMeanColumn(meanColumn_temp);
	      (*clusterIt)->setMeanRow(meanRow_temp);
	      (*clusterIt)->setTotCharge(totAdc_temp);
	      (*clusterIt)->setMaxTb(maxTb_temp);
	      (*clusterIt)->setClusterType(1);
	      (*clusterIt)->setNRawHits(nRawHits);
	      (*clusterIt)->setNRawHitsR(nRawHitsR);
	      (*clusterIt)->setNRawHitsPhi(nRawHitsPhi);
	      (*clusterIt)->addRawHit((*rawHitsIt));

	      //include the hit to the cluster's component vector
	      int itPosition = std::distance(clusteredHitsVec.begin(), clusteredHitsIt);
	      clusteredHitsVec = (*clusterIt)->getRawHitVec();
	      clusteredHitsIt = clusteredHitsVec.begin() + itPosition;

	      //remove the clustered ith raw hit from the raw hits list
	      int distance = std::distance(rawHitsVec.begin(), rawHitsIt);
	      rawHitsVec.erase(rawHitsIt);
	      rawHitsIt = rawHitsVec.begin() + distance;
	    }
	    else
	    {
	      rawHitsIt++;
	    }
	  }
	}

	//if the rawHitsIt_th hit does not belong to the existed ith clusters then create a new cluster.
	if(rawHitsVec.size() < 1) continue;

	rawHitsIt = rawHitsVec.begin();

	FstCluster *fstCluster_next = new FstCluster();
	fstCluster_next->Clear();
	fstCluster_next->setLayer((*rawHitsIt)->getLayer());
	fstCluster_next->setSensor((*rawHitsIt)->getSensor());
	fstCluster_next->setMeanColumn((*rawHitsIt)->getColumn());
	fstCluster_next->setMeanRow((*rawHitsIt)->getRow());
	fstCluster_next->setTotCharge((*rawHitsIt)->getCharge((*rawHitsIt)->getMaxTb()));
	fstCluster_next->setMaxTb((*rawHitsIt)->getMaxTb());
	fstCluster_next->setClusterType(1);
	fstCluster_next->setNRawHits(1);
	fstCluster_next->setNRawHitsR(1);
	fstCluster_next->setNRawHitsPhi(1);
	fstCluster_next->addRawHit((*rawHitsIt)); // save hits for ith cluster

	rawHitsVec.erase(rawHitsIt); //remove the new clustered 1st hit from the hits list

	int distanceCluster = std::distance(clustersVec_Simple.begin(), clusterIt);
	clustersVec_Simple.push_back(fstCluster_next); // push new cluster to container
	clusterIt = clustersVec_Simple.begin() + distanceCluster;
	clusterIt++;
      }
    }
  }

  // cout << "rawHitsVec_orig.size = " << rawHitsVec_orig.size() << ", clustersVec_Simple.size = " << clustersVec_Simple.size() << endl;

  return clustersVec_Simple;
}
//--------------cluster with Simple Algorithm---------------------

//--------------Find Track with Hits---------------------
std::vector<FstTrack *> FstClusterMaker::findTrack_Hits(std::vector<FstRawHit *> rawHitVec_orig)
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
std::vector<FstTrack *> FstClusterMaker::findTrack_Clusters(std::vector<FstCluster *> clusterVec_orig)
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
bool FstClusterMaker::initTree()
{
  mTree_FstEvent = new TTree("mTree_FstEvent","Fst Hits and Clusters Info");
  mFstEvent = new FstEvent();
  mTree_FstEvent->Branch("FstEvent","FstEvent",&mFstEvent);
  mTree_FstEvent->SetAutoSave(50000000);

  return true;
}

void FstClusterMaker::writeTree()
{
  mTree_FstEvent->Write();
}
//--------------Output TTree---------------------

//--------------Utility---------------------
int FstClusterMaker::getLayer(int arm, int port)
{
  if(arm == 0 && port == 0) return 1; // top IST
  if(arm == 0 && port == 1) return 2; // middle IST
  if(arm == 1 && port == 0) return 3; // bottom IST
  if(arm == 1 && port == 1) return 0; // FST module

  return -1;
}

int FstClusterMaker::getSensor(int arm, int port, int apv)
{
  int layer = this->getLayer(arm,port);
  if(layer == 0)
  { // layer = 0 for FST
    if(apv >= 0 && apv <= 3) return 0; // inner sector
    if(apv >= 4 && apv <= 7) return 1; // outer sector
  }
  else
  { // layer = 1-3 for IST
    if(apv >= 0 && apv <= 5) return 0; // sensor 0 for first half group of APVs
    if(apv >= 6 && apv <= 11) return 1; // sensor 1 for second half group of APVs 
  }

  return -1;
}

int FstClusterMaker::getColumn(int arm, int port, int apv, int ch)
{
  int col = -1;
  int layer = this->getLayer(arm,port);
  int sensor = this->getSensor(arm,port,apv);

  if(layer == 0)
  { // layer = 0 for FST
    if(this->getRStrip(apv,ch) >= 0)
    {
      col = 4*sensor + this->getRStrip(apv,ch);
    }
  }
  else
  { // layer = 1-3 for IST
    if(ch >= 0 && ch <= 63) col = apv*2;
    else col = apv*2 + 1;
  }

  return col;
}

int FstClusterMaker::getRow(int arm, int port, int apv, int ch)
{
  int row = -1;
  int layer = this->getLayer(arm,port);
  if(layer == 0)
  { // layer = 0 for FST
    row = this->getPhiSeg(apv,ch);
  }
  else
  { // layer = 1-3 for IST
    if(ch >= 0 && ch <= 63) row = ch;
    else row = 127 - ch;
  }

  return row;
}

int FstClusterMaker::getRStrip(int apv, int ch)
{
  int r_strip = -1;

  // only apply to half outer sector for now
  if(apv == 4) r_strip = ch%4;
  if(apv == 5) r_strip = 3-ch%4;

  return r_strip;
}

int FstClusterMaker::getPhiSeg(int apv, int ch)
{
  int phi_seg = -1;

  if(apv >= 0 && apv <= 3) phi_seg = apv*32 + ch/4; // inner
  if(apv >= 4 && apv <= 7) phi_seg = (apv-4)*32 + ch/4; // outer

  return phi_seg;
}

double FstClusterMaker::getPosX(int arm, int port, int apv, int ch)
{
  double posX = -999.9;
  int layer = this->getLayer(arm,port);
  if(layer == 0) // FST
  {
    double r_fst = FST::rOuter + (this->getColumn(arm,port,apv,ch)-4)*FST::pitchR + 0.5*FST::pitchR;
    double phi_fst = (63-this->getRow(arm,port,apv,ch))*FST::pitchPhi + 0.5*FST::pitchPhi;
    posX = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
  }
  else // IST1-3
  {
    posX = this->getColumn(arm,port,apv,ch)*FST::pitchColumn + 0.5*FST::pitchColumn;
  }

  return posX;
}

double FstClusterMaker::getPosY(int arm, int port, int apv, int ch)
{
  double posY = -999.9;
  int layer = this->getLayer(arm,port);
  if(layer == 0) // FST
  {
    double r_fst = FST::rOuter + (this->getColumn(arm,port,apv,ch)-4)*FST::pitchR + 0.5*FST::pitchR;
    double phi_fst = (63-this->getRow(arm,port,apv,ch))*FST::pitchPhi + 0.5*FST::pitchPhi;
    posY = r_fst*TMath::Sin(phi_fst); // y = r*sin(phi)
  }
  else // IST1-3
  {
    posY = (63-this->getRow(arm,port,apv,ch))*FST::pitchRow + 0.5*FST::pitchRow;
  }

  return posY;
}

bool FstClusterMaker::isBadAPV(int arm, int port, int apv)
{
  bool bAPV = false;

  if(arm == 0 && port==0 && (apv == 5||apv == 6)) bAPV = true;
  if(arm == 0 && port==1 && (apv <= 9)) bAPV = true;
  // if(arm == 1 && port==0 && (apv == 0||apv == 1)) bAPV = true;
  if(arm == 1 && port==0 && (apv == 0)) bAPV = true;
  // if(arm == 1 && port==1 && (apv == 6||apv == 7)) bAPV = true;

  return bAPV;
}
//--------------Utility---------------------
