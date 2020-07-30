#!/bin/bash
# date

#. ./splitCluster_config.sh Mod03 HV70V 4.0 2 withPed_withCMNCorr
if [ $# -eq 5 ]
then
  mod=$1
  hv=$2
  nFstHitsCut=$3
  numOfUsedTimeBins=$4
  config=$5

  InPutList=FstCluster_${mod}_${hv}.list
  InPutDate=Date_${mod}_${hv}.list

  mkdir -p configuration

  OutPutList=./configuration/FstCluster_${mod}_${hv}_Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}.list
  echo "OutPut list is: ${OutPutList}"
  rm $OutPutList
  touch $OutPutList
  for item in `cat $InPutDate`
  do
    # echo $item
    cat $InPutList | grep Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}_${item}
    cat $InPutList | grep Th${nFstHitsCut}Tb${numOfUsedTimeBins}Ped2.5Ped3.5_${config}_${item} >> $OutPutList
  done
fi
