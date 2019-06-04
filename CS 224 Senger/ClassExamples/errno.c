#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char * argv[]) {
  
  errno = 0;
  char * name = argv[1];
  char * value = (argc == 3) ? (argv[2]) : ("ignore");
  int ret = setenv(name,value,0);
  int errsave = errno;
  printf("ret %d\n",ret);
  printf("errno %d\n",errsave);
  if (errsave != 0) {
    perror("failure");
    return -1;
  }
  printf("env %s = %s \n",name,getenv(name));
}
