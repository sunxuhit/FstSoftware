#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  # echo "apply CMN correction"
  . ./fstTracking.sh true 4.5
  . ./fstTracking.sh true 4.0
  # . ./fstTracking.sh true 3.5
  # . ./fstTracking.sh true 3.0

  # echo "not apply CMN correction"
  . ./fstTracking.sh false 4.5
  . ./fstTracking.sh false 4.0
  # . ./fstTracking.sh false 3.5
  # . ./fstTracking.sh false 3.0
fi
