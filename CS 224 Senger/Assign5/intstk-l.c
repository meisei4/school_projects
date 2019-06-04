#include <stdlib.h>
#include <stdio.h>

#include "intstk.h"

// Stack implementation

struct intStack {
  unsigned int allocatedSize;
  struct intStackNode * head;
};

struct intStackNode {
  unsigned int * data;
  struct intStackNode * next;
  unsigned int tos;
  //top of stack index
};

struct intStackNode *
makeNewNode(intStack * stk) {
  struct intStackNode * tmp = malloc(sizeof(struct intStackNode));
  tmp->data = malloc(stk->allocatedSize * sizeof(int));
  tmp->tos = 0;
  tmp->next = NULL;
  return tmp;
}

intStack *
stkCreate(unsigned int size) {
  struct intStack * stk  = malloc(sizeof(struct intStack));
  stk->allocatedSize = size;
  stk->head = makeNewNode(stk);
  return stk;
}

int
stkEmpty(intStack * stk) {
  return (stk->head->tos == 0 && stk->head->next == NULL);
}

int
stkPush(intStack * stk, int v) {
  if(stk->head->tos >= stk->allocatedSize){
    struct intStackNode * newNode = makeNewNode(stk); 
    newNode->next = stk->head;
    stk->head = newNode; 
  }
  stk->head->data[stk->head->tos++] = v;
  return 1;
}

int
stkPop(intStack * stk) {
  if(stkEmpty(stk)){
      return '\0';
    }else{
      if(stk->head->tos == 0 && stk->head->next != NULL){
        struct intStackNode * tempNode = stk->head;
        stk->head = stk->head->next;
        free(tempNode->data);
        free(tempNode);
      }
    int top = stk->head->data[stk->head->tos - 1];
    stk->head->tos--;
    return top;
  }
 
}

int
stkTop(intStack * stk) {
  if(stkEmpty(stk)){
    return '\0';
  }
  if(stk->head->tos > 0){
    return stk->head->data[stk->head->tos - 1];
  }
  if(stk->head->next != NULL){
    return stk->head->next->data[stk->head->next->tos-1];
  }
}


void
stkReset(intStack * stk) {
  stk->head->next = NULL;
  stk->head->tos = 0;
}

void
stkState(char * str, intStack * stk) {
  printf("%s: %d %d -- ",str,stk->allocatedSize,stk->head->tos);
  for (int i = 0; i < stk->head->tos; i++) printf("%d ",stk->head->data[i]);
  printf("\n");
}
