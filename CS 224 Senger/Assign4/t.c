#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct test{
	int size;
	int array[size];

};

void
main(int argc, char * argv){
	struct test * nigs = malloc(sizeof(struct test));
	int i = 0;
	nigs->array[3] = 4;
	while(i < 8){
		printf("%d, ", nigs->array[i]);
		i++;
	}
}
