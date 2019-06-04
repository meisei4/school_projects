#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SampleAux.h"

struct NameNode * listHead;
int listCnt;

void
InitList() {
  // init list head pointer and cnt
  listHead = NULL;
  listCnt = 0;

  return;
}

void
AddName(char * name, int val) {
  // alloc space for node structure, set Seq and Val fields
  struct NameNode * node = malloc(sizeof(struct NameNode));
  node->seq = listCnt++;
  node->val = val;

  // get length of name
  int len = strlen(name);

  // malloc space for name and copy from arg
  node->name = malloc(len+1);
  strcpy(node->name,name);

  // alternatively, combine malloc and strcpy in strdup
  //node->Name = strdup(name);

  // node's next field points to current list head, head points to node
  node->next = listHead;
  listHead = node;

  return;
}

void
RemoveHead() {
  // get head node
  struct NameNode * node = listHead;

  // if no head node return
  if (!node) return;

  // head points to next
  listHead = node->next;

  // free node
  free(node->name);
  free(node);
}

char *
NameWithValue(int value) {
  // start at list head
  struct NameNode * node = listHead;

  // while node points to a node, check value field
  while (node) {
    if (node->val == value) {
      return node->name;
    }
    node = node->next;
  }

  return NULL;
}

void
DoForAll(void (*nameFunc)(struct NameNode *, void *),void * withArg) {
  // start at list head
  struct NameNode * node = listHead;

  // while node points to a node, write fields, advance to next
  while (node) {
    nameFunc(node,withArg);
    node = node->next;
  }

  return;
}
