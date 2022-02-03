void write_fst_pednoise(long inputRunId = 22361003)
{

  // if you want to use root.exe instead of root4star, uncomment block below:

  //    gSystem->AddDynamicPath("/usr/lib/mysql");
  //    gSystem->AddDynamicPath("/usr/lib64/mysql");
  //    gSystem->AddDynamicPath("$OPTSTAR/lib/mysql/");
  //    gSystem->Load("libmysqlclient");

  // Load all required libraries
  gROOT->Macro("LoadLogger.C");
  gSystem->Load("St_base.so");
  gSystem->Load("libStDb_Tables.so");
  gSystem->Load("StDbLib.so");

  gSystem->Setenv("DB_ACCESS_MODE","write");

  // read in run info
  char paraDir[256];
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/runInfo_%d.txt", inputRunId);
  cout << paraDir << endl;

  FILE *file_runInfo;
  file_runInfo = fopen(paraDir, "r");
  char runNumber[256], runDate[256], runTime[256];
  int numTb = 0;
  if (file_runInfo==0) {
    cout << "Cannot find file " << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_runInfo)) {
      char buff[256];

      if(fgets(buff,sizeof(buff),file_runInfo) == 0) continue ;
      int ret = sscanf(buff,"%s %s %s %d",&runNumber,&runDate,&runTime,&numTb);
      if(ret!=4) continue;
      cout << "runNumber = " << runNumber << ", runDate = " << runDate << ", runTime = " << runTime << ", numTb = " << numTb << endl;
    }
    fclose(file_runInfo);
  }
  // string runNumber_str(runNumber);
  // string runDate_str(runDate);
  // string runTime_str(runTime);
  // TString storeTime = Form("%s %s",runDate_str.c_str(),runTime_str.c_str());

  // Initialize db manager
  StDbManager* mgr = StDbManager::Instance();
  StDbConfigNode* node = mgr->initConfig("Calibrations_fst");
  StDbTable* dbtable = node->addDbTable("fstPedNoise");
  // TString storeTime = "2021-10-25 00:00:00"; // beginTime timestamp in MySQL format: "YYYY-MM-DD HH:mm:ss"
  TString storeTime = Form("%s %s",runDate,runTime);
  mgr->setStoreTime(storeTime.Data());

  // Create your c-struct
  fstPedNoise_st table;

  // Fill structure with data 
  // read in CMN
  FILE *file_cmn;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/cmn_%d_unsorted.txt", inputRunId);
  std::cout << "Opening CMN File " << paraDir << std::endl;

  file_cmn = fopen(paraDir, "r");
  if (file_cmn==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_cmn)) {
      int glbApvIdxTemp=0,rdoIdxTemp=0, armIdxTemp=0, apvIdxTemp=0, grpIdxTemp=0, tbIdxTemp=0;
      float cmnTemp=0.;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_cmn) == 0) continue ;
      int ret = sscanf(buff,"%d %d %d %d %d %d %f",&glbApvIdxTemp,&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&grpIdxTemp,&tbIdxTemp,&cmnTemp);
      if(ret!=7) continue;

      int index = (glbApvIdxTemp*4+grpIdxTemp)*numTb + tbIdxTemp;
      int cmn = (int)100*cmnTemp;
      // cout << "glbApvIdxTemp = " << glbApvIdxTemp << ", grpIdxTemp = " << grpIdxTemp << ", tbIdxTemp = " << tbIdxTemp << ", index = " << index << ", cmnTemp = " << cmnTemp << ", cmn = " << cmn << endl;
      table.cmNoise[index] = cmn;

    }
    fclose(file_cmn);
  }

  // read in pedestal 
  FILE *file_ped;
  sprintf(paraDir, "/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/pedestals_%d_unsorted.txt", inputRunId);
  std::cout << "Opening Pedestal File " << paraDir << std::endl;

  file_ped = fopen(paraDir, "r");
  if (file_ped==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_ped)) {
      int glbElecChanIdxTemp=0, rdoIdxTemp=0, armIdxTemp=0, apvIdxTemp=0, chanIdxTemp=0, tbIdxTemp=0;
      float pedestalTemp=0., totrmsTemp=0., ranrmsTemp=0.;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_ped) == 0) continue ;
      int ret = sscanf(buff,"%d %d %d %d %d %d %f %f %f",&glbElecChanIdxTemp,&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&pedestalTemp,&totrmsTemp,&ranrmsTemp);
      if(ret!=9) continue;

      int index    = glbElecChanIdxTemp*numTb + tbIdxTemp;
      int pedestal = (int)pedestalTemp;
      int totrms   = (int)100*totrmsTemp;
      int ranrms   = (int)100*ranrmsTemp;
      // cout << "glbElecChanIdxTemp = " << glbElecChanIdxTemp << ", tbIdxTemp = " << tbIdxTemp << ", index = " << index << ", pedestalTemp = " << pedestalTemp << ", pedestal = " << pedestal << ", totrmsTemp = " << totrmsTemp << ", totrms = " << totrms << ", ranrmsTemp = " << ranrmsTemp << ", ranrms = " << ranrms << endl;
      table.pedestal[index] = pedestal;
      table.totNoise[index] = totrms;
      table.ranNoise[index] = ranrms;
    }
    fclose(file_ped);
  }

  // Store data to the StDbTable
  dbtable->SetTable((char*)&table, 1);

  // uncomment next line to set "sim" flavor. "ofl" flavor is set by default, no need to set it.
  // dbtable->setFlavor("sim");

  // Store table to database
  mgr->storeDbTable(dbtable);
}
