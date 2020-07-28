#!/bin/bash
date

#. ./fstTracking.sh isApplyCMNCorr nFstHitsCut

if [ $# -eq 2 ]
then
  isSavePed=true
  isApplyCMNCorr=$1
  nFstHitsCut=$2

  # HV70V
  # root -l -b -q doFstTracking.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.5,3.5\)

  # HV100V
  # root -l -b -q doFstTracking.C\(\"HV100V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV100V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV100V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.5,3.5\)

  # HV120V
  # root -l -b -q doFstTracking.C\(\"HV120V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV120V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV120V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.5,3.5\)

  # HV140V
  # root -l -b -q doFstTracking.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.5,3.5\)
  root -l -b -q doFstTracking.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.5,3.5\)

  # HV200V
  # root -l -b -q doFstTracking.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)
  # root -l -b -q doFstTracking.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.5,3.5\)
fi
