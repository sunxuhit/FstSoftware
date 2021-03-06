TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
USR_CFLAGS += -I/usr/local/include        #Path to snmp if ness.
#USR_CFLAGS += `net-snmp-config --cflags`
USR_CFLAGS += -DNETSNMP_NO_INLINE
USR_LDFLAGS += `net-snmp-config --libs`
#==================================================
# Build an IOC support library
LIBRARY_IOC += devSnmp netsnmp

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install devXxxSoft.dbd into <top>/dbd
DBD += devSnmp.dbd
DBD += modbus.dbd
DBD += modbusSupport.dbd

INC += drvModbusAsyn.h

LIBRARY_IOC += modbus

LIB_SRCS += drvModbusAsyn.c
LIB_SRCS += modbusInterpose.c
LIB_LIBS += asyn
LIB_LIBS += $(EPICS_BASE_IOC_LIBS)

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
PROD_SRCS_DEFAULT += modbus_registerRecordDeviceDriver.cpp 

PROD_LIBS += modbus
PROD_LIBS += asyn
PROD_LIBS += $(EPICS_BASE_IOC_LIBS)

# The following are compiled and added to the Support library

devSnmp_SRCS += devSnmp.cpp
devSnmp_SRCS += snmpRegister.cpp
devSnmp_SRCS += snmpSessShow.c

devSnmp_LIBS += Com

#xxxSupport_LIBS += $(EPICS_BASE_IOC_LIBS)
devSnmp_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# build an ioc application

PROD_IOC = snmp
# snmp.dbd will be created and installed
DBD += snmp.dbd

# snmp.dbd will be made up from these files:
snmp_DBD += base.dbd

snmp_DBD +=  devSnmp.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
snmp_SRCS += snmp_registerRecordDeviceDriver.cpp
snmp_SRCS_DEFAULT += snmpMain.cpp
snmp_SRCS_vxWorks += -nil-

# Add locally compiled object code
#snmp_SRCS += dbSubExample.c

# The following adds support from base/src/vxWorks
snmp_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

snmp_LIBS +=  devSnmp    #main devSnmp lib

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of snmp
    snmp_SNCFLAGS += +r
    snmp_DBD += sncExample.dbd
    snmp_SRCS += sncExample.stt
    snmp_DBD += sncFGTRamping.dbd
    snmp_SRCS += sncFGTRamping.stt
    snmp_DBD += sncFGTControl.dbd
    snmp_SRCS += sncFGTControl.stt
    snmp_LIBS += seq pv

    # The following builds sncProgram as a standalone application
    PROD_HOST += sncProgram
    sncProgram_SNCFLAGS += +m
    sncProgram_SRCS += sncProgram.st
    sncProgram_LIBS += seq pv
    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

snmp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

