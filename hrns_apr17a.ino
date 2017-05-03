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
int steps[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int winkellast = 0;

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
  int readX = accx / 64;
  int readY = accy / 64;
  int readZ = accz / 64;
  winkel = sqrt( readX * readX + readY * readY + readZ * readZ - pow(270, 2));
  //Serial2.print("accx: ");
  //Serial2.print(readX);
  //Serial2.print(" ; ACCY: ");
  //Serial2.print(readY);
  //Serial2.print(" ; ACCZ: ");
  //Serial2.print(readZ);
  Serial2.print(" ; Winkel: ");
  Serial2.println(winkel);
  //for (int i =0; i < 19; i ++){
  //  steps[i] = steps[i + 1];
  //}
  //steps[19] = winkel;
  //if(!arrayIncluded(steps, winkellast)){
  //  winkellast = 0;
  //}
  //if(winkel > 125){
  //  if(winkel > winkellast){
  //    winkellast = winkel;
  //  }elsif( (float)((float)winkellast / (float) winkel) > 0.8){
  //    Serial2.print("STEP");
  //  }
  //}
  
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

boolean arrayIncluded(int ary[20], int a){
  for (int i = 0; i < sizeof(ary); i++) {
    if (ary[i] == a) {
      return true;
    }
  }
  return false;
}

