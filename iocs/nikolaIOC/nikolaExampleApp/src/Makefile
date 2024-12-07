TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = nikola
# nikola.dbd will be created and installed
DBD += nikola.dbd

# nikola.dbd will be made up from these files:
nikola_DBD += base.dbd

# Include dbd files from all support applications:
#nikola_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
nikola_LIBS += nikola
nikola_LIBS += asyn

# nikola_registerRecordDeviceDriver.cpp derives from nikola.dbd
nikola_SRCS += nikola_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
nikola_SRCS_DEFAULT += nikolaMain.cpp
nikola_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#nikola_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
nikola_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

