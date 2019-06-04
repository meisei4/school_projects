#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int waiting = 1;

void
handleSignal(int sig) {
  printf("handleSignal %d\n",sig);
  exit(1);
}

void
handleSegv(int sig) {
  printf("handle SegV\n");
  exit(1);
}

void
handleAlarm(int sig) {
  printf("handle alarm\n");
  waiting = 0;
}

int
main(int argc, char * argv[]) {
  
  printf("set signals\n");
  signal(SIGINT,handleSignal);
  signal(SIGSEGV,handleSegv);
  signal(SIGALRM, handleAlarm);
  
  printf("start alarm wait\n");
  alarm(3);
  while (waiting);
  
  printf("force seg fault\n");
  char * s = NULL;
  s[0] = 'a';
  
  printf("loop forever\n");
  while (1);
  
}
