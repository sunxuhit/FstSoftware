#!/bin/bash
date

#. ./genDate.sh HV200V

if [ $# -eq 1 ]
then
  hv=$1
  cut -d '_' -f 5 FstData_${hv}.list | sort | uniq > Date${hv}.list
fi
