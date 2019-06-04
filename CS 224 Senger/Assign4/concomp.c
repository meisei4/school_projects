#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ec.h"

void
displayArray(char * img, int width) {
	for (int j = 0; j < width+2; j++) printf("-");
	printf("\n");
	for (int i = 0; i < width; i++) {
		printf("|");
		for (int j = 0; j < width; j++) {
			printf("%c",img[i*width+j]);
		}
		printf("|\n");
	}
	for (int j = 0; j < width+2; j++) printf("-");
	printf("\n");
}

int
main(int argc, char * argv[]) {
	char * ptrn = malloc(40);
  char c;
	int i = 0;
  int currentsize = 40;
  while( (c = getchar()) != EOF){
    if(c != '\n'){
      ptrn[i++] = c;
      if(i > currentsize){
        ptrn = (char *) realloc(ptrn, 40 + currentsize);
        currentsize += 40;
      }
    }
	}
  
	// malloc an array for the pattern data
	// read pattern data one char at a time
	// into the pattern array, realloc as necessary
  ///////////////////////////////////////////////////////////////////////////////
  if((int)sqrt(i) *(int)sqrt(i) != i){
		printf("non square file\n");
		return 0;
	}
	// check that the number of characters read is a
	// square (i.e. sqr of the sqrt is the original size)
	// if not printf an error message and exit


  ///////////////////////////////////////////////////////////////////////////////
	int width = sqrt(i);
	displayArray(ptrn, width);
	struct ecData * ec  = makeEC(width * width);
	// the square root becomes the width/height of the data
	// display the input data
	// make an EC instance of size equal to the width^2


  ////////////////////////////////////////////////////////////////////////////////////


	// double loop through the pattern data (you will need
	// to calcuate array index from row/col)
	// compare elt to its four adjacent neighbors
	// call equate when equal
	int j = 0, k, current;
	while(j < width){
		k = 0;
		while(k < width){
			current = j*width + k;
			//check above
			if(j > 0 &&  ptrn[current] == ptrn[current - width]){
				equate(ec, current, current - width);
			}
			// compare right
			if(k < width-1 && ptrn[current] == ptrn[current+ 1]){
				equate(ec, current, current + 1);
			}
			//compare left
			if(k > 0 && ptrn[current] == ptrn[current -1]){
				equate(ec, current, current -1);
			}
			//compare below
			if(j < width-1 && ptrn[current] == ptrn[current + width]){
				equate(ec, current, current + width);
			}
			k++;
		}
		j++;
	}
	int cnt = classCount(ec);
	printf("#classes = %d\n",cnt);
	int * reps = classReps(ec);
	for (int i = 0; reps[i] >= 0; i++) {
		int * class = classOf(ec,reps[i]);
		printf("class of %d: ", reps[i]);
		for (int j = 0; class[j] >= 0; j++) {
			printf("%d ",class[j]);
		}
		printf("\n");	
	  free(class);
  }  
	free(reps);
	printf("---\n");
		// following the code in ex-test.c, display the classes
		// both as the contents of the classes array and as a new
		// image
		// free allocated data
	displayData(ec);
		// display the raw data of the EC instance
}
