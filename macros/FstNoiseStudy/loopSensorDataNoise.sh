#!/bin/bash

#. ./loopSensorDataNoise.sh

if [ $# -eq 0 ]
then
  mod=Mod01

  echo "Time Sequence Study for HV70V"
  InPutDate_HV70V=../../list/FST/data/${mod}/Date_${mod}_HV70V.list
  for item in `cat $InPutDate_HV70V`
  do
   . ./fstSensorDataNoiseStudy.sh ${mod} HV70V $item 
  done

  # echo "Time Sequence Study for HV100V"
  # InPutDate_HV100V=../../list/FST/Date_${mod}_HV100V.list
  # for item in `cat $InPutDate_HV100V`
  # do
  #  . ./fstSensorDataNoiseStudy.sh ${mod} HV100V $item 
  # done

  # echo "Time Sequence Study for HV120V"
  # InPutDate_HV120V=../../list/FST/Date_${mod}_HV120V.list
  # for item in `cat $InPutDate_HV120V`
  # do
  #  . ./fstSensorDataNoiseStudy.sh ${mod} HV120V $item 
  # done

  # echo "Time Sequence Study for HV140V"
  # InPutDate_HV140V=../../list/FST/Date_${mod}_HV140V.list
  # for item in `cat $InPutDate_HV140V`
  # do
  #  . ./fstSensorDataNoiseStudy.sh ${mod} HV140V $item 
  # done
fi
