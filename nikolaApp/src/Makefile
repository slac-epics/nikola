TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += nikola

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install nikola.dbd into <top>/dbd
DBD += nikola.dbd

# specify all source files to be compiled and added to the library
#nikola_SRCS += xxx

nikola_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

