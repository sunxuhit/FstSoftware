#!/bin/bash
date

#. ./plotQA.sh hv config

if [ $# -eq 7 ]
then
  mod=$1
  hv=$2
  isSavePed=$3
  isApplyCMNCorr=$4
  nFstHitsCut=$5
  numOfUsedTimeBins=$6
  config=$7

  folder=./figures/${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}
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

  # echo "Plot Ped Noise QA!!"
  root -l -b -q plotNoiseQA.C\(\"${mod}\",\"${hv}\",\"Ped\"\)
  mv ./figures/PedNoiseQA.pdf ${folder}/PedNoiseQA_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

  # echo "Plot Data Noise QA!!"
  root -l -b -q plotNoiseQA.C\(\"${mod}\",\"${hv}\",\"Data\"\)
  mv ./figures/DataNoiseQA.pdf ${folder}/DataNoiseQA_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.pdf

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
