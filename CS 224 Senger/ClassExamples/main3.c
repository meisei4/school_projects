#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char * argv[], char * envp[]) {
  
  for (int i = 0; argv[i]; i++) {
    printf("argv %d -- \"%s\"\n",i,argv[i]);
  }
  for (int i = 0; envp[i]; i++) {
    printf("env %d -- \"%s\"\n",i,envp[i]);
  }
}
