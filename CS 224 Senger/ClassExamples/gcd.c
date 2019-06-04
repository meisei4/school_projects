#include <stdio.h>
#include <stdlib.h>

int
gcd(int x, int y) {
  while (x != y) {
    if (x < y) {
      y = y - x;
    }
    else {
      x = x - y;
    }
  }
  return x;
}

int
main(int argc, char * argv[]) {
  
  if (argc == 3) {
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    if (x != 0 && y != 0) {
      printf("%d\n",gcd(x,y));
    }
  }
  else {
    printf("usage: gcd num1 num2\n");
  }
  
  return 0;
}
