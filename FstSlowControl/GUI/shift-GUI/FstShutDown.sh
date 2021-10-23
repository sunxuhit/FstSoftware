#!/bin/bash
#
# Wiener Caret ON V.1.12

####### crate  settings
ip1=130.199.61.3 # FST MPOD01 
ip2=130.199.61.4 # FST MPOD02 
ip3=130.199.61.5 # FST MPOD03 

#path=/home/ptribedy/Downloads/net-snmp-5.8/share/snmp/mibs/ # BNL Test Stand
path=/usr/share/snmp/mibs/ # Fst Integration Test

##### snmp settings
Arg="-L f log -r 10 -t 1 -Oqv -v 2c"

caput fst_power_status_trans 1 
caput fst_crate_status_ON 0   
caput fst_crate_status_ON 0 
    echo "------------------FST emergency shudown -------------  "
    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip1 sysMainSwitch.0 i 0)"
    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip1 sysMainSwitch.0 i 0)"
    echo "Crate 1 Power Status is = $TrunOFF    "

    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip2 sysMainSwitch.0 i 0)"
    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip2 sysMainSwitch.0 i 0)"
    echo "Crate 2 Power Status is = $TrunOFF    "

    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip3 sysMainSwitch.0 i 0)"
    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip3 sysMainSwitch.0 i 0)"
    echo "Crate 3 Power Status is = $TrunOFF    "
    echo "------------------FST emergency shudown -------------  "
  
caput fst_crate_status_ON 0
caput fstmpod01:crate_status_panic 1
caput fstmpod02:crate_status_panic 1
caput fstmpod03:crate_status_panic 1
caput fst_crate_status_panic 1
caput fst_power_status_trans 0

sleep 5

exit 0
