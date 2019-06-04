/* Author:      Ian Neville
   Created:     2/17/17
   Resources:   ClassExamples project
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 32

int gcd(int x, int y) {
	while (x != y) {
		if (x < y) {
			y = y - x;
		}
		else {
			x = x - y;
		}
	}
	return x;
}

void rotate(char * buf, int len, int cnt) {
	char * p = buf + 1; //initialize p as pointer to &buf[1]
	for(int i = 0; i < gcd(cnt, len); i++){
		char t = p[i]; //for ith pass
		int j = i; //set as starting index for pass;
		while(1){
			int k = j + cnt; //rotate index for switching j an k
			if(k >= len){ //if the rotation index overshoots the length
				k = k - len; //invert the value to correct indexing
			}
			if(k == i){
				break; //fully rotated
			}
			p[j] = p[k]; //start the swap
			j = k; //update j
		}
		p[j] = t; //finish swap
	}
}
int main(int argc, const char * argv[]) {
	int wlen = 0;
	char wbuf[MAX_WORD_LEN+1];

	while (1) {
		int c = getchar();
		if (isalpha(c)) {
			if (wlen < MAX_WORD_LEN) wbuf[wlen++] = c;
		}else{
			if (wlen > 3) {
				wbuf[wlen] = '\0';
				rotate(wbuf, wlen-2, (wlen-2)/2);
				fprintf(stdout,"%s", wbuf);
				wlen = 0;
			}else{
				wbuf[wlen] = '\0';
				fprintf(stdout,"%s", wbuf);
				wlen = 0;
			}
			putchar(c);
		}
		if (c == EOF) break;
	  }
	  return 0;
}
