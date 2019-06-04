#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>

int
main(int argc, char * argv[]) {
  int maxSize;
  
  if (argc == 2) {
    maxSize = strtol(argv[1],NULL,10);
  }
  if (maxSize <= 0) {
    maxSize = 1024;
  }
  
  int lastSize = 0;
  int lastAllocSize = 0;
  
  for (int i = 1; i < maxSize; i++) {
    char * b = malloc(i);
    size_t allocSize = malloc_size(b); // on linux use malloc_usable_size()
    free(b);
    if (lastAllocSize != allocSize) {
      printf("size %d -- %d alloc %d inc by %lu\n",lastSize,i-1,lastAllocSize,allocSize-lastAllocSize);
      lastSize = i;
      lastAllocSize = allocSize;
    }
  }
}
