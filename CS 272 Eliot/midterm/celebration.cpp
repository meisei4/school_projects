#include <Arduino.h>
#include "celebration.h"

//constructor
celebration::celebration(int step){
  if(step == 0){
    digitalWrite(15, LOW);
    digitalWrite(19, LOW);
  }else if(step == 1){
    digitalWrite(15, HIGH);
    digitalWrite(19, HIGH);
    
    digitalWrite(16, LOW);
    digitalWrite(20, LOW);
  }else if(step == 2){
    digitalWrite(16, HIGH);
    digitalWrite(20, HIGH);

    digitalWrite(10, LOW);
    digitalWrite(7, LOW);
  }else if(step == 3){
    digitalWrite(10, HIGH);
    digitalWrite(7, HIGH);

    digitalWrite(11, LOW);
    digitalWrite(8, LOW);
  }else if(step == 4){
    digitalWrite(11, HIGH);
    digitalWrite(8, HIGH);
    
    digitalWrite(12, LOW);
    digitalWrite(9, LOW);
  }else if(step == 5){
    digitalWrite(12, HIGH);
    digitalWrite(9, HIGH);

    digitalWrite(14, LOW);
    digitalWrite(18, LOW);
  }else if(step == 6){
    digitalWrite(14, HIGH);
    digitalWrite(18, HIGH);
  }
}

