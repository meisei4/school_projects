#ifndef HASH_TBL_H
#define HASH_TBL_H

#include <stdbool.h>

typedef struct hashtbl hashtbl;
typedef unsigned int (*hashfn)(const char *, unsigned int);
typedef void (*workfn)(char * key, void * value, void * data);

// basic API
hashtbl *   createTable(unsigned int size, hashfn hashFunc);
// create KV store using hash table implementation with initial size and hash function

void        freeTable(hashtbl * tbl);
// free all storage associated with the table including the keys

void *      setValueForKey(hashtbl * tbl, char * key, void * value);
// key must be new space which will be retained by the table, if key is
// present its previous value is returned

void *      valueForKey(hashtbl * tbl, char * key, bool * present);
// if present is supplied it will be set on return to indicate whether the table
// already contained the key

void *      removeKey(hashtbl * tbl, char * key);
// returns current value of key

void        performOnEntries(hashtbl * tbl, workfn workFunc, void * workData);

// functions for working with keys
char *      strf(char * buf, char * format, ...);
// use format to create string in buf, if buf is null then malloc space
// return is either buf or new space

// debugging
typedef void (*displayfn)(FILE *, char *, void *);

void        displayState(FILE * fd, hashtbl * tbl);
void        displayWith(FILE * fd, hashtbl * tbl, displayfn displayFunc);
#endif
