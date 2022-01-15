void write_fstSensorOnWedge()
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
  StDbTable* dbtable = node->addDbTable("fstSensorOnWedge");
  TString storeTime = "2021-10-25 00:00:00"; // beginTime timestamp in MySQL format: "YYYY-MM-DD HH:mm:ss"
  mgr->setStoreTime(storeTime.Data());

  // Create your c-struct
  const Int_t MAX_DB_INDEX = 108;
  Survey_st table[MAX_DB_INDEX];

  // Fill structure with data 
  for (Int_t i = 0; i < MAX_DB_INDEX; i++) {
    table[i].Id        = i+1000;
    table[i].r00       = 1.0;
    table[i].r01       = 0.0;
    table[i].r02       = 0.0;
    table[i].r10       = 0.0;
    table[i].r11       = 1.0;
    table[i].r12       = 0.0;
    table[i].r20       = 0.0;
    table[i].r21       = 0.0;
    table[i].r22       = 1.0;
    table[i].t0        = 0.0;
    table[i].t1        = 0.0;
    table[i].t2        = 0.0;
    table[i].sigmaRotX = 0.0;
    table[i].sigmaRotY = 0.0;
    table[i].sigmaRotZ = 0.0;
    table[i].sigmaTrX  = 0.0;
    table[i].sigmaTrY  = 0.0;
    table[i].sigmaTrZ  = 0.0;
    strcpy(table[i].comment,"initial commit");
  }

  // Store data to the StDbTable
  dbtable->SetTable((char*)&table, MAX_DB_INDEX);

  // uncomment next line to set "sim" flavor. "ofl" flavor is set by default, no need to set it.
  // dbtable->setFlavor("sim");

  // Store table to database
  mgr->storeDbTable(dbtable);

  std::cout << "INFO: table saved to database" << std::endl;
}
