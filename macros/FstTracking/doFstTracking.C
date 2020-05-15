#include <iostream>
#include <string>
#include "TSystem.h"
#include "TString.h"

R__LOAD_LIBRARY(../../lib/libFstTracking.dylib)

int doFstTracking(string hv = "HV200V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.5, int numOfUsedTimeBins = 2)
{
  std::cout << "gSystem::Load <- libFstTracking.dylib" << endl;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  std::string inputlist = Form("../../list/FST/configuration/FstCluster_%s_Th%1.1fTb%d_%s_%s.list",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,pedMode.c_str(),cmnMode.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile = Form("../../output/configuration/FstTracking_%s_Th%1.1fTb%d_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,pedMode.c_str(),cmnMode.c_str());
  cout << "output file set to: " << outputfile.c_str() << endl;

  FstTracking *fst = new FstTracking();
  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
