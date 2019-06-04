#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keylist.h"

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
comparePersons(const void * a, const void * b) {
  struct person * ap = (struct person *) a;
  struct person * bp = (struct person *) b;
  return strcmp(ap->last,bp->last);
}

int
main(int argc, char * argv[])
{
  list peopleList = makeList(comparePersons);
  
  char last[64], first[64];
  unsigned int age;
  while (scanf("%63s %63s %d\n",last,first,&age) == 3) {
    struct person * person = makePerson(last,first,age);
    insertList(peopleList,person);
  }
  
  struct person * person = firstData(peopleList);
  while (person) {
    printf("%s %s %d\n",person->last,person->first,person->age);
    person = nextData(peopleList);
  }
}
