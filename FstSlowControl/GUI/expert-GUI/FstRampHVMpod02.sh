#!/bin/sh
#
# Wiener Caret ON V.1.65
# Babak Abi
# Updated by Xu Sun for FST

####### crate  settings
ip=130.199.61.4 # BNL clean room stand-alone test stand

# path=mibs/
path=/usr/share/snmp/mibs/ # FST Module Assembly

#DO NOT chang the format
index=$1
setVoltage=$2
Turnondelay=5
##### snmp settings
Arg1="-r 5 -t 1 -OqvU -v 2c"

voltage="$(snmpset $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputVoltage.$index F $setVoltage)"
m=${voltage%.*}
n=${setVoltage%.*}
settingdiff=`expr $m - $n`
echo "SetVoltage for channel $index to ${voltage:0:4}V"

outputVoltage="$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.$index)"

if [ ${setVoltage%.*} -ge ${outputVoltage%.*} ]
then
  echo "The HV-channels' Voltage are ramping up"
  until [ ${outputVoltage%.*} -ge `expr ${setVoltage%.*} - 1` ]
  do
    outputVoltage="$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.$index)"
    outputCurrent="$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementCurrent.$index)"
    echo "Ramping up the high voltage. Current high voltage: ${outputVoltage}V. Output Current is ${outputCurrent}A."
    sleep 1
  done
fi

if [ ${setVoltage%.*} -lt ${outputVoltage%.*} ]
then
  echo "The HV-channels' Voltage are ramping down"
  until [ ${outputVoltage%.*} -le `expr ${setVoltage%.*} + 1` ]
  do
    outputVoltage="$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementTerminalVoltage.$index)"
    outputCurrent="$(snmpget $Arg1 -M $path -m +WIENER-CRATE-MIB -c seCrET $ip outputMeasurementCurrent.$index)"
    echo "Ramping down the high voltage. Current high voltage: ${outputVoltage}V. Output Current is ${outputCurrent}A."
    sleep 1
  done
fi

echo "WARNING Please wait to stablize the HV outputs "
COUNTER=0
COUNTER1=$Turnondelay
while [ $COUNTER -lt $Turnondelay ]; do
  echo "Wait $COUNTER1 seconds....."
  sleep 1
  let COUNTER=COUNTER+1
  let COUNTER1=Turnondelay-COUNTER
done
