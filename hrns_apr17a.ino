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
//int steps[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int winkellast = 0;

float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
float xavg;
float yavg;
float zavg;
int flag=0;
int sTeps=0;
float threshhold = 155.0;
int tim = 0;


void setup() {
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  Serial2.begin(115200);
  i2c_master_enable(I2C1, 0);
  

  lsm303Init();
  // put your setup code here, to run once:

  calibrate();

}

void loop() {
  // put your main code here, to run repeatedly:
  int winkel = 0;
  //readAccValues();
  //int readX = accx / 64;
  //int readY = accy / 64;
  //int readZ = accz / 64;
  //winkel = sqrt( readX * readX + readY * readY + readZ * readZ - pow(270, 2));
  int acc=0;
  float totvect[100]={0};
  float totave[100]={0};
  float xaccl[100]={0};
  float yaccl[100]={0};
  float zaccl[100]={0};
  for (int i=0;i<100;i++){
    readAccValues();
    xaccl[i]=float(accx/64.0);    
    yaccl[i]=float(accy/64.0);    
    zaccl[i]=float(accz/64.0);
    delay(1);
    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
    Serial2.println(totave[i]);
    delay(100);
    if (totave[i]>threshhold && flag==0 ){
      sTeps=sTeps+1;
      flag=1; 
      tim=0;
    }else if (totave[i] > threshhold && flag==1){
      tim++;
    }
    if (totave[i] <threshhold  && flag==1 && tim >= 2){
      flag=0;
    }else if(totave[i] < threshhold && flag==1 && tim <2){
      tim ++;
    }
    Serial2.println('\n');
    Serial2.print("steps=");
    Serial2.println(sTeps);
  }
   delay(100);
    
  
  //Serial2.print("accx: ");
  //Serial2.print(readX);
  //Serial2.print(" ; ACCY: ");
  //Serial2.print(readY);
  //Serial2.print(" ; ACCZ: ");
  //Serial2.print(readZ);
  //Serial2.print(" ; Winkel: ");
  //Serial2.println(winkel);
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
  //delay(50); 
}

boolean arrayIncluded(int ary[20], int a){
  for (int i = 0; i < sizeof(ary); i++) {
    if (ary[i] == a) {
      return true;
    }
  }
  return false;
}

void calibrate(){
  float sum=0;
  float sum1=0;
  float sum2=0;
  for (int i =0; i<100; i++){
    readAccValues();
    xval[i]=float( accx / 64.0);
    yval[i]=float( accy / 64.0);
    zval[i]=float( accz / 64.0);
    sum=xval[i] + sum;    
    sum1+=yval[i];
    sum2+=zval[i];
  }
  delay(100);
  xavg=sum/100.0;
  yavg=sum1/100.0;
  zavg=sum2/100.0;
  Serial2.println(xavg);
  Serial2.println(yavg);
  Serial2.println(zavg);
  
}

