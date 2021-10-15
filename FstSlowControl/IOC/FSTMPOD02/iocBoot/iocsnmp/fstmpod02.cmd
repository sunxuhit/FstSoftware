#!../../bin/linux-x86_64/snmp
## You may have to change snmp to something else
## everywhere it appears in this file
< envPaths

## Register all support components
cd ${TOP}/dbd
dbLoadDatabase("snmp.dbd")
dbLoadDatabase("modbus.dbd")
#dbLoadDatabase("calcSupport.dbd")
cd ${TOP}
snmp_registerRecordDeviceDriver(pdbbase)
modbus_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/crate.db","HOST=130.199.61.4,SYSTEM=fstmpod02")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=0")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=1")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=2")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=3")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=4")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=5")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=6")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u",ID=7")
##dbLoadRecords("db/fst_alarms.db","SYSTEM=fstmpod02,SLOT="u"")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=0")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=1")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=2")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=3")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=4")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=5")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=6")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u10",ID=7")
##dbLoadRecords("db/fst_alarms.db","SYSTEM=fstmpod02,SLOT="u10"")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=0")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=1")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=2")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=3")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=4")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=5")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=6")
dbLoadRecords("db/iseg.db","HOST=130.199.61.4,SYSTEM=fstmpod02,SLOT="u20",ID=7")
##dbLoadRecords("db/fst_alarms.db","SYSTEM=fstmpod02,SLOT="u20"")

## Load Control Interface db for seq
dbLoadRecords("db/fgt_controls.db","SYSTEM=fstmpod02")

##dbLoadRecords("db/fee_enable.db","ID=2")
##dbLoadRecords("db/fee_enable.db","ID=3")
dbLoadRecords("db/fee_temp.db","ID=2_0")
dbLoadRecords("db/fee_temp.db","ID=2_1")
dbLoadRecords("db/fee_temp.db","ID=2_2")
##dbLoadRecords("db/fee_temp.db","ID=2_3")
##dbLoadRecords("db/fee_temp.db","ID=2_4")
##dbLoadRecords("db/fee_temp.db","ID=2_5")
dbLoadRecords("db/fee_temp.db","ID=3_0")
dbLoadRecords("db/fee_temp.db","ID=3_1")
dbLoadRecords("db/fee_temp.db","ID=3_2")
##dbLoadRecords("db/fee_temp.db","ID=3_3")
##dbLoadRecords("db/fee_temp.db","ID=3_4")
##dbLoadRecords("db/fee_temp.db","ID=3_5")

## Set this to see messages from mySub
#var mySubDebug 1

cd ${TOP}/iocBoot/${IOC}
iocInit()
#iocLogInit()
epicsSnmpInit()

## Load record instances
seq sncFGTControl, "SYSTEM=fstmpod02,SLOT=u"
seq sncFGTRamping, "SYSTEM=fstmpod02,SLOT=u20,deviceName=u20"
seq sncFGTRamping, "SYSTEM=fstmpod02,SLOT=u10,deviceName=u10"
seq sncFGTRamping, "SYSTEM=fstmpod02,SLOT=u,deviceName=u"

dbpf("fstmpod02:hv_settings","/home/sysuser/iocTop/FST/HV/mpod02/def")
