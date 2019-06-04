#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void
exitIfError(int condition, char * msg) {
  if (condition) {
    perror(msg);
    exit(1);
  }
  return;
}

void *
threadWork(void * arg) {
  int cnt = 20;
  while (cnt > 0) {
    fprintf(stdout,"thread %d looping\n",(int)arg);
    sleep((int)arg);
    cnt--;
  }
  printf("thread quitting\n");
  return NULL;
}

int
main(int argc, char * argv[]) {
  pthread_t t1;
  int ret1 = pthread_create(&t1,NULL,threadWork,(void *)1);
  exitIfError(ret1,"pthread_create failed\n");
  
  pthread_t t2;
  int ret2 = pthread_create(&t2,NULL,threadWork,(void *)2);
  exitIfError(ret2,"pthread_create failed\n");
  
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
}






