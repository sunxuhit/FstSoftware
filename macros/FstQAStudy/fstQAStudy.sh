#!/bin/bash
date

#. ./fstQAStudy.sh

if [ $# -eq 0 ]
then
  root -l -b -q doFstQAStudy.C'("HV70V","Th4o5Tb3")'
  root -l -b -q doFstQAStudy.C'("HV70V","Th4o5Tb2")'
  root -l -b -q doFstQAStudy.C'("HV70V","Th4o5Tb1")'
  root -l -b -q doFstQAStudy.C'("HV70V","Th3Tb3")'
  root -l -b -q doFstQAStudy.C'("HV70V","Th3Tb2")'
  root -l -b -q doFstQAStudy.C'("HV70V","Th3Tb1")'

  root -l -b -q doFstQAStudy.C'("HV140V","Th4o5Tb3")'
  root -l -b -q doFstQAStudy.C'("HV140V","Th4o5Tb2")'
  root -l -b -q doFstQAStudy.C'("HV140V","Th4o5Tb1")'
  root -l -b -q doFstQAStudy.C'("HV140V","Th3Tb3")'
  root -l -b -q doFstQAStudy.C'("HV140V","Th3Tb2")'
  root -l -b -q doFstQAStudy.C'("HV140V","Th3Tb1")'

  root -l -b -q doFstQAStudy.C'("HV200V","Th4o5Tb3")'
  root -l -b -q doFstQAStudy.C'("HV200V","Th4o5Tb2")'
  root -l -b -q doFstQAStudy.C'("HV200V","Th4o5Tb1")'
  root -l -b -q doFstQAStudy.C'("HV200V","Th3Tb3")'
  root -l -b -q doFstQAStudy.C'("HV200V","Th3Tb2")'
  root -l -b -q doFstQAStudy.C'("HV200V","Th3Tb1")'
fi
