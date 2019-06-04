#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}


// Child code --------------------
int childLoopCnt = 0;

void
handleUSR1(int sig) {
  pid_t childPid = getpid();
  fprintf(stderr,"          <%d> USR cnt = %d\n",childPid,childLoopCnt);
}

void
handleINT(int sig) {
  pid_t childPid = getpid();
  fprintf(stderr,"          <%d> SIGINT terminating\n",childPid);
  exit(1);
}

void
runChild(int secs) {
  pid_t childPid = getpid();
  
  signal(SIGUSR1,handleUSR1);
  signal(SIGINT,handleINT);
  
  while (1) {
    fprintf(stderr,"          <%d>  cnt = %d\n",childPid,childLoopCnt);
    sleep(secs);
    childLoopCnt++;
  }
  fprintf(stderr,"          <%d> exiting\n",childPid);
}

// -------------------------------

#define LINE_SIZE 64

int
main(int argc, char * argv[]) {
  signal(SIGCHLD,SIG_IGN);
  
  while (1) {
    
    char lineBuf[LINE_SIZE];
    fprintf(stderr,"> "); fflush(stderr);
    fgets(lineBuf,LINE_SIZE,stdin);
    
    switch (lineBuf[0]) {
      case 'c': { // create new child - "c interval-secs"
        int secs;
        int ret = sscanf(lineBuf,"c %d",&secs);
        if (ret != 1) {
          fprintf(stderr,"malformed create command\n");
          break;
        }
        if (secs < 1) secs = 1;
        if (secs > 10) secs = 10;
        
        pid_t pid = fork();
        exitIfError(pid < 0,"Error on fork");
        
        if (pid == 0) {
          runChild(secs);
          exit(0);
        }
        else {
          // in original process
          fprintf(stderr,"child forked %d\n",pid);
        }
      } break;
      case 't': { // terminate child - "t pid"
        int pid;
        int ret = sscanf(lineBuf,"t %d",&pid);
        if (ret != 1) {
          fprintf(stderr,"malformed terminate command\n");
          break;
        }
        kill(pid,SIGINT);
      } break;
      case 'i': { // get child info - "i pid"
        int pid;
        int ret = sscanf(lineBuf,"i %d",&pid);
        if (ret != 1) {
          fprintf(stderr,"malformed info command\n");
          break;
        }
        kill(pid,SIGUSR1);
      } break;
    }
    
  }
}
