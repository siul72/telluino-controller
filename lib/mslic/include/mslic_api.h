//
// Created by siul72 on 10.02.2024.
//

#ifndef TELLUINO_CONTROLLER_MSLIC_API_H
#define TELLUINO_CONTROLLER_MSLIC_API_H

#include "slic_spi.h"
#include "mslic_enums.h"

/**< VP_REDUCED_API_IF
 * Define this to remove the I/F for unsupported functions. This reduces
 * code size with the only limitation that the application cannot call
 * functions that would return "Unsupported" for all devices used in the
 * application. The specific functions removed will depend on the device and
 * test packages compiled in.
 */
#define VP_REDUCED_API_IF

#define VP_CC_886_SERIES

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

typedef uint32_t VpDeviceIdType;

class MicrochipSlicApi {

public:
    MicrochipSlicApi(/* args */);
    ~MicrochipSlicApi();
    void VpMpiCmd(uint8_t deviceId,
                  uint8_t ecVal, uint8_t cmd, uint8_t cmdLen, uint8_t * dataPtr);
    VpStatusType VpMakeDeviceObject(VpDeviceType deviceType,
                                    VpDeviceIdType deviceId, void *pDevCtx, void *pDevObj);
    VpStatusType VpMakeLineObject( uint8_t termType, uint8_t channelId,
            void *pLineCtx, void *pLineObj, void *pDevCtx);
    VpStatusType VpInitDevice(  void *pDevCtx,  void *pDevProfile,  void *pAcProfile,
            void *pDcProfile,  void *pRingProfile,   void *pFxoAcProfile,  void *pFxoCfgProfile);

    VpStatusType Vp886InitDevicePcnRcn(void *pDevCtx);

private:
    SlicSpi * spi;
    boolean VpSlacRegWrite( void *pDevCtx, void *pLineCtx,
                            uint8_t cmd, uint8_t writeLen, const uint8_t *pWriteBuf);
    boolean VpSlacRegRead( void *pDevCtx,  void *pLineCtx,
                           uint8_t cmd,  uint8_t readLen, uint8_t *pReadBuf);


};


#endif //TELLUINO_CONTROLLER_MSLIC_API_H
