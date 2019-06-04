#include <stdio.h>
#include <ctype.h>

/* Author:      Ian Neville
   Created:     2/17/17
   Resources:   ClassExamples project

*/
int digitCnt(int val) {
	int count = 1;
	while(val >= 10){
		count++;
		val = val/10;
	}
	return count;
}

char digitAtPosition(int val, int i) {
	int cnt = digitCnt(val);
	int out = val;
	for(int j = 1; j < (cnt - i); j++){
		val = val/10;
	}
	return (char)val%10 + '0';
}

int main(int argc, char * argv[]) {
	int hist[26];
	int stage[26];
	for(int i = 0; i < 26; i++){
		hist[i] = 0;
		stage[i] = 0;
	}
	char c;
	while( (c = getchar()) != EOF){
		if(isalpha(c)){
			char up_c = toupper(c);
			int index = up_c - 'A';
			int h = hist[index];
			h++;
			hist[index] = h;
		}
	}
	int max = digitCnt(hist[0]);
	for(int i = 0; i < 26; i++){
		stage[i] = digitCnt(hist[i]);
		if(digitCnt(hist[i]) > max){
			max = digitCnt(hist[i]);
		}
	}

	for(int i = 0; i < max; i++){
		for(int j = 0; j < 26; j++){
			int cnt = digitCnt(hist[j]);
			if(cnt >= (max - i)){
				printf("%c ", digitAtPosition(hist[j], cnt - stage[j]));
				stage[j]--;
			}else{
				printf("  ");
			}
		}
		printf("\n");
	}

	for(int k = 'A'; k <= 'Z'; k++){
		printf("%c ", k);
	}
	printf("\n");
}
