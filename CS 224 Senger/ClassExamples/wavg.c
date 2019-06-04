/*
  Count the number of words (continguous sequences of alphabetic characters) 
  and the average word length.
*/

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

// counts words at leading alphabetic char
int
main(int argc, char * argv[])
{
  int wcnt = 0;
  bool isFirst = true;
  int totalAlpha = 0;
  
  int c;
  while ( (c = getchar()) != EOF) {
    if (isalpha(c) && isFirst) {
      wcnt++;
      isFirst = false;
    }
    else if (!isalpha(c)) {
      isFirst = true;
    }
    if (isalpha(c)) totalAlpha++;
  }
  printf("wcnt = %d avg len = %f\n",wcnt, totalAlpha / (float) wcnt);
}

/*
// counts words at trailing alphabetic char
int
main(int argc, const char * argv[])
{
  int wlen = 0;     // word length
  int wcnt = 0;     // word count
  int alphaCnt = 0; // count of alphabetic characters
  
  int c;
  while ( (c = getchar()) != EOF) {
    if (isalpha(c)) {
      wlen++;
      alphaCnt++;
    }
    else if (wlen > 0) {
        wcnt++;
        wlen = 0;
    }
  }
  if (wlen > 0) wcnt++;
  printf("num words: %d  avg length: %f\n",wcnt, alphaCnt / (float) wcnt);
  return 0;
}
*/
