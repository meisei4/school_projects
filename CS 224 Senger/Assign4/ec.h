#ifndef EC_H
#define EC_H

typedef struct ecData EC;

EC *  makeEC(unsigned int size);
void  freeEC(EC * ec);
int   classRepFor(EC * ec, unsigned int elt);
void  equate(EC * ec, int a, int b);
int * classReps(EC * ec);
int   classCount(EC * ec);
int * classOf(EC * ec,unsigned int elt);
void  displayData(EC * ec);

#endif
