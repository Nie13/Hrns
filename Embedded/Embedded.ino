#include <Arduino.h>
#include <functiontimer.h>
#include <ringbuffer.h>
#include <lsm303.h>

#include <libmaple/i2c.h>
#include <math.h>

int led1 = PE9;
int led2 = PE10;
int led3 = PE11;
int led4 = PE12;

double magnitude;
double minMagnitude;
double maxMagnitude;
int steps;

int htinput = PC1;
long counter = 0;

int beats[15];
int steps[15];

void TimerCounter(int arg) {
  counter += 5;

  timerStart(0, 5, TimerCounter, 0);
}

void DisplayTimer(int arg) {
  Serial2.print("current: ");
  Serial2.print(magnitude);
  Serial2.print(", min: ");
  Serial2.print(minMagnitude);
  Serial2.print(", max: ");
  Serial2.print(maxMagnitude);
  Serial2.print(", steps: ");
  Serial2.println(steps);
  
  timerStart(1, 500, DisplayTimer, 0);
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(htinput, INPUT);

  Serial2.begin(115200);

  timerInit();
  timerStart(0, 5, TimerCounter, 0);
  timerStart(1, 500, DisplayTimer, 0);

  i2c_master_enable(I2C1, 0);
  lsm303Init();
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

void loop() {
  static int beat = 0;
  int value = analogRead(htinput);
  readAccValues();
  magnitude = sqrt(pow(accx, 2) + pow(accy, 2) + pow(accz, 2));

  minMagnitude = 99999999999;
  maxMagnitude = 0;

  if (magnitude < min) min = magnitude;
  if (magnitude > max) max = magnitude;
  
  static int step = 0;
  steps = 0;

  /*
  Serial2.print("current: ");
  Serial2.print(magnitude);
  Serial2.print(", min: ");
  Serial2.print(min);
  Serial2.print(", max: ");
  Serial2.print(max);
  Serial2.print(", steps: ");
  Serial2.println(steps);

  delay(500); */

  if (magnitude > 18000) {
    if (step == 0) {
      step = 1;
      steps++;
    }
  } else if (magnitude < 17000) {
    step = 0;
  }

  /*
  static int beatCounter = 0;
  static long lastTick = 0;
  
  updateLEDs(value);
  
  if (value > 4000) {
    if (beat == 0) {
      beat = 1;
      Serial2.print(value);
      Serial2.print(" beat at ");
      Serial2.println(counter);

      long timeElapsed = counter - lastTick;
      Serial2.print(beatCounter);
      Serial2.print(" - ");
      Serial2.print(timeElapsed);
      Serial2.print(" - ");
      beats[beatCounter] = timeElapsed;
      if (++beatCounter > 14) beatCounter = 0;
      lastTick = counter;

      double avg = calcAverage();
      Serial2.print(avg);
      Serial2.print(" - ");
      Serial2.println(calcBPM(avg));
    }
  } else if (value < 3000) {
    beat = 0;
  }
  */
}
