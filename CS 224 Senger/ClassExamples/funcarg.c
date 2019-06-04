#include <stdio.h>

int
f(char  * str) {
  printf("f called with \"%s\"\n",str);
  return 0;
}

void
g(int (*func)(char *)) {
  (*func)("Howdy");    // invoke argument function func on 'b'
  func("Howdy again");
}

int
main(int argc, char * argv[]) {
  int (*x)(char *);    // declares * x to be a function taking a char argument and returning an int
  x = f;               // assigns f to x
  (*x)("That's All");  // invokes x passing 'a' as the argument
  x("Not exactly all");
  
  g(f);                // pass f to function g
}
