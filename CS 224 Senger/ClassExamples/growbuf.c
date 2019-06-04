#include <stdio.h>
#include <stdlib.h>

int bufChunkSize = 0;

char * buf;
int bufMax = 0;
int bufNext = 0;

void
append(char c) {
  if (bufNext >= bufMax) {
    bufMax += bufChunkSize;
    printf("growing to %d\n",bufMax);
    char * tmp = realloc(buf,bufMax); // should check result
    if (tmp != buf) printf("addr new %p old %p\n",tmp,buf);
    buf = tmp;
  }
  buf[bufNext++] = c;
}

int
main(int argc, char * argv[]) {

  if (argc == 2) {
    bufChunkSize = strtol(argv[1],NULL,10);
  }

  if (bufChunkSize <= 0) {
    bufChunkSize = 128;
  }
  
  bufMax = bufChunkSize;
  buf = malloc(bufMax); // should check result
  
  char c;
  while ((c = getchar()) != EOF) {
    append(c);
  }
  
  for (int i = 0; i < bufNext; i++) {
    putchar(buf[i]);
  }
}
