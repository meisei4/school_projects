#include <stdio.h>


int
f(int n)
{
  int vla[n];
  
  for (int i = 0; i < n; i++) vla[i] = i;
  int sum = 0;
  for (int i = 0; i < n; i++) sum += vla[i];
  
  return sum;
}

int
main(int argc, char * argv[])
{
  printf("5 %d\n",f(5));
  printf("100 %d\n",f(1000));
}
