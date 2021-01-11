#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(../../lib/libFstNoiseStudy.dylib)

int doFstChipTempNoiseStudy(string module = "Mod04", string mode = "NoCooling8")
{
  std::cout << "gSystem::Load <- libFstNoiseStudy.dylib" << endl;

  FstNoiseStudy *fst = new FstNoiseStudy();

  std::string inputlist = Form("../../list/FST/cooling/Fst%s_%s_HV0V.list",mode.c_str(),module.c_str());
  cout << "input list set to: " << inputlist.c_str() << endl;

  std::string outputfile = Form("../../output/noise/Cooling/Fst%sNoise_%s_HV0V.root",mode.c_str(),module.c_str());
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
