#!/bin/sh
Turnondelay=10
Logfilename=`date +%Y.%m.%d-%H.%M.%S`
caput fst_power_status_trans 1
sleep 1
caput fst_crate_status_ON 1
caput fst_crate_status_ON 1

# Use Shell Scripts
# sh FstControl.sh 1 130.199.60.175 2>&1 | tee "logs/TestStand_ON_$Logfilename.log" # BNL Test Stand
sh FstControl.sh 1 130.199.61.4 2>&1 | tee "logs/TestStand_ON_$Logfilename.log"  # FST SC Test

# Use PVs
# caput fstmpod02:pwr_setting 1
# sleep 20
# caput fstmpod02:master_control_switch 3
# sleep 10 
# caput fstmpod02:ramp_control_switch 1
# sleep 10
# caput fstmpod02:ramp_control_switch 2
# sleep 10
# caput fstmpod02:ramp_control_switch 3
# sleep 40

echo "WARNING Please wait to stablize the HV outputs "
  COUNTER=0
  COUNTER1=$Turnondelay
  while [ $COUNTER -lt $Turnondelay ]; do
  echo "Wait $COUNTER1 seconds....."
  sleep 1
  let COUNTER=COUNTER+1
  let COUNTER1=Turnondelay-COUNTER
  done
echo "--------------------------------------------- "
echo "--------------------------------------------- "
echo "FST is AWAKE, let's do some BigBang "
echo "--------------------------------------------- "
echo "--------------------------------------------- "


# caput istmpod01:crate_status_panic 0
caput fstmpod02:crate_status_panic 0
# caput istmpod03:crate_status_panic 0
caput fst_crate_status_panic 0
 
sleep 5
caput fst_power_status_trans 0
exit 0
