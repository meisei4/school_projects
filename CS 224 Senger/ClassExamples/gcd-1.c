#include <stdio.h>

int
main(int argc, char * argv[])
{ int x, y;

  x = 35;
  y = 21;

  printf("gcd(%d,%d) = ",x,y);
  
  while (x != y) {
    if (x < y) {
      y = y - x;
    }
    else {
      x = x - y;
    }
  }

  printf("%d\n",x);
}