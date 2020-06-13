#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  # echo "apply CMN correction"
  . ./fstTracking.sh true 4.5 3.5
  . ./fstTracking.sh true 4.0 3.5
  # . ./fstTracking.sh true 3.5 3.5
  # . ./fstTracking.sh true 3.0 3.5

  . ./fstTracking.sh true 4.5 3.0
  . ./fstTracking.sh true 4.0 3.0
  # . ./fstTracking.sh true 3.5 3.0
  # . ./fstTracking.sh true 3.0 3.0

  . ./fstTracking.sh true 4.5 2.5
  . ./fstTracking.sh true 4.0 2.5
  # . ./fstTracking.sh true 3.5 2.5
  # . ./fstTracking.sh true 3.0 2.5

  # echo "not apply CMN correction"
  . ./fstTracking.sh false 4.5 3.5
  . ./fstTracking.sh false 4.0 3.5
  # . ./fstTracking.sh false 3.5 3.5
  # . ./fstTracking.sh false 3.0 3.5

  . ./fstTracking.sh false 4.5 3.0
  . ./fstTracking.sh false 4.0 3.0
  # . ./fstTracking.sh false 3.5 3.0
  # . ./fstTracking.sh false 3.0 3.0

  . ./fstTracking.sh false 4.5 2.5
  . ./fstTracking.sh false 4.0 2.5
  # . ./fstTracking.sh false 3.5 2.5
  # . ./fstTracking.sh false 3.0 2.5
fi
