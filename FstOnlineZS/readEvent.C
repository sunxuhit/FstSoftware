#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

int readEvent()
{
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
	      adc[i_event][i_rdo][i_arm][i_apv][i_ch][i_tb] = 0;
	    }
	  }
	}
      }
    }
  }

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
    int numEvent = -1;
    while(!feof(file_testEvent)) 
    {
      int rdoIdxTemp=-1, armIdxTemp=-1, apvIdxTemp=-1, chanIdxTemp=-1, tbIdxTemp=-1, adcIdxTemp=-1;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_testEvent) == NULL) continue ;
      int ret = sscanf(buff,"%d %d %d %d %d %d",&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&adcIdxTemp);
      // cout << "ret = " << ret << ", chanIdxTemp = " << chanIdxTemp << ", adcIdxTemp = " << adcIdxTemp << endl;
      if(ret != 6) {numEvent++; cout << "numEvent: " << numEvent << endl; continue;}
      if(numEvent-1 > 0)
      {
	adc[numEvent-1][rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = adcIdxTemp;
	if(armIdxTemp == 1 && apvIdxTemp == 14 && chanIdxTemp == 24 && tbIdxTemp == 5) cout << "adc = " << adc[numEvent-1][rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << endl;;
      }
    }
  }
  fclose(file_testEvent);

  return 1;
}
