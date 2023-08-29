#ifndef	SLIC_SPI_H
#define SLIC_SPI_H

#include <SPI.h>

class SlicSpi
{
private:
  
    SPISettings *my_setting=NULL;
    
public:
    SlicSpi(/* args */);
    ~SlicSpi();
    void setup();
    uint16_t readByte();
    void writeByte(uint16_t value);
};




#endif