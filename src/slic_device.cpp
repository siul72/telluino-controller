#include "slic_device.h"

SlicDevice::SlicDevice()
{
    my_spi_bus = new SlicSpi();
    my_pcm = new SlicPcm();
    setup();
}

SlicDevice::~SlicDevice()
{
    delete my_spi_bus;
    delete my_pcm;
}

void SlicDevice::init_slic_device(){
    VpStatusType status;
    status = VpMakeDeviceObject(VP_DEV_887_SERIES, deviceId, &devCtx, &devObj);
    if (status != VP_STATUS_SUCCESS) {
        printf("Error making the device object: %s\n", MapStatus(status));

    }
}

void SlicDevice::setup()
{
    init_slic_device();
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
