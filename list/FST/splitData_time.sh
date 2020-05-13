#!/bin/bash
date

#. ./splitData_time.sh
if [ $# -eq 1 ]
then
  hv=$1
  InPutList=FstData_${hv}.list
  InPutDate=Date${hv}.list


  for item in `cat $InPutDate`
  do
    echo $item
    OutPutList=./timesequence/FstData_${hv}_${item}.list
    rm $OutPutList
    touch $OutPutList
    cat $InPutList | grep $item >> $OutPutList
  done
fi
