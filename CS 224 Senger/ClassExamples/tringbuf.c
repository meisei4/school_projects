#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "ringbuf.h"

struct data {
  int val;
};

void
displayData(void * data) {
  fprintf(stderr,"%d",((struct data *)data)->val);
}

int
main(int argc, char * argv[]) {
  
  int verbose = 0;
  int c;
  opterr = 0;
  while ((c = getopt(argc,argv,"v")) != -1) {
    switch (c) {
      case 'v':
        verbose = 1;
        break;
      case '?':
        if (optopt == 'o')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        return 1;
    }
  }
  
  // commands
  // c size cnt   -- create ring buffer of size and cnt
  // g val        -- give val to buffer
  // t            -- take and display value from buffer
  // k val        -- take and check equal to val
  // r grp cycles -- repeat grp size, cycles times
  // d            -- display buffer
  
  ringBuffer * ring;
  struct data data;
  char cmd[16];
  
  while (fgets(cmd,16,stdin)) {
    if (verbose) fprintf(stderr,"> %s",cmd);
    switch (cmd[0]) {
      case 'c': {
        size_t size, cnt;
        char mode;
        int ret = sscanf(&(cmd[1]),"%lu %lu %c\n",&size,&cnt,&mode);
        if (ret != 3) {
          fprintf(stderr,"malformed create command\n");
          break;
        }
        switch (mode) {
          case 'f':
            ring = createRingBuffer(size,cnt,MODE_FAIL);
            break;
          case 'n':
            ring = createRingBuffer(size,cnt,MODE_DISCARD_NEW);
            break;
          case 'o':
            ring = createRingBuffer(size,cnt,MODE_DISCARD_OLD);
            break;
        }
      } break;
      case 'g': {
        int val;
        int ret = sscanf(&(cmd[1]),"%d\n",&val);
        if (ret != 1) {
          fprintf(stderr,"malformed give command\n");
          break;
        }
        data.val = val;
        ret = give(ring,&data);
        if (ret) {
          fprintf(stderr,"give command failed\n");
          break;
        }
      } break;
      case 't': {
        int ret = take(ring,&data);
        if (ret) {
          fprintf(stderr,"take command failed\n");
          break;
        }
        fprintf(stdout,"val "); displayData(&data);
      } break;
      case 'k': {
        int val;
        int ret = sscanf(&(cmd[1]),"%d\n",&val);
        if (ret != 1) {
          fprintf(stderr,"malformed check command\n");
          break;
        }
        ret = take(ring,&data);
        if (ret) {
          fprintf(stderr,"take command failed\n");
          break;
        }
        if (data.val != val) fprintf(stdout,"check failed %d %d\n",val,data.val);
      } break;
     case 'r': {
        int val = 0;
        int grpcnt, cycles;
        int ret = sscanf(&(cmd[1]),"%d %d\n",&grpcnt,&cycles);
        for (int i = 0; i < cycles; i++) {
          for (int g = 0; g < grpcnt; g++) {
            data.val = val++;
            ret = give(ring,&data);
            if (ret) {
              fprintf(stderr,"give command failed\n");
              break;
            }
          }
          for (int g = 0; g < grpcnt; g++) {
            data.val = val++;
            ret = take(ring,&data);
            if (ret) {
              fprintf(stderr,"take command failed\n");
              break;
            }
            fprintf(stderr,"val %d\n",data.val);
          }
        }
      } break;
      case 'd': {
        display(ring,displayData);
      } break;
      default: {
        
      }
    }
  }
}
