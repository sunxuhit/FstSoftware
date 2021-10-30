#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

int genPedestal()
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
  
  // initialize pedestal, totStdDev and ranStdDev
  float pedestal[6][3][24][128][9];
  float totStdDev[6][3][24][128][9];
  float ranStdDev[6][3][24][128][9];
  float totRMS[6][3][24][128][9];
  float cmnStdDev[6][3][24][4][9];
  for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
  {
    for(int i_arm = 0; i_arm < 3; ++i_arm)
    {
      for(int i_apv = 0; i_apv < 24; ++i_apv)
      {
	for(int i_tb = 0; i_tb < 9; ++i_tb)
	{
	  for(int i_ch = 0; i_ch < 128; ++i_ch)
	  {
	    pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	    totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	    ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	    totRMS[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1.0;
	  }
	  for(int i_group = 0; i_group < 4; ++i_group)
	  {
	    cmnStdDev[i_rdo][i_arm][i_apv][i_group][i_tb] = -1.0;
	  }
	}
      }
    }
  }
  const float nPedsCut = 3.0;

  //  Calculate a rolling average and standard deviation
  int counters[6][3][24][128][9];
  float sumValues[6][3][24][128][9];
  float sumValuesSquared[6][3][24][128][9];
  //  Calculate a rolling average and standard deviation for CMN
  int counters_CMN[6][3][24][4][9];
  float sumValues_CMN[6][3][24][4][9];
  float sumValuesSquared_CMN[6][3][24][4][9];

  // numEvent = 10;

  { // 1st loop for rough pedestal & standard deviation
    cout << "=====>First Pedestal Pass...." << endl;
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
	      counters[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0;
	      sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	      sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	    }
	  }
	}
      }
    }

    // event loop
    for(int i_event = 0; i_event < numEvent; ++i_event)
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
		int adcTemp = adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb];
		// if(i_rdo == 0 && i_arm == 1 && i_apv == 14 && i_ch == 32 && i_tb == 0) cout << "i_event = " << i_event << ", adcTemp = " << adcTemp << endl;
		sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb] += adcTemp;
		sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb] += adcTemp * adcTemp;
		counters[i_rdo][i_arm][i_apv][i_ch][i_tb]++;
	      }
	    }
	  }
	}
      }
    } // end of event loop

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
	      if(counters[i_rdo][i_arm][i_apv][i_ch][i_tb] > 0)
	      {
		float meanAdc = sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb]/counters[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float sumSAdc = sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb];
		int coutAdc   = counters[i_rdo][i_arm][i_apv][i_ch][i_tb];
		pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] = meanAdc; // mean adc
		totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(float)coutAdc*meanAdc*meanAdc)/(float)(coutAdc-1)); // sample standard deviation
		totRMS[i_rdo][i_arm][i_apv][i_ch][i_tb] = sqrt(meanAdc*meanAdc+totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb]*totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb]); // rms^2 = mean^2 + std^2
	      }
	    }
	  }
	}
      }
    }
    // cout << "counters = " << counters[0][1][14][32][0] << ", pedestal = " << pedestal[0][1][14][32][0] << ", totStdDev = " << totStdDev[0][1][14][32][0] << endl;
  }

  { // 2nd loop for pedestal & standard deviation without hits (adc < pedestal + 3*totStdDev)
    cout << "=====>Second Pedestal Pass...." << endl;
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
	      counters[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0;
	      sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	      sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	    }
	  }
	}
      }
    }

    // event loop
    for(int i_event = 0; i_event < numEvent; ++i_event)
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
		int adcTemp   = adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb];
		float pedTemp = pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float devTemp = totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb];

		if( (adcTemp < pedTemp+nPedsCut*devTemp) && ( adcTemp >= 0 && adcTemp < 4096) )
		{ // exclude hits (adc < pedestal + 3*totStdDev)
		  sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb] += adcTemp;
		  sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb] += adcTemp * adcTemp;
		  counters[i_rdo][i_arm][i_apv][i_ch][i_tb]++;
		}
	      }
	    }
	  }
	}
      }
    } // end of event loop

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
	      if(counters[i_rdo][i_arm][i_apv][i_ch][i_tb] > 0)
	      {
		float meanAdc = sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb]/counters[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float sumSAdc = sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb];
		int coutAdc   = counters[i_rdo][i_arm][i_apv][i_ch][i_tb];
		pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] = meanAdc; // mean adc
		totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(float)coutAdc*meanAdc*meanAdc)/(float)(coutAdc-1)); // sample standard deviation
		totRMS[i_rdo][i_arm][i_apv][i_ch][i_tb] = sqrt(meanAdc*meanAdc+totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb]*totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb]); // rms^2 = mean^2 + std^2
	      }
	    }
	  }
	}
      }
    }
    // cout << "counters = " << counters[0][1][14][32][0] << ", pedestal = " << pedestal[0][1][14][32][0] << ", totStdDev = " << totStdDev[0][1][14][32][0] << endl;
  }

  { // 3rd loop for pedestal & standard deviation with CMN subtraction without hits (adc < pedestal + 3*totStdDev)
    cout << "=====>Third Pedestal Pass...." << endl;
    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_arm = 0; i_arm < 3; ++i_arm)
      {
	for(int i_apv = 0; i_apv < 24; ++i_apv)
	{
	  for(int i_tb = 0; i_tb < 9; ++i_tb)
	  {
	    for(int i_ch = 0; i_ch < 128; ++i_ch)
	    {
	      counters[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0;
	      sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	      sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb] = 0.0;
	    }
	    for(int i_group = 0; i_group < 4; ++i_group)
	    {
	      counters_CMN[i_rdo][i_arm][i_apv][i_group][i_tb] = 0;
	      sumValues_CMN[i_rdo][i_arm][i_apv][i_group][i_tb] = 0.0;
	      sumValuesSquared_CMN[i_rdo][i_arm][i_apv][i_group][i_tb] = 0.0;
	    }
	  }
	}
      }
    }

    // event loop
    for(int i_event = 0; i_event < numEvent; ++i_event)
    {
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

		if( (adcTemp < pedTemp+nPedsCut*devTemp) && ( adcTemp >= 0 && adcTemp < 4096) )
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
		if(groupIdx < 0) continue;
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

      // calculate differential noise and cmn
      for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
      {
	for(int i_arm = 0; i_arm < 3; ++i_arm)
	{
	  for(int i_apv = 0; i_apv < 24; ++i_apv)
	  {
	    for(int i_tb = 0; i_tb < 9; ++i_tb)
	    {
	      for(int i_ch = 0; i_ch < 128; ++i_ch)
	      {
		int adcTemp   = adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb];
		float pedTemp = pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float devTemp = totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float cmnTemp = cmnEvt[i_rdo][i_arm][i_apv][i_ch][i_tb];

		// differential noise
		if( (adcTemp < pedTemp+nPedsCut*devTemp) && ( adcTemp >= 0 && adcTemp < 4096) )
		{ // exclude hits (adc < pedestal + 3*totStdDev)
		  sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb] += adcTemp-pedTemp-cmnTemp;
		  sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb] += (adcTemp-pedTemp-cmnTemp)*(adcTemp-pedTemp-cmnTemp);
		  counters[i_rdo][i_arm][i_apv][i_ch][i_tb]++;
		}
	      }

	      for(int i_group = 0; i_group < 4; ++i_group)
	      {
		// common mode noise
		if(couEvt[i_rdo][i_arm][i_apv][i_group][i_tb] > 0) // eject bad channels
		{
		  float cmnTemp = sumEvt[i_rdo][i_arm][i_apv][i_group][i_tb]/couEvt[i_rdo][i_arm][i_apv][i_group][i_tb];
		  sumValues_CMN[i_rdo][i_arm][i_apv][i_group][i_tb] += cmnTemp;
		  sumValuesSquared_CMN[i_rdo][i_arm][i_apv][i_group][i_tb] += cmnTemp*cmnTemp;
		  counters_CMN[i_rdo][i_arm][i_apv][i_group][i_tb]++;
		}
	      }
	    }
	  }
	}
      }
    } // end of event loop

    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_arm = 0; i_arm < 3; ++i_arm)
      {
	for(int i_apv = 0; i_apv < 24; ++i_apv)
	{
	  for(int i_tb = 0; i_tb < 9; ++i_tb)
	  {
	    // random noise
	    for(int i_ch = 0; i_ch < 128; ++i_ch)
	    {
	      if(counters[i_rdo][i_arm][i_apv][i_ch][i_tb] > 0) // eject bad channels
	      {
		float meanAdc = sumValues[i_rdo][i_arm][i_apv][i_ch][i_tb]/counters[i_rdo][i_arm][i_apv][i_ch][i_tb];
		float sumSAdc = sumValuesSquared[i_rdo][i_arm][i_apv][i_ch][i_tb];
		int coutAdc   = counters[i_rdo][i_arm][i_apv][i_ch][i_tb];
		ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = sqrt((sumSAdc-(float)coutAdc*meanAdc*meanAdc)/(float)(coutAdc-1)); // sample standard deviation
	      }
	      for(int i_group = 0; i_group < 4; ++i_group)
	      {
		if(counters_CMN[i_rdo][i_arm][i_apv][i_group][i_tb] > 0) // eject bad channels
		{
		  float meanCMN = sumValues_CMN[i_rdo][i_arm][i_apv][i_group][i_tb]/counters_CMN[i_rdo][i_arm][i_apv][i_group][i_tb];
		  float sumSCMN = sumValuesSquared_CMN[i_rdo][i_arm][i_apv][i_group][i_tb];
		  int coutCMN = counters_CMN[i_rdo][i_arm][i_apv][i_group][i_tb];
		  cmnStdDev[i_rdo][i_arm][i_apv][i_group][i_tb] = sqrt((sumSCMN-(float)coutCMN*meanCMN*meanCMN)/(float)(coutCMN-1));
		}
	      }
	    }
	  }
	}
      }
    }
    // cout << "counters = " << counters[0][1][14][32][0] << ", pedestal = " << pedestal[0][1][14][32][0] << ", totStdDev = " << totStdDev[0][1][14][32][0] << ", ranStdDev = " << ranStdDev[0][1][14][32][0] << endl;
  }

  string outputfile = "./pedestal.txt";
  ofstream file_pedestal;
  file_pedestal.open(outputfile.c_str());

  file_pedestal << "rdo    " << "arm    " << "apv    " << "channel    " << "tb    " << "pedestal    " << "totRMS   " << "ranRMS    " << "cmnRMS" << endl;
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
	    int groupIdx = cmnGroup[i_apv][i_ch];
	    if(groupIdx < 0) continue;
	    file_pedestal << i_rdo+1 << "    " << i_arm << "    "  << i_apv << "    "  << i_ch << "    "  << i_tb << "    "  << pedestal[i_rdo][i_arm][i_apv][i_ch][i_tb] << "    "  << totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] << "    "  << ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] << "    " << cmnStdDev[i_rdo][i_arm][i_apv][groupIdx][i_tb] << endl;
	  }
	}
      }
    }
  }
  file_pedestal.close();

  return 1;
}
