#!/bin/bash
date

#. ./genDate.sh Mod03 HV200V

if [ $# -eq 2 ]
then
  mod=$1
  hv=$2
  cut -d '_' -f 5 FstData_${mod}_${hv}.list | sort | uniq > Date_${mod}_${hv}.list
fi
