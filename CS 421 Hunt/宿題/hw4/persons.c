#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persons.h"
#include <limits.h>

struct Person *personCreate(char *first, char *last, int age, char *email){
    struct Person *person = malloc(sizeof(struct Person));
    person->first = strdup(first);
    person->last = strdup(last);
    person->age  = age;
    person->email = strdup(email);
    return person;
}

void personDelete(struct Person *person){
    free(person->first);
    person->first = NULL;
    free(person->last);
    person->last = NULL;
    free(person->email);
    person->email = NULL;
    free(person);
    person = NULL;
}

char *personFirst(struct Person *person){
    return person->first;  
}

char *personLast(struct Person *person){
    return person->last;
}

char *personEmail(struct Person *person){
    return person->email;
}

int personAge(struct Person *person){
    return person->age;
}

struct PersonList *plCreate(){
    struct PersonList *db = malloc(sizeof(struct PersonList));
    db->size = 0;
    db->first = NULL;
    return db;
}

void plAdd(struct PersonList *db, struct Person *p){
    struct Node *pnode = malloc(sizeof(struct Node));
    pnode->value = p;
    pnode->next = db->first;
    db->first = pnode;
    db->size++;
}

void plRemove(struct PersonList *db, struct Person *p){
    struct Node *itr = db->first;
    if(itr->value == p){
        db->first = itr->next;
        free(itr);
        itr = NULL;
    }else{
        while(itr->next && itr->next->value != p){
            itr = itr->next;
        }
        if(itr->next){
            struct Node *temp;
            temp = itr->next;
            itr->next = itr->next->next;
            free(temp);
            temp = NULL;
        }
    }
    db->size--;
}

void plDelete(struct PersonList *db, enum CopyType copyType){    
    while(db->size > 0){
        struct Node *temp = db->first;
        plRemove(db, temp->value);
        if(copyType == PD_DEEP){
            personDelete(temp->value);
        }
    } 
    free(db); 
    db = NULL;
}

struct PersonList *plFindByFirstName(struct PersonList *db, char *name){
    struct PersonList *firsts = malloc(sizeof(struct PersonList));
    struct Node *itr = db->first;
    while(itr){
        if(strcmp(itr->value->first, name) == 0){
            plAdd(firsts, itr->value);
        }
        itr = itr->next;
    }
    return firsts;
}

struct PersonList *plFindByLastName(struct PersonList *db, char *name){
    struct PersonList *lasts = malloc(sizeof(struct PersonList));
    struct Node *itr = db->first;
    while(itr){
        if(strcmp(itr->value->last, name) == 0){
            plAdd(lasts, itr->value);
        }
        itr = itr->next;
    }
    return lasts;
}

struct PersonList *plFindAtleastAsOldAs(struct PersonList *db, int age){
    struct PersonList *ages = malloc(sizeof(struct PersonList));
    struct Node *itr = db->first;
    while(itr){
        if(itr->value->age >= age){
            plAdd(ages, itr->value);
        }
        itr = itr->next;
    }
    return ages;
}

struct Person *plGet(struct PersonList *db, int n){
    struct Node *itr = db->first;
    int cnt = 0;
    while(itr){
        if(cnt == n){
            return itr->value;
        }
        itr = itr->next;
        cnt++;
    }
    return NULL;
}

int plSize(struct PersonList *db){
    return db->size;
}
