#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char * argv[])
{
  for (int i = 1; i < argc; i++) {
    char * val = getenv(argv[i]);
    if (val) printf("%s=%s\n",argv[i],val);
  }
}