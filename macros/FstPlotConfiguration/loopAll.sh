#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  mod=Mod03

  . ./plotQA.sh ${mod}  HV70V true true 4.0 2 withPed_withCMNCorr
  # . ./plotQA.sh ${mod} HV100V true true 4.0 2 withPed_withCMNCorr
  # . ./plotQA.sh ${mod} HV120V true true 4.0 2 withPed_withCMNCorr
  # . ./plotQA.sh ${mod} HV140V true true 4.0 2 withPed_withCMNCorr
fi
