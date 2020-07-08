#!/bin/bash
date

#. ./genPedList.sh HV200V

if [ $# -eq 1 ]
then
  hv=$1
  rm FstPed_${hv}.list
  touch FstPed_${hv}.list
  ls ../../data/FstCosmicTestStand_*/ped/Fst*_${hv}_ped_*.root >> FstPed_${hv}.list
fi
