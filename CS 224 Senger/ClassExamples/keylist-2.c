#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
  char last[32];
  char first[32];
  unsigned int age;
};

struct personList {
  unsigned int initialSize;
  unsigned int allocatedSize;
  unsigned int size;
  struct person * contents;
};

struct personList *
makeList(size_t initialSize) {
  struct personList * list = malloc(sizeof(struct personList));
  list->initialSize = initialSize;
  list->allocatedSize = initialSize;
  list->size = 0;
  list->contents = malloc(list->allocatedSize * sizeof(struct person));
  return list;
}

void
insertList(struct personList * list, struct person person) {
  if (list->size >= list->allocatedSize) {
    list->allocatedSize += list->initialSize;
    list->contents = realloc(list->contents, list->allocatedSize * sizeof(struct person));
    fprintf(stderr,"realloc'ing\n");
  }
  int i = 0;
  while (i < list->size && strcmp(list->contents[i].last,person.last) < 0) i++;
  memmove(&(list->contents[i+1]),&(list->contents[i]),(list->size - i) * sizeof(struct person));
  list->contents[i] = person;
  list->size++;
}

int
main(int argc, char * argv[]) {
  
  int initsize = 10;
  if (argc == 2) {
    int val = strtol(argv[1],NULL,10);
    if (val > 0) initsize = val;
  }
  
  fprintf(stderr,"allocation size %d\n",initsize);
  
  struct personList * list = makeList(initsize);
  
  struct person person;
  while (scanf("%31s %31s %d\n",person.last,person.first,&person.age) == 3) {
    insertList(list,person);
  }
  
  for (int i = 0; i < list->size; i++) {
    printf("%s %s %d\n",list->contents[i].last,list->contents[i].first,list->contents[i].age);
  }
}
