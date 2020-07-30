#!/bin/bash
date

#. ./fstQAStudy.sh Mod03 isApplyCMNCorr nFstHitsCut

if [ $# -eq 3 ]
then
  mod=$1
  isSavePed=true
  isApplyCMNCorr=$2
  nFstHitsCut=$3

  # HV70V
  root -l -b -q doFstQAStudy.C\(\"${mod}\",\"HV70V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)

  # HV100V
  # root -l -b -q doFstQAStudy.C\(\"${mod}\",\"HV100V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)

  # HV120V
  # root -l -b -q doFstQAStudy.C\(\"${mod}\",\"HV120V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)

  # HV140V
  # root -l -b -q doFstQAStudy.C\(\"${mod}\",\"HV140V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)

  # HV200V
  # root -l -b -q doFstQAStudy.C\(\"${mod}\",\"HV200V\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},2,2.5,3.5\)
fi
