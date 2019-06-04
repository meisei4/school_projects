#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define JSMN_PARENT_LINKS
#include "jsmn/jsmn.h"

#define CHUNK_SIZE 16

typedef struct {
  char * json;
  size_t jsonSize;
  size_t jsonAllocatedSize;
  jsmntok_t * tokens;
  unsigned int tokensSize;
  unsigned int tokensAllocatedSize;
} jsmndata_t;

char * jsmnTypes[] = {"undefined","object","array","string","primitive"};

int
main(int argc, char * argv[]) {
  // create data
  jsmndata_t jsmndata;
  jsmndata.json = malloc(CHUNK_SIZE);
  jsmndata.jsonSize = 0;
  jsmndata.jsonAllocatedSize = CHUNK_SIZE;
  
  // read json text from stdin
  int c;
  while ((c = getchar()) != EOF) {
    if (jsmndata.jsonSize >= jsmndata.jsonAllocatedSize) {
      jsmndata.jsonAllocatedSize += CHUNK_SIZE;
      jsmndata.json = realloc(jsmndata.json,jsmndata.jsonAllocatedSize * sizeof(char));
    }
    jsmndata.json[jsmndata.jsonSize++] = c;
  }
  
  // display json text with numbered rows and cols
#define LINE_LENGTH 50
  printf("          01234567890123456789012345678901234567890123456789\n");
  int i = 0;
  int l = 0;
  printf("%5d     ",l);
  while (jsmndata.json[i]) {
    (jsmndata.json[i] == '\n') ? putchar(' ') : putchar(jsmndata.json[i]);
    i++;
    if ((i % LINE_LENGTH) == 0) {
      l++;
      printf("\n%5d     ",l*LINE_LENGTH);
    }
  }
  printf("\n");
  
//  printf("%s\n",json);
  
  // parse json text to token array
  jsmn_parser p;
  jsmn_init(&p);
  jsmndata.tokensAllocatedSize = 500;
  jsmndata.tokens = malloc(jsmndata.tokensAllocatedSize * sizeof(jsmntok_t));
  int r = jsmn_parse(&p, jsmndata.json, jsmndata.jsonSize, jsmndata.tokens, jsmndata.tokensAllocatedSize);
  
  printf("jsmn_parse return %d\n",r);
  
  // display the contents of the token array
  printf("index       type  strt   end  size parnt\n");
  for (int i = 0; i < r; i++) {
    printf("%5d %10s %5d %5d %5d %5d\n",i,jsmnTypes[jsmndata.tokens[i].type],jsmndata.tokens[i].start,jsmndata.tokens[i].end,jsmndata.tokens[i].size,jsmndata.tokens[i].parent);
  }
  
}


