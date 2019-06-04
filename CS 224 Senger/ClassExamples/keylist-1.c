#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
  char last[32];
  char first[32];
  unsigned int age;
};

struct person * list = NULL;
unsigned int size = 0;
unsigned int allocatedSize = 0;
unsigned int initialSize = 0;

void
insertList(struct person person) {
  if (size >= allocatedSize) {
    allocatedSize += initialSize;
    list = realloc(list,allocatedSize * sizeof(struct person));
    fprintf(stderr,"realloc'ing\n");
  }
  int i = 0;
  while (i < size && strcmp(list[i].last,person.last) < 0) i++;
  memmove(&(list[i+1]),&(list[i]),(size - i) * sizeof(struct person));
  list[i] = person;
  size++;
}

int
main(int argc, char * argv[]) {
  
  initialSize = 10;
  if (argc == 2) {
    int val = strtol(argv[1],NULL,10);
    if (val > 0) initialSize = val;
  }
  
  fprintf(stderr,"allocation size %d\n",initialSize);
  
  list = malloc(initialSize * sizeof(struct person));
  allocatedSize = initialSize;
  size = 0;
  
  struct person person;
  while (scanf("%31s %31s %d\n",person.last,person.first,&person.age) == 3) {
    insertList(person);
  }
  
  for (int i = 0; i < size; i++) {
    printf("%s %s %d\n",list[i].last,list[i].first,list[i].age);
  }
}
