#include <stdio.h>
#include <stdarg.h>

void
func(char * fmt, ...) {
  va_list ap;
  char * s;
  int d;
  
  va_start(ap,fmt);
  while (*fmt) {
    switch(*fmt++) {
     case 's':
        s = va_arg(ap,char *);
        printf("string %s\n",s);
        break;
     case 'd':
        d = va_arg(ap,int);
        printf("int %d\n",d);
        break;
    }
  }
  va_end(ap);
}

int
main(int argc, char * argv[]) {
  int x = 5;
  func("ssdsd","one","two",3,"four",x);
}
