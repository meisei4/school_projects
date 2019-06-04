#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char * argv[]) {
  int verbose = 0;
  char * outputName = NULL;
  int index;
  int c;
  
  opterr = 0;
  while ((c = getopt(argc,argv, "vo:")) != -1)
    switch (c) {
    case 'v':
      verbose = 1;
      break;
    case 'o':
      outputName = optarg;
      break;
    case '?':
      if (optopt == 'o')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort ();
    }
  
  printf ("verbose = %d, outputName = %s\n",verbose, outputName);
  
  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);
  return 0;
}







