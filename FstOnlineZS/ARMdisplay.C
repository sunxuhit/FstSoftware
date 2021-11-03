// includes {{{
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

using namespace std ;


#include <rtsLog.h>	// for my LOG() call
#include <rtsSystems.h>

#include <curses.h>

// this needs to be always included
#include <DAQ_READER/daqReader.h>
#include <DAQ_READER/daq_dta.h>

// only the detectors we will use need to be included
// for their structure definitions...
#include <DAQ_FGT/daq_fgt.h>

#include "TFile.h"
#include "TVector3.h"
#include "TChain.h"
#include "TTree.h"
#include "TApplication.h"
#include "TGraphErrors.h"
#include "TGraph2DErrors.h"
#include "TStyle.h"
#include "TH2.h"
#include "TGraph2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TBrowser.h"
#include <TROOT.h>

// }}}

//-------------------------------------------------------------------------------
// PARAMETERS THAT CAN BE CHANGED WITH SOME CAUTION
int STAVEID = 4;
int DATE 	= 130402;
int TIME	= 1400;
bool SENSOR	= 0;
int BIAS	= 60;
int VPSP	= 43; 

// MAXIMUM NUMBER OF EVENTS THAT ARE PROCESSED
// NOTE THAT THIS IS _NOT_ THE NUMBER EVENTS FOR PEDESTAL CALCULATION
const int numEvents = 500000;

int SEL_LO_APV1 =  0;
int SEL_HI_APV1 =  11;
int MAX_NO_HITS_PER_EVENT = 10;

// NUMBER OF EVENTS THAT SHOULD BE USED TO CALCULATE THE PEDESTALS
// Note, we should make up our mind whether to generate the pedestals
// using numEvents events or to use a subset, this has consequences
// for the broken event determination!
const int numPeds     = 1000;
const int numPedsDown = 0;
const int numPedsUp   = 500;
// THIS IS THE PEDESTAL CUT!
// Raw ADC values outside ( pedestal +- PEDCUT*sigma) are excluded from the second pedestal pass.
int PEDCUT = 3;

// THIS IS THE HIT CUT!
// Pedestal substracted ADC values above ( HITCUT*sigma ) are accepted as a hit
int HITCUT = 5;

// DEAD CHANNEL CRITERIUM = any channel with less than 10 ADC noise is declared dead.
const double MinimumNoise = 10.0;

// DON'T CHANGE ANYTHING BELOW THIS LINE
//-------------------------------------------------------------------------------
const int UseTimeBin = 3;

// This is the timebin which is going to be used to calculate the pedestals
const int PedTimeBin = 0;

const int numRDOs     = 6;
const int numARMs     = 3;
const int numAPVs     = 24;
const int numChannels = 128;
const int numTBins    = 9;
const int numGroups   = 4; // group to calculate CMN

char * mountpoint = NULL;

// Any event where any average APV pedestal goes outside of 3 sigma should be marked broken
bool BrokenEvent[numEvents];

//  To keep track of where we are and what we are doing.
daqReader *rdr = NULL;
// Event handle
char *argv1;
long long currentEvent = -1;

static int cmnGroup[24][128]; // group used for CMN calculation
static int run_type = 3; // 1 for pedestal | 3 for zs

void load_group()
{
  // initialize the group used for CMN calculation
  string inputCMN = "./cmnGroup.txt";
  std::ifstream file_cmnGroup ( inputCMN.c_str() );
  if ( !file_cmnGroup.is_open() )
  {
    std::cout << "Abort. Fail to read in CMN Group file: " << inputCMN << std::endl;
    exit(0);
  }

  int temp_apvIdx = -1, temp_chIdx = -1, temp_groupIdx = -1;
  std::cout << "reading group Index: " << std::endl;
  string header;
  getline(file_cmnGroup,header);
  // cout << header.c_str() << endl;;
  while (file_cmnGroup >> temp_apvIdx >> temp_chIdx >> temp_groupIdx)
  {
    // cout << "apvIdx = " << temp_apvIdx << ", chIdx = " << temp_chIdx << ", groupIdx = " << temp_groupIdx << endl;
    cmnGroup[temp_apvIdx][temp_chIdx] = temp_groupIdx;
  }
  file_cmnGroup.close();
}

void CalculatePedestals(char* daqFileName, char* mountpoint )
{
  cout << "===> Calculate Pedestals....." << endl;
  cout << "===> Load in CMN Groups....." << endl;
  load_group();

  const int defRdoIdx   = 1;

  const double nSigmaCut = 3.0; // reject signal

  double pedestals[numARMs][numAPVs][numChannels][numTBins]; // pedestal
  double pedStdDev[numARMs][numAPVs][numChannels][numTBins]; // total noise
  double ranStdDev[numARMs][numAPVs][numChannels][numTBins]; // random noise
  double cmnStdDev[numARMs][numAPVs][numGroups][numTBins]; // common mode noise

  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_tb = 0; i_tb < numTBins; ++i_tb)
      {
	for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	{
	  pedestals[i_arm][i_apv][i_ch][i_tb] = -1.0;
	  pedStdDev[i_arm][i_apv][i_ch][i_tb] = -1.0;
	  ranStdDev[i_arm][i_apv][i_ch][i_tb] = -1.0;
	}
	for(int i_group = 0; i_group < numGroups; ++i_group)
	{
	  cmnStdDev[i_arm][i_apv][i_group][i_tb] = -1.0;
	}
      }
    }
  }

  //  Calculate a rolling average and standard deviation
  int counters[numARMs][numAPVs][numChannels][numTBins];
  double sum[numARMs][numAPVs][numChannels][numTBins];
  double sum2[numARMs][numAPVs][numChannels][numTBins];
  //  Calculate a rolling average and standard deviation
  int counters_CMN[numARMs][numAPVs][numGroups][numTBins];
  double sum_CMN[numARMs][numAPVs][numGroups][numTBins];
  double sum2_CMN[numARMs][numAPVs][numGroups][numTBins];

  int count;

  // Sometimes events are corrupted, don't know how to detect this
  // I think these events can result in something that looks like common mode noise
  // Anyway, any event that has any APV exhibiting more than (RejectCMN * sigmaCMN) will be flagged
  //   as broken and taken out of the pedestal generation and out of the subsequent analysis
  // We start with all events being accepted
  for ( int i = 0; i < numEvents; i++ ) {
    BrokenEvent[i] = false;
  }


  // First pass over the data to find pedestals including hits
  cout << "=====>First Pedestal Pass...." << endl;

  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_ch = 0; i_ch < numChannels; ++i_ch)
      {
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  counters[i_arm][i_apv][i_ch][i_tb] = 0;
	  sum[i_arm][i_apv][i_ch][i_tb]      = 0.0;
	  sum2[i_arm][i_apv][i_ch][i_tb]     = 0.0;
	}
      }
    }
  }

  //  Setting up the daq reader.
  daqReader *rdr1 = new daqReader(daqFileName);
  if(mountpoint) rdr1->setEvpDisk(mountpoint);
  //  Reading EVENTS.
  rdr1->get(0,EVP_TYPE_ANY);

  // run over numPeds events (if possible)
  count = 0;
  while( rdr1->status != EVP_STAT_EOR && count < numPeds )
  {
    ++count;

    daq_dta *dd ;
    dd = rdr1->det("fst")->get("adc");

    if( count >= numPedsDown && count < numPedsUp )	
    {
      while( dd && dd->iterate() ) 
      {
	fgt_adc_t *f = (fgt_adc_t *) dd->Void;

	int rdoIdx  = dd->rdo; // rdoIdx: 1-6
	int armIdx  = dd->sec; // armIdx: 0-2
	int apvIdx  = dd->pad; // apvIdx: 0-7 && 12-19

	if(rdoIdx == defRdoIdx)
	{
	  for( u_int ii = 0; ii < dd->ncontent; ii++ ) 
	  {
	    int chIdx = f[ii].ch;  // chIdx: 0-127
	    int tbIdx = f[ii].tb;  // tbIdx: 0-8
	    int adc   = f[ii].adc; // adc: 0-4095

	    // if(count == 1) cout << "rdoIdx = " << rdoIdx << ", armIdx = " << armIdx << ", apvIdx = " << apvIdx << ", chIdx = " << chIdx << ", tbIdx = " << tbIdx << ", adc = " << adc << endl;

	    sum[armIdx][apvIdx][chIdx][tbIdx] += adc;
	    sum2[armIdx][apvIdx][chIdx][tbIdx] += adc * adc;
	    counters[armIdx][apvIdx][chIdx][tbIdx]++;
	  }
	}
      }
    } // event number cut
    rdr1->get(0,EVP_TYPE_ANY);
  } // end of event loop

  cout << "============>Ready with reading events " << endl;
  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_ch = 0; i_ch < numChannels; ++i_ch)
      {
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  if(counters[i_arm][i_apv][i_ch][i_tb] > 0)
	  {
	    double meanAdc = sum[i_arm][i_apv][i_ch][i_tb]/counters[i_arm][i_apv][i_ch][i_tb];
	    double sumSAdc = sum2[i_arm][i_apv][i_ch][i_tb];
	    int coutAdc   = counters[i_arm][i_apv][i_ch][i_tb];
	    pedestals[i_arm][i_apv][i_ch][i_tb] = meanAdc; // mean adc
	    // pedStdDev[i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(double)coutAdc*meanAdc*meanAdc)/(double)(coutAdc-1)); // sample standard deviation
	    pedStdDev[i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(double)coutAdc*meanAdc*meanAdc)/(double)(coutAdc)); // sample standard deviation
	  }
	}
      }
    }
  }

  delete rdr1;
  // end of first pedestal pass
  cout << "Number of Events: " << count << endl;


#if 0
  // Second pass over the data to find pedestals excluding hits
  cout << "=====>Second Pedestal Pass...." << endl;
  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_ch = 0; i_ch < numChannels; ++i_ch)
      {
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  counters[i_arm][i_apv][i_ch][i_tb] = 0;
	  sum[i_arm][i_apv][i_ch][i_tb]      = 0.0;
	  sum2[i_arm][i_apv][i_ch][i_tb]     = 0.0;
	}
      }
    }
  }

  //  Setting up the daq reader.
  daqReader *rdr2 = new daqReader(daqFileName);
  if(mountpoint) rdr2->setEvpDisk(mountpoint);
  //  Reading EVENTS.
  rdr2->get(0,EVP_TYPE_ANY);

  // Run over numPeds events (if possible)       
  count = 0;
  while( rdr2->status != EVP_STAT_EOR && count < numPeds )
  {
    ++count;

    daq_dta *dd ;
    dd = rdr2->det("fst")->get("adc");

    if( count >= numPedsDown && count < numPedsUp )        
    {
      //	Processing RDO/ARM/APVs.
      while( dd && dd->iterate() ) 
      {
	fgt_adc_t *f = (fgt_adc_t *) dd->Void;

	int rdoIdx  = dd->rdo; // rdoIdx: 1-6
	int armIdx  = dd->sec; // armIdx: 0-2
	int apvIdx  = dd->pad; // apvIdx: 0-7 && 12-19

	if(rdoIdx == defRdoIdx)
	{
	  for( u_int ii = 0; ii < dd->ncontent; ii++ ) 
	  {
	    int chIdx = f[ii].ch;  // chIdx: 0-127
	    int tbIdx = f[ii].tb;  // tbIdx: 0-8
	    int adc   = f[ii].adc; // adc: 0-4095

	    // if(count == 1) cout << "rdoIdx = " << rdoIdx << ", armIdx = " << armIdx << ", apvIdx = " << apvIdx << ", chIdx = " << chIdx << ", tbIdx = " << tbIdx << ", adc = " << adc << endl;
	    double pedTemp = pedestals[armIdx][apvIdx][chIdx][tbIdx];
	    double stdTemp = pedStdDev[armIdx][apvIdx][chIdx][tbIdx];

	    if ( (adc < pedTemp + nSigmaCut*stdTemp) && (adc >= 0 && adc < 4096) ) 
	    { // excluding hits (adc < pedestal + 3*pedStdDev)
	      sum[armIdx][apvIdx][chIdx][tbIdx] += adc;
	      sum2[armIdx][apvIdx][chIdx][tbIdx] += adc * adc;
	      counters[armIdx][apvIdx][chIdx][tbIdx]++;
	    }
	  }
	}
      }
    } // event number cut
    rdr2->get(0,EVP_TYPE_ANY);
  } // end of event loop

  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_ch = 0; i_ch < numChannels; ++i_ch)
      {
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  if(counters[i_arm][i_apv][i_ch][i_tb] > 0)
	  {
	    double meanAdc = sum[i_arm][i_apv][i_ch][i_tb]/counters[i_arm][i_apv][i_ch][i_tb];
	    double sumSAdc = sum2[i_arm][i_apv][i_ch][i_tb];
	    int coutAdc   = counters[i_arm][i_apv][i_ch][i_tb];
	    pedestals[i_arm][i_apv][i_ch][i_tb] = meanAdc; // mean adc
	    // pedStdDev[i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(double)coutAdc*meanAdc*meanAdc)/(double)(coutAdc-1)); // sample standard deviation
	    pedStdDev[i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(double)coutAdc*meanAdc*meanAdc)/(double)(coutAdc)); // sample standard deviation
	  }
	}
      }
    }
  }
  delete rdr2;
  // end of second pedestal pass
#endif

  // Third pass over the data to find ranStdDev and cmnStdDev
  cout << "=====>Third Pedestal Pass...." << endl;
  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_tb = 0; i_tb < numTBins; ++i_tb)
      {
	for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	{
	  counters[i_arm][i_apv][i_ch][i_tb] = 0;
	  sum[i_arm][i_apv][i_ch][i_tb]      = 0.0;
	  sum2[i_arm][i_apv][i_ch][i_tb]     = 0.0;
	}
	for(int i_group = 0; i_group < numGroups; ++i_group)
	{
	  counters_CMN[i_arm][i_apv][i_group][i_tb] = 0;
	  sum_CMN[i_arm][i_apv][i_group][i_tb]      = 0.0;
	  sum2_CMN[i_arm][i_apv][i_group][i_tb]     = 0.0;
	}
      }
    }
  }

  //  Setting up the daq reader.
  daqReader *rdr3 = new daqReader(daqFileName);
  if(mountpoint) rdr3->setEvpDisk(mountpoint);
  //  Reading EVENTS.
  rdr3->get(0,EVP_TYPE_ANY);

  // Run over numPeds events (if possible)       
  count = 0;
  double cmnEvt[numARMs][numAPVs][numChannels][numTBins];
  double sumEvt[numARMs][numAPVs][numGroups][numTBins];
  int couEvt[numARMs][numAPVs][numGroups][numTBins];
  while( rdr3->status != EVP_STAT_EOR && count < numPeds )
  {

    ++count;

    daq_dta *dd ;
    dd = rdr3->det("fst")->get("adc");

    if( count >= numPedsUp+1 && count < numPeds+1)        
    {
      // initialize event-by-event CMN
      for(int i_arm = 0; i_arm < numARMs; ++i_arm)
      {
	for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
	{
	  for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	  {
	    for(int i_group = 0; i_group < numGroups; ++i_group)
	    {
	      couEvt[i_arm][i_apv][i_group][i_tb] = 0;
	      sumEvt[i_arm][i_apv][i_group][i_tb] = 0.0;
	    }
	    for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	    {
	      cmnEvt[i_arm][i_apv][i_ch][i_tb] = 0.0;
	    }
	  }
	}
      }

      // Common Mode Noise
      while( dd && dd->iterate() ) 
      {
	fgt_adc_t *f = (fgt_adc_t *) dd->Void;

	int rdoIdx  = dd->rdo; // rdoIdx: 1-6
	int armIdx  = dd->sec; // armIdx: 0-2
	int apvIdx  = dd->pad; // apvIdx: 0-7 && 12-19

	if(rdoIdx == defRdoIdx)
	{
	  for( u_int ii = 0; ii < dd->ncontent; ii++ ) 
	  { // calculate event-by-event CMN
	    int chIdx = f[ii].ch;  // chIdx: 0-127
	    int tbIdx = f[ii].tb;  // tbIdx: 0-8
	    int adc   = f[ii].adc; // adc: 0-4095

	    double pedTemp = pedestals[armIdx][apvIdx][chIdx][tbIdx];
	    double stdTemp = pedStdDev[armIdx][apvIdx][chIdx][tbIdx];

	    int groupIdx = cmnGroup[apvIdx][chIdx];
	    if(groupIdx < 0) continue;
	    if ( (adc < pedTemp + nSigmaCut*stdTemp) && (adc >= 0 && adc < 4096) ) 
	    { // excluding hits (adc < pedestal + 3*pedStdDev)
	      sumEvt[armIdx][apvIdx][groupIdx][tbIdx] += adc-pedTemp;
	      couEvt[armIdx][apvIdx][groupIdx][tbIdx]++;
	    }
	  }
	}
      }
      // calculate event-by-event CMN
      for(int i_arm = 0; i_arm < numARMs; ++i_arm)
      {
	for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
	{
	  for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	  {
	    for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	    {
	      int groupIdx = cmnGroup[i_apv][i_ch];
	      if(groupIdx < 0) continue;
	      if(couEvt[i_arm][i_apv][groupIdx][i_tb] > 0)
	      {
		cmnEvt[i_arm][i_apv][i_ch][i_tb] = sumEvt[i_arm][i_apv][groupIdx][i_tb]/couEvt[i_arm][i_apv][groupIdx][i_tb];
	      }
	      if(couEvt[i_arm][i_apv][groupIdx][i_tb] <= 0)
	      {
		// cout << "bad channel? i_arm " <<  i_arm << " i_apv " << i_apv << " groupIdx " << groupIdx << " i_tb " << i_tb << endl;
	      }
	    }
	  }
	}
      }
      // calculate cmnStdDev
      for(int i_arm = 0; i_arm < numARMs; ++i_arm)
      {
	for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
	{
	  for(int i_group = 0; i_group < numGroups; ++i_group)
	  {
	    for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	    {
	      if(couEvt[i_arm][i_apv][i_group][i_tb] > 0) // eject bad channels
	      {
		double cmnTemp = sumEvt[i_arm][i_apv][i_group][i_tb]/couEvt[i_arm][i_apv][i_group][i_tb];
		sum_CMN[i_arm][i_apv][i_group][i_tb] += cmnTemp;
		sum2_CMN[i_arm][i_apv][i_group][i_tb] += cmnTemp*cmnTemp;
		counters_CMN[i_arm][i_apv][i_group][i_tb]++;
	      }
	    }
	  }
	}
      }

      daq_dta *dd3 ;
      dd3 = rdr3->det("fst")->get("adc");
      // Calculate Random Noise
      while( dd3 && dd3->iterate() ) 
      {
	fgt_adc_t *f = (fgt_adc_t *) dd3->Void;

	int rdoIdx  = dd3->rdo; // rdoIdx: 1-6
	int armIdx  = dd3->sec; // armIdx: 0-2
	int apvIdx  = dd3->pad; // apvIdx: 0-7 && 12-19

	if(rdoIdx == defRdoIdx)
	{
	  for( u_int ii = 0; ii < dd3->ncontent; ii++ ) 
	  {
	    int chIdx = f[ii].ch;  // chIdx: 0-127
	    int tbIdx = f[ii].tb;  // tbIdx: 0-8
	    int adc   = f[ii].adc; // adc: 0-4095

	    double pedTemp = pedestals[armIdx][apvIdx][chIdx][tbIdx];
	    double stdTemp = pedStdDev[armIdx][apvIdx][chIdx][tbIdx];
	    double cmnTemp = cmnEvt[armIdx][apvIdx][chIdx][tbIdx];

	    // ranStdDev
	    if ( (adc < pedTemp + nSigmaCut*stdTemp) && (adc >= 0 && adc < 4096) ) 
	    { // excluding hits (adc < pedestal + 3*pedStdDev)
	      sum[armIdx][apvIdx][chIdx][tbIdx] += adc-pedTemp-cmnTemp;
	      sum2[armIdx][apvIdx][chIdx][tbIdx] += (adc-pedTemp-cmnTemp) * (adc-pedTemp-cmnTemp);
	      counters[armIdx][apvIdx][chIdx][tbIdx]++;
	    }
	  }
	}
      }
    } // event number cut
    rdr3->get(0,EVP_TYPE_ANY);
  } // end of event loop

  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_tb = 0; i_tb < numTBins; ++i_tb)
      {
	for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	{
	  if(counters[i_arm][i_apv][i_ch][i_tb] > 0)
	  {
	    double meanAdc = sum[i_arm][i_apv][i_ch][i_tb]/counters[i_arm][i_apv][i_ch][i_tb];
	    double sumSAdc = sum2[i_arm][i_apv][i_ch][i_tb];
	    int coutAdc   = counters[i_arm][i_apv][i_ch][i_tb];
	    // ranStdDev[i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(double)coutAdc*meanAdc*meanAdc)/(double)(coutAdc-1));
	    ranStdDev[i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(double)coutAdc*meanAdc*meanAdc)/(double)(coutAdc));
	  }
	}
	for(int i_group = 0; i_group < numGroups; ++i_group)
	{
	  if(counters_CMN[i_arm][i_apv][i_group][i_tb] > 0) // eject bad channels
	  {
	    double meanCMN = sum_CMN[i_arm][i_apv][i_group][i_tb]/counters_CMN[i_arm][i_apv][i_group][i_tb];
	    double sumSCMN = sum2_CMN[i_arm][i_apv][i_group][i_tb];
	    int coutCMN = counters_CMN[i_arm][i_apv][i_group][i_tb];
	    // cmnStdDev[i_arm][i_apv][i_group][i_tb] = sqrt((sumSCMN-(double)coutCMN*meanCMN*meanCMN)/(double)(coutCMN-1));
	    cmnStdDev[i_arm][i_apv][i_group][i_tb] = sqrt((sumSCMN-(double)coutCMN*meanCMN*meanCMN)/(double)(coutCMN));
	  }
	}
      }
    }
  }
  delete rdr3;
  // end of second pedestal pass

  cout << "=====>Generate Output File...." << endl;
  string outputfile = "./pedestal.txt";
  ofstream file_pedestal;
  file_pedestal.open(outputfile.c_str());

  file_pedestal << "rdo    " << "arm    " << "apv    " << "channel    " << "tb    " << "pedestal    " << "totRMS   "   << "ranRMS    " << "cmnRMS" << endl;
  file_pedestal << std::fixed;
  file_pedestal << std::setprecision(3);
  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_ch = 0; i_ch < numChannels; ++i_ch)
      {
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  int groupIdx = cmnGroup[i_apv][i_ch];
	  if(groupIdx < 0) continue;
	  file_pedestal << defRdoIdx << "    " << i_arm << "    "  << i_apv << "    "  << i_ch << "    "  << i_tb <<   "    "  << pedestals[i_arm][i_apv][i_ch][i_tb] << "    "  << pedStdDev[i_arm][i_apv][i_ch][i_tb] << "      "  << ranStdDev[i_arm][i_apv][i_ch][i_tb] << "    " << cmnStdDev[i_arm][i_apv][groupIdx][i_tb] << endl;
	}
      }
    }
  }
  file_pedestal.close();

  FILE *f;
  f = fopen("pedestal_test.txt","w") ;
  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_ch = 0; i_ch < numChannels; ++i_ch)
      {
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  int groupIdx = cmnGroup[i_apv][i_ch];
	  if(groupIdx < 0) continue;
	  // fprintf(f,"%d %d %2d %3d %2d %7.3f %.3f %.3f %.3f\n",1,i_arm,i_apv,i_ch,i_tb,pedestals[i_arm][i_apv][i_ch][i_tb],pedStdDev[i_arm][i_apv][i_ch][i_tb],ranStdDev[i_arm][i_apv][i_ch][i_tb],cmnStdDev[i_arm][i_apv][groupIdx][i_tb]) ;
	  fprintf(f,"%d %d %2d %3d %2d %7.3f %.3f %.3f\n",1,i_arm,i_apv,i_ch,i_tb,pedestals[i_arm][i_apv][i_ch][i_tb],pedStdDev[i_arm][i_apv][i_ch][i_tb],ranStdDev[i_arm][i_apv][i_ch][i_tb]) ;
	}
      }
    }
  }
  fclose(f);
}

void do_zs(char* daqFileName)
{
  cout << "===> Do ZS....." << endl;
  cout << "===> Load in CMN Groups....." << endl;
  load_group();

  const int defRdoIdx   = 1;

  const double nSigmaCut = 3.0; // reject signal
  const double nSeedsCut = 4.0; // seed hits
  const int nSeedsTbs   = 2;
  const double nPedsCut1 = 3.5; // recovery hits
  const int nPedsTbs1   = 1;
  const double nPedsCut2 = 2.5; // recovery hits
  const int nPedsTbs2   = 2;

  // readin pedestal file
  double pedestals[numARMs][numAPVs][numChannels][numTBins]; // pedestal
  double pedStdDev[numARMs][numAPVs][numChannels][numTBins]; // total noise
  double ranStdDev[numARMs][numAPVs][numChannels][numTBins]; // random noise

  for(int i_arm = 0; i_arm < numARMs; ++i_arm)
  {
    for(int i_apv = 0; i_apv < numAPVs; ++i_apv)
    {
      for(int i_tb = 0; i_tb < numTBins; ++i_tb)
      {
	for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	{
	  pedestals[i_arm][i_apv][i_ch][i_tb] = -1.0;
	  pedStdDev[i_arm][i_apv][i_ch][i_tb] = -1.0;
	  ranStdDev[i_arm][i_apv][i_ch][i_tb] = -1.0;
	}
      }
    }
  }

  string inputPedestal = "./pedestal.txt";
  std::ifstream file_pedestal( inputPedestal.c_str() );
  if ( !file_pedestal.is_open() )
  {
    cout << "Abort. Fail to read in Pedestal file: " << inputPedestal.c_str() << endl;
    exit(0);
  }
  else
  {
    int rdoIdxTemp=-1, armIdxTemp=-1, apvIdxTemp=-1, chanIdxTemp=-1, tbIdxTemp=-1;
    float pedestalTemp = -1.0, pedStdDevTemp=-1.0, ranStdDevTemp=-1.0, cmnStdDevTemp=-1.0;
    std::cout << "read in pedestal: " << std::endl;
    string header;
    getline(file_pedestal,header);
    // cout << header.c_str() << endl;;
    while (file_pedestal >> rdoIdxTemp >> armIdxTemp >> apvIdxTemp >> chanIdxTemp >> tbIdxTemp >> pedestalTemp >> pedStdDevTemp >> ranStdDevTemp >> cmnStdDevTemp)
    {
      pedestals[armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = pedestalTemp;
      pedStdDev[armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = pedStdDevTemp;
      ranStdDev[armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = ranStdDevTemp;
    }
  }
  file_pedestal.close();

  // cout << "1 2 15 39 4 " << pedestals[2][15][39][4] << " " << pedStdDev[2][15][39][4] << " " << ranStdDev[2][15][39][4] << endl;

  // output ZS file
  string outputfile = "./test_zs.txt";
  ofstream file_testZs;
  file_testZs.open(outputfile.c_str());

  //  Setting up the daq reader.
  daqReader *rdr1 = new daqReader(daqFileName);
  //  Reading EVENTS.
  rdr1->get(0,EVP_TYPE_ANY);

  // run over numPeds events (if possible)
  int count = 0;
  double sumEvt[numGroups][numTBins];
  int couEvt[numGroups][numTBins];
  double cmnEvt[numChannels][numTBins];
  u_short dtaEvt[numChannels][numTBins];   // cache adc for each APV per event

  while( rdr1->status != EVP_STAT_EOR && count < numPeds )
  {
    ++count;

    daq_dta *dd ;
    dd = rdr1->det("fst")->get("adc");

    while( dd && dd->iterate() ) 
    {
      int rdoIdx  = dd->rdo; // rdoIdx: 1-6
      int armIdx  = dd->sec; // armIdx: 0-2
      int apvIdx  = dd->pad; // apvIdx: 0-7 && 12-19

      fgt_adc_t *f = (fgt_adc_t *) dd->Void;

      if(rdoIdx == defRdoIdx)
      {
	file_testZs << "Event " << count << ": ARC " << rdoIdx << ", ARM " << armIdx << ", APV " << apvIdx << endl; 
	// initialize event-by-event CMN for single APV
	for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	{
	  for(int i_group = 0; i_group < numGroups; ++i_group)
	  {
	    couEvt[i_group][i_tb] = 0;
	    sumEvt[i_group][i_tb] = 0.0;
	  }
	  for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	  {
	    cmnEvt[i_ch][i_tb] = 0.0;
	    dtaEvt[i_ch][i_tb] = 0;
	  }
	}

	// first pass, accumulate for the event-by-event CMN
	for( u_int ii = 0; ii < dd->ncontent; ii++ ) 
	{
	  int chIdx    = f[ii].ch;  // chIdx: 0-127
	  int tbIdx    = f[ii].tb;  // tbIdx: 0-8
	  int adc      = f[ii].adc; // adc: 0-4095
	  int groupIdx = cmnGroup[apvIdx][chIdx]; // groupIdx: 0-3
	  if(groupIdx < 0) continue;

	  double pedTemp = pedestals[armIdx][apvIdx][chIdx][tbIdx];
	  double stdTemp = pedStdDev[armIdx][apvIdx][chIdx][tbIdx];
	  dtaEvt[chIdx][tbIdx] = adc;

	  if(adc < (pedTemp+nSigmaCut*stdTemp))
	  {
	    sumEvt[groupIdx][tbIdx] += adc - pedTemp;
	    couEvt[groupIdx][tbIdx] ++;
	  }
	}

	// second pass, calculate event-by-event CMN
	for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	{
	  int groupIdx = cmnGroup[apvIdx][i_ch]; // groupIdx: 0-3
	  if(groupIdx < 0) continue;
	  for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	  {
	    if(couEvt[groupIdx][i_tb] > 0) 
	    {
	      cmnEvt[i_ch][i_tb] = sumEvt[groupIdx][i_tb]/couEvt[groupIdx][i_tb];
	    }
	    else 
	    {
	      cmnEvt[i_ch][i_tb] = 10000000.0;
	    }
	  }
	}

	//third pass, calculate ZS adc
	for(int i_ch = 0; i_ch < numChannels; ++i_ch)
	{
	  int groupIdx = cmnGroup[apvIdx][i_ch]; // groupIdx: 0-3
	  if(groupIdx < 0) continue;

	  int couSeed = 0; // counter (time bin) for Seed >=2
	  int couPed1 = 0; // counter (time bin) for Ped1 >=1
	  int couPed2 = 0; // counter (time bin) for Ped2 >=2
	  double adcZS[numTBins];

	  int pass = 0;

	  for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	  {
	    if(cmnEvt[i_ch][i_tb] > 10000.0)
	    { // channel with abnormal CMN
	      pass |= 8;
	      break;
	    }
	  }
	  for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	  {
	    double pedTemp = pedestals[armIdx][apvIdx][i_ch][i_tb];
	    double stdTemp = pedStdDev[armIdx][apvIdx][i_ch][i_tb];
	    double ranTemp = ranStdDev[armIdx][apvIdx][i_ch][i_tb];
	    double cmnTemp = cmnEvt[i_ch][i_tb];

	    double adcZSTemp = 0;

	    if(pass)
	    {
	      adcZSTemp = dtaEvt[i_ch][i_tb] - pedTemp;
	    }
	    else
	    {
	      adcZSTemp = dtaEvt[i_ch][i_tb] - pedTemp - cmnTemp;
	    }
	    if(adcZSTemp > nSeedsCut*ranTemp) couSeed++;
	    if(adcZSTemp > nPedsCut1*ranTemp) couPed1++;
	    if(adcZSTemp > nPedsCut2*ranTemp) couPed2++;
	    adcZS[i_tb] = adcZSTemp;
	  }
	  if(couSeed>=nSeedsTbs) pass |= 1 ;
	  if(couPed1>=nPedsTbs1) pass |= 2 ;
	  if(couPed2>=nPedsTbs2) pass |= 4 ;

	  // dump data
	  if(pass)
	  {
	    for(int i_tb = 0; i_tb < numTBins; ++i_tb)
	    {
	      int adcInt;
	      if(adcZS[i_tb] < 0) adcInt = 0;
	      else adcInt = ((int)(adcZS[i_tb]+0.5))&0xFFF;
	      file_testZs << "... ch " << i_ch << ", tb " << i_tb << ", ADC " << adcInt << ", flags " << pass << endl; 
	    }
	  }
	}
      }
    }
    rdr1->get(0,EVP_TYPE_ANY);

  } // end of event loop
  file_testZs.close();

}

int main(int argc, char *argv[]) 
{

  //==Peel the command line arguments of the program==========================================
  if (argc < 3) { 
    cout << "Minimal usage is -f <infile>" << endl;
    exit(0);
  } else {
    for (int i = 1; i < argc-1; i++) { /* We will iterate over argv[] to get the parameters stored inside.
					* Note that we're starting on 1 because we don't need to know the 
					* path of the program, which is stored in argv[0] */
      if        (string(argv[i]) == "-f") {
	argv1      = argv[i + 1];
	i++;
      } else if (string(argv[i]) == "--apvlo") {
	SEL_LO_APV1 = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "--apvhi") {
	SEL_HI_APV1 = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "--pedhitcut") {
	PEDCUT     = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "--hitcut") {
	HITCUT     = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "--maxhits") {
	MAX_NO_HITS_PER_EVENT = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "-stave") {
	STAVEID  = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "-bias") {
	BIAS     = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "-vpsp") {
	VPSP     = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "-date") {
	DATE     = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "-time") {
	TIME     = atoi(argv[i + 1]);
	i++;
      } else if (string(argv[i]) == "-sensor") {
	if (      string(argv[i+1]) == "on" ) {
	  SENSOR = true;
	} else if ( string(argv[i+1]) == "off" ) {
	  SENSOR = false;
	} else {
	  cout << "Wrong argument for sensor mount status, please try again..." << endl;
	  exit(0);
	}
	i++;
      } else if (string(argv[i]) == "--HELP") {
	cout << "Program Usage " << endl;
      } else {
	cout << "Not enough or invalid arguments, please try again.\n";
	exit(0);
      }
    }
  }
  //=============================================================================

  //  For graphics to work, I think. . .
  TApplication myapp( "Hello World", &argc, argv );
  // argv1 = myapp.Argv(1);

  //gROOT->SetStyle("classic");
  gStyle->SetPalette(1,0);

  rtsLogOutput(RTS_LOG_FILE);
  rtsLogLevel((char *)WARN);

  //	Next, let's set up the pedestals.
  if(run_type == 1) CalculatePedestals( argv1, mountpoint );
  if(run_type == 3) do_zs( argv1 );

  for (;;)
  {
    //  Finally, enter the main event loop. There are a few curses things
    //  that we want to set up first.
    WINDOW *theWin = initscr(); //   Initialize the window for curses.
    cbreak(); //    Turn off required carriage returns.
    noecho(); //    Turn off echoing.

    //  Let's print out a prompt.
    clear();
    int line = 0;
    mvaddstr(line++,0,"Press \"q\" to quit.");

    std::stringstream currentEventString;
    if ( currentEvent < 0 )
    {
      currentEventString << "No current event";
    }
    else
    {
      currentEventString << "Current event #: " << currentEvent;
    }

    mvaddstr(line++,0,currentEventString.str().c_str());
    mvaddstr(line++,0,"> ");

    int currentKey = getch();

    switch (currentKey)
    {
      case 'q':
	endwin();
	return 0;
      default:
	break;
    }
  }

  //	Return to normal terminal mode.

  return 0;
}
