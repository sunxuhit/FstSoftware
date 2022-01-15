void write_fstOnTpc()
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
  StDbConfigNode* node = mgr->initConfig("Geometry_fst");
  StDbTable* dbtable = node->addDbTable("fstOnTpc");
  TString storeTime = "2021-10-25 00:00:14"; // beginTime timestamp in MySQL format: "YYYY-MM-DD HH:mm:ss"
  mgr->setStoreTime(storeTime.Data());

  // Create your c-struct
  Survey_st table;

  // Fill structure with data 
  table.Id        = 1;
  table.r00       = 1.0;
  table.r01       = 0.0;
  table.r02       = 0.0;
  table.r10       = 0.0;
  table.r11       = 1.0;
  table.r12       = 0.0;
  table.r20       = 0.0;
  table.r21       = 0.0;
  table.r22       = 1.0;
  table.t0        = 0.0;
  table.t1        = 0.0;
  table.t2        = 0.0;
  table.sigmaRotX = 0.0;
  table.sigmaRotY = 0.0;
  table.sigmaRotZ = 0.0;
  table.sigmaTrX  = 0.0;
  table.sigmaTrY  = 0.0;
  table.sigmaTrZ  = 0.0;
  strcpy(table.comment,"initial commit");

  // Store data to the StDbTable
  dbtable->SetTable((char*)&table, 1);

  // uncomment next line to set "sim" flavor. "ofl" flavor is set by default, no need to set it.
  // dbtable->setFlavor("sim");

  // Store table to database
  mgr->storeDbTable(dbtable);
}
