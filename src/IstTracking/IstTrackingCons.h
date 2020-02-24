#ifndef IstTrackingCons_h
#define IstTrackingCons_h

namespace IST
{
  const int numARMs     = 2;
  const int numPorts    = 2;  // note that the readout just gives 24 APVs on a single ARM, there is
  const int numAPVs     = 12; // no actual Port information, we just introduce Port information ourselves
  const int numROChannels = 1152; // numFstChannels*numFstTBins
  const int numChannels = 128;
  const int numTBins    = 9;

  // IST Cosmic Test Stand Geometry
  const int noRows    = 64; // for one group of sensors
  const int noColumns = 24;
  const float pitchRow    = 0.596  ; //mm
  const float pitchColumn = 6.275  ; //mm

  const int nPedCuts = 3;
  const int nHitCuts = 5;
}

#endif
