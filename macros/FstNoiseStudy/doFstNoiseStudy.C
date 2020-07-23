#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(../../lib/libFstNoiseStudy.dylib)

int doFstNoiseStudy(string module = "Mod03", string hv = "HV70V", string mode = "Ped")
{
  std::cout << "gSystem::Load <- libFstNoiseStudy.dylib" << endl;

  FstNoiseStudy *fst = new FstNoiseStudy();

  std::string inputlist = Form("../../list/FST/Fst%s_%s_%s.list",mode.c_str(),module.c_str(),hv.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile = Form("../../output/noise/Fst%sNoise_%s_%s.root",mode.c_str(),module.c_str(),hv.c_str());
  cout << "output file set to: " << outputfile.c_str() << endl;

  fst->set_list(inputlist.c_str());
  fst->set_outputfile(outputfile.c_str());

  int stats = fst->Init();
  if(stats < 0)
  {
    return -1;
  }

  fst->Make();
  fst->Finish();
  std::cout << "All done" << endl;

  return 1;
}
