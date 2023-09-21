#include "slic_device.h"

SlicDevice::SlicDevice()
{
    my_spi_bus = new SlicSpi();
    my_pcm = new SlicPcm();
}

SlicDevice::~SlicDevice()
{
    delete my_spi_bus;
    delete my_pcm;
}

void SlicDevice::setup()
{
    my_spi_bus->setup();
    my_pcm->setup();
}

void SlicDevice::run()
{
    my_spi_bus->writeByte(test[count_array]);
    count_array++;
    if (count_array >= MAX_ARRAY){
        count_array = 0;
    }
}
