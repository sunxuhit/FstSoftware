#include "./FstNoiseStudy.h"

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

ClassImp(FstNoiseStudy)

//------------------------------------------

FstNoiseStudy::FstNoiseStudy() : mList("../../list/FST/FstPed_HV70.list"), mOutPutFile("./FstPed_HV70.root")
{
  cout << "FstNoiseStudy::FstNoiseStudy() -------- Constructor!  --------" << endl;
  mHome = getenv("HOME");
}

FstNoiseStudy::~FstNoiseStudy()
{
  cout << "FstNoiseStudy::~FstNoiseStudy() -------- Release Memory!  --------" << endl;
}

//------------------------------------------
int FstNoiseStudy::Init()
{
  cout << "FstNoiseStudy::Init => " << endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  bool isInPut = initChain(); // initialize input data/ped TChain;
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

bool FstNoiseStudy::initChain()
{
  cout << "FstNoiseStudy::initChain -> " << endl;

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

//------------------------------------------
int FstNoiseStudy::Make()
{
  cout << "FstNoiseStudy::Make => " << endl;

  bool isPed = calPedestal(); // calculate pedestals with first 1000 events
  if( !isPed ) 
  {
    cout << "No Pedestal!!!" << endl;
    return -1;
  }

  return 1;
}

int FstNoiseStudy::Finish()
{
  cout << "FstNoiseStudy::Finish => " << endl;
  writePedestal();

  return 1;
}

//--------------pedestal---------------------
bool FstNoiseStudy::clearPedestal()
{
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	{
	  for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
	  {
	    mPed[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	    mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	    mPedRMS[i_arm][i_port][i_apv][i_ch][i_tb] = -1.0;
	  }
	  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
	  {
	    mNoiseCMN[i_arm][i_port][i_apv][i_rstrip][i_tb] = -1.0;
	    mNoiseDiff[i_arm][i_port][i_apv][i_rstrip][i_tb] = -1.0;
	  }
	}
      }
    }
  }

  return true;
}

bool FstNoiseStudy::initPedestal()
{
  cout << "FstNoiseStudy::initPedestal -> " << endl;

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

      HistName = Form("h_mPedMean_FST_RStrip%d_TimeBin%d",i_layer,i_tb);
      h_mPedMean_FST[i_layer][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),FST::noRows,-0.5,FST::noRows-0.5);

      HistName = Form("h_mPedSigma_FST_RStrip%d_TimeBin%d",i_layer,i_tb);
      h_mPedSigma_FST[i_layer][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),FST::noRows,-0.5,FST::noRows-0.5);
    }
  }

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    std::string gName = Form("g_mNoiseCMN_FST_TimeBin%d",i_tb);
    g_mNoiseCMN[i_tb] = new TGraph();
    g_mNoiseCMN[i_tb]->SetName(gName.c_str());

    gName = Form("g_mNoiseDiff_FST_TimeBin%d",i_tb);
    g_mNoiseDiff[i_tb] = new TGraph();
    g_mNoiseDiff[i_tb]->SetName(gName.c_str());

    for(int i_apv = 0; i_apv < 2; ++i_apv)
    {
      for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
      {
	std::string HistName = Form("h_mNoiseCMN_FST_Apv%d_RStrip%d_TimeBin%d",i_apv+4,i_rstrip+4,i_tb);
	h_mNoiseCMN[i_apv][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),100,-200.0,200.0);

	HistName = Form("h_mNoiseCMN_Evt_FST_Apv%d_RStrip%d_TimeBin%d",i_apv+4,i_rstrip+4,i_tb);
	h_mNoiseCMN_Evt[i_apv][i_rstrip][i_tb] = new TH1F(HistName.c_str(),HistName.c_str(),100,-200.0,200.0);
      }
    }
  }

  return clearPedestal();
}

bool FstNoiseStudy::calPedestal()
{
  cout << "FstNoiseStudy::calPedestal -> " << endl;
  // cout << " Only use first 1000 event for Pedstal Calculation!" << endl;

  int NumOfEvents = (int)mChainInPut->GetEntries();
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
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
	    if(layer == 0 && col > 3) 
	    { // FST
	      h_mPedMean_FST[col-4][i_tb]->SetBinContent(row+1,mPed[i_arm][i_port][i_apv][i_ch][i_tb]);
	      h_mPedSigma_FST[col-4][i_tb]->SetBinContent(row+1,mPedStdDev[i_arm][i_port][i_apv][i_ch][i_tb]);
	    }
	  }
	}
      }
    }
  }
  //--------------------------------------------------------

  //--------------------------------------------------------
  // 3rd loop to find Commen Mode Noist and Differetial Noise excluding Hits
  cout << "=====>Third Pedestal Pass...." << endl;

  //  Calculate a rolling average and standard deviation
  int counters_CMN[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins];
  double sumValues_CMN[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins];
  double sumValuesSquared_CMN[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins];
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    counters_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb] = 0;
	    sumValues_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb] = 0.0;
	    sumValuesSquared_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb] = 0.0;
	  }
	}
      }
    }
  }

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  { 
    // if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    // calculate a rolling average for each event => CMN distribution
    int counters_evt[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins];
    double sumValues_evt[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins];
    for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < FST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
	{
	  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
	  {
	    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	    {
	      counters_evt[i_arm][i_port][i_apv][i_rstrip][i_tb] = 0;
	      sumValues_evt[i_arm][i_port][i_apv][i_rstrip][i_tb] = 0.0;
	    }
	  }
	}
      }
    }

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

		int layer = this->getLayer(arm,port);
		int col = this->getColumn(arm,port,apv,ch);
		int row = this->getRow(arm,port,apv,ch);
		if ( (adc < mPed[arm][port][apv][ch][tb]+nPedsCut*mPedStdDev[arm][port][apv][ch][tb]) && ( adc >= 0 && adc < 4096)  && layer == 0)
		{ // only adc below ped+3sigma on FST are considered for 3rd loop
		  sumValues_CMN[arm][port][apv][col][tb] += adc-mPed[arm][port][apv][ch][tb];
		  sumValuesSquared_CMN[arm][port][apv][col][tb] += (adc-mPed[arm][port][apv][ch][tb])*(adc-mPed[arm][port][apv][ch][tb]);
		  counters_CMN[arm][port][apv][col][tb]++;

		  sumValues_evt[arm][port][apv][col][tb] += adc-mPed[arm][port][apv][ch][tb];
		  counters_evt[arm][port][apv][col][tb]++;
		  h_mNoiseCMN[apv-4][col-4][tb]->Fill(adc-mPed[arm][port][apv][ch][tb]);
		}
	      // }
	    }
	  }
	}
      }
    }

    for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
    {
      for(int i_port = 0; i_port < FST::numPorts; ++i_port)
      {
	for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
	{
	  for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
	  {
	    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	    {
	      if(counters_evt[i_arm][i_port][i_apv][i_rstrip][i_tb] > 0) // eject bad channels
	      {
		h_mNoiseCMN_Evt[i_apv-4][i_rstrip-4][i_tb]->Fill(sumValues_evt[i_arm][i_port][i_apv][i_rstrip][i_tb]/counters_evt[i_arm][i_port][i_apv][i_rstrip][i_tb]);
	      }
	    }
	  }
	}
      }
    }
  }

  // calculate Commen Mode Noise & Differetial Noise excluding Hits
  for(int i_arm = 0; i_arm < FST::numARMs; ++i_arm)
  {
    for(int i_port = 0; i_port < FST::numPorts; ++i_port)
    {
      for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
      {
	for(int i_rstrip = 0; i_rstrip < FST::numRStrip; ++i_rstrip)
	{
	  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
	  {
	    if(counters_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb] > 0) // eject bad channels
	    {
	      // cout << "i_arm = " << i_arm << "i_port = " << i_port << ", i_apv = " << i_apv << ", i_rstrip = " << i_rstrip << ", i_tb = " << i_tb << ", sumValues_CMN = " << sumValues_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb] << ", counters_CMN = " << counters_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb] << endl;
	      mNoiseCMN[i_arm][i_port][i_apv][i_rstrip][i_tb] = sumValues_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb]/counters_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb];
	      mNoiseDiff[i_arm][i_port][i_apv][i_rstrip][i_tb] = sqrt((sumValuesSquared_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb]-(double)counters_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb]*mNoiseCMN[i_arm][i_port][i_apv][i_rstrip][i_tb]*mNoiseCMN[i_arm][i_port][i_apv][i_rstrip][i_tb])/(double)(counters_CMN[i_arm][i_port][i_apv][i_rstrip][i_tb]-1));

	      g_mNoiseCMN[i_tb]->SetPoint(i_apv*FST::numRStrip+i_rstrip,i_apv*FST::numRStrip+i_rstrip,mNoiseCMN[i_arm][i_port][i_apv][i_rstrip][i_tb]);
	      g_mNoiseDiff[i_tb]->SetPoint(i_apv*FST::numRStrip+i_rstrip,i_apv*FST::numRStrip+i_rstrip,mNoiseDiff[i_arm][i_port][i_apv][i_rstrip][i_tb]);
	    }
	  }
	}
      }
    }
  }
  //--------------------------------------------------------


  return true;
}

void FstNoiseStudy::writePedestal()
{
  cout << "FstNoiseStudy::writePedestal => save ped mean & sigma!" << endl;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
    {
      g_mPedMean[i_layer][i_tb]->Write();
      g_mPedSigma[i_layer][i_tb]->Write();
      h_mPedDisplay[i_layer][i_tb]->Write();
      h_mPedMean_FST[i_layer][i_tb]->Write();
      h_mPedSigma_FST[i_layer][i_tb]->Write();
    }
  }

  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    g_mNoiseCMN[i_tb]->Write();
    g_mNoiseDiff[i_tb]->Write();
    for(int i_apv = 0; i_apv < 2; ++i_apv)
    {
      for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
      {
	h_mNoiseCMN[i_apv][i_rstrip][i_tb]->Write();
	h_mNoiseCMN_Evt[i_apv][i_rstrip][i_tb]->Write();
      }
    }
  }
}
//--------------pedestal---------------------

//--------------Utility---------------------
int FstNoiseStudy::getLayer(int arm, int port)
{
  if(arm == 0 && port == 0) return 1; // top IST
  if(arm == 0 && port == 1) return 2; // middle IST
  if(arm == 1 && port == 0) return 3; // bottom IST
  if(arm == 1 && port == 1) return 0; // FST module

  return -1;
}

int FstNoiseStudy::getSensor(int arm, int port, int apv)
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

int FstNoiseStudy::getColumn(int arm, int port, int apv, int ch)
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

int FstNoiseStudy::getRow(int arm, int port, int apv, int ch)
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

int FstNoiseStudy::getRStrip(int apv, int ch)
{
  int r_strip = -1;

  // only apply to half outer sector for now
  if(apv == 4) r_strip = ch%4;
  if(apv == 5) r_strip = 3-ch%4;

  return r_strip;
}

int FstNoiseStudy::getPhiSeg(int apv, int ch)
{
  int phi_seg = -1;

  if(apv >= 0 && apv <= 3) phi_seg = apv*32 + ch/4; // inner
  if(apv >= 4 && apv <= 7) phi_seg = (apv-4)*32 + ch/4; // outer

  return phi_seg;
}

double FstNoiseStudy::getPosX(int arm, int port, int apv, int ch)
{
  double posX = -999.9;
  int layer = this->getLayer(arm,port);
  if(layer == 0) // FST
  {
    // double r_fst = FST::rOuter + (this->getColumn(arm,port,apv,ch)-4)*FST::pitchR + 0.5*FST::pitchR;
    // double phi_fst = (63-this->getRow(arm,port,apv,ch))*FST::pitchPhi + 0.5*FST::pitchPhi;
    // posX = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
    posX = FST::rOuter + (this->getColumn(arm,port,apv,ch)-4)*FST::pitchR + 0.5*FST::pitchR;
  }
  else // IST1-3
  {
    posX = this->getColumn(arm,port,apv,ch)*FST::pitchColumn + 0.5*FST::pitchColumn;
  }

  return posX;
}

double FstNoiseStudy::getPosY(int arm, int port, int apv, int ch)
{
  double posY = -999.9;
  int layer = this->getLayer(arm,port);
  if(layer == 0) // FST
  {
    // double r_fst = FST::rOuter + (this->getColumn(arm,port,apv,ch)-4)*FST::pitchR + 0.5*FST::pitchR;
    // double phi_fst = (63-this->getRow(arm,port,apv,ch))*FST::pitchPhi + 0.5*FST::pitchPhi;
    // posY = r_fst*TMath::Sin(phi_fst); // y = r*sin(phi)
    posY = (63-this->getRow(arm,port,apv,ch))*FST::pitchPhi + 0.5*FST::pitchPhi;
  }
  else // IST1-3
  {
    posY = (63-this->getRow(arm,port,apv,ch))*FST::pitchRow + 0.5*FST::pitchRow;
  }

  return posY;
}

double FstNoiseStudy::getMeanX(int layer, double meanColumn)
{
  double meanX = -999.9;
  if(layer == 0) // FST
  {
    // double r_fst = FST::rOuter + (this->getColumn(arm,port,apv,ch)-4)*FST::pitchR + 0.5*FST::pitchR;
    // double phi_fst = (63-this->getRow(arm,port,apv,ch))*FST::pitchPhi + 0.5*FST::pitchPhi;
    // posX = r_fst*TMath::Cos(phi_fst); // x = r*cos(phi)
    meanX = FST::rOuter + (meanColumn-4)*FST::pitchR + 0.5*FST::pitchR;
  }
  else // IST1-3
  {
    meanX = meanColumn*FST::pitchColumn + 0.5*FST::pitchColumn;
  }

  return meanX;
}

double FstNoiseStudy::getMeanY(int layer, double meanRow)
{
  double meanY = -999.9;
  if(layer == 0) // FST
  {
    meanY = (63-meanRow)*FST::pitchPhi + 0.5*FST::pitchPhi;
  }
  else // IST1-3
  {
    meanY = (63-meanRow)*FST::pitchRow + 0.5*FST::pitchRow;
  }

  return meanY;
}

bool FstNoiseStudy::isBadAPV(int arm, int port, int apv)
{
  bool bAPV = false;

  if(arm == 0 && port==0 && (apv == 5||apv == 6)) bAPV = true;
  if(arm == 0 && port==1 && (apv <= 9)) bAPV = true;
  // if(arm == 1 && port==0 && (apv == 0||apv == 1)) bAPV = true;
  if(arm == 1 && port==0 && (apv == 0)) bAPV = true;
  if(arm == 1 && port==1 && (apv == 6||apv == 7)) bAPV = true;

  return bAPV;
}
//--------------Utility---------------------
