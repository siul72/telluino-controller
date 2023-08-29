#ifndef	SLIC_DEVICE_H
#define SLIC_DEVICE_H
 
#include <Arduino.h>
#include "slic_spi.h"
#include "slic_pcm.h"


class SlicDevice{

    public:
      SlicDevice();
      void setup();

    private:
      SlicSpi *my_spi_bus;

};
 

#endif