#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(../../lib/libFstQAStudy.dylib)

int doFstQAStudy(string mod = "Mod03", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = false, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  std::cout << "gSystem::Load <- libFstQAStudy.dylib" << endl;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  std::string inputlist = Form("../../list/FST/configuration/FstCluster_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.list",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile = Form("../../output/configuration/FstQAStudy_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  cout << "output file set to: " << outputfile.c_str() << endl;

  FstQAStudy *fst = new FstQAStudy();
  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());
  fst->set_cmncorrection(isApplyCMNCorr);

  fst->Init();
  fst->Make();
  fst->Finish();

  std::cout << "All done" << endl;

  return 1;
}
