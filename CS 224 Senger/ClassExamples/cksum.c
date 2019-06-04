#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char * argv[]) {
  unsigned long sum = 0;
  unsigned long cnt = 0;
  char c;
  while ((c = fgetc(stdin)) > 0) {
    sum += c;
    cnt++;
  }

  fprintf(stderr,"file size %lu\n",cnt);
  fprintf(stderr,"checksum %lu\n",sum);
}
