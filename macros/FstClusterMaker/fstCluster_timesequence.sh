#!/bin/bash
date

#. ./fstCluster_timesequence.sh

if [ $# -eq 0 ]
then
  config=Th4o5Tb3
  isSavePed=true
  isApplyCMNCorr=true

  root -l -b -q makeFstCluster_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"03182020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04062020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04102020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04112020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04142020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04162020\"\)

  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"03202020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"03292020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"03302020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"03312020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04022020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04032020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04042020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04062020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04072020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04082020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04132020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04172020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04232020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04272020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04292020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"05022020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"05042020\"\)

  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04022020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04052020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04092020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04122020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04182020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04192020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04232020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04242020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04252020\"\)
  root -l -b -q makeFstCluster_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},${isApplyCMNCorr},4.5,3,2.0,\"04262020\"\)
fi
