#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define JSMN_PARENT_LINKS
#include "jsmn/jsmn.h"
#include "jsmnutil.h"


#define CHUNK_SIZE 16

void
exitIfError(int cond, char * msg) {
  if (!cond) return;
  fprintf(stderr,"%s\n",msg);
  exit(1);
}

int
main(int argc, char * argv[]) {
  // process command line arguments
  int verbose = 0;
  int tokenSize = 200;
  opterr = 0;
  int c;
  while ((c = getopt(argc, argv, "vt:")) != -1) {
    switch (c) {
      case 'v':
        verbose = 1;
        break;
      case 't':
        tokenSize = strtol(optarg,NULL,10);
        break;
      case '?':
        if (optopt == 'o')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        abort ();
    }
  }

  // form jsondata struct and read json string from stdin
  jsmndata_t jsmndata;
  jsmndata.json = malloc(CHUNK_SIZE);
  jsmndata.jsonSize = 0;
  jsmndata.jsonAllocatedSize = CHUNK_SIZE;
  
  while ((c = getchar()) != EOF) {
    // make sure we always have one more space than necessary to accomodate NUL
    if (jsmndata.jsonSize >= jsmndata.jsonAllocatedSize-1) {
      jsmndata.jsonAllocatedSize += CHUNK_SIZE;
      jsmndata.json = realloc(jsmndata.json,jsmndata.jsonAllocatedSize * sizeof(char));
    }
    jsmndata.json[jsmndata.jsonSize++] = c;
  }
  jsmndata.json[jsmndata.jsonSize++] = '\0';
  
  if (verbose) fprintf(stderr, "%s\n", jsmndata.json);
  
  // parse json data into token array
  jsmn_parser p;
  jsmn_init(&p);
  jsmndata.tokensAllocatedSize = tokenSize;
  jsmndata.tokens = malloc(jsmndata.tokensAllocatedSize * sizeof(jsmntok_t));
  int retCnt = jsmn_parse(&p, jsmndata.json, jsmndata.jsonSize, jsmndata.tokens, jsmndata.tokensAllocatedSize);
  exitIfError(retCnt == JSMN_ERROR_NOMEM,"jsmn_parse error - not enough token space");
  exitIfError(retCnt == JSMN_ERROR_INVAL,"jsmn_parse error - invalid character");
  exitIfError(retCnt == JSMN_ERROR_PART,"jsmn_parse error - not complete json string");
  jsmndata.tokensSize = retCnt;
  
  if (verbose) {
    fprintf(stderr,"index       type  strt   end  size parnt\n");
    for (int i = 0; i < retCnt; i++) {
      fprintf(stderr,"%5d %10s %5d %5d %5d %5d\n",i,jsmnTypes[jsmndata.tokens[i].type],jsmndata.tokens[i].start,jsmndata.tokens[i].end,jsmndata.tokens[i].size,jsmndata.tokens[i].parent);
    }
  }
  
  // display data from parsed tokens
  jsmnmarker_t root = makeMarker(&jsmndata);
  jsmnmarker_t faculty = objectDeref(root,"faculty");
  //get the start of the  all the faculty
  exitIfError(isErrorMarker(faculty) || TOKEN_TYPE(faculty) != JSMN_ARRAY,"json does not contain \"faculty\" array key");
  
  fprintf(stdout,"Faculty Schedules\n");
  for (int i = 0; i < TOKEN_SIZE(faculty); i++) {
    char * firstName = tokenText(derefBy(faculty,"ds",i,"firstName"));
    char * lastName = tokenText(derefBy(faculty,"ds",i,"lastName"));
    fprintf(stdout,"Prof. %s %s \n",firstName,lastName);
    free(firstName);
    free(lastName);
    jsmnmarker_t classes = derefBy(faculty,"ds",i,"classes");
    exitIfError(isErrorMarker(classes) || TOKEN_TYPE(classes) != JSMN_ARRAY,"faculty entry does not contain \"classes\" key");
    for (int j = 0; j < TOKEN_SIZE(classes); j++) {
      char * course = tokenText(arrayDeref(classes,j));
      char * location = tokenText(derefBy(root,"sss","courses", course, "location"));
      char * time = tokenText(derefBy(root,"sss","courses",course,"time"));
      if (location != NULL && time != NULL) {
        fprintf(stdout,"     %-10s @ %s in %s\n",course,time,location);
      }
      else {
        fprintf(stdout,"     %-10s @ unknown location and time\n",course);
      }
      free(course);
      free(location);
      free(time);
    }
    fprintf(stdout,"\n");
  }
  
  // display all of the keys of courses
  jsmnmarker_t courses = objectDeref(root,"courses");
  jsmnmarker_t key = courses;
  fprintf(stdout,"Course List\n");
  for (int i = 0; i < TOKEN_SIZE(courses); i++) {
    key = nextKeyForObject(courses,key);
    printToken(stdout,key);
    char * location = tokenText(derefBy(valueForKey(key),"s","location"));
    char * time = tokenText(derefBy(valueForKey(key),"s","time"));
    if (location != NULL && time != NULL) {
      fprintf(stdout,"\t @ %s in %s\n",time,location);
    }
    else {
      fprintf(stdout,"\t @ unknown location and time\n");
    }
    free(location);
    free(time);
  }
  fprintf(stdout,"\n");
  
}


