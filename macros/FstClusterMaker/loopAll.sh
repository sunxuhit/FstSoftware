#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  # isApplyCMNCorr=true
  isApplyCMNCorr=false

  echo "all statistics for HV140V"
  # . ./fstCluster.sh HV140V ${isApplyCMNCorr} 4.5 3.0
  # . ./fstCluster.sh HV140V ${isApplyCMNCorr} 4.0 3.0
  # . ./fstCluster.sh HV140V ${isApplyCMNCorr} 3.5 3.0
  # . ./fstCluster.sh HV140V ${isApplyCMNCorr} 3.0 3.0

  echo "all statistics for HV200V"
  . ./fstCluster.sh HV200V ${isApplyCMNCorr} 4.5 3.0
  # . ./fstCluster.sh HV200V ${isApplyCMNCorr} 4.0 3.0
  # . ./fstCluster.sh HV200V ${isApplyCMNCorr} 3.5 3.0
  # . ./fstCluster.sh HV200V ${isApplyCMNCorr} 3.0 3.0
fi
