/*
    Read stdin looping for sequences of digits. When found convert to integers and
    print the sum of all integers when done. 
 
    Relies on input ending in a newline. 
*/

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int
main(int argc, char * argv[])
{
  int c;
  int num;
  bool haveNum = false;
  int sum = 0;
  
  while ((c = getchar()) != EOF) {
    if (isdigit(c)) {
      num = num*10 + (c - '0');
      haveNum = true;
    }
    else {
      if (haveNum) {
        sum = sum + num;
        num = 0;
        haveNum = 0;
      }
    }
  }
  printf("sum %d\n",sum);
}
