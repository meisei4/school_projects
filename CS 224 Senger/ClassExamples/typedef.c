#include <stdio.h>

#include "typelist.h"

int
main(int argc, char * argv[]) {
  listHead list;
  
  addList(&list,"one");
  addList(&list,"two");
  addList(&list,"three");
  addList(&list,"four");
  
  printList(list);
}
