#!/bin/bash
date

#. ./genPedList.sh HV200V

if [ $# -eq 1 ]
then
  hv=$1
  rm FstPed_${hv}.list
  touch FstPed_${hv}.list
  ls ../../data/FstCosmicTestStand_*/ped/FstOuterTracking_Mod01_FST05_*_${hv}_ped_*.root >> FstPed_${hv}.list
fi
