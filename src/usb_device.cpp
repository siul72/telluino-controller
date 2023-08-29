#include "usb_device.h"

UsbDevice::UsbDevice(int rx_port, int tx_port)
{
    pinMode(rx_port, INPUT);
    pinMode(tx_port, OUTPUT);
    my_serial = new SoftwareSerial(rx_port, tx_port);
}

void UsbDevice::setup()
{
    my_serial->begin(115200);
    my_serial->println("Hello, world?");
}

void UsbDevice::read(){
    my_serial->listen();
    Serial.println("Data from port one:");
    // while there is data coming in, read it
    // and send to the hardware serial port:
    while (my_serial->available() > 0) {
        char inByte = my_serial->read();
        Serial.write(inByte);
    }
    my_serial->println("ACK");
}

void UsbDevice::write(char c){

    my_serial->write(c);
}
