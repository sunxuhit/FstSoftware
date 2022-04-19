#!../../bin/linux-x86_64/fstCooling                              
##-*- mode: sh -*-

< envPaths

cd ${TOP}

epicsEnvSet(IOCSH_PS1,"fstCooling> ")

##epicsEnvSet("EPICS_IOC_LOG_INET","18.229.0.251")
##epicsEnvSet("EPICS_IOC_LOG_PORT","20000")
##iocLogInit()

epicsEnvSet("STREAM_PROTOCOL_PATH",".:/usr/local/epics/soft/synApps_5_7GMT-12.1/support/stream-2-6/protocols")

## Register all support components
dbLoadDatabase "dbd/fstCooling.dbd"
fstCooling_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/adam-6017.db")
dbLoadRecords("db/adam-6052.db")

#dbLoadRecords("db/tc-48-temps.db")
#dbLoadRecords("db/tc-48-settings.db")
#dbLoadRecords("db/tc-48-misc.db")

dbLoadRecords("db/hx86pa.db")

#cd ${MODULE_TOP}
#dbLoadRecords("db/asynRecord.db","P=IST:,R=asyn,PORT=HX86PA,ADDR=0,IMAX=100,OMAX=100")


# Configure ASYN port drivers
drvAsynIPPortConfigure("ADAM-6017","130.199.61.22:502 TCP",0,0,1)
drvAsynIPPortConfigure("ADAM-6052","130.199.61.21:502 TCP",0,0,1)
#drvAsynIPPortConfigure("TC-48","130.199.61.23:2000 TCP",0,0,0)
#asynSetTraceIOMask("TC-48", 0, 2)
#asynSetTraceMask("TC-48", 0, 255)
drvAsynIPPortConfigure("HX86PA","130.199.61.24:2000 TCP",0,0,0)


# Configure modbus interpose drivers
modbusInterposeConfig("ADAM-6017",0,2000,0)
modbusInterposeConfig("ADAM-6052",0,2000,0)


# Configure modbus channels

# 6017 - read analog inputs
drvModbusAsynConfigure("ADAM-6017-IN","ADAM-6017",1,4,0,8,0,100,"ADAM6017")
# 6052 - read digital inputs
drvModbusAsynConfigure("ADAM-6052-IN","ADAM-6052",1,2,0,8,0,100,"ADAM6052")
# 6052 - read digital outputs
drvModbusAsynConfigure("ADAM-6052-OUTR","ADAM-6052",1,1,16,8,0,100,"ADAM6052")
# 6052 - write digital outputs
drvModbusAsynConfigure("ADAM-6052-OUTW","ADAM-6052",1,5,16,8,0,1,"ADAM6052")

cd ${TOP}/iocBoot/${IOC}
iocInit
