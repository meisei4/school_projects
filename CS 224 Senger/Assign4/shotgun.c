#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ec.h"
#define MIN(x,y) ( (x < y) ? (x) : (y) )
#define MAX(x,y) ( (x > y) ? (x) : (y) )

void
displayOverlap(char * a, char * b, int bShift) {
  printf("match   ");
  if (bShift < 0) for (int i = 0; i > bShift; i--) printf(" ");
  printf("%s\n",a);
  printf("        ");
  if (bShift > 0) for (int i = 0; i < bShift; i++) printf(" ");
  printf("%s\n",b);
}

bool
overlap(char * a, char * b) {

  int j, i = 0, eq = 0;
  ////////FIRST CASE///////
  //a: CCCTACTAAAATACAAA
  //b:     ACTAAAATACAAAATT
  ////where b moves to the right
  while(a[i] != '\0'){
    j = 0;
    eq = 0;
    while(a[i+j] != '\0'){
      if(a[i+j] == b[j]){
        eq++;
        if(eq > MIN(strlen(a), strlen(b))/2+1){
          return 1;
        }
      }else{
        eq = 0;
      }
      j++;
    } 
    i++;
  }
  ///////SECOND CASE//////
  //a:      AATTAGCCCAGCTTGGTGGT
  //b:TACAAAAATTAGCCCAGCTT
  /////where a moves to the right
  i = 0;
  while(b[i] != '\0'){
    j = 0;
    eq = 0;
    while(b[i+j] != '\0'){
      if(b[i+j] == a[j]){
        eq++;
        if(eq > MIN(strlen(a), strlen(b))/2+1){
          return 1;
        }
      }else{
        eq = 0;
      }
      j++;
    }
    i++;
  }
  return false;
}

int
main(int argc, char * argv[]) {
  
  char * seqs[1024];
  int nextSeq = 0;
  
  char buf[64];
  
  while (scanf("%s",buf) == 1) {
    if (strlen(buf) < 12) continue;
    seqs[nextSeq++] = strdup(buf);
  }
  
  printf("%d seqs\n",nextSeq);
  
  EC * ec = makeEC(nextSeq);
  
  for (int i = 0; i < nextSeq; i++) {
    for (int j = 0; j < nextSeq; j++) {
      if (i == j) continue;
      if (overlap(seqs[i],seqs[j])) {
        equate(ec,i,j);
      }
    }
  }
  
  int cnt = classCount(ec);
  printf("#classes = %d\n",cnt);
  int * reps = classReps(ec);
  for (int i = 0; reps[i] >= 0; i++) {
    int * class = classOf(ec,reps[i]);
    printf("class of %d: ",reps[i]);
    for (int j = 0; class[j] >= 0; j++) {
      printf("%d ",class[j]);
    }
    printf("\n");
  } 
}
