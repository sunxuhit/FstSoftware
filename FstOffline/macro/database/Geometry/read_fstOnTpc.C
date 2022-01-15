void read_fstOnTpc()
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
  dbMk->SetDateTime(20211026,7); // event or run start time, set to your liking
  dbMk->SetFlavor("ofl");

  dbMk->Init();
  dbMk->Make();

  TDataSet *DB = 0;
  DB = dbMk->GetDataBase("Geometry/fst/fstOnTpc");
  if (!DB) {
    std::cout << "ERROR: no table found in db, or malformed local db config" << std::endl;
    return;
  }

  St_Survey *dataset = 0;
  dataset = (St_Survey*) DB->Find("fstOnTpc");

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

    Survey_st *table = dataset->GetTable();
    std::cout << "Id:        " << table[0].Id        << std::endl;
    std::cout << "r00:       " << table[0].r00       << std::endl;
    std::cout << "r01:       " << table[0].r01       << std::endl;
    std::cout << "r02:       " << table[0].r02       << std::endl;
    std::cout << "r10:       " << table[0].r10       << std::endl;
    std::cout << "r11:       " << table[0].r11       << std::endl;
    std::cout << "r12:       " << table[0].r12       << std::endl;
    std::cout << "r20:       " << table[0].r20       << std::endl;
    std::cout << "r21:       " << table[0].r21       << std::endl;
    std::cout << "r22:       " << table[0].r22       << std::endl;
    std::cout << "t0:        " << table[0].t0        << std::endl;
    std::cout << "t1:        " << table[0].t1        << std::endl;
    std::cout << "t2:        " << table[0].t2        << std::endl;
    std::cout << "sigmaRotX: " << table[0].sigmaRotX << std::endl;
    std::cout << "sigmaRotY: " << table[0].sigmaRotY << std::endl;
    std::cout << "sigmaRotZ: " << table[0].sigmaRotZ << std::endl;
    std::cout << "sigmaTrX:  " << table[0].sigmaTrX  << std::endl;
    std::cout << "sigmaTrY:  " << table[0].sigmaTrY  << std::endl;
    std::cout << "sigmaTrZ:  " << table[0].sigmaTrZ  << std::endl;
    std::cout << "comment:   " << table[0].comment   << std::endl;
  } else {
    std::cout << "ERROR: dataset does not contain requested table" << std::endl;
  }

}
