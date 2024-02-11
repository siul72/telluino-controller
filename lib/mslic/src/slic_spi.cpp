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
    
}


SlicSpi::~SlicSpi()
{
    delete my_setting;
    
}

void SlicSpi::setup()
{
     pinMode(PIN_SPI_SS, OUTPUT);     // Set SPI slave select pin as output
     digitalWrite(PIN_SPI_SS, HIGH);  // Make sure spi_ss is held high 
     SPI.begin();                 // begin SPI
}



uint16_t SlicSpi::readByte()
{
    uint8_t byte_0, byte_1;    // First and second bytes read
    uint16_t spi_bytes;       // final 12 bit shited value
    // put your main code here, to run repeatedly:
    SPI.beginTransaction(*my_setting); 
    // set speed bit format and clock/data polarity while starting SPI transaction 
    digitalWrite(PIN_SPI_SS, LOW);
    // write the LTC CS pin low to initiate ADC sample and data transmit 
    byte_0 = SPI.transfer(0); // read firt 8 bits
    byte_1 = SPI.transfer(0); // read second 8 bits
    //
    digitalWrite(PIN_SPI_SS, HIGH);
    // wite LTC CS pin high to stop LTC from transmitting zeros. 
    SPI.endTransaction();
    // close SPI transaction
    spi_bytes = ( ( (byte_0 & B00011111) <<7) + (byte_1 >>1) );
    return spi_bytes;
}

void SlicSpi::spi_byte_write(uint32_t deviceId, uint16_t bValue){
    this->writeByte(bValue);

}

void SlicSpi::writeByte(uint16_t value)
{
    //SPI.setBitOrder(MSBFIRST);
    uint8_t byte_0, byte_1;    // First and second bytes write
    digitalWrite(PIN_SPI_SS, LOW); // A0
    byte_0 = (value >> 8);
    byte_1 = (value & 0xFF); 
    //byte_2 = (input_0 >> 8);
    //byte_3 = (input_0 & 0xFF);

    SPI.transfer(byte_0);
    SPI.transfer(byte_1);
   
  digitalWrite(PIN_SPI_SS, HIGH);
 
}
