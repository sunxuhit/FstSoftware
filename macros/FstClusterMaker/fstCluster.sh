#!/bin/bash
date

#. ./fstCluster.sh

if [ $# -eq 0 ]
then
  # HV70V
  root -l -b -q makeFstCluster.C\(\"HV70V\",\"Th4o5Tb3\",true,4.5,3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV70V\",\"Th4o5Tb2\",true,4.5,2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV70V\",\"Th4o5Tb1\",true,4.5,1,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV70V\",\"Th3Tb3\",true,3.0,3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV70V\",\"Th3Tb2\",true,3.0,2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV70V\",\"Th3Tb1\",true,3.0,1,2.0\)

  # HV140V
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"Th4o5Tb3\",true,4.5,3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"Th4o5Tb2\",true,4.5,2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"Th4o5Tb1\",true,4.5,1,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"Th3Tb3\",true,3.0,3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"Th3Tb2\",true,3.0,2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV140V\",\"Th3Tb1\",true,3.0,1,2.0\)

  # HV200V
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"Th4o5Tb3\",true,4.5,3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"Th4o5Tb2\",true,4.5,2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"Th4o5Tb1\",true,4.5,1,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"Th3Tb3\",true,3.0,3,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"Th3Tb2\",true,3.0,2,2.0\)
  root -l -b -q makeFstCluster.C\(\"HV200V\",\"Th3Tb1\",true,3.0,1,2.0\)
fi
