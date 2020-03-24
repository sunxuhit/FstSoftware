#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/lib/libFstTracking.dylib)

int doFstTracking()
{
  std::cout << "gSystem::Load <- libFstTracking.dylib" << endl;

  FstTracking *fst = new FstTracking();

  fst->set_list("/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/list/FST/FstData_HV70.list");
  fst->set_outputfile("FstData_HV70.root");

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
