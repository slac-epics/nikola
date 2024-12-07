#include <stdlib.h>
#include <string.h>

#include <cantProceed.h>
#include <iocsh.h>
#include <epicsExport.h>
#include <epicsThread.h>

#include <asynDriver.h>
#include <asynOctet.h>

typedef struct rateLimitPvt {
    //const char*    interposeName;
    const char*    portName;
    //int            addr;
    asynInterface  octet;
    asynOctet*     pasynOctet;
    void*          asynOctetPvt;
    double         minInterval;
    epicsTimeStamp lastTime;
} rateLimitPvt;

/* asynOctet methods */
static asynStatus writeIt(void* ppvt, asynUser* pasynUser,
    const char* data, size_t numchars, size_t* nbytesTransfered);
static asynStatus readIt(void* ppvt, asynUser* pasynUser,
    char* data, size_t maxchars, size_t* nbytesTransfered, int* eomReason);
/*static asynStatus flushIt(void *ppvt,asynUser *pasynUser);
static asynStatus registerInterruptUser(void *ppvt,asynUser *pasynUser,
    interruptCallbackOctet callback, void *userPvt,void **registrarPvt);
static asynStatus cancelInterruptUser(void *ppvt,asynUser *pasynUser,
    void *registrarPvt);
static asynStatus setInputEos(void *ppvt,asynUser *pasynUser,
    const char *eos,int eoslen);
static asynStatus getInputEos(void *ppvt,asynUser *pasynUser,
    char *eos, int eossize, int *eoslen);
static asynStatus setOutputEos(void *ppvt,asynUser *pasynUser,
    const char *eos,int eoslen);
static asynStatus getOutputEos(void *ppvt,asynUser *pasynUser,
    char *eos, int eossize, int *eoslen);

static asynOctet octet = {
    writeIt, readIt, flushIt,
    registerInterruptUser,cancelInterruptUser,
    setInputEos,getInputEos,setOutputEos,getOutputEos
};
*/
static asynOctet octet = {
    writeIt, readIt, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL
};


/* asynOctet methods */
static asynStatus writeIt(void* ppvt, asynUser* pasynUser,
    const char* data, size_t numchars, size_t* nbytesTransfered)
{
    epicsTimeStamp currTime;
    double elapsed;

    rateLimitPvt* pRateLimit = (rateLimitPvt*) ppvt;

    asynPrint(pasynUser, ASYN_TRACEIO_FILTER,
        "entered rateLimitInterpose::write\n");

    epicsTimeGetCurrent(&currTime);
    elapsed = epicsTimeDiffInSeconds(&currTime, &pRateLimit->lastTime);

    if (elapsed < pRateLimit->minInterval) {
        epicsThreadSleep(pRateLimit->minInterval - elapsed);
    }

    epicsTimeGetCurrent(&pRateLimit->lastTime);

    return asynSuccess;
}

static asynStatus readIt(void* ppvt, asynUser* pasynUser,
    char* data, size_t maxchars, size_t* nbytesTransfered, int* eomReason)
{
    epicsTimeStamp currTime;
    double elapsed;

    rateLimitPvt* pRateLimit = (rateLimitPvt*) ppvt;

    asynPrint(pasynUser, ASYN_TRACEIO_FILTER,
        "entered rateLimitInterpose::read\n");

    epicsTimeGetCurrent(&currTime);
    elapsed = epicsTimeDiffInSeconds(&currTime, &pRateLimit->lastTime);

    if (elapsed < pRateLimit->minInterval) {
        epicsThreadSleep(pRateLimit->minInterval - elapsed);
    }

    epicsTimeGetCurrent(&pRateLimit->lastTime);

    return asynSuccess;
}

static int rateLimitInterposeInit(const char* port, int interval)
{
    rateLimitPvt* pRateLimit;
    //char *interposeName;
    char* portName;
    asynStatus status;
    asynInterface* poctetasynInterface;

    //interposeName = callocMustSucceed(strlen(pmn)+1,sizeof(char),
    //    "rateLimitInterposeInit");
    //strcpy(interposeName,pmn);
    portName = callocMustSucceed(strlen(port) + 1, sizeof(char),
        "rateLimitInterposeInit");
    strcpy(portName, port);
    pRateLimit = callocMustSucceed(1, sizeof(rateLimitPvt), "rateLimitInterposeInit");
    //pRateLimit->interposeName = interposeName;
    pRateLimit->portName = portName;
    //pRateLimit->addr = addr;
    epicsTimeGetCurrent(&pRateLimit->lastTime);
    pRateLimit->minInterval = interval;
    pRateLimit->octet.interfaceType = asynOctetType;
    pRateLimit->octet.pinterface = &octet;
    pRateLimit->octet.drvPvt = pRateLimit;

    status = pasynManager->interposeInterface(portName, 0,
        &pRateLimit->octet, &poctetasynInterface);
    if ((status != asynSuccess) || !poctetasynInterface) {
        printf("%s rateLimitInterpose failed.\n", portName);
        free(pRateLimit);
        free(portName);
        //free(interposeName);
        return(0);
    }
    pRateLimit->pasynOctet = (asynOctet*) poctetasynInterface->pinterface;
    pRateLimit->asynOctetPvt = poctetasynInterface->drvPvt;
    return(0);
}

/* register rateLimitInterposeInit*/
static const iocshArg rateLimitInterposeInitArg0 =
    { "portName", iocshArgString };
static const iocshArg rateLimitInterposeInitArg1 =
    { "minimumInterval", iocshArgInt };
static const iocshArg* rateLimitInterposeInitArgs[] =
    {&rateLimitInterposeInitArg0, &rateLimitInterposeInitArg1};
static const iocshFuncDef rateLimitInterposeInitFuncDef =
    {"rateLimitInterposeInit", 2, rateLimitInterposeInitArgs};
static void rateLimitInterposeInitCallFunc(const iocshArgBuf* args)
{
    rateLimitInterposeInit(args[0].sval, args[1].ival);
}

static void rateLimitInterposeRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&rateLimitInterposeInitFuncDef, rateLimitInterposeInitCallFunc);
    }
}
epicsExportRegistrar(rateLimitInterposeRegister);
