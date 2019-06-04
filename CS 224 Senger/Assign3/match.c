/* Author:      Ian Neville
 * Created:     2/23/17
 * Resources:   ClassExamples project
 */
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int prefixsuffix(char*, int, int);

char nextchar();

int line = 1, col = 0;
void main (int argc, char * argv[]){
	int i = 0;
	char * ptrn = argv[1];

  ////////////////////////////////print indexing
	while(i < strlen(ptrn) + 1){
		printf("%3d", i++);
	}
	printf("\n");

  /////////////////////////////////print pattern
	i = 0;
	while(i < strlen(ptrn) + 1){
		printf("%3c", ptrn[i++]);
	}
	printf("\n");

  /////////////////////////////////////////////
	int j, shift[strlen(ptrn) + 1];
	i = strlen(ptrn) + 1;
	while(i >= 0){
		j = i-1;
		shift[i] = 0;
		while(j >= 0){
			if(prefixsuffix(ptrn, j, i) == 1){
				shift[i] = j;
				break;
			}
			j--;
		}
		i--;
	}
	for(int k = 0; k < strlen(ptrn) + 1 ; k++){
		printf("%3d", shift[k]);
	}
	printf("\n");
	j = 0;
	char c = nextchar();
	while(c != EOF){
		if(ptrn[j] == c){
			j++;
			c = nextchar();
		}
		if(ptrn[j] == '\0'){
			printf("line: %5d  col: %5d", line, col);
			printf("\n");
			j = shift[j];
		}else if(c != EOF && ptrn[j] != c){
			if(j != 0){
				j = shift[j];
			}else{
				c = nextchar();
			}
		}
	}

}

int prefixsuffix(char * ptrn, int pfxlen, int strlen){
	int x = 0;
	int y = strlen - pfxlen;
	int count = 0;
	while(x < pfxlen){
		if(ptrn[x++] != ptrn[y++]){
			return 0;
		}
		count++;
	}
	if(count == pfxlen){
		return 1;
	}
	return -1;
}

char nextchar(){
	char ch;
	if((ch = getchar()) == '\n'){
		line++;
		col = 0;
		return getchar();
	}
	col++;
	return ch;
}


