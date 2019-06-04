#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "SampleAux.h"

void
displayName(struct NameNode * node, void * arg) {
  fprintf((FILE *) arg,"%3d  %3d  \"%s\"\n",node->seq,node->val,node->name);
}

int
main(int argc, char *argv[]) {
  // init the list
  InitList();

  // open the input file
  FILE * fd = fopen("Names.txt","r");
  if (!fd) {
    fprintf(stdout,"Can't open name file.\n");
    exit(1);
  }

  // while not EOF read and add names to list
  char buffer[16];
  int val;
  int minVal = INT_MAX;
  int maxVal = INT_MIN;
  while (fscanf(fd,"%15s %d",buffer,&val) != EOF) {
    AddName(buffer,val);
    if (val < minVal) minVal = val;
    if (val > maxVal) maxVal = val;
  }

  // close the file handle
  fclose(fd);

  // display entries in order of value
  fprintf(stdout,"Value   Name\n");
  for (int i = minVal; i <= maxVal; i++) {
    char * name;
    name = NameWithValue(i);
    if (name) {
      fprintf(stdout,"%5d   \"%s\"\n",i,name);
    }
  }

  // remove head node of list - for no good reason
  RemoveHead();

  // display names in the list in the list order
  fprintf(stdout,"Seq  Val  Name\n");
  DoForAll(displayName,stdout);

  exit(0);
}
