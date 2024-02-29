//
// Created by siul72 on 20.02.2024.
//

#include "slic_spi.h"


SlicSpi::SlicSpi()
{
    const int speedMaximum=20000, dataOrder=MSBFIRST, dataMode=SPI_MODE0;

    my_setting = new SPISettings(speedMaximum, dataOrder, dataMode);
    /*
    .----------------------------.---------------------------------------.
    | Master/Slave (OLD)         | Controller/Peripheral (NEW)           |
    :----------------------------+---------------------------------------:
    | Master In Slave Out (MISO) | Controller In, Peripheral Out (CIPO)  |
    | Master Out Slave In (MOSI) | Controller Out Peripheral In (COPI)   |
    | Slave Select pin (SS)      | Chip Select Pin (CS)                  |
    '----------------------------'---------------------------------------'
    */

    /*
      //Mode	     | Clock Polarity (CPOL) |	Clock Phase (CPHA) |	Output Edge |	Data Capture
     //SPI_MODE0	 | 0	                 |   0	               |    Falling	    |   Rising
     //SPI_MODE1	 | 0	                 |   1	               |    Rising	    |   Falling
     //SPI_MODE2	 | 1	                 |   0	               |    Rising	    |   Falling
     //SPI_MODE3	 | 1	                 |   1	               |    Falling	    |   Rising*/


    //SPCR |= _BV(SPE); // turn on SPI in slave mode
    //SPI.attachInterrupt(); // turn on interrupt
}


SlicSpi::~SlicSpi()
{
    delete my_setting;

}

void SlicSpi::run(){

}

void SlicSpi::setup()
{
    pinMode(PIN_SPI_SS, OUTPUT);     // Set SPI slave select pin as output
    digitalWrite(PIN_SPI_SS, HIGH);  // Make sure spi_ss is held high
    SPI.begin();                 // begin SPI
}

uint8_t SlicSpi::readByte()
{
    uint8_t byte_0;    // First and second bytes read

    // put your main code here, to run repeatedly:
    SPI.beginTransaction(*my_setting);
    // set speed bit format and clock/data polarity while starting SPI transaction
    digitalWrite(PIN_SPI_SS, LOW);
    // write the LTC CS pin low to initiate ADC sample and data transmit
    byte_0 = SPI.transfer(0); // read first 8 bits

    digitalWrite(PIN_SPI_SS, HIGH);
    // write LTC CS pin high to stop LTC from transmitting zeros.
    SPI.endTransaction();
    // close SPI transaction

    return byte_0;
}

void SlicSpi::writeByte(uint8_t value)
{
    //SPI.setBitOrder(MSBFIRST);
    digitalWrite(PIN_SPI_SS, LOW); // A0
    SPI.transfer(value);
    digitalWrite(PIN_SPI_SS, HIGH);

}

void SlicSpi::spi_byte_write(uint8_t deviceId, byte reg){
    this->writeByte(reg);
}

uint16_t SlicSpi::spi_byte_read(uint8_t deviceId) {
    return readByte();
}
