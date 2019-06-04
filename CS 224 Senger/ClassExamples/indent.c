/*
  Echo input to output replacing initial dash characters on a line with 
  tab characters. 
*/

#include <stdio.h>
#include <stdbool.h>

int
main(int argc, char * argv[])
{
  bool lineStart = true;
  
  int c;
  while ((c = getchar()) != EOF) {
    if (lineStart && c == '-') {
      putchar('\t');
    }
    else {
      putchar(c);
      lineStart = (c == '\n');
    }
  }
}