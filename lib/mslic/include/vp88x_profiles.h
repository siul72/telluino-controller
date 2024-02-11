//
// Created by siul72 on 11.02.2024.
//

#ifndef TELLUINO_CONTROLLER_VP88X_PROFILES_H
#define TELLUINO_CONTROLLER_VP88X_PROFILES_H

#define VP886_DEV_PROFILE_ABS_SUPP_CFG_Y_SLAVE  0x10
#define VP886_DEV_PROFILE_ABS_SUPP_CFG_Z_SLAVE  0x20

#define VP886_DEV_PROFILE_BLANKING_USE_BITS     0x07

#define VP886_DEV_PROFILE_ABS_SUPP_CFG_MODE     0xC0
#define VP886_DEV_PROFILE_ABS_SUPP_CFG_SINGLE   0x00
#define VP886_DEV_PROFILE_ABS_SUPP_CFG_SLAVE    0x40
#define VP886_DEV_PROFILE_ABS_SUPP_CFG_MASTER   0x80

#define VP886_DEV_PROFILE_CP_ENABLE_MASK        0x01
#define VP886_DEV_PROFILE_CP_PROTECTION_MASK    0x0E
#define VP886_DEV_PROFILE_SHARED_SUPPLY         0x80
#define VP886_DEV_PROFILE_ABS_TO_1CH_TRK_OFFSET   16
#define VP886_DEV_PROFILE_ABS_TO_1CH_TRK        0x40

#define VP886_DEV_PROFILE_BAT_SEL_MASK          0x0F
#define VP886_DEV_PROFILE_BAT_SEL_GPIO_NONE     0x00
#define VP886_DEV_PROFILE_BAT_SEL_GPIO_IO11     0x01
#define VP886_DEV_PROFILE_BAT_SEL_GPIO_IO12     0x02
#define VP886_DEV_PROFILE_BAT_SEL_GPIO_IO21     0x03
#define VP886_DEV_PROFILE_BAT_SEL_GPIO_IO22     0x04

#define VP886_DEV_PROFILE_VBL_MASTER_TIMING_MASK    0x80
#define VP886_DEV_PROFILE_VBL_MASTER_TIMING_HIGH    0x00
#define VP886_DEV_PROFILE_VBL_MASTER_TIMING_MED     0x80

#define VP886_DEV_PROFILE_IO2_USE_BITS          0x03
#define VP886_DEV_PROFILE_IO2_USE_DIG_SIG       0x00
#define VP886_DEV_PROFILE_IO2_USE_DIG_INT       0x01
#define VP886_DEV_PROFILE_IO2_USE_VMM           0x02
#define VP886_DEV_PROFILE_IO2_USE_RSVD          0x03

#define VP886_NUM_PROFILE_TYPES 9
#define VP886_DEV_PROFILE_ABS_TO_1CH_TRK_OFFSET   16
#define VP886_DEV_PROFILE_ABS_TO_1CH_TRK        0x40
#define VP886_DEV_PROFILE_DEV_CFG_DATA0         (7u)
#define VP886_DEV_PROFILE_PD_CORR_MASK          0xF0
#define VP886_DEV_PROFILE_SW_CONF_MASK          0x0F
#define VP886_DEV_PROFILE_SW_CONF_BB            0x00
#define VP886_DEV_PROFILE_SW_CONF_FB100         0x01
#define VP886_DEV_PROFILE_SW_CONF_FB150         0x02
#define VP886_DEV_PROFILE_SW_CONF_IB100         0x03
#define VP886_DEV_PROFILE_SW_CONF_IB150         0x04
#define VP886_DEV_PROFILE_SW_CONF_IB100_5IN     0x05
#define VP886_DEV_PROFILE_SW_CONF_ABS100        0x01
#define VP886_DEV_PROFILE_SW_CONF_ABS120        0x02

#define VP886_DEV_PROFILE_OC_CNT0_OFFSET        (8u)
#define VP886_DEV_PROFILE_OC_CNT1_OFFSET        (9u)
#define VP886_DEV_PROFILE_BLANKING_OFFSET       (10u)

static const uint8 VP886_INTERNAL_DEFAULT_AC_PROFILE[] = {
        /* AC FXS RF100 600R Normal Coefficients */
        0xAD, 0x00, 0xAC, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0xBA, 0xEB, 0x2A,
        0x2C, 0xB5, 0x25, 0xAA, 0x24, 0x2C, 0x3D, 0x9A, 0xAA, 0xBA, 0x27, 0x9F,
        0x01, 0x8A, 0x2D, 0x01, 0x2B, 0xB0, 0x5A, 0x33, 0x24, 0x5C, 0x35, 0xA4,
        0x5A, 0x3D, 0x33, 0xB6, 0x88, 0x3A, 0x10, 0x3D, 0x3D, 0xB2, 0xA7, 0x6B,
        0xA5, 0x2A, 0xCE, 0x2A, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
        0x00, 0x86, 0x2A, 0x42, 0x22, 0x4B, 0x1C, 0xA3, 0xA8, 0xFF, 0x8F, 0xAA,
        0xF5, 0x9F, 0xBA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

static const uint8 VP887_INTERNAL_DEFAULT_AC_PROFILE[] = {
        /* AC FXS RF100 600R Normal Coefficients */
        0xAD, 0x00, 0xAC, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0xBA, 0xEB, 0x2A,
        0x2C, 0xB5, 0x25, 0xAA, 0x24, 0x2C, 0x3D, 0x9A, 0xAA, 0xBA, 0x27, 0x9F,
        0x01, 0x8A, 0x2D, 0x01, 0x2B, 0xB0, 0x5A, 0x33, 0x24, 0x5C, 0x35, 0xA4,
        0x5A, 0x3D, 0x33, 0xB6, 0x88, 0x3A, 0x10, 0x3D, 0x3D, 0xB2, 0xA7, 0x6B,
        0xA5, 0x2A, 0xCE, 0x2A, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
        0x00, 0x86, 0x2A, 0x42, 0x22, 0x4B, 0x1C, 0xA3, 0xA8, 0xFF, 0x8F, 0xAA,
        0xF5, 0x9F, 0xBA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

static const uint8 VP886_INTERNAL_DEFAULT_DC_PROFILE[] = {
/* DC FXS Parameters - 20mA Current Feed */
/* Profile Header ---------------------------------------------------------- */
        0x0C, 0x01, 0x00,       /* DC Profile for ZL886xx device */
        0x0B,                   /* Profile length = 11 + 4 = 15 bytes */
        0x01,                   /* Profile format version = 1 */
        0x03,                   /* MPI section length = 3 bytes */
/* Raw MPI Section --------------------------------------------------------- */
        0xC6, 0x8D, 0xE2,       /* DC Feed Parameters: ir_overhead = 100 ohm; */
        /* VOC = 45 V; LI = 50 ohm; ILA = 20 mA */
        /* Battery Switch Offset Voltage = 4.02 V */
/* Formatted Parameters Section -------------------------------------------- */
        0x1C,                   /* Switch Hook Threshold = 11 mA */
        /* Ground-Key Threshold = 18 mA */
        0x86,                   /* Switch Hook Debounce = 12 ms */
        /* Ground-Key Debounce = 16 ms */
        0x58,                   /* Low-Power Switch Hook Hysteresis = 2 V */
        /* Ground-Key Hysteresis = 6 mA */
        /* Switch Hook Hysteresis = 2 mA */
        0x80,                   /* Low-Power Switch Hook Threshold = 22 V */
        0x98,                   /* Reserved */
        0x00                    /* RPTC = 0 ohms */
};

static const uint8 VP887_INTERNAL_DEFAULT_DC_PROFILE[] = {
/* DC FXS Parameters - 20mA Current Feed */
/* Profile Header ---------------------------------------------------------- */
        0x0D, 0x01, 0x00,       /* DC Profile for ZL887xx device */
        0x0C,                   /* Profile length = 12 + 4 = 16 bytes */
        0x01,                   /* Profile format version = 1 */
        0x03,                   /* MPI section length = 3 bytes */
/* Raw MPI Section --------------------------------------------------------- */
        0xC6, 0xCE, 0x02,       /* DC Feed Parameters: ir_overhead = 200 ohm; */
        /* VOC = 45 V; LI = 100 ohm; VAS = 8.78 V; ILA = 20 mA */
        /* Maximum Voice Amplitude = 2.93 V */
/* Formatted Parameters Section -------------------------------------------- */
        0x1A,                   /* Switch Hook Threshold = 9 mA */
        /* Ground-Key Threshold = 18 mA */
        0x86,                   /* Switch Hook Debounce = 12 ms */
        /* Ground-Key Debounce = 16 ms */
        0x58,                   /* Low-Power Switch Hook Hysteresis = 2 V */
        /* Ground-Key Hysteresis = 6 mA */
        /* Switch Hook Hysteresis = 2 mA */
        0x40,                   /* Low-Power Switch Hook Threshold = 18 V */
        0x04,                   /* Floor Voltage = -25 V */
        0x98,                   /* Reserved */
        0x32                    /* RPTC = 50 ohms */
};

static const uint8 VP886_INTERNAL_DEFAULT_RING_PROFILE[] = {
/* 45vrms 25Hz Fixed Ringing, AC trip */
/* Profile Header ---------------------------------------------------------- */
        0x0C, 0x04, 0x00,       /* Ringing Profile for ZL88602 device */
        0x12,                   /* Profile length = 18 + 4 = 22 bytes */
        0x01,                   /* Profile format version = 1 */
        0x0C,                   /* MPI section length = 12 bytes */
/* Raw MPI Section --------------------------------------------------------- */
        0xC0, 0x08, 0x00, 0x00, /* Ringing Generator Parameters: */
        0x00, 0x44, 0x34, /* 24.9 Hz Sine; 1.41 CF; 63.00 Vpk; 0.00 V bias */
        0x3A, 0x00, 0x00, /* Ring trip cycles = 1; RTHALFCYC = 0 */
        0x00, 0x00,
/* Formatted Parameters Section -------------------------------------------- */
        0xA8,                   /* Ring Trip Method = AC; Threshold = 20.0 mA */
        0x00,                   /* Ringing Current Limit = 50 mA */
        0x4C,                   /* Fixed; Max Ringing Supply = 65 V */
        0x00                    /* Balanced; Ring Cadence Control Disabled */
};

static const uint8 VP887_INTERNAL_DEFAULT_RING_PROFILE[] = {
/* Ringing 25Hz 45Vrms Fixed, AC Trip */
/* Profile Header ---------------------------------------------------------- */
        0x0D, 0x04, 0x00,       /* Ringing Profile for ZL887xx device */
        0x12,                   /* Profile length = 18 + 4 = 22 bytes */
        0x01,                   /* Profile format version = 1 */
        0x0C,                   /* MPI section length = 12 bytes */
/* Raw MPI Section --------------------------------------------------------- */
        0xC0, 0x08, 0x00, 0x00, /* Ringing Generator Parameters: */
        0x00, 0x44, 0x34, /* 24.9 Hz Sine; 1.41 CF; 63.00 Vpk; 0.00 V bias */
        0x3A, 0x00, 0x00, /* Ring trip cycles = 1; RTHALFCYC = 0 */
        0x00, 0x00,
/* Formatted Parameters Section -------------------------------------------- */
        0xAA,                   /* Ring Trip Method = AC; Threshold = 21.0 mA */
        0x00,                   /* Ringing Current Limit = 50 mA */
        0x4C,                   /* Fixed; Max Ringing Supply = 65 V */
        0x00                    /* Balanced; Ring Cadence Control Disabled */
};

#endif //TELLUINO_CONTROLLER_VP88X_PROFILES_H
