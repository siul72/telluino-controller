//
// Created by siul72 on 10.02.2024.
//

#include "mslic_api.h"

#define EC_WRT_CMD 0x00

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
