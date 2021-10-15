#!/bin/bash
date

#. ./plotQA.sh Mod01 HV70V

if [ $# -eq 2 ]
then
  mod=$1
  hv=$2
  isSavePed=true
  isApplyCMNCorr=true
  nFstHitsCut=4.0
  numOfUsedTimeBins=2
  config=withPed_withCMNCorr

  folder=./figures/${mod}/${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}
  mkdir -p ${folder} 

  # echo "Plot Number of Raw Hits!!"
  root -l -b -q plotNumOfRawHits.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5\)
  mv ./figures/NumOfRawHits.pdf ${folder}/NumOfRawHits_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  # echo "Plot Signal QA!!"
  root -l -b -q plotSignalQA.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5,0\)
  mv ./figures/SignalQA_Sensor0.pdf ${folder}/SignalQA_Sensor0_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  root -l -b -q plotSignalQA.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5,1\)
  mv ./figures/SignalQA_Sensor1.pdf ${folder}/SignalQA_Sensor1_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  root -l -b -q plotSignalQA.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5,2\)
  mv ./figures/SignalQA_Sensor2.pdf ${folder}/SignalQA_Sensor2_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  # echo "Plot Residual of Fst Cluster Tracks from 2-Layer tracking!!"
  root -l -b -q plotResidual_FSTClusterTracks_2Layer.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5\)
  mv ./figures/Residual_FSTClusterTracks_2Layer.pdf ${folder}/Residual_FSTClusterTracks_2Layer_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  # echo "Calculate Efficiency of Fst Cluster Tracks from 2-Layer tracking!!"
  root -l -b -q calTrackClusterEfficiency_2Layer.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5\)
  mv ./figures/Efficiency_TrackCluster_2Layer.pdf ${folder}/Efficiency_TrackCluster_2Layer_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  # echo "Plot Event Display from 2-Layer tracking!!"
  root -l -b -q plotEventDisplay_2Layer.C\(\"${mod}\",\"${hv}\",${isSavePed},${isApplyCMNCorr},${nFstHitsCut},${numOfUsedTimeBins},2.5,3.5\)
  mv ./figures/EventDisplay_2Layer.pdf ${folder}/EventDisplay_2Layer_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf
fi
