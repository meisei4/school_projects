#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//===========================================
struct listNode {
  void * data;
  struct listNode * next;
};

struct list {
  unsigned int size;
  struct listNode * head;
};

struct list *
makeList() {
  struct list * list = malloc(sizeof(struct list));
  list->size = 0;
  list->head = NULL;
  return list;
}

void
insertList(struct list * list, void * data, int (* cmpfunc)(const void *, const void *)) {
  struct listNode * node = list->head;
  struct listNode * prevNode = NULL;
  
  while (node && cmpfunc(node->data,data) < 0) {
    prevNode = node;
    node = node->next;
  }
  
  struct listNode * newNode = malloc(sizeof(struct listNode));
  newNode->data = data;
  
  list->size++;
  if (prevNode) {
    // insertion not at head of list
    prevNode->next = newNode;
    newNode->next = node;
  }
  else {
    // insertion at head of list
    newNode->next = list->head;
    list->head = newNode;
  }
}
//===========================================

struct person {
  char * last;
  char * first;
  unsigned int age;
};

struct person *
makePerson(char * last, char * first, unsigned int age) {
  struct person * person = malloc(sizeof(struct person));
  person->last = strdup(last);
  person->first = strdup(first);
  person->age = age;
  return person;
}

int
comparePerson(const void * a, const void * b) {
  struct person * ap = (struct person *) a;
  struct person * bp = (struct person *) b;
  return strcmp(ap->last,bp->last);
}

int
main(int argc, char * argv[]) {
  struct list * list = makeList();
  
  char last[64], first[64];
  unsigned int age;
  while (scanf("%63s %63s %d\n",last,first,&age) == 3) {
    struct person * person = makePerson(last,first,age);
    insertList(list,person,comparePerson);
  }
  
  struct listNode * node = list->head;
  while (node) {
    struct person * person = node->data;
    printf("%s %s %d\n",person->last,person->first,person->age);
    node = node->next;
  }
}





