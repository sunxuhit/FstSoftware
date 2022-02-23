#!/bin/bash

inputList=$1
rdoIdx=6
armIdx=1
apvIdx=3
tbIdx=1

while IFS= read -r runNumber
do
  echo "input run number: $runNumber"
  root4star -b -q checkRms.C\($runNumber,$rdoIdx,$armIdx,$apvIdx,$tbIdx\)
done < $inputList
