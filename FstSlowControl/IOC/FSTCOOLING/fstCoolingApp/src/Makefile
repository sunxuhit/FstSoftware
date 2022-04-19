TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

STATIC_BUILD=YES

PROD_IOC = fstCooling
# fstCooling.dbd will be created and installed
DBD += fstCooling.dbd

# fstCooling.dbd will be made up from these files:
fstCooling_DBD += base.dbd
fstCooling_DBD += softIoc.dbd

# Include dbd files from all support applications:
fstCooling_DBD += stream.dbd
fstCooling_DBD += drvAsynIPPort.dbd
fstCooling_DBD += modbus.dbd
fstCooling_DBD += modbusSupport.dbd
#fstCooling_DBD += pcre.dbd
#fstCooling_DBD += pcreSupport.dbd

# Add all the support libraries needed by this IOC
fstCooling_LIBS += stream
fstCooling_LIBS += modbus
fstCooling_LIBS += asyn
#fstCooling_LIBS += pcre

# fstCooling_registerRecordDeviceDriver.cpp derives from fstCooling.dbd
fstCooling_SRCS += fstCooling_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
fstCooling_SRCS_DEFAULT += fstCoolingMain.cpp
fstCooling_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#fstCooling_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
fstCooling_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

