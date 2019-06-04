#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_WORD_LEN 32

struct growBuf {
  int maxIndex;
  int nextIndex;
  int chunkSize;
  char * * data;
};

int
cmp(const void *a, const void *b) {
  const char **ap = (const char **) a;
  const char **bp = (const char **) b;
  return strcmp(*ap,*bp);
}

void
append(struct growBuf * gBuf, char * wordBuf) {
  //printf("word %s\n",wordBuf);
  if (gBuf->nextIndex >= gBuf->maxIndex) {
    gBuf->maxIndex += gBuf->chunkSize;
    gBuf->data = realloc(gBuf->data,gBuf->maxIndex*sizeof(char *));
  }
  gBuf->data[gBuf->nextIndex++] = strdup(wordBuf);
}

int
main(int argc, char * argv[]) {
  
  int chunkSize = 0;
  
  if (argc == 2) {
    chunkSize = strtol(argv[1],NULL,10);
  }
  
  if (chunkSize <= 0) {
    chunkSize = 128;
  }
  
  struct growBuf * gBuf = malloc(sizeof(struct growBuf));
  gBuf->maxIndex = chunkSize;
  gBuf->nextIndex = 0;
  gBuf->chunkSize = chunkSize;
  gBuf->data = malloc(gBuf->maxIndex*sizeof(char *));
  
  int wlen = 0;
  char wbuf[MAX_WORD_LEN+1];
  int c;
  
  do {
    c = getchar();
    if (isalpha(c)) {
      if (wlen < MAX_WORD_LEN) wbuf[wlen++] = c;
    }
    else {
      if (wlen > 0) {
        wbuf[wlen] = '\0';
        append(gBuf,wbuf);
        wlen = 0;
      }
    }
  } while (c != EOF);

  qsort(gBuf->data,gBuf->nextIndex,sizeof(char *),cmp);
  
  for (int i = 0; i < gBuf->nextIndex; i++) {
    printf("%s\n",gBuf->data[i]);
  }
  
}
