/* Author:      Ian Neville
   Created:     2/17/17
   Resources:   pythag-1.c

*/

#include <stdio.h>
#include <math.h>



int
main(int argc, char * argv[]){
	int a, b, i, count, c;
	printf(" ");
	for(a = 1; a < 60; a = a + 1){
		printf("%d", a%10);
	}
	printf("\n");
	for(b = 1; b < 60; b = b + 1){
		printf("%d", b%10);
		for(a = 1; a < b; a = a + 1){
			c = a*a + b*b;
			if((int)sqrt(c) * (int)sqrt(c) == c){
				printf("*");
			}else{
				printf(" ");
			}
		}
		printf("\\");
		printf("\n");

	}
}

