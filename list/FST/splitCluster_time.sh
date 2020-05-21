#!/bin/bash
date

#. ./splitCluster_time.sh HV140V
if [ $# -eq 1 ]
then
  hv=$1
  nFstHitsCut=4.5
  numOfUsedTimeBins=3
  config=withPed_woCMNCorr
  # config=withPed_withCMNCorr

  InPutList=FstCluster_${hv}.list
  InPutDate=Date${hv}.list

  mkdir -p timesequence

  for item in `cat $InPutDate`
  do
    echo $item
    OutPutList=./timesequence/FstCluster_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}_${config}_${item}.list
    rm $OutPutList
    touch $OutPutList
    cat $InPutList | grep Th${nFstHitsCut}Tb${numOfUsedTimeBins}_${config}_${item} >> $OutPutList
  done
fi
