{
  gSystem->Setenv("DB_ACCESS_MODE", "write");
  gROOT->Macro("LoadLogger.C");
  gSystem->Load("St_base.so");
  gSystem->Load("libStDb_Tables.so");
  gSystem->Load("StDbLib.so");

  TString storeTime = "2014-04-08 14:44:00";
  cout <<"The RTS Start date and time is " << storeTime<<endl;  

  StDbManager* mgr = StDbManager::Instance();
  StDbConfigNode* node = mgr->initConfig("Calibrations_fst");
  StDbTable* dbtable = node->addDbTable("fstControl");
  mgr->setStoreTime(storeTime.Data());
 
  fstControl_st table;

  table.kFstChanMaxRmsNoiseLevel    = 35.0;
  table.kFstChanMinRmsNoiseLevel    = 12.0;
  table.kFstApvMaxCmNoiseLevel      = 15.0;
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
  table.kFstDefaultTimeBin          = 2;
  table.kFstCurrentTimeBinNum	    = 4;

  // Store data to the StDbTable
  dbtable->SetTable((char*)&table, 1);
  
  // Store table to database
  mgr->storeDbTable(dbtable);
};

