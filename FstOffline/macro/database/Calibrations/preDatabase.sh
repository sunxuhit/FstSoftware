#!/bin/bash

inputList=$1

while IFS= read -r runNumber
do
  echo "input run number: $runNumber"
  root4star -b -q mergePedFiles.C\($runNumber\)
done < $inputList
