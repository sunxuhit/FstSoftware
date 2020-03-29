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

FstTracking::FstTracking() : mList("../../list/FST/FstPed_HV70.list"), mOutPutFile("./FstPed_HV70.root")
{
  cout << "FstTracking::FstTracking() -------- Constructor!  --------" << endl;
  mHome = getenv("HOME");
  isSaveHits = false;
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
  bool isPed = initPedestal(); // initialize pedestal array;
  bool isSig = initSignal(); // initialize signal array;
  bool isHit = initHit(); // initialize Hit array;
  bool isTracking_Hits = initTracking_Hits();
  bool isTracking_Clusters = initTracking_Clusters();

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
  if(!isHit) 
  {
    cout << "Failed to initialize FST & IST Hits!" << endl;
    return -1;
  }
  if(!isTracking_Hits) 
  {
    cout << "Failed to initialize tracing with Hits method!" << endl;
    return -1;
  }
  if(!isTracking_Clusters) 
  {
    cout << "Failed to initialize tracing with Clusters method!" << endl;
    return -1;
  }

  initHitDisplay(); // initialize hit display
  // initTracking_ARMDisplay(); // initialize tracking as ARMDisplay

  return 1;
}

bool FstTracking::initChain()
{
  cout << "FstTracking::initChain -> " << endl;

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

bool FstTracking::initSignal()
{
  cout << "FstTracking::initSignal -> " << endl;
  return clearSignal();
}

bool FstTracking::initHit()
{
  cout << "FstTracking::initHit -> " << endl;
  return clearHit();
}

/*
bool FstTracking::initCluster()
{
  cout << "FstTracking::initCluster -> " << endl;
  return clearCluster_ARMDisplay() && clearCluster_Simple();
}
*/
//------------------------------------------
bool FstTracking::clearSignal()
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

bool FstTracking::clearHit()
{
  mHitsVec.clear();

  return true;
}
//------------------------------------------
int FstTracking::Make()
{
  cout << "FstTracking::Make => " << endl;


  if(isSaveHits)
  {
    string outputfile = "./hits.txt";
    file_mHits.open(outputfile.c_str());
    cout << "Open file_mHits.txt to save Hits info!" << endl;
    if (!file_mHits.is_open())
    {
      std::cout << "failed to open " << outputfile.c_str() << endl;
      return -1;
    }
  }

  bool isPed = calPedestal(); // calculate pedestals with first 1000 events
  if( !isPed ) 
  {
    cout << "No Pedestal!!!" << endl;
    return -1;
  }

  long NumOfEvents = (long)mChainInPut->GetEntries();
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 100;
  mChainInPut->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);
    clearHit();
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

	      mSigPedCorr[i_arm][i_port][i_apv][ch][tb] = adc-mPed[i_arm][i_port][i_apv][ch]; // adc - ped
	      mRawSig[i_arm][i_port][i_apv][ch][tb] = adc;
	    }
	  }
	}
      }
    }

    // find Hits
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
		( mPedStdDev[i_arm][i_port][i_apv][i_ch] > FST::MinNoise)  
	      )
	    {
	      double maxADC = mSigPedCorr[i_arm][i_port][i_apv][i_ch][0]; // init with 1st tb
	      int maxTB = 0;
	      double preADC = maxADC;
	      bool isHit = false;
	      for(int i_tb = 1; i_tb < FST::numTBins-1; ++i_tb)
	      { // only if 3 consequetive timebins of a ch exceed the threshold cut is considered as a hit
		if( 
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb-1] > FST::nHitCuts*mPedStdDev[i_arm][i_port][i_apv][i_ch]) &&
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb] > FST::nHitCuts*mPedStdDev[i_arm][i_port][i_apv][i_ch]) &&
		    ( mSigPedCorr[i_arm][i_port][i_apv][i_ch][i_tb+1] > FST::nHitCuts*mPedStdDev[i_arm][i_port][i_apv][i_ch])
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
	      if(isHit && numOfHits < FST::maxNHits)
	      { // set Hit info
		HIT hit_temp;
		hit_temp.layer = getLayer(i_arm,i_port);
		hit_temp.sensor = getSensor(i_arm,i_port,i_apv);
		hit_temp.column = getColumn(i_arm,i_port,i_apv,i_ch); // phi for FST, x for IST
		hit_temp.row = getRow(i_arm,i_port,i_apv,i_ch); // r_strip for FST, y for IST
		hit_temp.maxAdc = maxADC;
		hit_temp.maxTb = maxTB;
		hit_temp.filled = isHit;

		mHitsVec.push_back(hit_temp);
		numOfHits++;
	      }
	    }
	  }
	}
      }
    }

    if(numOfHits > 0 && numOfHits <= FST::maxNHitsPerEvent) // maximum hits to expect per event is 10
    {
      fillHitDisplay(mHitsVec); // fill hit display

      // findCluster_ARMDisplay(mHitsVec); // find cluster with ARMDisplay
      findCluster_Simple(mHitsVec); // find cluster with ARMDisplay
      doTracking_Hits(mHitsVec); // do tracking based on Hits
    }
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  if(isSaveHits)
  {
    cout << "Close file_mHits.txt for hits infor!" << endl;
    file_mHits.close();
  }

  return 1;
}

int FstTracking::Finish()
{
  cout << "FstTracking::Finish => " << endl;
  writePedestal();
  writeHitDisplay();
  // writeTracking_ARMDisplay();
  writeTracking_Hits();
  // writeTracking_Clusters();

  return 1;
}

//--------------pedestal---------------------
bool FstTracking::clearPedestal()
{
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  mPed[i_arm][i_port][i_apv][i_ch] = -1.0;
	  mPedStdDev[i_arm][i_port][i_apv][i_ch] = -1.0;
	  // pedRMS[i_arm][i_port][i_apv][i_ch] = -1.0;
	}
      }
    }
  }

  return true;
}

bool FstTracking::initPedestal()
{
  cout << "FstTracking::initPedestal -> " << endl;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    std::string gName = Form("g_mPedMean_Layer%d",i_layer);
    g_mPedMean[i_layer] = new TGraph();
    g_mPedMean[i_layer]->SetName(gName.c_str());

    gName = Form("g_mPedSigma_Layer%d",i_layer);
    g_mPedSigma[i_layer] = new TGraph();
    g_mPedSigma[i_layer]->SetName(gName.c_str());

    std::string HistName = Form("h_mPedDisplay_Layer%d",i_layer);
    if(i_layer == 0) h_mPedDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::numRStrip,-0.5,FST::numRStrip-0.5,FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5);
    else h_mPedDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::noRows,-0.5,FST::noRows-0.5);
  }

  return clearPedestal();
}

bool FstTracking::calPedestal()
{
  cout << "FstTracking::calPedestal -> " << endl;
  cout << " Only use first 1000 event for Pedstal Calculation!" << endl;

  int NumOfEvents = (int)mChainInPut->GetEntries();
  if(NumOfEvents > 1000) NumOfEvents = 1000;
  // const int NumOfEvents = 1000;
  mChainInPut->GetEntry(0);

  //  Calculate a rolling average and standard deviation
  int counters[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels];
  double sumValues[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels];
  double sumValuesSquared[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels];

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
	      if(tb == FST::pedTimeBin)
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
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  if(counters[i_arm][i_port][i_apv][i_ch] > 0) // eject bad channels
	  {
	    mPed[i_arm][i_port][i_apv][i_ch] = sumValues[i_arm][i_port][i_apv][i_ch]/counters[i_arm][i_port][i_apv][i_ch];
	    mPedStdDev[i_arm][i_port][i_apv][i_ch] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch]-(double)counters[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch])/(double)(counters[i_arm][i_port][i_apv][i_ch]-1));
	    // pedRMS[i_arm][i_port][i_apv][i_ch] = sqrt(mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]-mPedStdDev[i_arm][i_port][i_apv][i_ch]*mPedStdDev[i_arm][i_port][i_apv][i_ch]);
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
	      if(tb == FST::pedTimeBin)
	      {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ro]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ro];
		if ( (adc < mPed[arm][port][apv][ch]+FST::nPedCuts*mPedStdDev[arm][port][apv][ch]) && ( adc >= 0 && adc < 4096) )
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
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	{
	  if(counters[i_arm][i_port][i_apv][i_ch] > 0) // eject bad channels
	  {
	    mPed[i_arm][i_port][i_apv][i_ch] = sumValues[i_arm][i_port][i_apv][i_ch]/counters[i_arm][i_port][i_apv][i_ch];
	    mPedStdDev[i_arm][i_port][i_apv][i_ch] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch]-(double)counters[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch])/(double)(counters[i_arm][i_port][i_apv][i_ch]-1));
	    // pedRMS[i_arm][i_port][i_apv][i_ch] = sqrt(mPed[i_arm][i_port][i_apv][i_ch]*mPed[i_arm][i_port][i_apv][i_ch]-mPedStdDev[i_arm][i_port][i_apv][i_ch]*mPedStdDev[i_arm][i_port][i_apv][i_ch]);
	  }

	  int layer = this->getLayer(i_arm,i_port);
	  int col = this->getColumn(i_arm,i_port,i_apv,i_ch);
	  int row = this->getRow(i_arm,i_port,i_apv,i_ch);

	  g_mPedMean[layer]->SetPoint(i_apv*FST::numChannels+i_ch,i_apv*FST::numChannels+i_ch,mPed[i_arm][i_port][i_apv][i_ch]);
	  g_mPedSigma[layer]->SetPoint(i_apv*FST::numChannels+i_ch,i_apv*FST::numChannels+i_ch,mPedStdDev[i_arm][i_port][i_apv][i_ch]);
	  h_mPedDisplay[layer]->Fill(col,row,mPedStdDev[i_arm][i_port][i_apv][i_ch]);
	}
      }
    }
  }
  //--------------------------------------------------------

  return true;
}

void FstTracking::writePedestal()
{
  cout << "FstTracking::writePedestal => save ped mean & sigma!" << endl;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    g_mPedMean[i_layer]->Write();
    g_mPedSigma[i_layer]->Write();
    h_mPedDisplay[i_layer]->Write();
  }
}
//--------------pedestal---------------------

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

void FstTracking::fillHitDisplay(std::vector<HIT> hitVec)
{
  if(hitVec.size() < FST::maxNHits)
  {
    for(int i_hit = 0; i_hit < hitVec.size(); ++i_hit)
    {
      if(hitVec[i_hit].filled == true)
      {
	int layer = hitVec[i_hit].layer;
	h_mHitDisplay[layer]->Fill(hitVec[i_hit].column,hitVec[i_hit].row);
	h_mMaxTb[layer]->Fill(hitVec[i_hit].maxTb);
      }
    }
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

//--------------cluster with Simple Algorithm---------------------
bool FstTracking::clearCluster_Simple()
{
  mClustersVec_Simple.clear();

  return true;
}

bool FstTracking::findCluster_Simple(std::vector<HIT> hitVec_orig)
{
  double meanRow = 0., meanColumn = 0.;
  double totAdc = 0.;
  int clusterSize = 0, clusterSizeR = 0, clusterSizePhi = 0;

  clearCluster_Simple();

  int numOfHits = hitVec_orig.size();
  std::vector<HIT> hitVec;
  hitVec.clear();
  hitVec.reserve(hitVec_orig.size());
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  { // set temp ist hit container
    hitVec.push_back(hitVec_orig[i_hit]);
  }

  // find cluster
  if(hitVec.size() > 0)
  {
    // set 1st cluster to 1st hit
    std::vector<HIT>::iterator hitVecIt = hitVec.begin();

    CLUSTER istcluster_temp;
    istcluster_temp.layer        = (*hitVecIt).layer;
    istcluster_temp.sensor       = (*hitVecIt).sensor;
    istcluster_temp.meanColumn   = (*hitVecIt).column;
    istcluster_temp.meanRow      = (*hitVecIt).row;
    istcluster_temp.totAdc       = (*hitVecIt).maxAdc;
    istcluster_temp.maxTb        = (*hitVecIt).maxTb;
    istcluster_temp.clusterSize  = 1;
    istcluster_temp.clusterSizeR = 1;
    istcluster_temp.clusterSizePhi = 1;
    istcluster_temp.clusterType  = 1; // Simple

    mClustersVec_Simple.push_back(istcluster_temp);

    std::vector<HIT> hitVec_clustered; // save hits for ith cluster
    hitVec_clustered.clear();
    hitVec_clustered.push_back( (*hitVecIt) );

    hitVec.erase(hitVecIt); // remove 1st hit from ist hit container

    if( hitVec.size() !=0)
    {
      double weight, tempSumAdc;
      std::vector<CLUSTER>::iterator clusterIt = mClustersVec_Simple.begin(); // get the begin of cluster container

      while( clusterIt != mClustersVec_Simple.end() && !hitVec.empty() )
      { //loop the existed clusters vector constainer
	for(std::vector<HIT>::iterator cHitVecIt = hitVec_clustered.begin(); cHitVecIt != hitVec_clustered.end(); cHitVecIt++) 
	{ // loop over the hit belong to ith cluster
	  HIT cHit_temp = *cHitVecIt; // get hit from hitVec_clustered
	  hitVecIt = hitVec.begin(); // get hit from the rest of hitVec

	  while( hitVecIt != hitVec.end() ) 
	  { // loop over the rest of hits in hitVec
	    if( ( ((*hitVecIt).layer == cHit_temp.layer) && ((*hitVecIt).sensor == cHit_temp.sensor) && ((*hitVecIt).row == cHit_temp.row) && ( ((*hitVecIt).column == cHit_temp.column + 1) || ((*hitVecIt).column == cHit_temp.column - 1) )) ||
		( ((*hitVecIt).layer == cHit_temp.layer) && ((*hitVecIt).sensor == cHit_temp.sensor) && ((*hitVecIt).column == cHit_temp.column) && ( ((*hitVecIt).row == cHit_temp.row + 1) || ((*hitVecIt).row == cHit_temp.row - 1) ))   ) 
	    {
	      clusterSize = (*clusterIt).clusterSize + 1;
	      if( (*hitVecIt).row == cHit_temp.row ) clusterSizePhi = (*clusterIt).clusterSizePhi + 1; //same row
	      if( (*hitVecIt).column == cHit_temp.column ) clusterSizeR = (*clusterIt).clusterSizeR + 1; //same column

	      double currentAdc = (*hitVecIt).maxAdc;
	      tempSumAdc = (*clusterIt).totAdc + currentAdc;
	      weight = currentAdc/tempSumAdc;

	      int layer_temp        = (*clusterIt).layer;
	      int sensor_temp        = (*clusterIt).sensor;
	      double meanColumn_temp = (1.0 - weight) * (*clusterIt).meanColumn + weight * (*hitVecIt).column;
	      double meanRow_temp    = (1.0 - weight) * (*clusterIt).meanRow    + weight * (*hitVecIt).row;
	      double totAdc_temp     = tempSumAdc;

	      (*clusterIt).layer = layer_temp;
	      (*clusterIt).sensor = sensor_temp;
	      (*clusterIt).meanColumn = meanColumn_temp;
	      (*clusterIt).meanRow = meanRow_temp;
	      (*clusterIt).totAdc = totAdc_temp;
	      // (*clusterIt).maxTb = maxTb_temp;
	      (*clusterIt).clusterSize = clusterSize;
	      (*clusterIt).clusterSizeR = clusterSizeR;
	      (*clusterIt).clusterSizePhi = clusterSizePhi;
	      // (*clusterIt).clusterType = 1;

	      //include the hit to the cluster's component vector
	      int itPosition = std::distance(hitVec_clustered.begin(), cHitVecIt);
	      hitVec_clustered.push_back( (*hitVecIt) );
	      cHitVecIt = hitVec_clustered.begin() + itPosition;

	      //remove the clustered ith raw hit from the raw hits list
	      int distance = std::distance(hitVec.begin(), hitVecIt);
	      hitVec.erase(hitVecIt);
	      hitVecIt = hitVec.begin() + distance;
	    }
	    else
	    {
	      hitVecIt++;
	    }
	  }
	}

	//if the hitVecIt_th hit does not belong to the existed ith clusters then create a new cluster.
	if(hitVec.size() < 1) continue;

	hitVecIt = hitVec.begin();

	CLUSTER istcluster_next;
	istcluster_next.layer        = (*hitVecIt).layer;
	istcluster_next.sensor       = (*hitVecIt).sensor;
	istcluster_next.meanColumn   = (*hitVecIt).column;
	istcluster_next.meanRow      = (*hitVecIt).row;
	istcluster_next.totAdc       = (*hitVecIt).maxAdc;
	istcluster_next.maxTb        = (*hitVecIt).maxTb;
	istcluster_next.clusterSize  = 1;
	istcluster_next.clusterSizeR = 1;
	istcluster_next.clusterSizePhi = 1;
	istcluster_next.clusterType  = 1; // Simple

	int distanceCluster = std::distance(mClustersVec_Simple.begin(), clusterIt);
	mClustersVec_Simple.push_back(istcluster_next);
	clusterIt = mClustersVec_Simple.begin() + distanceCluster;

	// cout << "print clusters and hits information" << endl;
	// cout << "mClustersVec_Simple.size = " << mClustersVec_Simple.size() << endl;
	// cout <<  "distanceCluster = " << distanceCluster << endl;
	// printClusters(*clusterIt);
	// printHits(hitVec_clustered);
	// cout << endl;

	clusterIt++;

	hitVec_clustered.clear(); // clear hitVec for cluster
	hitVec_clustered.push_back( (*hitVecIt) ); // rest hitVec_clustered to new 1st hit in hitVec
	hitVec.erase(hitVecIt); //remove the new clustered 1st hit from the hits list
      }
    }
  }

  // cout << "hitVec_orig.size = " << hitVec_orig.size() << ", mClustersVec_Simple.size = " << mClustersVec_Simple.size() << endl;

  return true;
}

//--------------cluster with Simple Algorithm---------------------

//--------------Tracking with Hits---------------------
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
  h_mPhiResidual_Hits = new TH1F("h_mPhiResidual_Hits","h_mPhiResidual_Hits",100,-0.12,0.12);

  return true;
}

bool FstTracking::doTracking_Hits(std::vector<HIT> hitVec_orig)
{
  int numOfHits[4]; // 0 for fst | 1-3 for ist
  std::vector<HIT> hitVec[4];
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    numOfHits[i_layer] = 0;
    hitVec[i_layer].clear();
  }

  for(int i_hit = 0; i_hit < hitVec_orig.size(); ++i_hit)
  { // set temp ist hit container for each layer
    int layer = hitVec_orig[i_hit].layer;
    numOfHits[layer]++;
    hitVec[layer].push_back(hitVec_orig[i_hit]);
  }

  if(numOfHits[0] > 0 && numOfHits[1] > 0 && numOfHits[3] > 0)
  { // only do tracking when at least 1 hit is found in fst & ist1 & ist3
    // cout << "numOfHits[0] = " << numOfHits[0] << ", hitVec.size = " << hitVec[0].size() << endl;
    // cout << "numOfHits[1] = " << numOfHits[1] << ", hitVec.size = " << hitVec[1].size() << endl;
    // cout << "numOfHits[2] = " << numOfHits[2] << ", hitVec.size = " << hitVec[2].size() << endl;
    // cout << "numOfHits[3] = " << numOfHits[3] << ", hitVec.size = " << hitVec[3].size() << endl;
    // cout << "hitVec_orig.size = " << hitVec_orig.size() << ", sum of each layer = " << hitVec[0].size()+hitVec[1].size()+hitVec[2].size()+hitVec[3].size() << endl;
    // cout << endl;

    // QA Histograms
    h_mHitsCorrXR[0]->Fill(hitVec[1][0].column,hitVec[3][0].column);
    h_mHitsCorrXR[1]->Fill(hitVec[1][0].column,hitVec[0][0].column);
    h_mHitsCorrXR[2]->Fill(hitVec[3][0].column,hitVec[0][0].column);
    h_mHitsCorrXR[3]->Fill(5.25/2*hitVec[1][0].column-3.25/2*hitVec[3][0].column,hitVec[0][0].column);

    h_mHitsCorrYPhi[0]->Fill(hitVec[1][0].row,hitVec[3][0].row);
    h_mHitsCorrYPhi[1]->Fill(hitVec[1][0].row,hitVec[0][0].row);
    h_mHitsCorrYPhi[2]->Fill(hitVec[3][0].row,hitVec[0][0].row);
    h_mHitsCorrYPhi[3]->Fill(5.25/2*hitVec[1][0].row-3.25/2*hitVec[3][0].row+20,hitVec[0][0].row);

    double r_fst = FST::rOuter + (hitVec[0][0].column-4)*FST::pitchR + 0.5*FST::pitchR;
    double phi_fst = (63-hitVec[0][0].row)*FST::pitchPhi + 0.5*FST::pitchPhi;
    double x0_fst = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
    double y0_fst = r_fst*TMath::Sin(phi_fst); // y = r*sin(phi)
    double z0_fst = FST::pitchLayer03;

    double x1_ist = hitVec[1][0].column*FST::pitchColumn + 0.5*FST::pitchColumn;
    double y1_ist = (63-hitVec[1][0].row)*FST::pitchRow + 0.5*FST::pitchRow;
    double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;

    double x3_ist = hitVec[3][0].column*FST::pitchColumn + 0.5*FST::pitchColumn;
    double y3_ist = (63-hitVec[3][0].row)*FST::pitchRow + 0.5*FST::pitchRow;
    double z3_ist = 0.0;

    double x0_proj = x3_ist + (x1_ist-x3_ist)*z0_fst/z1_ist;
    double y0_proj = y3_ist + (y1_ist-y3_ist)*z0_fst/z1_ist;

    h_mXResidual_Hits_Before->Fill(x0_fst-x0_proj);
    h_mYResidual_Hits_Before->Fill(y0_fst-y0_proj);

    if(isSaveHits)
    {
      file_mHits << x0_fst << "    " << y0_fst << "    " << x1_ist << "    " << y1_ist<< "    " << x3_ist << "    " << y3_ist  << endl;
    }

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

  return true;
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
//--------------Tracking with Hits---------------------

//--------------Tracking with Clusters---------------------
bool FstTracking::initTracking_Clusters()
{
  h_mXResidual_Simple = new TH1F("h_mXResidual_Simple","h_mXResidual_Simple",150,-9,9);
  h_mYResidual_Simple = new TH1F("h_mYResidual_Simple","h_mYResidual_Simple",150,-9,9);

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    std::string HistName = Form("h_mAdc_Simple_Layer%d",i_layer);
    h_mAdc_Simple[i_layer] = new TH1F(HistName.c_str(),HistName.c_str(),100,0,4000);

    HistName = Form("h_mAdcAngleCorr_Simple_Layer%d",i_layer);
    h_mAdcAngleCorr_Simple[i_layer] = new TH1F(HistName.c_str(),HistName.c_str(),100,0,4000);
  }

  h_mTrackAngle_Simple = new TH1F("h_mTrackAngle_Simple","h_mTrackAngle_Simple",100,0,90);

  return true;
}

void FstTracking::writeTracking_Clusters()
{
  h_mXResidual_Simple->Write();
  h_mYResidual_Simple->Write();

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mAdc_Simple[i_layer]->Write();
    h_mAdcAngleCorr_Simple[i_layer]->Write();
  }

  h_mTrackAngle_Simple->Write();
}
//--------------Tracking with Clusters---------------------

//--------------Utility---------------------
int FstTracking::getLayer(int arm, int port)
{
  if(arm == 0 && port == 0) return 1; // top IST
  if(arm == 0 && port == 1) return 2; // middle IST
  if(arm == 1 && port == 0) return 3; // bottom IST
  if(arm == 1 && port == 1) return 0; // FST module

  return -1;
}

int FstTracking::getSensor(int arm, int port, int apv)
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

int FstTracking::getColumn(int arm, int port, int apv, int ch)
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

int FstTracking::getRow(int arm, int port, int apv, int ch)
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

int FstTracking::getRStrip(int apv, int ch)
{
  int r_strip = -1;

  // only apply to half outer sector for now
  if(apv == 4) r_strip = ch%4;
  if(apv == 5) r_strip = 3-ch%4;

  return r_strip;
}

int FstTracking::getPhiSeg(int apv, int ch)
{
  int phi_seg = -1;

  if(apv >= 0 && apv <= 3) phi_seg = apv*32 + ch/4; // inner
  if(apv >= 4 && apv <= 7) phi_seg = (apv-4)*32 + ch/4; // outer

  return phi_seg;
}

bool FstTracking::isBadAPV(int arm, int port, int apv)
{
  bool bAPV = false;

  if(arm == 0 && port==0 && (apv == 5||apv == 6)) bAPV = true;
  if(arm == 0 && port==1 && (apv <= 9)) bAPV = true;
  // if(arm == 1 && port==0 && (apv == 0||apv == 1)) bAPV = true;
  if(arm == 1 && port==0 && (apv == 0)) bAPV = true;
  // if(arm == 1 && port==1 && (apv == 6||apv == 7)) bAPV = true;

  return bAPV;
}

void FstTracking::printHits(std::vector<HIT> hitVec)
{
  cout << "Number of Hits: " << hitVec.size() << endl;
  for(int i_hit = 0; i_hit < hitVec.size(); ++i_hit)
  {
    cout << "hitVec.layer  = " << hitVec[i_hit].layer << endl;
    cout << "hitVec.sensor = " << hitVec[i_hit].sensor << endl;
    cout << "hitVec.column = " << hitVec[i_hit].column << endl;
    cout << "hitVec.row    = " << hitVec[i_hit].row << endl;
    cout << "hitVec.maxAdc = " << hitVec[i_hit].maxAdc << endl;
    cout << "hitVec.maxTb  = " << hitVec[i_hit].maxTb << endl;
    cout << "hitVec.filled = " << hitVec[i_hit].filled << endl;
  }
}

void FstTracking::printClusters(CLUSTER cluster)
{
  cout << "cluster.layer          = "  << cluster.layer  << endl;
  cout << "cluster.sensor         = " << cluster.sensor << endl;
  cout << "cluster.meanColumn     = " << cluster.meanColumn << endl;
  cout << "cluster.meanRow        = "    << cluster.meanRow << endl;
  cout << "cluster.totAdc         = " << cluster.totAdc << endl;
  cout << "cluster.maxTb          = "  << cluster.maxTb  << endl;
  cout << "cluster.clusterSize    = " << cluster.clusterSize << endl;
  cout << "cluster.clusterSizeR   = " << cluster.clusterSizeR << endl;
  cout << "cluster.clusterSizePhi = " << cluster.clusterSizePhi << endl;
  cout << "cluster.clusterType    = " << cluster.clusterType << endl;
}
//--------------Utility---------------------
