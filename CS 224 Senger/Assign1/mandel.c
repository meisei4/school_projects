/* Author:      Ian Neville
   Created:     2/17/17
   Resources:   ClassExamples project

*/

#include <stdio.h>
#include <math.h>



#define X_LOW -2.0
#define X_HIGH 1.0
#define X_STEP 0.05
#define Y_LOW -1.0
#define Y_HIGH 1.0
#define Y_STEP 0.125

/*
#define X_LOW -1.0
#define X_HIGH 0.0
#define X_STEP 0.01
#define Y_LOW -1.0
#define Y_HIGH 0.0
#define Y_STEP 0.025
*/



int
main(int argc, char * argv[]) {

	double mag = 0;
	double temp;
	int iter = 0;
	for(double y = Y_LOW; y < Y_HIGH; y = y + Y_STEP){

		for(double x = X_LOW; x < X_HIGH; x = x + X_STEP){
			double ci = y;
			double cr = x;
			mag = sqrt(x*x + y*y);
			double zr = 0;
			double zi = 0;
			for(iter = 0; mag <= 2 && iter < 100; iter++){
				temp = zr;
				zr =  zr*zr - zi*zi + cr;
				zi =  2*zi*temp + ci;
				mag = sqrt(zr*zr + zi*zi);
			}
			if(iter == 100){
				printf(" ");
			}else if(iter > 20){
				printf("*");
			}else if(iter < 11 && iter > 9){
				printf("+");
			}else if(iter < 8 && iter > 4){
				printf(":");
			}else if(iter < 4 && iter > 0){
				printf(";");
			}else{
				printf(".");
			}
		}
		printf("\n");
	}
}
