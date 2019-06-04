/* Author:      Ian Neville
 * Created:     2/23/17
 * Resources:   ClassExamples project
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

void readinput();
void displaybuffer();
int i, skip, length, width, bytecnt, prints;
//prints is boolean
char format, groups;
void
main(int argc, char * argv[]){
	int i;
	prints = 0;
	groups = 1;
	skip = 0;
	length = 256;
	width = 32;
	bytecnt = 0;
	format = getopt(argc, argv, "-x-o-d-a-g:-s:-l:-w:");
	while((i = getopt(argc, argv, "-x-o-d-a-g:-s:-l:-w:")) != -1){
		switch(i){
			case 'a': prints = 1;
				break;
			case 'g': groups = atoi(optarg);
				break;
			case 's': skip = atoi(optarg);
				break;
			case 'l': length = atoi(optarg);
				break;
			case 'w': width = atoi(optarg);
				break;
			default: printf("Error man");
				break;
		}
		while(bytecnt < length){
			readinput();
		}
	}
}

void
readinput(){
	char buf[groups];
	printf("%5d -- ", bytecnt);
	int i, iter = 0;
	unsigned char c;
	while(iter < width){
		i = 0;
		while(i < groups && (c = getchar()) != EOF){
			buf[i++] = c;
		}
		iter++;
		displaybuffer(buf);
		printf(" ");
	}
	bytecnt+=iter;
	printf("\n");
}


void
displaybuffer(char * buf){
	int dumpint = 0;
	short dumpshort = 0;
	if(prints && isprint(* buf)){
		printf("%2c", * buf);
	}else{
		char foo[4] = "%0";
		foo[2] = groups;
		foo[3] = format;
		if(groups == 2){
			for(int j = 0; j < 2; j++){
				dumpshort = dumpshort << 8;
				dumpshort = dumpshort + buf[j];
			}
			printf(foo, dumpshort);
		}else{
			if(groups == 4){
				for(int j = 0; j < 4; j++){
					dumpint = dumpint << 8;
					dumpint = dumpint + buf[j];
				}
				printf(foo, dumpint);
			}else{
				printf(foo, (unsigned char)* buf);
			}
		}

	}
}



