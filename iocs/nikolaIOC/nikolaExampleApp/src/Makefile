TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = nikolaIOC
# nikolaIOC.dbd will be created and installed
DBD += nikolaIOC.dbd

# nikola.dbd will be made up from these files:
nikolaIOC_DBD += base.dbd

# Include dbd files from all support applications:
nikolaIOC_DBD += nikola.dbd
nikolaIOC_DBD += asyn.dbd

# Add all the support libraries needed by this IOC
nikolaIOC_LIBS += nikola
nikolaIOC_LIBS += asyn

# nikolaIOC_registerRecordDeviceDriver.cpp derives from nikolaIOC.dbd
nikolaIOC_SRCS += nikolaIOC_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
nikolaIOC_SRCS_DEFAULT += nikolaIOCMain.cpp
nikolaIOC_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#nikolaIOC_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
nikolaIOC_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

