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

struct personNode *
insertList(struct personNode * list, struct person person) {
  struct personNode * node = list;
  struct personNode * prevNode = NULL;
  
  while (node && strcmp(node->person.last,person.last) < 0) {
    prevNode = node;
    node = node->next;
  }
  // new node goes between prevNode and node
  struct personNode * newNode = malloc(sizeof(struct personNode));
  newNode->person = person;
  
  if (prevNode) {
    // insertion not at head of list
    prevNode->next = newNode;
    newNode->next = node;
    return list;
  }
  else {
    // insertion at head of list
    newNode->next = list;
    return newNode;
  }
}

void
insertListAlt(struct personNode * * listRef, struct person person) {
  struct personNode * node = * listRef;
  struct personNode * prevNode = NULL;
  while (node && strcmp(node->person.last,person.last) < 0) {
    prevNode = node;
    node = node->next;
  }
  struct personNode * newNode = malloc(sizeof(struct personNode));
  newNode->person = person;
  if (prevNode) {
    prevNode->next = newNode;
    newNode->next = node;
  }
  else {
    newNode->next = * listRef;
    *listRef = newNode;
  }
}

int
main(int argc, char * argv[]) {
  struct personNode * list = NULL;
  
  struct person person;
  while (scanf("%31s %31s %d\n",person.last,person.first,&person.age) == 3) {
    list = insertList(list,person);
    //insertListAlt(&list,person);
  }
  
  struct personNode * node = list;
  while (node) {
    printf("%s %s %d\n",node->person.last,node->person.first,node->person.age);
    node = node->next;
  }
}
