#include <Metro.h>
#include "timer.h"
#include "lamp.h"

//PINS
int IRpin1 = 3;
int IRpin2 = 14;
int limitPin = 22;

//lamptimer that's actually just a Metro
timer *t;
lamp *l;
Metro *clock;

//personal info
int i, timelimit, personCount, innerState, outerState, in, out = 0;

void setup() {
  l = new lamp(11, 15, 23);
  pinMode(limitPin, INPUT);
  pinMode(IRpin1, INPUT);
  pinMode(IRpin2, INPUT);
  //Serial.println(analogRead(limitPin));
  timelimit = analogRead(limitPin) / 102;
  if (timelimit == 0) {
    timelimit = 1;
  }
  t = new timer(timelimit);
  clock = new Metro(200);
  Serial.begin(9600);
}

void loop() {
  l->lampmonitor();
  int read = limiter();
  if (timelimit != read) {
    timelimit = read;
    t->change(timelimit);
  }
  if (t->check()) {
    l->turnoff();
    personCount = 0;
  }
  if (clock->check()) {
    inOutStates();
  }
}

int limiter() {
  int t = analogRead(limitPin) / 102;
  if (t == 0) {
    return 1;
  }
  return t;
}

boolean state1() {
  return innerState < outerState;
}
boolean state2() {
  return innerState > outerState;
}
boolean state3() {
  return innerState == 1 && innerState == outerState;
}

void inOutStates() {
  innerState = digitalRead(IRpin1);
  outerState = digitalRead(IRpin2);
  if (state1() && in == 0 && out == 0) {
    in++;
  }
  if (state2() && in == 1 && out == 0) {
    in++;
  }
  if(state3() && in == 2) {
    personCount++;
    l->turnon();
    t->reset();
    in = 0;
    out = 0;
  }
  if (state2() && in == 0 && out == 0) {
    out++;
  }
  if (state1() && in == 0 && out == 1) {
    out++;
  }
  if(state3() && out == 2){
    personCount--;
    if(personCount < 0){
      personCount = 0;
    }
    if (personCount == 0) {
      l->turnoff();
    }
    out = 0;
    in = 0;
  }
}

