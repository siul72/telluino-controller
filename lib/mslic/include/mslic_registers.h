//
// Created by siul72 on 20.02.2024.
//

#ifndef TELLUINO_CONTROLLER_SLIC_REGISTERS_H
#define TELLUINO_CONTROLLER_SLIC_REGISTERS_H

#define VP886_R_STATE_SS_SHUTDOWN               0x0F

/* 56/57 : System State Register */
#define VP886_R_STATE_WRT                       0x56
#define VP886_R_STATE_RD                        0x57
#define VP886_R_STATE_LEN                       1

/* 06 : No Operation */
#define VP886_R_NOOP_WRT                        0x06
#define VP886_R_NOOP_LEN                        0
/* No-op abbreviation */
#define VP886_NOOP                              VP886_R_NOOP_WRT

/* 04 : Hardware Reset Command */
#define VP886_R_HWRESET_WRT                     0x04
#define VP886_R_HWRESET_LEN                     0

/* 73 : Revision and Product Code Number */
#define VP886_R_RCNPCN_RD                       0x73
#define VP886_R_RCNPCN_LEN                      2
#define VP886_R_RCNPCN_RCN_IDX                  0
#define VP886_R_RCNPCN_PCN_IDX                  1
/* Byte 0 */
#define VP886_R_RCNPCN_RCN_AAA                  0x01
#define VP886_R_RCNPCN_RCN_BAA                  0x02
/* Byte 1 */
#define VP886_R_RCNPCN_PCN_ZL88601              0x73    /* Low voltage ABS */
#define VP886_R_RCNPCN_PCN_ZL88602              0x77    /* High voltage ABS (Also Le9662) */
#define VP886_R_RCNPCN_PCN_ZL88701              0x7B    /* Low voltage 2ch Tracker */
#define VP886_R_RCNPCN_PCN_ZL88702              0x7F    /* High voltage 2ch Tracker (Also Le9672) */
#define VP886_R_RCNPCN_PCN_LE9661               0x4B    /* Low voltage 1ch Tracker */
#define VP886_R_RCNPCN_PCN_LE9671               0x4F    /* High voltage 1ch Tracker */
#define VP886_R_RCNPCN_PCN_LE9642               0x75    /* SF2 High Voltage ABS */
#define VP886_R_RCNPCN_PCN_LE9641               0x49    /* SF1 Low voltage 1ch Tracker */
#define VP886_R_RCNPCN_PCN_LE9652               0x7D    /* SF2 High voltage 2ch Tracker */
#define VP886_R_RCNPCN_PCN_LE9651               0x4D    /* SF1 High voltage 1ch Tracker */
#define VP886_R_RCNPCN_PCN_NOT_SF               0x02
#define VP886_R_RCNPCN_PCN_LV_HV                0x04
#define VP886_R_RCNPCN_PCN_ABS_TRACKER          0x08
#define VP886_R_RCNPCN_PCN_2CH                  0x30

/* 3A/3B : Fuse Register 8 */
#define VP886_R_FUSE_REG_8_WRT                  0x3A
#define VP886_R_FUSE_REG_8_RD                   0x3B
#define VP886_R_FUSE_REG_8_LEN                  4
/* Byte 0 */
#define VP886_R_FUSE_REG_8_SHSUP                0x80

/* EC abbreviations */
#define VP886_EC_1                              0x01
#define VP886_EC_2                              0x02
#define VP886_EC_BOTH                           0x03
#define VP886_EC_GLOBAL                         0x03


#endif //TELLUINO_CONTROLLER_SLIC_REGISTERS_H
