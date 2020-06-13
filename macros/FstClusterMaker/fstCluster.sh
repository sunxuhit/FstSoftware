#!/bin/bash
date

#. ./fstCluster.sh hv isApplyCMNCorr nFstHitsCut nFstThresholdCut

if [ $# -eq 4 ]
then
  hv=$1
  isSavePed=true
  isApplyCMNCorr=$2
  nFstHitsCut=$3
  nFstThresholdCut=$4

  root -l -b -q makeFstCluster.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,${nFstThresholdCut}\)
  root -l -b -q makeFstCluster.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,${nFstThresholdCut}\)
  # root -l -b -q makeFstCluster.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,${nFstThresholdCut}\)
fi
