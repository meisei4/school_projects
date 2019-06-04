#include <stdio.h>
#include <stdlib.h>

#include "ec.h"

int
main(int argc, char * argv[]) {

 	EC * ec = makeEC(16);
	equate(ec,0,8);
  equate(ec,2,4);
  equate(ec,4,8);
  equate(ec,6,10);
  equate(ec,7,11);
  equate(ec,11,12);
  equate(ec,6,7);
  equate(ec,7,9);
  equate(ec,9,15);
  equate(ec,1,5);
  equate(ec,3,13);
  equate(ec,13,14);
  equate(ec,5,14);
  equate(ec,0,8);
   	equate(ec,2,4);
  	equate(ec,4,8);
  	equate(ec,6,10);
  	equate(ec,7,11);
  	equate(ec,11,12);
  	equate(ec,10,12);
  	equate(ec,12,9);
  	equate(ec,9,15);
  	equate(ec,1,5);
  	equate(ec,3,13);
  	equate(ec,13,14);
  	equate(ec,5,14);
		
  	int cnt = classCount(ec);
  	printf("#classes = %d\n",cnt);
  	int * reps = classReps(ec);
  	for (int i = 0; reps[i] >= 0; i++) {
    	int * class = classOf(ec,reps[i]);
    	printf("class of %2d: ", reps[i]);
    	for (int j = 0; class[j] >= 0; j++) {
      		printf("%d ",class[j]);
    	}
    	printf("\n");
    	free(class);
  	}		
	free(reps);
	printf("---\n");
	displayData(ec);
}
