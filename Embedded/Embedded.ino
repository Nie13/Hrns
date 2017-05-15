#include <Arduino.h>
#include <ringbuffer.h>
#include <lsm303.h>

#include <libmaple/i2c.h>
#include <math.h>

#include "HardwareTimer.h"
#include "functiontimer.h"
//#include "ArduinoPedometer.h"


int led1 = PE9;
int led2 = PE10;
int led3 = PE11;
int led4 = PE12;

double magnitude;
double minMagnitude;
double maxMagnitude;
int stepCount;
int mstepCounter = 0;

int htinput = PC1;
long counter = 0;
int mstepFlag = 0;
float mThreshold = 4000.0;

int beats[15];
//int steps[15];

HardwareTimer timerCounter(4);

//ArduinoPedometer fp; //create an instacne called "fp" for the class "ArduinoPedometer"

void DisplayTimer(int arg) {
  Serial2.print("current: ");
  Serial2.print(magnitude);
  Serial2.print(", min: ");
  Serial2.print(minMagnitude);
  Serial2.print(", max: ");
  Serial2.print(maxMagnitude);
  Serial2.print(", steps: ");
  Serial2.println(stepCount);
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(htinput, INPUT);

  Serial2.begin(115200);

  minMagnitude = 99999999999;
  maxMagnitude = 0;
  stepCount = 0;

  timerCounter.setMode(TIMER_CH4, TIMER_OUTPUT_COMPARE);
  timerCounter.pause();
  timerCounter.setCount(0);
  timerCounter.setPrescaleFactor(720000);
  timerCounter.setOverflow(65535);
  timerCounter.setCompare(TIMER_CH4, 1);
  timerCounter.refresh();
  timerCounter.resume();
  
  i2c_master_enable(I2C1, 0);
  lsm303Init();

  calibrate();
//  calibrate2();
  
  timerInit();

  Serial2.println("starting...");
  
  delay(1000);
  timerStart(0, 20, readPedometer, 0);
  timerStart(1, 20, sendData, 0);
//  timerStart(2, 20, stepLoop, 0);
}

void loop() {  
  int value = analogRead(htinput);
  updateLEDs(value);
  readHeartrate(value);
  //Serial2.println("test");
}

void updateLEDs(int value) {
  if (value > 1000) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }
  
  if (value > 2000) {
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led2, LOW);
  }
  
  if (value > 3000) {
    digitalWrite(led3, HIGH);
  } else {
    digitalWrite(led3, LOW);
  }
  
  if (value > 4000) {
    digitalWrite(led4, HIGH);
  } else {
    digitalWrite(led4, LOW);
  }
}

double calcAverage() {
  int sum = 0;
  int total = 0;
  for (int i = 0; i < 15; i++) {
    if (beats[i] != 0) {
      sum += beats[i];
      total++;
    }
  }

  return ((double)sum/total);
}

double calcBPM(double avg) {
  return 1.0/(avg/1000)*60;
}

void readHeartrate(int value) {
  static int beat = 0;
  static int beatCounter = 0;
  static long lastTick = 0;
  
  if (value > 4000) {
    if (beat == 0) {
      beat = 1;
      //Serial2.print(value);
      //Serial2.print(" beat at ");
      //Serial2.println(counter);

      long timeElapsed = timerCounter.getCount();
      timerCounter.setCount(0);
//      Serial2.print(beatCounter);
//      Serial2.print(" - ");
//      Serial2.print(timeElapsed);
//      Serial2.print(" - ");
      beats[beatCounter] = timeElapsed;
      if (++beatCounter > 14) beatCounter = 0;
      lastTick = counter;

      double avg = calcAverage();
//      Serial2.print(avg);
//      Serial2.print(" - ");
    }
  } else if (value < 3000) {
    beat = 0;
  }
}

//int led1 = PE9;
//int led2 = PE10;
//int led3 = PE11;
int stepcount = 0;

int winkellast = 0;

float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
float xavg;
float yavg;
float zavg;
//int flag=0;
int sTeps=0;
float threshhold = 6400.0;
int tim = 0;

//float xval[15]={0};
//float yval[15]={0};
//float zval[15]={0};
//float xavg;
//float yavg;
//float zavg;
int steps,flag=0;
//float threshhold=2000.0;
int currentReading = 0;
float readings[10];


void sendData(int arg) {
  float energy = calcEnergy();
  float heartrate = calcBPM(calcAverage());

  Serial2.print("H:");
  Serial2.print(heartrate);
  Serial2.print(";S:");
  Serial2.print(mstepCounter);
  Serial2.print(";E:");
  Serial2.print(energy);
  Serial2.println();

  timerStart(1, 100, sendData, 0);
}

void readPedometer(int arg) {

  //int steps = getPedo(); //get the no. of steps
  if (currentReading == sizeof(readings)/sizeof(float)) {
    currentReading = 0;

    float energy = calcEnergy();
    float heartrate = calcBPM(calcAverage());
    
//    Serial2.print("Energy: ");
//    Serial2.println(energy);
//    Serial2.print("Heartrate: ");
//    Serial2.println(heartrate);
//    Serial2.println();

//    Serial2.print("{\"energy\" : ");
//    Serial2.print(energy);
//    Serial2.print(", \"heartrate\" : ");
//    Serial2.print(heartrate);
//    Serial2.println("}");

      //Serial2.println(energy);
  }

  float reading = getPedo();
  readings[currentReading] = reading;
  currentReading++;
  if(reading>mThreshold && mstepFlag==0){
    mstepFlag=1;
    mstepCounter++;
  }else if(reading<mThreshold && mstepFlag==1){
    mstepFlag=0;
  }
  //Serial2.println(reading);
    
  //Serial2.println(steps);

  timerStart(0, 20, readPedometer, 0);
}

int getPedo()
{
  int acc=0;
  float totvect[15]={0};
  float totave[15]={0};
  float xaccl[15]={0};
  float yaccl[15]={0};
  float zaccl[15]={0};
  /*readAccValues();
  xaccl[currentReading]=float(accx);
  yaccl[currentReading]=float(accy);
  zaccl[currentReading]=float(accz);
  totvect[currentReading]=sqrt(((xaccl[currentReading]-xavg)* (xaccl[currentReading]-xavg))+ ((yaccl[currentReading] - yavg)*(yaccl[currentReading] - yavg)) + ((zval[currentReading] - zavg)*(zval[currentReading] - zavg)));
  totave[currentReading]=(totvect[currentReading]+totvect[currentReading-1])/2;
  if(totave[currentReading]>mThreshold && flag==0){
    flag=1;
    return 1;
  }else if(totave[currentReading]<mThreshold && flag==1){
    flag=0;
    return 0;
  }else{
    return 0;
  }*/
  //problem 1, getPedo timer is 20, sendData data is 100, step count get all number in the array size of 10;
  //problem 2, 
  
  
  for (int i=0;i<15;i++)
  {
    readAccValues();
    xaccl[i]=float(accx);
    delay(1);
    yaccl[i]=float(accy);
    delay(1);
    zaccl[i]=float(accz);
    delay(1);
    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
    //delay(150);

    //cal steps 
    //Serial2.println(totave[i]);
    if (totave[i]>threshhold && flag==0)
    {
       steps=steps+1;
       flag=1;
    }
    else if (totave[i] > threshhold && flag==1)
    {
        //do nothing 
    }
    if (totave[i] <threshhold  && flag==1)
    {
      flag=0;
    }
   //Serial2.print("steps=");
   //Serial2.println(steps);
//   return steps;
    return totave[i];
  }
  //delay(100);
}

/*void calibrate()
{
  digitalWrite(13,HIGH);
  Serial2.println("Calibrating......");
  float sum=0;
  float sum1=0;
  float sum2=0;
  for (int i=0;i<15;i++)
  {
    readAccValues();
    xval[i]=float(accx);
    sum=xval[i]+sum;
  }
  delay(15);
  xavg=sum/15.0;
  Serial2.println(xavg);
  for (int j=0;j<15;j++)
  {
    readAccValues();
    yval[j]=float(accy);
    sum1=yval[j]+sum1;
  }
  yavg=sum1/15.0;
  Serial2.println(yavg);
  delay(15);
  for (int i=0;i<15;i++)
  {
    readAccValues();
    zval[i]=float(accz);
    sum2=zval[i]+sum2;
  }
  zavg=sum2/15.0;
  delay(15);
  Serial2.println(zavg);
  digitalWrite(13,LOW);
  Serial2.println("Calibration Successful!");
  //delay(1500);
}*/

float calcEnergy() {
  int i;
  float before = 0, after = 0, localmax = 0;
  int Index = 0;
  float count = 0;
  for (i = 0; i<sizeof(readings)/sizeof(float); i++) {
    count += readings[i];
  }
  int energy = count / float(sizeof(readings)/sizeof(float));
  //Serial2.print("steps counted: ");
  //Serial2.println(count);
  steps += count;
  mThreshold = fixThreshold(energy);
  return energy;
}



void stepLoop(int arg) {
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
//    Serial2.println(totave[i]);
  //  threshhold = fixThreshold(i, totave);
//    Serial2.println(threshhold);
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
//    Serial2.println('\n');
//    Serial2.print("steps=");
//    Serial2.println(sTeps);
  }
   delay(100);

   timerStart(2, 200, stepLoop, 0);
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

float fixThreshold(int newenergy){
  //float newthld;
  //float sumto;
  //for(int j = 0; j < 20; j++){
  //  sumto += to[i-j]; 
  //}
  //newthld = sumto/20.0;
  if(newenergy >= 4000.0){
    return (float)newenergy;  
  }else{
    return 4000.0;
  }
}

