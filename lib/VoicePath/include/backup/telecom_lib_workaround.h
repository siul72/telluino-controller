//
// Created by lukit on 1/28/24.
//

#ifndef MICRO_SLIC_API_TELECOM_LIB_WORKAROUND_H
#define MICRO_SLIC_API_TELECOM_LIB_WORKAROUND_H

#include "vp_api_cfg.h"
#include "vp_api_common.h"
#include "vp_CSLAC_types.h"


#define RESET_CFG_REG 0x00
#define FEATURE_CTRL_REG 0x00

#define TELECOM_MOD_FILE_DES 0x00
#define TELECOM_MOD_IOCTL_TSA_RESET 0x00
#define TELECOM_FPGA_PIN_ORIDE_MASK 0x00
#define TELECOM_FPGA_PIN_ORIDE_EN 0x00

#define TELECOM_DIN 0x00

#define INTERUPT_REG 0x00
#define  INT_DIN0 0x00

typedef struct {
    uint32 pcmClk;
    uint16 connector;
} TelecomPcmClkType;


#endif //MICRO_SLIC_API_TELECOM_LIB_WORKAROUND_H
