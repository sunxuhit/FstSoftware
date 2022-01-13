void write_fst_simPar()
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
  StDbTable* dbtable = node->addDbTable("fstSimPar");
  TString storeTime = "2021-10-25 00:00:00"; // beginTime timestamp in MySQL format: "YYYY-MM-DD HH:mm:ss"
  mgr->setStoreTime(storeTime.Data());

  // Create your c-struct
  fstSimPar_st table;

  // Fill structure with data 
  table.mode   = 1;   // effMode: 0-ideal; 1 (default) -momentum depenent; 2 nomimal
  table.pCut   = 0.1; // momentum cut for momentum depenent efficiency
  table.effFst = 1.0; // FST efficiency

  // Store data to the StDbTable
  dbtable->SetTable((char*)&table, 1);

  // uncomment next line to set "sim" flavor. "ofl" flavor is set by default, no need to set it.
  // dbtable->setFlavor("sim");

  // Store table to database
  mgr->storeDbTable(dbtable);
}
