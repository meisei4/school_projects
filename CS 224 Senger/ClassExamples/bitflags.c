#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define VERBOSE_FLAG    (0x01)  // (1)
#define SKIP_FLAG       (0x02)  // (1 << 1)
#define USE_STDOUT_FLAG (0x04)  // (1 << 2)
#define REMOTE_FLAG     (0x08)  // (1 << 3)
#define DISCARD_FLAG    (0x10)  // (1 << 4)

void
displayFlags(unsigned char flags) {
  if (flags & VERBOSE_FLAG) printf("verbose set\n");
  if (flags & SKIP_FLAG) printf("skip set\n");
  if (flags & USE_STDOUT_FLAG) printf("use stdout set\n");
  if (flags & REMOTE_FLAG) printf("remote set\n");
  if (flags & DISCARD_FLAG) printf("discard set\n");
}

int
main(int argc, char * argv[]) {
  
  unsigned char flags = 0;
  
  opterr = 0;
  int c;
  while ((c = getopt(argc, argv, "VSORDvsord")) != -1)
    switch (c) {
      case 'V':
        flags |= VERBOSE_FLAG;
        break;
      case 'S':
        flags |= SKIP_FLAG;
        break;
      case 'O':
        flags |= USE_STDOUT_FLAG;
        break;
      case 'R':
        flags |= REMOTE_FLAG;
        break;
      case 'D':
        flags |= DISCARD_FLAG;
        break;
      case 'v':
        flags &= ~VERBOSE_FLAG;
        break;
      case 's':
        flags &= ~SKIP_FLAG;
        break;
      case 'o':
        flags &= ~USE_STDOUT_FLAG;
        break;
      case 'r':
        flags &= ~REMOTE_FLAG;
        break;
      case 'd':
        flags &= ~DISCARD_FLAG;
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
  
  displayFlags(flags);
}
