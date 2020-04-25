#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(../../lib/libFstQAStudy.dylib)

int doFstQAStudy_timesequence(string hv = "HV140V", string config = "Th4o5Tb3", bool isSavePed = "true", string date = "04022020")
{
  std::cout << "gSystem::Load <- libFstQAStudy.dylib" << endl;

  FstQAStudy *fst = new FstQAStudy();

  // bool isSavePed = true;
  // bool isSavePed = false;
  // std::string hv = "HV140V";
  std::string inputlist;
  if(isSavePed) inputlist = Form("../../list/FST/timesequence/FstCluster_%s_withPed_%s_%s.list",hv.c_str(),config.c_str(),date.c_str());
  if(!isSavePed) inputlist = Form("../../list/FST/timesequence/FstCluster_%s_woPed_%s_%s.list",hv.c_str(),config.c_str(),date.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile;
  if(isSavePed) outputfile = Form("../../output/timesequence/FstQAStudy_%s_withPed_%s_%s.root",hv.c_str(),config.c_str(),date.c_str());
  if(!isSavePed) outputfile = Form("../../output/timesequence/FstQAStudy_%s_woPed_%s_%s.root",hv.c_str(),config.c_str(),date.c_str());
  cout << "output file set to: " << outputfile.c_str() << endl;

  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
