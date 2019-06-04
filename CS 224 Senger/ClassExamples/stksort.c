#include <stdio.h>
#include "intstk.h"

int
main(int argc, char * argv[]) {
  intStack * incStk = stkCreate(5);
  intStack * decStk = stkCreate(5);
  
  // read chars, push onto stack keeping stack in increasing order
  int c;
  while ((c = getchar()) != EOF) {
    if (stkEmpty(incStk) || c >= stkTop(incStk)) {
      if (!stkPush(incStk,c)) perror("stkPush(incStk)");
    }
    else {
      while (!stkEmpty(incStk) && c < stkTop(incStk)) {
        if (!stkPush(decStk,stkPop(incStk))) perror("stkPush(decStk)");
      }
      if (!stkPush(incStk,c)) perror("stkPush(incStk)");
      while (!stkEmpty(decStk)) {
        if (!stkPush(incStk,stkPop(decStk))) perror("stkPush(decStk)");
      }
    }
  }
  
  // shift incStk to decStk reversing order
  while (!stkEmpty(incStk)) stkPush(decStk,stkPop(incStk));
  
  // display decStk
  while (!stkEmpty(decStk)) putchar(stkPop(decStk));
  
  printf("\n");
}