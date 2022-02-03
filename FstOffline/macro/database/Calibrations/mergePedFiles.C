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

void mergePedFiles(long inputRunId = 22361003, bool calcCMN = true)
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

  // dump run Info to a file
  FILE *file_runInfo;
  sprintf(paraDir, "/star/data01/pwg/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/runInfo_%d.txt", runNumber);
  file_runInfo = fopen(paraDir,"w");
  fprintf(file_runInfo, "%d %s %d \n", runNumber, ts.AsString("s"), numTb);
  fclose(file_runInfo);
  //-----------------------------------------------------------------------

  //-----------------------------------------------------------------------
  // initialize the group used for CMN calculation
  int cmnGroup[24][128]; // group used for CMN calculation
  string inputCMN = "./cmnGroup";
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
  //-----------------------------------------------------------------------

  //-----------------------------------------------------------------------
  const int totCh  = 36864;
  const int totTb  = numTb;
  const int totApv = 288;
  const int numGrp = 4;
  float cmn[totApv][numGrp][totTb];
  int cmnCou[totApv][numGrp][totTb];
  for(int i_apv = 0; i_apv < 288; ++i_apv)
  {
    for(int i_grp = 0; i_grp < 4; ++i_grp)
    {
      for(int i_tb = 0; i_tb < numTb; ++i_tb)
      {
	cmn[i_apv][i_grp][i_tb] = 0.0;
	cmnCou[i_apv][i_grp][i_tb] = 0;
      }
    }
  }

  FILE *file_pedestal;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/pedestals_%d_unsorted.txt", inputRunId);
  file_pedestal = fopen(paraDir, "w");

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
      int glbElecChanIdxTemp = (rdoIdxTemp-1)*3*16*128 + armIdxTemp*16*128 + refApvIdxTemp*128 + chanIdxTemp; // 0-36863
      fprintf(file_pedestal,"%d %d %d %2d %3d %2d %7.3f %.3f %.3f\n",glbElecChanIdxTemp,rdoIdxTemp,armIdxTemp,refApvIdxTemp,chanIdxTemp,tbIdxTemp,pedestalTemp,totrmsTemp,ranrmsTemp);

      if(calcCMN)
      {
	int glbApvIdxTemp = (rdoIdxTemp-1)*3*16 + armIdxTemp*16 + refApvIdxTemp; // 0-287
	int grpTemp       = cmnGroup[apvIdxTemp][chanIdxTemp]; // 0-3
	if(totrmsTemp > ranrmsTemp)
	{
	  cmn[glbApvIdxTemp][grpTemp][tbIdxTemp] += sqrt(totrmsTemp*totrmsTemp-ranrmsTemp*ranrmsTemp);
	  cmnCou[glbApvIdxTemp][grpTemp][tbIdxTemp]++;
	}
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
      int glbElecChanIdxTemp = (rdoIdxTemp-1)*3*16*128 + armIdxTemp*16*128 + refApvIdxTemp*128 + chanIdxTemp; // 0-36863
      fprintf(file_pedestal,"%d %d %d %2d %3d %2d %7.3f %.3f %.3f\n",glbElecChanIdxTemp,rdoIdxTemp,armIdxTemp,refApvIdxTemp,chanIdxTemp,tbIdxTemp,pedestalTemp,totrmsTemp,ranrmsTemp);

      if(calcCMN)
      {
	int glbApvIdxTemp = (rdoIdxTemp-1)*3*16 + armIdxTemp*16 + refApvIdxTemp; // 0-287
	int grpTemp       = cmnGroup[apvIdxTemp][chanIdxTemp]; // 0-3
	if(totrmsTemp > ranrmsTemp)
	{
	  cmn[glbApvIdxTemp][grpTemp][tbIdxTemp] += sqrt(totrmsTemp*totrmsTemp-ranrmsTemp*ranrmsTemp);
	  cmnCou[glbApvIdxTemp][grpTemp][tbIdxTemp]++;
	}
      }
    }
    fclose(file_inPed2);
  }
  fclose(file_pedestal);
  //-----------------------------------------------------------------------

  //-----------------------------------------------------------------------
  if(calcCMN)
  {
    FILE *file_cmn;
    sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/cmn_%d_unsorted.txt", inputRunId);
    file_cmn = fopen(paraDir, "w");

    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_arm = 0; i_arm < 3; ++i_arm)
      {
	for(int i_apv = 0; i_apv < 16; ++i_apv)
	{
	  int calcGlbApvIdxTemp = i_rdo*3*16 + i_arm*16 + i_apv; // 0-287
	  for(int i_grp = 0; i_grp < 4; ++i_grp)
	  {
	    for(int i_tb = 0; i_tb < numTb; ++i_tb)
	    {
	      float cmnTemp = 100.0;
	      if(cmnCou[calcGlbApvIdxTemp][i_grp][i_tb] > 0) 
		cmnTemp = cmn[calcGlbApvIdxTemp][i_grp][i_tb]/cmnCou[calcGlbApvIdxTemp][i_grp][i_tb];

	      fprintf(file_cmn,"%d %d %d %2d %3d %2d %.3f\n",calcGlbApvIdxTemp,i_rdo+1,i_arm,i_apv,i_grp,i_tb,cmnTemp);
	    }
	  }
	}
      }
    }

    fclose(file_cmn);
  }
  //-----------------------------------------------------------------------

}
