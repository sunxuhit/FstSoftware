#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  # echo "apply CMN correction"
  . ./fstQAStudy.sh true 4.5 3.5
  . ./fstQAStudy.sh true 4.0 3.5
  # . ./fstQAStudy.sh true 3.5 3.5
  # . ./fstQAStudy.sh true 3.0 3.5

  . ./fstQAStudy.sh true 4.5 3.0
  . ./fstQAStudy.sh true 4.0 3.0
  # . ./fstQAStudy.sh true 3.5 3.0
  # . ./fstQAStudy.sh true 3.0 3.0

  . ./fstQAStudy.sh true 4.5 2.5
  . ./fstQAStudy.sh true 4.0 2.5
  # . ./fstQAStudy.sh true 3.5 2.5
  # . ./fstQAStudy.sh true 3.0 2.5

  # echo "not apply CMN correction"
  . ./fstQAStudy.sh false 4.5 3.5
  . ./fstQAStudy.sh false 4.0 3.5
  # . ./fstQAStudy.sh false 3.5 3.5
  # . ./fstQAStudy.sh false 3.0 3.5

  . ./fstQAStudy.sh false 4.5 3.0
  . ./fstQAStudy.sh false 4.0 3.0
  # . ./fstQAStudy.sh false 3.5 3.0
  # . ./fstQAStudy.sh false 3.0 3.0

  . ./fstQAStudy.sh false 4.5 2.5
  . ./fstQAStudy.sh false 4.0 2.5
  # . ./fstQAStudy.sh false 3.5 2.5
  # . ./fstQAStudy.sh false 3.0 2.5
fi
