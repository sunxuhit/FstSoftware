#!/bin/bash
date

#. ./fstCluster.sh hv isApplyCMNCorr nFstHitsCut

if [ $# -eq 3 ]
then
  hv=$1
  isSavePed=true
  isApplyCMNCorr=$2
  nFstHitsCut=$3

  root -l -b -q makeFstCluster.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.0\)
  root -l -b -q makeFstCluster.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.0\)
  root -l -b -q makeFstCluster.C\(\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.0\)
fi
