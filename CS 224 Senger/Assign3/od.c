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
int skip, length, width, bytecnt, prints, groups;
char format;

int
main(int argc, char * argv[]){
	int i;
	prints = 0;
	groups = 1;
	skip = 0;
	length = 256;
	width = 32;
	bytecnt = 0;
	while((i = getopt(argc, argv, "xodag:s:l:w:")) != -1){
		switch(i){
			case 'x': format = 'x';
				break;
			case 'o': format = 'o';
				break;
			case 'd': format = 'u';
				break;
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
			default: printf("error");
				break;
		}
	}
	while(bytecnt < length){
		if(length%groups != 0){
			printf("error in length");
			break;
		}
		readinput();
	}
}

void
readinput(){
	for(int k = 0; k < skip; k++){
		getchar();
	}
	unsigned char buf[groups];
	printf("%5d -- ", bytecnt);
	int i, iter = 0, c;
	while(iter < width){
		i = 0;
		while(i < groups && (c = getchar()) != EOF){
			buf[i++] = c;
		}
		iter+=groups;
		displaybuffer(buf);
		printf(" ");
	}
	bytecnt+=iter;
	printf("\n");
}

void displaybuffer(char * buf){
	char form[] = "%***\0";
	form[3] = format;
	unsigned int num;
	switch(groups){
		case 2:
			num = (buf[1] << 8) | buf[0];
			switch(format){
				case 'x':
					form[1] = '0';
					form[2] = '4';
					break;
				case 'o':
					form[1] = '0';
					form[2] = '6';
					break;
				case 'u':
					form[1] = '0';
					form[2] = '5';
					break;
			}
			printf(form, (unsigned short) num);
			break;
		case 4:
			num = ((buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3]);
			switch(format){
				case 'x':
					form[1] = '0';
					form[2] = '8';
					break;
				case 'o':
					form[1] = '1';
					form[2] = '1';
					break;
				case 'u':
					form[1] = '1';
					form[2] = '0';
					break;
			}
			printf(form, (unsigned int)num);
			break;
		default:
			num = * buf;
			switch(format){
				case 'x':
					form[1] = '0';
					form[2] = '2';
					break;
				case 'o':
					form[1] = '0';
					form[2] = '3';
					break;
				case 'u':
					form[1] = '0';
					form[2] = '3';
					break;
			}
			if(groups == 1 && prints && isprint(* buf)){
				printf("%2c", * buf);
			}else{
				printf(form, (unsigned char) num);
			}
			break;
	}
}



