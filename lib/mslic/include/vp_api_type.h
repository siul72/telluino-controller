//
// Created by siul72 on 10.02.2024.
//

#ifndef TELLUINO_CONTROLLER_VP_API_TYPE_H
#define TELLUINO_CONTROLLER_VP_API_TYPE_H

#include "limits.h"

#ifndef NULL
#define NULL (0)
#endif
#define VP_NULL NULL
#ifdef EXTERN
#undef EXTERN
  #error EXTERN was redefined!
#endif /* undef EXTERN */

#define EXTERN extern "C"

/********************* DECLARATIONS ***************************/
/* Constants */
#define FALSE   (0)     /* Boolean constant */
#define TRUE    (1)     /* Boolean constant */
#ifndef __cplusplus
/* C++ language provides a boolean data type; So no need to define
 * one more data type; Make use of it
 * NOTE: The 'C' potions of the VP-API assume C++ "bool" to be of the
 * same size as that of "char". Please make sure this assumption is correct.
 */
//typedef unsigned char bool;
#endif /* __cplusplus */

typedef unsigned char   uchar;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int   uint32;
typedef signed char    int8;
typedef signed short int int16;
typedef signed int  int32;
typedef unsigned char VpProfileDataType;
typedef const VpProfileDataType * VpProfilePtrType;
typedef uint16 * VpVectorPtrType;

#define FORCE_STANDARD_C_ENUM_SIZE  (INT_MAX)
#define FORCE_SIGNED_ENUM  (INT_MIN)

/* VpDeviceIdType defines the type for the deviceId in the VpDevCtxType type.
 * This information is passed through the API to the HAL to communicate
 * with a specific device.  The values assigned via VpMakeDeviceObject()
 * are user defined and may be simple device indexing (0, 1, .. (n-1)):
 * where n = device number in system
 */
typedef int VpDeviceIdType;

/*
 * The 'VpLineIdType'  defines a system wide Line identification that the system
 * could use to identify a line. This type can be defined to contain anything
 * that the customer chooses. It could be defined to contain just an index or
 * a pointer. The system wide line identity could be set using the
 * VpMapLineId() function for a given line. The VP-API returns this line id
 * information when line specific events occur (along with the event).
 */
typedef unsigned char VpLineIdType;

/** Valid Termination Types (per line) supported by API-II.  See the VP-API-II
    Reference Guide for descriptions. */
typedef enum VpTermType {
    /* FXS */
    VP_TERM_FXS_GENERIC            = 0x00, VP_TERM_FXS_FIRST = VP_TERM_FXS_GENERIC,
    VP_TERM_FXS_ISOLATE            = 0x01,
    VP_TERM_FXS_TITO_TL_R          = 0x02,
    VP_TERM_FXS_CO_TL              = 0x03,
    VP_TERM_FXS_75181              = 0x04,
    VP_TERM_FXS_75282              = 0x05,
    VP_TERM_FXS_RDT                = 0x06,
    VP_TERM_FXS_RR                 = 0x07,
    VP_TERM_FXS_SPLITTER           = 0x08,
    VP_TERM_FXS_TO_TL              = 0x09,
    VP_TERM_FXS_LOW_PWR            = 0x0A,
    VP_TERM_FXS_TI                 = 0x0B,
    VP_TERM_FXS_SPLITTER_LP        = 0x0C,
    VP_TERM_FXS_ISOLATE_LP         = 0x0D,
    VP_TERM_FXS_LCAS               = 0x0E,
    VP_TERM_FXS_RR_TI              = 0x0F,
    VP_TERM_FXS_CUSTOM             = 0x10,
    VP_TERM_FXS_TL_MW              = 0x11,
    VP_TERM_FXS_RR_MW              = 0x12, VP_TERM_FXS_LAST = VP_TERM_FXS_RR_MW,

    /* MeLT */
    VP_TERM_MELT_RELAY_CAL_CT      = 0x40, VP_TERM_MELT_FIRST = VP_TERM_MELT_RELAY_CAL_CT,
    VP_TERM_MELT_RELAY_CAL_SPLIT   = 0x41,
    VP_TERM_MELT_RELAY_CAL_BRIDGED = 0x42,
    VP_TERM_MELT_LTAS_CT           = 0x43,
    VP_TERM_MELT_RELAY_CT          = 0x44,
    VP_TERM_MELT_RELAY_SPLIT       = 0x45,
    VP_TERM_MELT_RELAY_BRIDGED     = 0x46,
    VP_TERM_MELT_LTAS_BRIDGED      = 0x47, VP_TERM_MELT_LAST = VP_TERM_MELT_LTAS_BRIDGED,

    /* FXO */
    VP_TERM_FXO_GENERIC            = 0x80, VP_TERM_FXO_FIRST = VP_TERM_FXO_GENERIC,
    VP_TERM_FXO_DISC               = 0x81,
    VP_TERM_FXO_CUSTOM             = 0x82, VP_TERM_FXO_LAST = VP_TERM_FXO_CUSTOM,

    VP_TERM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpTermType;

/** Ring control option */
typedef enum VpLineStateType {

    /* FXS */
    VP_LINE_STANDBY         = 0x00, /**< Low power line feed state */
    VP_LINE_TIP_OPEN        = 0x01, /**< Tip open circuit state */
    VP_LINE_ACTIVE          = 0x02, /**< Line Feed w/out VF */
    VP_LINE_ACTIVE_POLREV   = 0x03, /**< Polarity Reversal Line Feed w/out VF */
    VP_LINE_TALK            = 0x04, /**< Normal off-hook Active State; Voice Enabled */
    VP_LINE_TALK_POLREV     = 0x05, /**< Normal Active with reverse polarity; Voice Enabled */
    VP_LINE_OHT             = 0x06, /**< On-Hook tranmission state */
    VP_LINE_OHT_POLREV      = 0x07, /**< Polarity Reversal On-Hook tranmission state */
    VP_LINE_DISCONNECT      = 0x08, /**< Denial of service */
    VP_LINE_RINGING         = 0x09, /**< Ringing state */
    VP_LINE_RINGING_POLREV  = 0x0A, /**< Ringing w/Polarity Reversal */
    VP_LINE_STANDBY_POLREV  = 0x10, /**< Low power polrev line feed state */
    VP_LINE_PARK            = 0x11, /**< Park mode */
    VP_LINE_RING_OPEN       = 0x12, /**< Ring open */
    VP_LINE_HOWLER          = 0x13, /**< Howler */
    VP_LINE_TESTING         = 0x14, /**< Testing */
    VP_LINE_DISABLED        = 0x15, /**< Disabled */
    VP_LINE_NULLFEED        = 0x16, /**< Null-feed */
    VP_LINE_HOWLER_PCM      = 0x17, /**< Howler with PCM highway enabled */
    VP_LINE_HOWLER_POLREV   = 0x18, /**< Howler with Polarity Reversal */

    /* FXO */
    VP_LINE_FXO_OHT,        /**< FXO Line providing Loop Open w/VF */
    VP_LINE_FXO_LOOP_OPEN,  /**< FXO Line providing Loop Open w/out VF */
    VP_LINE_FXO_LOOP_CLOSE, /**< FXO Line providing Loop Close w/out VF */
    VP_LINE_FXO_TALK,       /**< FXO Line providing Loop Close w/VF */
    VP_LINE_FXO_RING_GND,   /**< FXO Line providing Ring Ground (GS only)*/

    VP_NUM_LINE_STATES,
    VP_LINE_STATE_ENUM_RSVD = FORCE_SIGNED_ENUM,
    VP_LINE_STATE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLineStateType;

/* Option IDs.  (See Options chapter in VP-API-2 Reference Guide.)
 * Line-specific option IDs begin with "VP_OPTION_ID_".  Device-specific
 * option IDs begin with "VP_DEVICE_OPTION_ID_".  When new option IDs are added,
 * the VpOptionValueType struct (below) must be updated accordingly.
 */
typedef enum VpOptionIdType {
    VP_DEVICE_OPTION_ID_PULSE            = 0x00,
    VP_DEVICE_OPTION_ID_CRITICAL_FLT     = 0x01,
    VP_OPTION_ID_ZERO_CROSS              = 0x02,
    VP_DEVICE_OPTION_ID_RAMP2STBY        = 0x03,
    VP_OPTION_ID_PULSE_MODE              = 0x04,
    VP_OPTION_ID_TIMESLOT                = 0x05,
    VP_OPTION_ID_CODEC                   = 0x06,
    VP_OPTION_ID_PCM_HWY                 = 0x07,
    VP_OPTION_ID_LOOPBACK                = 0x08,
    VP_OPTION_ID_LINE_STATE              = 0x09,
    VP_OPTION_ID_EVENT_MASK              = 0x0A,
    VP_OPTION_ID_RESERVED_1              = 0x0B,
    VP_OPTION_ID_RING_CNTRL              = 0x0C,
    VP_OPTION_ID_RESERVED_2              = 0x0D,
    VP_OPTION_ID_DTMF_MODE               = 0x0E,
    VP_DEVICE_OPTION_ID_DEVICE_IO        = 0x0F,
    VP_OPTION_ID_RESERVED_EVENT_MASK_VCP = 0x10,
    VP_OPTION_ID_PCM_TXRX_CNTRL          = 0x11,
    VP_DEVICE_OPTION_ID_PULSE2           = 0x12,
    VP_OPTION_ID_LINE_IO_CFG             = 0x13,
    VP_DEVICE_OPTION_ID_DEV_IO_CFG       = 0x14,
    VP_OPTION_ID_DTMF_SPEC               = 0x15,
    VP_DEVICE_OPTION_ID_PARK_MODE        = 0x16,
    VP_OPTION_ID_DCFEED_SLOPE            = 0x17,
    VP_OPTION_ID_SWITCHER_CTRL           = 0x18,
    VP_OPTION_ID_HOOK_DETECT_MODE        = 0x19,
    VP_OPTION_ID_AUTO_LOOP_COND          = 0x1A,
    VP_OPTION_ID_DCFEED_PARAMS           = 0x1B,
    VP_OPTION_ID_RINGING_PARAMS          = 0x1C,
    VP_OPTION_ID_GND_FLT_PROTECTION      = 0x1D,
    VP_DEVICE_OPTION_ID_ADAPTIVE_RINGING = 0x1E,
    VP_OPTION_ID_RINGTRIP_CONFIRM        = 0x1F,
    VP_DEVICE_OPTION_ID_RING_PHASE_SYNC  = 0x20,
    VP_OPTION_ID_HIGHPASS_FILTER         = 0x21,
    VP_DEVICE_OPTION_ID_FSYNC_RATE       = 0x22,
    VP_OPTION_ID_DTMF_PARAMS             = 0x23,
    VP_OPTION_ID_PULSE                   = 0x24,
    VP_OPTION_ID_DEBUG_SELECT            = 0x25,
    VP_OPTION_ID_ABS_GAIN                = 0x26,
    VP_DEVICE_OPTION_ID_PCM_SIG_CTL      = 0x27,
    VP_OPTION_ID_LINESTATE_CTL_MODE      = 0x28,
    VP_DEVICE_OPTION_ID_SEALING_CURRENT  = 0x29,
    VP_DEVICE_OPTION_ID_ACCESS_CTRL      = 0x2A,
    VP_DEVICE_OPTION_ID_SPI_ERROR_CTRL   = 0x2B,

    VP_NUM_OPTION_IDS
} VpOptionIdType;

/** Tests supported by VpTestLine function */
typedef enum VpTestIdType {
    VP_TEST_ID_PREPARE        = 0x00, /* Prepare for a test sequence */
    VP_TEST_ID_CONCLUDE       = 0x01, /* Conclude a test sequence */
    VP_TEST_ID_SET_SENSE_GAIN = 0x02, /* Set ILG and IMT gains high or low */
    VP_TEST_ID_OPEN_VDC       = 0x03, /* Measure open DC voltage */
    VP_TEST_ID_OPEN_VAC       = 0x04, /* Measure open AC voltage */
    VP_TEST_ID_DIFF_VAC       = 0x05, /* Measure differential AC voltage */
    VP_TEST_ID_DC_RLOOP       = 0x06, /* Measure DC loop resistance */
    VP_TEST_ID_AC_RLOOP       = 0x07, /* Measure AC loop impedance */
    VP_TEST_ID_NULL_IGND      = 0x08, /* Null ground current */
    VP_TEST_ID_3ELE_RES       = 0x09, /* Measure 3-element insulation resistance */
    VP_TEST_ID_3ELE_CAP       = 0x0A, /* Measure 3-element capacitance */
    VP_TEST_ID_IDC            = 0x0B, /* Measure DC currents */
    VP_TEST_ID_IAC            = 0x0C, /* Measure AC currents */
    VP_TEST_ID_KEYPAD         = 0x0D, /* Keypad DTMF and Pulse dial test */
    VP_TEST_ID_NOISE          = 0x0E, /* Measure Active or Passive Noise 3, 3.4 & 15KhZ */
    VP_TEST_ID_SNR_QDIST      = 0x0F, /* Measure SNR and quantization distortion */
    VP_TEST_ID_ARB_1TONE      = 0x10, /* Measure arbitrary single tone */
    VP_TEST_ID_TONE_GEN       = 0x11, /* Generate a single/dual/quad-tone */
    VP_TEST_ID_DIALTONE       = 0x12, /* Detect and measure dial tone (Inward test) */
    VP_TEST_ID_HYBRID_LOSS    = 0x13, /* Measure trans-hybrid loss */
    VP_TEST_ID_ALT_REN        = 0x14, /* Alternative method used to measure REN */
    VP_TEST_ID_HOOK_STATUS    = 0x15, /* Check Hook Status (while in test mode) */
    VP_TEST_ID_PCM_COLLECT    = 0x16, /* Collect PCM samples */
    VP_TEST_ID_AC_RLOOP_PHASE = 0x17, /* Rloop with Phase information */
    VP_TEST_ID_SET_BATTERIES  = 0x18, /* Select batteries for line testing */
    VP_TEST_ID_MONITOR_IV     = 0x19, /* Non-Intrusive Monitoring Test */
    VP_TEST_ID_UNBAL_TONE     = 0x1A, /* Generates an unbalanced tracing tone */
    VP_TEST_ID_3ELE_RES_VMID  = 0x1B, /* Measure 3-element insulation resistance with a specified vMid */
    VP_TEST_ID_AC_TRANS       = 0x1C, /* Measures digital-digital transmission characteristics */
    VP_TEST_ID_GEN_TEST       = 0x1D, /* Multi-functional primitive */
    VP_TEST_ID_SLOPE_REN      = 0x1E, /* REN test with a ramp instead of step */
    /* Test IDs above this line are supported by one or more VCP devices.
       Their values must not change.  More VCP tests might be added above this
       line in the future. */

    VP_TEST_ID_RSVD_MIN       = 0x23, /* Custom VCP tests (not currently used) */
    VP_TEST_ID_RSVD_MAX       = 0x23,

    /* The values of the constants below this line can change from time to time. */

    VP_TEST_ID_3ELE_RES_HG,     /* Alternative 3-element resistance high gain */
    VP_TEST_ID_3ELE_RES_LG,     /* Alternative 3-element resistance low gain */
    VP_TEST_ID_3ELE_CAP_CSLAC,  /* CSLAC 3-element capacitance test */
    VP_TEST_ID_MSOCKET_TYPE2,   /* Master Socket test for UK termination */
    VP_TEST_ID_XCONNECT,        /* Cross Connect detection test */
    VP_TEST_ID_LOOP_CONDITIONS, /* Measure current loop conditions */
    VP_TEST_ID_LOOPBACK,        /* Setup loopback conditions */
    VP_TEST_ID_LINE_IMPD,       /* Measure impedance at a frequency */
    VP_TEST_ID_RING_TRIP,       /* Test ability to detect ring trip */
    VP_TEST_ID_LOOP_DETECT,     /* Test ability to src current and detect
                                 * off-hook */
    VP_TEST_ID_CALIBRATE,       /* Perform line calibration */
    VP_TEST_ID_USE_LINE_CAL,    /* Copy dev cal factors from lineObj to devObj */
    VP_TEST_ID_OPEN_VXC,        /* Measure both DC and AC voltages */

    VP_TEST_ID_GR909_HEMF,      /* Hazardous Potential Voltage Test for AC/DC
                                 * Tip and Ring */
    VP_TEST_ID_GR909_EMF,       /* FEMF test for AC/DC Tip and Ring */
    VP_TEST_ID_GR909_RES_FLT,   /* Resistive Faults (T/R, T-Gnd, R-Gnd) */
    VP_TEST_ID_GR909_OFF_HOOK,  /* Receiver Off-Hook */
    VP_TEST_ID_GR909_RINGERS,   /* REN per FCC part 68 (1REN = 7Kohm) */
    VP_TEST_ID_GR909_ALL,       /* Run all GR909 tests */
    VP_TEST_ID_DELAY,           /* Implements non-blocking delay */
    VP_TEST_ID_RAMP_INIT,       /* Implements a ramp to voltage test init */
    VP_TEST_ID_RAMP,            /* Implements a ramp to voltage test */
    VP_TEST_ID_FLT_DSCRM,       /* Implements the Fault discrimination test (880 890 only)*/
    VP_TEST_ID_PREPARE_EXT,     /* Req'd for backward compatibility with
                                 * VeriVoice P1.7.2 and earlier */
    VP_TEST_ID_SET_SENSE_GAIN_792, /* Set Low/Normal/High sense gain for VCP792.
                                    * The API translates this to the value of
                                    * VP_TEST_ID_SET_SENSE_GAIN before sending
                                    * it down to the VCP. */
    VP_TEST_ID_VP_MPI_CMD,      /* Primitive to allow for direct regsiter
                                    * reads / writes similar to  LowLevel
                                    * Command */
    VP_TEST_ID_GAIN_TLBX,       /* Primitive to allow for direct gain measurements */
    VP_TEST_ID_LOOP_CONDITIONS_EXT, /* Measure extended loop conditions */
    VP_TEST_ID_DRAW_BREAK_DIALTONE, /* Draw and Break Dialtone test*/
    VP_TEST_ID_ADC_TLBX,        /* Primitive allowing direct access to the ZL880/miSLIC
                                    * SADC measurement toolbox */
    VP_TEST_ID_PCM4,            /* PCM-4 style measurements */

    VP_NUM_TEST_IDS,

    VP_TEST_ID_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpTestIdType;



#endif //TELLUINO_CONTROLLER_VP_API_TYPE_H
