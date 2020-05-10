#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  # echo "apply CMN correction"
  . ./fstCluster.sh true 4.5
  . ./fstCluster.sh true 4.0
  . ./fstCluster.sh true 3.5
  . ./fstCluster.sh true 3.0

  # echo "not apply CMN correction"
  . ./fstCluster.sh false 4.5
  . ./fstCluster.sh false 4.0
  . ./fstCluster.sh false 3.5
  . ./fstCluster.sh false 3.0
fi
