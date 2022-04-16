#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <TString.h>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <map>
#include <ctime>

using namespace std;

void checkRms(long inputRunId = 23047025, int rdoIdx = 5, int armIdx = 0, int apvIdx = 0, int tbIdx = 1)
{
  char paraDir[256];

  //-----------------------------------------------------------------------
  // set map for run timestamp
  std::map<long,long> map_runTs;
  sprintf(paraDir, "../TimeStamp/runTime_run22.txt");
  cout << paraDir << endl;

  FILE *file_timeInfo;
  file_timeInfo = fopen(paraDir, "r");
  if (file_timeInfo==0) {
    cout << "Cannot find file " << paraDir << endl;
    exit(0);
  }
  else {
    while(!feof(file_timeInfo)) {
      long runNumberTemp, runTimeTemp;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_timeInfo) == 0) continue ;
      int ret = sscanf(buff,"%d %d",&runNumberTemp,&runTimeTemp);
      if(ret!=2) continue;
      map_runTs[runNumberTemp] = runTimeTemp;
      // cout << "runNumber = " << runNumberTemp << ", runTime = " << map_runTs[runNumberTemp] << endl;
    }
    fclose(file_timeInfo);
  }

  // get run info
  FILE *file_inRunInfo;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_s1_pedestals_%d_GOOD.txt", inputRunId);
  std::cout << "Opening Pedestal File 1 for run Info: " << paraDir << std::endl;

  long runNumber = -1;
  int numTb = -1;
  file_inRunInfo = fopen(paraDir, "r");
  if (file_inRunInfo==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    int counter = 0;
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_inRunInfo)) {
      char charHeader[128], charName[128];
      char buff[256];

      if(fgets(buff,sizeof(buff),file_inRunInfo) == 0) continue ;
      if(buff[0] != '#') continue;

      int ret = -1;
      // cout << "cout = " << counter << buff;

      if(counter == 1) ret = sscanf(buff,"%s %s %d",&charHeader,&charName,&runNumber);
      if(counter == 3) ret = sscanf(buff,"%s %s %d",&charHeader,&charName,&numTb);

      counter++;
      if(ret!=3) continue;
    }
    fclose(file_inRunInfo);
  }

  // get UTC/GMT time
  long unixtime = map_runTs[runNumber];
  TTimeStamp ts( unixtime, true );
  cout << "inputRunId = " << inputRunId << ", runNumber from file = " << runNumber << ", date: " << ts.AsString("s") << ", numTb = " << numTb << endl;

  if(inputRunId != runNumber)
  {
    cout << "runId don't match: inputRunId: " << inputRunId << ", runNumber from file: " << runNumber << endl;
    exit(0);
  }

  //-----------------------------------------------------------------------
  float meanTotRms = 0.;
  float meanRanRms = 0.;
  int   couRms = 0;

  int glbElecApvIdx = (rdoIdx-1)*48 + armIdx*16 + apvIdx; // 0-287
  FILE *file_rmsInfo;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/qa/rmsInfo_%d_APV%d.txt", inputRunId,glbElecApvIdx);
  file_rmsInfo = fopen(paraDir, "w");

  //load external pedstal/RMS value for all channels
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_s1_pedestals_%d_GOOD.txt", inputRunId);
  std::cout << "Opening Pedestal File 1 " << paraDir << std::endl;

  FILE *file_inPed1;
  file_inPed1 = fopen(paraDir, "r");
  if (file_inPed1==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_inPed1)) {
      int rdoIdxTemp=0, armIdxTemp=0, apvIdxTemp=0, chanIdxTemp=0, tbIdxTemp=0;
      float pedestalTemp=0., totrmsTemp=0., ranrmsTemp=0.;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_inPed1) == 0) continue ;
      switch(buff[0]) {
	case '#' :
	case '!' :
	case '*' :
	case '/' :
	case '.' :
	  continue ;
      }
      int ret = sscanf(buff,"%d %d %d %d %d %f %f %f",&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&pedestalTemp,&totrmsTemp,&ranrmsTemp);
      if(ret!=8) continue;

      int refApvIdxTemp = apvIdxTemp;
      if(apvIdxTemp > 7) refApvIdxTemp = apvIdxTemp - 4; // 0-15
      if( (rdoIdxTemp == rdoIdx) && (armIdxTemp == armIdx) && (refApvIdxTemp == apvIdx) && (tbIdxTemp == tbIdx))
      {
	meanTotRms += totrmsTemp;
	meanRanRms += ranrmsTemp;
	couRms ++;
	// cout << "rdoIdxTemp = " << rdoIdxTemp << ", armIdxTemp = " << armIdxTemp << ", refApvIdxTemp = " << refApvIdxTemp << ", tbIdxTemp = " << tbIdxTemp << ", couRms = " << couRms << endl;
      }
    }
    fclose(file_inPed1);
  }

  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_s2_pedestals_%d_GOOD.txt", inputRunId);
  std::cout << "Opening Pedestal File 2 " << paraDir << std::endl;

  FILE *file_inPed2;
  file_inPed2 = fopen(paraDir, "r");
  if (file_inPed2==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_inPed2)) {
      int rdoIdxTemp=0, armIdxTemp=0, apvIdxTemp=0, chanIdxTemp=0, tbIdxTemp=0;
      float pedestalTemp=0., totrmsTemp=0., ranrmsTemp=0.;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_inPed2) == 0) continue ;
      switch(buff[0]) {
	case '#' :
	case '!' :
	case '*' :
	case '/' :
	case '.' :
	  continue ;
      }
      int ret = sscanf(buff,"%d %d %d %d %d %f %f %f",&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&pedestalTemp,&totrmsTemp,&ranrmsTemp);
      if(ret!=8) continue;

      // int portIdxTemp        = apvIdxTemp/12; // 0: 0-7 | 1: 12-19
      // int refApvIdxTemp      = apvIdxTemp - portIdxTemp*ApvNumOffset + portIdxTemp*ApvPerPort; // 0-15
      int refApvIdxTemp = apvIdxTemp;
      if(apvIdxTemp > 7) refApvIdxTemp = apvIdxTemp - 4; // 0-15
      if( (rdoIdxTemp == rdoIdx) && (armIdxTemp == armIdx) && (refApvIdxTemp == apvIdx) && (tbIdxTemp == tbIdx))
      {
	meanTotRms += totrmsTemp;
	meanRanRms += ranrmsTemp;
	couRms ++;
	// cout << "rdoIdxTemp = " << rdoIdxTemp << ", armIdxTemp = " << armIdxTemp << ", refApvIdxTemp = " << refApvIdxTemp << ", tbIdxTemp = " << tbIdxTemp << ", couRms = " << couRms << endl;
      }
    }
    fclose(file_inPed2);
  }
  //-----------------------------------------------------------------------

  meanTotRms = meanTotRms/couRms;
  meanRanRms = meanRanRms/couRms;
  string date = ts.AsString("s");
  fprintf(file_rmsInfo,"%d %s %d %d %d %d %d %2.3f %2.3f\n",runNumber,date.c_str(),glbElecApvIdx,rdoIdx,armIdx,apvIdx,tbIdx,meanTotRms,meanRanRms);
  fclose(file_rmsInfo);

}
