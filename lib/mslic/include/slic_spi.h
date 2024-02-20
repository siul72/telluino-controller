//
// Created by siul72 on 20.02.2024.
//

#ifndef TELLUINO_CONTROLLER_SLIC_SPI_H
#define TELLUINO_CONTROLLER_SLIC_SPI_H

#include <SPI.h>

class SlicSpi
{
private:

    SPISettings *my_setting=NULL;

public:

    SlicSpi();
    ~SlicSpi();
    void setup();
    uint16_t readByte();
    void writeByte(uint16_t value);
    void spi_byte_write(uint32_t deviceId, byte reg);
    uint16_t spi_byte_read(uint8_t deviceId);

};




class slic_spi {

};


#endif //TELLUINO_CONTROLLER_SLIC_SPI_H
