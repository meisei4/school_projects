#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "da.h"

struct person {
  char last[32];
  char first[32];
  unsigned int age;
};


int
main(int argc, char * argv[]) {
  
  struct person nullPerson = {NULL, NULL, 0};
  
  DA * da = makeDA(sizeof(struct person),&nullPerson);
  
  struct person person;
  
  int indx = 0;
  while (scanf("%31s %31s %d\n",person.last,person.first,&person.age) == 3) {
    put(da,indx,&person);
    indx++;
  }
  
  for (int i = 0; i < indx; i++) {
    const struct person * p = get(da,i);
    printf("%s %s %d\n",p->last,p->first,p->age);
  }
  
  freeDA(da);
}
