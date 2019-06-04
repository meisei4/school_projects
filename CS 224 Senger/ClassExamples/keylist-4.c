#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
  char last[32];
  char first[32];
  unsigned int age;
};

struct personNode {
  struct person person;
  struct personNode * next;
};

struct personList {
  unsigned int size;
  struct personNode * head;
};

struct personList *
makeList() {
  struct personList * list = malloc(sizeof(struct personList));
  list->size = 0;
  list->head = NULL;
  return list;
}

void
insertList(struct personList * list, struct person person) {
  struct personNode * node = list->head;
  struct personNode * prevNode = NULL;
  
  while (node && strcmp(node->person.last,person.last) < 0) {
    prevNode = node;
    node = node->next;
  }
  
  struct personNode * newNode = malloc(sizeof(struct personNode));
  newNode->person = person;
  
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

int
main(int argc, char * argv[]) {
  struct personList * list = makeList();
  
  struct person person;
  while (scanf("%31s %31s %d\n",person.last,person.first,&person.age) == 3) {
    insertList(list,person);
  }
  
  struct personNode * node = list->head;
  while (node) {
    printf("%s %s %d\n",node->person.last,node->person.first,node->person.age);
    node = node->next;
  }
}
