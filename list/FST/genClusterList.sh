#!/bin/bash
date

#. ./genClusterList.sh Mod03 HV200V

if [ $# -eq 2 ]
then
  mod=$1
  hv=$2
  rm FstCluster_${mod}_${hv}.list
  touch FstCluster_${mod}_${hv}.list
  ls ../../output/clusters/FstClusters_${mod}_${hv}_*.root >> FstCluster_${mod}_${hv}.list
fi
