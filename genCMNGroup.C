#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int genCMNGroup()
{
  string outputfile = "./cmnGroup.txt";
  ofstream file_cmnGroup;
  file_cmnGroup.open(outputfile.c_str());

  const int ApvNumOffset = 12;

  if (!file_cmnGroup.is_open())
  {
    std::cout << "failed to open " << outputfile.c_str() << endl;
    return -1;
  }
  else
  {
    file_cmnGroup << "apv    " << "channel    " << "group" << endl;
    int group = -1;
    for(int apvIdx = 0; apvIdx < 24; ++apvIdx)
    {
      int lclApvIdx = apvIdx < ApvNumOffset ? apvIdx:(apvIdx-ApvNumOffset);
      int secIdx = lclApvIdx < 4 ? 0:1;
      if(lclApvIdx > 7) secIdx = -1;
      for(int chanIdx = 0; chanIdx < 128; ++ chanIdx)
      {
	if(lclApvIdx == 0 || lclApvIdx == 1 || lclApvIdx == 4 || lclApvIdx == 6) 
	{
	  // group = chanIdx%4 + 4*secIdx;
	  group = chanIdx%4;
	}
	if(lclApvIdx == 2 || lclApvIdx == 3 || lclApvIdx == 5 || lclApvIdx == 7) 
	{
	  // group = 3-chanIdx%4 + 4*secIdx;
	  group = 3-chanIdx%4;
	}
	if(lclApvIdx > 7) 
	{
	  group = -1;
	}

	// cout << "apvIdx = " << apvIdx << ", lclApvIdx = " << lclApvIdx << ", secIdx = " << secIdx << ", chanIdx = " << chanIdx << ", group = " << group << endl;
	file_cmnGroup << apvIdx << "    " << chanIdx << "    " << group << endl;
      }
    }
  }

  file_cmnGroup.close();

  return 1;
}
