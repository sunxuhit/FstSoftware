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

using namespace std;

void mergePedFiles(string runId = "22340056")
{
  char paraDir[256];
  // initialize map for month
  std::map<string,int> map_month;
  string month[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  for(int iMon = 0; iMon < 12; ++iMon)
  {
    map_month[month[iMon]] = iMon+1;
  }

  // get run info
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_s1_pedestals_%s_GOOD.txt", runId.c_str());

  std::ifstream file_Input(paraDir);
  if (!file_Input.is_open()) 
  {
    std::cout << "Can't find file!\n";
    exit(0); 
  }

  char runNumber[256], runDate[256], runTime[256], numTb[256];
  string header;
  for(int iHeader = 0; iHeader < 4; ++iHeader)
  {
    getline(file_Input,header);
    char input[256];
    strcpy(input,header.c_str());

    if(iHeader < 1) continue;
    // cout << input << endl;
    if(iHeader == 1) // get run number
    {
      char *token = std::strtok(input, " ");
      int count = 0;
      while (token) {
	// output[count] = malloc((strlen(token) + 1));
	// std::cout << "input " << token << ", output " << output[count] << std::endl;
	if(count == 2) strcpy(runNumber,token);
	token = std::strtok(NULL, " ");
	count++;
      }
      cout << "runNumber: " << runNumber << endl;
    }
    if(iHeader == 2) // get run time
    {
      char *token = std::strtok(input, " ");
      char monTemp[4];
      char dateTemp[3];
      char yearTemp[5];
      int count = 0;
      while (token) {
	// std::cout << "count: " << count << ", input: " << token << std::endl;
	if(count == 3) 
	{
	  strcpy(monTemp,token);
	}
	if(count == 4) 
	{
	  strcpy(dateTemp,token);
	  if(dateTemp[1] == NULL) 
	  {
	    dateTemp[1] = dateTemp[0];
	    dateTemp[0] = '0';
	    dateTemp[2] = '\0';
	  }
	  // cout << dateTemp[0] << ", " << dateTemp[1] << endl;
	}
	if(count == 5) strcpy(runTime,token);
	if(count == 6) 
	{
	  strcpy(yearTemp,token);
	}
	token = std::strtok(NULL, " ");
	count++;
      }
      string monTemp_str(monTemp);
      std::map<string,int>::iterator it = map_month.find(monTemp_str);
      std::ostringstream ssMonTemp;
      ssMonTemp << it->second;
      // cout << "monTemp: " << ssMonTemp.str().c_str() << endl;
      // cout << "dateTemp: " << dateTemp << endl;
      // cout << "yearTemp: " << yearTemp << endl;
      strcpy(runDate,yearTemp);
      strcat(runDate,"-");
      strcat(runDate,ssMonTemp.str().c_str());
      strcat(runDate,"-");
      strcat(runDate,dateTemp);
      cout << "runDate: " << runDate << endl;
      cout << "runTime: " << runTime << endl;
    }
    if(iHeader == 3) // get number of Time Bin
    {
      char *token = std::strtok(input, " ");
      int count = 0;
      while (token) {
	// output[count] = malloc((strlen(token) + 1));
	// std::cout << "input " << token << ", output " << output[count] << std::endl;
	if(count == 2) strcpy(numTb,token);
	token = std::strtok(NULL, " ");
	count++;
      }
      cout << "numTb: " << numTb << endl;
    }
  }
  file_Input.close();

  FILE *file_runInfo;
  sprintf(paraDir, "/star/data01/pwg/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/runInfo_%s.txt", runNumber);
  file_runInfo = fopen(paraDir,"w");
  fprintf(file_runInfo, "%s %s %s %s \n", runNumber, runDate, runTime, numTb);
  fclose(file_runInfo);

  int usedTb = (int)numTb[0] - 48; // convert char to int
  // cout << "numTb = " << numTb[0] << ", usedTb = " << usedTb << endl;

  //
  FILE *file_pedestal;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/pedestals_%s_unsorted.txt", runId.c_str());
  file_pedestal = fopen(paraDir, "w");

  //load external pedstal/RMS value for all channels
  FILE *file_inPed1;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_s1_pedestals_%s_GOOD.txt", runId.c_str());
  std::cout << "Opening Pedestal File 1 " << paraDir << std::endl;

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

      // int portIdxTemp        = apvIdxTemp/12; // 0: 0-7 | 1: 12-19
      // int refApvIdxTemp      = apvIdxTemp - portIdxTemp*ApvNumOffset + portIdxTemp*ApvPerPort; // 0-15
      int refApvIdxTemp = apvIdxTemp;
      if(apvIdxTemp > 7) refApvIdxTemp = apvIdxTemp - 4; // 0-15
      int glbElecChanIdxTemp = (rdoIdxTemp-1)*3*16*128 + armIdxTemp*16*128 + refApvIdxTemp*128 + chanIdxTemp; // 0-36863
      fprintf(file_pedestal,"%d %d %d %2d %3d %2d %7.3f %.3f %.3f\n",glbElecChanIdxTemp,rdoIdxTemp,armIdxTemp,refApvIdxTemp,chanIdxTemp,tbIdxTemp,pedestalTemp,totrmsTemp,ranrmsTemp);

    }
    fclose(file_inPed1);
  }

  FILE *file_inPed2;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_s2_pedestals_%s_GOOD.txt", runId.c_str());
  std::cout << "Opening Pedestal File 2 " << paraDir << std::endl;

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

    }
    fclose(file_inPed2);
  }

  fclose(file_pedestal);

  FILE *file_cmn;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/cmn_%s_unsorted.txt", runId.c_str());
  file_cmn = fopen(paraDir, "w");

  // fake CMN for now
  FILE *file_inCmn;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal/fst_cmn_22340057_GOOD.txt");

  std::cout << "Opening CMN File " << paraDir << std::endl;

  file_inCmn = fopen(paraDir, "r");
  if (file_inCmn==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_inCmn)) {
      int rdoIdxTemp=0, armIdxTemp=0, apvIdxTemp=0, grpIdxTemp=0, tbIdxTemp=0;
      float cmnTemp=0.;
      int glbApvIdxTemp=0;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_inCmn) == 0) continue ;
      switch(buff[0]) {
	case '#' :
	case '!' :
	case '*' :
	case '/' :
	case '.' :
	  continue ;
      }
      int ret = sscanf(buff,"%d %d %d %d %d %d %f",&glbApvIdxTemp,&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&grpIdxTemp,&tbIdxTemp,&cmnTemp);
      if(ret!=7) continue;

      if(tbIdxTemp >= usedTb) continue;
      int calcGlbApvIdxTemp = (rdoIdxTemp-1)*3*16 + armIdxTemp*16 + apvIdxTemp; // 0-287
      // fprintf(file_cmn,"%d %d %d %2d %3d %2d %.3f\n",calcGlbApvIdxTemp,rdoIdxTemp,armIdxTemp,apvIdxTemp,grpIdxTemp,tbIdxTemp,cmnTemp);
      fprintf(file_cmn,"%d %d %d %2d %3d %2d %.3f\n",calcGlbApvIdxTemp,rdoIdxTemp,armIdxTemp,apvIdxTemp,grpIdxTemp,tbIdxTemp,10.0);

    }
    fclose(file_inCmn);
  }

  fclose(file_cmn);
}
