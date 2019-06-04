#include <stdlib.h>
#include <string.h>

#include "da.h"

#define MAX(x,y) ( (x < y) ? (y) : (x) )

struct dynArray {
  unsigned int eltSize;
  unsigned int allocedBytes;
  unsigned int nextIndex;
  void * nullElt;
  void * contents;
};

DA *
makeDA(unsigned int eltSize, void * nullElt)
{
  struct dynArray * da = malloc(sizeof(struct dynArray));
  da->eltSize = eltSize;
  da->allocedBytes = eltSize;
  da->nextIndex = 0;
  da->nullElt = nullElt;
  da->contents = malloc(da->allocedBytes);
  memcpy(da->contents,da->nullElt,da->eltSize);
  return da;
}

void
freeDA(DA * da) {
  free(da->contents);
  free(da);
}

void
expand(DA * da, unsigned int indx) {
  if (indx >= da->allocedBytes / da->eltSize) {
    unsigned int newSize = MAX(indx * da->eltSize,da->allocedBytes + 10 * da->eltSize);
    da->contents = realloc(da->contents,newSize);
    void * p = da->contents + da->allocedBytes;
    while (p < da->contents + newSize) {
      memcpy(p,da->nullElt,da->eltSize);
      p += da->eltSize;
    }
    da->allocedBytes = newSize;
  }
}

void
put(DA * da, unsigned int indx, void * data) {
  expand(da,indx);
  da->nextIndex = indx + 1;
  memcpy(da->contents + indx * da->eltSize,data,da->eltSize);
}

const void *
get(DA * da, unsigned int indx) {
  expand(da,indx);
  return da->contents + indx * da->eltSize;
}

unsigned int
lastIndex(DA * da) {
  return da->nextIndex - 1;
}
