#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  isApplyCMNCorr=true
  # isApplyCMNCorr=false

  # echo "Time Sequence Study for HV70V"
  # InPutDate_HV70V=../../list/FST/DateHV70V.list
  # for item in `cat $InPutDate_HV70V`
  # do
   # . ./fstCluster.sh HV70V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh HV70V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh HV70V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh HV70V $isApplyCMNCorr 3.0 $item 
  # done

  echo "Time Sequence Study for HV100V"
  InPutDate_HV100V=../../list/FST/DateHV100V.list
  for item in `cat $InPutDate_HV100V`
  do
   # . ./fstCluster.sh HV100V $isApplyCMNCorr 4.5 $item 
   . ./fstCluster.sh HV100V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh HV100V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh HV100V $isApplyCMNCorr 3.0 $item 
  done

  # echo "Time Sequence Study for HV140V"
  # InPutDate_HV140V=../../list/FST/DateHV140V.list
  # for item in `cat $InPutDate_HV140V`
  # do
   # . ./fstCluster.sh HV140V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh HV140V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh HV140V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh HV140V $isApplyCMNCorr 3.0 $item 
  # done

  # echo "Time Sequence Study for HV200V"
  # InPutDate_HV200V=../../list/FST/DateHV200V.list
  # for item in `cat $InPutDate_HV200V`
  # do
   # . ./fstCluster.sh HV200V $isApplyCMNCorr 4.5 $item 
   # . ./fstCluster.sh HV200V $isApplyCMNCorr 4.0 $item 
   # . ./fstCluster.sh HV200V $isApplyCMNCorr 3.5 $item 
   # . ./fstCluster.sh HV200V $isApplyCMNCorr 3.0 $item 
  # done
fi
