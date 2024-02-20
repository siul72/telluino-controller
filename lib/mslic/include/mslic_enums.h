//
// Created by siul72 on 20.02.2024.
//

#ifndef TELLUINO_CONTROLLER_MSLIC_ENUMS_H
#define TELLUINO_CONTROLLER_MSLIC_ENUMS_H

#include "limits.h"

#define FORCE_STANDARD_C_ENUM_SIZE  (INT_MAX)

/******************************************************************************
 *                    ENUMERATIONS AND NEW DATA TYPES                         *
 ******************************************************************************/
/** Standard return value for most API library functions.  See the VP-API-II
    Reference Guide for descriptions. */
typedef enum VpStatusType {
    VP_STATUS_SUCCESS              = 0,
    VP_STATUS_FAILURE              = 1,
    VP_STATUS_FUNC_NOT_SUPPORTED   = 2,
    VP_STATUS_INVALID_ARG          = 3,
    VP_STATUS_MAILBOX_BUSY         = 4,
    VP_STATUS_ERR_VTD_CODE         = 5,
    VP_STATUS_OPTION_NOT_SUPPORTED = 6,
    VP_STATUS_ERR_VERIFY           = 7,
    VP_STATUS_DEVICE_BUSY          = 8,
    VP_STATUS_MAILBOX_EMPTY        = 9,
    VP_STATUS_ERR_MAILBOX_DATA     = 10,
    VP_STATUS_ERR_HBI              = 11,
    VP_STATUS_ERR_IMAGE            = 12,
    VP_STATUS_IN_CRTCL_SECTN       = 13,
    VP_STATUS_DEV_NOT_INITIALIZED  = 14,
    VP_STATUS_ERR_PROFILE          = 15,
    VP_STATUS_CUSTOM_TERM_NOT_CFG  = 17,
    VP_STATUS_DEDICATED_PINS       = 18,
    VP_STATUS_INVALID_LINE         = 19,
    VP_STATUS_LINE_NOT_CONFIG      = 20,
    VP_STATUS_ERR_SPI              = 21,
    VP_STATUS_ACCESS_BLOCKED       = 22,
    VP_STATUS_INPUT_PARAM_OOR,  /* This enum value does not yet require to be a set value */
    VP_STATUS_NUM_TYPES,
    VP_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpStatusType;

/******************************************************************************
 *                                ENUMERATIONS                                *
 ******************************************************************************/
/* Valid Device Types supported by VP-API-II. For the corresponding part numbers
 * supported by each device type please see VP-API-II documentation.
 */
typedef enum VpDeviceType {
    VP_DEV_RSRVD1 = 0,
    VP_DEV_580_SERIES,
    VP_DEV_790_SERIES,
    VP_DEV_VCP_SERIES,
    VP_DEV_880_SERIES = 4,  /* Fixed value to match calibration profile */
    VP_DEV_RSVD_SERIES,
    VP_DEV_VCP2_SERIES,
    VP_DEV_KWRAP,
    VP_DEV_890_SERIES = 8,  /* Fixed value to match calibration profile */
    VP_DEV_792_SERIES,
    VP_DEV_MELT_SERIES,
    VP_DEV_792_GROUP,
    VP_DEV_886_SERIES = 12,
    VP_DEV_887_SERIES = 13,
    VP_DEV_MELT792_SERIES = 14,
    VP_NUM_DEV_TYPES,
    VP_DEV_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceType;

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

#endif //TELLUINO_CONTROLLER_MSLIC_ENUMS_H
