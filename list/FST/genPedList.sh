#!/bin/bash
date

#. ./genPedList.sh Mod03 HV200V

if [ $# -eq 2 ]
then
  mod=$1
  hv=$2
  rm FstPed_${mod}_${hv}.list
  touch FstPed_${mod}_${hv}.list
  ls ../../data/FstCosmicTestStand_*/ped/FstInnerTracking*${mod}*_${hv}_ped_*.root >> FstPed_${mod}_${hv}.list
fi
