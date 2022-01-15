void read_fst_simPar()
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
  dbMk->SetDateTime(20211025,1); // event or run start time, set to your liking
  dbMk->SetFlavor("ofl");

  dbMk->Init();
  dbMk->Make();

  TDataSet *DB = 0;
  DB = dbMk->GetDataBase("Calibrations/fst/fstSimPar");
  if (!DB) {
    std::cout << "ERROR: no table found in db, or malformed local db config" << std::endl;
    return;
  }

  St_fstSimPar *dataset = 0;
  dataset = (St_fstSimPar*) DB->Find("fstSimPar");

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

    fstSimPar_st *table = dataset->GetTable();
    std::cout << "mode: " << table[0].mode << std::endl;
    std::cout << "pCut: " << table[0].pCut << std::endl;
    std::cout << "effFst: " << table[0].effFst << std::endl;
  } else {
    std::cout << "ERROR: dataset does not contain requested table" << std::endl;
  }

}
