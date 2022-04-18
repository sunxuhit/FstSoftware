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
#include <TH2F.h>

using namespace std;

void plotChannelAdc(long inputRunId = 23030027, int rdoIdx = 4, int armIdx = 1, int apvIdx = 13, int tbIdx = 1)
{
  TH2F *h_adc = new TH2F("h_adc","h_adc",32,-0.5,127.5,90,700,1600);
  char paraDir[256];

  //load external pedstal/RMS value for all channels
  sprintf(paraDir, "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstInstallation/pedestal/fst_s1_pedestals_%d_GOOD.txt", inputRunId);
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
	h_adc->Fill(chanIdxTemp,pedestalTemp);
      }
    }
    fclose(file_inPed1);
  }

  sprintf(paraDir, "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstInstallation/pedestal/fst_s2_pedestals_%d_GOOD.txt", inputRunId);
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
	h_adc->Fill(chanIdxTemp,pedestalTemp);
      }
    }
    fclose(file_inPed2);
  }
  //-----------------------------------------------------------------------

  h_adc->Draw("col");
  h_adc->Draw("col");
}
