#include <stdlib.h>
#include <stdio.h>

#include "intstk.h"

// Stack implementation based upon realloc of array as stack size grows

struct intStack {
  unsigned int initialSize;
  unsigned int allocatedSize;
  unsigned int tos;
  int * contents;
};

intStack *
stkCreate(unsigned int size) {
  intStack * stk = malloc(sizeof(intStack));
  if (!stk) return NULL;
  stk->contents = malloc(size*sizeof(int));
  if (!stk->contents) {
    free(stk);
    return NULL;
  }
  stk->initialSize = size;
  stk->allocatedSize = size;
  stk->tos = 0;
  return stk;
}

int
stkEmpty(intStack * stk) {
  return (stk->tos == 0);
}

int
stkPush(intStack * stk,int c) {
  if (stk->tos >= stk->allocatedSize) {
    stk->allocatedSize += stk->initialSize;
    int * newContents = realloc(stk->contents,stk->allocatedSize * sizeof(int));
    if (!newContents) return 0;
    stk->contents = newContents;
  }
  stk->contents[stk->tos++] = c;
  return 1;
}

int
stkPop(intStack * stk) {
  if (!stkEmpty(stk)) return stk->contents[--(stk->tos)];
  return '\0';
}

int
stkTop(intStack * stk) {
  if (!stkEmpty(stk)) return stk->contents[stk->tos - 1];
  return '\0';
}

void
stkReset(intStack * stk) {
  stk->tos = 0;
}

void
stkState(char * str, struct intStack * stk) {
  printf("%s: %d %d %d -- ",str,stk->initialSize,stk->allocatedSize,stk->tos);
  for (int i = 0; i < stk->tos; i++) printf("%d ",stk->contents[i]);
  printf("\n");
}
