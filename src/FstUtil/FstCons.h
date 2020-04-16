#ifndef FstTrackingCons_h
#define FstTrackingCons_h

#include "TMath.h"

namespace FST
{
  const int numARMs     = 2;
  const int numPorts    = 2;  // note that the readout just gives 24 APVs on a single ARM, there is
  const int numAPVs     = 12; // no actual Port information, we just introduce Port information ourselves
  const int numROChannels = 1152; // numFstChannels*numFstTBins
  const int numChannels = 128;
  const int numTBins    = 9;
  const int pedTimeBin  = 3; // time bin used for pedestal calculation
  const int defaultTimeBin  = 3; // default time bin

  // FST Geometry
  const int numRStrip = 8; // 0-3 for inner sector | 4-7 for outer sector
  const int numPhiSeg = 128; // 128 segmentations in phi direction
  const double rInner = 50.0; // mm | smallest radius of inner sector
  const double rOuter = 165.0; // mm | smallest radius of outer sector
  const double pitchR  = 28.75; // mm | (280-165)/4.0
  const double pitchPhi = TMath::Pi()*30.0/180.0/numPhiSeg; // rad | 30 degree with 128 segmentations
  const double pitchLayer03 = 134.9375; // mm

  // IST Cosmic Test Stand Geometry
  const int noRows    = 64; // for one group of sensors
  const int noColumns = 24;
  const double pitchRow    = 0.596; //mm
  const double pitchColumn = 6.275; //mm
  const double pitchLayer12  = 34.925; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
  const double pitchLayer23  = 30.1625; //mm, distances between the 2nd & 3rd Layer

  // Alignment Parameters
  const double phi_rot_ist1 = -0.00113551; // aligned IST1 & IST3 to IST2
  const double x1_shift = 0.530444;
  const double y1_shift = 1.17133;
  const double phi_rot_ist3 = -0.00379431;
  const double x3_shift = 0.45811;
  const double y3_shift = 1.0116;
  const double phi_rot_ist2 = 0.0438495; // aligned IST2 to FST with 2-layer tracking
  const double x2_shift = 134.99;
  const double y2_shift = -20.7848;

  // const int nPedCuts = 3;
  // const int nHitCuts = 5;
  const float nFstPedsCut = 3.0; // used for ped 
  const float nIstPedsCut = 3.0;
  const float nFstHitsCut = 4.5; // used for hits
  const float nIstHitsCut = 6.0;
  const float nFstThresholdCut = 2.0; // used for threshold & cluster study
  const float nIstThresholdCut = 3.0;
  const int maxNHits = 25; // max number of hits array
  const int maxNHitsPerEvent = 20; // max number of hits per event => bail the event if found more than this number
  const double MinNoise = 10.0;
  const double maxAngle = 100.0;
  const double rad2deg = 180.0/3.141593;
}

#endif
