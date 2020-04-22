#include <iostream>
#include <string>
#include "TSystem.h"
#include "TString.h"

R__LOAD_LIBRARY(../../lib/libFstTracking.dylib)

int doFstTracking(string hv = "HV140V", string config = "Th4o5Tb3")
{
  std::cout << "gSystem::Load <- libFstTracking.dylib" << endl;

  FstTracking *fst = new FstTracking();

  bool isSavePed = true;
  // bool isSavePed = false;
  // std::string hv = "HV140V";
  string inputlist;
  if(isSavePed) inputlist = Form("../../list/FST/FstCluster_%s_withPed_%s.list",hv.c_str(),config.c_str());
  if(!isSavePed) inputlist = Form("../../list/FST/FstCluster_%s_woPed_%s.list",hv.c_str(),config.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile;
  if(isSavePed) outputfile = Form("../../output/FstTracking_%s_withPed_%s.root",hv.c_str(),config.c_str());
  if(!isSavePed) outputfile = Form("../../output/FstTracking_%s_woPed_%s.root",hv.c_str(),config.c_str());

  cout << "output file set to: " << outputfile.c_str() << endl;

  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
