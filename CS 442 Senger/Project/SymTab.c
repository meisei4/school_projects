/* Author:      Ian Neville
   Created:     09/25.18
   Resources:   http://www.tutorialspoint.com/ansi_c/ternary_operator_examples.htm
                Nic Welch, Louis Wulterkins, Steven Senger
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "SymTab.h"


/* The symbol table entry structure proper.
*/
struct SymEntry {
  struct SymTab * table;
  char *name;
  int attrKind;
  void *attributes;
  struct SymEntry *next;
};

/* The symbol table structure proper. The hash table array Contents
   is dynamically allocated according to size
*/
struct SymTab {
  struct SymTab * parent;
  char *scopeName;
  int size;
  struct SymEntry **contents;
};

struct SymTab *
CreateSymTab(int size, char * scopeName, struct SymTab * parentTable) {
    struct SymTab * table = malloc(sizeof(struct SymTab));
    if(!scopeName){
        table->scopeName = NULL; 
    }else{
        table->scopeName = strdup(scopeName);
    }
    table->size = size;
    table->parent = parentTable;
    table->contents = malloc(sizeof(struct SymEntry *) * size);
    for(int i = 0; i < size; i++){
        table->contents[i] = NULL;
    }
    return table;
}

void
entryFree(struct SymEntry *content){
    if(!content){
        return;
    }
    free(content->name);
    struct SymEntry * temp = content;
    content = content->next;
    free(temp);
    entryFree(content);
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
    if(!aTable){
        return NULL;
    }
    struct SymTab *par = aTable->parent;
    void * args;
    for(int i = 0; i < aTable->size; i++){
        entryFree(aTable->contents[i]);
    }
    free(aTable->contents);
    free(aTable->scopeName);
    free(aTable);
    return par;
}

int
HashName(int size, const char *name) {
    int len = strlen(name);
    int hash = 0;
    for(int i = 0; i < len; i++){
        hash += name[i];
    }
    int i = hash % size;
    return i;
}

struct SymEntry *
checkForName(struct SymTab *aTable, const char * name){
    if(!aTable){ 
        return NULL;
    }
    int i = HashName(aTable->size, name);
    struct SymEntry * tmp = aTable->contents[i];
    while(tmp){
        if(strcmp(name, tmp->name) == 0){
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {
    if(!aTable){
        return NULL;
    }
    struct SymEntry * check = checkForName(aTable, name);
    if(check){
        return check;
    }
    return LookupName(aTable->parent, name);
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name){
    struct SymEntry * check = checkForName(aTable, name);
    if(check){
        return check;
    }
    struct SymEntry *entry = malloc(sizeof(struct SymEntry));
    int i = HashName(aTable->size, name);
    struct SymEntry *space = aTable->contents[i];
    entry->next = space;
    entry->table = aTable;
    entry->name = strdup(name);
    aTable->contents[i] = entry;
    return entry;
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
    anEntry->attrKind = kind;
    anEntry->attributes = attributes;
    return;
}

int
GetAttrKind(struct SymEntry *anEntry) {
    return !anEntry ? 0 : anEntry->attrKind;
}

void *
GetAttr(struct SymEntry *anEntry) {
    return !anEntry ? NULL : anEntry->attributes;
}

const char *
GetName(struct SymEntry *anEntry) {
    return !anEntry ? NULL : anEntry->name;
}

struct SymTab *
GetTable(struct SymEntry *anEntry) {
    return !anEntry ? NULL : anEntry->table;
}

const char *
GetScopeName(struct SymTab *aTable) {
    return !aTable ? NULL : aTable->scopeName;
}

char *
linkPath(char * path1, char * path2){
    int len1 = strlen(path1);
    int len2 = strlen(path2);
    if(len1 == 0){
        char *p2 = strdup(path2);
        return p2;
    }
    char * linkedPath = malloc(sizeof(char) * (len1 + 1 + len2 + 1));
    snprintf(linkedPath, len1 + len2 + 2, "%s%s%s", path1, ">", path2);
    return linkedPath;
}

char *
GetScopePath(struct SymTab *aTable) {
    if(!aTable){
        return  "";
    }
    char *parentpth = GetScopePath(aTable->parent);
    char *path = linkPath(parentpth, aTable->scopeName);
    return path;
}

struct SymTab *
GetParentTable(struct SymTab *aTable) {
    return !aTable ? NULL : aTable->parent;
}

void
InvokeOnEntries(struct SymTab *aTable, bool includeParentTable, entryWorkFunc workFunc, int startCnt, void * withArgs) {
    int cnt = startCnt;
    if(!aTable){
        return;
    }
    for(int i = 0; i < aTable->size; i++){
        struct SymEntry * entry = aTable->contents[i];
        while(entry){
            workFunc(entry, startCnt, withArgs);
            entry = entry->next;
            startCnt++;
        }
    }
    if(includeParentTable){
        InvokeOnEntries(aTable->parent, true, workFunc, startCnt, withArgs);      
    }
    return;
}    

int 
length(struct SymEntry * content){
    int size = 0;
    while(content){
        size++;
        content = content->next;
    }
    return size;
}
//STATS
int 
max(int * lengths, int len){
    int max = lengths[0];
    for(int i = 0; i < len; i++){
        if(lengths[i] > max){
            max = lengths[i];
        }
    }
    return max;
}
//check if no length
int 
min(int * lengths, int len){
    int min = lengths[0];
    for(int i = 0; i < len; i++){
        if(lengths[i] < min){
            min = lengths[i];
        }
    }
    return min;
}

int 
sum(int * lengths, int len){
    int sum = 0;
    for(int i = 0; i < len; i++){
        sum += lengths[i];
    }
    return sum;
}

struct Stats *
Statistics(struct SymTab *aTable) {
    struct Stats * stats = malloc(sizeof(struct Stats *));
    int lengths[aTable->size];
    for(int i = 0; i < aTable->size; i++){
        lengths[i] = length(aTable->contents[i]);
    }
    stats->entryCnt = sum(lengths, aTable->size);
    stats->minLen = min(lengths, aTable->size);
    stats->maxLen = max(lengths, aTable->size);
    stats->avgLen = stats->entryCnt/aTable->size;
    return stats;
}
