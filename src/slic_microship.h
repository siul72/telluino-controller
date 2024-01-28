#ifndef	SLIC_MICROCHIP_H
#define SLIC_MICROCHIP_H

#include "slic_chip.h"
#include "slic_spi.h"

class SlicMicrochip : public SlicChip {

    public:
        SlicMicrochip();
        virtual ~SlicMicrochip();
      void initialize();
      void run();

    private:
    SlicSpi *my_spi_bus;

};

#endif