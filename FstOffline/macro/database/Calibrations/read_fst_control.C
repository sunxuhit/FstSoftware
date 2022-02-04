void read_fst_control()
{

  // if you want to use root.exe instead of root4star, uncomment block below:

  //    gSystem->AddDynamicPath("/usr/lib/mysql");
  //    gSystem->AddDynamicPath("/usr/lib64/mysql");
  //    gSystem->AddDynamicPath("$OPTSTAR/lib/mysql/");
  //    gSystem->Load("libmysqlclient");

  // base libraries
  gSystem->Load("St_base");
  gSystem->Load("StChain");
  gSystem->Load("StUtilities");
  gSystem->Load("StIOMaker");
  gSystem->Load("StarClassLibrary");

  // db-related libraries
  gSystem->Load("St_Tables");
  gSystem->Load("StDbLib");
  gSystem->Load("StDbBroker");
  gSystem->Load("St_db_Maker");

  St_db_Maker *dbMk=new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
  dbMk->SetDebug();
  dbMk->SetDateTime(20211228,1); // event or run start time, set to your liking
  dbMk->SetFlavor("ofl");

  dbMk->Init();
  dbMk->Make();

  TDataSet *DB = 0;
  DB = dbMk->GetDataBase("Calibrations/fst/fstControl");
  if (!DB) {
    std::cout << "ERROR: no table found in db, or malformed local db config" << std::endl;
    return;
  }

  St_fstControl *dataset = 0;
  dataset = (St_fstControl*) DB->Find("fstControl");

  if (dataset) {

    Int_t rows = dataset->GetNRows();
    if (rows > 1) {
      std::cout << "INFO: found INDEXED table with " << rows << " rows" << std::endl;
    }

    TDatime val[2];
    dbMk->GetValidity((TTable*)dataset,val);
    std::cout << "Dataset validity range: [ " << val[0].GetDate() << "." << val[0].GetTime() << " - " 
      << val[1].GetDate() << "." << val[1].GetTime() << " ] "
      << std::endl;

    fstControl_st *table = dataset->GetTable();
    std::cout << "kFstChanMaxRmsNoiseLevel: " << table[0].kFstChanMaxRmsNoiseLevel << std::endl;
    std::cout << "kFstChanMinRmsNoiseLevel: " << table[0].kFstChanMinRmsNoiseLevel << std::endl;
    std::cout << "kFstApvMaxCmNoiseLevel  : " << table[0].kFstApvMaxCmNoiseLevel   << std::endl;
    std::cout << "kFstPedCutDefault       : " << table[0].kFstPedCutDefault        << std::endl;
    std::cout << "kFstMinHitCutDefault    : " << table[0].kFstMinHitCutDefault     << std::endl;
    std::cout << "kFstMedHitCutDefault    : " << table[0].kFstMedHitCutDefault     << std::endl;
    std::cout << "kFstMaxHitCutDefault    : " << table[0].kFstMaxHitCutDefault     << std::endl;
    std::cout << "kFstCMNCutDefault       : " << table[0].kFstCMNCutDefault        << std::endl;
    std::cout << "kFstMinNumOfRawHits     : " << table[0].kFstMinNumOfRawHits      << std::endl;
    std::cout << "kFstMaxNumOfRawHits     : " << table[0].kFstMaxNumOfRawHits      << std::endl;
    std::cout << "kFstAlldata             : " << table[0].kFstAlldata              << std::endl;
    std::cout << "kFstADCdata             : " << table[0].kFstADCdata              << std::endl;
    std::cout << "kFstZSdata              : " << table[0].kFstZSdata               << std::endl;
    std::cout << "kFstDefaultTimeBin      : " << table[0].kFstDefaultTimeBin       << std::endl;
    std::cout << "kFstCurrentTimeBinNum   : " << table[0].kFstCurrentTimeBinNum    << std::endl;
  } else {
    std::cout << "ERROR: dataset does not contain requested table" << std::endl;
  }

}
