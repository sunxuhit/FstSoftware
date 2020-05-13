#!/bin/bash
date

#. ./splitData_time.sh HV140V
if [ $# -eq 1 ]
then
  hv=$1
  nFstHitsCut=4.5
  numOfUsedTimeBins=3
  config=withPed_woCMNCorr

  InPutList=FstCluster_${hv}.list
  InPutDate=Date${hv}.list


  for item in `cat $InPutDate`
  do
    echo $item
    OutPutList=./timesequence/FstCluster_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}_${config}_${item}.list
    rm $OutPutList
    touch $OutPutList
    cat $InPutList | grep Th${nFstHitsCut}Tb${numOfUsedTimeBins}_${config}_${item} >> $OutPutList
  done
fi
