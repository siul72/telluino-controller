//
// Created by siul72 on 10.02.2024.
//

#include <Arduino.h>
#include "mslic_api.h"
#include "mslic_registers.h"

#define EC_WRT_CMD 0x4a

MicrochipSlicApi::MicrochipSlicApi(){
   this->spi = new SlicSpi();
   this->spi->setup();
}

MicrochipSlicApi::~MicrochipSlicApi(){
    delete this->spi;
}

/**
 * VpMakeDeviceObject()
 *  This function creates a device context using the information that is
 * provided. This funciton should be the first API function that should be
 * called. This function is like C++ constructor. If the passed device type is
 * not valid or the code for the device type is not compiled in, this function
 * returns error. Please see VP-API documentation for more information.
 *
 * Preconditions:
 *  The device context, device object pointers must be non zero and device type
 * must be valid. The type of device object should match with device type. The
 * deviceId must uniquely determine a chipselect for the device of interest in
 * HAL layer.
 *
 * Postconditions:
 *  The device context and device object are initialized and this function
 * returns success if context is created properly.
 */
VpStatusType MicrochipSlicApi::VpMakeDeviceObject(
        VpDeviceType deviceType,    /**< Device Type */
        VpDeviceIdType deviceId,    /**< Hardware chip select for this device */
        void *pDevCtx,      /**< Device Context to be initialized by other
                                 * input
                                 */
        void *pDevObj)              /**< Device Object to be pointed to by device
                                 * context
                                 */
{
    uint8_t i;
    void *funcPtr;
    VpStatusType status = VP_STATUS_SUCCESS;
    //VP_API_ENTER(None, VP_NULL, "MakeDeviceObject");

    /* Basic argument checking */
    if ((pDevObj == nullptr) || (pDevCtx == nullptr)) {
        //VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", VP_STATUS_INVALID_ARG);
        return VP_STATUS_INVALID_ARG;
    } else if ((deviceType != VP_DEV_VCP_SERIES) &&
               (deviceType != VP_DEV_VCP2_SERIES) &&
               (deviceType != VP_DEV_880_SERIES) &&
               (deviceType != VP_DEV_886_SERIES) &&
               (deviceType != VP_DEV_887_SERIES) &&
               (deviceType != VP_DEV_890_SERIES) &&
               (deviceType != VP_DEV_792_SERIES) &&
               (deviceType != VP_DEV_792_GROUP) &&
               (deviceType != VP_DEV_MELT_SERIES) &&
               (deviceType != VP_DEV_MELT792_SERIES) &&
               (deviceType != VP_DEV_KWRAP)) {
        //VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", VP_STATUS_ERR_VTD_CODE);
        return VP_STATUS_ERR_VTD_CODE;
    }


    //VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    //pDevCtx->deviceType = deviceType;
    //status = VpMakeVp886DeviceObject(pDevCtx, pDevObj);
    //VpMemSet(pDevObj, 0, sizeof(Vp886DeviceObjectType));
    //pDevObj->staticInfo.maxChannels = VP886_MAX_NUM_CHANNELS;
    //Vp886SetDefaultCal(pDevObj, 0);
    //Vp886SetDefaultCal(pDevObj, 1);
//    pDevObj->calData[0].valid = FALSE;
//    pDevObj->calData[1].valid = FALSE;
//    pDevObj->isPowerLimited[0] = FALSE;
//    pDevObj->isPowerLimited[1] = FALSE;
//    pDevObj->ecVal = VP886_EC_GLOBAL;
//    pDevObj->slacBufData.buffering = FALSE;
//    pDevObj->options.accessCtrl = VP_ACCESS_CTRL_NORMAL;
 //   pDevObj->timerOverride = FALSE;
    uint8_t channelCount, maxChan;
    void *pFunc;


    /* Initialize Device context */
    //pDevCtx->pDevObj = pDevObj;

    /* Initialize all of the line context pointers to null in the device context */
//    maxChan = pDevObj->staticInfo.maxChannels;
//    for (channelCount = 0; channelCount < maxChan; channelCount++) {
//        pDevCtx->pLineCtx[channelCount] = nullptr;
//    }

//    pFunc = &pDevCtx->funPtrsToApiFuncs;
//
//    /* Init functions (vp886_init.c) */
//    pFunc->MakeLineObject = Vp886MakeLineObject;
//    pFunc->InitDevice = Vp886InitDevice;
//    pFunc->InitLine = Vp886InitLine;
//    pFunc->ConfigLine = Vp886ConfigLine;
//    pFunc->InitProfile = Vp886InitProfile;

    /* Control functions (vp886_control.c) */
//    pFunc->SetLineState = Vp886SetLineState;
//    pFunc->SetLineTone = Vp886SetLineTone;
//    pFunc->SetOption = Vp886SetOption;
//    pFunc->DeviceIoAccess = Vp886DeviceIoAccess;
//    pFunc->LineIoAccess = Vp886LineIoAccess;
//    pFunc->SetRelayState = Vp886SetRelayState;
//    pFunc->DtmfDigitDetected = Vp886DtmfDigitDetected;
//    pFunc->FreeRun = Vp886FreeRun;
//    pFunc->BatteryBackupMode = Vp886BatteryBackupMode;
//    pFunc->ShutdownDevice = Vp886ShutdownDevice;
//    /* Query functions (vp886_query.c) */
//    pFunc->GetLineStatus = VpCSLACGetLineStatus;
//    pFunc->GetDeviceStatus = Vp886GetDeviceStatus;
//    pFunc->GetDeviceInfo = Vp886GetDeviceInfo;
//    pFunc->GetOption = Vp886GetOption;
//    pFunc->GetOptionImmediate = Vp886GetOptionImmediate;
//    pFunc->GetLoopCond = Vp886GetLoopCond;
//    pFunc->Query = Vp886Query;
//    pFunc->QueryImmediate = Vp886QueryImmediate;
//    /* Event functions (vp886_event.c) */
//    pFunc->ApiTick = Vp886ApiTick;
//    pFunc->VirtualISR = Vp886VirtualISR;
//    pFunc->GetEvent = Vp886GetEvent;
//    pFunc->FlushEvents = Vp886FlushEvents;
//    pFunc->GetResults = Vp886GetResults;
//    pFunc->Cal = Vp886Cal;
//    /* HAL Wrapper functions (vp886_slac.c) */
//    pFunc->SlacBufStart = Vp886SlacBufStart;
//    pFunc->SlacBufSend = Vp886SlacBufSend;
//    pFunc->SlacRegWrite = Vp886SlacRegWrite;
//    pFunc->SlacRegRead = Vp886SlacRegRead;
//    ((Vp886DeviceObjectType *)pDevObj)->deviceId = deviceId;
//    ((Vp886DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0] = 0;

    //VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    //VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", status);
    return status;
}

/**
 * VpMakeLineObject()
 *  This function initializes a line context using the information that is
 * passed. This function is like a C++ constructor. It initializes the passed
 * line context and line object based on the paramters provided. The passed line
 * object type should match with the type of device object type. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  This function assumes device context has already been created and
 * initialized. This function should only be called after downloading the boot
 * image the device when applicable (like for VCP class of devices).
 *
 * Postconditions:
 *  This function initializes the line context/line object. Line related VP-API
 * functions can be called after calling this function.
 */
VpStatusType MicrochipSlicApi::VpMakeLineObject(
        uint8_t termType,
        uint8_t channelId,
        void *pLineCtx,
        void *pLineObj,
        void *pDevCtx)
{
    VpStatusType status;
    //VP_API_ENTER(VpDevCtxType, pDevCtx, "MakeLineObject");

    /* Basic argument checking */
    if ((pLineObj == nullptr) || (pDevCtx == nullptr) || (pLineCtx == nullptr)) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        //pLineCtx->pLineObj = nullptr;
        //status = VP_CALL_DEV_FUNC(MakeLineObject, (termType, channelId, pLineCtx, pLineObj, pDevCtx));
        //void *pDevObj = pDevCtx->pDevObj;

//        if (channelId >= pDevObj->staticInfo.maxChannels) {
//            VP_ERROR(VpDevCtxType, pDevCtx, ("Invalid channelId %d. Max channels on this device: %d",
//                    channelId, pDevObj->staticInfo.maxChannels));
//            return VP_STATUS_INVALID_LINE;
//        }

        //Vp886EnterCritical(pDevCtx, VP_NULL, "Vp886MakeLineObject");

        //status = Vp886MakeLineObjectInt(termType, channelId, pLineCtx, pLineObj, pDevCtx);
        //VpMemSet(pLineObj, 0, sizeof(Vp886LineObjectType));

        switch (termType) {
            case VP_TERM_FXS_GENERIC:
                //pLineObj->isFxs = TRUE;
                break;
            case VP_TERM_FXS_LOW_PWR:
                //pLineObj->isFxs = TRUE;
                break;
            default:
                return VP_STATUS_ERR_VTD_CODE;
        }

//        pLineCtx->pLineObj = pLineObj;
//        pLineCtx->pDevCtx = pDevCtx;
//
//        pDevCtx->pLineCtx[channelId] = pLineCtx;
//        pLineObj->channelId = channelId;
//        pLineObj->termType = termType;
//        pLineObj->ecVal = ((channelId == 0) ? VP886_EC_1 : VP886_EC_2);

    }

    //VP_API_EXIT(VpDevCtxType, pDevCtx, "MakeLineObject", status);
    return status;
} /* VpMakeLineObject() */

/*
 * VpInitDevice()
 *  This function calibrates the device and initializes all lines (for which
 * line context has been created and intialized) on the device with the AC, DC,
 * and Ringing parameters passed. See VP-API reference guide for more
 * information.
 *
 * Preconditions:
 *  This function should be called only after creating and initializing the
 * device context and line context (atleast for those lines which need service).
 *
 * Postconditions:
 *  Device is calibrated and all lines (for whom line context has been created
 * and initialized) associated with this device are initialized with the AC, DC,
 * and Ringing Paramaters passed (DC and Ringing apply to FXS type lines only).
 */
VpStatusType MicrochipSlicApi::VpInitDevice(
        void *pDevCtx,          /**< Pointer to device context for the
                                     * device that will be initialized
                                     */
        void *pDevProfile,   /**< Pointer to Device Profile */
        void *pAcProfile,    /**< Pointer to AC Profile that is applied
                                     * to all FXS lines on this device
                                     */
        void *pDcProfile,    /**< Pointer to DC Profile that is applied
                                     * to all FXS lines on this device
                                     */
        void *pRingProfile,  /**< Pointer to Ringing Profile that is
                                     * applied to all FXS lines on this device
                                     */
        void *pFxoAcProfile, /**< Pointer to AC Profile that is applied
                                     * to all FXO lines on this device
                                     */
        void *pFxoCfgProfile)/**< Pointer to Config Profile that is
                                     * applied to all FXO lines on this device
                                     */
{
    VpStatusType status;
    //VP_API_ENTER(VpDevCtxType, pDevCtx, "InitDevice");
    void *pProf;
    uint8_t mpiLen;
    uint8_t shutdownState = VP886_R_STATE_SS_SHUTDOWN;
    //Vp886EnterCritical(pDevCtx, VP_NULL, "Vp886InitDevice");

    /* Device profile is required. */
//    if (pDevProfile == nullptr) {
//        //VP_ERROR(VpDevCtxType, pDevCtx, ("Device profile is required."));
//        //Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
//        return VP_STATUS_ERR_PROFILE;
//    }

    /* Reset device object variables */
    //Vp886InitDeviceData(pDevCtx);

    /* Clear the MPI buffer before trying to access the device, in case there
       was a command issued earlier that is still expecting data.  Sending a
       stream of no-ops will satisfy the device's need for more data, and leave
       it in a state where it is ready to accept a command.  We're calling the
       common ClearMPIBuffer function twice because the longest command length
       for ZL880 is longer than it was for other CSLAC devices. */
    //VpCSLACClearMPIBuffer(pDevObj->deviceId);
    //VpCSLACClearMPIBuffer(pDevObj->deviceId);

    /* Hardware Reset the part, but first make sure the switchers are shut down
       to avoid catching a switcher duty cycle on with the reset signal. */
    VpSlacRegWrite(pDevCtx, nullptr, VP886_R_STATE_WRT, VP886_R_STATE_LEN, &shutdownState);
    VpSlacRegWrite(pDevCtx, nullptr, VP886_R_NOOP_WRT, 0, nullptr);
    VpSlacRegWrite(pDevCtx, nullptr, VP886_R_NOOP_WRT, 0, nullptr);
    VpSlacRegWrite(pDevCtx, nullptr, VP886_R_NOOP_WRT, 0, nullptr);
    VpSlacRegWrite(pDevCtx, nullptr, VP886_R_HWRESET_WRT, 0, nullptr);

    /* Read and verify the revision and product codes.  Need to know this now
       so we can verify that the profiles are targeted for the correct device. */
    status = Vp886InitDevicePcnRcn(pDevCtx);
    if (status != VP_STATUS_SUCCESS) {
        //pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
        //Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return status;
    }

    //VP_API_EXIT(VpDevCtxType, pDevCtx, "InitDevice", status);
    return status;
}

/** Vp886InitDevicePcnRcn()
Flush the MPI buffer and read the RCN and PCN info into the device object.
*/
VpStatusType MicrochipSlicApi::Vp886InitDevicePcnRcn(void *pDevCtx){
    //Vp886DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    //VpDeviceIdType deviceId = pDevObj->deviceId;
    VpDeviceIdType deviceId = 0x00;
    uint8_t devicePcn, deviceRcn;

    //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDevicePcnRcn+"));

    /* If Device has already been succesfully initialized, don't need to do
       anything here. */
//    if (pDevObj->busyFlags & VP_DEV_INIT_CMP) {
//        //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDevicePcnRcn-"));
//        return VP_STATUS_SUCCESS;
//    }

    /* If this is being called before InitDevice, clear the MPI buffer first */
//    if (!((pDevObj->busyFlags & VP_DEV_INIT_CMP) ||
//          (pDevObj->busyFlags & VP_DEV_INIT_IN_PROGRESS)))
//    {
//        VpCSLACClearMPIBuffer(deviceId);
//        VpCSLACClearMPIBuffer(deviceId);
//    }
    uint8_t buf_read[VP886_R_RCNPCN_LEN];
    VpSlacRegRead(pDevCtx, nullptr, (uint8_t)VP886_R_RCNPCN_RD, (uint8_t)VP886_R_RCNPCN_LEN, (uint8_t*)&buf_read);

    devicePcn = buf_read[VP886_R_RCNPCN_PCN_IDX];
    printf("read devicePcn=0x%02X\n", devicePcn);
    deviceRcn = buf_read[VP886_R_RCNPCN_RCN_IDX];
    printf("read deviceRcn=0x%02X\n", devicePcn);

    /* MPI Failure if the PCN and RCN are both 0x00 or 0xFF */
    if (((devicePcn == 0xFF) && (deviceRcn == 0xFF)) || ((devicePcn == 0x00) && (deviceRcn == 0x00))) {
        printf("MPI Failure if the PCN and RCN are both 0x00 or 0xFF\n");
        //pDevObj->busyFlags &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);

        //VP_ERROR(VpDevCtxType, pDevCtx, ("Device Failed to Detect Revision/PCN Properly: 0x%02X 0x%02X",
        //        deviceRcn, devicePcn));

        //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDevicePcnRcn-"));

        return VP_STATUS_ERR_SPI;
    }

    /* Make sure the product code is for a known device */
    switch (devicePcn) {
        case VP886_R_RCNPCN_PCN_ZL88601:
        case VP886_R_RCNPCN_PCN_ZL88602:
        case VP886_R_RCNPCN_PCN_ZL88701:
        case VP886_R_RCNPCN_PCN_ZL88702:
        case VP886_R_RCNPCN_PCN_LE9661:
        case VP886_R_RCNPCN_PCN_LE9671:
        case VP886_R_RCNPCN_PCN_LE9642:
        case VP886_R_RCNPCN_PCN_LE9641:
        case VP886_R_RCNPCN_PCN_LE9652:
        case VP886_R_RCNPCN_PCN_LE9651:
            break;
        default:
            printf("Device PCN not recognized in ZL880 or miSLIC families: 0x%02X\n", devicePcn);
            //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDevicePcnRcn-"));
            return VP_STATUS_ERR_SPI;
    }

//    if (VP886_IS_1CH(pDevObj)) {
//        VP_INFO(VpDevCtxType, pDevCtx, ("Detected single channel device (0x%02X)", devicePcn));
//        pDevObj->staticInfo.maxChannels = 1;
//    }

    /* Check for SF1AB */
//    if (VP886_IS_SF1(pDevObj)) {
//        uint8 ec = VP886_EC_BOTH;
//        VpSlacRegWrite(pDevCtx, NULL, VP886_R_EC_WRT, VP886_R_EC_LEN, &ec);
//        VpSlacRegRead(pDevCtx, NULL, VP886_R_EC_RD, VP886_R_EC_LEN, &ec);
//        if (ec != VP886_EC_1) {
//            /* Change the revision to 7 to indicate SF1AB */
//            deviceRcn = pDevObj->staticInfo.rcnPcn[VP886_R_RCNPCN_RCN_IDX] = 7;
//        }
//    }

    /* Save the IO capabilities of the device based on rcn/pcn */
//    if (VP886_IS_SF(pDevObj)) {
//        if (VP886_IS_SF1(pDevObj)) {
//            pDevObj->ioCapability = VP886_IO_CAPABILITY_CH0_IO2;
//        } else { /* SF2 or SF1AB */
//            pDevObj->ioCapability = VP886_IO_CAPABILITY_NONE;
//        }
//    } else if (VP886_IS_SF0(pDevObj)) {
//        pDevObj->ioCapability = VP886_IO_CAPABILITY_NONE;
//    } else {
//        pDevObj->ioCapability = VP886_IO_CAPABILITY_TWO_PER_CH;
//    }

    /* Save the fuse register 8 value, which contains an internal product code */
    uint8_t fuse8[VP886_R_FUSE_REG_8_LEN];
    VpSlacRegRead(pDevCtx, nullptr, VP886_R_FUSE_REG_8_RD, VP886_R_FUSE_REG_8_LEN,
                  (uint8_t *)&fuse8);

    /* Some ABS devices (ZL88801, Le9662) may have a fuse register bit set to
       indicate that they should be allowed to be treated as shared tracker
       instead of ABS.
       If an ABS has this bit set AND the application created the device object
       as VP_DEV_887_SERIES, allow the device to be configured as shared
       tracker. */
//    if (VP886_IS_ABS(pDevObj) && (pDevObj->registers.fuse8[0] & VP886_R_FUSE_REG_8_SHSUP)) {
//        if (pDevCtx->deviceType == VP_DEV_887_SERIES) {
//            /* Setting this bit changes the ABS PCN to tracker */
//            pDevObj->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] |= VP886_R_RCNPCN_PCN_ABS_TRACKER;
//            /* Set a flag to enabled shared-tracker behaviors. */
//            pDevObj->stateInt |= VP886_SHARED_SUPPLY;
//            /* Set a flag to call Vp886ChangeDeviceType() later on to
//               actually reprogram the device to run in the other mode. */
//            pDevObj->stateInt |= VP886_CONVERT_DEVICE_TYPE;
//        }
//    }

//    VP_INFO(VpDevCtxType, pDevCtx, ("Device is %s %s, rev 0x%02X\n",
//            VP886_IS_ABS(pDevObj) ? "886 (ABS)" : "887 (TRACKER)",
//            VP886_IS_HV(pDevObj) ? "HV" : "LV", deviceRcn));
//    VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDevicePcnRcn-"));

    return VP_STATUS_SUCCESS;
} /* Vp886InitDevicePcnRcn */

/**
* Performs buffered and unbuffered writes to the device.  If buffering has been
* started by VpSlacBufStart(), this function will add to the write buffer.  If
        * buffering is not currently active, it will perform an immediate write.
*
* This function can accept either a device context or line context, and the
* device-specific implementation can format the final output based on which
* is provided.
* If both pDevCtx and pLineCtx are NULL the function will return FALSE.
*
* This function will perform error checks on the input data before attempting
* to access the device. The function returns TRUE if successful.
*/
boolean MicrochipSlicApi::VpSlacRegWrite(
        void *pDevCtx,
        void *pLineCtx,
        uint8_t cmd,
        uint8_t writeLen,
        const uint8_t *pWriteBuf)
{

    /* Read commands should not be allowed */
    if (cmd & 0x01) {
        //VP_WARNING(VpDevCtxType, pDevCtx, ("Invalid read command in Vp886SlacRegWrite (0x%02X)", cmd));
        return false;
    }

    /* Unbuffered write */
    uint8_t deviceId = 0x00;
    /* EC value to be used when a device context is passed to Vp886SlacRegWrite,
       Vp886SlacRegRead, or Vp886SlacBufSend.  During normal operation, this
       should be set to VP886_EC_GLOBAL.  The exception is when a routine
       within VpInitDevice() needs to write or read a channel register.  Since
       there is no requirement to have line contexts/objects defined at the time
       of InitDevice, we can't pass a line context to our register write/read
       functions.  To access channel registers, set this value to VP886_EC_1 or
       VP886_EC_2, do the command using the device context, and then set this
       back to VP886_EC_GLOBAL. */
    uint8_t ecVal = VP886_EC_1;
    VpMpiCmd(deviceId, ecVal, cmd, writeLen, (uint8_t *)pWriteBuf);

    /* Add to the traffic count. Data length + command + EC command + EC data */
    //pDevObj->trafficBytes += dataLen + 3;


    return true;
}

/**
 * Internal API functions that need to retrieve data from the device
 * should call this function.
 *
 * If buffering has been started by VpSlacBufStart() and the write buffer is
 * not empty, this function will write the current contents of the buffer to the
 * device before performing the read.  This is done to prevent write->read
 * errors when buffering while allowing internal code to behave the same way
 * regardless of whether buffering is enabled or not.
 *
 * This function can accept either a device context or line context, and the
 * device-specific implementation can take different actions based on which
 * is provided.
 * If both pDevCtx and pLineCtx are NULL the function will return FALSE.
 *
 * This function will perform error checks on the input data before attempting
 * to access the device. The function returns TRUE if successful.
 */
boolean MicrochipSlicApi::VpSlacRegRead( void *pDevCtx,  void *pLineCtx,
        uint8_t cmd,  uint8_t readLen,  uint8_t *pReadBuf)
{

    /* Write commands should not be allowed */
    if (!(cmd & 0x01)) {
        printf("Invalid write command in Vp886SlacRegRead (0x%02X)", cmd);
        return false;
    }

    uint8_t deviceId = 0x00;
    uint8_t ecVal = VP886_EC_1;
    VpMpiCmd(deviceId, ecVal, cmd, readLen, pReadBuf);

    /* Add to the traffic count. Data length + command + EC command + EC data */
    //pDevObj->trafficBytes += dataLen + 3;

    return true;

}

void MicrochipSlicApi::VpMpiCmd(uint8_t deviceId,
                                uint8_t ecVal, uint8_t cmd, uint8_t cmdLen,
                                uint8_t *dataPtr) {
    uint8_t cmdIndex = 0;
/* Write the EC Value to the EC Register */
    printf("Register");
    spi->spi_byte_write((uint32_t)deviceId, (byte)EC_WRT_CMD);
    spi->spi_byte_write((uint32_t)deviceId, (byte)ecVal);
/* Write the Command Byte */
    spi->spi_byte_write((uint32_t)deviceId, (byte)cmd);
    if (cmd & 0x01) { /* If cmd LSB = ’1’, Perform Read Operation */
        uint16_t char_read = spi->spi_byte_read(deviceId);
        dataPtr[0] = (uint8_t)char_read;
        dataPtr[1] = (uint8_t)char_read>>8;
//        for (cmdIndex = 0; cmdIndex < cmdLen; cmdIndex++) {
//            dataPtr[cmdIndex] = spi->spi_byte_read(deviceId);
//        }
    } else { /* If cmd LSB = ’0’, Perform Write Operation */
        //todo check byte alignment
        for (cmdIndex = 0; cmdIndex < cmdLen; cmdIndex++) {
            spi->spi_byte_write(deviceId, dataPtr[cmdIndex]);
        }
    }
}

