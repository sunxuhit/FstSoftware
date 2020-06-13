#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 1 ]
then
  nFstThresholdCut=$1

  # . ./plotQA.sh HV70V true true 4.5 3 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 4.5 2 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 4.5 1 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 4.0 3 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 4.0 2 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 4.0 1 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 3.5 3 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 3.5 2 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 3.5 1 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 3.0 3 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 3.0 2 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true true 3.0 1 withPed_withCMNCorr
  # . ./plotQA.sh HV70V true false 4.5 3 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 4.5 2 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 4.5 1 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 4.0 3 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 4.0 2 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 4.0 1 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 3.5 3 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 3.5 2 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 3.5 1 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 3.0 3 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 3.0 2 withPed_woCMNCorr
  # . ./plotQA.sh HV70V true false 3.0 1 withPed_woCMNCorr

  # . ./plotQA.sh HV140V true true 4.5 3 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 4.5 2 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 4.5 1 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 4.0 3 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 4.0 2 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 4.0 1 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 3.5 3 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 3.5 2 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 3.5 1 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 3.0 3 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 3.0 2 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true true 3.0 1 withPed_withCMNCorr
  # . ./plotQA.sh HV140V true false 4.5 3 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 4.5 2 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 4.5 1 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 4.0 3 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 4.0 2 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 4.0 1 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 3.5 3 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 3.5 2 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 3.5 1 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 3.0 3 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 3.0 2 withPed_woCMNCorr
  # . ./plotQA.sh HV140V true false 3.0 1 withPed_woCMNCorr

  . ./plotQA.sh HV200V true true 4.5 3 ${nFstThresholdCut} withPed_withCMNCorr
  . ./plotQA.sh HV200V true true 4.5 2 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 4.5 1 ${nFstThresholdCut} withPed_withCMNCorr
  . ./plotQA.sh HV200V true true 4.0 3 ${nFstThresholdCut} withPed_withCMNCorr
  . ./plotQA.sh HV200V true true 4.0 2 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 4.0 1 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 3.5 3 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 3.5 2 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 3.5 1 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 3.0 3 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 3.0 2 ${nFstThresholdCut} withPed_withCMNCorr
  # . ./plotQA.sh HV200V true true 3.0 1 ${nFstThresholdCut} withPed_withCMNCorr
  . ./plotQA.sh HV200V true false 4.5 3 ${nFstThresholdCut} withPed_woCMNCorr
  . ./plotQA.sh HV200V true false 4.5 2 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 4.5 1 ${nFstThresholdCut} withPed_woCMNCorr
  . ./plotQA.sh HV200V true false 4.0 3 ${nFstThresholdCut} withPed_woCMNCorr
  . ./plotQA.sh HV200V true false 4.0 2 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 4.0 1 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 3.5 3 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 3.5 2 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 3.5 1 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 3.0 3 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 3.0 2 ${nFstThresholdCut} withPed_woCMNCorr
  # . ./plotQA.sh HV200V true false 3.0 1 ${nFstThresholdCut} withPed_woCMNCorr
fi
