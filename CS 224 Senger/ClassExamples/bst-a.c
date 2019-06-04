#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "DA.h"

// assuming the root node is at index 1, then 2*index and 2*index+1
// give the indexes of the left and right children

#define LCHILD(indx) ((indx)*2)
#define RCHILD(indx) ((indx)*2+1)

struct node {
  char * name;
};

void
insert(DA * tree, int indx, char * buf) {
  const struct node * node = get(tree,indx);
  if (node->name == NULL) {
    struct node new;
    new.name = strdup(buf);
    put(tree,indx,&new);
  }
  else {
    int res = strcmp(buf,node->name);
    if (res == 0) printf("duplicate %s\n",buf);
    if (res < 0) insert(tree,LCHILD(indx),buf);
    if (res > 0) insert(tree,RCHILD(indx),buf);
  }
  return;
}

struct walkData {
  int nodeCnt;
  int depth;
};

struct walkData
walk(DA * tree, int indx) {
  const struct node * node = get(tree,indx);
  if (node->name == NULL) {
    struct walkData wd = {0,0};
    return wd;
  }
  struct walkData lwd = walk(tree,LCHILD(indx));
  printf("%s\n",node->name);
  struct walkData rwd = walk(tree,RCHILD(indx));
  struct walkData wd;
  wd.nodeCnt = lwd.nodeCnt + rwd.nodeCnt + 1;
  wd.depth = (lwd.depth < rwd.depth) ? (rwd.depth) : (lwd.depth) + 1;
  return wd;
}

int
main(int argc, char * argv[]) {
  
  struct node nullNode = {NULL};
  
  DA * tree = makeDA(sizeof(struct node),&nullNode);
  
  char buf[64];
  while (scanf("%63s",buf) == 1) {
    insert(tree,1,buf);
  }
  
  struct walkData wd = walk(tree,1);

  fprintf(stderr,"walk cnt %d depth %d opt %f full %d\n",wd.nodeCnt,wd.depth,log2(wd.nodeCnt),1 << wd.depth);
}
