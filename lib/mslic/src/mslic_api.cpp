//
// Created by siul72 on 10.02.2024.
//

#include "mslic_api.h"

#define EC_WRT_CMD 0x4a

MicrochipSlicApi::MicrochipSlicApi(){
   this->spi = new SlicSpi();
}

MicrochipSlicApi::~MicrochipSlicApi(){
    delete this->spi;
}


void MicrochipSlicApi::VpMpiCmd(
        VpDeviceIdType deviceId,
        uint8_t ecVal, uint8_t cmd, uint8_t cmdLen, uint8_t * dataPtr)
{
    uint8_t cmdIndex = 0;
/* Write the EC Value to the EC Register */
    spi->spi_byte_write((uint32_t)deviceId, (byte)EC_WRT_CMD);
    spi->spi_byte_write((uint32_t)deviceId, (byte)ecVal);
/* Write the Command Byte */
    spi->spi_byte_write((uint32_t)deviceId, (byte)cmd);
    if (cmd & 1) { /* If cmd LSB = ’1’, Perform Read Operation */
        for (cmdIndex = 0; cmdIndex < cmdLen; cmdIndex++) {
            dataPtr[cmdIndex] = spi->spi_byte_read(deviceId);
        }
    } else { /* If cmd LSB = ’0’, Perform Write Operation */
        for (cmdIndex = 0; cmdIndex < cmdLen; cmdIndex++) {
            spi->spi_byte_write(deviceId, dataPtr[cmdIndex]);
        }
    }
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
VpStatusType
VpMakeDeviceObject(
        VpDeviceType deviceType,    /**< Device Type */
        VpDeviceIdType deviceId,    /**< Hardware chip select for this device */

        VpDevCtxType *pDevCtx,      /**< Device Context to be initialized by other
                                 * input
                                 */
        void *pDevObj)              /**< Device Object to be pointed to by device
                                 * context
                                 */
{
    uint8 i;
    VpTempFuncPtrType *funcPtr;
    VpStatusType status;
    VP_API_ENTER(None, VP_NULL, "MakeDeviceObject");

    /* Basic argument checking */
    if ((pDevObj == VP_NULL) || (pDevCtx == VP_NULL)) {
        VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", VP_STATUS_INVALID_ARG);
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
        VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", VP_STATUS_ERR_VTD_CODE);
        return VP_STATUS_ERR_VTD_CODE;
    }

    /* Since every application needs to use this function, use this as a place
       to perform the check on vp_api_types.h */
    status = VpTypesTest();
    if (status != VP_STATUS_SUCCESS) {
        VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", status);
        return status;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* User wants to create device context as well; First clear it */
    /* Initialize All the funciton pointers to zero */
    funcPtr = (VpTempFuncPtrType *)&pDevCtx->funPtrsToApiFuncs;
    for(i = 0; i < (sizeof(ApiFunctions) / sizeof(VpTempFuncPtrType)); i++){
        *funcPtr = VP_NULL;
        funcPtr++;
    }

    /* Initialize all the line context pointers to null */
    for(i = 0; i < VP_MAX_LINES_PER_DEVICE; i++) {
        pDevCtx->pLineCtx[i] = VP_NULL;
    }

    pDevCtx->deviceType = deviceType;

    switch (deviceType) {
#if defined (VP_CC_VCP_SERIES)
        case VP_DEV_VCP_SERIES:
            ((VpVcpDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = VpMakeVcpDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_VCP2_SERIES)
        case VP_DEV_VCP2_SERIES:
            ((VpVcp2DeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = Vcp2MakeDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_MELT_SERIES)
        case VP_DEV_MELT_SERIES:
            ((VpMeltDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = MeltMakeDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_MELT792_SERIES)
        case VP_DEV_MELT792_SERIES:
            status = Melt792MakeDeviceObject(pDevCtx, pDevObj);
            ((VpMelt792DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            status = VpMakeVp880DeviceObject(pDevCtx, pDevObj);
            ((Vp880DeviceObjectType *)pDevObj)->deviceId = deviceId;
            ((Vp880DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0] = 0;
            break;
#endif

#if defined (VP_CC_886_SERIES)
        case VP_DEV_886_SERIES:
        case VP_DEV_887_SERIES:
            status = VpMakeVp886DeviceObject(pDevCtx, pDevObj);
            ((Vp886DeviceObjectType *)pDevObj)->deviceId = deviceId;
            ((Vp886DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0] = 0;
            break;
#endif

#if defined (VP_CC_890_SERIES)
        case VP_DEV_890_SERIES:
            status = VpMakeVp890DeviceObject(pDevCtx, pDevObj);
            ((Vp890DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_792_SERIES)
        case VP_DEV_792_SERIES:
            status = Vp792MakeDeviceObject(pDevCtx, pDevObj);
            ((Vp792DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_792_GROUP)
        case VP_DEV_792_GROUP:
            pDevCtx->pDevObj = pDevObj;
            pDevCtx->funPtrsToApiFuncs.GetEvent = Vp792GroupGetEvent;
            ((Vp792GroupDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = VP_STATUS_SUCCESS;
            break;
#endif

#if defined (VP_CC_KWRAP)
        case VP_DEV_KWRAP:
            status = VP_STATUS_FUNC_NOT_SUPPORTED;
            break;
#endif
        default:
            /*
             * This error check should be performed pointers are initialized, so
             * this code should never be executed -- error checking above
             */
            status = VP_STATUS_ERR_VTD_CODE;
            break;
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    VP_API_EXIT(None, VP_NULL, "MakeDeviceObject", status);
    return status;
} /* VpMakeDeviceObject() */
