#include <stdio.h>
#include <string.h>
#include <ctype.h>

void
alphaRange(char * string, int * start, int * end) {
  int s;
  int e;
  while (s < strlen(string) && !isalpha(string[s])) s++;
  e = s;
  while (e < strlen(string) && isalpha(string[e])) e++;
  *start = s;
  *end = e;
}

int
main(int argc, char *argv[]) {
  
  char * str = "012345Some Text<>";
  
  int s, e;
  alphaRange(str,&s,&e);
  
  printf("%d %d\n",s,e);
}
