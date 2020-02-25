#include "./IstTracking.h"

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

ClassImp(IstTracking)

//------------------------------------------

IstTracking::IstTracking() : mList("../list/IstTracking.list"), mOutPutFile("./IstTracking.root")
{
  cout << "IstTracking::IstTracking() -------- Constructor!  --------" << endl;
  mHome = getenv("HOME");
}

IstTracking::~IstTracking()
{
  cout << "IstTracking::~IstTracking() -------- Release Memory!  --------" << endl;
}

//------------------------------------------
int IstTracking::Init()
{
  cout << "IstTracking::Init => " << endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  bool isInPut = initChain();
  bool isPed = initPedestal(); // initialize pedestal array;
  initHitDisplay(); // initialize hit display
  initTracking_ARMDisplay(); // initialize tracking as ARMDisplay

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

  return 1;
}

bool IstTracking::initChain()
{
  cout << "IstTracking::initChain -> " << endl;
  string inputdir = Form("%s/WorkSpace/STAR/Data/ForwardSiliconTracker/",mHome.c_str());

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
	  addfile = inputdir + addfile;
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

bool IstTracking::initPedestal()
{
  cout << "IstTracking::initPedestal -> " << endl;
  for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < IST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	{
	  mPed[i_arm][i_port][i_apv][i_ch] = -1.0;
	  mPedStdDev[i_arm][i_port][i_apv][i_ch] = -1.0;
	  // pedRMS[i_arm][i_port][i_apv][i_ch] = -1.0;
	}
      }
    }
  }
  g_mPedMean_Layer1 = new TGraph();
  g_mPedSigma_Layer1 = new TGraph();
  g_mPedMean_Layer2 = new TGraph();
  g_mPedSigma_Layer2 = new TGraph();
  g_mPedMean_Layer3 = new TGraph();
  g_mPedSigma_Layer3 = new TGraph();

  return true;
}

bool IstTracking::initSignal()
{
  cout << "IstTracking::initSignal -> " << endl;
  return clearSignal();
}

bool IstTracking::initHit()
{
  cout << "IstTracking::initHit -> " << endl;
  return clearHit();
}

bool IstTracking::initHitDisplay()
{
  h_mHitDisplay_Layer1 = new TH2F("h_mHitDisplay_Layer1","h_mHitDisplay_Layer1",IST::noColumns,-0.5,IST::noColumns-0.5,IST::noRows,-0.5,IST::noRows-0.5);
  h_mHitDisplay_Layer2 = new TH2F("h_mHitDisplay_Layer2","h_mHitDisplay_Layer2",IST::noColumns,-0.5,IST::noColumns-0.5,IST::noRows,-0.5,IST::noRows-0.5);
  h_mHitDisplay_Layer3 = new TH2F("h_mHitDisplay_Layer3","h_mHitDisplay_Layer3",IST::noColumns,-0.5,IST::noColumns-0.5,IST::noRows,-0.5,IST::noRows-0.5);

  h_mMaxTb_Layer1 = new TH1F("h_mMaxTb_Layer1","h_mMaxTb_Layer1",10,-0.5,9.5); 
  h_mMaxTb_Layer2 = new TH1F("h_mMaxTb_Layer2","h_mMaxTb_Layer2",10,-0.5,9.5);
  h_mMaxTb_Layer3 = new TH1F("h_mMaxTb_Layer3","h_mMaxTb_Layer3",10,-0.5,9.5);

  return true;
}

bool IstTracking::initCluster()
{
  cout << "IstTracking::initCluster -> " << endl;
  return clearCluster();
}

bool IstTracking::initTracking_ARMDisplay()
{
  h_mXResidual = new TH1F("h_mXResidual","h_mXResidual",150,-9,9);
  h_mYResidual = new TH1F("h_mYResidual","h_mYResidual",150,-9,9);

  h_mAdc_Layer1 = new TH1F("h_mAdc_Layer1","h_mAdc_Layer1",100,0,4000);
  h_mAdc_Layer3 = new TH1F("h_mAdc_Layer3","h_mAdc_Layer3",100,0,4000);
  h_mAdcAngleCorr_Layer1 = new TH1F("h_mAdcAngleCorr_Layer1","h_mAdcAngleCorr_Layer1",100,0,4000);
  h_mAdcAngleCorr_Layer3 = new TH1F("h_mAdcAngleCorr_Layer3","h_mAdcAngleCorr_Layer3",100,0,4000);

  h_mTrackAngle = new TH1F("h_mTrackAngle","h_mTrackAngle",100,0,90);

  return true;
}

//------------------------------------------
bool IstTracking::clearSignal()
{
  for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < IST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < IST::numTBins; ++i_tb)
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

bool IstTracking::clearHit()
{
  for(int i_hit = 0; i_hit < IST::maxNHits; ++i_hit)
  {
    mIstHit[i_hit].layer = -1;
    mIstHit[i_hit].sensor = -1;
    mIstHit[i_hit].column = -1;
    mIstHit[i_hit].row = -1;
    mIstHit[i_hit].maxAdc = -1;
    mIstHit[i_hit].maxTb = -1;
    mIstHit[i_hit].filled = false;
  }

  return true;
}

bool IstTracking::clearCluster()
{
  for(int i_cluster = 0; i_cluster < IST::maxNHits; ++i_cluster)
  {
    mIstCluster[i_cluster].layer = -1;
    mIstCluster[i_cluster].sensor = -1;
    mIstCluster[i_cluster].x = -1;
    mIstCluster[i_cluster].y = -1;
    mIstCluster[i_cluster].adc = -1;
    mIstCluster[i_cluster].filled = false;
  }

  return true;
}
//------------------------------------------

int IstTracking::Make()
{
  cout << "IstTracking::Make => " << endl;

  bool isPed = calPedestal(); // calculate pedestals with first 1000 events
  if( !isPed ) 
  {
    cout << "No Pedestal!!!" << endl;
    return -1;
  }

  long NumOfEvents = (long)mChainInPut->GetEntries();
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  mChainInPut->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);
    clearHit();
    clearSignal();

    // calculate ped corrected signal for each ch & time bin
    // ped is calcualte from pedTimeBin = 0
    for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < IST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
	{
	  int rdo  = evt_rdo[i_arm][i_port][i_apv];
	  int arm  = evt_arm[i_arm][i_port][i_apv];
	  int port = evt_port[i_arm][i_port][i_apv];
	  int apv  = evt_apv[i_arm][i_port][i_apv];

	  // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
	  bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < IST::numAPVs ) ) ;
	  if(pass)
	  {
	    for(int i_ro = 0; i_ro < IST::numROChannels; ++i_ro)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ro]; // time bin
	      int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
	      int adc = hit_adc[i_arm][i_port][i_apv][i_ro];

	      mSigPedCorr[i_arm][i_port][i_apv][ch][tb] = adc-mPed[i_arm][i_port][i_apv][ch]; // adc - ped
	      mRawSig[i_arm][i_port][i_apv][ch][tb] = adc;
	    }
	  }
	}
      }
    }

    // find Hits
    int numOfHits = 0;
    for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < IST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
	{
	  for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	  {
	    if( // some hit quality cuts => 1st one is questionable | 2nd made sure ch shows reasonable noise
		( mSigPedCorr[i_arm][i_port][i_apv][i_ch][0] < mSigPedCorr[i_arm][i_port][i_apv][i_ch][3] ) &&
		( mPedStdDev[i_arm][i_port][i_apv][i_ch] > IST::MinNoise)  
	      )
	    {
	      double maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][0]; // init with 1st tb
	      int maxTB = 0;
	      double preADC = maxADC;
	      bool isHit = false;
	      for(int i_tb = 1; i_tb < IST::numTBins-1; ++i_tb)
	      { // only if 3 consequetive timebins of a ch exceed the threshold cut is considered as a hit
		if( 
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb-1] > IST::nHitCuts*mPedStdDev[i_arm][i_port][i_apv][i_ch]) &&
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > IST::nHitCuts*mPedStdDev[i_arm][i_port][i_apv][i_ch]) &&
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > IST::nHitCuts*mPedStdDev[i_arm][i_port][i_apv][i_ch])
		  ) 
		{
		  isHit = true; // set isHit to true if 3 consequetive time bins exceed the threshold
		  if(mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > preADC)
		  { // find time bin with max adc for 0-IST::numTBins-2
		    maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb];
		    maxTB = i_tb;
		    preADC = maxADC;
		  }
		  if(i_tb == IST::numTBins-2 && mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > preADC)
		  { // check if last time bin has the max ADC
		    maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1];
		    maxTB = i_tb+1;
		  }
		}
	      }
	      if(isHit && numOfHits < IST::maxNHits)
	      { // set Hit info
		mIstHit[numOfHits].layer = getLayer(i_arm,i_port);
		mIstHit[numOfHits].sensor = getSensor(i_apv);
		mIstHit[numOfHits].column = getColumn(i_apv,i_ch);
		mIstHit[numOfHits].row = getRow(i_apv,i_ch);
		mIstHit[numOfHits].maxAdc = maxADC;
		mIstHit[numOfHits].maxTb = maxTB;
		mIstHit[numOfHits].filled = isHit;
		numOfHits++;
	      }
	    }
	  }
	}
      }
    }

    if(numOfHits > 0 && numOfHits <= IST::maxNHitsPerEvent) // maximum hits to expect per event is 10
    {
      fillHitDisplay(mIstHit); // fill hit display
      findCluster_ARMDisplay(mIstHit,numOfHits); // find cluster
    }
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 1;
}

bool IstTracking::calPedestal()
{
  cout << "IstTracking::calPedestal -> " << endl;
  cout << " Only use first 1000 event for Pedstal Calculation!" << endl;


  int NumOfEvents = (int)mChainInPut->GetEntries();
  if(NumOfEvents > 1000) NumOfEvents = 1000;
  // const int NumOfEvents = 1000;
  mChainInPut->GetEntry(0);

  //  Calculate a rolling average and standard deviation
  const int pedTimeBin = 0; // use first time bin for ped
  int counters[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
  double sumValues[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
  double sumValuesSquared[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];

  //--------------------------------------------------------
  // 1st loop to get a rough estimate of ped (including Hits)
  cout << "=====>First Pedestal Pass...." << endl;
  for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < IST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	{
	  counters[i_arm][i_port][i_apv][i_ch] = 0;
	  sumValues[i_arm][i_port][i_apv][i_ch] = 0.0;
	  sumValuesSquared[i_arm][i_port][i_apv][i_ch] = 0.0;
	}
      }
    }
  }

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  { 
    // if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < IST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
	{
	  int rdo  = evt_rdo[i_arm][i_port][i_apv];
	  int arm  = evt_arm[i_arm][i_port][i_apv];
	  int port = evt_port[i_arm][i_port][i_apv];
	  int apv  = evt_apv[i_arm][i_port][i_apv];

	  // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
	  bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < IST::numAPVs ) ) ;
	  if(pass)
	  {
	    for(int i_ro = 0; i_ro < IST::numROChannels; ++i_ro)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ro]; // time bin
	      if(tb == pedTimeBin)
	      {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ro];
		sumValues[arm][port][apv][ch] += adc;
		sumValuesSquared[arm][port][apv][ch] += adc * adc;
		counters[arm][port][apv][ch]++;
	      }
	    }
	  }
	}
      }
    }
  }

  // rough estimate of ped (including Hits)
  for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < IST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	{
	  mPed[i_arm][i_port][i_apv][i_ch] = sumValues[i_arm][i_port][i_apv][i_ch]/counters[i_arm][i_port][i_apv][i_ch];
	  mPedStdDev[i_arm][i_port][i_apv][i_ch] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch]-(double)counters[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch])/(double)(counters[i_arm][i_port][i_apv][i_ch]-1));
	  // pedRMS[i_arm][i_port][i_apv][i_ch] = sqrt(mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]-mPedStdDev[i_arm][i_port][i_apv][i_ch]*mPedStdDev[i_arm][i_port][i_apv][i_ch]);
	}
      }
    }
  }
  //--------------------------------------------------------

  //--------------------------------------------------------
  // 2nd loop to find ped excluding Hits
  cout << "=====>Second Pedestal Pass...." << endl;
  for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < IST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	{
	  counters[i_arm][i_port][i_apv][i_ch] = 0;
	  sumValues[i_arm][i_port][i_apv][i_ch] = 0.0;
	  sumValuesSquared[i_arm][i_port][i_apv][i_ch] = 0.0;
	}
      }
    }
  }
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  { 
    // if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < IST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
	{
	  int rdo  = evt_rdo[i_arm][i_port][i_apv];
	  int arm  = evt_arm[i_arm][i_port][i_apv];
	  int port = evt_port[i_arm][i_port][i_apv];
	  int apv  = evt_apv[i_arm][i_port][i_apv];

	  // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
	  bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < IST::numAPVs ) ) ;
	  if(pass)
	  {
	    for(int i_ro = 0; i_ro < IST::numROChannels; ++i_ro)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ro]; // time bin
	      if(tb == pedTimeBin)
	      {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ro];
		if ( (adc < mPed[arm][port][apv][ch]+IST::nPedCuts*mPedStdDev[arm][port][apv][ch]) && ( adc >= 0 && adc < 4096) )
		{ // only adc belew ped+3sigma are considered for 2nd loop
		  sumValues[arm][port][apv][ch] += adc;
		  sumValuesSquared[arm][port][apv][ch] += adc * adc;
		  counters[arm][port][apv][ch]++;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  // calculate ped excluding Hits
  for(int i_arm = 0; i_arm < IST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < IST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < IST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < IST::numChannels; ++i_ch)
	{
	  mPed[i_arm][i_port][i_apv][i_ch] = sumValues[i_arm][i_port][i_apv][i_ch]/counters[i_arm][i_port][i_apv][i_ch];
	  mPedStdDev[i_arm][i_port][i_apv][i_ch] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch]-(double)counters[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch])/(double)(counters[i_arm][i_port][i_apv][i_ch]-1));
	  // pedRMS[i_arm][i_port][i_apv][i_ch] = sqrt(mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]-mPedStdDev[i_arm][i_port][i_apv][i_ch]*mPedStdDev[i_arm][i_port][i_apv][i_ch]);

	  if(getLayer(i_arm,i_port) == 1) g_mPedMean_Layer1->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,mPed[i_arm][i_port][i_apv][i_ch]);
	  if(getLayer(i_arm,i_port) == 1) g_mPedSigma_Layer1->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,mPedStdDev[i_arm][i_port][i_apv][i_ch]);

	  if(getLayer(i_arm,i_port) == 2) g_mPedMean_Layer2->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,mPed[i_arm][i_port][i_apv][i_ch]);
	  if(getLayer(i_arm,i_port) == 2) g_mPedSigma_Layer2->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,mPedStdDev[i_arm][i_port][i_apv][i_ch]);

	  if(getLayer(i_arm,i_port) == 3) g_mPedMean_Layer3->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,mPed[i_arm][i_port][i_apv][i_ch]);
	  if(getLayer(i_arm,i_port) == 3) g_mPedSigma_Layer3->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,mPedStdDev[i_arm][i_port][i_apv][i_ch]);
	}
      }
    }
  }
  //--------------------------------------------------------

  return true;
}

void IstTracking::fillHitDisplay(IstHit isthit[])
{
  for(int i_hit = 0; i_hit < IST::maxNHits; ++i_hit)
  {
    if(isthit[i_hit].filled == true)
    {
      if(isthit[i_hit].layer == 1) h_mHitDisplay_Layer1->Fill(isthit[i_hit].column,isthit[i_hit].row);
      if(isthit[i_hit].layer == 1) h_mMaxTb_Layer1->Fill(isthit[i_hit].maxTb);
      // if(isthit[i_hit].column >=2 && isthit[i_hit].column<=5 && isthit[i_hit].row < 10 && isthit[i_hit].layer == 1)
      // {
	// cout << "adc = " << isthit[i_hit].maxAdc << endl;
	// cout << "tb = " << isthit[i_hit].maxTb << endl;
      // }
      if(isthit[i_hit].layer == 2) h_mHitDisplay_Layer2->Fill(isthit[i_hit].column,isthit[i_hit].row);
      if(isthit[i_hit].layer == 2) h_mMaxTb_Layer2->Fill(isthit[i_hit].maxTb);
      if(isthit[i_hit].layer == 3) h_mHitDisplay_Layer3->Fill(isthit[i_hit].column,isthit[i_hit].row);
      if(isthit[i_hit].layer == 3) h_mMaxTb_Layer3->Fill(isthit[i_hit].maxTb);
    }
  }
}

bool IstTracking::findCluster_ARMDisplay(IstHit isthit[], int numOfHits)
{
  clearCluster();
  for(int i_hit_1st = 0; i_hit_1st < numOfHits-1; ++i_hit_1st) 
  { 
    for(int i_hit_2nd = i_hit_1st+1; i_hit_2nd < numOfHits; ++i_hit_2nd) 
    {
      if(isthit[i_hit_1st].layer == isthit[i_hit_2nd].layer && isthit[i_hit_1st].sensor == isthit[i_hit_2nd].sensor) 
      {
	if ( 
	    ( isthit[i_hit_1st].column == isthit[i_hit_2nd].column && (isthit[i_hit_1st].row == isthit[i_hit_2nd].row-1 || isthit[i_hit_1st].row == isthit[i_hit_2nd].row+1) ) ||
	    ( isthit[i_hit_1st].row == isthit[i_hit_2nd].row && (isthit[i_hit_1st].column == isthit[i_hit_2nd].column-1 || isthit[i_hit_1st].column == isthit[i_hit_2nd].column +1))
	   ) 
	{
	  if(isthit[i_hit_1st].maxAdc > isthit[i_hit_2nd].maxAdc) 
	  {
	    isthit[i_hit_1st].maxAdc += isthit[i_hit_2nd].maxAdc;
	    isthit[i_hit_2nd].maxAdc  = -1.0*isthit[i_hit_2nd].maxAdc;
	    isthit[i_hit_2nd].filled = false;
	  }
	  else 
	  {
	    isthit[i_hit_2nd].maxAdc += isthit[i_hit_1st].maxAdc;
	    isthit[i_hit_1st].maxAdc  = -1.0*isthit[i_hit_1st].maxAdc;
	    isthit[i_hit_1st].filled = false;
	  }
	}
      }
    } 
  }

  int numOfCluster = 0;
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  {
    if(isthit[i_hit].filled == true) 
    {
      mIstCluster[numOfCluster].layer = isthit[i_hit].layer;
      mIstCluster[numOfCluster].sensor = isthit[i_hit].sensor;
      // mIstCluster[numOfCluster].x = isthit[i_hit].column*IST::pitchColumn + (IST::pitchColumn*rand()/double(RAND_MAX));
      // mIstCluster[numOfCluster].y = isthit[i_hit].row*IST::pitchRow + (IST::pitchRow*rand()/double(RAND_MAX));
      // if( isthit[i_hit].layer == 1) mIstCluster[numOfCluster].z = 0.0;
      // if( isthit[i_hit].layer == 2) mIstCluster[numOfCluster].z = IST::pitchLayer12;
      // if( isthit[i_hit].layer == 3) mIstCluster[numOfCluster].z = IST::pitchLayer12+IST::pitchLayer23;
      mIstCluster[numOfCluster].x = isthit[i_hit].column*IST::pitchColumn;
      mIstCluster[numOfCluster].y = isthit[i_hit].row*IST::pitchRow;
      if( isthit[i_hit].layer == 1) mIstCluster[numOfCluster].z = 0.0;
      if( isthit[i_hit].layer == 2) mIstCluster[numOfCluster].z = IST::pitchLayer12;
      if( isthit[i_hit].layer == 3) mIstCluster[numOfCluster].z = IST::pitchLayer12+IST::pitchLayer23;
      mIstCluster[numOfCluster].adc = isthit[i_hit].maxAdc;
      mIstCluster[numOfCluster].filled = true;
      numOfCluster++;
    }
  }

  if(numOfCluster > 0 && numOfCluster <= IST::maxNHitsPerEvent)
  {
    fillTracking_ARMDisplay(mIstCluster,numOfCluster);
  }

  return true;
}

void IstTracking::fillTracking_ARMDisplay(IstCluster istcluster[], int numOfCluster)
{
  for ( int i_cluster_1st = 0; i_cluster_1st < numOfCluster-1; ++i_cluster_1st) 
  {
    for ( int i_cluster_3rd = i_cluster_1st+1; i_cluster_3rd < numOfCluster; ++i_cluster_3rd) 
    {
      TVector3 Normal, Track;
      // Find a hit on layer 1 and a hit on layer 3, we only have a sensor 0 in both layers
      if ( 
	  ( ( istcluster[i_cluster_1st].filled == true ) && ( istcluster[i_cluster_3rd].filled == true ) ) &&
	  ( ( ( istcluster[i_cluster_1st].layer  ==  1 ) && ( istcluster[i_cluster_3rd].layer  ==  3 ) ) || ( ( istcluster[i_cluster_1st].layer  ==  3 ) && ( istcluster[i_cluster_3rd].layer  ==  1 ) )  )
	 ) 
      {
	float x1 = istcluster[i_cluster_1st].x;
	float y1 = istcluster[i_cluster_1st].y;
	float z1 = istcluster[i_cluster_1st].z;

	float x3 = istcluster[i_cluster_3rd].x;
	float y3 = istcluster[i_cluster_3rd].y;
	float z3 = istcluster[i_cluster_3rd].z;

	Normal.SetXYZ(0, 0, z3-z1);
	Track.SetXYZ(x3-x1, y3-y1, z3-z1);
	h_mTrackAngle->Fill(IST::rad2deg*Track.Angle(Normal), 1.0);
	// Cut on maximum track angle to weed out invalid 2-hit combinations a bit
	if ( IST::rad2deg*Track.Angle(Normal) < IST::maxAngle ) 
	{
	  // not angle corrected hits on a track
	  h_mAdc_Layer1->Fill(istcluster[i_cluster_1st].adc, 1.0);
	  h_mAdc_Layer3->Fill(istcluster[i_cluster_3rd].adc, 1.0);
	  // angle corrected hits on a track
	  h_mAdcAngleCorr_Layer1->Fill((istcluster[i_cluster_1st].adc)*cos(Track.Angle(Normal)), 1.0);
	  h_mAdcAngleCorr_Layer3->Fill((istcluster[i_cluster_3rd].adc)*cos(Track.Angle(Normal)), 1.0);
	  // Do the residuals of the found track with the hit from Layer 2
	  for ( int i_cluster_2nd = 0; i_cluster_2nd < numOfCluster; ++i_cluster_2nd) 
	  {
	    if ( ( istcluster[i_cluster_2nd].filled ==  true) &&
		( istcluster[i_cluster_2nd].layer  ==  2 )
	       ) 
	    {
	      // Calculate randomized position of hit
	      float x2 = istcluster[i_cluster_2nd].x;
	      float y2 = istcluster[i_cluster_2nd].y;
	      float z2 = istcluster[i_cluster_2nd].z;

	      float x2proj = x1 + (x3-x1)*IST::pitchLayer12/(IST::pitchLayer12+IST::pitchLayer23);
	      float y2proj = y1 + (y3-y1)*IST::pitchLayer12/(IST::pitchLayer12+IST::pitchLayer23);
	      float z2proj = IST::pitchLayer12 ;
	      // Calculate residual
	      //Residual = sqrt( (x2-x2proj)*(x2-x2proj) + (y2-y2proj)*(y2-y2proj) );
	      float xResidual = (x2-x2proj);
	      float yResidual = (y2-y2proj);
	      h_mXResidual->Fill(xResidual, 1.0);
	      h_mYResidual->Fill(yResidual, 1.0);
	    }
	  }
	}
      }
    }
  }
}

int IstTracking::Finish()
{
  cout << "IstTracking::Finish => " << endl;
  writePedestal();
  writeHitDisplay();
  writeTracking_ARMDisplay();

  return 1;
}

void IstTracking::writePedestal()
{
  cout << "IstTracking::writePedestal => save ped mean & sigma!" << endl;

  g_mPedMean_Layer1->SetName("g_mPedMean_Layer1");
  g_mPedMean_Layer1->Write();
  g_mPedSigma_Layer1->SetName("g_mPedSigma_Layer1");
  g_mPedSigma_Layer1->Write();

  g_mPedMean_Layer2->SetName("g_mPedMean_Layer2");
  g_mPedMean_Layer2->Write();
  g_mPedSigma_Layer2->SetName("g_mPedSigma_Layer2");
  g_mPedSigma_Layer2->Write();

  g_mPedMean_Layer3->SetName("g_mPedMean_Layer3");
  g_mPedMean_Layer3->Write();
  g_mPedSigma_Layer3->SetName("g_mPedSigma_Layer3");
  g_mPedSigma_Layer3->Write();
}

void IstTracking::writeHitDisplay()
{
  cout << "IstTracking::writeHitDisplay => save Hits at each Layer!" << endl;
  h_mHitDisplay_Layer1->Write();
  h_mHitDisplay_Layer2->Write();
  h_mHitDisplay_Layer3->Write();

  h_mMaxTb_Layer1->Write();
  h_mMaxTb_Layer2->Write();
  h_mMaxTb_Layer3->Write();
}

void IstTracking::writeTracking_ARMDisplay()
{
  h_mXResidual->Write();
  h_mYResidual->Write();

  h_mAdc_Layer1->Write();
  h_mAdc_Layer3->Write();
  h_mAdcAngleCorr_Layer1->Write();
  h_mAdcAngleCorr_Layer3->Write();

  h_mTrackAngle->Write();
}


//------------------------------------------
int IstTracking::getLayer(int arm, int port)
{
  if(arm == 0 && port == 0) return 1; // top IST
  if(arm == 0 && port == 1) return 2; // middle IST
  if(arm == 1 && port == 0) return 3; // bottom IST
  if(arm == 1 && port == 1) return 0; // future FST module

  return -1;
}

int IstTracking::getSensor(int apv)
{
  if(apv >= 0 && apv <= 5) return 0; // sensor 0 for first half group of APVs
  if(apv >= 6 && apv <= 11) return 1; // sensor 1 for second half group of APVs 

  return -1;
}

int IstTracking::getColumn(int apv, int ch)
{

  int col = -1;

  if(ch >= 0 && ch <= 63) col = apv*2;
  else col = apv*2 + 1;

  return col;
}

int IstTracking::getRow(int apv, int ch)
{

  int row = -1;

  if(ch >= 0 && ch <= 63) row = ch;
  else row = 127 - ch;

  return row;
}
