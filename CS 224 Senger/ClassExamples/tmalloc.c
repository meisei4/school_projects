#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void
hexDump(unsigned char * p, int bytes) {
  for (int i = 0; i < bytes; i++) {
    if (i != 0 && i % 16 == 0) printf("\n");
    if (isprint(*p)) {
      printf(" %c ",*p++);
    }
    else {
      printf("%2x ",*p++);
    }
  }
  printf("\n");
}

int
main(int argc, char * argv[]) {
  char * s = "ABCDEFG";
  char * a = malloc(20);
  char * b = malloc(10);
  char * c = malloc(10);
  
  printf("a %p b %p c %p\n",a,b,c);
  
  strcpy(a,"0123456789");
  strcpy(b,s);
  strcpy(c,"abcd");
  
  hexDump((unsigned char *)a-32,6*16);
}
