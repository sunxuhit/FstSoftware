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
  const int numTBins    = 7;
  const int pedTimeBin  = 0; // time bin used for pedestal calculation

  // FST Geometry
  const int numRStrip = 8; // 0-3 for inner sector | 4-7 for outer sector
  const int numPhiSeg = 128; // 128 segmentations in phi direction
  const double rInner = 50.0; // mm | smallest radius of inner sector
  const double rOuter = 165.0; // mm | smallest radius of outer sector
  const double pitchR  = 28.75; // mm | (280-165)/4.0
  const double pitchPhi = TMath::Pi()*30.0/180.0/numPhiSeg; // rad | 30 degree with 128 segmentations
  const double pitchLayer03 = 134.9375; // mm
  // const double phi_rot = 0.049982273; // rad
  // const double x_shift = 133.23313; // mm 
  // const double y_shift = -18.801690; // mm 
  const double phi_rot = 0.0499824; 
  const double x_shift = 133.229; 
  const double y_shift = -18.8284;
  // const double phi_rot_ist1 = 0.0674768; // rad
  // const double phi_rot_ist3 = 0.0714665; // rad
  // const double x_shift = 129.384; //mm
  // const double y_shift = -17.5406; //mm

  // IST Cosmic Test Stand Geometry
  const int noRows    = 64; // for one group of sensors
  const int noColumns = 24;
  const double pitchRow    = 0.596; //mm
  const double pitchColumn = 6.275; //mm
  const double pitchLayer12  = 34.925; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
  const double pitchLayer23  = 30.1625; //mm, distances between the 2nd & 3rd Layer

  const int nPedCuts = 3;
  const int nHitCuts = 5;
  const int maxNHits = 15; // max number of hits array
  const int maxNHitsPerEvent = 10; // max number of hits per event => bail the event if found more than this number
  const double MinNoise = 10.0;
  const double maxAngle = 100.0;
  const double rad2deg = 180.0/3.141593;
}

#endif
