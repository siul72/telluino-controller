#include <Arduino.h>

#define LED_OP1 8
#define LED_OP2 9
 // the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_OP1, OUTPUT);
  pinMode(LED_OP2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_OP1, HIGH);
  digitalWrite(LED_OP2, HIGH);
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(LED_OP1, LOW);
  digitalWrite(LED_OP2, LOW);
  delay(1000);                      // wait for a second
}