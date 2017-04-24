#include <ringbuffer.h>

#include <lsm303.h>
#include <lsm303reg.h>

#include <Arduino.h>
#include <libmaple/i2c.h>
#include <math.h>

int led1 = PE9;
int led2 = PE10;
int led3 = PE11;
int stepcount = 0;
void setup() {
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  Serial2.begin(115200);
  i2c_master_enable(I2C1, 0);

  lsm303Init();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int winkel = 0;
  readAccValues();

  //winkel = sqrt( accx*accx );
  Serial2.print("accx: ");
  Serial2.print(accx);
  Serial2.print(" ; ACCY: ");
  Serial2.print(accy);
  Serial2.print(" ; ACCZ: ");
  Serial2.println(accz);
  //if (winkel >= 10){
    //stepcount += 1;
   // digitalWrite( led1, HIGH);
  //}
  //if (winkel >= 100){
    //stepcount += 1;
   // digitalWrite( led2, HIGH);
  //}
  //if (winkel >= 10000){
    //stepcount += 1;
   // digitalWrite( led3, HIGH);
  //}
/*  if ((stepcount % 2) == 0){
    digitalWrite( led2, HIGH);
  }else{
    digitalWrite( led2, LOW);
  }*/
  //accUpdate();
  delay(50);

  
  
}
