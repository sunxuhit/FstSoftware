#!/bin/bash
date

#. ./genDataList.sh Mod03 HV200V

if [ $# -eq 2 ]
then
  mod=$1
  hv=$2
  rm FstData_${mod}_${hv}.list
  touch FstData_${mod}_${hv}.list
  ls ../../data/FstCosmicTestStand_*/data/Fst*${mod}*_${hv}_data_*.root >> FstData_${mod}_${hv}.list
fi
