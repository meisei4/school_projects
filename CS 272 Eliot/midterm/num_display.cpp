#include <Arduino.h>
#include "num_display.h"

void num_display::set_ones(int val){
  val = val%10;
  if(val == 0){
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    digitalWrite(20, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
  else if(val == 1){
    digitalWrite(20, LOW);
    digitalWrite(7, LOW);
  }else if(val == 2){
    digitalWrite(19, LOW);
    digitalWrite(20, LOW);
    digitalWrite(17, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
  }else if(val == 3){
    digitalWrite(19, LOW);
    digitalWrite(20, LOW);
    digitalWrite(17, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }else if(val == 4){
    digitalWrite(18, LOW);
    digitalWrite(17, LOW);
    digitalWrite(20, LOW);
    digitalWrite(7, LOW);
  }else if(val == 5){
    digitalWrite(19, LOW);
    digitalWrite(18, LOW);
    digitalWrite(17, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }else if(val == 6){
    digitalWrite(19, LOW);
    digitalWrite(18, LOW);
    digitalWrite(17, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }else if(val == 7){
    digitalWrite(19, LOW);
    digitalWrite(20, LOW);
    digitalWrite(7, LOW);
  }else if(val == 8){
    digitalWrite(19, LOW);
    digitalWrite(20, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(17, LOW);
    digitalWrite(18, LOW);
  }else if(val == 9){
    digitalWrite(19, LOW);
    digitalWrite(20, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(17, LOW);
    digitalWrite(18, LOW);
  }
}

void num_display::set_tens(int val){
  val = val/10;
  if(val == 1){
    digitalWrite(16 ,LOW);
    digitalWrite(10 ,LOW);
  }else if(val == 2){
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
  }else if(val == 3){
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(13, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }else if(val == 4){
    digitalWrite(14, LOW);
    digitalWrite(13, LOW);
    digitalWrite(16, LOW);
    digitalWrite(10, LOW);
  }else if(val == 5){
    digitalWrite(15, LOW);
    digitalWrite(14, LOW);
    digitalWrite(13, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }else if(val == 6){
    digitalWrite(15, LOW);
    digitalWrite(14, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
  }else if(val == 7){
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(10, LOW);
  }else if(val == 8){
    digitalWrite(15, LOW);
    digitalWrite(14, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(16, LOW);
  }else if(val == 9){
    digitalWrite(15, LOW);
    digitalWrite(14, LOW);
    digitalWrite(13, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(16, LOW);
  }
}

void num_display::clear_displays(){
  //tens_digits
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);

  //ones_digits
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(20, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
}

