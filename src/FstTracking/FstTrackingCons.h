#ifndef FstTrackingCons_h
#define FstTrackingCons_h

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

  // IST Cosmic Test Stand Geometry
  const int noRows    = 64; // for one group of sensors
  const int noColumns = 24;
  const float pitchRow    = 0.596  ; //mm
  const float pitchColumn = 6.275  ; //mm
  const float pitchLayer12  = 34.925   ; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
  const float pitchLayer23  = 30.1625   ; //mm, distances between the 2nd & 3rd Layer

  const int nPedCuts = 3;
  const int nHitCuts = 5;
  const int maxNHits = 15; // max number of hits array
  const int maxNHitsPerEvent = 10; // max number of hits per event => bail the event if found more than this number
  const float MinNoise = 10.0;
  const float maxAngle = 100.0;
  const float rad2deg = 180.0/3.141593;
}

#endif
