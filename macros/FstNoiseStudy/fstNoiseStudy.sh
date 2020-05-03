#!/bin/bash
date

#. ./fstNoiseStudy.sh

if [ $# -eq 0 ]
then
  isSavePed=true
  root -l -b -q doFstNoiseStudy.C\(\"HV70V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"HV70V\",\"Data\"\)

  root -l -b -q doFstNoiseStudy.C\(\"HV140V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"HV140V\",\"Data\"\)

  root -l -b -q doFstNoiseStudy.C\(\"HV200V\",\"Ped\"\)
  root -l -b -q doFstNoiseStudy.C\(\"HV200V\",\"Data\"\)
fi
