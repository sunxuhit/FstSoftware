#!/bin/bash

PedDirectory="/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/pedestal"

cd $PedDirectory
rm pedRun_run22.list
rm pedRunNumber_run22.list

ls fst*pedestals_2*GOOD.txt > pedRun_run22.list

awk -F_ '{print $(NF-1)}' pedRun_run22.list | sort | uniq > pedRunNumber_run22.list

rm pedRun_run22.list

cd -
