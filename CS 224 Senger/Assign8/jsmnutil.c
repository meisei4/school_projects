#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "jsmnutil.h"

char * jsmnTypes[] = {"undefined","object","array","string","primitive"};

const jsmnmarker_t jsmnErrorMarker = {NULL,-1};

jsmnmarker_t
makeMarker(jsmndata_t * data) {
  jsmnmarker_t marker;
  marker.data = data;
  marker.index = 0;
  return marker;
}

int
areEqualMarkers(jsmnmarker_t marker1, jsmnmarker_t marker2) {
  return marker1.data == marker2.data && marker1.index == marker2.index;
}

int
isErrorMarker(jsmnmarker_t marker) {
  return areEqualMarkers(marker,jsmnErrorMarker);
}

jsmnmarker_t
objectDeref(jsmnmarker_t marker, char * key) { //get the object
  if (marker.index == -1) return marker; //error eror marker returned
  int childCnt = TOKEN_SIZE(marker); //TOKEN_SIZE(m) == (m.data->tokens[m.index].size)
  jsmnmarker_t child = marker; //create a child from the marker
  child.index++;
  //start iterating through the data to find child
  while (childCnt > 0) {
    if (TOKEN_PARENT(child) == marker.index) { //TOKEN_PARENT(m) == (m.data->tokens[m.index].parent)
      if (strncmp(TOKEN_ADDR(child), key, TOKEN_LENGTH(child)) == 0) { //TOKEN_ADDR(m) == (&(m.data->json[m.data->tokens[m.index].start]))
        child.index++;
        return child;
      }
      childCnt--;    //for every child found remove from child Count
    }
    child.index++;
  }
  return jsmnErrorMarker;
}

jsmnmarker_t
arrayDeref(jsmnmarker_t marker, int index) {
  if (marker.index == -1) return marker;
  int childCnt = TOKEN_SIZE(marker);
  if (index >= childCnt) return jsmnErrorMarker;
  jsmnmarker_t child = marker;
  child.index++;
  while (1) {
    if (TOKEN_PARENT(child) == marker.index) {
      if (index == 0) {
        return child;
      }
      index--;
    }
    child.index++;
  }
  return jsmnErrorMarker;
}

jsmnmarker_t
derefBy(jsmnmarker_t marker, char * fmt, ...){
  char * s;
  int d;
  va_list ap;
  va_start(ap, fmt);
  while(*fmt){
    switch(*fmt++){
      case 's':
        s = va_arg(ap, char *);
        marker = objectDeref(marker, s);
        break;
      case 'd':
        d = va_arg(ap, int);
        marker = arrayDeref(marker, d);
        break;
    }
  }
  va_end(ap);
  return marker;
}

jsmnmarker_t
nextKeyForObject(jsmnmarker_t objMarker, jsmnmarker_t keyMarker) {
  do{
    keyMarker.index++;
  }while(TOKEN_PARENT(keyMarker) != objMarker.index);
  return keyMarker;
}

jsmnmarker_t
valueForKey(jsmnmarker_t keyMarker) {
  keyMarker.index++;
  return keyMarker;
}

char * *
keysForObject(jsmnmarker_t marker) {
  if (marker.index == -1) return NULL;
  if (TOKEN_TYPE(marker) != JSMN_OBJECT) return NULL;
  int childSize = TOKEN_SIZE(marker);
  char * * keys = malloc((childSize + 1) * sizeof(char *));
  jsmnmarker_t child = marker;
  child.index++;
  int cnt = 0;
  while (cnt < childSize) {
    if (TOKEN_PARENT(child) == marker.index) {
      keys[cnt] = tokenText(child);
      cnt++;
    }
    child.index++;
  }
  keys[cnt] = NULL;
  return keys;
}

char *
tokenText(jsmnmarker_t marker) {
  if (marker.index < 0) return NULL;
  return strndup(TOKEN_ADDR(marker),TOKEN_LENGTH(marker));
}

void
printToken(FILE * fh, jsmnmarker_t marker) {
  if (marker.index >= 0 ) {
    fprintf(fh,"%.*s",TOKEN_LENGTH(marker),TOKEN_ADDR(marker));
  }
  else {
    fprintf(fh,"unresolved reference");
  }
}

void
dprintToken(int fd, jsmnmarker_t marker) {
  if (marker.index >= 0 ) {
    dprintf(fd,"%.*s",TOKEN_LENGTH(marker),TOKEN_ADDR(marker));
  }
  else {
    dprintf(fd,"unresolved reference");
  }
}

