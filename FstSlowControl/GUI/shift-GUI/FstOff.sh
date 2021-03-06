#!/bin/sh
Rampdelay=40
Logfilename=`date +%Y.%m.%d-%H.%M.%S`
caput fst_power_status_trans 1
sleep 1

# Use Shell Scripts
# sh FstControl.sh 2 130.199.60.175 2>&1 | tee "logs/TestStand_OFF_$Logfilename.log" # BNL Test Stand
# sh FstControl.sh 2 130.199.61.4 2>&1 | tee "logs/TestStand_OFF_$Logfilename.log" # FST Integration test

# Use PVs
caput fstmpod01:ramp_control_switch 2
caput fstmpod02:ramp_control_switch 2
caput fstmpod03:ramp_control_switch 2
echo "Start Ramping Down HV..."
  COUNTER=0
  COUNTER1=$Rampdelay
  while [ $COUNTER -lt $Rampdelay ]; do
  echo "Wait $COUNTER1 seconds....."
  sleep 5
  let COUNTER=COUNTER+5
  let COUNTER1=Rampdelay-COUNTER
  done
echo "--------------------------------------------- "
caput fstmpod01:ramp_control_switch 1
caput fstmpod02:ramp_control_switch 1
caput fstmpod03:ramp_control_switch 1
sleep 10 
caput fstmpod01:pwr_setting 0
caput fstmpod02:pwr_setting 0
caput fstmpod03:pwr_setting 0
sleep 20

echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "FST went to Sleep Bye Bye :)) "
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "--------------------------------------------- "

sleep 5
caput fst_crate_status_ON 0
caput fst_crate_status_ON 0
caput fst_power_status_trans 0

exit 0
