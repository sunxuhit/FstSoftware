#!/bin/bash
# date

#. ./splitCluster_config.sh HV140V 4.5 3 3.5 withPed_withCMNCorr
if [ $# -eq 5 ]
then
  hv=$1
  nFstHitsCut=$2
  numOfUsedTimeBins=$3
  nFstThresholdCut=$4
  config=$5

  InPutList=FstCluster_${hv}.list
  InPutDate=Date${hv}.list

  mkdir -p configuration

  OutPutList=./configuration/FstCluster_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped${nFstThresholdCut}_${config}.list
  echo "OutPut list is: ${OutPutList}"
  rm $OutPutList
  touch $OutPutList
  for item in `cat $InPutDate`
  do
    # echo $item
    cat $InPutList | grep Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped${nFstThresholdCut}_${config}_${item}
    cat $InPutList | grep Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped${nFstThresholdCut}_${config}_${item} >> $OutPutList
  done
fi
