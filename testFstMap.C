#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int testFstMap()
{
  //constants declarations...
  const int totDisk      = 3;     // 1 disk per crate
  const int totRdo       = 6;     // 2 rdos * 3 crates
  const int totArm       = 18;    // 3 arms * 2 rdos * 3 crates
  const int totMod       = 36;    // 12 modules * 3 disks (6 modules * 6 rdos)
  const int totSec       = 72;    // 2 sections * 12 moudles * 3 disks
  const int totSensor    = 144;   // 2 sensors (inner: 0-1 & outer: 2-3) * 2 sections * 12 moudles * 3 disks
  const int totAPV       = 288;   // 8 APV chips * 12 modules * 3 disks
  const int totCh        = 36864; // 128 channels * 288 APV chips

  const int RdoPerDisk   = 2;     // per disk
  const int ArmPerDisk   = 6;     // per disk
  const int ModPerDisk   = 12;    // per disk
  const int SecPerDisk   = 24;    // 2 (SecPerMod) * 2 (PortPerArm) * 3 (ArmPerRdo) * 2 (RdoPerDisk)
  const int ArmPerRdo    = 3;     // per RDO
  const int ModPerRdo    = 6;     // per RDO
  const int SecPerRdo    = 12;    // 2 (SecPerMod) * 2 (PortPerArm) * 3 (ArmPerRdo)
  const int PortPerArm   = 2;     // per ARM
  const int ModPerArm    = 2;     // per ARM
  const int SecPerArm    = 4;     // 2 (SecPerMod) * 2 (PortPerArm)
  const int SecPerMod    = 2;     // per module
  const int SensorPerMod = 4;     // per module
  const int RstripPerMod = 8;     // per module: 0-3 for inner section | 4-7 for outer section
  const int PhiSegPerMod = 128;   // per module

  const int ChPerApv     = 128;
  const int ApvPerSensor = 2;     // sensor0: APV0&1 | sensor1: APV2&3 | sensor2: APV4&5 | sensor3: APV6&7
  const int ChPerSensor  = 256;   // 128 channels * 2 APV chips
  const int ApvPerSec    = 4;
  const int ChPerSec     = 512;   // 128 channels * 4 APV chips
  const int ApvPerMod    = 8;
  const int ChPerMod     = 1024;  // 128 channels * 8 APV chips
  const int ApvPerPort   = 8;
  const int ChPerPort    = 1024;  // 128 channels * 8 APV chips
  const int ApvPerArm    = 16;    // 8 APV chips * 2 modules | APV number: port0: 0-7 & port1: 12-19
  const int ChPerArm     = 2048;  // 128 channels * 16 APV chips
  const int ApvPerRdo    = 48;    // 8 APV chips * 6 modules
  const int ChPerRdo     = 6144;  // 128 channels * 48 APV chips
  const int ApvPerDisk   = 96;    // 8 APV chips * 12 modules
  const int ChPerDisk    = 12288; // 128 channels * 96 APV chips

  const int ApvNumOffset = 12;    // APV RO number same as IST | used for APV number convertion
  const int ApvRoPerPort = 12;    // APV RO number same as IST
  const int ApvRoPerArm  = 24;    // APV RO number same as IST

  int fstElecMapping[totCh]; //FST channel mapping (geometry ID & electronics ID transform)
  int fstGeomMapping[totCh]; //FST channel mapping (electronics ID to geometry ID transform)

  // constant used for FST Geometry Hit Map
  // all values are defined by inner direction
  const int zFilp[totDisk]         = {1,-1,1};
  const int zDirct[ModPerDisk]     = {1,-1,1,-1,1,-1,1,-1,1,-1,1,-1};
  const float phiStart[ModPerDisk] = {2.0, 2.0, 0.0, 12.0, 10.0, 10.0, 8.0, 8.0, 6.0, 6.0, 4.0, 4.0}; // * pi/6
  const float phiStop[ModPerDisk]  = {3.0, 1.0, 1.0, 11.0, 11.0,  9.0, 9.0, 7.0, 7.0, 5.0, 5.0, 3.0}; // * pi/6
  const float phiDelta             = TMath::TwoPi()/(PhiSegPerMod*ModPerDisk); // 2pi/1536 in rad
  const float rStart[RstripPerMod] = {50.00, 78.75, 107.50, 136.25, 165.00, 193.75, 222.50, 251.25}; // in mm
  const float rStop[RstripPerMod]  = {78.75, 107.5, 136.25, 165.00, 193.75, 222.50, 251.25, 280.00}; // in mm
  const float rDelta               = 28.75; // in mm

  //initializing FST mapping
  cout << "Initializing FST Map!" << endl;
  string outputfile = "./FstChannelMap.txt";
  ofstream file_channelMap;
  file_channelMap.open(outputfile.c_str());

  file_channelMap << "rdoIdx    " << " armIdx    " << "refApvIdx    " << "apvIdx    " << "sigElecChanId    " << "glbElecChanId    " << "glbGeomChanId" << endl;
  for(int rdoIdx=1; rdoIdx<=totRdo; rdoIdx++) 
  { // 1-6
    for(int armIdx=0; armIdx<ArmPerRdo; armIdx++) 
    { // 0-2
      for(int refApvIdx=0; refApvIdx<ApvPerArm; refApvIdx++) 
      { // 0-15
	int portIdx      = refApvIdx/ApvPerPort;                            // 0-1
	int lclApvIdx    = refApvIdx-portIdx*ApvPerPort;                    // 0-7
	int lclSensorIdx = lclApvIdx/ApvPerSensor;                          // 0-3
	int apvIdx       = lclApvIdx + portIdx*ApvNumOffset;                // 0-7 || 12-19

	int diskIdx   = (rdoIdx-1)/2+1;                                     // 1-3
	int moduleIdx = (rdoIdx-1)%2*ModPerRdo+armIdx*PortPerArm+portIdx+1; // 1-12
	for(int sigElecChanId=0; sigElecChanId<ChPerApv; sigElecChanId++) 
	{ // 0-127
	  int glbElecChanId = (rdoIdx-1)*ChPerRdo + armIdx*ChPerArm + refApvIdx*ChPerApv + sigElecChanId; // 0-36863
	  int lclRstripIdx = -1; // 0-7
	  int lclPhiSegIdx = -1; // 0-127

	  // Inner Section: APV 0-1 for sensor 0 | APV 2-3 for sensor 1
	  // Outer Section: APV 4-5 for sensor 2 | APV 6-7 for sensor 3
	  if(lclApvIdx == 0 || lclApvIdx == 1 || lclApvIdx == 4 || lclApvIdx == 6) lclRstripIdx = sigElecChanId%4;
	  if(lclApvIdx == 2 || lclApvIdx == 3 || lclApvIdx == 5 || lclApvIdx == 7) lclRstripIdx = 3-sigElecChanId%4;
	  if(lclSensorIdx == 2 || lclSensorIdx == 3) lclRstripIdx = lclRstripIdx+4; // outer

	  if(lclSensorIdx == 0 || lclSensorIdx == 1) lclPhiSegIdx = lclApvIdx*32+sigElecChanId/4; // inner
	  if(lclSensorIdx == 2 || lclSensorIdx == 3) lclPhiSegIdx = (lclApvIdx-ApvPerSec)*32+sigElecChanId/4; // outer

	  int glbGeomChanId = (diskIdx-1)*ChPerDisk + (moduleIdx-1)*ChPerMod + lclRstripIdx*PhiSegPerMod + lclPhiSegIdx; // 0-36863

	  fstGeomMapping[glbElecChanId] = glbGeomChanId;
	  fstElecMapping[glbGeomChanId] = glbElecChanId;

	  file_channelMap << rdoIdx << "    " << armIdx << "    " << refApvIdx << "    " << apvIdx << "    " << sigElecChanId << "    " << glbElecChanId << "    " << glbGeomChanId << endl;
	}
      }
    }
  }
  file_channelMap.close();

  // convert the glbElecChanId to RO#
  cout << "Convert glbElecChanId to RO#!" << endl;
  outputfile = "./FstGlbElecChannelMap.txt";
  ofstream file_glbElecChannelMap;
  file_glbElecChannelMap.open(outputfile.c_str());

  file_glbElecChannelMap  << "glbElecChanId    "<< "rdoIdx    " << "armIdx    " << "refApvIdx    " << "apvIdx    " << "sigElecChanId" <<  endl;
  for(int glbElecChanId = 0; glbElecChanId < totCh; ++glbElecChanId)
  { // 0-36863
    int rdoIdx        = glbElecChanId/ChPerRdo + 1;                                    // 1-6
    int armIdx        = (glbElecChanId - (rdoIdx-1)*ChPerRdo)/ChPerArm;                // 0-2
    int refElecChanId = glbElecChanId - (rdoIdx-1)*ChPerRdo - armIdx*ChPerArm;         // 0-2047
    int refApvIdx     = refElecChanId/ChPerApv;                                        // 0-15
    int glbElecApvIdx = (rdoIdx-1)*ApvPerRdo + armIdx*ApvPerArm + refApvIdx;           // 0-287
    int portIdx       = refApvIdx/ApvPerPort;                                          // 0-1
    int lclApvIdx     = refApvIdx-portIdx*ApvPerPort;                                  // 0-7
    int apvIdx        = lclApvIdx + portIdx*ApvNumOffset;                              // 0-7 || 12-19
    int lclElecChanId = refElecChanId - portIdx*ChPerMod;                              // 0-1023
    int sigElecChanId = lclElecChanId%ChPerApv;                                        // 0-127
    int glbSecIdx     = (rdoIdx-1)*SecPerRdo + armIdx*SecPerArm + refApvIdx/ApvPerSec; // 0-71
    file_glbElecChannelMap << glbElecChanId  << "    " <<  rdoIdx << "    " << armIdx << "    " << refApvIdx << "    " << apvIdx << "    " << sigElecChanId << endl;
  }
  file_glbElecChannelMap.close();

  // convert the glbGeomChanId to Geometry#
  cout << "Convert glbGeomChanId to Geometry!" << endl;
  outputfile = "./FstGlbGeomChannelMap.txt";
  ofstream file_glbGeomChannelMap;
  file_glbGeomChannelMap.open(outputfile.c_str());

  file_glbGeomChannelMap << "glbGeomChanId    "<< "diskIdx    " << "moduleIdx    " << "lclRstripIdx    " << "lclPhiSegIdx    " << "rStrip    " << "phiSeg" <<  endl;
  for(int glbGeomChanId = 0; glbGeomChanId < totCh; ++glbGeomChanId) 
  { // loop over glbGeomChanId
    int diskIdx       = glbGeomChanId/ChPerDisk + 1;                                   // 1-3
    int moduleIdx     = (glbGeomChanId-(diskIdx-1)*ChPerDisk)/ChPerMod + 1;            // 1-12
    int glbModuleIdx  = (diskIdx-1)*ModPerDisk + moduleIdx;                            // 1-36
    int lclGeomChanId = glbGeomChanId- (diskIdx-1)*ChPerDisk - (moduleIdx-1)*ChPerMod; // 0-1023
    int lclRstripIdx  = lclGeomChanId/PhiSegPerMod;                                    // 0-7
    int lclPhiSegIdx  = lclGeomChanId%PhiSegPerMod;                                    // 0-127
    int glbPhiSegIdx  = (moduleIdx-1)*PhiSegPerMod + lclPhiSegIdx;                     // 0-1535

    // FST Geometry Hit Map
    float phiInner = -999.9;
    float phiOuter = -999.9;
    float phiSeg   = -999.9;
    float rStrip   = -999.9;
    if(diskIdx == 1 || diskIdx == 3)
    { // Disk 1 & 3
      phiInner = phiStart[moduleIdx-1]*TMath::Pi()/6.0 + 0.5*zDirct[moduleIdx-1]*phiDelta;
      phiOuter = phiStop[moduleIdx-1]*TMath::Pi()/6.0  - 0.5*zDirct[moduleIdx-1]*phiDelta;
    }
    if(diskIdx == 2)
    { // Disk 2
      phiInner = phiStop[moduleIdx-1]*TMath::Pi()/6.0  - 0.5*zDirct[moduleIdx-1]*phiDelta;
      phiOuter = phiStart[moduleIdx-1]*TMath::Pi()/6.0 + 0.5*zDirct[moduleIdx-1]*phiDelta;
    }

    if(lclRstripIdx < RstripPerMod/2)
    { // inner
      phiSeg = phiInner + zFilp[diskIdx-1]*zDirct[moduleIdx-1]*lclPhiSegIdx*phiDelta;
      rStrip = rStart[lclRstripIdx] + 0.5*rDelta;
    }
    else
    {
      // outer
      phiSeg = phiOuter - zFilp[diskIdx-1]*zDirct[moduleIdx-1]*lclPhiSegIdx*phiDelta;
      rStrip = rStart[lclRstripIdx] + 0.5*rDelta;
    }
    file_glbGeomChannelMap << glbGeomChanId << "    "<< diskIdx << "    " << moduleIdx << "    " << lclRstripIdx << "    " << lclPhiSegIdx << "    " << rStrip << "    " << phiSeg <<  endl;
  }
  file_glbGeomChannelMap.close();

#if 0
  cout << "Conversion from module index to RO#:" << endl;
  for(int index = 0; index < 36; index++)
  { // per module
    int rdoIndex   = index/ModPerRdo + 1;         //1-6
    int armIndex   = (index%ModPerRdo)/ModPerArm; //0-2
    int portIndex  = index%PortPerArm;            //0-1

    int diskId = (rdoIndex-1)/2+1;                                                 // 1-3
    int moduleId = (rdoIndex-1)%2*ModPerRdo + PortPerArm*armIndex + portIndex + 1; // 1-12
    cout << "index = " << index << ", diskId = " << diskId << ", moduleId = " << moduleId << ", rdoIndex = " << rdoIndex << ", armIndex = " << armIndex << ", portIndex = " << portIndex << endl;
  }

  cout << "Conversion from section index to RO#:" << endl;
  for(int index = 0; index < 72; index++)
  { // per section
    int rdoIndex   = index/SecPerRdo + 1;          //1-6
    int armIndex   = (index%SecPerRdo)/SecPerArm;  //0-2
    int portIndex  = (index/SecPerMod)%PortPerArm; //0-1
    int secIndex   = index%SecPerMod; //0-1

    int diskId = (rdoIndex-1)/2+1;                                                 // 1-3
    int moduleId = (rdoIndex-1)%2*ModPerRdo + PortPerArm*armIndex + portIndex + 1; // 1-12
    cout << "index = " << index << ", diskId = " << diskId << ", moduleId = " << moduleId << ", rdoIndex = " << rdoIndex << ", armIndex = " << armIndex << ", portIndex = " << portIndex << ", secIndex = " << secIndex << endl;
  }
#endif

  return 1;
}
