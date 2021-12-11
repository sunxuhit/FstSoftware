{
#include <iomanip>
#include "fstPedNoise_Timebin.h"
 
  //gSystem->Setenv("DB_ACCESS_MODE", "write");
  //gROOT->Macro("LoadLogger.C");
  //gSystem->Load("St_base.so");
  //gSystem->Load("libStDb_Tables.so");
  //gSystem->Load("StDbLib.so");

  TString storeTime = "2021-12-01 00:00:01";
  cout <<"The RTS Start date and time is " << storeTime<<endl;  

  //StDbManager* mgr = StDbManager::Instance();
  //StDbConfigNode* node = mgr->initConfig("Calibrations_ist");
  //StDbTable* dbtable = node->addDbTable("istPedNoise");
  //mgr->setStoreTime(storeTime.Data());
 
  //open up file to read in pedesstals and RMS noise
  TString infile = "fstPedNoise.dat";
  cout<<" Opening File "<<infile<<endl;

  std::ifstream in(infile);
  if (!in.is_open()) {
    cout << "Can't find file!\n"; 
    exit(0); 
  } 

  //open up file to read in CM noise
  TString infile2 = "fstCmNoise.dat";
  cout<<" Opening File "<<infile2<<endl;

  std::ifstream in2(infile2);
  if (!in2.is_open()) {
    cout << "Can't find file!\n";
    exit(0);
  }

  Int_t rdo, arm, apv, chan;

  Float_t pedestalT, rmsT, gainT;
  Int_t channelId, timebin;

  Float_t cmNoiseT;
  Int_t chipId, timebin2;

  Int_t usedTB = 4;

  //istPedNoise_st table;
 
  //for (int i = 0; i < 110592; i++) {
  //  table.pedestal[i] = 0; 
  //  table.rmsNoise[i] = 0;
  //}

  //for (int i = 0; i < 864; i++) {
  //  table.cmNoise[i] = 0;  
  //}


  cout<<"TDataSet *CreateTable() {"<<endl;
  cout<<"// -----------------------------------------------------------------"<<endl;
  cout<<"  if (!gROOT->GetClass(\"St_fstPedNoise\")) return 0;"<<endl;
  cout<<"  fstPedNoise_st row;"<<endl;
  cout<<"   St_fstPedNoise *tableSet = new St_fstPedNoise(\"fstPedNoise\",1);"<<endl;

  Int_t counter = 0;
  while (!in2.eof()){
    in2 >> chipId >> rdo >> arm >> apv >> timebin >> cmNoiseT;
    if(timebin==0) cout<<"//Chip"<<chipId<<endl;
    cout<<"         row.cmNoise["<<chipId<<"]["<<timebin<<"]="<<100*(int)cmNoiseT<<";"<<endl;
  }
  in2.close();

  counter = 0;
  while (!in.eof()){
    in >> channelId >> rdo >> arm >> apv >> chan >> timebin >> pedestalT >> rmsT;
    cout<<"	row.pedestal["<<channelId<<"]["<<timebin<<"]="<<pedestalT<<";"<<endl;
    cout<<"	row.totNoise["<<channelId<<"]["<<timebin<<"]="<<100*(int)rmsT<<";"<<endl;
    cout<<"	row.ranNoise["<<channelId<<"]["<<timebin<<"]="<<100*(int)rmsT<<";"<<endl; 
  }
  in.close();
  cout<<" tableSet->AddAt(&row, 0);"<<endl;
  cout<<" return (TDataSet *)tableSet;"<<endl;
  cout<<"}"<<endl;
 
  // Store data to the StDbTable
  //dbtable->SetTable((char*)&table, 1);
  
  // Store table to database
  //mgr->storeDbTable(dbtable);
  

};

