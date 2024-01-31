/*
 * profiles.h --
 *
 * This header file exports the Profile data types
 *
 * Project Info --
 *   File:   U:\dbuckley\ztap\trunk\applications\quickstarts\sdk_examples\Le9641_DTMF_CID\profiles.vpw
 *   Type:   miSLIC- Le9641 - Buck-Boost Fixed Ringing
 *   Date:   Thursday, April 21, 2016 16:06:25
 *   Device: miSLIC Le9641
 *
 *   This file was generated with Profile Wizard Version: P2.8.0
 */

#ifndef PROFILES_H
#define PROFILES_H

#ifdef VP_API_TYPES_H
#include "vp_api_types.h"
#else
typedef unsigned char VpProfileDataType;
#endif


/************** Device Profile **************/
extern const VpProfileDataType DEV_PROFILE_Buck_Boost[];/* Device Configuration Data - 5V 22uH 1.5A Buck-Boost */

/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_miSLIC_BB_DEF[];/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */

/************** AC Profile **************/
extern const VpProfileDataType AC_FXS_RF14_600R_DEF[];/* AC FXS RF14 600R Normal Coefficients (Default)  */

/************** Ringing Profile **************/
extern const VpProfileDataType RING_miSLIC_BB_DEF[]; /* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */

/************** Tone Profile **************/

/************** Cadence Profile **************/
extern const VpProfileDataType CR_CN[];              /* China Ringing Signal Cadence */

/************** Caller ID Profile **************/
extern const VpProfileDataType CID_DTMF[];           /* DTMF CID Type1 */

/************** Metering_Profile **************/

#endif /* PROFILES_H */

