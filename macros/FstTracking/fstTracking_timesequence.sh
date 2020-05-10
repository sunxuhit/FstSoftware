#!/bin/bash
date

#. ./fstTracking_timesequence.sh

if [ $# -eq 0 ]
then
  isSavePed=true
  isApplyCMNCorr=true

  root -l -b -q doFstTracking_timesequence.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"03182020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04062020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04102020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04112020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04142020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV70V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04162020\"\)

  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"03202020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"03292020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"03302020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"03312020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04022020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04032020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04042020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04062020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04072020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04082020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04132020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04172020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04232020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04272020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04292020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"05022020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV140V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"05042020\"\)

  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04022020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04052020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04092020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04122020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04182020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04192020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04232020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04242020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04252020\"\)
  root -l -b -q doFstTracking_timesequence.C\(\"HV200V\",${isSavePed},${isApplyCMNCorr},4.5,3,\"04262020\"\)
fi
