//
// Created by siul72 on 11.02.2024.
//

#ifndef TELLUINO_CONTROLLER_VP_TYPES_COMMON_H
#define TELLUINO_CONTROLLER_VP_TYPES_COMMON_H

#define VP_CSLAC_DEV_PROF_TABLE_SIZE            1
#define VP_CSLAC_AC_PROF_TABLE_SIZE             2
#define VP_CSLAC_DC_PROF_TABLE_SIZE             2
#define VP_CSLAC_RINGING_PROF_TABLE_SIZE        2
#define VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE   11
#define VP_CSLAC_TONE_PROF_TABLE_SIZE           10
#define VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE   4
#define VP_CSLAC_METERING_PROF_TABLE_SIZE       2
#define VP_CSLAC_CALLERID_PROF_TABLE_SIZE       2
#define VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE     1
#define VP_CSLAC_CUSTOM_TERM_PROF_TABLE_SIZE    4

#define VP_CSLAC_MAX_RCN_PCN_SIZE   2
typedef struct VpDeviceStaticInfoType {
    uint8 rcnPcn[VP_CSLAC_MAX_RCN_PCN_SIZE];    /**< Revision Code Number and
                                                 * Product Code Number as
                                                 * applicable
                                                 */
    uint8 maxChannels;  /**< How many lines supported by this device */
} VpDeviceStaticInfoType;

typedef struct VpDeviceDynamicInfoType {
    uint8 lastChan;     /**< The last line checked */
    bool clkFault;      /** TRUE if a clock fault is active */
    bool bat1Fault;     /** TRUE if a bat1 fault is active and device level */
    bool bat2Fault;     /** TRUE if a bat2 fault is active and device level */
    bool bat3Fault;     /** TRUE if a bat3 fault is active and device level */
} VpDeviceDynamicInfoType;

/**<
 * IMPORTANT: Make sure to update the type used for "state" member inside
 * objects, structs, and passed to functions where the comment states the value
 * is a bit-mask of "VpDeviceBusyFlagsType". Seach by "VpDeviceBusyFlagsType".
 * There are no instance of this type itself.
 */
typedef enum VpDeviceBusyFlagsType {
    /**< Indicates if function is running from InitDevice. Not the same as in
     * the middle of an initialization process (including calibration). Used to
     * allow InitDevice function calling other functions to be enabled. Otherwise,
     * some functions will return "Device Not Initialized" error.
     */
    VP_DEV_INIT_IN_PROGRESS = 0x0001,

    VP_DEV_INIT_CMP = 0x0002,    /**< Set if device has been initialized */
    VP_DEV_IN_CAL = 0x0004,      /**< Set if device is busy calibrating */
    VP_DEV_PENDING_INT = 0x0008, /**< Set if there is a pending interrupt */

    VP_DEV_ABS_BAT_CAL = 0x0010, /**< Set when running ABS (batt switch) Cal */
    VP_DEV_ABV_CAL = 0x0020,     /**< Set when running ABV Calibration on Tracker */
    VP_DEV_ABV_CAL_ABS = 0x0040, /**< Set when running ABV Calibration on ABS */
    VP_DEV_DISC_PENDING = 0x0080, /**< Set when a Disconnect Timer was suspsended */

    VP_DEV_TEST_BUFFER_READ = 0x0100, /**< Set if test buffer was read this tick */

    /**< Set if device in process of warm reboot. Note that warm reboot
     * detection alone only helps skipping of VpCalCodec() because it is run
     * with VpInitDevice(). But it does not ensure that VpCalLine() was
     * previously run.
     */
    VP_DEV_WARM_REBOOT = 0x0200,

    /* Set while performing VpImplementDefaultSettings().  This can be used to
       avoid entering nested critical sections, or to take other necessary
       precautions due to internally calling the top level VpSetOption(). */
    VP_DEV_IMPL_DEFAULT_OPTIONS = 0x0400,

    VP_TEMP_IGNORE_ALL_BUSY_FLAGS = 0x4000  /* Set to temporarily ignore all other flags */

} VpDeviceBusyFlagsType;

#endif //TELLUINO_CONTROLLER_VP_TYPES_COMMON_H
