#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  # echo "apply CMN correction"
  . ./fstQAStudy.sh true 4.5
  . ./fstQAStudy.sh true 4.0
  # . ./fstQAStudy.sh true 3.5
  # . ./fstQAStudy.sh true 3.0

  # echo "not apply CMN correction"
  . ./fstQAStudy.sh false 4.5
  . ./fstQAStudy.sh false 4.0
  # . ./fstQAStudy.sh false 3.5
  # . ./fstQAStudy.sh false 3.0
fi
