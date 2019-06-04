#ifndef KEYLIST_H
#define KEYLIST_H

typedef struct list * list;

list makeList(int (* cmpfunc)(const void *, const void *));
void insertList(list aList, void * data);
void * firstData(list aList);
void * nextData(list aList);

#endif
