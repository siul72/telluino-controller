//
// Created by siul72 on 11.02.2024.
//

#include "vp886_device.h"
#include "vp88x_profiles.h"

void
Vp886EnterCritical(
        VpDevCtxType *pDevCtx,
        VpLineCtxType *pLineCtx,
        const char* funcName)
{
    Vp886DeviceObjectType *pDevObj;
    VpDeviceIdType deviceId;

    if (pDevCtx == VP_NULL) {
        pDevCtx =  pLineCtx->pDevCtx;
    }
    pDevObj = (Vp886DeviceObjectType*)pDevCtx->pDevObj;
    deviceId = pDevObj->deviceId;

    if (funcName != VP_NULL) {
        if (pLineCtx != VP_NULL) {
            //VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("%s+", funcName));
        } else {
            //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("%s+", funcName));
        }
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pDevObj->criticalDepth++;

#if defined(VP_DEBUG) && (VP_CC_DEBUG_SELECT & VP_DBG_WARNING)
    Vp886NestedCriticalCheck(pDevCtx, funcName);
#endif /* defined(VP_DEBUG) && (VP_CC_DEBUG_SELECT & VP_DBG_WARNING) */

    /* Shouldn't need to do this here, but just to be safe.. */
    pDevObj->timestampValid = FALSE;

    /* Start the SLAC Write Buffer. */
    if (!VpSlacBufStart(pDevCtx)) {
        //VP_WARNING(VpDevCtxType, pDevCtx, ("Failed VpSlacBufStart, %s",
        //        funcName != VP_NULL ? funcName : ""));
    }

    return;
}

/**  Vp886ExitCritical()
  Wrapper function for VpSysExitCritical() that also performs other actions
  that we want to surround each API call.

  This function accepts either device or line context.  If the line context is
  provided, the FUNC_INT debug message will be done for the line.  Otherwise the
  debug message is done at the device level, and other operation is the same.
  If funcName is VP_NULL, the FUNC_INT message is skipped.

  Operations:
   - Invalidates the saved timestamp to ensure we update it before using it next
   - Sends the MPI command buffer
   - Checks for critical section mismatches
   - Calls VpSysExitCritical()
   - Handles FUNC_INT debug messages

  This function should be the only place in the Vp886 API where VpSysExitCritical
  is called.
*/
void
Vp886ExitCritical(
        VpDevCtxType *pDevCtx,
        VpLineCtxType *pLineCtx,
        const char* funcName)
{
    Vp886DeviceObjectType *pDevObj;
    VpDeviceIdType deviceId;

    if (pDevCtx == VP_NULL) {
        pDevCtx = pLineCtx->pDevCtx;
    }
    pDevObj = (Vp886DeviceObjectType*)pDevCtx->pDevObj;
    deviceId = pDevObj->deviceId;

    /* Cached timestamp is invalid until after the first timestamp register read
       during this critical section */
    pDevObj->timestampValid = FALSE;

    /* Send the SLAC Write Buffer. */
    VpSlacBufSend(pDevCtx);

    if (pDevObj->ecVal != VP886_EC_GLOBAL) {
        //VP_WARNING(VpDevCtxType, pDevCtx, ("pDevObj->ecVal was not reset during %s",
        //        funcName != VP_NULL ? funcName : ""));
        pDevObj->ecVal = VP886_EC_GLOBAL;
    }

    if (pDevObj->criticalDepth == 0) {
        //VP_WARNING(VpDevCtxType, pDevCtx, ("Too many Vp886ExitCritical calls, depth cannot go lower (%s)",
        //        funcName != VP_NULL ? funcName : ""));
    } else {
        pDevObj->criticalDepth--;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (funcName != VP_NULL) {
        if (pLineCtx != VP_NULL) {
            //VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("%s-", funcName));
        } else {
            //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("%s-", funcName));
        }
    }

    return;
}



void
Vp886SubDefaultProfiles(
        VpDevCtxType *pDevCtx,
        VpProfilePtrType *ppAcProfile,
        VpProfilePtrType *ppDcProfile,
        VpProfilePtrType *ppRingProfile)
{
    VpDeviceType deviceType = pDevCtx->deviceType;

    /* If no AC profile is provided, use power-up default values. */
    if (*ppAcProfile == VP_NULL) {
        if (deviceType == VP_DEV_886_SERIES) {
            *ppAcProfile = VP886_INTERNAL_DEFAULT_AC_PROFILE;
        } else {
            *ppAcProfile = VP887_INTERNAL_DEFAULT_AC_PROFILE;
        }
    }

    /* If no DC profile is provided, use power-up default values. */
    if (*ppDcProfile == VP_NULL) {
        if (deviceType == VP_DEV_886_SERIES) {
            *ppDcProfile = VP886_INTERNAL_DEFAULT_DC_PROFILE;
        } else {
            *ppDcProfile = VP887_INTERNAL_DEFAULT_DC_PROFILE;
        }
    }

    /* If no Ringing profile is provided, use power-up default values. */
    if (*ppRingProfile == VP_NULL) {
        if (deviceType == VP_DEV_886_SERIES) {
            *ppRingProfile = VP886_INTERNAL_DEFAULT_RING_PROFILE;
        } else {
            *ppRingProfile = VP887_INTERNAL_DEFAULT_RING_PROFILE;
        }
    }

    return;
}

/** Vp886InitBatSelGpio()
  Checks for conflicts between the battery select GPIO, device IO capabilities,
  and switcher configuration.  Then programs the IO registers to set the
  specified GPIO as output, defaulting to a 0 value.
  This function assumes that the device type is already known to be ABS and
  devProfileData.batSelGpio is not VP886_DEV_PROFILE_BAT_SEL_GPIO_NONE.
*/
VpStatusType
Vp886InitBatSelGpio(
        VpDevCtxType *pDevCtx)
{
    Vp886DeviceObjectType *pDevObj = (Vp886DeviceObjectType*)pDevCtx->pDevObj;
    uint8 ioDirMask;
    uint8 ioDirSet;
    uint8 ioDirReg[VP886_R_IODIR_LEN];
    uint8 ioDataMask;
    uint8 ioDataReg[VP886_R_IODATA_LEN];
    uint8 io21Usage;
    uint8 io22Usage;

    /* Device profile I/O 2 usage */
    io21Usage = pDevObj->devProfileData.io2Use;
    io21Usage &= VP886_DEV_PROFILE_IO2_USE_BITS;
    io22Usage = pDevObj->devProfileData.io2Use >> 4;
    io22Usage &= VP886_DEV_PROFILE_IO2_USE_BITS;

    if (pDevObj->devProfileData.swCfg == VP886_DEV_PROFILE_SW_CONF_BB) {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - BBABS and GPIO battery switch are not compatible."));
        return VP_STATUS_ERR_PROFILE;
    }

    if (pDevObj->ioCapability == VP886_IO_CAPABILITY_NONE) {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - batSelGpio %d invalid, device does not support GPIOs",
        //        pDevObj->devProfileData.batSelGpio));
        return VP_STATUS_ERR_PROFILE;
    }

    if (pDevObj->ioCapability == VP886_IO_CAPABILITY_CH0_IO2 &&
        pDevObj->devProfileData.batSelGpio != VP886_DEV_PROFILE_BAT_SEL_GPIO_IO21)
    {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - batSelGpio %d invalid, device only supports IO21",
        //        pDevObj->devProfileData.batSelGpio));
        return VP_STATUS_ERR_PROFILE;
    }

    if ((io21Usage == VP886_DEV_PROFILE_IO2_USE_DIG_INT || io21Usage == VP886_DEV_PROFILE_IO2_USE_VMM) &&
        pDevObj->devProfileData.batSelGpio == VP886_DEV_PROFILE_BAT_SEL_GPIO_IO21)
    {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - IO21 profile conflict, io21Usage %d, batSelGpio %d",
        //        io21Usage, pDevObj->devProfileData.batSelGpio));
        return VP_STATUS_ERR_PROFILE;
    }

    if ((io22Usage == VP886_DEV_PROFILE_IO2_USE_DIG_INT || io22Usage == VP886_DEV_PROFILE_IO2_USE_VMM) &&
        pDevObj->devProfileData.batSelGpio == VP886_DEV_PROFILE_BAT_SEL_GPIO_IO22)
    {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - IO22 profile conflict, io22Usage %d, batSelGpio %d",
        //        io22Usage, pDevObj->devProfileData.batSelGpio));
        return VP_STATUS_ERR_PROFILE;
    }

    switch (pDevObj->devProfileData.batSelGpio) {
        case VP886_DEV_PROFILE_BAT_SEL_GPIO_NONE:
         //    //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - batSelGpio is NONE, should not be possible here\n"));
            return VP_STATUS_FAILURE;
        case VP886_DEV_PROFILE_BAT_SEL_GPIO_IO11:
            pDevObj->ecVal = VP886_EC_1;
            ioDirMask = VP886_R_IODIR_IOD1;
            ioDirSet = VP886_R_IODIR_IOD1_DIG_OUTPUT;
            ioDataMask = VP886_R_IODATA_IO1;
            break;
        case VP886_DEV_PROFILE_BAT_SEL_GPIO_IO12:
            pDevObj->ecVal = VP886_EC_2;
            ioDirMask = VP886_R_IODIR_IOD1;
            ioDirSet = VP886_R_IODIR_IOD1_DIG_OUTPUT;
            ioDataMask = VP886_R_IODATA_IO1;
            break;
        case VP886_DEV_PROFILE_BAT_SEL_GPIO_IO21:
            pDevObj->ecVal = VP886_EC_1;
            ioDirMask = VP886_R_IODIR_IOD2;
            ioDirSet = VP886_R_IODIR_IOD2_OUTPUT;
            ioDataMask = VP886_R_IODATA_IO2;
            break;
        case VP886_DEV_PROFILE_BAT_SEL_GPIO_IO22:
            pDevObj->ecVal = VP886_EC_2;
            ioDirMask = VP886_R_IODIR_IOD2;
            ioDirSet = VP886_R_IODIR_IOD2_OUTPUT;
            ioDataMask = VP886_R_IODATA_IO2;
            break;
        default:
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitBatSelGpio - Unknown batSelGpio %d\n", pDevObj->devProfileData.batSelGpio));
            return VP_STATUS_ERR_PROFILE;
    }

    VpSlacRegRead(pDevCtx, NULL, VP886_R_IODIR_RD, VP886_R_IODIR_LEN, ioDirReg);
    ioDirReg[0] &= ~ioDirMask;
    ioDirReg[0] |= ioDirSet;
    VpSlacRegWrite(pDevCtx, NULL, VP886_R_IODIR_WRT, VP886_R_IODIR_LEN, ioDirReg);

    VpSlacRegRead(pDevCtx, NULL, VP886_R_IODATA_RD, VP886_R_IODATA_LEN, ioDataReg);
    ioDataReg[0] &= ~ioDataMask;
    VpSlacRegWrite(pDevCtx, NULL, VP886_R_IODATA_WRT, VP886_R_IODATA_LEN, ioDataReg);

    pDevObj->ecVal = VP886_EC_GLOBAL;

    return VP_STATUS_SUCCESS;
}


/** Vp886InitDeviceSM()
  Implements the 886 device initialization state machine.

  Steps summary:
   - Device bring-up
   - Calibration
   - Switcher bring-up
   - Initialize lines
   - Generate VP_DEV_EVID_DEV_INIT_CMP
*/
void
Vp886InitDeviceSM(
        VpDevCtxType *pDevCtx)
{
    Vp886DeviceObjectType *pDevObj = (Vp886DeviceObjectType*)pDevCtx->pDevObj;
    VpStatusType status;
    bool runAnotherState = TRUE;
    static uint8 clkTestCount;
    uint16 eventData = VP_DEV_INIT_CMP_SUCCESS;
    uint32 timerDuration = 0;

    //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDeviceSM+"));
    /* Keep looping throught the main switch until it's time to set a timer and
       leave.  Each state should either start a timer or measurement that will
       generate an interrupt OR set runAnotherState = TRUE. */
    while (runAnotherState) {
        runAnotherState = FALSE;

        ////VP_INFO(VpDevCtxType, pDevCtx, ("Vp886InitDeviceSM: Running state %d\n", pDevObj->initDeviceState));

        /* Switch on the state variable to enter the appropriate state (case) */
        switch (pDevObj->initDeviceState) {
            case VP886_INIT_DEVICE_SWITCHER_PREP: {
                uint8 mpiCmdData[6];
                uint8 mpiLen = pDevObj->pDevProfile[VP_PROFILE_MPI_LEN];
                uint8 channel;

                /* Program the profile switcher timing parameters with the low
                   power parameters included.  This will be overwritten later
                   in InitLine if any line is non-lowpower.
                   TODO: While this is slightly more efficient than programming
                   the params for each InitLine call, it is still redundant with
                   the MPI section write that we just did.  However, we do want
                   the low power params set by default, before we know about
                   the lines.  It still seems like there's a better way to do
                   this. */
                //Vp886SetSwTimingParams(pDevCtx);

                /* Turn off the switchers' external circuits in the calibration control reg
                   by setting ZBATIN and YBATIN to 01 --> Disconnected.
                   This needs to be done for both lines.
                   Nothing about this in the Device Profile */
                mpiCmdData[0] = 0x50; mpiCmdData[1] = 0x00; mpiCmdData[2] = 0x00;
                /* For both channels */
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_CALCTRL_WRT, VP886_R_CALCTRL_LEN, mpiCmdData);

                /* Increase the Switcher Over-current thresholds

                   This can be retrieved from the Switcher Over-Current
                   Threshold field of the Device Profile

                   The blanking parameter is also obtained from the
                   Device Profile */
                mpiCmdData[0] = 0x00; mpiCmdData[1] = 0x00;
                mpiCmdData[2] = pDevObj->pDevProfile[mpiLen+VP886_DEV_PROFILE_OC_CNT0_OFFSET];
                mpiCmdData[3] = pDevObj->pDevProfile[mpiLen+VP886_DEV_PROFILE_OC_CNT1_OFFSET];
                mpiCmdData[4] = pDevObj->pDevProfile[mpiLen+VP886_DEV_PROFILE_BLANKING_OFFSET];

                /* For both channels */
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_SW_OC_WRT, VP886_R_SW_OC_LEN, mpiCmdData);

                /* Configure for auto-shutdown upon:
                   switcher over-current
                   switcher over-voltage
                   charge pump under-voltage

                   This probably isn't necessary since this is the power up default. Leaving
                   it in for completeness. Consider removing.
                   Nothing about this in the Device Profile.

                   The over voltage/current and undervoltage will by default be turned off by
                   the SWITCHER_CTRL option default value. This comes later. */
                mpiCmdData[0] = VP886_R_SSCFG_AUTO_SYSSTATE;
                mpiCmdData[1] = VP886_R_SSCFG_AUTO_OVERCURRENT | VP886_R_SSCFG_AUTO_OVERVOLTAGE | VP886_R_SSCFG_AUTO_CP_UV;

                /* If charge pump protection mode is set to disabled in the device profile
                   and this is not an SF device, disable the auto shutdown */
                if (pDevObj->devProfileData.cpProtection == VP886_CP_PROT_DISABLED && !VP886_IS_SF(pDevObj)) {
                    mpiCmdData[1] &= ~VP886_R_SSCFG_AUTO_CP_UV;
                }

                /* For both channels */
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_SSCFG_WRT, VP886_R_SSCFG_LEN, mpiCmdData);

                /* Enable over voltage detection

                   This is accomplished by applying the Switching Regulator Parameters
                   field of the Device Profile and making sure that the SWOVP is set. */
                pDevObj->swParams[1] |= VP886_R_SWPARAM_SWOVP;

                for (channel = 0; channel < pDevObj->staticInfo.maxChannels; channel++) {
                    uint8 swParamBuf[VP886_R_SWPARAM_LEN];

                    /* The SWP reg needs to be written even if the LineCtx
                       for that line is NULL. This needs to be done to ensure that the
                       batteries come up */
                    swParamBuf[0] = pDevObj->swParams[0];
                    swParamBuf[1] = pDevObj->swParams[1];
                    swParamBuf[2] = pDevObj->swParams[2];

                    /* If this is an ABS part, swap in the SWY or SWZ voltage
                       as specified in the Dev Prof */
                    if (VP886_IS_ABS(pDevObj)) {

                        uint8 swv;
                        swParamBuf[0] &= ~VP886_R_SWPARAM_ABS_V;
                        if (channel == 0) {
                            swv = (uint8)VpRoundedDivide(pDevObj->devProfileData.swyv, 5);
                        } else {
                            swv = (uint8)VpRoundedDivide(pDevObj->devProfileData.swzv, 5);
                        }
                        if (swv > 0) {
                            swv = swv - 1;
                        }
                        swParamBuf[0] |= swv;
                    }

                    /* Send the SWR write command to the channel.*/
                    pDevObj->ecVal = (channel == 0) ? VP886_EC_1 : VP886_EC_2;
                    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_SWPARAM_WRT, VP886_R_SWPARAM_LEN, swParamBuf);
                    pDevObj->ecVal = VP886_EC_GLOBAL;
                }

                pDevObj->initDeviceState = VP886_INIT_DEVICE_CFAIL_CHECK_PREP;
                runAnotherState = TRUE;
                break;
            }
            case VP886_INIT_DEVICE_CFAIL_CHECK_PREP: {
                uint8 channel;

                /* Call Vp886ConfigLineInt now to blast down the AC, DC, and Ring profiles.
                   This way these profiles do not have to be stored in the device object */
                for (channel = 0; channel < pDevObj->staticInfo.maxChannels; channel++) {
                    VpLineCtxType *pLineCtx = pDevCtx->pLineCtx[channel];
                    Vp886LineObjectType *pLineObj;
                    VpLineIdType lineId;
                    if (pLineCtx == VP_NULL) {
                        continue;
                    }
                    pLineObj =  pLineCtx->pLineObj;

                    /* Initialize the line object variables, without disturbing lineId.
                       Do this now instead of during the later InitLine calls so that
                       data stored from the profiles is not lost. */
                    lineId = pLineObj->lineId;
                    //Vp886MakeLineObjectInt(pLineObj->termType, pLineObj->channelId, pLineCtx, pLineObj, pDevCtx);
                    pLineObj->lineId = lineId;

                    //Vp886ConfigLineInt(pLineCtx, pDevObj->pAcProfile, pDevObj->pDcProfile,
                    //                   pDevObj->pRingProfile, TRUE);
                }

                /* Initialize CFAIL failure count */
                clkTestCount = 10;

                /* Set a timer to give the clock some time to settle */
                timerDuration = VP886_CFAIL_WAIT_TIME;
                pDevObj->initDeviceState = VP886_INIT_DEVICE_CFAIL_CHECK;
                break;
            }
            case VP886_INIT_DEVICE_CFAIL_CHECK: {
                uint8 clkNotStable;

                /* Check for CFAIL */
                clkNotStable = pDevObj->registers.sigreg[0] & VP886_R_SIGREG_CFAIL;

                /* If clock fault is present and there are still some more tries allowed... */
                if (clkNotStable && (--clkTestCount)) {
                    /* Set a timer to return to this state in 10ms and check again */
                    timerDuration = VP886_CFAIL_WAIT_TIME;
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_CFAIL_CHECK;
                    break;
                }

                if (clkNotStable) {
                    /* If clock fault has persisted for too long */

                    if (VP886_IS_SF1(pDevObj) && pDevObj->registers.devCfg[0] == pDevObj->devProfileData.devCfg) {
                        /* SF1 supports 16kHz framesync.  If the device profile
                           was set for 8kHz see if 16 works, or vice-versa. */
                        pDevObj->registers.devCfg[0] ^= VP886_R_DEVCFG_FS_16K;
                        VpSlacRegWrite(pDevCtx, NULL, VP886_R_DEVCFG_WRT, VP886_R_DEVCFG_LEN, pDevObj->registers.devCfg);
                        clkTestCount = 10;
                        timerDuration = VP886_CFAIL_WAIT_TIME;
                        pDevObj->initDeviceState = VP886_INIT_DEVICE_CFAIL_CHECK;
                        break;
                    }

                    /* Send a Device Init Complete event with the CFAIL flag.
                       Go to the VP886_INIT_DEVICE_GEN_EVENT state. */
                     //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device Failed to Reset Clock Fault"));
                    eventData = VP_DEV_INIT_CMP_CFAIL;
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                    runAnotherState = TRUE;

                } else {
                    /* Clock fault cleared, proceed */
                    if (VP886_IS_SF1(pDevObj)) {
                        if (pDevObj->registers.devCfg[0] & VP886_R_DEVCFG_FS_16K) {
                            pDevObj->options.fsyncRate = VP_FSYNC_RATE_16KHZ;
                        } else {
                            pDevObj->options.fsyncRate = VP_FSYNC_RATE_8KHZ;
                        }
                    }
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_ZSI_DETECT;
                    runAnotherState = TRUE;
                }
                break;
            }
            case VP886_INIT_DEVICE_ZSI_DETECT: {
                uint8 csRd, csWrt;

                /* Check for ZSI mode by flipping the XE bit and verifying that it took */
                VpSlacRegRead(pDevCtx, NULL, VP886_R_CLKSLOTS_RD, VP886_R_CLKSLOTS_LEN, &csRd);
                csWrt = csRd ^ VP886_R_CLKSLOTS_EDGE;
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_CLKSLOTS_WRT, VP886_R_CLKSLOTS_LEN, &csWrt);
                VpSlacRegRead(pDevCtx, NULL, VP886_R_CLKSLOTS_RD, VP886_R_CLKSLOTS_LEN, &csRd);

                if (csWrt == csRd) {
                    /* If the XE bit flip took, this is NOT ZSI mode.
                       Set the flag in the Device Object and put the
                       XE bit back to its original value. */
                    csWrt ^= VP886_R_CLKSLOTS_EDGE;
                    VpSlacRegWrite(pDevCtx, NULL, VP886_R_CLKSLOTS_WRT, VP886_R_CLKSLOTS_LEN, &csWrt);
                    pDevObj->stateInt &= ~VP886_ZSI_DETECTED;
                    //VP_INFO(VpDevCtxType, pDevCtx, ("ZSI not detected"));
                } else {
                    /* The XE bit flip did not change, this is ZSI mode.
                       Set the flag in the device object. */
                    pDevObj->stateInt |= VP886_ZSI_DETECTED;
                    //VP_INFO(VpDevCtxType, pDevCtx, ("ZSI detected"));
                }

                pDevObj->initDeviceState = VP886_INIT_DEVICE_ENABLE_VREF;
                runAnotherState = TRUE;
                break;
            }
            case VP886_INIT_DEVICE_ENABLE_VREF: {
                uint8 icr3Data[4];

                /* Enable Vref in ICR3 */
                icr3Data[0] = VP886_R_ICR3_VREF_EN; icr3Data[1] = VP886_R_ICR3_VREF_EN;
                icr3Data[2] = 0x00; icr3Data[3] = 0x00;
                /* For both channels */
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_ICR3_WRT, VP886_R_ICR3_LEN, icr3Data);

                /* Set a timer to allow Vref to come up */
                timerDuration = VP886_VREF_WAIT_TIME;
                pDevObj->initDeviceState = VP886_INIT_DEVICE_VREF_CHECK;
                break;
            }
            case VP886_INIT_DEVICE_VREF_CHECK: {
                uint8 icr6Data[4];

                /* Read ICR6 to see if Vref came up */
                VpSlacRegRead(pDevCtx, NULL, VP886_R_ICR6_RD, VP886_R_ICR6_LEN, icr6Data);

                /* If it didn't .... */
                if ((icr6Data[0] & 0x10) == 0x00) {

                    /* Send a Device Init Complete event with the VREF_FAIL flag.
                       Go to the VP886_INIT_DEVICE_GEN_EVENT state. */
                     //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device Failed to Reset Vref did not come up successfully"));
                    eventData = VP_DEV_INIT_CMP_VREF_FAIL;

                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;

                    runAnotherState = TRUE;
                    break;

                }

                /* If it did come up ... give it a little bit more timer to settle */
                timerDuration = VP886_VREF_WAIT_TIME;
                if (pDevObj->devProfileData.cpEnable && !VP886_IS_SF(pDevObj)) {
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_CP_ENABLE;
                } else {
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_CH1_CH2_DISC;
                }

                break;
            }
            case VP886_INIT_DEVICE_CP_ENABLE: {
                uint8 dmData[2];

                /* Re-enable the Charge Pump
                   This is done by applying the Device Mode field of the Device
                   Profile, with CPEN = 1 */
                dmData[0] = pDevObj->devProfileData.devMode[0] | VP886_R_DEVMODE_CHARGEPUMP_EN;
                dmData[1] = pDevObj->devProfileData.devMode[1];
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_DEVMODE_WRT, VP886_R_DEVMODE_LEN, dmData);

                /* Give the charge pump some time to come up */
                timerDuration = VP886_CP_WAIT_TIME;
                pDevObj->initDeviceState = VP886_INIT_DEVICE_CH1_CH2_DISC;

                break;
            }

            case VP886_INIT_DEVICE_CH1_CH2_DISC: {
                uint8 ssData = VP886_R_STATE_SS_DISCONNECT;

                /* If ABS, put switchers into low power mode without actually turning them on */
                if (VP886_IS_ABS(pDevObj)) {
                    pDevObj->registers.swCtrl[0] &= ~(VP886_R_SWCTRL_MODE_Z | VP886_R_SWCTRL_MODE_Y);
                    pDevObj->registers.swCtrl[0] |= (VP886_R_SWCTRL_MODE_Y_LP | VP886_R_SWCTRL_MODE_Z_LP);
                    VpSlacRegWrite(pDevCtx, NULL, VP886_R_SWCTRL_WRT, VP886_R_SWCTRL_LEN, pDevObj->registers.swCtrl);
                }

                /* Put both channels into disconnect */
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_STATE_WRT, VP886_R_STATE_LEN, &ssData);

                /* Set a timer to delay for a bit */
                timerDuration = VP886_DISC_WAIT_TIME;
                pDevObj->initDeviceState = VP886_INIT_DEVICE_CP_CHECK;
                break;
            }
            case VP886_INIT_DEVICE_CP_CHECK: {
                uint8 ssData1, ssData2;

                /* If there was an under-volt on the charge pump ... */
                if ((pDevObj->devProfileData.cpProtection != VP886_CP_PROT_DISABLED && !VP886_IS_SF(pDevObj)) &&
                    (pDevObj->registers.sigreg[2] & VP886_R_SIGREG_CPUVLO) == VP886_R_SIGREG_CPUVLO) {

                    /* Send a Device Init Complete event with the CP_FAIL flag.
                       Go to the VP886_INIT_DEVICE_GEN_EVENT state. */
                     //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device Failed to Reset Charge Pump Undervoltage"));
                    eventData = VP_DEV_INIT_CMP_CP_FAIL;

                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;

                    runAnotherState = TRUE;
                    break;

                }

                /* Verify that the channels are still in disconnect */
                pDevObj->ecVal = VP886_EC_1;
                VpSlacRegRead(pDevCtx, NULL, VP886_R_STATE_RD, VP886_R_STATE_LEN, &ssData1);
                pDevObj->ecVal = VP886_EC_2;
                VpSlacRegRead(pDevCtx, NULL, VP886_R_STATE_RD, VP886_R_STATE_LEN, &ssData2);
                pDevObj->ecVal = VP886_EC_GLOBAL;

                if ((ssData1 == 0x0F) || (ssData2 == 0x0F)) {

                    /* If channel 1 went back into shutdown ... */
                    if (ssData1 == 0x0F) {
                        /* Send a Device Init Complete event with the CH1_SD flag.
                           Go to the VP886_INIT_DEVICE_GEN_EVENT state. */
                         //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device Failed to Reset Ch1 Shutdown after CP bringup"));
                        eventData |= VP_DEV_INIT_CMP_CH1_SD;
                        pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                        runAnotherState = TRUE;
                        break;
                    }

                    /* If channel 2 went back into shutdown ... */
                    if (ssData2 == 0x0F) {
                        /* Send a Device Init Complete event with the CH2_SD flag.
                           Go to the VP886_INIT_DEVICE_GEN_EVENT state. */
                         //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device Failed to Reset Ch2 Shutdown after CP bringup"));
                        eventData |= VP_DEV_INIT_CMP_CH2_SD;
                        pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                        runAnotherState = TRUE;
                        break;
                    }

                }

                pDevObj->initDevSwitcherMode = (VP886_R_SWCTRL_MODE_Z_LP | VP886_R_SWCTRL_MODE_Y_LP);
                pDevObj->initDeviceState = VP886_INIT_DEVICE_CAL_CODEC_START;
                runAnotherState = TRUE;
                break;
            }
            case VP886_INIT_DEVICE_CAL_CODEC_START: {

                /* Set the first step of calCodec */
                pDevObj->calCodecState[0] = VP886_CAL_CODEC_START;

                pDevObj->initDeviceState = VP886_INIT_DEVICE_CAL_CODEC;
                runAnotherState = TRUE;
                break;
            }
            case VP886_INIT_DEVICE_CAL_CODEC: {

                //Vp886CalCodecHandler(pDevCtx);

                if (pDevObj->calCodecState[0] == VP886_CAL_CODEC_COMPLETE) {
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_SW_ENABLE;
                    runAnotherState = TRUE;
                } else if (pDevObj->calCodecState[0] == VP886_CAL_CODEC_FAILED) {
                    eventData |= VP_DEV_INIT_CMP_CAL_FAIL;
                     //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitDevice(): Device calibration failed."));
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                    runAnotherState = TRUE;
                }
                /* else calibration is ongoing. */
                break;
            }
            case VP886_INIT_DEVICE_SW_ENABLE: {
                uint8 calCtrl[VP886_R_CALCTRL_LEN];

                /* Turn on both switchers
                   This is accomplished by writing the Switching Regulator Control
                   Register value provided in the Device Profile forced to a
                   certain power mode */
                pDevObj->registers.swCtrl[0] &= ~(VP886_R_SWCTRL_MODE_Z | VP886_R_SWCTRL_MODE_Y);
                pDevObj->registers.swCtrl[0] |= pDevObj->initDevSwitcherMode;

                VpSlacRegWrite(pDevCtx, NULL, VP886_R_SWCTRL_WRT, VP886_R_SWCTRL_LEN, pDevObj->registers.swCtrl);

                /* Turn the switchers external circuits back on */
                calCtrl[0] = calCtrl[1] = calCtrl[2] = 0;
                /* For both channels */
                VpSlacRegWrite(pDevCtx, NULL, VP886_R_CALCTRL_WRT, VP886_R_CALCTRL_LEN, calCtrl);

                /* Wait a few ms. Come back and check for over current again */
                timerDuration = VP886_SW_EN_WAIT_TIME;
                pDevObj->initDeviceState = VP886_INIT_DEVICE_ALARM_CHECK;
                if (pDevObj->softStartDuration != 0) {
                    /* Soft startup - wait a longer time in low power */
                    timerDuration = pDevObj->softStartDuration * 5;
                }
                break;
            }
            case VP886_INIT_DEVICE_ALARM_CHECK: {
                bool alarm;

                /* Check for Switcher Overcurrent/Overvoltage, and CP Undervoltage */
                //alarm = Vp886InitDeviceAlarmCheck(pDevCtx, &eventData);

                /* If there was an alarm ... */
                if (alarm) {
                    /* Go to the VP886_INIT_DEVICE_GEN_EVENT state. */
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                    runAnotherState = TRUE;
                    break;
                }

                if (pDevObj->softStartDuration != 0) {
                    /* Soft startup - restore the normal low power timings */
                    //Vp886SetSwTimingParams(pDevCtx);
                }

                /* At this point, the charge pump is enabled, over voltage protection is enabled
                   and the switchers are on, regardless of what is specified
                   in the device profile. */

                /* If this is a tracker device */
                if (VP886_IS_TRACKER(pDevObj)) {

                    /* Start the calibration */
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_INIT_LINES;

                    runAnotherState = TRUE;
                    break;
                }

                /* This is an ABS device */

                if ((pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_MODE) ==
                    VP886_DEV_PROFILE_ABS_SUPP_CFG_SLAVE) {
                    /* In slave mode, leave switchers at low power and move on to
                       calibration */
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_INIT_LINES;

                } else if ((pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Y) == VP886_R_SWCTRL_MODE_Y_HP ||
                           (pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Z) == VP886_R_SWCTRL_MODE_Z_HP ||
                           ((pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Y) == VP886_R_SWCTRL_MODE_Y_MP &&
                            pDevObj->devProfileData.vblUseMedTiming == TRUE)) {
                    /* After reaching high power on either switcher, or medium on Y if the
                       vblUseMedTiming flag is set, move on to init lines */
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_INIT_LINES;

                } else if ((pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Y) == VP886_R_SWCTRL_MODE_Y_LP &&
                           (pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Z) == VP886_R_SWCTRL_MODE_Z_LP) {
                    /* From low power, go back to VP886_INIT_DEVICE_SW_ENABLE and change
                       to medium power.
                       If the vblUseMedTiming flag is set, move Y to medium and skip Z straight
                       to high so that they can use different timing params.  */
                    if (!(pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_Y_SLAVE)) {
                        pDevObj->initDevSwitcherMode &= ~VP886_R_SWCTRL_MODE_Y;
                        pDevObj->initDevSwitcherMode |= VP886_R_SWCTRL_MODE_Y_MP;
                    }
                    if (!(pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_Z_SLAVE)) {
                        if (pDevObj->devProfileData.vblUseMedTiming == TRUE) {
                            pDevObj->initDevSwitcherMode &= ~VP886_R_SWCTRL_MODE_Z;
                            pDevObj->initDevSwitcherMode |= VP886_R_SWCTRL_MODE_Z_HP;
                        } else {
                            pDevObj->initDevSwitcherMode &= ~VP886_R_SWCTRL_MODE_Z;
                            pDevObj->initDevSwitcherMode |= VP886_R_SWCTRL_MODE_Z_MP;
                        }
                    }
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_SW_ENABLE;

                } else if ((pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Y) == VP886_R_SWCTRL_MODE_Y_MP ||
                           (pDevObj->initDevSwitcherMode & VP886_R_SWCTRL_MODE_Z) == VP886_R_SWCTRL_MODE_Z_MP) {
                    /* From medium power, go back to VP886_INIT_DEVICE_SW_ENABLE and change
                       to high power */
                    if (!(pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_Y_SLAVE)) {
                        pDevObj->initDevSwitcherMode &= ~VP886_R_SWCTRL_MODE_Y;
                        pDevObj->initDevSwitcherMode |= VP886_R_SWCTRL_MODE_Y_HP;
                    }
                    if (!(pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_Z_SLAVE)) {
                        pDevObj->initDevSwitcherMode &= ~VP886_R_SWCTRL_MODE_Z;
                        pDevObj->initDevSwitcherMode |= VP886_R_SWCTRL_MODE_Z_HP;
                    }
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_SW_ENABLE;

                }

                runAnotherState = TRUE;
                break;
            }
            case VP886_INIT_DEVICE_INIT_LINES: {
                uint8 channel;

                /* Call Vp886InitLineInt() once for each channel. */
                for (channel = 0; channel < pDevObj->staticInfo.maxChannels; channel++) {
                    VpLineCtxType *pLineCtx = pDevCtx->pLineCtx[channel];
                    if (pLineCtx == VP_NULL) {
                        continue;
                    }

                    //status = Vp886InitLineInt(pLineCtx, VP_NULL, VP_NULL, VP_NULL, TRUE);
                    if (status != VP_STATUS_SUCCESS) {
                        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Vp886InitDevice(): Channel %d initialization failed!", channel));
                        eventData |= VP_DEV_INIT_CMP_LINE_FAIL;
                    }
                }

                /* Initialize options. */
                if (eventData == VP_DEV_INIT_CMP_SUCCESS) {
                    pDevObj->busyFlags |= VP_TEMP_IGNORE_ALL_BUSY_FLAGS;
                    pDevObj->busyFlags |= VP_DEV_IMPL_DEFAULT_OPTIONS;
                    //status = VpImplementDefaultSettings(pDevCtx, VP_NULL);
                    pDevObj->busyFlags &= ~VP_DEV_IMPL_DEFAULT_OPTIONS;
                    pDevObj->busyFlags &= ~VP_TEMP_IGNORE_ALL_BUSY_FLAGS;
                    if (status != VP_STATUS_SUCCESS) {
                        eventData |= VP_DEV_INIT_CMP_FAIL;
                    }
                }

                if (eventData != VP_DEV_INIT_CMP_SUCCESS) {
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                    runAnotherState = TRUE;
                    break;
                }

                /* Override the API default for adaptive ringing power with the
                   profile value */
                pDevObj->options.adaptiveRinging.power = pDevObj->devProfileData.adaptiveRingingMaxPower;

                if (VP886_IS_ABS(pDevObj) || pDevObj->calData[0].valid) {
                    /* Calibration factors already set */
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                } else {
                    pDevObj->busyFlags |= VP_DEV_IN_CAL;

                    for (channel = 0; channel < pDevObj->staticInfo.maxChannels; channel++) {
                        pDevObj->calCodecState[channel] = VP886_CAL_CODEC_LONGITUDINAL;
                        pDevObj->calCodecSubState[channel] = VP886_LONG_INIT;
                        pDevObj->channelCalBack[channel] = TRUE;
                    }

                    pDevObj->initDeviceState = VP886_INIT_DEVICE_LONG_CAL;
                }

                runAnotherState = TRUE;
                break;
            }
            case VP886_INIT_DEVICE_LONG_CAL: {
                uint8 channel, runChan = 0, queueSize = 0;
                uint8 otherChan = pDevObj->staticInfo.maxChannels - 1;

                for (channel = 0; channel < pDevObj->staticInfo.maxChannels; channel++) {
                    /* Check which channels need to be precessed */
                    if (pDevObj->channelCalBack[channel]) {
                        if (queueSize++ == 0) {
                            runChan = channel;
                        }
                    }
                }

                if (queueSize > 0) {
                    pDevObj->channelCalBack[runChan] = FALSE;
                    //Vp886LongitudinalCalibration(pDevCtx, runChan);
                    /* Check if another channel needs to be processes (max two channels per device) */
                    if (queueSize > 1) {
                        /* The first channel is always processed first if both channels are waiting,
                            if (queueSize > 1), it's always the second channel */
                        pDevObj->channelCalBack[1] = FALSE;
                        //Vp886LongitudinalCalibration(pDevCtx, 1);
                    }
                } else {
                     //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Unexpected longitudinal calibration error"));
                }

                /* If longitudinal calibration is done generate a complete event */
                if ((pDevObj->calCodecState[0] == VP886_CAL_CODEC_SYNC) &&
                    (pDevObj->calCodecState[otherChan] == VP886_CAL_CODEC_SYNC)) {
                    pDevObj->busyFlags &= ~VP_DEV_IN_CAL;
                    pDevObj->initDeviceState = VP886_INIT_DEVICE_GEN_EVENT;
                    runAnotherState = TRUE;
                }
                break;
            }
            case VP886_INIT_DEVICE_GEN_EVENT: {
                uint8 channel;

                /* Apply the cal if it was loaded from a calibration profile */
                for (channel = 0; channel < pDevObj->staticInfo.maxChannels; channel++) {
                    VpLineCtxType *pLineCtx = pDevCtx->pLineCtx[channel];

                    if (pLineCtx == VP_NULL) {
                        continue;
                    }

                    if (pDevObj->calData[channel].valid) {
                        /* Apply calibration for the line */
                        //Vp886ApplyCalGeneral(pLineCtx);
                        //Vp886ApplyCalDC(pLineCtx);
                        //Vp886ApplyCalRing(pLineCtx);
                        //Vp886ProgramCalRegisters(pLineCtx);
                    }
                }

                /* Return the second channel to shutdown if this is a single
                   channel tracking device */
                if (VP886_IS_TRACKER(pDevObj) && pDevObj->staticInfo.maxChannels == 1) {
                    uint8 ssData = VP886_R_STATE_SS_SHUTDOWN;
                    pDevObj->ecVal = VP886_EC_2;
                    VpSlacRegWrite(pDevCtx, NULL, VP886_R_STATE_WRT, VP886_R_STATE_LEN, &ssData);
                    pDevObj->ecVal = VP886_EC_GLOBAL;
                }

                /* For devices without both I/O pins, set the I/Os to output low for safety and
                   power saving. */
                switch (pDevObj->ioCapability) {
                    case VP886_IO_CAPABILITY_TWO_PER_CH:
                        if (pDevObj->staticInfo.maxChannels == 1) {
                            /* For single-channel parts, set the second channel IOs. */
                            uint8 ioDir = VP886_R_IODIR_IOD1_DIG_OUTPUT | VP886_R_IODIR_IOD2_OUTPUT;
                            uint8 ioData = 0x00;
                            pDevObj->ecVal = VP886_EC_2;
                            VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODIR_WRT, VP886_R_IODIR_LEN, &ioDir);
                            VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODATA_WRT, VP886_R_IODATA_LEN, &ioData);
                            pDevObj->ecVal = VP886_EC_GLOBAL;
                        }
                        break;
                    case VP886_IO_CAPABILITY_NONE: {
                        uint8 ioDir = VP886_R_IODIR_IOD1_DIG_OUTPUT | VP886_R_IODIR_IOD2_OUTPUT;
                        uint8 ioData = 0x00;
                        VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODIR_WRT, VP886_R_IODIR_LEN, &ioDir);
                        VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODATA_WRT, VP886_R_IODATA_LEN, &ioData);
                        break;
                    }
                    case VP886_IO_CAPABILITY_CH0_IO2: {
                        /* Set IO1 on the first channel and both IOs on the second channel */
                        uint8 ioDir;
                        uint8 ioData = 0x00;
                        pDevObj->ecVal = VP886_EC_1;
                        VpSlacRegRead(pDevCtx, VP_NULL, VP886_R_IODIR_RD, VP886_R_IODIR_LEN, &ioDir);
                        ioDir |= VP886_R_IODIR_IOD1_DIG_OUTPUT;
                        VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODIR_WRT, VP886_R_IODIR_LEN, &ioDir);
                        pDevObj->ecVal = VP886_EC_2;
                        ioDir = VP886_R_IODIR_IOD1_DIG_OUTPUT | VP886_R_IODIR_IOD2_OUTPUT;
                        VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODIR_WRT, VP886_R_IODIR_LEN, &ioDir);
                        pDevObj->ecVal = VP886_EC_GLOBAL;
                        VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_IODATA_WRT, VP886_R_IODATA_LEN, &ioData);
                    }
                    default:
                        break;
                }

                /* Unmask hook and gkey interrupts */
                pDevObj->registers.intMask[0] &= ~(VP886_R_SIGREG_GNK | VP886_R_SIGREG_HOOK);
                if (pDevObj->staticInfo.maxChannels == 2) {
                    pDevObj->registers.intMask[1] &= ~(VP886_R_SIGREG_GNK | VP886_R_SIGREG_HOOK);
                }
                VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_INTMASK_WRT, VP886_R_INTMASK_LEN, pDevObj->registers.intMask);

                pDevObj->busyFlags &= ~(VP_DEV_WARM_REBOOT | VP_DEV_INIT_IN_PROGRESS);
                if (eventData == VP_DEV_INIT_CMP_SUCCESS) {
                    /* Mark device as initialized to allow other functions to execute. */
                    pDevObj->busyFlags |= VP_DEV_INIT_CMP;
                } else {
                    /* Set global failure flag. */
                    eventData |= VP_DEV_INIT_CMP_FAIL;
                }
                Vp886PushEvent(pDevCtx, VP886_DEV_EVENT, VP_EVCAT_RESPONSE, VP_DEV_EVID_DEV_INIT_CMP,
                               eventData, Vp886GetTimestamp(pDevCtx), FALSE);
                pDevObj->initDeviceState = VP886_INIT_DEVICE_COMPLETE;
                break;
            }
            default:
                break;
        }

    }

    /* Set a timer, if requested */
    if (timerDuration != 0) {
        Vp886AddTimerMs(pDevCtx, NULL, VP886_TIMERID_INIT_DEVICE, timerDuration, 0, 0);
    }

    //VP_API_FUNC_INT(VpDevCtxType, pDevCtx, ("Vp886InitDeviceSM-"));

} /* Vp886InitDeviceSM */


/** Vp886InitDeviceData()
  Resets device object data during VpInitDevice().  This does not simply set
  all of the device object data to 0 because there are many variables that do
  not need to be reset, and others that must not be reset.

  Another possibility for this function would be to use memset to clear all of
  the device object data, while caching and restoring the parts that must not
  be reset.
*/
void
Vp886InitDeviceData(
        VpDevCtxType *pDevCtx)
{
    Vp886DeviceObjectType *pDevObj = (Vp886DeviceObjectType*)pDevCtx->pDevObj;

    /* Reset timestamp data */
    pDevObj->timestamp = 0;
    pDevObj->rolloverCount = 0;
    pDevObj->rolloverBuffer = 0;

    /* Reset API device state flags */
    if (pDevObj->busyFlags & VP_DEV_INIT_CMP) {
        pDevObj->busyFlags = (VP_DEV_WARM_REBOOT | VP_DEV_INIT_IN_PROGRESS);
    } else {
        pDevObj->busyFlags = VP_DEV_INIT_IN_PROGRESS;
    }
    pDevObj->stateInt &= (VP886_SYS_CAL_COMPLETE | VP886_DEVICE_CAL_COMPLETE);
    pDevObj->absPowerReq[0] = pDevObj->absPowerReq[1] = VP886_ABS_POWER_REQ_LOW;
    pDevObj->spiError.count = 0;
    pDevObj->spiError.block = FALSE;
    pDevObj->spiError.clearMpi = FALSE;
    pDevObj->spiError.skipGetEvent = FALSE;
    pDevObj->absPowerReq[0] = VP886_ABS_POWER_REQ_LOW;
    pDevObj->absPowerReq[1] = VP886_ABS_POWER_REQ_LOW;
    pDevObj->absRingingBattRequired[0] = FALSE;
    pDevObj->absRingingBattRequired[1] = FALSE;
    pDevObj->absRingingPeak[0] = 0;
    pDevObj->absRingingPeak[1] = 0;
    pDevObj->samplingTimerReq[0] = 0;
    pDevObj->samplingTimerReq[1] = 0;
    pDevObj->samplingTimerCurrent = 0;
    pDevObj->battFltStatus = 0;
    pDevObj->swSkipPresent = FALSE;
    pDevObj->batSelGpioState = 0;

    /* Reset the options cache */
    VpMemSet(&pDevObj->options, 0, sizeof(Vp886DevOptionsCacheType));

    /* Reset timer data */
    Vp886InitTimerQueue(pDevCtx);
#if (VP886_USER_TIMERS > 0)
    pDevObj->userTimers = 0;
#endif

    /* Reset event data */
    Vp886InitEventQueue(pDevCtx);
    pDevObj->getResultsRequired = FALSE;
}



VpStatusType Vp886InitDevice(
        VpDevCtxType *pDevCtx,
        VpProfilePtrType pDevProfile,
        VpProfilePtrType pAcProfile,
        VpProfilePtrType pDcProfile,
        VpProfilePtrType pRingProfile,
        VpProfilePtrType pFxoAcProfile,
        VpProfilePtrType pFxoCfgProfile)
{
    Vp886DeviceObjectType *pDevObj = (Vp886DeviceObjectType*)pDevCtx->pDevObj;
    VpProfilePtrType pProf;
    uint8 mpiLen;
    uint8 shutdownState = VP886_R_STATE_SS_SHUTDOWN;
    VpStatusType status;

    Vp886EnterCritical(pDevCtx, VP_NULL, (const char *)"Vp886InitDevice");

    /* Device profile is required. */
    if (pDevProfile == VP_PTABLE_NULL) {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device profile is required."));
        Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return VP_STATUS_ERR_PROFILE;
    }

    /* Reset device object variables */
    Vp886InitDeviceData(pDevCtx);

    /* Clear the MPI buffer before trying to access the device, in case there
       was a command issued earlier that is still expecting data.  Sending a
       stream of no-ops will satisfy the device's need for more data, and leave
       it in a state where it is ready to accept a command.  We're calling the
       common ClearMPIBuffer function twice because the longest command length
       for ZL880 is longer than it was for other CSLAC devices. */
    VpCSLACClearMPIBuffer(pDevObj->deviceId);
    VpCSLACClearMPIBuffer(pDevObj->deviceId);

    /* Hardware Reset the part, but first make sure the switchers are shut down
       to avoid catching a switcher duty cycle on with the reset signal. */
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_STATE_WRT, VP886_R_STATE_LEN, &shutdownState);
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_NOOP_WRT, 0, VP_NULL);
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_NOOP_WRT, 0, VP_NULL);
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_NOOP_WRT, 0, VP_NULL);
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_HWRESET_WRT, 0, VP_NULL);

    /* Read and verify the revision and product codes.  Need to know this now
       so we can verify that the profiles are targeted for the correct device. */
    status = Vp886InitDevicePcnRcn(pDevCtx);
    if (status != VP_STATUS_SUCCESS) {
        pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
        Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return status;
    }

#ifdef VP886_INCLUDE_MPI_QUICK_TEST
    /* Run a quick test of the MPI interface and HAL layer code */
    status = Vp886QuickMpiTest(pDevCtx);
    if (status != VP_STATUS_SUCCESS) {
        pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
        Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return status;
    }
#endif /* VP886_INCLUDE_MPI_QUICK_TEST */

    /* Check for valid profile arguments. */
    if ((Vp886GetProfileArg(pDevCtx, VP_PROFILE_DEVICE, &pDevProfile) != VP_STATUS_SUCCESS) ||
        (Vp886GetProfileArg(pDevCtx, VP_PROFILE_AC, &pAcProfile) != VP_STATUS_SUCCESS) ||
        (Vp886GetProfileArg(pDevCtx, VP_PROFILE_DC, &pDcProfile) != VP_STATUS_SUCCESS) ||
        (Vp886GetProfileArg(pDevCtx, VP_PROFILE_RING, &pRingProfile) != VP_STATUS_SUCCESS)
        /* Assuming no FXO support. So don't check the FXO related profiles */
            ) {
        pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
        Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return VP_STATUS_ERR_PROFILE;
    }

    /* Make sure the Device Ctxt Device Type, and the Device Profile Device Type
       are in agreement */
    if (pDevProfile[VP_PROFILE_TYPE_MSB] != pDevCtx->deviceType) {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device profile device type %02X does not match specified device type %02X",
        //        pDevProfile[VP_PROFILE_TYPE_MSB], pDevCtx->deviceType));
        pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
        Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return VP_STATUS_ERR_PROFILE;
    }

    /* If this flag is set, the actual device is ABS, and the requested
       deviceType is tracker, treat the ABS device as a 1-channel tracker. */
    pProf = pDevProfile + VP_PROFILE_MPI_LEN + pDevProfile[VP_PROFILE_MPI_LEN] + 1 + VP886_DEV_PROFILE_ABS_TO_1CH_TRK_OFFSET;
    if ((*pProf & VP886_DEV_PROFILE_ABS_TO_1CH_TRK) &&
        VP886_IS_ABS(pDevObj) &&
        pDevCtx->deviceType == VP_DEV_887_SERIES &&
        pDevProfile[VP_PROFILE_TYPE_MSB] == VP_DEV_887_SERIES)
    {
        ////VP_INFO(VpDevCtxType, pDevCtx, ("Treating ABS device as 1ch Tracker"));
        /* Setting this bit changes the ABS PCN to tracker */
        pDevObj->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX] |= VP886_R_RCNPCN_PCN_ABS_TRACKER;
        /* Set a flag to call Vp886ChangeDeviceType() later on to
           actually reprogram the device to run in the other mode. */
        pDevObj->stateInt |= VP886_CONVERT_DEVICE_TYPE;
        /* Limit max channels to 1 */
        pDevObj->staticInfo.maxChannels = 1;
    }

    /* Check that the product code matches the device type that was passed in
       to VpMakeDeviceObject */
    if (VP886_DEVICE_SERIES(pDevObj) != pDevCtx->deviceType) {
        // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Specified device type %02X does not match the device PCN 0x%02X",
        //        pDevCtx->deviceType, pDevObj->staticInfo.rcnPcn[VP886_R_RCNPCN_PCN_IDX]));
        pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
        Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
        return VP_STATUS_ERR_VTD_CODE;
    }

    /* Validate and store the PCM Clock rate from the device profile */
    pDevObj->devProfileData.devCfg = pDevProfile[VP886_DEV_PROFILE_DEV_CFG_DATA0];
    switch(pDevObj->devProfileData.devCfg & VP886_R_DEVCFG_CLKSEL) {
        case VP886_R_DEVCFG_CLKSEL_1536:
            pDevObj->devProfileData.pcmClkRate = 1536;
            break;
        case VP886_R_DEVCFG_CLKSEL_1544:
            pDevObj->devProfileData.pcmClkRate = 1544;
            break;
        case VP886_R_DEVCFG_CLKSEL_2048:
            pDevObj->devProfileData.pcmClkRate = 2048;
            break;
        case VP886_R_DEVCFG_CLKSEL_1024:
            pDevObj->devProfileData.pcmClkRate = 1024;
            break;
        case VP886_R_DEVCFG_CLKSEL_3072:
            pDevObj->devProfileData.pcmClkRate = 3072;
            break;
        case VP886_R_DEVCFG_CLKSEL_3088:
            pDevObj->devProfileData.pcmClkRate = 3088;
            break;
        case VP886_R_DEVCFG_CLKSEL_4096:
            pDevObj->devProfileData.pcmClkRate = 4096;
            break;
        case VP886_R_DEVCFG_CLKSEL_6144:
            pDevObj->devProfileData.pcmClkRate = 6144;
            break;
        case VP886_R_DEVCFG_CLKSEL_6176:
            pDevObj->devProfileData.pcmClkRate = 6176;
            break;
        case VP886_R_DEVCFG_CLKSEL_8192:
            pDevObj->devProfileData.pcmClkRate = 8192;
            break;
        default:
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Invalid PCM Clock Rate selection in device profile, 0x%02X",
            //        pDevObj->devProfileData.devCfg));
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return VP_STATUS_ERR_PROFILE;
    }
    if (VP886_IS_SF(pDevObj)) {
        if (pDevObj->devProfileData.pcmClkRate % 512 != 0) {
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Invalid PCM Clock Rate selection in device profile, %dkHz",
            //        pDevObj->devProfileData.pcmClkRate));
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return VP_STATUS_ERR_PROFILE;
        }
    }

    /* The device profile is good. Save a pointer to it. This pointer
       is used only in the first state of the Vp886InitDevice() state
       machine. The pointer cannot be guaranteed to be valid after that
       and should not be used elsewhere. */
    pDevObj->pDevProfile = pDevProfile;

    /* Substitute in default profiles for any that are not provided */
    Vp886SubDefaultProfiles(pDevCtx, &pAcProfile, &pDcProfile, &pRingProfile);

    /* Save a pointer to the AC profile. This pointer
       is used only in the first state of the Vp886InitDevice() state
       machine. The pointer cannot be guaranteed to be valid after that
       and should not be used elsewhere. */
    pDevObj->pAcProfile = pAcProfile;

    /* Save a pointer to the DC profile. This pointer
       is used only in the first state of the Vp886InitDevice() state
       machine. The pointer cannot be guaranteed to be valid after that
       and should not be used elsewhere. */
    pDevObj->pDcProfile = pDcProfile;

    /* Save a pointer to the Ring profile. This pointer
       is used only in the first state of the Vp886InitDevice() state
       machine. The pointer cannot be guaranteed to be valid after that
       and should not be used elsewhere. */
    pDevObj->pRingProfile = pRingProfile;


    /* Store configuration and registers from device profile */
    mpiLen = pDevProfile[VP_PROFILE_MPI_LEN];
    pProf = pDevProfile + VP_PROFILE_MPI_LEN + mpiLen + 1;

    /* Store the I/O 2 use from the Dev profile */
    pDevObj->devProfileData.io2Use = *pProf;
    pProf += 1;

    /* Store dial pulse correction value from the Dev profile */
    pDevObj->devProfileData.dialPulseCorrection = *pProf;
    pDevObj->devProfileData.dialPulseCorrection &= VP886_DEV_PROFILE_PD_CORR_MASK;
    pDevObj->devProfileData.dialPulseCorrection >>= 4;
    /* Determine low voltage override from the Dev profile */
    pDevObj->devProfileData.swCfg = (*pProf & VP886_DEV_PROFILE_SW_CONF_MASK);
    if (VP886_IS_TRACKER(pDevObj)) {
        switch (pDevObj->devProfileData.swCfg) {
            case VP886_DEV_PROFILE_SW_CONF_BB:
            case VP886_DEV_PROFILE_SW_CONF_FB100:
            case VP886_DEV_PROFILE_SW_CONF_IB100:
            case VP886_DEV_PROFILE_SW_CONF_IB100_5IN:
                pDevObj->devProfileData.lowVoltOverride = TRUE;
                break;
            case VP886_DEV_PROFILE_SW_CONF_FB150:
            case VP886_DEV_PROFILE_SW_CONF_IB150:
                pDevObj->devProfileData.lowVoltOverride = FALSE;
                break;
            default:
                // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Invalid switcher configuration 0x%02X in device profile", pDevObj->devProfileData.swCfg));
                pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
                Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
                return VP_STATUS_ERR_PROFILE;
        }
    } else {
        switch (pDevObj->devProfileData.swCfg) {
            case VP886_DEV_PROFILE_SW_CONF_BB:
            case VP886_DEV_PROFILE_SW_CONF_ABS100:
            case VP886_DEV_PROFILE_SW_CONF_ABS120:
                pDevObj->devProfileData.lowVoltOverride = TRUE;
                break;
            default:
                // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Invalid switcher configuration 0x%02X in device profile", pDevObj->devProfileData.swCfg));
                pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
                Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
                return VP_STATUS_ERR_PROFILE;
        }
    }
    pProf += 1;

    /* Save Switcher Over-Current Count */
    pDevObj->devProfileData.swOCC[0] = pProf[0];
    pDevObj->devProfileData.swOCC[1] = pProf[1];
    pProf += 2;

    /* Save ABS Supply Configuration */
    pDevObj->devProfileData.absSuppCfg = *pProf;
    /* Correct a problem from Profile Wizard P2.3.2 and earlier where both supply
       bits can be set to slave mode without the main bit being set for slave
       mode. */
    if ((pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_Y_SLAVE) &&
        (pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_Z_SLAVE))
    {
        pDevObj->devProfileData.absSuppCfg &= ~VP886_DEV_PROFILE_ABS_SUPP_CFG_MODE;
        pDevObj->devProfileData.absSuppCfg |=  VP886_DEV_PROFILE_ABS_SUPP_CFG_SLAVE;
    }
    switch (pDevObj->devProfileData.absSuppCfg & VP886_DEV_PROFILE_ABS_SUPP_CFG_MODE) {
        case VP886_DEV_PROFILE_ABS_SUPP_CFG_SINGLE:
        case VP886_DEV_PROFILE_ABS_SUPP_CFG_SLAVE:
        case VP886_DEV_PROFILE_ABS_SUPP_CFG_MASTER:
            break;
        default:
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Illegal ABS Supply Configuration"));
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return VP_STATUS_ERR_PROFILE;
    }
    /* Save Leading Edge Blanking Window */
    pDevObj->devProfileData.blanking = (*pProf & VP886_DEV_PROFILE_BLANKING_USE_BITS);
    pProf += 1;

    /* Copy the Free Run Switching regulator timing parameters from the Dev profile */
    VpMemCpy(pDevObj->swTimingParamsFR, pProf, VP886_R_SWTIMING_LEN);
    pProf += VP886_R_SWTIMING_LEN;

    if (VP886_IS_ABS(pDevObj)) {
        /* Store the SWY Voltage from the Dev profile. Applies to ABS only */
        pDevObj->devProfileData.swyv = *pProf;
        pDevObj->swyv = *pProf;
        pProf += 1;

        /* Store the SWZ Voltage from the Dev profile. Applies to ABS only */
        pDevObj->devProfileData.swzv = *pProf;
        pDevObj->swzv = *pProf;
        pProf += 1;
    }

    /* Disable Adaptive Ringing by default (override whatever the profile says) */
    pDevObj->devProfileData.adaptiveRingingMaxPower = VP_ADAPTIVE_RINGING_DISABLED;
    pProf += 1;

    /* Save the low power switcher timings (version 2+) */
    if (pDevProfile[VP_PROFILE_VERSION] < 2) {
        pDevObj->useLowPowerTiming = FALSE;
    } else {
        pDevObj->useLowPowerTiming = TRUE;
        pDevObj->swTimingParamsLP[0] = pProf[0];
        pDevObj->swTimingParamsLP[1] = pProf[1];
        pProf += 2;
    }

    /* Version 3+ */
    if (pDevProfile[VP_PROFILE_VERSION] < 3) {
        /* Defaults */
        pDevObj->devProfileData.swyLimit = 0;
        pDevObj->devProfileData.swzLimit = 0;
        pDevObj->devProfileData.cpEnable = TRUE;
        pDevObj->devProfileData.cpProtection = VP886_CP_PROT_CYCLE_SKIP;
        if (pDevObj->stateInt & VP886_SHARED_SUPPLY) {
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Shared supply device detected, but device profile does not support shared supply."));
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        /* Save the SWY and SWZ limit voltages */
        pDevObj->devProfileData.swyLimit = pProf[0];
        pDevObj->devProfileData.swzLimit = pProf[1];
        pProf += 2;

        /* Save charge pump enable/disable setting */
        pDevObj->devProfileData.cpEnable = ((*pProf) & VP886_DEV_PROFILE_CP_ENABLE_MASK);
        /* Save the charge pump protection mode */
        pDevObj->devProfileData.cpProtection = (Vp886CPProtectionModeType)(((*pProf) & VP886_DEV_PROFILE_CP_PROTECTION_MASK) >> 1);
        /* If we detected a shared supply device, make sure the device profile
           specifies shared supply. */
        if ((pDevObj->stateInt & VP886_SHARED_SUPPLY) && !(*pProf & VP886_DEV_PROFILE_SHARED_SUPPLY)) {
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Shared supply device detected, but device profile does not specify shared supply."));
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return VP_STATUS_ERR_PROFILE;
        }
        /* If the device profile specifies shared supply, make sure we detected a
           shared supply device. */
        if ((*pProf & VP886_DEV_PROFILE_SHARED_SUPPLY) && !(pDevObj->stateInt & VP886_SHARED_SUPPLY)) {
            // //VP_ERROR
 //(VpDevCtxType, pDevCtx, ("Device profile specifies shared supply, but shared supply device not detected."));
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return VP_STATUS_ERR_PROFILE;
        }
        pProf += 1;
    }

    /* Version 4+ */
    if (pDevProfile[VP_PROFILE_VERSION] < 4) {
        /* Defaults */
        pDevObj->devProfileData.vsw = 0;
        pDevObj->devProfileData.vbhOffset = 0;
        pDevObj->devProfileData.vbhOverhead = 0;
    } else {
        pDevObj->devProfileData.vsw = *pProf;
        pProf += 1;
        if (VP886_IS_ABS(pDevObj)) {
            pDevObj->devProfileData.vbhOffset = *pProf;
            pProf += 1;
        }
    }

    /* Version 5+ */
    if (pDevProfile[VP_PROFILE_VERSION] < 5) {
        /* Defaults */
        pDevObj->devProfileData.vbhOverhead = 0;
    } else {
        if (VP886_IS_ABS(pDevObj)) {
            pDevObj->devProfileData.vbhOverhead = *pProf;
            pProf += 1;
        } else {
            pDevObj->devProfileData.vbhOverhead = 0;
        }
    }

    /* Version 6+ */
    if (pDevProfile[VP_PROFILE_VERSION] < 6) {
        /* Defaults */
        VpMemSet(pDevObj->swTimingParamsBattBackup, 0x00, VP886_R_SWTIMING_LEN);
    } else {
        /* Copy the battery backup Switching regulator timing parameters from the Dev profile */
        VpMemCpy(pDevObj->swTimingParamsBattBackup, pProf, VP886_R_SWTIMING_LEN);
        pProf += VP886_R_SWTIMING_LEN;
    }

    /* Version 7+ */
    if (pDevProfile[VP_PROFILE_VERSION] < 7) {
        /* Defaults */
        pDevObj->swTimingParamsSoftStart[0] = 0x00;
        pDevObj->swTimingParamsSoftStart[1] = 0x00;
        pDevObj->softStartDuration = 0;
    } else {
        /* Get the soft startup timing params and duration */
        pDevObj->swTimingParamsSoftStart[0] = pProf[0];
        pDevObj->swTimingParamsSoftStart[1] = pProf[1];
        pDevObj->softStartDuration = pProf[2];
        pProf += 3;
    }

    /* Version 8+ */
    if (pDevProfile[VP_PROFILE_VERSION] < 8) {
        /* Defaults */
        pDevObj->devProfileData.swCtrlFreeRun = 0xFF;
        pDevObj->devProfileData.swCtrlBattBackup = 0xFF;
        pDevObj->devProfileData.batSelGpio = VP886_DEV_PROFILE_BAT_SEL_GPIO_NONE;
        pDevObj->devProfileData.vblUseMedTiming = FALSE;
    } else {
        pDevObj->devProfileData.swCtrlFreeRun = pProf[0];
        pDevObj->devProfileData.swCtrlBattBackup = pProf[1];
        pProf += 2;
        if (VP886_IS_ABS(pDevObj)) {
            pDevObj->devProfileData.batSelGpio = (pProf[0] & VP886_DEV_PROFILE_BAT_SEL_MASK);
            if ((pProf[0] & VP886_DEV_PROFILE_VBL_MASTER_TIMING_MASK) == VP886_DEV_PROFILE_VBL_MASTER_TIMING_MED) {
                pDevObj->devProfileData.vblUseMedTiming = TRUE;
            } else {
                pDevObj->devProfileData.vblUseMedTiming = FALSE;
            }
            pProf += 1;
        } else {
            pDevObj->devProfileData.batSelGpio = VP886_DEV_PROFILE_BAT_SEL_GPIO_NONE;
            pDevObj->devProfileData.vblUseMedTiming = FALSE;
        }
    }

    if (VP886_IS_ABS(pDevObj) && pDevObj->devProfileData.batSelGpio != VP886_DEV_PROFILE_BAT_SEL_GPIO_NONE) {
        status = Vp886InitBatSelGpio(pDevCtx);
        if (status != VP_STATUS_SUCCESS) {
            pDevObj->busyFlags &= ~VP_DEV_INIT_IN_PROGRESS;
            Vp886ExitCritical(pDevCtx, VP_NULL, "Vp886InitDevice");
            return status;
        }
    }

    /* If this flag is set, it means that the device is capable of being
       reprogrammed as either tracker or ABS, and the provided deviceType does
       not match the device's default PCN. */
    if (pDevObj->stateInt & VP886_CONVERT_DEVICE_TYPE) {
        //Vp886ChangeDeviceType(pDevCtx);
    }

    /* Unmask all device interrupts except for those we know we will not use.
       - IO2 interrupts may be unmasked in Vp886InitLineInt if the pins are
         specified as interrupt inputs.
       - Mask hook and gkey interrupts during initialization.  We ignore them
         anyway, so masking them reduces unnecessary interrupt handling
       - Mask second-channel interrupts for single-channel devices */
    pDevObj->registers.intMask[0] = VP886_R_INTMASK_IO2 | VP886_R_SIGREG_GNK | VP886_R_SIGREG_HOOK;
    pDevObj->registers.intMask[1] = VP886_R_INTMASK_IO2 | VP886_R_SIGREG_GNK | VP886_R_SIGREG_HOOK;
    pDevObj->registers.intMask[2] = 0x00;
    pDevObj->registers.intMask[3] = 0x00;
    if (pDevObj->staticInfo.maxChannels == 1) {
        pDevObj->registers.intMask[1] |= VP886_R_INTMASK_OCALM | VP886_R_INTMASK_TEMPA |
                                         VP886_R_INTMASK_CAD | VP886_R_INTMASK_CID;
        pDevObj->registers.intMask[3] |= VP886_R_INTMASK_OVALM;
    }
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_R_INTMASK_WRT, VP886_R_INTMASK_LEN, pDevObj->registers.intMask);

    /* Apply device profile MPI section */
    VpSlacRegWrite(pDevCtx, VP_NULL, VP886_NOOP, pDevProfile[VP_PROFILE_MPI_LEN],
                   &pDevProfile[VP_PROFILE_DATA_START]);

    /* Read back data from the profile MPI section that we'll need later */
    VpSlacRegRead(pDevCtx, NULL, VP886_R_SWTIMING_RD, VP886_R_SWTIMING_LEN, pDevObj->swTimingParams);
    VpSlacRegRead(pDevCtx, VP_NULL, VP886_R_SWPARAM_RD, VP886_R_SWPARAM_LEN, pDevObj->swParams);
    VpSlacRegRead(pDevCtx, NULL, VP886_R_DEVMODE_RD, VP886_R_DEVMODE_LEN, pDevObj->devProfileData.devMode);
    VpSlacRegRead(pDevCtx, NULL, VP886_R_SWCTRL_RD, VP886_R_SWCTRL_LEN, pDevObj->registers.swCtrl);
    pDevObj->devProfileData.swCtrlNormal = pDevObj->registers.swCtrl[0];
    pDevObj->registers.devCfg[0] = pDevObj->devProfileData.devCfg;

    /* Defaulting the VpInitDevice state variable to the first state */
    pDevObj->initDeviceState = VP886_INIT_DEVICE_SWITCHER_PREP;

    /* Call into the state machine for the first time to set things off */
    Vp886InitDeviceSM(pDevCtx);

    Vp886ExitCritical(pDevCtx, VP_NULL, (const char *)"Vp886InitDevice");

    return VP_STATUS_SUCCESS;

} /* Vp886InitDevice */
