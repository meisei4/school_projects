#include <stdio.h>
#include <stdlib.h>

#define REC_SIZE 32
#define CMD_SIZE 64

char rec[REC_SIZE];

int
main(int argc, char * argv[]) {
  FILE * file = fopen("tseek-data","r+");
  
//  fseek(file,0 * REC_SIZE,SEEK_SET);
//  fputs("0 some text.    ",file);
//  fseek(file,2 * REC_SIZE,SEEK_SET);
//  fputs("2 final line.   ",file);
//  fseek(file,1 * REC_SIZE,SEEK_SET);
//  fputs("1 another line. ",file);
  
  int running = 1;
  
  while (running) {
    char cmd[CMD_SIZE];
    fprintf(stdout,"> ");
    fgets(cmd,CMD_SIZE,stdin);
    switch(cmd[0]) {
      case 'd': {
        int index;
        int cnt = sscanf(cmd,"d %d",&index);
        if (cnt != 1) break;
        fseek(file,index * REC_SIZE,SEEK_SET);
        fread(rec,REC_SIZE,1,file);
        fwrite(rec,REC_SIZE,1,stdout);
        break; }
      case 'w': {
        int index;
        int cnt = sscanf(cmd,"w %d",&index);
        if (cnt != 1) break;
        fprintf(stdout,"|------------------------------|\n");
        fread(rec,REC_SIZE,1,stdin);
        fseek(file,index * REC_SIZE,SEEK_SET);
        fwrite(rec,REC_SIZE,1,file);
        break; }
      case 'q':
        running = 0;
        break;
    }
  }
  
  fclose(file);
  
}
