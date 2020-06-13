#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  isApplyCMNCorr=true
  # isApplyCMNCorr=false

  # echo "Time Sequence Study for HV140V"
  # InPutDate_HV140V=../../list/FST/DateHV140V.list
  # for item in `cat $InPutDate_HV140V`
  # do
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.5 3.5 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.0 3.5 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.5 3.5 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.0 3.5 $item 
  #
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.5 3.0 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.0 3.0 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.5 3.0 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.0 3.0 $item 
  #
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.5 2.5 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.0 2.5 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.5 2.5 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.0 2.5 $item 
  #
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.5 2.0 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 4.0 2.0 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.5 2.0 $item 
  #  . ./fstCluster_timesequence.sh HV140V $isApplyCMNCorr 3.0 2.0 $item 
  # done

  echo "Time Sequence Study for HV200V"
  InPutDate_HV200V=../../list/FST/DateHV200V.list
  for item in `cat $InPutDate_HV200V`
  do
   . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.5 3.5 $item 
   . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.0 3.5 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.5 3.5 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.0 3.5 $item 

   . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.5 3.0 $item 
   . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.0 3.0 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.5 3.0 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.0 3.0 $item 

   . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.5 2.5 $item 
   . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.0 2.5 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.5 2.5 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.0 2.5 $item 

   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.5 2.0 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 4.0 2.0 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.5 2.0 $item 
   # . ./fstCluster_timesequence.sh HV200V $isApplyCMNCorr 3.0 2.0 $item 
  done
fi
