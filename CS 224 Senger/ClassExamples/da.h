#ifndef DA_H
#define DA_H

typedef struct dynArray DA;

DA *          makeDA(unsigned int eltSize, void * nullElt);
void          freeDA(DA * da);
void          put(DA * da, unsigned int indx, void * data);
const void *  get(DA * da, unsigned int indx);
unsigned int  lastIndex(DA * da);

#endif
