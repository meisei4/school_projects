/* Author:      Ian Neville
   Created:     2/17/17
   Resources:   https://en.wikipedia.org/wiki/Pythagorean_triple

*/

#include <stdio.h>
#include <math.h>

int
main(int argc, char * argv[]) {
	int a, b, c;
	for(b = 2; b < 60; b = b + 1){
		for(a = 1; a < b; a = a + 1){
			c = (a*a) + (b*b);
			if((int)sqrt(c)* (int)sqrt(c) == c){
				printf("triple: %d %d %d \n", a, b, (int)sqrt(c));
			}
		}
	}
}
