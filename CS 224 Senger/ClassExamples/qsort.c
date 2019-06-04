#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
cmp(const void * a, const void * b) {
  const char ** ap = (const char **) a;
  const char ** bp = (const char **) b;
  return strcmp(*ap,*bp);
}

int
main(int argc, char * argv[]) {
  
  char * names[] = {"Sarah", "Billy", "Stella", "Mitchell", "Birdie"};

  qsort(names,5,sizeof(char *), cmp);
  
  for (int i = 0; i < 5; i++) {
    printf("%s\n",names[i]);
  }
}
