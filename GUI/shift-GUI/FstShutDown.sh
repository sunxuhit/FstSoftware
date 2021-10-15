#!/bin/bash
#
# Wiener Caret ON V.1.12

####### crate  settings
# ip=130.199.60.175 # BNL Test Stand
ip=130.199.61.4 # BNL Integration Test

#path=/home/ptribedy/Downloads/net-snmp-5.8/share/snmp/mibs/ # BNL Test Stand
path=/usr/share/snmp/mibs/ # Fst Integration Test

##### snmp settings
Arg="-L f log -r 10 -t 1 -Oqv -v 2c"

caput fst_power_status_trans 1 
caput fst_crate_status_ON 0   
caput fst_crate_status_ON 0 
    echo "------------------FST emergency shudown -------------  "
    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 0)"
    TrunOFF="$(snmpset $Arg -M $path -m +WIENER-CRATE-MIB -c seCrET $ip sysMainSwitch.0 i 0)"
    echo "Crate 2 Power Status is = $TrunOFF    "
    echo "------------------FST emergency shudown -------------  "
  
caput fst_crate_status_ON 0
# caput istmpod01:crate_status_panic 1
caput fstmpod02:crate_status_panic 1
# caput istmpod03:crate_status_panic 1
caput fst_crate_status_panic 1
caput fst_power_status_trans 0

sleep 5

exit 0
