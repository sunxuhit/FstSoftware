#!/bin/sh

Logfilename=`date +%Y.%m.%d-%H.%M.%S`
caput fst_power_status_trans 1
sleep 1
caput fst_crate_status_ON 0
caput fst_crate_status_ON 0

# sh FstControl.sh 2 130.199.60.175 2>&1 | tee "logs/TestStand_OFF_$Logfilename.log" # BNL Test Stand
sh FstControl.sh 2 130.199.61.4 2>&1 | tee "logs/TestStand_OFF_$Logfilename.log" # FST Integration test

echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "FST went to Sleep Bye Bye :)) "
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "--------------------------------------------- "

sleep 5
caput fst_power_status_trans 0

exit 0
