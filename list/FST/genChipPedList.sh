#!/bin/bash
date

#. ./genChipPedList.sh Mod03 Inner/Outer

if [ $# -eq 2 ]
then
  mod=$1
  sector=$2
  rm FstChipPed_${mod}_${sector}.list
  touch FstChipPed_${mod}_${sector}.list
  ls ../../data/FstCosmicTestStand_*/ped/Fst${sector}Chip*${mod}*_ped_*.root >> FstChipPed_${mod}_${sector}.list
fi
