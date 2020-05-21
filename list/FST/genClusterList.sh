#!/bin/bash
date

#. ./genClusterList.sh HV200V

if [ $# -eq 1 ]
then
  hv=$1
  rm FstCluster_${hv}.list
  touch FstCluster_${hv}.list
  ls ../../output/clusters/FstClusters_${hv}_*.root >> FstCluster_${hv}.list
fi
