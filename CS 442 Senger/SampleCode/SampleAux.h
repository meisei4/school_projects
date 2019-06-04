/* Routines to manage a list of names
   This is intended as a short example of C needed for the assignments.
*/

// structure used to build linked list of names and values
// could hide the definition in the implementation and have have
// struct NameNode;
struct NameNode {
  char * name;
  int val;
  int seq;
  struct NameNode * next;
};

void   InitList();
void   AddName(char *name, int val);
void   RemoveHead();
char * NameWithValue(int value);
void   DoForAll(void (*nameFunc)(struct NameNode *, void * ),void * withArg);
