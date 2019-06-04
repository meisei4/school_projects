#include <stdio.h>
#include <ctype.h>

#define MAX_STK_SIZE 256

char stk[MAX_STK_SIZE];
int tos = 0; // next avail place to put char

void
push(int c)
{
  if (tos >= MAX_STK_SIZE) return;
  stk[tos++] = c;
}

void
putStk()
{
  while (tos >= 0) {
    putchar(stk[--tos]);
  }
}

int
main(int argc, char * argv[])
{
  int c;
  do {
    c = getchar();
    if (isalpha(c)) {
      push(c);
    }
    else {
      putStk();
      putchar(c);
    }
  } while (c != EOF);
  
}

