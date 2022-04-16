#!/bin/bash

inputList=$1
rdoIdx=5
armIdx=0
apvIdx=7
tbIdx=1

while IFS= read -r runNumber
do
  echo "input run number: $runNumber"
  root4star -b -q checkRms.C\($runNumber,$rdoIdx,$armIdx,$apvIdx,$tbIdx\)
done < $inputList
