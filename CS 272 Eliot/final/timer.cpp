#include <Arduino.h>
#include "timer.h"
#include <Metro.h>

Metro * m;

timer::timer(int limit){
  m = new Metro(limit * 60000);
}

boolean timer::check(){
  return m->check();
}

void timer::change(int limit){
  m->interval(limit * 60000);
}

void timer::reset(){
  m->reset();
}

