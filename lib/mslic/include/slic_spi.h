#ifndef	SLIC_SPI_H
#define SLIC_SPI_H

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
    void spi_byte_write(uint32_t deviceId, uint16_t bValue);
    uint16_t spi_byte_read(uint32_t deviceId);

};




#endif