#!/bin/bash
#
# Wiener Caret ON V.1.65
# Babak Abi
# Updated by Xu Sun for FST
E_BADARGS=85

#######  settings
Turnoffdelay=10
Turnondelay=10
TRUNONRetery="no"
Wlines=0
Nretry=5
####### crate  settings
#ip=130.199.60.175 # BNL stand-alone test stand
#ip1=130.199.61.3 # FSTMPOD01
#ip2=130.199.61.4 # FSTMPOD02
#ip3=130.199.61.5 # FSTMPOD03
ip=$2

path=/usr/share/snmp/mibs/
channelCount=0
# NumChennels=24 # for FST final setup
NumChennels=16 # for FST SC test
declare -a x 
# x1=(u0 u1 u2 u3 u4 u5 u6 u7 u100 u101 u102 u103 u104 u105 u106 u107 u200 u201 u202 u203 u204 u205 u206 u207) # for FST final setup
x1=(u0 u1 u2 u3 u4 u5 u6 u7 u100 u101 u102 u103 u104 u105 u106 u107) # for FST SC test

#DO NOT chang the format
defVoltage=(70.00 140.0 70.00 140.0 70.00 140.0 70.00 140.0 70.00 140.0 70.00 140.0 70.00 140.0 70.00 140.0)
#defVoltage=(30.00 50.0 30.00 50.0 30.00 50.0 30.00 50.0 30.00 50.0 30.00 50.0 30.00 50.0 30.00 50.0)
setCurrent=0.000010
setStatus=1
setRamp=5.0
setFall=5.0
##### snmp settings
Arg="-L f log -r 5 -t 1 -Oqv -v 2c"
# Arg="-r 5 -t 1 -Oqv -v 2c"
# Arg1="-L f log -r 5 -t 2 -d -OqvU -v 2c"
# Arg1="-L f log -r 5 -t 1 -OqvU -v 2c"
Arg1="-r 5 -t 1 -OqvU -v 2c"


TurnpowerON()
{
  TurnON="$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
  echo "Power Status is = $TurnON Now TURNING ON .... "
  TurnON="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 1)"
  TurnON="$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
  echo "Power Status is = $TurnON Now"
  until [ $TurnON = "on" ]; do
    echo "Retrying to TURNING ON ............"
    sleep 1
    TurnON="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 1)"
    TurnON="$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
    echo "Power Status is = $TurnON   .... "
  done

  COUNTER=0
  COUNTER1=$Turnondelay
  while [ $COUNTER -lt $Turnondelay ]; do
    echo "Wait $COUNTER1 seconds....."
    sleep 1
    let COUNTER=COUNTER+1
    let COUNTER1=Turnondelay-COUNTER
  done
}

TurnpowerOFF()
{
  COUNTER=0
  COUNTER1=$Turnoffdelay
  while [ $COUNTER -lt $Turnoffdelay ]; do
    echo "Wait $COUNTER1 seconds. To TURN OFF FST .."
    sleep 1
    let COUNTER=COUNTER+1
    let COUNTER1=Turnoffdelay-COUNTER
  done

  TurnOFF="$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
  echo "Power Status is = $TurnOFF Now truning OFF .... "
  TurnOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 0)"
  echo "Power Status is = $TurnOFF Now  "
  until [ $TurnOFF = "off" ]; do
    echo "Retrying to TurnOFF ............"
    sleep 1
    TurnOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 0)"
    TurnOFF="$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
    echo "Power Status is = $TurnOFF Now  .... "
  done
}

PowerRecycle()
{
  echo "Houston we have a problem "
  echo "Emergency PowerRecycle"
  TurnOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 0)"
  echo "FST Power became = $TurnOFF Now  "
  sleep 2
  TurnpowerON
  sleep 5
  echo "Emergency PowerRecycle Done"
}


GetChennelNum()
{
  channelCount=$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputNumber.0) 
  echo "channelCount= $channelCount"
  Gcount=0
  until [ $channelCount == $NumChennels  ]; do
    sleep 1
    PowerRecycle
    echo "Retrying to get the right channelCount ............"
    channelCount=$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputNumber.0)
    echo "channelCount= $channelCount"
    let Gcount=Gcount+1  
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"
      TurnpowerOFF
      exit 0
    fi
  done
}

GetChennelsIndex()
{
  indices=$(snmpwalk $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputIndex)
  echo  $indices
  x=(`echo $indices | tr ' ' ' '`)

  Gcount=0
  # until [ ${x1[23]} = ${x[23]} ] && [ ${x1[7]} = ${x[7]}] && [ ${x1[13]} = ${x[13]}] ; do
  until [ ${x1[7]} = ${x[7]} ] && [ ${x1[13]} = ${x[13]} ] ; do
    echo "Retrying to get the right ChennelsIndex ............"
    PowerRecycle
    sleep 1
    indices=$(snmpwalk $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputIndex)
    echo  $indices
    x=(`echo $indices | tr ' ' ' '`)
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "FST TURNOFF and call expert, FST is broken"
      TurnpowerOFF
      exit 0
    fi
  done
}

SetVoltage()
{
  voltage="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltage.$index F $setVoltage)"
  m=${voltage%.*}
  n=${setVoltage%.*}
  settingdiff=`expr $m - $n`
  echo "SetVoltage for channel $index to ${voltage:0:4}V"

  Gcount=0
  until [ $settingdiff == 0 ]; do
    sleep 1
    voltage="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltage.$index F $setVoltage)"
    m=${voltage%.*}
    n=${setVoltage%.*}
    settingdiff=`expr $m - $n`
    echo "retrying the seting voltage for channel $index to $voltage  "
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"
      TRUNONRetery="doit"
      break
    fi
  done

  sleep $Wlines
}

SetCurrentlimit()
{
  iLimit="i$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputCurrent.$index F $setCurrent)"
  m=${iLimit:7:2}
  isetCurrent="i$setCurrent"
  n=${isetCurrent:7:2}
  settingdiff=`expr $m - $n`
  #echo "n= $n   m= $m  $settingdiff     iLimit=$iLimit  setCurrent=$setCurrent"
  echo "SetCurrent for channel $index to ${iLimit:7:2}uA"
  Gcount=0
  until [ $settingdiff -le 2 ]; do
    sleep 1
    iLimit="i$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputCurrent.$index F $setCurrent)"
    m=${iLimit:5:2}
    isetCurrent="i$setCurrent"
    n=${isetCurrent:7:2}
    settingdiff=`expr $m - $n`
    echo "retrying the seting Current for channel $index to $setCurrent  "
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"

      TRUNONRetery="doit"
      break
    fi
  done

  sleep $Wlines
}

SetVoltageRiseRate()
{
  VoltageRiseRate="i$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltageRiseRate.$index F $setRamp)"
  m=${VoltageRiseRate:1:1}
  isetRamp="i$setRamp"
  n=${isetRamp:1:1}
  settingdiff=`expr $m - $n`

  #echo "n= $n   m= $m  $settingdiff "
  echo "VoltageRiseRate for channel $index to ${isetRamp:1:3}V"
  Gcount=0
  until [ $settingdiff -le 2 ]; do
    sleep 1
    VoltageRiseRate="i$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltageRiseRate.$index F $setRamp)"
    m=${VoltageRiseRate:1:1}
    isetRamp="i$setRamp"
    n=${isetRamp:1:1}
    echo "setRamp= $n  VoltageRiseRate= $m  "
    settingdiff=`expr $m - $n`
    echo "retrying to set VoltageRiseRate for channel $index to $setRamp "
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"

      TRUNONRetery="doit"
      break
    fi
  done

  sleep $Wlines
}

SetVoltageFallRate()
{
  Fallspeed="i$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltageFallRate.$index F $setFall)"
  m=${Fallspeed:1:1}
  isetFall="i$setFall"
  n=${isetFall:1:1}
  settingdiff=`expr $m - $n`
  #echo "n= $n   m= $m  $settingdiff "
  echo "VoltageFallRate for channel $index to ${isetFall:1:3}V"
  Gcount=0
  until [ $settingdiff -le 2 ]; do
    sleep 1
    Fallspeed="i$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltageRiseRate.$index F $setFall)"
    m=${Fallspeed:1:1}
    isetFall="i$setFall"
    n=${isetFall:1:1}
    #  echo "setFall= $n  Fallspeed= $m  "
    settingdiff=`expr $m - $n`
    echo "retrying the seting Fallspeed for channel $index to $setFall  "
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"

      TRUNONRetery="doit"
      break
    fi
  done

  sleep $Wlines
}

SetAllHVON()
{
  # snmpset -r 2 -t 3 -OqvU -v 2c  -M ~/temp/mibs -m +WIENER-CRATE-MIB -c seCrET 130.199.60.169 groupsSwitch.64 i 1
  Allchannelson="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip groupsSwitch.64 i 1 )"
  echo "HV outputs are  $Allchannelson Now truning ON .... "
  Gcount=0
  until [ $Allchannelson = "on" ]; do
    sleep 1
    echo "Retrying to set all HV ON "
    Allchannelson="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip groupsSwitch.64 i 1 )"
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"

      TRUNONRetery="doit"
      break
    fi
  done
}

SetAllHVOFF()
{
  echo "Set all HV off "
  Allchannelsoff="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip groupsSwitch.64 i 0)"
  Gcount=0
  until [ $Allchannelsoff = "off" ]; do
    sleep 1
    echo "Retrying to set all HV off "
    Allchannelsoff="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip groupsSwitch.64 i 0)"
    let Gcount=Gcount+1
    if [ $Gcount -eq $Nretry ] 
    then
      echo "Houston we have a problem "
      echo "TURNOFF and call expert, FST is broken"
      TurnpowerOFF
      break
    fi
  done
  echo "--------------------------------------------- "
  echo "OK Dude All HV outputs is setting to off NOW"
  echo "--------------------------------------------- "
}


DoRetryturnon()
{
  TurnpowerOFF
  sleep 2
  TurnpowerON
  GetChennelNum
  GetChennelsIndex
  COUNTER=0
  while [ $COUNTER -lt $channelCount ]; do
    index=$(echo ${x[${COUNTER}]})
    setVoltage=$(echo ${defVoltage[${COUNTER}]})
    SetVoltage
    SetCurrentlimit
    SetVoltageRiseRate
    SetVoltageFallRate
    let COUNTER=COUNTER+1
  done
  sleep 1
  SetAllHVON

  exit 0
}

##############  start  #########################
if [ ! -n "$1" ] 
then
  echo "Usage: `basename $0` argument1 argument2 etc."
exit $E_BADARGS
fi 

############################################################
# TURN ON
if [ "$1" == 1  ] 
then
  echo "--------------------------------------------- "
  echo " TURNING CRATES ON "
  echo "--------------------------------------------- "
  TurnpowerON
  GetChennelNum
  GetChennelsIndex
  COUNTER=0
  while [ $COUNTER -lt $channelCount ]; do
    index=$(echo ${x[${COUNTER}]})
    setVoltage=$(echo ${defVoltage[${COUNTER}]})
    SetVoltage
    SetCurrentlimit
    SetVoltageRiseRate
    SetVoltageFallRate
    let COUNTER=COUNTER+1
  done

  sleep 1
  SetAllHVON
  echo "The HV-channels' Voltage are ramping up"
  outputVoltage="$(snmpget  $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.u1)"
  until [ ${outputVoltage%.*} -ge `expr ${setVoltage%.*} - 1` ]
  do
    outputVoltage="$(snmpget  $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.u1)"
    echo "Ramping up the high voltage. Current high voltage: $outputVoltage"
    sleep 1
  done

  echo "--------------------------------------------- "
  if [ "$TRUNONRetery" = "doit"  ] 
  then 
    DoRetryturnon
  fi
  exit 1
fi
############################################################

############################################################
# TURN OFF
if [ "$1" == 2  ] 
then
  TurnON="$(snmpget  $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
  echo "Power Status is = $TurnON  "
  if [ $TurnON = "off" ]
  then
    echo "WARNING Power Status is ALREADY  $TurnON    "
    exit 0
  fi

  echo "--------------------------------------------- "
  echo " TURNING CRATES OFF "

  sleep 1
  SetAllHVOFF
  echo "The HV-channels' Voltage are ramping down"
  outputVoltage="$(snmpget  $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.u1)"
  until [ ${outputVoltage%.*} -le 0 ]
  do
    outputVoltage="$(snmpget  $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.u1)"
    echo "Ramping down the high voltage. Current high voltage: $outputVoltage"
    sleep 1
  done

  echo "Wait $Turnoffdelay seconds before FST turned off"
  TurnpowerOFF

  exit 0
fi
############################################################


###############################################################333333
# POWER RECYCLE
if [ "$1" == 5  ] 
then
  echo "Houston we have a problem "
  echo "emergency Power shut down"
  TurnpowerOFF
  echo "emergency Power shut down done"
  exit 0
fi
############################################################


###################################
# if [ $1 == 3 ] || [ $1 == 4 ]
# then
#
#
#   if  [ $1 == 4 ]
#   then
#     Arg1=" -r 2 -t 3 -d -OqvU -v 2c"
#   fi
#
#
#   TurnON="$(snmpget  $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 )"
#   echo "Power Status is = $TurnON  "
#   if [ $TurnON = "off" ]
#   then
#     echo "Power Status is = $TurnON Please Turn ON FST than try again  "
#     exit 0
#   fi 
#
#
#   echo "--------------------------------------------- "
#   echo "SCAN STARTS soon----- sit tight "
#   echo "--------------------------------------------- "
#   channelCount=$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputNumber.0)
#
#   channelCount=$(snmpget $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputNumber.0)
#
# fi
#
# indices=$(snmpwalk $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputIndex)
# x=(`echo $indices | tr ' ' ' '`)
#
# Run=2
# COUNTERrun=0
# while [ $COUNTERrun -lt $Run ]; do
#
#   COUNTER=0
#   while [ $COUNTER -lt $channelCount ]; do
#     index=$(echo ${x[${COUNTER}]})    
#     voltage=$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c pub-seCrET $ip outputVoltage.$index)
#     iLimit=$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c pub-seCrET $ip outputCurrent.$index)
#     sense=$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c pub-seCrET $ip outputMeasurementSenseVoltage.$index)
#     current=$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c pub-seCrET $ip outputMeasurementCurrent.$index)
#     rampspeed=$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c pub-seCrET $ip outputVoltageRiseRate.$index)
#     Fallspeed=$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltageFallRate.$index F $setFall)
#
#     status=$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c pub-seCrET $ip outputSwitch.$index)
#     echo "HV-CH=$index Vsense=$sense Isense=$current @Setting(Vset=$voltage Iset=$iLimit Vramp=$rampspeed Vfall=$Fallspeed Status=$status)"
#     let COUNTER=COUNTER+1
#   done
#   let COUNTERrun=COUNTERrun+1
# done
# exit 0
#
# fi 
