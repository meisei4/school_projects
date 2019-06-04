/*
   Test whether command line argument is a palindrome.
*/

#include <stdio.h>
#include <string.h>

int
main(int argc, char * argv[])
{
  if (argc != 2) {
    fprintf(stderr,"usage: pal string\n");
    return 1;
  }
  
  char * str = argv[1];
  int len = strlen(str);
  
  for (int i = 0; i < len/2; i++) {
    if (str[i] != str[len-i-1]) {
      fprintf(stdout,"no\n");
      return 1;
    }
  }
  fprintf(stdout,"yes\n");
  return 0;
}

