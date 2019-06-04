#include <Arduino.h>
#include "lamp.h"
int buttonState = 0;
int prevState = buttonState;
int inbut, outbut, outlamp, inpot, outpot, count;
int lampState = 0;
int dimness = 0;
  
lamp::lamp(int i1, int i2, int o3) {
  inbut = i1;
  inpot = i2;
  outlamp = o3;

  pinMode(inbut, INPUT);
  pinMode(inpot, INPUT);
  pinMode(outlamp, OUTPUT);
  Serial.begin(9600);
}

void lamp::lampdim(){
  dimness = analogRead(inpot)/4;
  if(dimness <= 50){
    dimness = 50;
  }
  analogWrite(outlamp, dimness);
}

void lamp::lampswitch(){
  if(lampState == 0){
    turnon();
  }else{
    turnoff();
  }
}

void lamp::lampmonitor(){
  if(lampState == 1){
    lampdim();
  }
  debouncebut();
  if(buttonState == 0 && prevState == 1){
    lampswitch();
  }
  prevState = buttonState;
}

void lamp::turnoff(){
  analogWrite(outlamp, 0);
  lampState = 0;
}

void lamp::turnon(){
  analogWrite(outlamp, dimness);
  lampState = 1;
}

void lamp::debouncebut(){
  int reading = digitalRead(inbut);
  //Serial.print(reading);
  if(reading != buttonState){
    count++;
  }else{
    count = 0;
  }
  if(count > 80){
    prevState = buttonState;
    buttonState = reading;
    count = 0;
  }
}
