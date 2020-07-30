#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  mod=Mod03
  # echo "apply CMN correction"
  . ./fstTracking.sh ${mod} true 4.0

  # echo "not apply CMN correction"
  # . ./fstTracking.sh ${mod} false 4.0
fi
