#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ec.h"

struct ecData {
    int size;
    int * data;
};

struct ecData *
makeEC(unsigned int size){
    struct ecData * eq = malloc(sizeof(struct ecData));
    eq->size = size;
    eq->data = malloc(size * 4);
}

void
freeEC(struct ecData * ec){
    free(ec->data);
}

int
classRepFor(struct ecData *ec, unsigned int elt){
    int * chain = ec->data;
    int parent = chain[elt];
    int e = elt;
    while(parent != -1){
        e = chain[parent];
        parent = chain[parent];
    }
    return e;
}

void
equate(struct ecData * ec, int a, int b){
    ec->data[a] = b;
}

int * 
classReps(struct ecData * ec){
    int i = 0; 
    int j = 0;
    int * reps = malloc(sizeof(ec->data));
    while(i < ec->size){
        if(classRepFor(ec, i) == i){
               reps[j++] = i;
        }
        i++;
    }
    reps[j] = -1;
    return reps;
}

int
classCount(struct ecData *ec){
    int i = 0;
    int j = 0;
    while(i < ec->size){
        if(classRepFor(ec, i) == i){
            j++;
        }
    }
    return j;
}   

int *
classOf(struct ecData * ec, unsigned int elt){
    int i = 0;
    int j = 0;
    int * class = malloc(sizeof(ec->data));
    while(i < ec->size){
        if(ec->data[i] == elt){
            class[j++] = ec->data[i];
        }
        i++;
    }
    class[j] = -1;
    return class;
}

void
displayData(struct ecData * ec){
    int i = 0;
    printf("index");
    while(i < ec->size){
        printf("%2d", i++);
    }
    printf("\n parent");
    i = 0;
    while(i < ec->size){
        printf("%2d", ec->data[i++]);
    }
}
















