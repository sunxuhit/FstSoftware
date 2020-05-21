#!/bin/bash
date

#. ./genDataList.sh HV200V

if [ $# -eq 1 ]
then
  hv=$1
  rm FstData_${hv}.list
  touch FstData_${hv}.list
  ls ../../data/FstCosmicTestStand_*/data/FstOuterTracking_Mod01_FST05_*_${hv}_data_*.root >> FstData_${hv}.list
fi
