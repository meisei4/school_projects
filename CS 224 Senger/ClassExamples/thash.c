#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

#include "hashtbl.h"
#include "hashfn.h"

void
displayKeyValue(FILE * fd, char * key, void * value) {
  fprintf(fd,"key: %s val: %d\n",key,(int)value);
}

int
main(int argc, char * argv[]) {
  unsigned int tableSize = 97;
  int verbose = 0;
  unsigned int (*hashfunc)(const char *, unsigned int) = sumHash;
  
  opterr = 0;
  int c;
  while ((c = getopt(argc, argv, "vsfbn:")) != -1)
    switch (c) {
      case 'v':
        verbose = 1;
        break;
      case 's':
        hashfunc = sumHash;
        break;
      case 'f':
        hashfunc = FNV_1a;
        break;
      case 'b':
        hashfunc = FNV_1a_byte;
        break;
      case 'n':
        tableSize = strtol(optarg,NULL,10);
        break;
      case '?':
        if (optopt == 'o')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        abort ();
    }
  
  hashtbl * tbl = createTable(tableSize,hashfunc);
  
  char last[32];
  char first[32];
  char full[66];
  long age;
  while (scanf("%31s %31s %ld\n",last,first,&age) == 3) {
    sprintf(full,"%s, %s",last,first);
    void * value;
    bool present;
    if ((value = valueForKey(tbl,full,&present))) {
      if (verbose && present) fprintf(stderr,"already entered \"%s\" with val %u\n",full,(unsigned int)value);
    }
    else {
      setValueForKey(tbl,strdup(full),(void *)age);
    }
  }
  
  displayState(stderr,tbl);
  if (verbose) displayWith(stderr,tbl,displayKeyValue);
}
