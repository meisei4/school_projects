#include <stdio.h>


void
merge(char * a, char * b, char * c)
{
  while (*a && *b) *c++ = (*a <= *b) ? (*a++) : (*b++);
  while (*a) *c++ = *a++;
  while (*b) *c++ = *b++;
  *c++ = '\0';
}

int
main(int argc, char * argv[])
{
  char m[256];
  
  merge("afjlpsx","bcmnoquz",m);
  printf("m: \"%s\"\n",m);
}
