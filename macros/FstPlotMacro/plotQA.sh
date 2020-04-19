#!/bin/bash
date

#. ./plotQA.sh

if [ $# -eq 0 ]
then
  hv=HV140V
  config=Th3Tb1

  mkdir -p ./figures/${hv}_${config}

  # echo "Plot Number of Raw Hits!!"
  root -l -b -q plotNumOfRawHits.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/NumOfRawHits.pdf ./figures/${hv}_${config}/NumOfRawHits_${hv}_${config}.pdf

  # echo "Plot Residual of Fst Clusters from 2-Layer tracking!!"
  # root -l -b -q plotResidual_FSTClusters_2Layer.C\(\"${hv}\",\"${config}\"\)
  # mv ./figures/Residual_FSTClusters_2Layer.pdf ./figures/${hv}_${config}/Residual_FSTClusters_2Layer_${hv}_${config}.pdf

  # echo "Plot Residual of Fst Cluster Tracks from 2-Layer tracking!!"
  root -l -b -q plotResidual_FSTClusterTracks_2Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/Residual_FSTClusterTracks_2Layer.pdf ./figures/${hv}_${config}/Residual_FSTClusterTracks_2Layer_${hv}_${config}.pdf

  # echo "Calculate Efficiency of Fst Cluster Tracks from 2-Layer tracking!!"
  root -l -b -q calTrackClusterEfficiency_2Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/Efficiency_TrackCluster_2Layer.pdf ./figures/${hv}_${config}/Efficiency_TrackCluster_2Layer_${hv}_${config}.pdf

  # echo "Plot Event Display from 2-Layer tracking!!"
  root -l -b -q plotEventDisplay_2Layer.C\(\"${hv}\",\"${config}\"\)
  mv ./figures/EventDisplay_2Layer.pdf ./figures/${hv}_${config}/EventDisplay_2Layer_${hv}_${config}.pdf

  # echo "Plot Residual of Fst Clusters from 3-Layer tracking!!"
  # root -l -b -q plotResidual_FSTClusters_3Layer.C\(\"${hv}\",\"${config}\"\)
  # mv ./figures/Residual_FSTClusters_3Layer.pdf ./figures/${hv}_${config}/Residual_FSTClusters_3Layer_${hv}_${config}.pdf

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
