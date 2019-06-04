#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "typelist.h"

struct listNode {
  char * name;
  struct listNode * next;
};

void
addList(listHead *list, char * name) {
  struct listNode * newNode = malloc(sizeof(struct listNode));
  newNode->name = strdup(name);
  newNode->next = *list;
  *list = newNode;
}

void
printList(listHead l) {
  listHead p = l;
  while (p) {
    printf("name: \"%s\"\n",p->name);
    p = p->next;
  }
}
