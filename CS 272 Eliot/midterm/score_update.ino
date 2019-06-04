#include "celebration.h"
#include "num_display.h"
#include "protocol_read.h"

int read_input = 6;
int pin_7 = 7;
int pin_8 = 8;
int pin_9 = 9;
int pin_10 = 10;
int pin_11 = 11;
int pin_12 = 12;
int pin_13 = 13;
int pin_14 = 14;
int pin_15 = 15;
int pin_16 = 16;
int pin_17 = 17;
int pin_18 = 18;
int pin_19 = 19;
int pin_20 = 20;
int val = 0;

void setup() {
  pinMode(read_input, INPUT);
  pinMode(pin_7, OUTPUT);
  digitalWrite(pin_7, HIGH);
  pinMode(pin_8, OUTPUT);
  digitalWrite(pin_8, HIGH);
  pinMode(pin_9, OUTPUT);
  digitalWrite(pin_9, HIGH);
  pinMode(pin_10, OUTPUT);
  digitalWrite(pin_10, HIGH);
  pinMode(pin_11, OUTPUT);
  digitalWrite(pin_11, HIGH);
  pinMode(pin_12, OUTPUT);
  digitalWrite(pin_12, HIGH);
  pinMode(pin_13, OUTPUT);
  digitalWrite(pin_13, HIGH);
  pinMode(pin_14, OUTPUT);
  digitalWrite(pin_14, HIGH);
  pinMode(pin_15, OUTPUT);
  digitalWrite(pin_15, HIGH);
  pinMode(pin_16, OUTPUT);
  digitalWrite(pin_16, HIGH);
  pinMode(pin_17, OUTPUT);
  digitalWrite(pin_17, HIGH);
  pinMode(pin_18, OUTPUT);
  digitalWrite(pin_18, HIGH);
  pinMode(pin_19, OUTPUT);
  digitalWrite(pin_19, HIGH);
  pinMode(pin_20, OUTPUT);
  digitalWrite(pin_20, HIGH);
  Serial.begin(9600); //printing
}

void loop() {
  //minimum 5ms wait between scores
  delay(5);
  //read and translate sequence
  protocol_read *p = new protocol_read();
  val = p->translate_signal(); //10ms read delay in function
  if(val != 0){
    //celebration sequence
    for(int i = 0; i < 7; i++){
      celebration *c = new celebration(i);
      delay(100);
      delete c;
      }
      //print val to scoreboard
      num_display *d = new num_display();
      d->clear_displays();
      d->set_tens(val);
      d->set_ones(val);
      delete d;    
    }
  delete p;
}
