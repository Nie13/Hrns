#include <Arduino.h>
#include <ringbuffer.h>

#include <gyro.h>
#include <l3gd20.h>

void setup() {
  // put your setup code here, to run once:
  gyroSpiInit();
  gyroInit();
  gyroStart();
  Serial2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  gyroUpdate();
  Serial2.print("winkelx : ");
  Serial2.print(winkelx);
  Serial2.print("   winkely : ");
  Serial2.print(winkely);
  Serial2.print("   winkelz : ");
  Serial2.println(winkelz);
  delay(200);
}
