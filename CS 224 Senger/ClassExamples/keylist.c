#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keylist.h"

struct listNode {
  void * data;
  struct listNode * next;
};

struct list {
  int (* cmpfunc)(const void *, const void *);
  struct listNode * enumNode;
  struct listNode * head;
};

list
makeList(int (* cmpfunc)(const void *, const void *)) {
  list aList = malloc(sizeof(struct list));
  aList->cmpfunc = cmpfunc;
  aList->enumNode = NULL;
  aList->head = NULL;
  return aList;
}

void
insertList(list aList, void * data) {
  struct listNode * node = aList->head;
  struct listNode * prevNode = NULL;
  
  while (node && aList->cmpfunc(node->data,data) < 0) {
    prevNode = node;
    node = node->next;
  }
  struct listNode * newNode = malloc(sizeof(struct listNode));
  newNode->data = data;
  if (prevNode) {
    // insertion not at head of list
    prevNode->next = newNode;
    newNode->next = node;
  }
  else {
    // insertion at head of list
    newNode->next = aList->head;
    aList->head = newNode;
  }
}

void *
firstData(list aList) {
  aList->enumNode = aList->head;
  return aList->enumNode->data;
}

void *
nextData(list aList) {
  aList->enumNode = aList->enumNode->next;
  return (aList->enumNode) ? (aList->enumNode->data) : (NULL);
}
