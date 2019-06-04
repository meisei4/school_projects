//
//  persons.h
//  Person Database
//
//

#ifndef Person_Database_persons_h
#define Person_Database_persons_h

enum CopyType { PD_SHALLOW = 0, PD_DEEP = 1 };

struct Node {
    struct Person *value;
    struct Node *next;
};

struct PersonList {
    int size;
    struct Node *first;
};

struct Person {
    char *first;
    char *last;
    char *email;
    int age;
};

struct Person *personCreate(char *f, char *l, int age, char *email);
char *personFirstName(struct Person *p);
char *personLastName(struct Person *p);
char *personEmail(struct Person *p);
int personAge(struct Person *p);
void personDelete(struct Person *p);


struct PersonList *plCreate();
void plDelete(struct PersonList *db, enum CopyType copyType);
void plAdd(struct PersonList *db, struct Person *p);
void plRemovePerson(struct PersonList*db, struct Person *p);
struct PersonList *plFindByFirstName(struct PersonList *db, char *first);
struct PersonList *plFindByLastName(struct PersonList *db, char *last);
struct PersonList *plFindAtLeastAsOldAs(struct PersonList *db, int age);
struct Person *plGet(struct PersonList *db ,int n);
int plSize(struct PersonList *db);

#endif
