#include <stdio.h>
#include <ctype.h>

int
isVowel(int c)
{
  int uc = toupper(c);
  return (uc == 'A' || uc == 'E' || uc == 'I' || uc == 'O' || uc == 'U' || uc == 'Y');
}

int
isConsonant(int c)
{
  return !isVowel(c);
}

int
main(int argc, char * argv[])
{
  int c;
  while ((c = getchar()) != EOF) {
    if (isConsonant(c)) {
      putchar(c);
    }
    else {
      putchar('_');
    }
  }
}
