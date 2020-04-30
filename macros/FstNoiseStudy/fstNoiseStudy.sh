#!/bin/bash
date

#. ./fstNoiseStudy.sh

if [ $# -eq 0 ]
then
  isSavePed=true
  root -l -b -q doFstNoiseStudy.C\(\"HV70V\"\)
  root -l -b -q doFstNoiseStudy.C\(\"HV140V\"\)
  root -l -b -q doFstNoiseStudy.C\(\"HV200V\"\)
fi
