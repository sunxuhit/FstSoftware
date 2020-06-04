#!/bin/bash
date

#. ./fstCluster_timesequence.sh hv isApplyCMNCorr nFstHitsCut date

if [ $# -eq 4 ]
then
  hv=$1
  isSavePed=true
  isApplyCMNCorr=$2
  nFstHitsCut=$3
  date=$4

  root -l -b -q makeFstCluster_timesequence.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.0,\"${date}\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.0,\"${date}\"\)
  # root -l -b -q makeFstCluster_timesequence.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.0,\"${date}\"\)
fi
