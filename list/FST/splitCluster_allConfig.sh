#!/bin/bash
date

#. ./splitCluster_allConfig.sh hv nFstThresholdCut

if [ $# -eq 2 ]
then
  hv=$1
  nFstThresholdCut=$2

  echo "Split Cluster list for ${hv}"  
   . ./splitCluster_config.sh ${hv} 4.5 3 ${nFstThresholdCut} withPed_withCMNCorr
   . ./splitCluster_config.sh ${hv} 4.5 2 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 4.5 1 ${nFstThresholdCut} withPed_withCMNCorr
   . ./splitCluster_config.sh ${hv} 4.0 3 ${nFstThresholdCut} withPed_withCMNCorr
   . ./splitCluster_config.sh ${hv} 4.0 2 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 4.0 1 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.5 3 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.5 2 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.5 1 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.0 3 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.0 2 ${nFstThresholdCut} withPed_withCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.0 1 ${nFstThresholdCut} withPed_withCMNCorr

   . ./splitCluster_config.sh ${hv} 4.5 3 ${nFstThresholdCut} withPed_woCMNCorr
   . ./splitCluster_config.sh ${hv} 4.5 2 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 4.5 1 ${nFstThresholdCut} withPed_woCMNCorr
   . ./splitCluster_config.sh ${hv} 4.0 3 ${nFstThresholdCut} withPed_woCMNCorr
   . ./splitCluster_config.sh ${hv} 4.0 2 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 4.0 1 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.5 3 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.5 2 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.5 1 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.0 3 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.0 2 ${nFstThresholdCut} withPed_woCMNCorr
   # . ./splitCluster_config.sh ${hv} 3.0 1 ${nFstThresholdCut} withPed_woCMNCorr
fi
