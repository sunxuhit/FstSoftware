#!/bin/bash
date

#. ./fstCluster.sh isApplyCMNCorr nFstHitsCut

if [ $# -eq 2 ]
then
  isSavePed=true
  isApplyCMNCorr=$1
  nFstHitsCut=$2

  # HV70V
  # root -l -b -q makeFstCluster.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.0\)
  # root -l -b -q makeFstCluster.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.0\)
  # root -l -b -q makeFstCluster.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.0\)

  # HV140V
  root -l -b -q makeFstCluster.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.0\)

  # HV200V
  root -l -b -q makeFstCluster.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},1,2.0\)
fi
