#!/bin/bash
date

#. ./fstCluster.sh hv isApplyCMNCorr nFstHitsCut nFstThresholdCut date

if [ $# -eq 5 ]
then
  hv=$1
  isSavePed=true
  isApplyCMNCorr=$2
  nFstHitsCut=$3
  nFstThresholdCut=$4
  date=$5

  root -l -b -q makeFstCluster_timesequence.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,${nFstThresholdCut},\"${date}\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,${nFstThresholdCut},\"${date}\"\)
  # root -l -b -q makeFstCluster_timesequence.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,${nFstThresholdCut},\"${date}\"\)
fi
