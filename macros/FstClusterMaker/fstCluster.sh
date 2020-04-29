#!/bin/bash
date

#. ./fstCluster.sh

if [ $# -eq 0 ]
then
  config=Th4o5Tb3
  isSavePed=true

  root -l -b -q makeFstCluster.C\(\"HV70V\",\"${config}\",${isSavePed}\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"${config}\",${isSavePed}\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"${config}\",${isSavePed}\)
fi
