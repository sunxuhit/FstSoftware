#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(../../lib/libFstClusterMaker.dylib)

int makeFstCluster(string mod = "Mod01", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5, string date = "03182020")
{
  std::cout << "gSystem::Load <- libFstClusterMaker.dylib" << endl;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  std::string inputlist = Form("../../list/FST/data/%s/FstData_%s_%s_%s.list",mod.c_str(),mod.c_str(),hv.c_str(),date.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile = Form("../../output/clusters/%s/FstClusters_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_%s.root",mod.c_str(),mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str(),date.c_str());
  cout << "output file set to: " << outputfile.c_str() << endl;

  FstClusterMaker *fst = new FstClusterMaker();
  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());
  fst->set_saveped(isSavePed);
  fst->set_cmncorrection(isApplyCMNCorr);
  fst->set_nFstHitsCut(nFstHitsCut);
  fst->set_numOfUsedTimeBins(numOfUsedTimeBins);
  fst->set_nFstThresholdCut(nFstThresholdCut2,nFstThresholdCut1);

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
