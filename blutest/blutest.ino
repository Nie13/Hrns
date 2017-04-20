#include <Arduino.h>
//#include <mbed.h>
//#include <i2c.h>
//#include <icrmacros.h>
//#include <SoftwareSerial.h>
#include <ringbuffer.h>
//int counter = 0;
//int incomingByte;

void setup() {
  Serial2.begin(115200);
}

void loop() {
  
  Serial2.println("RESET");
  
  //Serial2.println(counter);
  
  
  delay(250);
}

