#include "slic_microship.h"



SlicMicrochip::SlicMicrochip()
{
    my_spi_bus = new SlicSpi();
  
}

SlicMicrochip::~SlicMicrochip()
{
    
    delete my_spi_bus;
    
}

void SlicMicrochip::initialize()
{
    my_spi_bus->setup();
    //send initialization stream

    //read back the data
 
}

void SlicMicrochip::run()
{
    my_spi_bus->run();
}