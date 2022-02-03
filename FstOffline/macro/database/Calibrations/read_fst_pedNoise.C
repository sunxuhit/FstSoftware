void read_fst_pedNoise()
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
  DB = dbMk->GetDataBase("Calibrations/fst/fstPedNoise");
  if (!DB) {
    std::cout << "ERROR: no table found in db, or malformed local db config" << std::endl;
    return;
  }

  St_fstPedNoise *dataset = 0;
  dataset = (St_fstPedNoise*) DB->Find("fstPedNoise");

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

    fstPedNoise_st *table = dataset->GetTable();
    for(int index = 0; index < 10368; ++index)
    {
      std::cout << "index: " << index << " cmn " << table[0].cmNoise[index] << std::endl;
    }
    for(int index = 0; index < 331776; ++index)
    {
      std::cout << "index: " << index << " pedestal " << table[0].pedestal[index] << std::endl;
      std::cout << "index: " << index << " totNoise " << table[0].totNoise[index] << std::endl;
      std::cout << "index: " << index << " ranNoise " << table[0].ranNoise[index] << std::endl;
    }
  } else {
    std::cout << "ERROR: dataset does not contain requested table" << std::endl;
  }

}
