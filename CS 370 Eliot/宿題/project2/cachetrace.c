#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

//set parameters
unsigned int style;
unsigned int offset;
unsigned int indx;
char *alloc;
char *write;

//cache sets array and masking info
struct cacheset *sets;
unsigned int numsets;
unsigned int offsetmsk;
unsigned int indxmsk;
unsigned int tagmsk;

//statistics
int rhits = 0;
int whits = 0;
int rmisses = 0;
int wmisses = 0;
float hrate;
int wb = 0;
int wt = 0;

struct cacheset{
    int fifoflag[2];
    int valid[2];
    int dirt[2];
    unsigned int tag[2];
};

void setParameters(){
    alloc = malloc(sizeof(char) * 3);
    write = malloc(sizeof(char) * 2);
    FILE *fin = fopen("parameters.txt", "r");
    fscanf(fin, "%d", &style);
    fscanf(fin, "%d", &offset);
    fscanf(fin, "%d", &indx);
    fscanf(fin, "%s", alloc);
    fscanf(fin, "%s", write);
    numsets = (int)pow(2.0, (double)indx);
    indxmsk = (int)pow(2.0, (double)indx) - 1 << offset;
    offsetmsk = (int)pow(2.0, (double)offset) - 1;
    tagmsk = ~(indxmsk | offsetmsk);
    sets = calloc(numsets, sizeof(struct cacheset));
    ///NEVERFORGETAGAIN
    for(int i = 0; i < numsets; i++){
        sets[i].tag[0] = 0;
        sets[i].tag[1] = 0;
        sets[i].dirt[0] = 0;
        sets[i].dirt[1] = 0;
        sets[i].valid[0] = 0;
        sets[i].valid[1] = 0;
        sets[i].fifoflag[0] = 0;
        sets[i].fifoflag[1] = 0;
    }
}

void replace(char rw, unsigned int i, unsigned int tag, int way){
    if(write && strcmp(write, "wb") == 0 && sets[i].dirt[way] == 1){
        wb++;
    }
    if(rw == 'r'){
        sets[i].dirt[way] = 0;
    }
    sets[i].tag[way] = tag;
    sets[i].valid[way] = 1;
    sets[i].fifoflag[way] = 0;
    int other = way ? 0 : 1;
    sets[i].fifoflag[other] = 1;
}

void update(char rw, char hm, unsigned int i, int way){
    if(rw == 'r'){
        if(hm == 'h'){
            rhits++;
        }else{
            rmisses++;
        }
    }else if(rw == 'w'){
        if(write && strcmp(write, "wt") == 0){
            wt++;
        }
        sets[i].dirt[way] = 1; //ignore wt policy because doesnt use it anyway
        if(hm == 'h'){
            whits++;
        }else{
            if(write && strcmp(write, "wb") == 0 && alloc && strcmp(alloc, "wna")){
                wt++;
            }
            wmisses++;
        }
    }
}

void missReplace(char rw, unsigned int i, unsigned int tag, int way){
    update(rw, 'm', i, way);
    if(style == 2){
        if(alloc && strcmp(alloc, "wa") == 0 || rw == 'r'){
            if(sets[i].fifoflag[0]){
                replace(rw, i, tag, 0);
            }else{
                replace(rw, i, tag, 1);
            }
        }
    }else{
        if(alloc && strcmp(alloc, "wa") == 0 || rw == 'r'){
            replace(rw, i, tag, 0);
        }
    }
}

void wayAccess(char rw, unsigned int i, unsigned int tag){
    if(sets[i].valid[0] == 1){
        if(sets[i].tag[0] == tag){
            update(rw, 'h', i, 0);
        }else if(sets[i].valid[1] == 1){
            if(sets[i].tag[1] == tag){
                update(rw, 'h', i, 1);
            }else{
                missReplace(rw, i, tag, 1);
            }
        }else{
            missReplace(rw, i, tag, 0);   
        }   
    }else{
        missReplace(rw, i, tag, 0);
    }
}

void directAccess(char rw, unsigned int i, unsigned int tag){
    if(sets[i].valid[0] == 1){
        if(sets[i].tag[0] == tag){
            update(rw, 'h', i, 0); 
        }else{ 
            missReplace(rw, i, tag, 0);  
        }
    }else{
        missReplace(rw, i, tag, 0);
    }
}

void access(char rw, unsigned int addr, unsigned int style){
    unsigned int i = (addr & indxmsk) >> offset;
    unsigned int off = (addr & offsetmsk);
    unsigned int tag = (addr & tagmsk) >> (offset + indx);
    //if style == 1 then it is direct mapped
    if(style == 1){ 
        directAccess(rw, i, tag);
    }else{
        wayAccess(rw, i, tag);
    }
}

int main(int argc, char * argv[]){
    setParameters();
    FILE *fin = fopen("accesses.txt", "r");
    char rw;
    unsigned int addr;
    //double reading for some reason with null rws
    while(fscanf(fin, "%c", &rw) != EOF){
        fscanf(fin, "%x", &addr);
        access(rw, addr, style);
    }
    float totalchecks = rhits + whits + rmisses + wmisses;
    hrate = (rhits + whits) / totalchecks;
    FILE *fout = fopen("statistics.txt", "w");
    fprintf(fout, "rhits: %d\n", rhits); 
    fprintf(fout,"whits: %d\n", whits);
    fprintf(fout, "rmisses: %d\n", rmisses);
    fprintf(fout, "wmisses: %d\n", wmisses); 
    fprintf(fout, "hrate: %.6g\n", hrate);
    fprintf(fout, "wb: %d\n", wb);
    fprintf(fout, "wt: %d\n", wt);
}
