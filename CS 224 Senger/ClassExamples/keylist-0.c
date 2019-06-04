#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
  char last[32];
  char first[32];
  unsigned int age;
};

#define MAX_SIZE 64
struct person list[MAX_SIZE];
unsigned int size = 0;

void
insertList(struct person person) {
  if (size >= MAX_SIZE) return;
  int i = 0;
  while (i < size && strcmp(list[i].last,person.last) < 0) i++;
  memmove(&(list[i+1]),&(list[i]),(size - i) * sizeof(struct person));
  list[i] = person;
  size++;
}

int
main(int argc, char * argv[]) {
  struct person person;
  while (scanf("%31s %31s %d\n",person.last,person.first,&person.age) == 3) {
    insertList(person);
  }
  
  for (int i = 0; i < size; i++) {
    printf("%s %s %d\n",list[i].last,list[i].first,list[i].age);
  }
}
