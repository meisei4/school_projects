#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main(int argc, char * argv[]) {
  
  time_t clock = time(NULL);
  printf("clock %ld\n",clock);
  
  struct tm * tm = localtime(&clock);
  printf("tm sec: %d min: %d hour: %d\n",tm->tm_sec,tm->tm_min,tm->tm_hour);
  
  char * format = (argc == 2) ? (argv[1]) : ("%d/%m/%Y @ %H:%M:%S");
  
  char buf[32];
  strftime(buf,32,format,tm);
  printf("time %s\n",buf);
}
