#ifndef	SLIC_DEVICE_H
#define SLIC_DEVICE_H
 
#include <Arduino.h>


#include "mslic_api.h"
#include "slic_pcm.h"
#include "vp886_device.h"


struct timespec {
    //time_t tv_sec;
    long tv_nsec;
    long tv_usec;
};

struct itimerspec {
    struct timespec  it_interval;
    struct timespec  it_value;
};

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
        void init_slic_device();

    private:
        VpDevCtxType devCtx;
        VpLineCtxType lineCtx[NUM_LINES];
        Vp886DeviceObjectType devObj;
        Vp886LineObjectType lineObj[NUM_LINES];
        VpDeviceIdType deviceId = (SM | SPI8_3WIRE);
        SlicSpi *my_spi_bus;
        SlicPcm *my_pcm;
        int count_array = 0;
        const int MAX_ARRAY = 3;
        const char16_t test[3] = {SLIC_REG_TEST1, SLIC_REG_TEST2, SLIC_REG_TEST3};

};
 

#endif