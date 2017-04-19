#include <Arduino.h>
int led1 = PE9;
int htinput = PA1;
void setup() {
  // put your setup code here, to run once:
  pinMode (led1, OUTPUT);
  pinMode (htinput, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if( analogRead(htinput) >= 550){
    digitalWrite(led1, HIGH);
  }else{
    digitalWrite(led1, LOW);
  }
  delay(50);
}
