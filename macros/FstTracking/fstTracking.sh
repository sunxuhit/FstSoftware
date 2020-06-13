#!/bin/bash
date

#. ./fstTracking.sh isApplyCMNCorr nFstHitsCut nFstThresholdCut

if [ $# -eq 3 ]
then
  isSavePed=true
  isApplyCMNCorr=$1
  nFstHitsCut=$2
  nFstThresholdCut=$3

  # HV70V
  # root -l -b -q doFstTracking.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,${nFstThresholdCut}\)
  # root -l -b -q doFstTracking.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,${nFstThresholdCut}\)
  # root -l -b -q doFstTracking.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,${nFstThresholdCut}\)

  # HV140V
  # root -l -b -q doFstTracking.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,${nFstThresholdCut}\)
  # root -l -b -q doFstTracking.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,${nFstThresholdCut}\)
  # root -l -b -q doFstTracking.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,${nFstThresholdCut}\)

  # HV200V
  root -l -b -q doFstTracking.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,${nFstThresholdCut}\)
  root -l -b -q doFstTracking.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,${nFstThresholdCut}\)
  # root -l -b -q doFstTracking.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,${nFstThresholdCut}\)
fi
