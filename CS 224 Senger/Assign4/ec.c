#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ec.h"

struct ecData {
	int size;
	int * data;
};

struct ecData *
makeEC(unsigned int size) {
	struct ecData * equiclass = malloc(sizeof(struct ecData));
	equiclass->size = size;
	equiclass->data = malloc(size * sizeof(int));
	int i  = 0;
	while(i < size){	
		equiclass->data[i] = -1;
		i++;
	}
	return equiclass;
}

void
freeEC(struct ecData * ec) {
	free(ec->data);
  free(ec);
}

int
classRepFor(struct ecData * ec, unsigned int elt) {
	int * chain = ec->data;
	int parent = chain[elt];
	int e = elt;
	while(parent != -1){
		e = parent;
		parent = chain[parent];	
	}
	return e;

}

void
equate(struct ecData * ec, int a, int b) {
  int repa = classRepFor(ec, a);
  int repb = classRepFor(ec, b);
  if(repa != repb){
    ec->data[repa] = repb;
  }
}

int *
classReps(struct ecData * ec) {
	int i = 0;
	int * reps = malloc((ec->size+ 1) * sizeof(int));
	int j = 0;
	while(i < ec->size){
		if(ec->data[i] == -1){
			reps[j] = i;
			j++;
		}
		i++;
	}
	reps[j] = -1;
	return reps;
}

int
classCount(struct ecData * ec) {
	int i = 0;
	int j = 0;
	while(i < ec->size){
		if(ec->data[i] == -1){
			j++;
		}
		i++;
	}
	return j;
}

int *
classOf(struct ecData * ec, unsigned int elt){
	int i = 0;
	int j = 0; 
	int * class = malloc((ec->size+2) * sizeof(int));
	while(i < ec->size){
		if(classRepFor(ec,i) == elt){
			class[j] = i;
			j++;
		}
		i++;
	}
	class[j] = -1;
  return class;
}

void
displayData(struct ecData * ec) {
	int i = 0;
	printf("index ");
	while(i < ec->size){
		printf("%4d", i++);
	}
	printf("\nparent");
	i = 0;
	while(i < ec->size){
		printf("%4d", ec->data[i++]);
	}
	printf("\n");
}

