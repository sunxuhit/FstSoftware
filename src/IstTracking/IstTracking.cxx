#include "./IstTracking.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include <TFile.h>
#include <TChain.h>
#include <TGraph.h>
#include <TMath.h>

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
	  ped[i_arm][i_port][i_apv][i_ch] = -1.0;
	  pedStdDev[i_arm][i_port][i_apv][i_ch] = -1.0;
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

int IstTracking::Make()
{
  cout << "IstTracking::Make => " << endl;

  calPedestal(); // calculate pedestals with first 1000 events

  // long NumOfEvents = (long)mChainInPut->GetEntries();
  long NumOfEvents = 10;
  mChainInPut->GetEntry(0);

  // int arm = 1;
  // int port = 0;
  // int apv = 9;
  // int ch = 1024;
  // int tb = 3;

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    // cout << "hit_ch = " << hit_ch[arm][port][apv][ch] << endl;
    // cout << "Layer = " << this->getLayer(evt_arm[arm][port][apv],evt_port[arm][port][apv]) << endl;
    // cout << "Colum = " << this->getColumn(evt_apv[arm][port][apv],hit_ch[arm][port][apv][ch]) << endl;
    // cout << "Row = " << this->getRow(evt_apv[arm][port][apv],hit_ch[arm][port][apv][ch]) << endl;
    // cout << endl;

  }

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
  float sumValues[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
  float sumValuesSquared[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];

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
	    for(int i_ch = 0; i_ch < IST::numROChannels; ++i_ch)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ch]; // time bin
	      if(tb == pedTimeBin)
	      {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ch]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ch];
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
	  ped[i_arm][i_port][i_apv][i_ch] = sumValues[i_arm][i_port][i_apv][i_ch]/counters[i_arm][i_port][i_apv][i_ch];
	  pedStdDev[i_arm][i_port][i_apv][i_ch] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch]-(double)counters[i_arm][i_port][i_apv][i_ch]*ped[i_arm][i_port][i_apv][i_ch]*ped[i_arm][i_port][i_apv][i_ch])/(double)(counters[i_arm][i_port][i_apv][i_ch]-1));
	  // pedRMS[i_arm][i_port][i_apv][i_ch] = sqrt(ped[i_arm][i_port][i_apv][i_ch]*ped[i_arm][i_port][i_apv][i_ch]-pedStdDev[i_arm][i_port][i_apv][i_ch]*pedStdDev[i_arm][i_port][i_apv][i_ch]);
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
	    for(int i_ch = 0; i_ch < IST::numROChannels; ++i_ch)
	    {
	      int tb = hit_tb[i_arm][i_port][i_apv][i_ch]; // time bin
	      if(tb == pedTimeBin)
	      {
		int ch = hit_ch[i_arm][i_port][i_apv][i_ch]; // real channel number 
		int adc = hit_adc[i_arm][i_port][i_apv][i_ch];
		if ( (adc < ped[arm][port][apv][ch]+IST::nPedCuts*pedStdDev[arm][port][apv][ch]) && ( adc >= 0 && adc < 4096) )
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
	  ped[i_arm][i_port][i_apv][i_ch] = sumValues[i_arm][i_port][i_apv][i_ch]/counters[i_arm][i_port][i_apv][i_ch];
	  pedStdDev[i_arm][i_port][i_apv][i_ch] = sqrt((sumValuesSquared[i_arm][i_port][i_apv][i_ch]-(double)counters[i_arm][i_port][i_apv][i_ch]*ped[i_arm][i_port][i_apv][i_ch]*ped[i_arm][i_port][i_apv][i_ch])/(double)(counters[i_arm][i_port][i_apv][i_ch]-1));
	  // pedRMS[i_arm][i_port][i_apv][i_ch] = sqrt(ped[i_arm][i_port][i_apv][i_ch]*ped[i_arm][i_port][i_apv][i_ch]-pedStdDev[i_arm][i_port][i_apv][i_ch]*pedStdDev[i_arm][i_port][i_apv][i_ch]);

	  if(getLayer(i_arm,i_port) == 1) g_mPedMean_Layer1->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,ped[i_arm][i_port][i_apv][i_ch]);
	  if(getLayer(i_arm,i_port) == 1) g_mPedSigma_Layer1->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,pedStdDev[i_arm][i_port][i_apv][i_ch]);

	  if(getLayer(i_arm,i_port) == 2) g_mPedMean_Layer2->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,ped[i_arm][i_port][i_apv][i_ch]);
	  if(getLayer(i_arm,i_port) == 2) g_mPedSigma_Layer2->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,pedStdDev[i_arm][i_port][i_apv][i_ch]);

	  if(getLayer(i_arm,i_port) == 3) g_mPedMean_Layer3->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,ped[i_arm][i_port][i_apv][i_ch]);
	  if(getLayer(i_arm,i_port) == 3) g_mPedSigma_Layer3->SetPoint(i_apv*IST::numChannels+i_ch,i_apv*IST::numChannels+i_ch,pedStdDev[i_arm][i_port][i_apv][i_ch]);
	}
      }
    }
  }
  //--------------------------------------------------------

  return true;
}


int IstTracking::Finish()
{
  cout << "IstTracking::Finish => " << endl;
  writePedestal();

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

//------------------------------------------
int IstTracking::getLayer(int arm, int port)
{
  if(arm == 0 && port == 0) return 1; // top IST
  if(arm == 0 && port == 1) return 2; // middle IST
  if(arm == 1 && port == 0) return 3; // bottom IST
  if(arm == 1 && port == 1) return 0; // future FST module

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
