#include <stdio.h>

#include "intstk.h"

int
priority(int op) {
  switch (op) {
    case '+': return 1;
    case '-': return 1;
    case '*': return 2;
    case '/': return 2;
    case '%': return 2;
    default: return 0;
  }
}

void
doOp(struct intStack * stk, int op) {
  if (stkEmpty(stk)) { printf("syntax error\n"); return;}
  int v2 = stkPop(stk);
  if (stkEmpty(stk)) { printf("syntax error\n"); return;}
  int v1 = stkPop(stk);
  switch (op) {
    case '+':
      stkPush(stk,v1+v2);
      break;
    case '-':
      stkPush(stk,v1-v2);
      break;
    case '*':
      stkPush(stk,v1*v2);
      break;
    case '/':
      stkPush(stk,v1/v2);
      break;
    case '%':
      stkPush(stk,v1 % v2);
  }
}

int
main(int argc, char * argv[]) {
  struct intStack * evalstk = stkCreate(5);
  struct intStack * opstk = stkCreate(5);
  
  int inDigits = 0;
  
  int c;
  while ((c = getchar()) != EOF) {
    switch (c) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9': {
        if (!inDigits) {
          stkPush(evalstk,0);
        }
        int v = stkPop(evalstk);
        v = 10 * v + (c - '0');
        stkPush(evalstk,v);
        inDigits = 1;
        } break;
      case '+': case '-': case '*': case '/': case '%':
        inDigits = 0;
        while (!stkEmpty(opstk) && priority(stkTop(opstk)) >= priority(c)) {
          doOp(evalstk,stkPop(opstk));
        }
        stkPush(opstk,c);
        break;
      case '(':
        stkPush(opstk,c);
        inDigits = 0;
        break;
      case ')':
        while (!stkEmpty(opstk) && stkTop(opstk) != '(') {
          doOp(evalstk,stkPop(opstk));
        }
        stkPop(opstk);
        inDigits = 0;
        break;
      case '=':
        while (!stkEmpty(opstk)) {
          doOp(evalstk,stkPop(opstk));
        }
        if (stkEmpty(evalstk)) {
          printf("eval stk empty\n");
        }
        else {
          printf("res = %d\n",stkPop(evalstk));
        }
        inDigits = 0;
        break;
      case '\n':
        stkReset(evalstk);
        stkReset(opstk);
        break;
      default:
        break;
    }
  }
  
}
