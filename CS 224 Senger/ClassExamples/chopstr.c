#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int
chopstr(char * str, int maxSubs, char ** subStr) {
  bool isSub = false;
  int i = 0;
  
  char * p = str;
  while (*p != '\0') {
    if (!isSub && !isspace(*p)) {
      if (i < maxSubs-1) {
        subStr[i++] = p;
      }
      isSub = true;
    }
    else if (isspace(*p)){
      *p = '\0';
      isSub = false;
    }
    p++;
  }
  return i;
}

int
main(int argc, char * argv[]) {
  
  if (argc != 2) {
    printf("usage: chopstr a-string\n");
    return -1;
  }
  
  char * lst[25];
  int cnt = chopstr(argv[1],25,lst);
  
  for (int i = 0; i < cnt; i++) {
    printf("%d -- \"%s\"\n",i,lst[i]);
  }
  
  return 0;
}
