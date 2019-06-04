#include <stdio.h>

#include "multi-1.h"

int multi_global_int = 0;

int
main(int argc, char * argv[]) {
  char msg[] = "invocation count";
  int i;
  
  for (i =0; i < 10; i++) {
    countInvocations();
  }
  
  printf("%s %d\n",msg,invocationCount);
}
