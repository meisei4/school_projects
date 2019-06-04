#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>

#include "hashtbl.h"

struct entryNode {
  char * key;
  void * value;
  struct entryNode * next;
};

struct hashtbl {
  unsigned int (*hashfn)(const char *, unsigned int);
  unsigned int size;
  struct entryNode **tbl;
};

hashtbl *
createTable(unsigned int size, hashfn hashFunc) {
  if (size <= 0) return NULL;
  struct hashtbl * tbl = malloc(sizeof(struct hashtbl));
  tbl->size = size;
  tbl->hashfn = hashFunc;
  tbl->tbl = calloc(tbl->size,sizeof(struct entryNode *));
  return tbl;
}

void
freeTable(hashtbl * tbl) {
  for (int i = 0; i < tbl->size; i++) {
    while (tbl->tbl[i]) {
      struct entryNode * node = tbl->tbl[i];
      tbl->tbl[i] = node->next;
      free(node->key);
      free(node);
    }
  }
  free(tbl->tbl);
  free(tbl);
}

void *
setValueForKey(hashtbl * tbl, char * key, void * value) {
  unsigned int hashval = tbl->hashfn(key,tbl->size);
  struct entryNode * node = tbl->tbl[hashval];
  while (node) {
    if (strcmp(node->key,key) == 0) {
      void * tmp = node->value;
      node->value = value;
      return tmp;
    }
    node = node->next;
  }
  node = malloc(sizeof(struct entryNode));
  node->key = key;
  node->value = value;
  node->next = tbl->tbl[hashval];
  tbl->tbl[hashval] = node;
  return NULL;
}

void *
valueForKey(hashtbl * tbl, char * key, bool * present) {
  unsigned int hashval = tbl->hashfn(key,tbl->size);
  struct entryNode * node = tbl->tbl[hashval];
  while (node) {
    if (strcmp(node->key,key) == 0) {
      if (present) *present = true;
      return node->value;
    }
    node = node->next;
  }
  if (present) *present = false;
  return NULL;
}

void *
removeKey(hashtbl * tbl, char * key) {
  unsigned int hashval = tbl->hashfn(key,tbl->size);
  struct entryNode * node = tbl->tbl[hashval];
  struct entryNode ** referrer = &(tbl->tbl[hashval]);
  while (node) {
    if (strcmp(node->key,key) == 0) {
      *referrer = node->next;
      void * tmp = node->value;
      free(node);
      return tmp;
    };
    node = node->next;
  }
  return NULL;
}

void
performOnEntries(hashtbl * tbl, workfn workFunc, void * workData) {
  for (int i = 0; i < tbl->size; i++) {
    struct entryNode * node = tbl->tbl[i];
    while (node) {
      workFunc(node->key,node->value,workData);
      node = node->next;
    }
  }
}

char *
strf(char * buf, char * format, ...) {
  va_list ap;
  
  va_start(ap,format);
  int size = vsnprintf(NULL,0,format,ap);
  va_end(ap);
  
  if (!buf) buf = malloc(size+1);
  va_start(ap,format);
  vsprintf(buf,format,ap);
  va_end(ap);
  
  return buf;
}

void
displayState(FILE * fd, hashtbl * tbl) {
  fprintf(fd,"Table size: %d\n",tbl->size);
  unsigned int cnt = 0;
  unsigned int minlen = INT_MAX;
  unsigned int maxlen = 0;
  for (int i = 0; i < tbl->size; i++) {
    unsigned int len = 0;
    struct entryNode * node = tbl->tbl[i];
    while (node) {
      len++;
      node = node->next;
    }
    fprintf(fd,"%d ",len);
    if (len < minlen) minlen = len;
    if (len > maxlen) maxlen = len;
    cnt += len;
  }
  fprintf(fd,"\ncnt: %d minlen: %d maxlen: %d\n",cnt,minlen,maxlen);
}

void
displayWith(FILE * fd, hashtbl * tbl, displayfn displayFunc) {
  fprintf(fd,"Table Contents\n");
  int cnt = 0;
  for (int i = 0; i < tbl->size; i++) {
    struct entryNode * node = tbl->tbl[i];
    while (node) {
      fprintf(fd,"%d ",cnt);
      displayFunc(fd,node->key,node->value);
      cnt++;
      node = node->next;
    }
  }
}

