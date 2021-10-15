#!/bin/sh

Logfilename=`date +%Y.%m.%d-%H.%M.%S`
 

sh FstShutDown.sh 2>&1 | tee "logs/Crate_ShutDown_$Logfilename.log" 

echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "FST went to SHUTDOWN   "
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "--------------------------------------------- "

sleep 5
caput fst_power_status_trans 0

exit 0
