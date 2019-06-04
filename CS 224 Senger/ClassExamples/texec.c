#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void
catRand(char * s, int n) {
  for (int i = 0; i < n; i++) {
    *s++ = "abcdefghijklmnopqrstuvwxyz"[random() % 26];
  }
  *s++ = '\0';
}

int
main(int argc, char * argv[], char * envp[])
{
  printf("%s starting as <%d>\n",argv[0],getpid());
  while (*envp) {
    if (strstr(*envp,"texec")) printf("ENV %s\n",*envp);
    srandom((unsigned int) *envp);
    envp++;
  }
  
  if (argc <= 1) {
    printf("<%d> stopping\n",getpid());
    return 0;
  }
  
  printf("about to execve\n");
  
  char * envstr = malloc(16);
  strcpy(envstr,argv[1]);
  strcat(envstr,"=");
  catRand(envstr + strlen(envstr),5);
  printf("new env %s\n",envstr);
  
  char *nenvp[] = {envstr, 0};
  execve(argv[1],&argv[1],nenvp);
  
  printf("this should never occur\n");
}
