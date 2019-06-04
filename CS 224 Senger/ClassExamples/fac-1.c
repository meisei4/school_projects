#include <stdio.h>

int
main(int argc, char * argv[])
{
  for (int n = 1; n <= 10; n++) {
    int fac = 1;
    for (int i = 1; i <= n; i++) {
      fac = fac * i;
    }
    printf("fac(%d) = %d\n",n,fac);
  }
  
}