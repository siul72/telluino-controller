#ifndef	SLIC_DEVICE_H
#define SLIC_DEVICE_H
 
#include <Arduino.h>
#include "slic_spi.h"
#include "slic_pcm.h"

enum SlicRegister{

  NONE = 0x00,
  SLIC_REG_TEST1 = 0xAA,
  SLIC_REG_TEST2 = 0xF0,
  SLIC_REG_TEST3 = 0x0F

};



class SlicDevice{

    public:
      SlicDevice();
      ~SlicDevice();
      void setup();
      void run();

    private:
      SlicSpi *my_spi_bus;
      SlicPcm *my_pcm;
      int count_array = 0;
      const int MAX_ARRAY = 3;
      const char16_t test[3] = {SLIC_REG_TEST1, SLIC_REG_TEST2, SLIC_REG_TEST3};


};
 

#endif