#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>

#define LINE_SIZE 1000

struct smem {
  int id;
  char buf[LINE_SIZE];
};

void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

int
main(int argc, char * argv[])
{
  if (argc != 2) {
    fprintf(stderr,"usage: tsmem id#\n");
    return 0;
  }
  
  int id = strtol(argv[1],NULL,10);
  exitIfError(id <= 0,"id must be > 0");
  
  fprintf(stdout,"ID %d\n",id);
  
  int fd = open("SharedMem",O_RDWR);
  exitIfError(fd < 0,"file SharedMem must exist");
  
  struct stat stats;
  int ret = stat("SharedMem",&stats);
  
  fprintf(stderr,"file size %lld\n",stats.st_size);
  
  struct smem * base = mmap(NULL, stats.st_size,PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
  
  fprintf(stderr,"base %p\n",base);
  
  base->id = 0;
  //  fprintf(stderr,"%64s\n",base);
  
  close(fd);
  
  // base->id is
  // -1 when memory being changed
  // 0 when available
  // > 0 when message for id is present
  
  // There is no assurance that two instances of this program
  // will not try to modify memory at the same time. Using 0 and -1 values
  // for base->id attempts to regulate this but the program may be interrupted
  // in between testing for a condition and starting to modify memory. A real
  // solution will involve semaphores or mutex locks allocated within the
  // shared memory space.
  
  char line[LINE_SIZE];
  int dstId;
  int haveLine = 0;
  
  fprintf(stderr,"Enter line of form \"id# message\"\n");

  while (1) {
    if (base->id > 0 && base->id == id) {
      int msgId = base->id;
      base->id = -1;
      fprintf(stdout,"msg: %d %s",msgId,base->buf);
      base->id = 0;
    }
    if (haveLine) {
      while (base->id != 0) { usleep(100); }
      base->id = -1;
      strcpy(base->buf,line);
      haveLine = 0;
      base->id = dstId;
    }
    if (!haveLine) {
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO,&fds);
      struct timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = 100;
      int ret = select(STDIN_FILENO+1,&fds,NULL,NULL,&timeout);
      if (FD_ISSET(STDIN_FILENO,&fds)) {
        dstId = 0;
        fscanf(stdin,"%d",&dstId);
        if (dstId <= 0) break;
        char * p = fgets(line,LINE_SIZE,stdin);
        if (p == NULL) break;
        haveLine = 1;
      }
    }
  }
  
  munmap(base,stats.st_size);
  
}
