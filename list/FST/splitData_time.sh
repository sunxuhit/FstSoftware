#!/bin/bash
date

#. ./splitData_time.sh HV200V
if [ $# -eq 1 ]
then
  hv=$1
  InPutList=FstData_${hv}.list
  InPutDate=Date${hv}.list

  mkdir -p timesequence

  for item in `cat $InPutDate`
  do
    echo $item
    OutPutList=./timesequence/FstData_${hv}_${item}.list
    rm $OutPutList
    touch $OutPutList
    cat $InPutList | grep $item >> $OutPutList
  done
fi
