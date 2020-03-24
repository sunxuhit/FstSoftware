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
  mHITsVec.clear();

  return true;
}
//------------------------------------------
int FstTracking::Make()
{
  cout << "FstTracking::Make => " << endl;

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
		HIT isthit_temp;
		isthit_temp.layer = getLayer(i_arm,i_port);
		isthit_temp.sensor = getSensor(i_arm,i_port,i_apv);
		isthit_temp.column = getColumn(i_arm,i_port,i_apv,i_ch); // phi for FST, x for IST
		isthit_temp.row = getRow(i_arm,i_port,i_apv,i_ch); // r_strip for FST, y for IST
		isthit_temp.maxAdc = maxADC;
		isthit_temp.maxTb = maxTB;
		isthit_temp.filled = isHit;

		mHITsVec.push_back(isthit_temp);
		numOfHits++;
	      }
	    }
	  }
	}
      }
    }

    if(numOfHits > 0 && numOfHits <= FST::maxNHitsPerEvent) // maximum hits to expect per event is 10
    {
      fillHitDisplay(mHITsVec); // fill hit display

      // findCluster_ARMDisplay(mHITsVec); // find cluster with ARMDisplay
      // findCluster_Simple(mHITsVec); // find cluster with ARMDisplay
    }
  }

  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 1;
}

int FstTracking::Finish()
{
  cout << "FstTracking::Finish => " << endl;
  writePedestal();
  writeHitDisplay();
  // writeTracking_ARMDisplay();

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
    if(i_layer == 0) h_mPedDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5,FST::numRStrip,-0.5,FST::numRStrip-0.5);
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
    if(i_layer == 0) h_mHitDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::numPhiSeg,-0.5,FST::numPhiSeg-0.5,FST::numRStrip,-0.5,FST::numRStrip-0.5);
    else h_mHitDisplay[i_layer] = new TH2F(HistName.c_str(),HistName.c_str(),FST::noColumns,-0.5,FST::noColumns-0.5,FST::noRows,-0.5,FST::noRows-0.5);
    
    HistName = Form("h_mMaxTb_Layer%d",i_layer);
    h_mMaxTb[i_layer] = new TH1F(HistName.c_str(),HistName.c_str(),10,-0.5,9.5); 
  }

  return true;
}

void FstTracking::fillHitDisplay(std::vector<HIT> isthitvec)
{
  if(isthitvec.size() < FST::maxNHits)
  {
    for(int i_hit = 0; i_hit < isthitvec.size(); ++i_hit)
    {
      if(isthitvec[i_hit].filled == true)
      {
	int layer = isthitvec[i_hit].layer;
	h_mHitDisplay[layer]->Fill(isthitvec[i_hit].column,isthitvec[i_hit].row);
	h_mMaxTb[layer]->Fill(isthitvec[i_hit].maxTb);
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


#if 0
//--------------cluster with ARMDisplay---------------------
bool FstTracking::clearCluster_ARMDisplay()
{
  mCLUSTERVec_ARMDisplay.clear();

  return true;
}

bool FstTracking::initTracking_ARMDisplay()
{
  h_mXResidual_ARMDisplay = new TH1F("h_mXResidual_ARMDisplay","h_mXResidual_ARMDisplay",150,-9,9);
  h_mYResidual_ARMDisplay = new TH1F("h_mYResidual_ARMDisplay","h_mYResidual_ARMDisplay",150,-9,9);

  h_mAdc_Layer1_ARMDisplay = new TH1F("h_mAdc_Layer1_ARMDisplay","h_mAdc_Layer1_ARMDisplay",100,0,4000);
  h_mAdc_Layer3_ARMDisplay = new TH1F("h_mAdc_Layer3_ARMDisplay","h_mAdc_Layer3_ARMDisplay",100,0,4000);
  h_mAdcAngleCorr_Layer1_ARMDisplay = new TH1F("h_mAdcAngleCorr_Layer1_ARMDisplay","h_mAdcAngleCorr_Layer1_ARMDisplay",100,0,4000);
  h_mAdcAngleCorr_Layer3_ARMDisplay = new TH1F("h_mAdcAngleCorr_Layer3_ARMDisplay","h_mAdcAngleCorr_Layer3_ARMDisplay",100,0,4000);

  h_mTrackAngle_ARMDisplay = new TH1F("h_mTrackAngle_ARMDisplay","h_mTrackAngle_ARMDisplay",100,0,90);

  return true;
}

bool FstTracking::findCluster_ARMDisplay(std::vector<HIT> isthit_orig)
{
  clearCluster_ARMDisplay();
  int numOfHits = isthit_orig.size();
  std::vector<HIT> isthit;
  isthit.clear();
  isthit.reserve(isthit.size());
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  { // set temp ist hit container
    isthit.push_back(isthit_orig[i_hit]);
  }

  // find cluster
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

  // fill cluster
  int numOfCluster = 0;
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  {
    if(isthit[i_hit].filled == true) 
    {
      CLUSTER istcluster_temp;
      istcluster_temp.layer = isthit[i_hit].layer;
      istcluster_temp.sensor = isthit[i_hit].sensor;
      istcluster_temp.meanColumn = isthit[i_hit].column;
      istcluster_temp.meanRow = isthit[i_hit].row;
      istcluster_temp.totAdc = isthit[i_hit].maxAdc;
      istcluster_temp.maxTb = isthit[i_hit].maxTb;
      istcluster_temp.clusterSize = 1; // not real cluster size
      istcluster_temp.clusterSizeR = 1; // not real cluster size
      istcluster_temp.clusterSizePhi = 1; // not real cluster size
      istcluster_temp.clusterType = 0; // ARMDisplay

      mCLUSTERVec_ARMDisplay.push_back(istcluster_temp);
      numOfCluster++;
    }
  }

  if(numOfCluster > 0 && numOfCluster <= FST::maxNHitsPerEvent)
  {
    fillTracking_ARMDisplay(mCLUSTERVec_ARMDisplay);
  }

  return true;
}

void FstTracking::fillTracking_ARMDisplay(std::vector<CLUSTER> istcluster_orig)
{
  int numOfCluster = istcluster_orig.size();
  std::vector<CLUSTER> istcluster;
  istcluster.clear();
  istcluster.reserve(istcluster_orig.size());
  for(int i_cluster = 0; i_cluster < numOfCluster; ++i_cluster)
  { // set temp ist cluster container
    istcluster.push_back(istcluster_orig[i_cluster]);
  }

  for ( int i_cluster_1st = 0; i_cluster_1st < numOfCluster-1; ++i_cluster_1st) 
  {
    for ( int i_cluster_3rd = i_cluster_1st+1; i_cluster_3rd < numOfCluster; ++i_cluster_3rd) 
    {
      TVector3 Normal, Track;
      // Find a hit on layer 1 and a hit on layer 3, we only have a sensor 0 in both layers
      if ( 
	  ( ( istcluster[i_cluster_1st].clusterSize > 0 ) && ( istcluster[i_cluster_3rd].clusterSize > 0 ) ) &&
	  ( ( ( istcluster[i_cluster_1st].layer  ==  1 ) && ( istcluster[i_cluster_3rd].layer  ==  3 ) ) || ( ( istcluster[i_cluster_1st].layer  ==  3 ) && ( istcluster[i_cluster_3rd].layer  ==  1 ) )  )
	 ) 
      {
	float x1 = istcluster[i_cluster_1st].meanColumn*FST::pitchColumn;
	float y1 = istcluster[i_cluster_1st].meanRow*FST::pitchRow;
	float z1;
	if(istcluster[i_cluster_1st].layer == 1) z1 = 0.0;
	if(istcluster[i_cluster_1st].layer == 3) z1 = FST::pitchLayer12+FST::pitchLayer23;

	float x3 = istcluster[i_cluster_3rd].meanColumn*FST::pitchColumn;
	float y3 = istcluster[i_cluster_3rd].meanRow*FST::pitchRow;
	float z3;
	if(istcluster[i_cluster_3rd].layer == 3) z3 = FST::pitchLayer12+FST::pitchLayer23;
	if(istcluster[i_cluster_3rd].layer == 1) z3 = 0.0;


	Normal.SetXYZ(0, 0, z3-z1);
	Track.SetXYZ(x3-x1, y3-y1, z3-z1);
	h_mTrackAngle_ARMDisplay->Fill(FST::rad2deg*Track.Angle(Normal), 1.0);
	// Cut on maximum track angle to weed out invalid 2-hit combinations a bit
	if ( FST::rad2deg*Track.Angle(Normal) < FST::maxAngle ) 
	{
	  // not angle corrected hits on a track
	  h_mAdc_Layer1_ARMDisplay->Fill(istcluster[i_cluster_1st].totAdc, 1.0);
	  h_mAdc_Layer3_ARMDisplay->Fill(istcluster[i_cluster_3rd].totAdc, 1.0);
	  // angle corrected hits on a track
	  h_mAdcAngleCorr_Layer1_ARMDisplay->Fill((istcluster[i_cluster_1st].totAdc)*cos(Track.Angle(Normal)), 1.0);
	  h_mAdcAngleCorr_Layer3_ARMDisplay->Fill((istcluster[i_cluster_3rd].totAdc)*cos(Track.Angle(Normal)), 1.0);
	  // Do the residuals of the found track with the hit from Layer 2
	  for ( int i_cluster_2nd = 0; i_cluster_2nd < numOfCluster; ++i_cluster_2nd) 
	  {
	    if ( ( istcluster[i_cluster_2nd].clusterSize > 0) &&
		( istcluster[i_cluster_2nd].layer  ==  2 )
	       ) 
	    {
	      // Calculate randomized position of hit
	      float x2 = istcluster[i_cluster_2nd].meanColumn*FST::pitchColumn;
	      float y2 = istcluster[i_cluster_2nd].meanRow*FST::pitchRow;
	      float z2 = FST::pitchLayer12;

	      float x2proj = x1 + (x3-x1)*FST::pitchLayer12/(FST::pitchLayer12+FST::pitchLayer23);
	      float y2proj = y1 + (y3-y1)*FST::pitchLayer12/(FST::pitchLayer12+FST::pitchLayer23);
	      float z2proj = FST::pitchLayer12 ;
	      // Calculate residual
	      //Residual = sqrt( (x2-x2proj)*(x2-x2proj) + (y2-y2proj)*(y2-y2proj) );
	      float xResidual = (x2-x2proj);
	      float yResidual = (y2-y2proj);
	      h_mXResidual_ARMDisplay->Fill(xResidual, 1.0);
	      h_mYResidual_ARMDisplay->Fill(yResidual, 1.0);
	    }
	  }
	}
      }
    }
  }
}

void FstTracking::writeTracking_ARMDisplay()
{
  h_mXResidual_ARMDisplay->Write();
  h_mYResidual_ARMDisplay->Write();

  h_mAdc_Layer1_ARMDisplay->Write();
  h_mAdc_Layer3_ARMDisplay->Write();
  h_mAdcAngleCorr_Layer1_ARMDisplay->Write();
  h_mAdcAngleCorr_Layer3_ARMDisplay->Write();

  h_mTrackAngle_ARMDisplay->Write();
}
//--------------cluster with ARMDisplay---------------------
#endif

#if 0
//--------------cluster with Simple Algorithm---------------------
bool FstTracking::clearCluster_Simple()
{
  mCLUSTERVec_Simple.clear();

  return true;
}

bool FstTracking::initTracking_Simple()
{
  h_mXResidual_Simple = new TH1F("h_mXResidual_Simple","h_mXResidual_Simple",150,-9,9);
  h_mYResidual_Simple = new TH1F("h_mYResidual_Simple","h_mYResidual_Simple",150,-9,9);

  h_mAdc_Layer1_Simple = new TH1F("h_mAdc_Layer1_Simple","h_mAdc_Layer1_Simple",100,0,4000);
  h_mAdc_Layer3_Simple = new TH1F("h_mAdc_Layer3_Simple","h_mAdc_Layer3_Simple",100,0,4000);
  h_mAdcAngleCorr_Layer1_Simple = new TH1F("h_mAdcAngleCorr_Layer1_Simple","h_mAdcAngleCorr_Layer1_Simple",100,0,4000);
  h_mAdcAngleCorr_Layer3_Simple = new TH1F("h_mAdcAngleCorr_Layer3_Simple","h_mAdcAngleCorr_Layer3_Simple",100,0,4000);

  h_mTrackAngle_Simple = new TH1F("h_mTrackAngle_Simple","h_mTrackAngle_Simple",100,0,90);

  return true;
}

bool FstTracking::findCluster_Simple(std::vector<HIT> isthit_orig)
{
  clearCluster_Simple();
  int numOfHits = isthit_orig.size();
  std::vector<HIT> isthit;
  isthit.clear();
  isthit.reserve(isthit_orig.size());
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  { // set temp ist hit container
    isthit.push_back(isthit_orig[i_hit]);
  }

  // find cluster
  if(isthit.size() > 0)
  {
    // set 1st cluster to 1st hit
    std::vector<HIT>::iterator hitIt = isthit.begin();

    CLUSTER istcluster_temp;
    istcluster_temp.layer        = (*hitIt).layer;
    istcluster_temp.sensor       = (*hitIt).sensor;
    istcluster_temp.meanColumn   = (*hitIt).column;
    istcluster_temp.meanRow      = (*hitIt).row;
    istcluster_temp.totAdc       = (*hitIt).maxAdc;
    istcluster_temp.maxTb        = (*hitIt).maxTb;
    istcluster_temp.clusterSize  = 1;
    istcluster_temp.clusterSizeR = 1;
    istcluster_temp.clusterSizePhi = 1;
    istcluster_temp.clusterType  = 1; // Simple

    mCLUSTERVec_Simple.push_back(istcluster_temp);

    std::vector<HIT> isthit_saved; // save hits for ith cluster
    isthit_saved.clear();
    isthit_saved.push_back( (*hitIt) );

    isthit.erase(hitIt); // remove 1st hit from ist hit container

    /*
    if( isthit.size() !=0)
    {
      float weight, tempSumAdc;
      std::vector<CLUSTER>::iterator clusterIt = mCLUSTERVec_Simple.begin(); // get the begin of cluster container

      //loop the existed clusters vector constainer
      while( clusterIt != mCLUSTERVec_Simple.end() && !isthit.empty() )
      {
      }
    }
    */
  }

  /*
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
  */

  /*
  // fill cluster
  int numOfCluster = 0;
  for(int i_hit = 0; i_hit < numOfHits; ++i_hit)
  {
    if(isthit[i_hit].filled == true) 
    {
      CLUSTER istcluster_temp;
      istcluster_temp.layer = isthit[i_hit].layer;
      istcluster_temp.sensor = isthit[i_hit].sensor;
      // istcluster_temp.x = isthit[i_hit].column*FST::pitchColumn + (FST::pitchColumn*rand()/double(RAND_MAX));
      // istcluster_temp.y = isthit[i_hit].row*FST::pitchRow + (FST::pitchRow*rand()/double(RAND_MAX));
      // if( isthit[i_hit].layer == 1) istcluster_temp.z = 0.0;
      // if( isthit[i_hit].layer == 2) istcluster_temp.z = FST::pitchLayer12;
      // if( isthit[i_hit].layer == 3) istcluster_temp.z = FST::pitchLayer12+FST::pitchLayer23;
      istcluster_temp.x = isthit[i_hit].column*FST::pitchColumn;
      istcluster_temp.y = isthit[i_hit].row*FST::pitchRow;
      if( isthit[i_hit].layer == 1) istcluster_temp.z = 0.0;
      if( isthit[i_hit].layer == 2) istcluster_temp.z = FST::pitchLayer12;
      if( isthit[i_hit].layer == 3) istcluster_temp.z = FST::pitchLayer12+FST::pitchLayer23;
      istcluster_temp.adc = isthit[i_hit].maxAdc;
      istcluster_temp.filled = true;

      mCLUSTERVec_Simple.push_back(istcluster_temp);
      numOfCluster++;
    }
  }

  if(numOfCluster > 0 && numOfCluster <= FST::maxNHitsPerEvent)
  {
    fillTracking_ARMDisplay(mCLUSTERVec_ARMDisplay);
  }
  */

  return true;
}

void FstTracking::writeTracking_Simple()
{
  h_mXResidual_Simple->Write();
  h_mYResidual_Simple->Write();

  h_mAdc_Layer1_Simple->Write();
  h_mAdc_Layer3_Simple->Write();
  h_mAdcAngleCorr_Layer1_Simple->Write();
  h_mAdcAngleCorr_Layer3_Simple->Write();

  h_mTrackAngle_Simple->Write();
}
//--------------cluster with Simple Algorithm---------------------
#endif

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

  if(layer == 0)
  { // layer = 0 for FST
    col = this->getPhiSeg(apv,ch);
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
  int sensor = this->getSensor(arm,port,apv);

  if(layer == 0)
  { // layer = 0 for FST
    if(this->getRStrip(apv,ch) >= 0)
    {
      row = 4*sensor + this->getRStrip(apv,ch);
    }
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
  if(arm == 1 && port==0 && (apv == 0||apv == 1)) bAPV = true;
  // if(arm == 1 && port==1 && (apv == 6||apv == 7)) bAPV = true;

  return bAPV;
}
//--------------Utility---------------------
