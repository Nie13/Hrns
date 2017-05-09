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

int winkellast = 0;

float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
float xavg;
float yavg;
float zavg;
int flag=0;
int sTeps=0;
float threshhold = 6400.0;
int tim = 0;


void setup() {
  Serial2.begin(115200);
  i2c_master_enable(I2C1, 0); 

  lsm303Init();
  calibrate();
}

void loop() {
  int acc=0;
  float totvect[100]={0};
  float totave[100]={0};
  float xaccl[100]={0};
  float yaccl[100]={0};
  float zaccl[100]={0};
  for (int i=0;i<100;i++){
    readAccValues();
    xaccl[i]=float(accx);    
    yaccl[i]=float(accy);    
    zaccl[i]=float(accz);
    delay(1);
    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
    Serial2.println(totave[i]);
    threshhold = fixThreshold(i, totave);
    Serial2.println(threshhold);
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
    xval[i]=float( accx );
    yval[i]=float( accy );
    zval[i]=float( accz );
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

float fixThreshold(int i, float to[100]){
  float newthld;
  float sumto;
  for(int j = 0; j < 20; j++){
    sumto += to[i-j]; 
  }
  newthld = sumto/20.0;
  if(newthld >= 6400.0){
    return newthld;  
  }else{
    return 6400.0;
  }
}

