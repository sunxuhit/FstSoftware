#!../../bin/linux-x86_64/snmp
## You may have to change snmp to something else
## everywhere it appears in this file
< envPaths
modbus_registerRecordDeviceDriver(pdbbase)

## Register all support components
cd ${TOP}/dbd
cd ${TOP}
drvModbusAsynConfigure(myPortName,127.0.0.1,127.0.0.1,1,0,1,0,10,"Modicon")

## Load record instances

## Set this to see messages from mySub
#var mySubDebug 1

cd ${TOP}/iocBoot/${IOC}
iocInit()
epicsSnmpInit()
