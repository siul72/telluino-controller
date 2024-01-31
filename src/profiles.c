/*
 * profiles.c --
 *
 * This file contains profile data in byte format
 *
 * Project Info --
 *   File:   U:\dbuckley\ztap\trunk\applications\quickstarts\sdk_examples\Le9641_DTMF_CID\profiles.vpw
 *   Type:   miSLIC- Le9641 - Buck-Boost Fixed Ringing
 *   Date:   Thursday, April 21, 2016 16:06:25
 *   Device: miSLIC Le9641
 *
 *   This file was generated with Profile Wizard Version: P2.8.0
 */

#include "profiles.h"


/************** Device Profile **************/

/* Device Configuration Data - 5V 22uH 1.5A Buck-Boost */
const VpProfileDataType DEV_PROFILE_Buck_Boost[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0xFF, 0x00,       /* Device Profile for Le9641 device */
   0x31,                   /* Profile length = 49 + 4 = 53 bytes */
   0x07,                   /* Profile format version = 7 */
   0x14,                   /* MPI section length = 20 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0x46, 0x0A,             /* PCLK = 8.192 MHz; INTM = CMOS-compatible */
   0x44, 0x06,             /* PCM Clock Slot = 6 TX / 0 RX; XE = Neg. */
   0x5E, 0x14, 0x00,       /* Device Mode Register */
   0xF6, 0x95, 0x00, 0x94, /* Switching Regulator Timing Params */
         0x40, 0xE4, 0x40,
   0xE4, 0x44, 0x8F, 0x0A, /* Switching Regulator Params */
   0xE6, 0x60,             /* Switching Regulator Control */
/* Formatted Parameters Section -------------------------------------------- */
   0x00,                   /* IO21 Mode: Digital */
                           /* IO22 Mode: Digital */
   0x40,                   /* Dial pulse correction: 4 ms */
                           /* Switcher Configuration =
                              BuckBoost 22uH 1.5A miSLIC (5V in, 100 V out) */
   0x00, 0x00,             /* Over-current event threshold = Off */
   0x01,                   /* Leading edge blanking window = 81 ns */
   0x30, 0x14, 0x30, 0x14, /* FreeRun Timing Parameters */
         0x30, 0x14,
   0xFF,
   0x95, 0x00,             /* Low-Power Timing Parameters */
   0x78, 0x78,             /* Switcher Limit Voltages 120 V, 120 V */
   0x44,                   /* Charge pump disabled */
                           /* Charge Pump Overload Protection: Disabled */
   0x3C,                   /* Switcher Input 12 V */
   0x00, 0x00, 0x00, 0x00, /* Battery Backup Timing Parameters */
         0x00, 0x00,
   0x00, 0x00, 0x00        /* Soft Startup Timing (Disabled) */
};

/************** DC Profile **************/

/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */
const VpProfileDataType DC_FXS_miSLIC_BB_DEF[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x01, 0x00,       /* DC Profile for Le9641 device */
   0x0C,                   /* Profile length = 12 + 4 = 16 bytes */
   0x02,                   /* Profile format version = 2 */
   0x03,                   /* MPI section length = 3 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC6, 0x92, 0x27,       /* DC Feed Parameters: ir_overhead = 100 ohm; */
                           /* VOC = 48 V; LI = 50 ohm; VAS = 8.78 V; ILA = 25 mA */
                           /* Maximum Voice Amplitude = 2.93 V */
/* Formatted Parameters Section -------------------------------------------- */
   0x1C,                   /* Switch Hook Threshold = 11 mA */
                           /* Ground-Key Threshold = 18 mA */
   0x84,                   /* Switch Hook Debounce = 8 ms */
                           /* Ground-Key Debounce = 16 ms */
   0x58,                   /* Low-Power Switch Hook Hysteresis = 2 V */
                           /* Ground-Key Hysteresis = 6 mA */
                           /* Switch Hook Hysteresis = 2 mA */
   0x80,                   /* Low-Power Switch Hook Threshold = 22 V */
   0x03,                   /* Floor Voltage = -20 V */
   0x00,                   /* R_OSP = 0 ohms */
   0x07                    /* R_ISP = 7 ohms */
};

/************** AC Profile **************/

/* AC FXS RF14 600R Normal Coefficients (Default)  */
const VpProfileDataType AC_FXS_RF14_600R_DEF[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/************** Ringing Profile **************/

/* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */
const VpProfileDataType RING_miSLIC_BB_DEF[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0D, 0x04, 0x00,       /* Ringing Profile for Le9641 device */
   0x12,                   /* Profile length = 18 + 4 = 22 bytes */
   0x01,                   /* Profile format version = 1 */
   0x0C,                   /* MPI section length = 12 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC0, 0x08, 0x00, 0x00, /* Ringing Generator Parameters: */
         0x00, 0x44, 0x3A, /* 24.9 Hz Sine; 1.41 CF; 70.70 Vpk; 0.00 V bias */
         0x9D, 0x00, 0x00, /* Ring trip cycles = 1; RTHALFCYC = 0 */
         0x00, 0x00,
/* Formatted Parameters Section -------------------------------------------- */
   0xAA,                   /* Ring Trip Method = AC; Threshold = 21.0 mA */
   0x02,                   /* Ringing Current Limit = 54 mA */
   0x4E,                   /* Fixed; Max Ringing Supply = 75 V */
   0x00                    /* Balanced; Ring Cadence Control Disabled */
};

/************** Tone Profile **************/

/************** Cadence Profile **************/

/* China Ringing Signal Cadence */
const VpProfileDataType CR_CN[] =
{
  /* Cadence Profile */
 0x00, 0x08, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1A, 0x01, 0x07, 0x20, 0xC8,
 0x01, 0x05, 0x20, 0x28, 0x02, 0x01, 0x22, 0xF8, 0x01, 0x07, 0x20, 0xC8,
 0x01, 0x00, 0x20, 0x28, 0x01, 0x05, 0x22, 0xF8, 0x46, 0x00
};

/************** Caller ID Profile **************/

/* DTMF CID Type1 */
const VpProfileDataType CID_DTMF[] =
{
  /* Space=2200Hz, Mark=1200Hz, Amp=-10.00dBm */
  /* Caller ID Profile */
  0x00, 0x05, 0x00, 0x14, 0x00, 0x00,
  0x09, 0xD4, /* MPI Length and Command */
  0x17, 0x77, 0x1C, 0x32, 0x0C, 0xCC, 0x1C, 0x32, /* MPI Data */
  0x00, 0x00, /* Checksum Part of Message Data */
  0x00, 0x04, /* Length of Elements Data */
  0x00, 0x0E, /* Message Data (DTMF Format) */
  0x00, 0x0D  /* End of Transmission */
};

/************** Metering_Profile **************/

/* end of file profiles.c */
