#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct node {
  char * name;
  struct node * lChild;
  struct node * rChild;
};

// V1 passes the variable holding the pointer to the tree node to insert which
// means that insert can assign values to this variable
//
// V2 pases the pointer to the tree node to insert which means that insert
// cannot change the vaiable holding this value and instead relies upon the
// to do the assignment from the return value to effect this change

#define V2

#ifdef V1
void
insert(struct node * * tree, char * buf) {
  if (*tree == NULL) {
    struct node * new = malloc(sizeof(struct node));
    new->name = strdup(buf);
    new->lChild = new->rChild = NULL;
    *tree = new;
  }
  else {
    int res = strcmp(buf,(*tree)->name);
    if (res == 0) printf("duplicate\n");
    if (res < 0) insert(&((*tree)->lChild),buf);
    if (res > 0) insert(&((*tree)->rChild),buf);
  }
  return;
}
#endif

#ifdef V2
struct node *
insert(struct node * tree, char * buf) {
  if (tree == NULL) {
    struct node * new = malloc(sizeof(struct node));
    new->name = strdup(buf);
    new->lChild = new->rChild = NULL;
    tree = new;
  }
  else {
    int res = strcmp(buf,tree->name);
    if (res == 0) printf("duplicate\n");
    if (res < 0) tree->lChild = insert(tree->lChild,buf);
    if (res > 0) tree->rChild = insert(tree->rChild,buf);
  }
  return tree;
}
#endif

struct walkData {
  int nodeCnt;
  int depth;
};

struct walkData
walk(struct node * tree) {
  if (!tree) {
    struct walkData wd = {0,0};
    return wd;
  }
  struct walkData lwd = walk(tree->lChild);
  printf("%s\n",tree->name);
  struct walkData rwd = walk(tree->rChild);
  struct walkData wd;
  wd.nodeCnt = lwd.nodeCnt + rwd.nodeCnt + 1;
  wd.depth = (lwd.depth < rwd.depth) ? (rwd.depth) : (lwd.depth) + 1;
  return wd;
}

int
main(int argc, char * argv[]) {
  
  struct node * tree = NULL;
  
  char buf[64];
  while (scanf("%63s",buf) == 1) {
#ifdef V1
    insert(&tree,buf);
#endif
#ifdef V2
    tree = insert(tree,buf);
#endif
  }
  
  struct walkData wd = walk(tree);
  
  fprintf(stderr,"walk cnt %d depth %d opt %f full %d\n",wd.nodeCnt,wd.depth,log2(wd.nodeCnt),1 << wd.depth);
}
