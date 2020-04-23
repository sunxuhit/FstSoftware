#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(../../lib/libFstClusterMaker.dylib)

int makeFstCluster(string hv = "HV140V", string config = "Th4o5Tb3", bool isSavePed = true)
{
  std::cout << "gSystem::Load <- libFstClusterMaker.dylib" << endl;

  FstClusterMaker *fst = new FstClusterMaker();

  // bool isSavePed = true;
  // bool isSavePed = false;
  // std::string hv = "HV140V";
  std::string inputlist = Form("../../list/FST/FstData_%s.list",hv.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile;
  if(isSavePed) outputfile = Form("../../output/FstClusters_%s_withPed_%s.root",hv.c_str(),config.c_str());
  if(!isSavePed) outputfile = Form("../../output/FstClusters_%s_woPed_%s.root",hv.c_str(),config.c_str());
  cout << "output file set to: " << outputfile.c_str() << endl;

  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());
  fst->set_saveped(isSavePed);

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
