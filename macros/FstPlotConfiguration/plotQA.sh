#!/bin/bash
date

#. ./plotQA.sh hv config

if [ $# -eq 2 ]
then
  hv=$1
  config=$2

  mkdir -p ./figures/${hv}_${config}

  # echo "Plot Number of Raw Hits!!"
  root -l -b -q plotNumOfRawHits.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/NumOfRawHits.pdf ./figures/${hv}_${config}/NumOfRawHits_${hv}_${config}.pdf

  # echo "Plot Signal QA!!"
  root -l -b -q plotSignalQA.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/SignalQA.pdf ./figures/${hv}_${config}/SignalQA_${hv}_${config}.pdf

  # echo "Plot Ped Noise QA!!"
  root -l -b -q plotNoiseQA.C\(\"${hv}\",\"Ped\"\)
  mv ./figures/PedNoiseQA.pdf ./figures/${hv}_${config}/PedNoiseQA_${hv}_${config}.pdf

  # echo "Plot Data Noise QA!!"
  root -l -b -q plotNoiseQA.C\(\"${hv}\",\"Data\"\)
  mv ./figures/DataNoiseQA.pdf ./figures/${hv}_${config}/DataNoiseQA_${hv}_${config}.pdf

  # echo "Plot Residual of Fst Cluster Tracks from 2-Layer tracking!!"
  root -l -b -q plotResidual_FSTClusterTracks_2Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/Residual_FSTClusterTracks_2Layer.pdf ./figures/${hv}_${config}/Residual_FSTClusterTracks_2Layer_${hv}_${config}.pdf

  # echo "Calculate Efficiency of Fst Cluster Tracks from 2-Layer tracking!!"
  root -l -b -q calTrackClusterEfficiency_2Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/Efficiency_TrackCluster_2Layer.pdf ./figures/${hv}_${config}/Efficiency_TrackCluster_2Layer_${hv}_${config}.pdf

  # echo "Plot Event Display from 2-Layer tracking!!"
  root -l -b -q plotEventDisplay_2Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/EventDisplay_2Layer.pdf ./figures/${hv}_${config}/EventDisplay_2Layer_${hv}_${config}.pdf

  # echo "Plot Residual of Fst Cluster Tracks from 3-Layer tracking!!"
  root -l -b -q plotResidual_FSTClusterTracks_3Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/Residual_FSTClusterTracks_3Layer.pdf ./figures/${hv}_${config}/Residual_FSTClusterTracks_3Layer_${hv}_${config}.pdf

  # echo "Calculate Efficiency of Fst Cluster Tracks from 3-Layer tracking!!"
  root -l -b -q calTrackClusterEfficiency_3Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/Efficiency_TrackCluster_3Layer.pdf ./figures/${hv}_${config}/Efficiency_TrackCluster_3Layer_${hv}_${config}.pdf

  # echo "Plot Event Display from 3-Layer tracking!!"
  root -l -b -q plotEventDisplay_3Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/EventDisplay_3Layer.pdf ./figures/${hv}_${config}/EventDisplay_3Layer_${hv}_${config}.pdf
fi
