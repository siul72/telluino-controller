//
// Created by siul72 on 11.02.2024.
//

#ifndef TELLUINO_CONTROLLER_VP886_DEVICE_H
#define TELLUINO_CONTROLLER_VP886_DEVICE_H

#include "vp_api_common.h"
#include "vp_types_common.h"
#include "vp886_registers.h"
#include "vp88x_profiles.h"

#define VP886_MAX_NUM_CHANNELS   2


/* Macro for extracting the device type (VpDeviceType) from the Device Object */
#define VP886_DEVICE_SERIES(pDevObj) \
    (((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] & VP886_R_RCNPCN_PCN_ABS_TRACKER) ? VP_DEV_887_SERIES : VP_DEV_886_SERIES)
#define VP886_IS_TRACKER(pDevObj) /* 887 */ \
    ((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] & VP886_R_RCNPCN_PCN_ABS_TRACKER)
#define VP886_IS_ABS(pDevObj) /* 886 */ \
    !VP886_IS_TRACKER(pDevObj)
#define VP886_IS_HV(pDevObj) \
    (((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] & VP886_R_RCNPCN_PCN_LV_HV) \
        && !((pDevObj)->devProfileData.lowVoltOverride))
#define VP886_IS_LV(pDevObj) \
    !VP886_IS_HV(pDevObj)
#define VP886_IS_1CH(pDevObj) \
    (!((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] & VP886_R_RCNPCN_PCN_2CH))
#define VP886_REVISION(pDevObj) \
    ((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_RCN_IDX])
#define VP886_IS_SF(pDevObj) \
    (!((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] & VP886_R_RCNPCN_PCN_NOT_SF) && VP886_REVISION(pDevObj) >= 7)
#define VP886_IS_SF0(pDevObj) /* Apollo2 in 48-pin package with SF PCN */ \
    (!((pDevObj)->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] & VP886_R_RCNPCN_PCN_NOT_SF) && VP886_REVISION(pDevObj) < 7)
#define VP886_IS_SF1(pDevObj) \
    (VP886_IS_SF(pDevObj) && VP886_REVISION(pDevObj) >= 8 && VP886_IS_1CH(pDevObj))
#define VP886_IS_SF1AB(pDevObj) \
    (VP886_IS_SF(pDevObj) && VP886_REVISION(pDevObj) == 7 && VP886_IS_1CH(pDevObj))
#define VP886_IS_SF2(pDevObj) \
    (VP886_IS_SF(pDevObj) && VP886_REVISION(pDevObj) >= 8 && !VP886_IS_1CH(pDevObj))

typedef enum {
    VP886_IO_CAPABILITY_TWO_PER_CH = 0, /* 2 IOs per channel */
    VP886_IO_CAPABILITY_NONE = 1,       /* No IOs */
    VP886_IO_CAPABILITY_CH0_IO2 = 2     /* IO2 on channel 0 only */
} Vp886IoCapabilityType;

/* Union of basic results types for Vp886.  Does not include linetest,
 * low level cmd, or result types not supported by Vp886. */
typedef union {
    VpLoopCondResultsType getLoopCond;         /* VP_LINE_EVID_RD_LOOP        */
    VpDeviceIoAccessDataType deviceIoAccess;   /* VP_DEV_EVID_IO_ACCESS_CMP   */
    VpLineIoAccessType lineIoAccess;           /* VP_LINE_EVID_LINE_IO_RD_CMP */
} Vp886BasicResultsType;

/* Enforce a minimum configurable event queue size */
#define VP886_EVENT_QUEUE_SIZE 6
#define VP886_EVENT_QUEUE_MIN_SIZE 6
#if VP886_EVENT_QUEUE_SIZE < VP886_EVENT_QUEUE_MIN_SIZE
    #error VP886_EVENT_QUEUE_SIZE configured below the minimum
#endif
/* Define a new event structure to use in the queue to reduce size.  VpEventType
 * contains several data members that are not necessary internally. */
typedef struct {
    uint8 channelId;
    VpEventCategoryType eventCategory;
    uint16 eventId;
    uint16 eventData;
    uint16 parmHandle;
    bool hasResults;
} Vp886QueuedEventType;

typedef struct {
    Vp886QueuedEventType events[VP886_EVENT_QUEUE_SIZE];
    uint8 numQueued;
    uint8 pushIndex;
    uint8 popIndex;
    boolean overflowed;
} Vp886EventQueueType;

/** Parameters for dial pulse, flash, and on-hook */
typedef struct VpOptionPulseType {
    uint16 breakMin;        /**< Minimum pulse break time (in 125uS) */
    uint16 breakMax;        /**< Maximum pulse break time (in 125uS) */
    uint16 makeMin;         /**< Minimum pulse make time (in 125uS) */
    uint16 makeMax;         /**< Maximum pulse make time (in 125uS) */
    uint16 interDigitMin;   /**< Minimum pulse interdigit time (in 125uS) */
    uint16 flashMin;        /**< Minimum flash break time (in 125uS) */
    uint16 flashMax;        /**< Maximum flash break time (in 125uS) */
#ifdef EXTENDED_FLASH_HOOK
    uint16 onHookMin;       /**< Minimum on-hook time (in 125uS) */
#endif
#ifdef VP_ENABLE_OFFHOOK_MIN
    uint16 offHookMin;      /**< Minimum off-hook time (in 125uS) */
#endif
} VpOptionPulseType;

/** Method for line control when critical faults are detected */
typedef struct VpOptionCriticalFltType {
    /**< The line is set to disconnect when the specified fault is active and
     * the "En" bit is set TRUE
     */
    bool acFltDiscEn;           /**< AC fault detected */
    bool dcFltDiscEn;           /**< DC fault detected */
    bool thermFltDiscEn;        /**< Thermal fault detected */
} VpOptionCriticalFltType;

typedef struct VpOptionDeviceIoType {
    uint32 directionPins_31_0;      /* Device specific IO pin direction
                                     * (Pins 0 - 31) */
    uint32 directionPins_63_32;     /* Device specific IO pin direction
                                     * (Pins 32 - 63) */

    uint32 outputTypePins_31_0;     /* Output pin type (Pins 0 - 31) */
    uint32 outputTypePins_63_32;    /* Output pin type (Pins 32 - 63) */
} VpOptionDeviceIoType;

/* Mode values for VP_DEVICE_OPTION_ID_ADAPTIVE_RINGING */
typedef enum VpAdaptiveRingingModeType {
    VP_ADAPT_RING_SHARED_TRACKER    = 0x0,  /* For shared tracker supplies */
    VP_ADAPT_RING_SHARED_BB_ABS     = 0x1,  /* For Buck Boost ABS supplies */
    VP_ADAPT_RING_SINGLE_BB_TRACKER = 0x2,  /* For Buck Boost single channel tracker supplies */
    VP_ADAPT_RING_FIXED_TRACKER     = 0x3,  /* For all fixed tracking supplies */
    VP_ADAPT_RING_FULL_TRACKER      = 0x4,  /* For non-fixed tracking supplies */
    VP_ADAPT_RING_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpAdaptiveRingingModeType;

typedef struct VpOptionAdaptiveRingingType {
    uint32 validMask;
    uint8 power;
    uint8 minVoltagePercent;
    VpAdaptiveRingingModeType mode;
} VpOptionAdaptiveRingingType;

/* Option values for VP_DEVICE_OPTION_ID_RING_PHASE_SYNC: */
typedef enum VpOptionRingPhaseSyncType {
    VP_RING_PHASE_SYNC_DISABLE = 0,
    VP_RING_PHASE_SYNC_90_OFFSET = 1    /* 90 degree offset between channels */
} VpOptionRingPhaseSyncType;

/* Option values for VP_DEVICE_OPTION_ID_FSYNC_RATE: */
typedef enum VpOptionFsyncRateType {
    VP_FSYNC_RATE_8KHZ = 0,
    VP_FSYNC_RATE_16KHZ = 1
} VpOptionFsyncRateType;

typedef enum {
    VP_ACCESS_CTRL_NORMAL       = 0,    /* Do not block anything */
    VP_ACCESS_CTRL_BLOCK_DEVICE = 1,    /* Block all functions that access the device */

    VP_ACCESS_CTRL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionAccessCtrlType;

typedef enum {
    VP_SPI_ERROR_MODE_BLOCK     = 0,
    VP_SPI_ERROR_MODE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpSpiErrorModeType;

typedef struct {
    VpSpiErrorModeType mode;
    uint16 count;
} VpOptionSpiErrorCtrlType;

typedef struct {
    VpOptionPulseType pulse;                       /* VP_DEVICE_OPTION_ID_PULSE        */
    VpOptionPulseType pulse2;                      /* VP_DEVICE_OPTION_ID_PULSE2       */
    VpOptionCriticalFltType criticalFlt;           /* VP_DEVICE_OPTION_ID_CRITICAL_FLT */
    VpOptionDeviceIoType deviceIo;                 /* VP_DEVICE_OPTION_ID_DEVICE_IO    */
    VpOptionEventMaskType eventMask;               /* VP_OPTION_ID_EVENT_MASK          */
    VpOptionAdaptiveRingingType adaptiveRinging;   /* VP_DEVICE_OPTION_ID_ADAPTIVE_RINGING */
    VpOptionRingPhaseSyncType ringPhaseSync;       /* VP_DEVICE_OPTION_ID_RING_PHASE_SYNC */
    VpOptionFsyncRateType fsyncRate;               /* VP_DEVICE_OPTION_ID_FSYNC_RATE   */
    VpOptionAccessCtrlType accessCtrl;             /* VP_DEVICE_OPTION_ID_ACCESS_CTRL  */
    VpOptionSpiErrorCtrlType spiErrorCtrl;         /* VP_DEVICE_OPTION_ID_SPI_ERROR_CTRL */
} Vp886DevOptionsCacheType;

/* ABS Supply Configurations: Single, Slave, Master */
typedef enum {
    VP886_ABS_SUPP_CFG_SINGLE = 0,
    VP886_ABS_SUPP_CFG_SLAVE  = 1,
    VP886_ABS_SUPP_CFG_MASTER = 2
} Vp886AbsSuppCfgType;
/* Charge Pump protection modes */
typedef enum {
    VP886_CP_PROT_UV_SHUTDOWN = 0,
    VP886_CP_PROT_CYCLE_SKIP = 1,
    VP886_CP_PROT_DISABLED = 2
} Vp886CPProtectionModeType;

/*
 * Vp886DeviceProfileType -------------------------------- Vp886DeviceProfileType
 */
typedef struct {
    uint8   profVersion;
    uint16  pcmClkRate;      /**< Used to verify valid TX/RX Timeslot setting */
    uint8   devCfg;
    uint8   devMode[2];
    uint8   swyv;
    uint8   swzv;
    uint8   swyLimit;
    uint8   swzLimit;
    uint8   io2Use;
    boolean   lowVoltOverride;
    uint8   dialPulseCorrection;
    uint8   swCfg;
    uint8   swOCC[2];
    uint8   blanking;
    Vp886AbsSuppCfgType absSuppCfg;
    uint8   adaptiveRingingMaxPower;
    boolean   cpEnable;
    Vp886CPProtectionModeType cpProtection;
    uint8   vsw;
    int8    vbhOffset;
    int8    vbhOverhead;
    uint8   swCtrlNormal;
    uint8   swCtrlFreeRun;
    uint8   swCtrlBattBackup;
    uint8   batSelGpio;
    boolean    vblUseMedTiming;
} Vp886DeviceProfileType;

typedef struct VpCSLACDeviceProfileTableType {
    VpProfilePtrType pDevProfileTable[VP_CSLAC_DEV_PROF_TABLE_SIZE];
    VpProfilePtrType pAcProfileTable[VP_CSLAC_AC_PROF_TABLE_SIZE];
    VpProfilePtrType pDcProfileTable[VP_CSLAC_DC_PROF_TABLE_SIZE];
    VpProfilePtrType pRingingProfileTable[VP_CSLAC_RINGING_PROF_TABLE_SIZE];
    VpProfilePtrType pToneCadProfileTable[VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE];
    VpProfilePtrType pToneProfileTable[VP_CSLAC_TONE_PROF_TABLE_SIZE];
    VpProfilePtrType pRingingCadProfileTable[VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE];
    VpProfilePtrType pMeteringProfileTable[VP_CSLAC_METERING_PROF_TABLE_SIZE];
    VpProfilePtrType pCallerIdProfileTable[VP_CSLAC_CALLERID_PROF_TABLE_SIZE];
    VpProfilePtrType pFxoConfigProfileTable[VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE];
    VpProfilePtrType pCustomTermProfileTable[VP_CSLAC_CUSTOM_TERM_PROF_TABLE_SIZE];
} VpCSLACDeviceProfileTableType;

/* Vp886InitDeviceSM() state variable.  Setting constant values so that we don't
   need to check debug logs against the exact API version being used. */
typedef enum {
    VP886_INIT_DEVICE_SWITCHER_PREP = 2,
    VP886_INIT_DEVICE_CFAIL_CHECK_PREP = 3,
    VP886_INIT_DEVICE_CFAIL_CHECK = 4,
    VP886_INIT_DEVICE_ZSI_DETECT = 19,
    VP886_INIT_DEVICE_ENABLE_VREF = 5,
    VP886_INIT_DEVICE_VREF_CHECK = 6,
    VP886_INIT_DEVICE_CP_ENABLE = 7,
    VP886_INIT_DEVICE_CH1_CH2_DISC = 9,
    VP886_INIT_DEVICE_CP_CHECK = 8,
    VP886_INIT_DEVICE_CH1_CH2_SW_ENABLE = 10,
    VP886_INIT_DEVICE_CAL_CODEC_START = 13,
    VP886_INIT_DEVICE_CAL_CODEC = 14,
    VP886_INIT_DEVICE_SW_ENABLE = 11,
    VP886_INIT_DEVICE_ALARM_CHECK = 12,
    VP886_INIT_DEVICE_INIT_LINES = 15,
    VP886_INIT_DEVICE_LONG_CAL = 16,
    VP886_INIT_DEVICE_GEN_EVENT = 17,
    VP886_INIT_DEVICE_COMPLETE = 18
} Vp886InitDeviceStateType;

typedef enum {
    VP886_ABS_POWER_REQ_LOW = 0,
    VP886_ABS_POWER_REQ_MED = 1,
    VP886_ABS_POWER_REQ_HIGH = 2,
    VP886_ABS_POWER_REQ_OFF = 3
} Vp886AbsPowerReqType;

/* Vp886CalCodecSM() state variable */
typedef enum {
    VP886_CAL_CODEC_START = 0,
    VP886_CAL_CODEC_PREPARE,
    VP886_CAL_CODEC_SADC,
    VP886_CAL_CODEC_SWY_SENSE,
    VP886_CAL_CODEC_SWZ_SENSE,
    VP886_CAL_CODEC_TIP_SENSE,
    VP886_CAL_CODEC_RING_SENSE,
    VP886_CAL_CODEC_IO2_SENSE,
    VP886_CAL_CODEC_VOC_SENSE,
    VP886_CAL_CODEC_FLOOR,
    VP886_CAL_CODEC_BATTERY,
    VP886_CAL_CODEC_BATTERY_SENSE,
    VP886_CAL_CODEC_BATTERY_SAT,
    VP886_CAL_CODEC_BATTERY_LIMIT,
    VP886_CAL_CODEC_DISCONNECT,
    VP886_CAL_CODEC_LONGITUDINAL,
    VP886_CAL_CODEC_SYNC,
    VP886_CAL_CODEC_CONCLUDE,
    VP886_CAL_CODEC_GEN_EVENT,
    VP886_CAL_CODEC_COMPLETE,
    VP886_CAL_CODEC_FAILED
} Vp886CalCodecStateType;

typedef enum {
    VP_NORMAL,
    VP_POLREV,
    VP_RINGING
} VpPolarityType;

/* Calibration registers scratch-pad */
typedef struct {
    uint8 dcFeed[VP886_R_DCFEED_LEN];
    uint8 sysConfig[VP886_R_SSCFG_LEN];
    uint8 ringGen[VP886_R_RINGGEN_LEN];
    uint8 icr1[VP886_R_ICR1_LEN];
    uint8 icr2[VP886_R_ICR2_LEN];
    uint8 icr3[VP886_R_ICR3_LEN];
    uint8 icr4[VP886_R_ICR4_LEN];
    uint8 icr5[VP886_R_ICR5_LEN];
    uint8 icr6[VP886_R_ICR6_LEN];
    uint8 srp[VP886_R_SWPARAM_LEN];
    uint8 devMode[VP886_R_DEVMODE_LEN];
    uint8 normCal[VP886_R_NORMCAL_LEN];
    uint8 revCal[VP886_R_REVCAL_LEN];
    uint8 indCal[VP886_R_INDCAL_LEN];
    uint8 opCond[VP886_R_OPCOND_LEN];
    uint8 opFunc[VP886_R_OPFUNC_LEN];
    uint8 loopSup[VP886_R_LOOPSUP_LEN];
    uint8 slacState[VP886_R_STATE_LEN];
    uint8 ioDir[VP886_R_IODIR_LEN];
} Vp886RegPadType;


typedef struct {
    int16 gain;
    int16 offset;
} Vp886CalGainOffsetType;

/* Common calibration structure */
typedef struct {
    /* Senses calibration -> accurate ADC readings */
    Vp886CalGainOffsetType sadc;
    Vp886CalGainOffsetType sadcVMode;
    Vp886CalGainOffsetType vadcActive;
    Vp886CalGainOffsetType swySense;
    Vp886CalGainOffsetType swzSense;
    Vp886CalGainOffsetType tipSense;
    Vp886CalGainOffsetType ringSense;
    Vp886CalGainOffsetType io2Sense;
    Vp886CalGainOffsetType vabSenseNormal;
    Vp886CalGainOffsetType vabSenseReverse;
    Vp886CalGainOffsetType vabSenseRinging;

    /* Features calibration -> the device generates accurate signals */
    /* ILA */
    Vp886CalGainOffsetType ilaNormal;
    Vp886CalGainOffsetType ilaReverse;
    /* VOC */
    Vp886CalGainOffsetType vocNormal;
    Vp886CalGainOffsetType vocReverse;
    Vp886CalGainOffsetType vocBuffer;
    /* Ringing */
    Vp886CalGainOffsetType ringingGenerator;
    Vp886CalGainOffsetType ringingBuffer;
    /* VOC, Ringing shared */
    Vp886CalGainOffsetType vocSenseNormal;
    /* Fixed Battery voltage */
    Vp886CalGainOffsetType fixedBat;
    Vp886CalGainOffsetType batterySense;
    /* Longitudinal point */
    Vp886CalGainOffsetType longActive;
    Vp886CalGainOffsetType longRinging;
    Vp886CalGainOffsetType reserved_1;
    Vp886CalGainOffsetType reserved_2;
    /* Hook Detector (1 offset, 4 gains) */
    Vp886CalGainOffsetType hookLPM;
    Vp886CalGainOffsetType hookReverse;
    Vp886CalGainOffsetType hookNormal;
    /* Battery saturation (offset only) */
    Vp886CalGainOffsetType batSat;
    /* Switcher limit */
    Vp886CalGainOffsetType swLimit50V;
    Vp886CalGainOffsetType swLimit100V;
    /* Ground Key detector */
    Vp886CalGainOffsetType gndKeyLong;
    /* Capacitance test tip/ring offsets (pF) */
    int32 tipCapCal;
    int32 ringCapCal;
} Vp886CmnCalDeviceDataType;

/* Tracker calibration structure */
typedef struct {
    /* Tracking battery */
    Vp886CalGainOffsetType trackerVabNormal;
    Vp886CalGainOffsetType trackerVabReverse;
    Vp886CalGainOffsetType trackerVasNormal;
    Vp886CalGainOffsetType trackerVasReverse;
    Vp886CalGainOffsetType reserved_5;
    Vp886CalGainOffsetType reserved_6;
    Vp886CalGainOffsetType trackerBatSense;
} Vp886TrkCalDeviceDataType;

/* ABS calibration structure */
typedef struct {
    Vp886CalGainOffsetType absVabNormal;
    Vp886CalGainOffsetType absVabReverse;
    Vp886CalGainOffsetType reserved_7;
    Vp886CalGainOffsetType reserved_8;
    Vp886CalGainOffsetType reserved_5;
    Vp886CalGainOffsetType reserved_6;
    Vp886CalGainOffsetType absBatSense;
} Vp886AbsCalDeviceDataType;

typedef struct {
    bool valid;
    Vp886CmnCalDeviceDataType cmn;
    union {
        Vp886TrkCalDeviceDataType trk;
        Vp886AbsCalDeviceDataType abs;
    } spe;
} Vp886CalDeviceDataType;

/* CalCodec state variable */
typedef enum {
    VP886_SADC_INIT = 0,
    VP886_SADC_ICAL_L,
    VP886_SADC_ICAL_H,
    VP886_SENSE_INIT,
    VP886_SENSE_ICAL_L,
    VP886_SENSE_ICAL_H,
    VP886_VOC_S_INIT,
    VP886_VOC_S_START,
    VP886_VOC_S_ICAL_L,
    VP886_VOC_S_ICAL_H,
    VP886_FLOOR_INIT,
    VP886_FLOOR_START,
    VP886_FLOOR_LOW,
    VP886_FLOOR_HIGH,
    VP886_TRACKER_INIT,
    VP886_TRACKER_VAB_START,
    VP886_TRACKER_VAB_LOW,
    VP886_TRACKER_VAB_HIGH,
    VP886_TRACKER_VAS_LOW,
    VP886_TRACKER_VAS_HIGH,
    VP886_TRACKER_COMPUTE,
    VP886_BAT_SENSE_INIT,
    VP886_BAT_SENSE_LOW,
    VP886_BAT_SENSE_HIGH,
    VP886_BAT_SAT_INIT,
    VP886_BAT_SAT_MEAS,
    VP886_SW_LIM_INIT,
    VP886_SW_LIM_LOW,
    VP886_SW_LIM_HIGH,
    VP886_LONG_INIT,
    VP886_LONG_GET_MEAS,
    VP886_LONG_GET_MEAS_RINGING
} Vp886CalCodecSubStateType;

/*
 * Vp886DeviceObjectType ------------------------------------------------------ Vp886DeviceObjectType
 */
typedef struct {
    /* Device identifier set by the application -- passed to the HAL */
    VpDeviceIdType deviceId;

    /* Silicon RCN/PCN and number of channels supported */
    VpDeviceStaticInfoType staticInfo;

    /* IO capabilities of this device */
    Vp886IoCapabilityType ioCapability;

    /* Weak attempt to put everything that changes during run-time into a
       single structure. Good idea in principle, but in practice the devices
       are too dis-similar to share many common structures.
       This holds the "last channel checked" event information, clock fault,
       and battery fault. */
    VpDeviceDynamicInfoType dynamicInfo;

    /* Variety of common device level status such as init/not init, init in
       progress, test buffer read y/n?, etc.. Internally used by the VP-API-II
       to communicate device state.
       This is a bit-mask of VpDeviceBusyFlagsType values */
    uint16 busyFlags;

    /* Indicates whether an interrupt has been reported by the system service
       layer.  Used for backward compatibility with VpApiTick/VpVirtualISR */
    boolean pendingInterrupt;

    /* Similar to common device level status, this is for 886 specific type of
       device level information.
       This is a bit-mask of Vp886DeviceStateIntType values */
    uint16 stateInt;

    /* EC value to be used when a device context is passed to Vp886SlacRegWrite,
       Vp886SlacRegRead, or Vp886SlacBufSend.  During normal operation, this
       should be set to VP886_EC_GLOBAL.  The exception is when a routine
       within VpInitDevice() needs to write or read a channel register.  Since
       there is no requirement to have line contexts/objects defined at the time
       of InitDevice, we can't pass a line context to our register write/read
       functions.  To access channel registers, set this value to VP886_EC_1 or
       VP886_EC_2, do the command using the device context, and then set this
       back to VP886_EC_GLOBAL. */
    uint8 ecVal;

    /* Flag indicates that a function which generates results has been called,
       but VpGetResults() has not been called.  The results are not necessarily
       ready, but no more result-generating functions can run.  This does not
       apply to linetest results, which are handled per-line. */
    boolean getResultsRequired;

    /* Union storage for a single basic (non-linetest) result */
    Vp886BasicResultsType basicResults;

    /* Device event queue */
    Vp886EventQueueType eventQueue;

    /* We cache the device timestamp so that we do not end up reading it
       multiple times in quick succession.  We use the flag
       timestampValid so that Vp886GetTimestamp can decide whether to read
       from the device or the cached value.  This flag is set FALSE at the
       beginning/end of each critical section, and TRUE when Vp886GetTimestamp
       reads the register. */
    uint16 timestamp;
    boolean timestampValid;

    /* Count of timestamp rollovers that can be used to support long-duration
       timers.  These two variables are used together to keep the timestamp
       and rollover count in sync.  See Vp886GetTimestamp() for details */
    uint16 rolloverCount;
    int32 rolloverBuffer;

    /* When pushing an event into the event, we may need to force an interrupt
       to prompt the application to call VpGetEvent.  If we are already in
       a VpGetEvent call when pushing the event, there is no need to force the
       interrupt.  This flag is set at the beginning of Vp886GetEvent, and
       cleared at the end. */
    boolean inGetEvent;

    /* Indicates whether we still have a signaling register read remaining to
       be processed in VpGetEvent().  This is useful for determining if we need
       to force an interrupt to update status information when a detector mask
       ends */
    boolean sigregReadPending;

    struct {
        /* Count of SPI errors for VP_DEVICE_OPTION_ID_SPI_ERROR_CTRL. */
        uint16 count;
        /* If the SPI error mode is BLOCK and the count exceeds the specified
           threshold, this flag will be set to TRUE and will block most API
           calls */
        boolean block;
        /* This flag is set when a SPI error is detected to indicate that we
           should send NOOPs to clear the MPI buffer before the next transaction */
        boolean clearMpi;
        /* When VpGetEvent() returns a SPI error event, we want to break out of
           the application-side "while(VpGetEvent==TRUE)" loop.  This flag will
           be set to cause the next VpGetEvent to return FALSE without reading
           the sigreg. */
        boolean skipGetEvent;
    } spiError;

    /* Keep track of critical section depth.  This is currently only used to
       generate warnings if it goes above 1 (indicating nested critical
       sections) or below 0. */
    uint8 criticalDepth;

#define VP886_TIMER_QUEUE_API_SIZE 8
#if defined(VP886_USER_TIMERS)
#define VP886_TIMER_QUEUE_SIZE (VP886_TIMER_QUEUE_API_SIZE + VP886_USER_TIMERS)
#else
#define VP886_TIMER_QUEUE_SIZE (VP886_TIMER_QUEUE_API_SIZE)
#endif
    /* Software timer queue, see vp_timer_queue.c/h */
    VpTimerQueueInfoType timerQueueInfo;
    VpTimerQueueNodeType timerQueueNodes[VP886_TIMER_QUEUE_SIZE];

#if (VP886_USER_TIMERS > 0)
    /* Number of currently running user timers.  Used to implement a limit
       of VP886_USER_TIMERS */
    int16 userTimers;
#endif

    /* Cache for all device option settings */
    Vp886DevOptionsCacheType options;

    /* Soft values from the device profile that are used throughout the VP-API-II
       (not only in Init). Note that other values may be provided by the device
       profile, but are stored in other ways generally because they also may NOT
       be provided in the device profile (i.e., API has default values). */
    Vp886DeviceProfileType devProfileData;

    /* This is the profile pointer table per the VP-API-II User's Guide. */
    VpCSLACDeviceProfileTableType devProfileTable;

    /* This is the structure indicating which profile table entries have been
       initialized. If profile tables are disabled, this and the device profile
       table (devProfileTable) can be removed. */
    uint16 profTableEntryValid[VP886_NUM_PROFILE_TYPES];

    /* Structure to support buffered writes */
    VpSlacBufDataType slacBufData;

    /* Flag to temporarily disable flushing the write buffer before performing
       a read. By default, Vp886SlacRegRead() will send the contents of the
       write buffer before performing the read to prevent issues with assumed
       write->read operations. This flag can be set when it is known that the
       read does not depend on any actions in the write buffer. The flag is set
       back to FALSE on each read. */
    bool dontFlushSlacBufOnRead;

    /* Number of bytes sent/received to/from the device since the last
       VP_QUERY_ID_DEV_TRAFFIC query */
    uint32 trafficBytes;

    /* These switcher parameters are from the user setting in the
       device profile. They should never be changed except in Init (in case
       the values provided are illegal). */
    uint8 swParams[VP886_R_SWPARAM_LEN];

    /* Switching Regulator Timings for normal, Free Run, and battery backup
       modes */
    uint8 swTimingParams[VP886_R_SWTIMING_LEN];
    uint8 swTimingParamsFR[VP886_R_SWTIMING_LEN];
    uint8 swTimingParamsBattBackup[VP886_R_SWTIMING_LEN];

    /* The device profile contains switcher timing parameters for low power
       mode (the first two bytes) that should only be applied if all lines are
       low power termination type.  The flag useLowPowerTiming begins TRUE,
       but is set to FALSE if any non-lowpower line is initialized.  As each
       line is initialized, the switcher timing params are set based on this. */
    bool useLowPowerTiming;
    uint8 swTimingParamsLP[2];

    /* Optionally use different low power params during the initial power supply
       bringup to protect components.  If softStartDuration is 0 this is
       disabled, and we will use the normal params and the normal delay. */
    uint8 softStartDuration; /* 5ms stepsize */
    uint8 swTimingParamsSoftStart[VP886_R_SWTIMING_LEN];

    /* State variable for the Vp886InitDevice state machine. This is used
       inside the Vp886InitDeviceSM function to keep track of the state
       and specify which state (case) of the switch statement to enter. */
    Vp886InitDeviceStateType initDeviceState;

    /* Current ABS power mode requirement for each channel.  This is needed for
       some cases where an operation on one line needs to know the current state
       of the other line.  For example, if one line is exiting ringing, we need
       to know the state of the other line to know whether we can lower the
       switcher power mode. */
    Vp886AbsPowerReqType absPowerReq[VP886_MAX_NUM_CHANNELS];

    /* When TRUE for a channel, indicates that the channel requires ringing
       battery levels.  This will be set by Vp886SetABSRingingBattFlag() each
       time the line state changes. */
    bool absRingingBattRequired[VP886_MAX_NUM_CHANNELS];

    /* Peak ringing voltage (amplitude+bias) for each channel that must be
       supported by the battery voltage */
    uint8 absRingingPeak[VP886_MAX_NUM_CHANNELS];

    /* Used by Adaptive Ringing to see if the other line is power limited */
    bool isPowerLimited[2];

    /* Current switcher voltage settings, which can change on the fly to support
       different ringing voltages for ABS */
    uint8 swyv;
    uint8 swzv;

    /* Indicates which channels are using the sampling timer (ch2 timer) so that
       a line-specific function can know whether it is in use by the other line.
       The value is the required timer setting in 0.5ms steps. */
    uint8 samplingTimerReq[VP886_MAX_NUM_CHANNELS];

    /* The current sampling timer period */
    uint8 samplingTimerCurrent;

    /* Current battery fault status, bitmask of VpBatFltEventDataType values */
    uint16 battFltStatus;

    /* Interval in ms that we are polling to check for battery fault changes */
    uint16 battFltPollTime;

    /* Indicates whether we think there's currently a switcher cycle skip
       condition based on filtered processing of the interrupt */
    bool swSkipPresent;

    /* Timestamps of the most recent overcurrent interrupts, per channel.
       We need to see two interrupts within a defined period to declare an
       actual overcurrent event. */
    uint32 overCurrentTimestamp[VP886_MAX_NUM_CHANNELS];

    struct {
        uint8 vsw;
        uint8 swy;
        int8 vbhOverhead;
    } battBackup;

    /* Keep track of the battery select GPIO state so we only need to write
       it when it changes */
    uint8 batSelGpioState;

    /* State variable for the Vp886CalCodec state machine. This is used
       inside the Vp886CalCodecSM function to keep track of the state
       and specify which state (case) of the switch statement to enter. */
    Vp886CalCodecStateType calCodecState[VP886_MAX_NUM_CHANNELS];

    /* ChannelId of the line under test, 0->line1, 1->line2 */
    bool channelCalBack[VP886_MAX_NUM_CHANNELS];
    bool channelLocked[VP886_MAX_NUM_CHANNELS];

    /* Calibration data */
    int16 icalH;
    int16 icalL;

    VpPolarityType polarity[VP886_MAX_NUM_CHANNELS];

    /* Calibration registers scratch-pad */
    Vp886RegPadType regPad[VP886_MAX_NUM_CHANNELS];

    /* Calibration profile structure */
    Vp886CalDeviceDataType calData[VP886_MAX_NUM_CHANNELS];

    /* CalCodec sub state machine variable */
    Vp886CalCodecSubStateType calCodecSubState[VP886_MAX_NUM_CHANNELS];

    /* Temporary SADC measurements */
    /* Should be channel dependent for concurrent calibration (tmpResultX[VP886_MAX_NUM_CHANNELS]) */
    int16 sadcSetting[VP886_MAX_NUM_CHANNELS];
    int16 sadcSettingGp[VP886_MAX_NUM_CHANNELS][5];
    int16 tmpResultA[VP886_MAX_NUM_CHANNELS];
    int16 tmpResultB[VP886_MAX_NUM_CHANNELS];
    int16 tmpResultC[VP886_MAX_NUM_CHANNELS];
    int16 tmpResultD[VP886_MAX_NUM_CHANNELS];
    int16 tmpResultE[VP886_MAX_NUM_CHANNELS];

    /* Save pointers to the Device, AC, DC, and Ringing profiles.
       These will be used for Vp886InitDevice() only and will only
       be guaranteed valid until the end of the first state of the
       Vp886InitDeviceSM() state machine. They will be used to send down
       the profiles via Vp886ConfigLineInt() before the device bringup
       sequence. */
    VpProfilePtrType pDevProfile;
    VpProfilePtrType pAcProfile;
    VpProfilePtrType pDcProfile;
    VpProfilePtrType pRingProfile;

    /* Switcher mode during InitDevice. Transient value. */
    uint8 initDevSwitcherMode;

    /* Cache for global registers */
    struct {
        uint8 sigreg[VP886_R_SIGREG_LEN];
        uint8 swCtrl[VP886_R_SWCTRL_LEN];
        uint8 swTiming[VP886_R_SWTIMING_LEN];
        uint8 intMask[VP886_R_INTMASK_LEN];
        uint8 devCfg[VP886_R_DEVCFG_LEN];
        uint8 fuse8[VP886_R_FUSE_REG_8_LEN];
    } registers;

#ifdef VP_DEBUG
    /* For runtime enabling of debug output: */
    uint32 debugSelectMask;

#if (VP_CC_DEBUG_SELECT & VP_DBG_WARNING)
    /* Array of function names to help debug nested critical section issues */
    #define VP886_CRITSEC_STRING_NUM 3
    #define VP886_CRITSEC_STRING_LENGTH 20
    char critSecFuncs[VP886_CRITSEC_STRING_NUM][VP886_CRITSEC_STRING_LENGTH+1];
#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_WARNING) */

#endif /* VP_DEBUG */

    bool timerOverride;
} Vp886DeviceObjectType;

class Vp886Device {

public:
    VpStatusType Vp886InitDevice(VpDevCtxType *pDevCtx,
                                 VpProfilePtrType pDevProfile,
                                 VpProfilePtrType pAcProfile,
                                 VpProfilePtrType pDcProfile,
                                 VpProfilePtrType pRingProfile,
                                 VpProfilePtrType pFxoAcProfile,
                                 VpProfilePtrType pFxoCfgProfile);

};

/* IMPORTANT: Make sure to update the "stateInt" member of the device object if
   the size of this type changes. There is no instance of this type itself. */
typedef enum {
    VP886_SYS_CAL_COMPLETE = 0x0001,        /**< Set when the system calibration structure has been initialied */
    /* This value is set when either Device Calibration (VpInitDevice()/VpCalCodec()
       completes successfully, or when loaded values by Apply System Coeff. After
       VpInitDevice() this should only be NOT SET if device calibration failed or
       if null was passed to Apply System Coeff. */
    VP886_DEVICE_CAL_COMPLETE = 0x0002,

    VP886_DEVICE_ICAL_L_IN_USE  = 0x0004,  /* Low calibration current in use */
    VP886_DEVICE_ICAL_H_IN_USE  = 0x0008,  /* High calibration current in use */

    VP886_FORCE_FREE_RUN = 0x0010,         /**< Set when app calls VpFreeRun() (start), cleared when called with stop.
                                             * This prevents the VP-API-II from automatically exiting free run mode
                                             * upon PCLK recovery.
                                             */
    VP886_ZSI_DETECTED = 0x0020,
    VP886_SHARED_SUPPLY = 0x0040,
    VP886_CONVERT_DEVICE_TYPE = 0x0080,
    VP886_FREE_RUN = 0x0100,
    VP886_BATTERY_BACKUP = 0x0200
} Vp886DeviceStateIntType;


/*
 * Vp886LineObjectType -------------------------------------------------------- Vp886LineObjectType
 */
typedef struct {
    uint8 channelId;
    VpLineIdType lineId;
    VpTermType termType;
    uint8 ecVal;

    boolean isFxs;

    /* These flags are used by Vp886ReadyStatus() to decide whether a line can be
       accessed (i.e. is not "busy").  This is a bitmask of
       Vp886LineBusyFlagsType values. */
    uint16 busyFlags;

    //VpApiIntLineStateType lineState;

    /* Cache for all line option settings */
    //Vp886LineOptionsCacheType options;

    /* Set of VP_CSLAC_____ values defining which line conditions we are
       currently ignoring (such as VP_CSLAC_HOOK | VP_CSLAC_GKEY) */
    uint16 detectMasks;

    /* State variable for the Vp886CalLine state machine. This is used
       inside the Vp886CalLineSM function to keep track of the state
       and specify which state (case) of the switch statement to enter. */
    //Vp886CalLineStateType calLineState;
    //Vp886CalLineSubStateType calLineSubState;

    /* Info for the quick recalibration routine. */
    //Vp886QuickCalDataType quickCal;

    /* Remember if the calibration registers have been programmed. They need to
       be cleared before re-running VpCalLine(). */
    boolean calRegsProgrammed;

    /* Vp886GetLoopCond() backups */
    uint16 getLoopCondHandle;
    boolean getLoopCondRegsCached;
    uint8 vadcConvConf[VP886_R_VADC_LEN];
    uint8 getLoopIcr2[VP886_R_ICR2_LEN];
    uint8 getLoopIcr3[VP886_R_ICR3_LEN];
    uint8 getLoopIcr4[VP886_R_ICR4_LEN];

    /* Pulse decoding state data */
    VpPulseDecodeDataType pulseDecodeData;

    /* Line Topology Info */
    VpLineTopologyType lineTopology;

#ifdef VP_CSLAC_SEQ_EN
    /* Sequencer state information */
    Vp886SeqDataType cadence;
    Vp886HowlerStateType howlerState;
    uint16 howlerLevel;

    /* Ringing cadence set by VpInitRing() */
    VpProfilePtrType pRingingCadence;

    /* Caller ID state information */
    Vp886CidDataType cid;

    /* SendSignal state information */
    Vp886SendSignalDataType sendSignal;

    /* Metering state information */
    Vp886MeteringDataType metering;
#endif /* VP_CSLAC_SEQ_EN */

    /* Keeps track of whether the internal test termination is applied */
    boolean intTestTermApplied;

    boolean ringExitInProgress;

    /* State to end up in after the ring exit polrev workaround */
    VpLineStateType ringExitCleanupState;

    //Vp886RingSyncStateType ringSyncState;

    /* Stores the ringing state that was requested but delayed for the channel
       that wants to start ringing. */
    VpLineStateType ringSyncLineState;

    /* Used for VP_OPTION_ID_RINGTRIP_CONFIRM */
    struct {
        boolean confirming;
        boolean glitchCheck;
        uint16 lastHookTimestamp;
    } ringTripConfirm;

#if (VP886_USER_TIMERS > 0)
    /* Number of currently running user timers.  Used to implement a limit
       of VP886_USER_TIMERS */
    int16 userTimers;
#endif

#ifdef VP886_INCLUDE_TESTLINE_CODE
    VpRelayControlType relayState;
#endif

    /* Ringing parameters from the ringing profile, in register format. */
    int16 ringBias;
    int16 ringAmplitude;
    int16 ringAmplitudeCal;
    int16 ringFrequency;
    boolean ringSine;
    int16 fixedRingBat;

    /* Ringing mode: balanced (FALSE) or unbalanced (TRUE) */
    boolean unbalancedRinging;

    /* Ring trip detection algorithm.  Saved from the ringing profile
       so that we can restore it after a msg waiting pulse signal or
       any other potential operation that changes it on the fly */
    uint8 ringTripAlg;

    /* Number of cycles for ring trip (1 or 2), saved so that we can subtract
       it from the ring trip confirm timer */
    uint8 ringTripCycles;

    /* Bit mask of the tones programmed by the most recent tone profile.
       This is used to restrict which tones are enabled in VpSetLineTone() and
       by cadence commands. */
    uint8 toneGens;

    /* Set based on the DC profile, this will determine whether
       groundkey interrupts are interpreted as groundkey events (for
       ground start applications) or DC fault events (loop start). */
    boolean reportDcFaults;

    //Vp886GroundFaultProtDataType gndFltProt;

    /* Floor voltage in 1V steps */
    uint8 floorVoltage;

    /* Target VOC in 1V steps */
    uint8 targetVoc;

    uint16 gxBase;      /**< Cached GX register, in 2.14 int format */
    uint16 gxUserLevel; /**< User specified relative GX level (2.14) */
    uint16 grBase;      /**< Cached GR register, in 2.14 int format */
    uint16 grUserLevel; /**< User specified relative GR level (2.14) */

    /* Most recent non-QUIET settings for the ABS_GAIN option to support
       VP_OPTION_ABS_GAIN_RESTORE. */
    int16 absGxRestoreOption;
    int16 absGrRestoreOption;
    uint8 absGxRestoreReg[VP886_R_GX_LEN];
    uint8 absGrRestoreReg[VP886_R_GR_LEN];

    /* Event results for VpSetRelGain() */
    VpRelGainResultsType setRelGainResults;

    /* Keep track of IO2 status when it's configured as an interrupt */
    boolean io2State;

    /* Set to TRUE if we're using the ila_fdrng bit of ICR2 to enable
       ringing current limits below 50mA.  This changes the way we have to
       read the ringing current limit value for VP_OPTION_ID_RINGING_PARAMS.
       It also changes how the VADC reads current, so ringing power management
       and line test need to be aware of it. */
    boolean lowIlr;

    /* Saved base value of RTTH.  The low ILR workaround will double
       the base value, but will be capped at the max register value, so we
       need to remember the original to be able to restore it. */
    uint8 rtth;

    /* Data for VP_QUERY_ID_SW_DUTY_CYCLE */
    struct {
        /* Prevent it from being run again if it is already in progress */
        boolean inProgress;

        /* Keep track of initial switcher mode, line state, and line conditions
           (hook, gkey, etc) so that we can set the 'stateChanged' flag in the
           results if anything changes */
        uint8 switcherMode;
        VpLineStateType lineState;
        uint16 lineCondition;

        /* A place to store the results so that we can read the switcher on-time
           register in VpGetEvent() rather than waiting for VpGetResults() */
        VpSwDutyCycleType result;
    } swDutyCycle;

    /* Handle for VP_QUERY_ID_TEMPERATURE */
    uint16 queryTempHandle;

    /* Result for VP_QUERY_ID_TEMPERATURE */
    int16 queryTempResult;

    /* Flag and value for mid battery compensation, to set the 60V clamp
       calibration to help with onhook detection when using a mid battery below
       60V (such as 48V) */
    boolean midBatCompEnable;
    uint8 midBatCompValue;

#ifdef VP886_INCLUDE_DTMF_DETECT
    /* Flags and data for DTMF detection */
    Vp886DtmfType dtmf;
#endif /* VP886_INCLUDE_DTMF_DETECT */

#ifdef VP_HIGH_GAIN_MODE_SUPPORTED
    bool inHighGainMode;
    struct {
        uint8 icr1[VP886_R_ICR1_LEN];
        uint8 icr2[VP886_R_ICR2_LEN];
        uint8 icr3[VP886_R_ICR3_LEN];
        uint8 icr4[VP886_R_ICR4_LEN];
        uint8 dcFeed[VP886_R_DCFEED_LEN];
        uint8 vpGain[VP886_R_VPGAIN_LEN];
        uint8 grValue[VP886_R_GR_LEN];
        uint8 rValue[VP886_R_R_FILT_LEN];
        uint8 disn[VP886_R_DISN_LEN];
        uint8 opFunc[VP886_R_OPFUNC_LEN];
        uint8 swParam[VP886_R_SWPARAM_LEN];
    } highGainCache;
#endif /* VP_HIGH_GAIN_MODE_SUPPORTED */

    /* Cache for line-specific registers */
    struct {
        uint8 sysState[VP886_R_STATE_LEN];
        uint8 loopSup[VP886_R_LOOPSUP_LEN];
        uint8 opCond[VP886_R_OPCOND_LEN];
        uint8 opFunc[VP886_R_OPFUNC_LEN];
        uint8 swParam[VP886_R_SWPARAM_LEN];
        uint8 calCtrl[VP886_R_CALCTRL_LEN];
        uint8 icr1[VP886_R_ICR1_LEN];
        uint8 icr2[VP886_R_ICR2_LEN];
        uint8 icr3[VP886_R_ICR3_LEN];
        uint8 icr4[VP886_R_ICR4_LEN];
        uint8 dcFeed[VP886_R_DCFEED_LEN];
        uint8 ssCfg[VP886_R_SSCFG_LEN];
        uint8 indCal[VP886_R_INDCAL_LEN];
        uint8 normCal[VP886_R_NORMCAL_LEN];
        uint8 revCal[VP886_R_REVCAL_LEN];
        uint8 ringCal[VP886_R_RINGCAL_LEN];
        uint8 batCal[VP886_R_BATCAL_LEN];
        uint8 ringDelay[VP886_R_RINGDELAY_LEN];
    } registers;


#ifdef VP886_INCLUDE_TESTLINE_CODE
    /* Testing structure */
    Vp886TestInfoType testInfo;

    /* Test results structure */
    VpTestResultType testResults;
#endif

    /* Setting this flag to TRUE will disable all automatic line state changes
       performed by the API for this line. */
    bool inLineTest;

#ifdef VP886_INCLUDE_ADAPTIVE_RINGING

    /* Data structure for ringing thermal management algorithms */
    RingPowerAdaptModuleType ringPowerAdapt;

    /* Running Thermal Adaptive Ringing mode: */
    bool thermalRinging;

    /* Ringing cadence started */
    bool startRingingCadence;

    /* To detect customer cadenced ringing "standby => ringing -> oht -> ringing..." */
    bool wasStandby;
#endif

#ifdef VP_DEBUG
    /* For runtime enabling of debug output: */
    uint32 debugSelectMask;
#endif /* VP_DEBUG */

    /* Reference ringing battery voltage value requested via ringing profile
       or the VP_OPTION_ID_RINGING_PARAMS option applies to tracking devices only */
    uint8 refRingingBatV;

} Vp886LineObjectType;


/* In vp886_timers.c */
boolean
Vp886InitTimerQueue(
        VpDevCtxType *pDevCtx);

void
Vp886InitEventQueue(
        VpDevCtxType *pDevCtx);

void
VpCSLACClearMPIBuffer(
        VpDeviceIdType deviceId);

VpStatusType
Vp886InitDevicePcnRcn(
        VpDevCtxType *pDevCtx);

/* In vp_886_common.c */
VpStatusType
Vp886GetProfileArg(
        VpDevCtxType *pDevCtx,
        VpProfileType profType,
        VpProfilePtrType *ppProfileArg);

/* Switcher enable wait time in ms */
#define VP886_SW_EN_WAIT_TIME 10

/* Disconnect come up wait time in ms */
#define VP886_DISC_WAIT_TIME 10

/* Charge Pump turn on wait time in ms */
#define VP886_CP_WAIT_TIME 10

/* Vref turn on wait time in ms */
#define VP886_VREF_WAIT_TIME 10

/* CFAIL wait time for clock to settle at Hardware Reset */
#define VP886_CFAIL_WAIT_TIME 10

/* Use this for 'channelId' to Vp886PushEvent() for device events */
#define VP886_DEV_EVENT 255

uint16
Vp886GetTimestamp(
        VpDevCtxType *pDevCtx);


/* Timer ID Definitions (uint16) */
#define VP886_TIMERID_INIT_DEVICE               0
#define VP886_TIMERID_CADENCE                   1
#define VP886_TIMERID_CAL_CODEC                 2
#define VP886_TIMERID_CAL_LINE                  3
#define VP886_TIMERID_HOOK_FREEZE               4
#define VP886_TIMERID_PULSE_DECODE              5
#define VP886_TIMERID_SENDSIGNAL                6
#define VP886_TIMERID_METERING                  7
#define VP886_TIMERID_CID                       8
#define VP886_TIMERID_INT_TEST_TERM             9
#define VP886_TIMERID_POLREV_FIX                10
#define VP886_TIMERID_RING_EXIT                 11
#define VP886_TIMERID_RING_EXIT_CLEANUP         12
#define VP886_TIMERID_USER                      13
#define VP886_TIMERID_HOWLER                    14
#define VP886_TIMERID_ABS_POWER                 15
#define VP886_TIMERID_LINE_TEST                 16
#define VP886_TIMERID_THERMAL_RINGING           17
#define VP886_TIMERID_BATFLT_POLL               18
#define VP886_TIMERID_QUICKCAL                  19
#define VP886_TIMERID_GKEY_FREEZE               20
#define VP886_TIMERID_GND_FLT_PROT              21
#define VP886_TIMERID_GROUNDSTART               22
#define VP886_TIMERID_RINGTRIP_CONFIRM          23
#define VP886_TIMERID_SWSKIP_CHECK              24
#define VP886_TIMERID_SWSKIP_CLEAR              25
#define VP886_TIMERID_QUERY_SW_DUTY_CYCLE       26

bool
Vp886PushEvent(
        VpDevCtxType *pDevCtx,
        uint8 channelId,
        VpEventCategoryType eventCategory,
        uint16 eventId,
        uint16 eventData,
        uint16 parmHandle,
        boolean hasResults);

boolean
Vp886PopEvent(
        VpDevCtxType *pDevCtx,
        VpEventType *pEvent);

boolean
Vp886AddTimerMs(
        VpDevCtxType *pDevCtx,
        VpLineCtxType *pLineCtx,
        uint16 timerId,
        uint32 duration,
        uint32 overrun,
        uint32 handle);


#endif //TELLUINO_CONTROLLER_VP886_DEVICE_H
