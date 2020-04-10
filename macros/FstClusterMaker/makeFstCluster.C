#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/lib/libFstClusterMaker.dylib)

int makeFstCluster()
{
  std::cout << "gSystem::Load <- libFstClusterMaker.dylib" << endl;

  FstClusterMaker *fst = new FstClusterMaker();

  bool isSavePed = true;
  // bool isSavePed = false;
  std::string hv = "HV140";
  std::string inputlist = "/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/list/FST/FstData_" + hv + ".list";
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile;
  if(isSavePed) outputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstClusters_" + hv + "_withPed.root";
  if(!isSavePed) outputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstClusters_" + hv + "_woPed.root";
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
