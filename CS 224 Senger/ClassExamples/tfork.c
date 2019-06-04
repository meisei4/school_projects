#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void
exitIfError(int condition, char * msg)
{
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

void
runChild(int secs, int cnt)
{
  pid_t childPid = getpid();
  
  while (cnt > 0) {
    fprintf(stderr,"<%d>  cnt = %d\n",childPid,cnt);
    sleep(secs);
    cnt--;
  }
  fprintf(stderr,"<%d> exiting\n",childPid);
}

#define LINE_SIZE 64

int
main(int argc, char * argv[])
{
  int secs;
  int cnt;
  while (1) {
    
    char lineBuf[LINE_SIZE];
    fprintf(stderr,"> "); fflush(stderr);
    fgets(lineBuf,LINE_SIZE,stdin);
    int ret = sscanf(lineBuf,"%d %d",&secs,&cnt);
    if (secs < 1 || secs > 10 || cnt < 1 || cnt > 100) continue;
    if (ret != 2) return 0;
    
    pid_t pid = fork();
    exitIfError(pid < 0,"Error on fork");
    
    if (pid == 0) {
      // now in child process
      runChild(secs,cnt);
      exit(0);
      // do not want to continue with main() code in child
      // since the child will then start to call fork()
    }
    else {
      // in original process
      fprintf(stderr,"child forked %d\n",pid);
    }
  }
  
}