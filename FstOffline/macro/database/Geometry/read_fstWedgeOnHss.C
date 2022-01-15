void read_fstWedgeOnHss()
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
  dbMk->SetDateTime(20211026,0); // event or run start time, set to your liking
  dbMk->SetFlavor("ofl");

  dbMk->Init();
  dbMk->Make();

  TDataSet *DB = 0;
  DB = dbMk->GetDataBase("Geometry/fst/fstWedgeOnHss");
  if (!DB) {
    std::cout << "ERROR: no table found in db, or malformed local db config" << std::endl;
    return;
  }

  St_Survey *dataset = 0;
  dataset = (St_Survey*) DB->Find("fstWedgeOnHss");

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
    for (Int_t i = 0; i < rows; i++) {
      std::cout << i << "th wedge Id:        " << table[i].Id        << std::endl;
      std::cout << i << "th wedge r00:       " << table[i].r00       << std::endl;
      std::cout << i << "th wedge r01:       " << table[i].r01       << std::endl;
      std::cout << i << "th wedge r02:       " << table[i].r02       << std::endl;
      std::cout << i << "th wedge r10:       " << table[i].r10       << std::endl;
      std::cout << i << "th wedge r11:       " << table[i].r11       << std::endl;
      std::cout << i << "th wedge r12:       " << table[i].r12       << std::endl;
      std::cout << i << "th wedge r20:       " << table[i].r20       << std::endl;
      std::cout << i << "th wedge r21:       " << table[i].r21       << std::endl;
      std::cout << i << "th wedge r22:       " << table[i].r22       << std::endl;
      std::cout << i << "th wedge t0:        " << table[i].t0        << std::endl;
      std::cout << i << "th wedge t1:        " << table[i].t1        << std::endl;
      std::cout << i << "th wedge t2:        " << table[i].t2        << std::endl;
      std::cout << i << "th wedge sigmaRotX: " << table[i].sigmaRotX << std::endl;
      std::cout << i << "th wedge sigmaRotY: " << table[i].sigmaRotY << std::endl;
      std::cout << i << "th wedge sigmaRotZ: " << table[i].sigmaRotZ << std::endl;
      std::cout << i << "th wedge sigmaTrX:  " << table[i].sigmaTrX  << std::endl;
      std::cout << i << "th wedge sigmaTrY:  " << table[i].sigmaTrY  << std::endl;
      std::cout << i << "th wedge sigmaTrZ:  " << table[i].sigmaTrZ  << std::endl;
      std::cout << i << "th wedge comment:   " << table[i].comment   << std::endl;
    }
  } else {
    std::cout << "ERROR: dataset does not contain requested table" << std::endl;
  }

}
