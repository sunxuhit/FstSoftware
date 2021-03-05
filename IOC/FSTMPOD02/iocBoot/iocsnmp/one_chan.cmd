#!../../bin/linux-x86/snmp
## You may have to change snmp to something else
## everywhere it appears in this file
< envPaths
cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/snmp.dbd")
snmp_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/disk.db")
dbLoadRecords("db/crate.db","HOST=130.199.60.147")
dbLoadRecords("db/iseg.db","HOST=130.199.60.147,ID=0")

## Set this to see messages from mySub
#var mySubDebug 1

cd ${TOP}/iocBoot/${IOC}
iocInit()
epicsSnmpInit()
