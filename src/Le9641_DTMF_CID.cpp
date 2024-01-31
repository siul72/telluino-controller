
#include "slic_device.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>


/*
 * These files are required by all applications.  "vp_api.h" includes all of the
 * API libraries required by the application.  "profiles.h" includes the
 * profiles used by the application.  At a minimum, there must be a Device
 * Profile for the device to initialize.
 */
#include "vp_api.h"
#include "profiles.h"

/* ZTAP specific initialization header */
#include "sdk_qs_board.h"

/* Common utility functions */
#include "sdk_qs_utils.h"
#include "vp886_api.h"


/* ZTAP specific values for device ID depending on the MPI Mode */
#define MPI_3WIRE       0x0000 /* 3 wire SPI w/ 2.5us interbyte chipselect off-time */
#define MPI_4WIRE       0x0100 /* 4 wire SPI w/ 2.5us interbyte chipselect off-time */
#define SPI8_3WIRE      0x0200 /* 3 wire SPI w/ one clock chipselect off-time */
#define SPI8_4WIRE      0x0400 /* 4 wire SPI w/ one clock chipselect off-time */
#define SPI8_3WIRE_CFM  0x0300 /* 3 wire SPI w/o chipselect off-time per VpMpiCmd() transation */
#define SPI8_4WIRE_CFM  0x0500 /* 4 wire SPI w/o chipselect off-time per VpMpiCmd() transation */
#define ZSI_4WIRE       0x0E00 /* 4 wire ZSI transation*/

#define SM              0x0010
#define DIN             0x0000

/* Number of lines supported by the quick start */
#define NUM_LINES       1

/* Internal Helper functions */
static void alarmHandle(int val);
static void InitSetMasks(void);
static void BeginRinging(uint8 channelId);
static void ContinueCid(uint8 channelId);


/*
 * Application memory space for the API required Device/Line contexts and
 * objects. This application uses the simplest memory scheme -- global, but the
 * API will work with any memory management scheme.
 */

VpDevCtxType devCtx;
VpLineCtxType lineCtx[NUM_LINES];
Vp886DeviceObjectType devObj;
Vp886LineObjectType lineObj[NUM_LINES];
VpStatusType status;

/*
 * Create a deviceId. Generally this is just the chipselect number of the device on the SPI bus.
 * However, this is a customer defined type in the vp_api_types.h file and can be whatever
 * the customer chooses.
 *
 * The following example is for the Microsemi ZTAP platform and combines the
 * location of the SLAC on the platform and the SPI interface type used by the SLAC.
 */
VpDeviceIdType deviceId = (SM | ZSI_4WIRE);

/* 
 * CID Message, ASCII encoding
 * - The start code for calling number shall be either DTMF "A" or "D".
 * - The start code for redirecting number shall be DTMF "D".
 * - The start code for information values shall be DTMF "B".
 * - The information may be sent in any order and shall always end with DTMF "C".
 */
char cidMessage[] = "A5559871234C";

/* Keep track of how much of the CID message has been sent */
uint8 cidIndex[NUM_LINES];

/* Flag for whether to perform ringing on the next on-hook event */
bool ringNext[NUM_LINES];

EXTERN bool VpSysInit();

/**
 * Function:  main()
 *
 * Description: This function initializes the API and Devices/Lines.
 * It also starts the "alarmHandle" function to be called at a periodic rate.
 */
int init_slic_chip(void)
{
    struct itimerspec timeStruct, timeOldVal;
    uint8 lineNum = 0;

    /* ZTAP board initialization (platform specific) */
    //BoardInit(); /* See "../common/board.c" */
    //VpSysInit();

    /*
     * The following structure is specific to Linux.  It sets up a timed call
     * to a user specified function.  For the purpose of this demo, the timing
     * is 20mS and the function that will be called is "alarmHandle".  The timer
     * and function call does not start until "setitimer()" and "signal()" are
     * called (below).
     */
    timeStruct.it_interval.tv_sec = 0L;
    timeStruct.it_interval.tv_usec = 19990L;
    timeStruct.it_value.tv_sec = 0L;
    timeStruct.it_value.tv_usec = 19990L;

    timeOldVal.it_interval.tv_sec = 0L;
    timeOldVal.it_interval.tv_usec = 0L;
    timeOldVal.it_value.tv_sec = 0L;
    timeOldVal.it_value.tv_usec = 0L;

    /* Make the objects (1 device, 2 lines) */
    status = VpMakeDeviceObject(VP_DEV_887_SERIES, deviceId, &devCtx, &devObj);
    if (status != VP_STATUS_SUCCESS) {
        QS_DEBUG("Error making the device object: %s\n", MapStatus(status));
        exit(-1);
    }

    for (; lineNum < NUM_LINES; lineNum++) {
        status = VpMakeLineObject(VP_TERM_FXS_LOW_PWR, lineNum, &lineCtx[lineNum], &lineObj[lineNum], &devCtx);
        if (status != VP_STATUS_SUCCESS) {
            QS_DEBUG("Error calling VpMakeLineObject on lineNum %i: %s\n", lineNum, MapStatus(status));
            exit(-1);
        }
        status = VpMapLineId(&lineCtx[lineNum], lineNum);
        if (status != VP_STATUS_SUCCESS) {
            QS_DEBUG("Error calling VpMapLineId on lineNum %i: %s\n", lineNum, MapStatus(status));
            exit(-1);
        }
        QS_DEBUG("--- Created lineNum %i---\n", lineNum);
    }

    /* Start VpInitDevice and wait for the event VP_DEV_EVID_DEV_INIT_CMP */
    status = VpInitDevice(&devCtx, DEV_PROFILE_Buck_Boost, AC_FXS_RF14_600R_DEF,
        DC_FXS_miSLIC_BB_DEF, RING_miSLIC_BB_DEF, VP_PTABLE_NULL, VP_PTABLE_NULL);

    if (status != VP_STATUS_SUCCESS) {
        QS_DEBUG("Device not properly initialized: %s\n", MapStatus(status));
        exit(-1);
    } else {
        QS_DEBUG("\nDevice Initialization started\n");
    }

    /*
     * The following two function calls were described above as Linux specific.
     * They are used to call a user defined function at set increments.  The
     * user may have several other methods to perform the same.
     */
    //signal(SIGVTALRM, alarmHandle);
    //setitimer(ITIMER_VIRTUAL, &timeStruct, &timeOldVal);

    /*
     * Once the "tick" is setup, the main program can enter a "while(1)" loop
     * and the user specified function will take over the application (with the
     * help of the Linux OS)
     */

    while(TRUE);
    return 0;
}

/**
 * Function:  alarmHandle()
 *
 * Description: This function is called at a periodic rate to perform all
 * required API operations.  It implements the functional requirements of the
 * application mentioned in the header.
 */
void alarmHandle(int val)
{
    VpEventType event;
    static bool deviceInitialized = FALSE;
    static uint8 cmpCounter = 0;

    /*
     * This loop will query the FXS device for events, and when an event is
     * found, it will parse the event and perform further operations.
     */

    while(VpGetEvent(&devCtx, &event)) {
        /* Print the incoming event */
        UtilPrintEvent(&event); /* See "../common/utils.c" */

        /* Parse the RESPONSE category */
        if (event.eventCategory == VP_EVCAT_RESPONSE) {
            if(event.eventId == VP_DEV_EVID_DEV_INIT_CMP) {
                uint8 lineNum = 0;
                InitSetMasks();
                QS_DEBUG("Device Initialization completed\n");

                for (; lineNum < NUM_LINES; lineNum++) {
                    status = VpSetLineState(&lineCtx[lineNum], VP_LINE_STANDBY);
                    if (status != VP_STATUS_SUCCESS) {
                        QS_DEBUG("Error setting lineNum %i to VP_LINE_STANDBY: %s\n", lineNum, MapStatus(status));
                        exit(-1);
                    }
                    status = VpCalLine(&lineCtx[lineNum]);
                    QS_DEBUG("Starting Calibration on lineNum %i\n", lineNum);
                    if (status != VP_STATUS_SUCCESS) {
                        QS_DEBUG("Error calling VpCalLine on lineNum %i:: %s\n", lineNum, MapStatus(status));
                        exit(-1);
                    }
                    ringNext[lineNum] = TRUE;
                }

            } else if (event.eventId == VP_EVID_CAL_CMP) {
                if (++cmpCounter == NUM_LINES) {
                    deviceInitialized = TRUE;
                    QS_DEBUG("Calibration completed on both lines\n\n");
                }
            }

        /* Parse the SIGNALING category */
        } else if (event.eventCategory == VP_EVCAT_SIGNALING) {
            if (event.eventId == VP_LINE_EVID_HOOK_OFF) {
                status = VpSetLineState(event.pLineCtx, VP_LINE_ACTIVE);

                if (status != VP_STATUS_SUCCESS) {
                    QS_DEBUG("Error running VpSetLineState on channel %d: %s\n", event.channelId, MapStatus(status));
                    exit(-1);
                }
                QS_DEBUG("Set channel %d in ACTIVE state\n", event.channelId);

            } else if (event.eventId == VP_LINE_EVID_HOOK_ON) {
                if (ringNext[event.channelId]) {
                    BeginRinging(event.channelId);
                    ringNext[event.channelId] = FALSE;
                } else {
                    status = VpSetLineState(event.pLineCtx, VP_LINE_STANDBY);
                    if (status != VP_STATUS_SUCCESS) {
                        QS_DEBUG("Error running VpSetLineState on channel %d: %s\n", event.channelId, MapStatus(status));
                        exit(-1);
                    }
                    QS_DEBUG("Set channel %d in STANDBY state\n", event.channelId);
                    ringNext[event.channelId] = TRUE;
                }
            }
        } else if (event.eventCategory == VP_EVCAT_PROCESS) {
            if (event.eventId == VP_LINE_EVID_CID_DATA) {
                if (event.eventData == VP_CID_DATA_TX_DONE) {
                    QS_DEBUG("Caller ID transmission has ended on channel %d\n", event.channelId);
                } else if (event.eventData == VP_CID_DATA_NEED_MORE_DATA) {
                    ContinueCid(event.channelId);
                }
            }
        }

    } /* end while(VpGetEvent(&devCtx, &event)) */

    return;
}

/**
 * InitSetMasks(void)
 *
 * Description: This function initialize the masks to unmask all events
 * except the rollover event.
 */

void
InitSetMasks(void)
{
    VpOptionEventMaskType eventMask;

    /* mask everything */
    VpMemSet(&eventMask, 0xFF, sizeof(VpOptionEventMaskType));

    /* unmask only the events the application is interested in */
    eventMask.faults = VP_EVCAT_FAULT_UNMASK_ALL;
    eventMask.signaling = VP_EVCAT_SIGNALING_UNMASK_ALL | VP_DEV_EVID_TS_ROLLOVER;
    eventMask.response = VP_EVCAT_RESPONSE_UNMASK_ALL;
    eventMask.test = VP_EVCAT_TEST_UNMASK_ALL;
    eventMask.process = VP_EVCAT_PROCESS_UNMASK_ALL;

    /* inform the API of the mask */
    VpSetOption(VP_NULL, &devCtx, VP_OPTION_ID_EVENT_MASK, &eventMask);

    return;
}

static void
BeginRinging(
    uint8 channelId)
{
    VpLineCtxType *pLineCtx = &lineCtx[channelId];
    uint8 cidLen;
    uint8 initCidLen;
    VpStatusType status;

    cidLen = strlen(cidMessage);

    /* We can initialize a maximum of 32 bytes */
    if (cidLen >= 32) {
        initCidLen = 32;
    } else {
        initCidLen = cidLen;
    }

    QS_DEBUG("Beginning ringing on ch%d with CID length %d (initial %d)\n",
        channelId, cidLen, initCidLen);

    /* Provide the ringing cadence and CID profiles */
    //status = VpInitRing(pLineCtx, CR_CN, CID_DTMF);
    status = VP_STATUS_SUCCESS;
    if (status != VP_STATUS_SUCCESS) {
        QS_DEBUG("Error running VpInitRing on channel %d: %s\n", channelId, MapStatus(status));
        exit(-1);
    }

    /* Initialize up to 32 bytes of the CID data */
    //status = VpInitCid(pLineCtx, initCidLen, cidMessage);
    status = VP_STATUS_SUCCESS;
    if (status != VP_STATUS_SUCCESS) {
        QS_DEBUG("Error running VpInitCid on channel %d: %s\n", channelId, MapStatus(status));
        exit(-1);
    }
    
    /* Set the CID message index to where we will need to continue from */
    cidIndex[channelId] = initCidLen;

    /* Begin the ringing+CID cadence */
    status = VpSetLineState(pLineCtx, VP_LINE_RINGING);
    if (status != VP_STATUS_SUCCESS) {
        QS_DEBUG("Error running VpSetLineState on channel %d: %s\n", channelId, MapStatus(status));
        exit(-1);
    }
}

static void
ContinueCid(
    uint8 channelId)
{
    VpLineCtxType *pLineCtx = &lineCtx[channelId];
    uint8 cidLen;
    uint8 continueLen;
    uint8 *pCidData;
    VpStatusType status;
    
    cidLen = strlen(cidMessage);
    
    QS_DEBUG("Servicing CID NEED_MORE_DATA event on ch%d ...\n", channelId);

    if (cidIndex[channelId] >= cidLen) {
        /* No data left to send, we can ignore the event */
        QS_DEBUG("    No data remaining - finished\n");
        return;
    }

    continueLen = cidLen - cidIndex[channelId];
    
    /* We can provide up to 16 bytes of additional data */
    if (continueLen >= 16) {
        continueLen = 16;
    }
    
    QS_DEBUG("    Providing %d bytes of data to VpContinueCid()\n", continueLen);
    
    pCidData = &cidMessage[cidIndex[channelId]];
    
    //status = VpContinueCid(pLineCtx, continueLen, pCidData);
    status = VP_STATUS_SUCCESS;
    if (status != VP_STATUS_SUCCESS) {
        QS_DEBUG("Error running VpContinueCid on channel %d: %s\n", channelId, MapStatus(status));
        exit(-1);
    }

    /* Move the CID message index forward */
    cidIndex[channelId] += continueLen;
}


