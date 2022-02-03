void write_fst_control()
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

  // Initialize db manager
  StDbManager* mgr = StDbManager::Instance();
  StDbConfigNode* node = mgr->initConfig("Calibrations_fst");
  StDbTable* dbtable = node->addDbTable("fstControl");
  TString storeTime = "2021-10-26 00:00:00"; // beginTime timestamp in MySQL format: "YYYY-MM-DD HH:mm:ss"
  mgr->setStoreTime(storeTime.Data());

  // Create your c-struct
  fstControl_st table;

  // Fill structure with data 
  table.kFstChanMaxRmsNoiseLevel    = 50.0;
  table.kFstChanMinRmsNoiseLevel    = 5.0;
  table.kFstApvMaxCmNoiseLevel      = 25.0;
  table.kFstPedCutDefault           = 3.0;
  table.kFstMinHitCutDefault        = 2.5;
  table.kFstMedHitCutDefault        = 3.5;
  table.kFstMaxHitCutDefault        = 4.0;
  table.kFstCMNCutDefault           = 3.0;
  table.kFstMinNumOfRawHits         = 1;
  table.kFstMaxNumOfRawHits         = 460;
  table.kFstAlldata                 = 0;
  table.kFstADCdata                 = 1;
  table.kFstZSdata                  = 2;
  table.kFstDefaultTimeBin          = 1;
  table.kFstCurrentTimeBinNum       = 3;

  // Store data to the StDbTable
  dbtable->SetTable((char*)&table, 1);

  // uncomment next line to set "sim" flavor. "ofl" flavor is set by default, no need to set it.
  // dbtable->setFlavor("sim");

  // Store table to database
  mgr->storeDbTable(dbtable);
}
