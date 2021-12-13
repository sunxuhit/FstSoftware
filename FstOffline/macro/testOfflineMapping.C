#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int testOfflineMapping()
{
  //constants declarations...
  const int kFstNumWedges               = 36;  ///< 36 FST wedge
  const int kFstNumSensorsPerWedge      = 3;   ///< 3 sensor per one FST Wedge
  const int kFstNumInnerSensorsPerWedge = 1;   ///< 1 inner sensor per one FST Wedge
  const int kFstNumOuterSensorsPerWedge = 2;   ///< 2 outer sensor per one FST Wedge
  const int kFstNumSensors              = 108; ///< 3*36 sensors
  const int kFstNumStripsPerInnerSensor = 512; ///< 128*4 strips in each inner sensor
  const int kFstNumStripsPerOuterSensor = 256; ///< 64*4 strips in each outer sensor
  const int kFstNumWedgePerDisk         = 12;  ///< 12 wedge per one FST Disk
  const int kFstNumPhiSegPerWedge       = 128; ///< phi segmentation in r-phi direction per each wedge
  const int kFstNumPhiSegPerSensor      = 128; ///< phi segmentation in r-phi direction per each sensor: 128 for sensor 1 | 64 for sensor 2 & 3
  const int kFstNumRStripsPerSensor     = 4;   ///< 4 R-strips in r-phi direction per each sensor

  /*
   * Hard-coded constants
   */
  //electronics constants
  const int kFstNumRdos            = 6;     // Rdos, numbering from 1, 2, 3, ..., 6
  const int kFstNumArmsPerRdo      = 3;     // 3 ARMs covered per Rdo
  const int kFstNumApvsPerArm      = 16;    // 16 APV chips covered by one ARM, numbering 0-7 & 12-19
  const int kFstNumApvChannels     = 128;   // 128 readout channels per one APV chip
  const int kFstNumChanPerArm      = 2048;  // 16*128 channels per ARM board
  const int kFstNumElecIds         = 36864; // 8*36*128 channels, elec. ID numbering from 0 to 36863
  //const int kFstApvsPerSection   = 12;      //  APV chips covered by one section of a Wedge
  const int kFstApvsPerWedge       = 8;     // 8 APV chips covered in one Wedge
  const int kFstNumApvs            = 288;   // 36*8 APV chips in whole FST detector
  const int kFstNumRStripsPerWedge = 8;     // 8 R-strips per each Wedge
  const int kFstNumDisk            = 3;     // 3 Disk
  const int kFstNumWedsPerRdo      = 6;     // 6 wedegs per Rdo
  const int kFstNumWedsPerArm      = 2;     // 2 wedges per ARM

  int wedgesId = -1;
  for(int geoId = 0; geoId < kFstNumElecIds; ++geoId)
  {
    int wedge = 1 + geoId / (kFstNumInnerSensorsPerWedge * kFstNumStripsPerInnerSensor + kFstNumOuterSensorsPerWedge * kFstNumStripsPerOuterSensor);
    if(wedgesId != wedge)
    {
      wedgesId = wedge;
      cout << "geoId = " << geoId << ", wedgesId = " << wedgesId << endl;
    }
  }

  for(int geoId = 0; geoId < kFstNumElecIds; ++geoId)
  {
    int rstrip = (geoId % (kFstNumInnerSensorsPerWedge * kFstNumStripsPerInnerSensor + kFstNumOuterSensorsPerWedge * kFstNumStripsPerOuterSensor))/kFstNumPhiSegPerWedge;
    cout << "geoId = " << geoId << ", rstrip = " << rstrip << endl;
  }

  return 1;
}
