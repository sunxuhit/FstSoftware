#!/bin/bash

#. ./loopAll.sh

if [ $# -eq 0 ]
then
  echo "config is Th4o5Tb3"
  . ./plotQA.sh HV70V Th4o5Tb3
  . ./plotQA.sh HV70V Th4o5Tb2
  . ./plotQA.sh HV70V Th4o5Tb1

  . ./plotQA.sh HV140V Th4o5Tb3
  . ./plotQA.sh HV140V Th4o5Tb2
  . ./plotQA.sh HV140V Th4o5Tb1

  . ./plotQA.sh HV200V Th4o5Tb3
  . ./plotQA.sh HV200V Th4o5Tb2
  . ./plotQA.sh HV200V Th4o5Tb1

  echo "config is Th3Tb3"
  . ./plotQA.sh HV70V Th3Tb3
  . ./plotQA.sh HV70V Th3Tb2
  . ./plotQA.sh HV70V Th3Tb1

  . ./plotQA.sh HV140V Th3Tb3
  . ./plotQA.sh HV140V Th3Tb2
  . ./plotQA.sh HV140V Th3Tb1

  . ./plotQA.sh HV200V Th3Tb3
  . ./plotQA.sh HV200V Th3Tb2
  . ./plotQA.sh HV200V Th3Tb1
fi
