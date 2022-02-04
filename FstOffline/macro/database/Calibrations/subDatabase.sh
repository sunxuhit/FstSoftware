#!/bin/bash

inputList=$1
date="02032022"

outputList="/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database/inDatabase_${date}.list"
databaseDir="/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/database"

while IFS= read -r runNumber
do
  echo "input run number: $runNumber"
  root4star -b -q write_fst_pedNoise.C\($runNumber\)
  head -n 1 $databaseDir/runInfo_$runNumber.txt >> $outputList
done < $inputList
