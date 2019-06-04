

/* Author:      Ian Neville
   Created:     2/17/17
   Resources:   ClassExamples project
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
bool
isPrime(int n) {
	for (int i = 2; i <= n/2; i++) {
	  if (n % i == 0) {
		return false;
	  }
	}
	return true;
}


int
main(int argc, char * argv[]) {
	int i, p, exp;
	for(int pr = 2; pr < 100; pr = pr + 1){
		printf("%d = ", pr);
		i = pr;
		exp = 1;
		while(i % 2 == 0){
			if(i / 2 == 1){
				printf("%d ", 2);
			}else{
				printf("%d * ", 2);
			}
			i = i / 2;
		}
//		if(exp == 1){
//			printf("%d * ", p);
//		}else{
//			printf("2^%d * ", exp);
//		}
		for(p = 3; p <= pr/2; p = p+2){
			if(isPrime(p)){
				exp = 1;
				while(i % p == 0){
					if(i / p == 1){
						printf("%d", p);
					}else{
						printf("%d * ", p);
					}
					i = i / p;
					exp++;
				}
//				if(exp == 1){
//					printf("%d * ", p);
//				}else{
//					printf("%d^%d * ", p, exp);
//				}
			}
		}
		printf("\b");
		printf("\b");
		printf("\n");
	}

}
