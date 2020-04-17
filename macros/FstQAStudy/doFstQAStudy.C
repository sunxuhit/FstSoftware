#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/lib/libFstQAStudy.dylib)

int doFstQAStudy()
{
  std::cout << "gSystem::Load <- libFstQAStudy.dylib" << endl;

  FstQAStudy *fst = new FstQAStudy();

  bool isSavePed = true;
  // bool isSavePed = false;
  std::string hv = "HV140V";
  std::string inputlist;
  if(isSavePed) inputlist  = "/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/list/FST/FstQAStudy_" + hv + "_withPed.list";
  if(!isSavePed) inputlist  = "/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/list/FST/FstQAStudy_" + hv + "_woPed.list";
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile;
  if(isSavePed) outputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstQAStudy_" + hv + "_withPed.root";
  if(!isSavePed) outputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstQAStudy_" + hv + "_woPed.root";
  cout << "output file set to: " << outputfile.c_str() << endl;

  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
