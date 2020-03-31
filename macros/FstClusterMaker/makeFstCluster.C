#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/lib/libFstClusterMaker.dylib)

int makeFstCluster()
{
  std::cout << "gSystem::Load <- libFstClusterMaker.dylib" << endl;

  FstClusterMaker *fst = new FstClusterMaker();

  std::string hv = "HV140";
  std::string inputlist = "/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/list/FST/FstData_" + hv + ".list";
  std::string outputfile = "FstData_" + hv + ".root";

  cout << "input list set to: " << inputlist.c_str() << endl;
  cout << "output file set to: " << outputfile.c_str() << endl;

  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
