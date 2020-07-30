#ifndef FstTrackingCons_h
#define FstTrackingCons_h

#include "TMath.h"

namespace FST
{
  const int numARMs        = 2;
  const int numPorts       = 2;  // note that the readout just gives 24 APVs on a single ARM, there is
  const int numAPVs        = 12; // no actual Port information, we just introduce Port information ourselves
  const int numROChannels  = 1152; // numFstChannels*numFstTBins
  const int numChannels    = 128;
  const int numTBins       = 9;
  const int pedTimeBin     = 3; // time bin used for pedestal calculation
  const int defaultTimeBin = 3; // default time bin

  // FST Geometry
  const int numFstAPVs      = 8; // 0-3 for inner sector | 4-7 for outer sector
  const int numRStrip       = 8; // 0-3 for inner sector | 4-7 for outer sector
  const int numPhiSeg       = 128; // 128 segmentations in phi direction
  const double rInner       = 50.0; // mm | smallest radius of inner sector
  const double rOuter       = 165.0; // mm | smallest radius of outer sector
  const double pitchR       = 28.75; // mm | (280-165)/4.0
  const double pitchPhi     = TMath::Pi()*30.0/180.0/numPhiSeg; // rad | 30 degree with 128 segmentations
  const double gapPhi       = TMath::Pi()*1.0/180.0; // 1 degree gap between 2 outer sensor
  const double pitchLayer03 = 134.9375;

  // FST Clustering
  const int mFstNumSensorsPerModule = 3; // 0 for APV 0-3(1-4) on inner sector | 1 for APV 4&5(5&6) on outer sector | 2 for APV 6&7(7&8) on outer sector
  const int mFstNumPhiSegPerSensor  = 128; // 128 for sensor 0 | 64 for sensor 1 & 2
  const int mFstNumRstripPerSensor  = 4;
  const int mFstNumMatching = 8; // 0: no matching | 1-7 matching within (1-7)*pitchR in r & 3.0(6.0)*pitchPhi in phi

  // FST Limits 
  const double mFstRMax[mFstNumSensorsPerModule]   = {rInner+4.0*pitchR, rOuter+4.0*pitchR, rOuter+4.0*pitchR}; // 165 mm, 280 mm, 280 mm
  const double mFstRMin[mFstNumSensorsPerModule]   = {rInner, rOuter, rOuter}; // 50 mm, 165 mm, 165 mm
  const double mFstPhiMax[mFstNumSensorsPerModule] = {TMath::Pi()*15.0/180.0, TMath::Pi()*15.0/180.0+0.5*gapPhi, -0.5*gapPhi}; // 64.0*FST::pitchPhi, 64.0*FST::pitchPhi +0.5*gapPhi, -0.5*gapPhi
  const double mFstPhiMin[mFstNumSensorsPerModule] = {-TMath::Pi()*15.0/180.0, 0.5*gapPhi, -TMath::Pi()*15.0/180.0-0.5*gapPhi}; // -64.0*FST::pitchPhi, +0.5*gapPhi, -64.0*FST::pitchPhi-0.5*gapPhi

  // FST Histograms Limits
  const double rMax[mFstNumSensorsPerModule]   = {rInner+5.0*pitchR, rOuter+5.0*pitchR, rOuter+5.0*pitchR}; // mFstRMax + 1.0*pitchR
  const double rMin[mFstNumSensorsPerModule]   = {rInner-1.0*pitchR, rOuter-1.0*pitchR, rOuter-1.0*pitchR}; // mFstRMin - 1.0*pitchR
  const double phiMax[mFstNumSensorsPerModule] = {TMath::Pi()*15.0/180.0+10*pitchPhi, TMath::Pi()*15.0/180.0+0.5*gapPhi+10*pitchPhi, -0.5*gapPhi+10*pitchPhi}; // mFstPhiMax + 10.0*pitchPhi
  const double phiMin[mFstNumSensorsPerModule] = {-TMath::Pi()*15.0/180.0-10*pitchPhi, 0.5*gapPhi-10*pitchPhi, -TMath::Pi()*15.0/180.0-0.5*gapPhi-10*pitchPhi}; // mFstPhiMin - 10.0*pitchPhi
  const double phiMatchCut[mFstNumSensorsPerModule][mFstNumRstripPerSensor] = {{0.04,0.03,0.02,0.02},{0.02,0.015,0.01,0.01},{0.02,0.015,0.01,0.01}};

  // IST Cosmic Test Stand Geometry
  const int noRows          = 64; // for one group of sensors
  const int noColumns       = 24;
  const double pitchRow     = 0.596; //mm
  const double pitchColumn  = 6.275; //mm
  const double pitchLayer12 = 34.925; //mm, distances between the 1st & 2nd Layer
  const double pitchLayer23 = 30.1625; //mm, distances between the 2nd & 3rd Layer

  // Alignment Parameters
  const double phi_rot_ist1 = -0.00113551; // aligned IST1 & IST3 to IST2
  const double x1_shift     = 0.530444;
  const double y1_shift     = 1.17133;
  const double phi_rot_ist3 = -0.00379431;
  const double x3_shift     = 0.45811;
  const double y3_shift     = 1.0116;

  // alignment from IST2 to each sensor of FST01
  // const int mDefSenorId = 1; // default FST sensor for alignment (phi_rot, x_shift and y_shift | z_shift is from the sensorId to be aligned)
  // const double delta_x = 0.0;
  // const double phi_rot[mFstNumSensorsPerModule] = {0.0, 0.0311535, 0.0};
  // const double x_shift[mFstNumSensorsPerModule] = {0.0, 141.268,   0.0}; 
  // const double y_shift[mFstNumSensorsPerModule] = {0.0, -19.7014,  0.0};
  // const double z_shift[mFstNumSensorsPerModule] = {0.0, 0.644155,  0.0};

  // alignment from IST2 to each sensor of FST03 HV70V-HV120V
  const int mDefSenorId = 0; // default FST sensor for alignment (phi_rot, x_shift and y_shift | z_shift is from the sensorId to be aligned)
  const double delta_x = 3.0;
  const double phi_rot[mFstNumSensorsPerModule] = {3.16613, 3.15984, 3.16009};
  const double x_shift[mFstNumSensorsPerModule] = {216.908+delta_x, 221.566+delta_x, 222.031+delta_x};
  const double y_shift[mFstNumSensorsPerModule] = {33.5933, 34.1950, 33.7715};
  const double z_shift[mFstNumSensorsPerModule] = {22.9890, 10.3229, 11.4462};

  // alignment from IST2 to each sensor of FST03 HV140V
  // const int mDefSenorId = 0; // default FST sensor for alignment (phi_rot, x_shift and y_shift | z_shift is from the sensorId to be aligned)
  // const double delta_x = 0.0;
  // const double phi_rot[mFstNumSensorsPerModule] = {3.16689, 3.16012, 3.15895};
  // const double x_shift[mFstNumSensorsPerModule] = {266.293+delta_x, 270.505+delta_x, 269.488+delta_x};
  // const double y_shift[mFstNumSensorsPerModule] = {34.2772, 35.3617, 34.8087};
  // const double z_shift[mFstNumSensorsPerModule] = {22.1754, 10.5996, 8.84978};

  // Hits Selection
  const float nFstPedsCut      = 3.0; // used for ped
  const float nIstPedsCut      = 3.0;
  const float nIstHitsCut      = 6.0;
  const float nIstThresholdCut = 3.0;
  const int maxNHits           = 25; // max number of hits array
  const int maxNHitsPerEvent   = 20; // max number of hits per event = > bail the event if found more than this number
  const double MinNoise        = 10.0;
  const double maxAngle        = 100.0;
  const double rad2deg         = 180.0/3.141593;

  const double mSigWeight_FST[8] = {1.0,1.0,1.0,1.0,1.0,0.571882,0.299947,0.164641};
  const double mSigWeight_IST = 1.0;
}

#endif
