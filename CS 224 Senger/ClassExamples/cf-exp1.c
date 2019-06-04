#include <stdio.h>

void
f(int * v)
{
  printf("before %d\n",*(v-1));
  printf("at     %d\n",*v);
  printf("after  %d\n",*(v+1));
  *(v-1) = *v;
  *(v+1) = *v;
}

int
main(int argc, char * argv[])
{
  int x = 1;
  int y = 2;
  int z = 3;
  
  f(&y);
  
  printf("x: %d y: %d z: %d\n",x,y,z);
}