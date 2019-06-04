#include <stdio.h>

char * a = "first string";
char b[] = "second string";

int
main(int argc, char * argv[]) {
  char * c = "third string";
  char d[] = "fourth string";
  
  printf("addrs    %15p %15p %15p %15p\n",&a,&b,&c,&d);
  printf("contents %15p %15p %15p %15p\n",a,b,c,d);
  printf("%s|%s|%s|%s\n",a,b,c,d);

  a = b;
  //  b = a;
  c = d;
  //  d = c;
  
  printf("addrs    %15p %15p %15p %15p\n",&a,&b,&c,&d);
  printf("contents %15p %15p %15p %15p\n",a,b,c,d);
  printf("%s|%s|%s|%s\n",a,b,c,d);
  
}
