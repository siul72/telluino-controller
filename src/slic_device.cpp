#include "slic_device.h"

SlicDevice::SlicDevice()
{

    my_pcm = new SlicPcm();
    slic_api = new MicrochipSlicApi();
    setup();
}

SlicDevice::~SlicDevice()
{

    delete my_pcm;
    delete slic_api;
}

VpStatusType SlicDevice::init_slic_device(){
    VpStatusType status;
    status = slic_api->VpMakeDeviceObject(VP_DEV_887_SERIES, deviceId, nullptr, nullptr);
    if (status != VP_STATUS_SUCCESS) {
        //printf("Error making the device object: %s\n", MapStatus(status));
        return status;
    }
    int lineNum = 0;

    status = slic_api->VpMakeLineObject(VP_TERM_FXS_LOW_PWR, lineNum, nullptr, nullptr, nullptr);
    if (status != VP_STATUS_SUCCESS) {
        //printf("Error making the device object: %s\n", MapStatus(status));
        return status;
    }

    status = slic_api->VpInitDevice(nullptr, nullptr, nullptr,
                                    nullptr, nullptr, nullptr,
                                    nullptr);

    return status;

}

void SlicDevice::setup()
{
    init_slic_device();

    my_pcm->setup();
}

void SlicDevice::run()
{

}
