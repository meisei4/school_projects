#include <stdio.h>

char * ss = "abcde";
char tt[] = "abcde";

int
main(int argc, char * argv[])
{
  int x;
  int * p;
  int y;
  
label:

  printf("main %p\n",main);
  printf("label %p\n",&&label);
  
  x = 13;
  printf("x: %d\n",x);
  
  p = &x;
  printf("*p: %d\n",*p);
  
  y = *p;
  printf("y: %d\n",y);
  
  y = * & x;
  printf("y: %d\n",y);
  
  printf("&x: %p &p: %p &y %p\n",&x,&p,&y);
  
  char * s = "abcde";
  char t[] = "abcde";
  
  printf("s: %p t: %p\n",s,t);
  
  //  s[0] = 'A'; // produces a bus error because literal string is readonly
  t[0] = 'A';
  //  ss[0] = 'A';  // produces a bus error because literal string is readonly
  tt[0] = 'A';
  
  printf("s: %p  t: %p\n",s,t);
  printf("ss: %p  tt: %p\n",ss,tt);
}
























