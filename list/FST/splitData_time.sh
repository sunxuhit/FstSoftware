#!/bin/bash
date

#. ./splitData_time.sh Mod03 HV200V
if [ $# -eq 2 ]
then
  mod=$1
  hv=$2
  InPutList=FstData_${mod}_${hv}.list
  InPutDate=Date_${mod}_${hv}.list

  mkdir -p timesequence

  for item in `cat $InPutDate`
  do
    echo $item
    OutPutList=./timesequence/FstData_${mod}_${hv}_${item}.list
    rm $OutPutList
    touch $OutPutList
    cat $InPutList | grep $item >> $OutPutList
  done
fi
