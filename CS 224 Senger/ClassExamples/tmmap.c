#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int
main(int argc, char * argv[]) {
  int fd = open("DNA-22",O_RDONLY);

  struct stat stats;
  int ret = stat("DNA-22",&stats);
  
  fprintf(stderr,"file size %lld\n",stats.st_size);
  
  char * base = mmap(NULL,stats.st_size,PROT_READ,MAP_FILE | MAP_PRIVATE,fd,0);
  
  fprintf(stderr,"base %p\n",base);
  
  //  fprintf(stderr,"%64s\n",base);
  
  close(fd);
  
  unsigned long sum = 0;
  char * p = base;
  for (int i = 0; i < stats.st_size; i++) {
    sum += *p;
    p++;
  }
  
  munmap(base,stats.st_size);
  
  fprintf(stderr,"checksum %lu\n",sum);
}
