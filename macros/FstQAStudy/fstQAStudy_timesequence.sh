#!/bin/bash
date

#. ./fstQAStudy_timesequence.sh

if [ $# -eq 0 ]
then
  config=Th4o5Tb3
  isSavePed=true

  root -l -b -q doFstQAStudy_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},\"03182020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},\"04062020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},\"04102020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},\"04112020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},\"04142020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV70V\",\"${config}\",${isSavePed},\"04162020\"\)

  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"03202020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"03292020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"03302020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"03312020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04022020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04032020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04042020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04062020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04072020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04082020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04132020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04172020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV140V\",\"${config}\",${isSavePed},\"04232020\"\)

  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04022020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04052020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04092020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04122020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04182020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04192020\"\)
  root -l -b -q doFstQAStudy_timesequence.C\(\"HV200V\",\"${config}\",${isSavePed},\"04232020\"\)
fi
