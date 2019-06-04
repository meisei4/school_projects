#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int
main(int argc, char * argv[]) {
  
  if (argc != 3) {
    fprintf(stderr,"usage: fmerge file-1 file-2\n");
    exit(1);
  }
  
  FILE * fd1 = fopen(argv[1],"r");
  FILE * fd2 = fopen(argv[2],"r");
  
  if (!fd1) {
    fprintf(stderr,"open \"%s\" failed\n",argv[1]);
    exit(1);
  }
  
  if (!fd2) {
    fprintf(stderr,"open \"%s\" failed\n",argv[2]);
    exit(1);
  }
  
  char str1[32];
  char str2[32];
  
  //fscanf can return 1 if read string, 0 if did not read string -1 if at EOF
  
  bool have1 = fscanf(fd1,"%31s",str1) == 1;
  bool have2 = fscanf(fd2,"%31s",str2) == 1;
  
  while (have1 || have2) {
    if ( (have1 && have2 && strcmp(str1,str2) <= 0) || (have1 && !have2)) {
      printf("%s\n",str1);
      have1 = fscanf(fd1,"%31s",str1) == 1;
    }
    else {
      printf("%s\n",str2);
      have2 = fscanf(fd2,"%31s",str2) == 1;
    }
  }
}
