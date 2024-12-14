#!../../bin/rhel7-x86_64/nikolaIOC

#- You may have to change nikola to something else
#- everywhere it appears in this file

< envPaths

epicsEnvSet( "DEVICE_PORT",  "127.0.0.1" )
epicsEnvSet( "LOCA", "OUTS" )
epicsEnvSet( "POS", "123" )
epicsEnvSet( "PV_BASE", "TEMP:$(LOCA):$(POS)" )
epicsEnvSet( "STREAM_PROTOCOL_PATH", "$(TOP)/db" )

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/nikolaIOC.dbd"
nikolaIOC_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure( "P0", "$(DEVICE_PORT):22222", 0, 0, 0 )
#asynSetTraceMask("P0", 0, "0x08")
#asynSetTraceIOMask("P0", 0, "0x01")
rateLimitInterposeInit("P0", "0.5")

## Load record instances
dbLoadRecords("db/nikola.db","device=$(PV_BASE),port=P0")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=marcio"
