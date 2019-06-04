/*

 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 32

// Scramble letters of buf except for first and last using the
// Fischer/Yates/Durstenfeld/Knuth algorithm.
void
scramble(char * buf)
{ char * p = buf+1;
  int len = strlen(buf) - 2;
  
  for (int i = 0; i < len - 1; i++) {
    int j = arc4random_uniform(len-i);
    int t = p[i];
    p[i] = p[i+j];
    p[i+j] = t;
  }
}

int
main(int argc, const char * argv[])
{
  int wlen = 0;
  char wbuf[MAX_WORD_LEN+1];
  
  while (1) {
    int c = getchar();
    if (isalpha(c)) {
      if (wlen < MAX_WORD_LEN) wbuf[wlen++] = c;
    }
    else {
      if (wlen > 0) {
        wbuf[wlen] = '\0';
        scramble(wbuf);
        fprintf(stdout,"%s",wbuf);
        wlen = 0;
      }
      putchar(c);
    }
    if (c == EOF) break;
  }

  return 0;
}
