#include <Arduino.h>
#include "protocol_read.h"
int twos[7] = {1, 2, 3, 4, 8, 32, 64};

int protocol_read::translate_signal(){
  int count = 7;
  int num = 0;
  delay(10);
  while(count >= 0){
    if(digitalRead(6)){
      //digitalWrite(13, LOW);
      num += twos[count];
    }
    delay(10);
  }
  return num;
}
