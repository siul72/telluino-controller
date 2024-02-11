//
// Created by siul72 on 11.02.2024.
//

#include "vp_api_common.h"

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
boolean
VpSlacRegWrite(
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx,
    uint8 cmd,
    uint8 writeLen,
    const uint8 *pWriteBuf)
{
    VpDevCtxType *pDevCtxLocal = pDevCtx;

    if (pDevCtx == NULL && pLineCtx == NULL) {
        //VP_HAL(None, NULL, ("VpSlacRegWrite(): Invalid NULL pDevCtx and pLineCtx"));
        return FALSE;
    }

    if (pDevCtxLocal == NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
    }

    return TRUE;

    //if (pDevCtxLocal->funPtrsToApiFuncs.SlacRegWrite == VP_NULL) {
    //    VP_HAL(VpDevCtxType, pDevCtxLocal, ("VpSlacRegWrite(): Function not supported by device type"));
    //    return FALSE;
    //}

    //return pDevCtxLocal->funPtrsToApiFuncs.SlacRegWrite(pDevCtx, pLineCtx, cmd, writeLen, pWriteBuf);
}
