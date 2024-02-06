#ifndef	SLIC_DEVICE_H
#define SLIC_DEVICE_H
 
#include <Arduino.h>


#include "vp_api.h"
#include "vp886_api.h"
#include "sdk_qs_utils.h"
#include "slic_spi.h"
#include "slic_pcm.h"

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