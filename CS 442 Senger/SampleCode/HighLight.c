/*
  HighLight.c
  Takes a single command line argument string and reads lines from
  stdin looking for occurences of the string. Lines containing the
  string are written to stdout proceeded by an over-bar line
  highlighting the occurrence.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 2048

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr,"usage: highlight search-string\n");
    return -1;
  }

  char * target = argv[1];

  // use stdin and stdout by default
  FILE * inFD = stdin;
  FILE * outFD = stdout;

  // alloc buffer for input line
  char * inputBuf = malloc(MAX_LINE);

  // alloc buffer for over bar line
  char * overBuf = malloc(MAX_LINE);

  int lineNum = 0;

  // read a line of input
  while (fgets(inputBuf,MAX_LINE,inFD)) {
    lineNum++;

    // init over bar buffer
    memset(overBuf,' ',MAX_LINE);

    // set string search pointer to start of input buffer
    // as an alternative could access inputBuf using array index
    char * p = inputBuf;
    // deliberately cause memory access violation to demonstrate debugger
    //char * p;

    // init last occurence index
    int last = 0;

    // loop while target string occurences are found
    // strstr returns a pointer to next occurrence of target
    while ((p = strstr(p,target))) {

      // fill in over bar line
      int strt = p - inputBuf;
      for (int i = 0; i < strlen(target); i++) {
        overBuf[strt+i] = '_';
      }

      // advance search position by the length of the target
      p += strlen(target);
      last = p - inputBuf;
    }

    // if a last occurrence
    if (last) {

      // terminate over bar string
      overBuf[last] = '\0';

      // print over bar and input line
      fprintf(outFD,"      %s\n",overBuf);
      fprintf(outFD,"%5d %s",lineNum,inputBuf);
    }
  }

  free(inputBuf);
  free(overBuf);

  fflush(outFD);
  fclose(outFD);

  return 0;
}
