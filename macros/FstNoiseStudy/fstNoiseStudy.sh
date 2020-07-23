#!/bin/bash
date

#. ./fstNoiseStudy.sh

if [ $# -eq 0 ]
then
  module=Mod03
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV0V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV5V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV10V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV20V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV30V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV40V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV50V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV60V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV70V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV80V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV90V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV100V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV110V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV120V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV130V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"${module}\",\"HV140V\",\"Ped\"\)
fi
