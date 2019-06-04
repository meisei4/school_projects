#ifndef INTSTACK_H
#define INTSTACK_H

typedef struct intStack intStack;

intStack * stkCreate(unsigned int size);
int        stkEmpty(intStack * stk);
int        stkPush(intStack * stk,int v);
int        stkPop(intStack * stk);
int        stkTop(intStack * stk);
void       stkReset(intStack * stk);
void       stkState(char * str, intStack * stk);

#endif
