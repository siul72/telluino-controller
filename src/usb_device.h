#ifndef	USB_DEVICE_H
#define USB_DEVICE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class UsbDevice {

    public:
    UsbDevice(int rx_port, int tx_port);
    void setup();

    void read();

    void write(char c);

private:
    SoftwareSerial *my_serial;
};

#endif