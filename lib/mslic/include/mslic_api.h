//
// Created by siul72 on 10.02.2024.
//

#ifndef TELLUINO_CONTROLLER_MSLIC_API_H
#define TELLUINO_CONTROLLER_MSLIC_API_H

#include <Arduino.h>
#include "vp_api_common.h"
#include "slic_spi.h"


/* ZTAP specific values for device ID depending on the MPI Mode */
#define MPI_3WIRE       0x0000 /* 3 wire SPI w/ 2.5us interbyte chipselect off-time */
#define MPI_4WIRE       0x0100 /* 4 wire SPI w/ 2.5us interbyte chipselect off-time */
#define SPI8_3WIRE      0x0200 /* 3 wire SPI w/ one clock chipselect off-time */
#define SPI8_4WIRE      0x0400 /* 4 wire SPI w/ one clock chipselect off-time */
#define SPI8_3WIRE_CFM  0x0300 /* 3 wire SPI w/o chipselect off-time per VpMpiCmd() transation */
#define SPI8_4WIRE_CFM  0x0500 /* 4 wire SPI w/o chipselect off-time per VpMpiCmd() transation */
#define ZSI_4WIRE       0x0E00 /* 4 wire ZSI transation*/

#define SM              0x0010
#define DIN             0x0000

/* Number of lines supported by the quick start */
#define NUM_LINES       1


//typedef uint32_t VpDeviceIdType;

class MicrochipSlicApi {

public:
    MicrochipSlicApi(/* args */);
    ~MicrochipSlicApi();
    void VpMpiCmd(VpDeviceIdType deviceId,
                  uint8_t ecVal, uint8_t cmd, uint8_t cmdLen, uint8_t * dataPtr);
private:
    SlicSpi * spi;

};


#endif //TELLUINO_CONTROLLER_MSLIC_API_H
