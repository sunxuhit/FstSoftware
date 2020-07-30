#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  mod=Mod03
  isApplyCMNCorr=true
  # isApplyCMNCorr=false

  echo "Time Sequence Study for HV70V"
  InPutDate_HV70V=../../list/FST/Date_${mod}_HV70V.list
  for item in `cat $InPutDate_HV70V`
  do
   # . ./fstCluster.sh ${mod} HV70V $isApplyCMNCorr 4.5 $item 
   . ./fstCluster.sh ${mod} HV70V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh ${mod} HV70V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh ${mod} HV70V $isApplyCMNCorr 3.0 $item 
  done

  # echo "Time Sequence Study for HV100V"
  # InPutDate_HV100V=../../list/FST/Date_${mod}_HV100V.list
  # for item in `cat $InPutDate_HV100V`
  # do
   # . ./fstCluster.sh ${mod} HV100V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh ${mod} HV100V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh ${mod} HV100V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh ${mod} HV100V $isApplyCMNCorr 3.0 $item 
  # done

  # echo "Time Sequence Study for HV120V"
  # InPutDate_HV120V=../../list/FST/Date_${mod}_HV120V.list
  # for item in `cat $InPutDate_HV120V`
  # do
   # . ./fstCluster.sh ${mod} HV120V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh ${mod} HV120V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh ${mod} HV120V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh ${mod} HV120V $isApplyCMNCorr 3.0 $item 
  # done

  # echo "Time Sequence Study for HV140V"
  # InPutDate_HV140V=../../list/FST/Date_${mod}_HV140V.list
  # for item in `cat $InPutDate_HV140V`
  # do
   # . ./fstCluster.sh ${mod} HV140V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh ${mod} HV140V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh ${mod} HV140V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh ${mod} HV140V $isApplyCMNCorr 3.0 $item 
  # done

  # echo "Time Sequence Study for HV200V"
  # InPutDate_HV200V=../../list/FST/Date_${mod}_HV200V.list
  # for item in `cat $InPutDate_HV200V`
  # do
   # . ./fstCluster.sh ${mod} HV200V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh ${mod} HV200V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh ${mod} HV200V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh ${mod} HV200V $isApplyCMNCorr 3.0 $item 
  # done
fi
