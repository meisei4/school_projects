/* Author:      Ian Neville
   Created:     10/3.18
   Resources:   Steven Senger I/O Basics
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOMngr.h"

FILE *sourceFile;
char buffer[MAXLINE];
int bufLen, nextPos;
int curLine;
bool echoAll;
bool needsDisplay;
int eof;

struct message {
    int startColumn;
    int endColumn;
    char * message;
};
#define MAX_MESSAGES 26
struct message messages[MAX_MESSAGES];
int messageCnt;

void WriteStartSeq(int messageNum) {
    fprintf(stdout,"\033[7m %c \033[0m\033[4m",'A' + messageNum);
}

void WriteResetSeq() {
    fprintf(stdout,"\033[0m");
}

void WriteBuffer(){
    if(buffer[0] != '\0' && echoAll || messageCnt > 0){
        fprintf(stdout, "%5d: ", curLine);
    }
    int start = 0;
    for(int i = 0; i < messageCnt; i++){
        int j;
        for(j = start; j < messages[i].startColumn-1; j++){
            fprintf(stdout, "%c", buffer[j]);
        }
        WriteStartSeq(i);
        int k;
        for(k = j; k < messages[i].endColumn-1; k++){
            fprintf(stdout, "%c", buffer[k]);
        }
        WriteResetSeq();
        start = k;
    }
    for(int i = start; i < bufLen; i++){
        fprintf(stdout, "%c", buffer[i]);
    }
    buffer[0] = '\0';
    bufLen = 0;
}

void fillBuffer(){
    char * tmp = fgets(buffer, MAXLINE, sourceFile);
    if(tmp){
        bufLen = strlen(buffer); 
    }else{
        eof = 1;
    }
}

bool OpenSource(const char * aFileName, bool mode) {
    sourceFile = fopen(aFileName, "r");
    if(!sourceFile){
        return false;
    }
    echoAll = mode;
    eof = 0;
    fillBuffer();
    bufLen = strlen(buffer);
    curLine = 1;
    nextPos = 0;
    messageCnt = 0;
    return 1;
}

void writeMsg(){
    for(int i = 0; i < messageCnt; i++){
        fprintf(stdout, "       -%s\n", messages[i].message);
    } 
}

void printMsgs(){
    if(echoAll || messageCnt > 0){
        WriteBuffer();
        writeMsg();
    }
}

void CloseSource() {
    if(messageCnt > 0){
        printMsgs(); 
    }
    fclose(sourceFile);
}

char GetSourceChar() {
    char c;
    if(eof){
        return EOF;
    }
    if(nextPos == bufLen){
        printMsgs();
        curLine++;
        messageCnt = 0;
        fillBuffer();
        nextPos = 0;
    }
    c = buffer[nextPos];
    nextPos++;
    return c;
    
}

void PostMessage(int aColumn, int aLength, const char * aMessage){
    messages[messageCnt].startColumn = aColumn;
    messages[messageCnt].endColumn = aColumn + aLength;
    char * marked = malloc(sizeof(char) * (3 + strlen(aMessage)));
    snprintf(marked, 3 + strlen(aMessage), "%c %s", 'A' + messageCnt, aMessage);
    messages[messageCnt].message = marked;
    messageCnt++;
}

int GetCurrentLine() {
    return curLine;
}

int GetCurrentColumn() {
    return nextPos;
}

