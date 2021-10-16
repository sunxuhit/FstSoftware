#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

int testOnlineZS()
{
  // initialize the group used for CMN calculation
  int cmnGroup[24][128]; // group used for CMN calculation 
  string inputCMN = "./cmnGroup.txt";
  std::ifstream file_cmnGroup ( inputCMN.c_str() );
  if ( !file_cmnGroup.is_open() )
  {
    std::cout << "Abort. Fail to read in CMN Group file: " << inputCMN << std::endl;
    return 0;
  }
  else
  {
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
  }
  file_cmnGroup.close();

  // read in pedestal and noise
  float pedestal[6][3][24][128][9];
  float totStdDev[6][3][24][128][9];
  float ranStdDev[6][3][24][128][9];
  for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
  {
    for(int i_arm = 0; i_arm < 3; ++i_arm)
    {
      for(int i_apv = 0; i_apv < 24; ++i_apv)
      {
	for(int i_ch = 0; i_ch < 128; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < 9; ++i_tb)
	  {
	    pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	    totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	    ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	  }
	}
      }
    }
  }
  string inputPedestal = "./pedestal.txt";
  std::ifstream file_pedestal( inputPedestal.c_str() );
  if ( !file_pedestal.is_open() )
  {
    cout << "Abort. Fail to read in Pedestal file: " << inputPedestal.c_str() << endl;
    return 0;
  }
  else
  {
    int rdoIdxTemp=-1, armIdxTemp=-1, apvIdxTemp=-1, chanIdxTemp=-1, tbIdxTemp=-1;
    float pedestalTemp = -1.0, totStdDevTemp=-1.0, ranStdDevTemp=-1.0;
    std::cout << "read in pedestal: " << std::endl;
    string header;
    getline(file_pedestal,header);
    // cout << header.c_str() << endl;;
    while (file_pedestal >> rdoIdxTemp >> armIdxTemp >> apvIdxTemp >> chanIdxTemp >> tbIdxTemp >> pedestalTemp >> totStdDevTemp >> ranStdDevTemp)
    {
      // cout << "rdoIdxTemp = " << rdoIdxTemp << ", armIdxTemp = " << armIdxTemp << ", apvIdxTemp = " << apvIdxTemp << ", chanIdxTemp = " << chanIdxTemp << ", tbIdxTemp = " << tbIdxTemp << ", pedestalTemp = " << pedestalTemp << ", totStdDevTemp = " << totStdDevTemp << ", ranStdDevTemp = " << ranStdDevTemp << endl;
      pedestal[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = pedestalTemp;
      totStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = totStdDevTemp;
      ranStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = ranStdDevTemp;
    }
  }
  file_pedestal.close();


  // read in ADC info for all events
  float adc[100][6][3][24][128][9];
  for(int i_event = 0; i_event < 100; ++i_event)
  {
    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_arm = 0; i_arm < 3; ++i_arm)
      {
	for(int i_apv = 0; i_apv < 24; ++i_apv)
	{
	  for(int i_ch = 0; i_ch < 128; ++i_ch)
	  {
	    for(int i_tb = 0; i_tb < 9; ++i_tb)
	    {
	      adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb] = -1;
	    }
	  }
	}
      }
    }
  }
  int numEvent = -1;

  FILE *file_testEvent;
  char inputfile[256]; 
  sprintf(inputfile, "./testEventAdc.txt");
  cout << "inputfile: " << inputfile << endl;
  file_testEvent = fopen(inputfile,"r");
  if(file_testEvent == 0) 
  {
    cout << "can't open input file " << inputfile << endl;;
  }
  else
  {
    cout << "Start to read in " << inputfile << endl;;
    while(!feof(file_testEvent)) 
    {
      int rdoIdxTemp=-1, armIdxTemp=-1, apvIdxTemp=-1, chanIdxTemp=-1, tbIdxTemp=-1, adcIdxTemp=-1;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_testEvent) == NULL) continue ;
      int ret = sscanf(buff,"%d %d %d %d %d %d",&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&adcIdxTemp);
      // cout << "ret = " << ret << ", chanIdxTemp = " << chanIdxTemp << ", adcIdxTemp = " << adcIdxTemp << endl;
      if(ret != 6) {numEvent++; cout << "read in event: " << numEvent << endl; continue;}
      if(numEvent > 0)
      {
	adc[numEvent-1][rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = adcIdxTemp;
      }
    }
  }
  fclose(file_testEvent);

  // Apply Zero-Suppression
  { 
    cout << "=====>Apply Zero-Suppression...." << endl;
    const float nPedsCut1 = 3.5; // 1 time bin
    const float nPedsCut2 = 2.5; // 2 time bin
    const float nSeedsCut = 4.0; // 2 time bin
    
    string outputSeed = "./testZsEventSeedAdc.txt";
    ofstream file_testZsEventSeed;
    file_testZsEventSeed.open(outputSeed.c_str());
    file_testZsEventSeed << "rdo    " << "arm    " << "apv    " << "channel    " << "tb    " << "adcZS" << endl;

    string outputHit = "./testZsEventHitAdc.txt";
    ofstream file_testZsEventHit;
    file_testZsEventHit.open(outputHit.c_str());
    file_testZsEventHit << "rdo    " << "arm    " << "apv    " << "channel    " << "tb    " << "adcZS" << endl;

    // event loop
    for(int i_event = 0; i_event < numEvent; ++i_event)
    {
      file_testZsEventSeed << "EventId " << i_event << endl;
      file_testZsEventHit  << "EventId " << i_event << endl;
      // initialize event-by-event CMN
      float cmnEvt[6][3][24][128][9];
      float sumEvt[6][3][24][4][9];
      int couEvt[6][3][24][4][9];
      for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
      {
	for(int i_arm = 0; i_arm < 3; ++i_arm)
	{
	  for(int i_apv = 0; i_apv < 24; ++i_apv)
	  {
	    for(int i_tb = 0; i_tb < 9; ++i_tb)
	    {
	      for(int i_group = 0; i_group < 4; ++i_group)
	      {
		couEvt[i_rdo][i_arm][i_apv][i_group][i_tb] = 0;
		sumEvt[i_rdo][i_arm][i_apv][i_group][i_tb] = 0.0;
	      }
	      for(int i_ch = 0; i_ch < 128; ++i_ch)
	      {
		cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	      }
	    }
	  }
	}
      }

      // calculate event-by-event CMN
      for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
      {
	for(int i_arm = 0; i_arm < 3; ++i_arm)
	{
	  for(int i_apv = 0; i_apv < 24; ++i_apv)
	  {
	    for(int i_ch = 0; i_ch < 128; ++i_ch)
	    {
	      for(int i_tb = 0; i_tb < 9; ++i_tb)
	      {
		int adcTemp = adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb];
		float pedTemp = pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float devTemp = totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb];

		if( (adcTemp < pedTemp+3.0*devTemp) && ( adcTemp >= 0 && adcTemp < 4096) )
		{ // exclude hits (adc < pedestal + 3*totStdDev)
		  int groupIdx = cmnGroup[i_apv][i_ch];
		  sumEvt[i_rdo][i_arm][i_apv][groupIdx][i_tb] += adcTemp-pedTemp;
		  couEvt[i_rdo][i_arm][i_apv][groupIdx][i_tb]++;
		  // if(i_rdo == 0 && i_arm == 2 && i_apv == 19 && i_tb == 7) cout << "i_apv = " << i_apv << ", i_ch = " << i_ch << ", groupIdx = " << groupIdx << endl;
		}
	      }
	    }
	  }
	}
      }
      for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
      { // convert group CMN to corresponding channels
	for(int i_arm = 0; i_arm < 3; ++i_arm)
	{
	  for(int i_apv = 0; i_apv < 24; ++i_apv)
	  {
	    for(int i_ch = 0; i_ch < 128; ++i_ch)
	    {
	      for(int i_tb = 0; i_tb < 9; ++i_tb)
	      {
		int groupIdx = cmnGroup[i_apv][i_ch];
		if(couEvt[i_rdo][i_arm][i_apv][groupIdx][i_tb] > 0)
		{
		  cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb] = sumEvt[i_rdo][i_arm][i_apv][groupIdx][i_tb]/couEvt[i_rdo][i_arm][i_apv][groupIdx][i_tb];
		  // if(i_rdo == 0 && i_arm == 2 && i_apv == 19 && i_tb == 7) cout << "i_apv = " << i_apv << ", i_ch = " << i_ch << ", groupIdx = " << groupIdx << ", cmn = " << cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb] << endl;
		}
	      }
	    }
	  }
	}
      }

      // calculate zero-suppressed ADC
      int couSeed[6][3][24][128]; // counter (time bin) for Seed >=2
      int couPed1[6][3][24][128]; // counter (time bin) for Ped1 >=1
      int couPed2[6][3][24][128]; // counter (time bin) for Ped2 >=2
      for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
      {
	for(int i_arm = 0; i_arm < 3; ++i_arm)
	{
	  for(int i_apv = 0; i_apv < 24; ++i_apv)
	  {
	    for(int i_ch = 0; i_ch < 128; ++i_ch)
	    {
	      couSeed[i_rdo][i_arm][i_apv][i_ch] = 0;
	      couPed1[i_rdo][i_arm][i_apv][i_ch] = 0;
	      couPed2[i_rdo][i_arm][i_apv][i_ch] = 0;
	    }
	  }
	}
      }

      for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
      {
	for(int i_arm = 0; i_arm < 3; ++i_arm)
	{
	  for(int i_apv = 0; i_apv < 24; ++i_apv)
	  {
	    for(int i_ch = 0; i_ch < 128; ++i_ch)
	    {
	      for(int i_tb = 0; i_tb < 9; ++i_tb)
	      {
		int adcTemp   = adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb];
		float pedTemp = pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float devTemp = totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float ranTemp = ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float cmnTemp = cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb];

		if(adcTemp >=0 && adcTemp <4096)
		{
		  float adcZS = adcTemp - pedTemp - cmnTemp;
		  if(adcZS > nSeedsCut*ranTemp) couSeed[i_rdo][i_arm][i_apv][i_ch]++;
		  if(adcZS > nPedsCut1*ranTemp) couPed1[i_rdo][i_arm][i_apv][i_ch]++;
		  if(adcZS > nPedsCut2*ranTemp) couPed2[i_rdo][i_arm][i_apv][i_ch]++;
		}
	      }

	      if(couSeed[i_rdo][i_arm][i_apv][i_ch] >= 2) // only save seed hits
	      {
		for(int i_tb = 0; i_tb < 9; ++i_tb)
		{
		  file_testZsEventSeed << i_rdo+1 << "    " << i_arm << "    "  << i_apv << "    "  << i_ch << "    "  << i_tb << "    "  << adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb] - pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] - cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb] << endl;;
		}
	      }
	      if(couSeed[i_rdo][i_arm][i_apv][i_ch] >= 2 || couPed1[i_rdo][i_arm][i_apv][i_ch] >= 1 || couPed2[i_rdo][i_arm][i_apv][i_ch] >= 2) // save both seed hits and recovery hits
	      {
		for(int i_tb = 0; i_tb < 9; ++i_tb)
		{
		  file_testZsEventHit << i_rdo+1 << "    " << i_arm << "    "  << i_apv << "    "  << i_ch << "    "  << i_tb << "    "  << adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb] - pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] - cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb] << endl;;
		}
	      }
	    }
	  }
	}
      }
    } // end of event loop
    file_testZsEventSeed.close();
  }
  
  return 1;
}
