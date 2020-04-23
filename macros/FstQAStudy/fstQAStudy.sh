#!/bin/bash
date

#. ./fstQAStudy.sh

if [ $# -eq 0 ]
then
  isSavePed=true
  root -l -b -q doFstQAStudy.C\(\"HV70V\",\"Th4o5Tb3\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV70V\",\"Th4o5Tb2\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV70V\",\"Th4o5Tb1\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV70V\",\"Th3Tb3\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV70V\",\"Th3Tb2\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV70V\",\"Th3Tb1\",${isSavePed}\)

  root -l -b -q doFstQAStudy.C\(\"HV140V\",\"Th4o5Tb3\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV140V\",\"Th4o5Tb2\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV140V\",\"Th4o5Tb1\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV140V\",\"Th3Tb3\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV140V\",\"Th3Tb2\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV140V\",\"Th3Tb1\",${isSavePed}\)

  root -l -b -q doFstQAStudy.C\(\"HV200V\",\"Th4o5Tb3\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV200V\",\"Th4o5Tb2\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV200V\",\"Th4o5Tb1\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV200V\",\"Th3Tb3\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV200V\",\"Th3Tb2\",${isSavePed}\)
  root -l -b -q doFstQAStudy.C\(\"HV200V\",\"Th3Tb1\",${isSavePed}\)
fi
