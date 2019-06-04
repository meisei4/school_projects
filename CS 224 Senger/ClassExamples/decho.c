#include <stdio.h>
#include <ctype.h>

int
main(int argc, char * argv[]) {
  
  int c;
  int cnt = 0;
  while ( (c = getchar()) != EOF) {
    if (isprint(c) && !isspace(c)) {
      printf("%4c",c);
    }
    else {
      printf("%4d",c);
    }
    cnt++;
    if (cnt % 16 == 15) {
      putchar('\n');
      cnt = 0;
    }
  }
  printf(" EOF\n");
}
