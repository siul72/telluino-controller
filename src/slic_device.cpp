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

char *
SlicDevice::MapStatus(
        VpStatusType status)
{
    int idx = 0;

    static char buff[50];

    static const char *strTable[VP_STATUS_NUM_TYPES] = {
            "VP_STATUS_SUCCESS",
            "VP_STATUS_FAILURE",
            "VP_STATUS_FUNC_NOT_SUPPORTED",
            "VP_STATUS_INVALID_ARG",
            "VP_STATUS_MAILBOX_BUSY",
            "VP_STATUS_ERR_VTD_CODE",
            "VP_STATUS_OPTION_NOT_SUPPORTED",
            "VP_STATUS_ERR_VERIFY",
            "VP_STATUS_DEVICE_BUSY",
            "VP_STATUS_MAILBOX_EMPTY",
            "VP_STATUS_ERR_MAILBOX_DATA",
            "VP_STATUS_ERR_HBI",
            "VP_STATUS_ERR_IMAGE",
            "VP_STATUS_IN_CRTCL_SECTN",
            "VP_STATUS_DEV_NOT_INITIALIZED",
            "VP_STATUS_ERR_PROFILE",
            "VP_STATUS_INVALID_VOICE_STREAM",
            "VP_STATUS_CUSTOM_TERM_NOT_CFG",
            "VP_STATUS_DEDICATED_PINS",
            "VP_STATUS_INVALID_LINE",
            "VP_STATUS_LINE_NOT_CONFIG",
            "VP_STATUS_ERR_SPI"
    };

    if (status < VP_STATUS_NUM_TYPES) {
        idx = sprintf(&buff[idx], "(%s)", (char *)strTable[status]);
    }
    idx = sprintf(&buff[idx], " = %d", (uint16_t)status);

    return buff;
}

VpStatusType SlicDevice::init_slic_device(){
    VpStatusType status;
    status = slic_api->VpMakeDeviceObject(VP_DEV_887_SERIES, deviceId, nullptr, nullptr);
    if (status != VP_STATUS_SUCCESS) {
        printf("Error making the device object: %s\n", MapStatus(status));
        //return status;
    }
    int lineNum = 0;

    status = slic_api->VpMakeLineObject(VP_TERM_FXS_LOW_PWR, lineNum, nullptr, nullptr, nullptr);
    if (status != VP_STATUS_SUCCESS) {
        printf("Error making the device object: %s\n", MapStatus(status));
        //return status;
    }

    status = slic_api->VpInitDevice(nullptr, nullptr, nullptr,
                                    nullptr, nullptr, nullptr,
                                    nullptr);

    printf("<< init_slic_device\n");
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
