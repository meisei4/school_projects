#include <stdlib.h>
#include <stdio.h>

#include "intstk.h"

struct intStack {
  unsigned int orgSize;
  unsigned int max;
  unsigned int tos;
  int * stk;
};

intStack *
stkCreate(unsigned int size) {
  intStack * stk = malloc(sizeof(intStack));
  if (!stk) return NULL;
  stk->stk = malloc(size*sizeof(int));
  if (!stk->stk) {
    free(stk);
    return NULL;
  }
  stk->orgSize = size;
  stk->max = size;
  stk->tos = 0;
  return stk;
}

int
stkEmpty(intStack * stk) {
  return (stk->tos == 0);
}

int
stkPush(intStack * stk,int c) {
  if (stk->tos >= stk->max) {
    stk->max += stk->orgSize;
    int * newStk = realloc(stk->stk,stk->max*sizeof(int));
    if (!newStk) return 0;
    stk->stk = newStk;
  }
  stk->stk[stk->tos++] = c;
  return 1;
}

int
stkPop(intStack * stk) {
  if (!stkEmpty(stk)) return stk->stk[--(stk->tos)];
  return '\0';
}

int
stkTop(intStack * stk) {
  if (!stkEmpty(stk)) return stk->stk[stk->tos - 1];
  return '\0';
}

void
stkReset(intStack * stk) {
  stk->tos = 0;
}

void
stkState(char * str, struct intStack * stk) {
  printf("%s: %d %d %d -- ",str,stk->orgSize,stk->max,stk->tos);
  for (int i = 0; i < stk->tos; i++) printf("%d ",stk->stk[i]);
  printf("\n");
}
