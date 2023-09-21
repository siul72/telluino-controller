#include <Arduino.h>
#include <TaskScheduler.h>
#include "usb_device.h"
#include "slic_device.h" 

#define TEL_VERSION_MAJOR 0
#define TEL_VERSION_MINOR 1
#define TEL_HW_VERSION_MAJOR 1
#define TEL_HW_VERSION_MINOR 0 

#define LED_OP1 8
#define LED_OP2 9

#define USB_SER_RX 4
#define USB_SER_TX 5

//buffers for logging
char buffer[128], szF[6];

int led_status = LOW;

void blinkLeds();
void readUsb();
void slicRunner();

//create Instances of the Classes

Scheduler runner;
UsbDevice my_usb_device(USB_SER_RX, USB_SER_TX);
SlicDevice my_slic_device;

Task blinkLedTask(1000, TASK_FOREVER, &blinkLeds);
Task readUsbTask(3000, TASK_FOREVER, &readUsb);
Task slicRunnerTask(500, TASK_FOREVER, &slicRunner);

//*************************** TASK FUNCTIONS *********************************
void blinkLeds() {
  //Serial.println(">> blinkLeds");
  led_status = 1 - led_status;
  digitalWrite(LED_BUILTIN, led_status);   
  digitalWrite(LED_OP1, led_status);
  digitalWrite(LED_OP2, led_status);

}

void readUsb() {
  Serial.println(">> readUsb");
  readUsbTask.disable();
  my_usb_device.read();
  readUsbTask.enableDelayed(3000);

}

void slicRunner(){
  Serial.println(">> slicRunner");
  slicRunnerTask.disable();
  my_slic_device.run();
  slicRunnerTask.enableDelayed(5000);
}

 // the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_OP1, OUTPUT);
  pinMode(LED_OP2, OUTPUT);

  Serial.begin(115200);
  Serial.println("setup...");
  my_usb_device.setup();
  my_slic_device.setup();
  Serial.println("... initialized usb ...");
  runner.init();
  Serial.println("... initialized scheduler ...");
  runner.addTask(blinkLedTask);
  runner.addTask(readUsbTask);
  runner.addTask(slicRunnerTask);
  blinkLedTask.enable();
  readUsbTask.enable();
  slicRunnerTask.enable();
  sprintf(buffer, "... starting Telluino V%d.%d for hardware V%d.%d", TEL_VERSION_MAJOR, TEL_VERSION_MINOR,
  TEL_HW_VERSION_MAJOR, TEL_HW_VERSION_MINOR);
  Serial.println(buffer);

}

// the loop function runs over and over again forever
void loop() {
  runner.execute();

}