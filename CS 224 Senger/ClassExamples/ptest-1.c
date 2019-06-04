#include <stdio.h>
#include <stdbool.h>

int
main(int argc, char * argv[])
{
  for (int n = 2; n <= 100; n++) {
    bool ans = true;
    for (int i = 2; i <= n / 2; i++) {
      if (n % i == 0) {
        ans = false;
        break;
      }
    }
  
    if (ans) {
      printf("%d is prime\n",n);
    }
  }
}
