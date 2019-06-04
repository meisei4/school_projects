/*
  Read input and construct histogram of word lengths.
 
 */

#include <stdio.h>
#include <ctype.h>

#define MAX_LEN 16

int
main(int argc, const char * argv[])
{
  int wlen = 0;     // word length
  int wcnt = 0;     // word count
  int alphaCnt = 0; // count of alphabetic characters

  int hist[MAX_LEN];
  for (int i = 0; i < MAX_LEN; i++) hist[i] = 0;
  
  while (1) {
    int c = getchar();
    if (isalpha(c)) {
      wlen++;
      alphaCnt++;
    }
    else {
      if (wlen > 0) {
        if (wlen < MAX_LEN) hist[wlen]++;
        wlen = 0;
      }
    }
    if (c == EOF) break;
  }
  
  
  fprintf(stdout,"bin -- cnt\n");
  int sum = 0;
  for (int i = 0; i < MAX_LEN; i++) {
    fprintf(stdout,"%3d -- %d\n",i,hist[i]);
    sum += hist[i];
  }
  fprintf(stdout,"word cnt: %d\n",sum);
  
  return 0;
}
