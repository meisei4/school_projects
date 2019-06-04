#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <complex.h>

int
main(int argc, char * argv[])
{
  printf("type                size                 min                  max\n");
  printf("char                %3ld %20d %20d\n",  sizeof(char),       SCHAR_MIN,SCHAR_MAX);
  printf("unsigned char       %3ld %20d %20d\n",  sizeof(char),       0,UCHAR_MAX);
  printf("int                 %3ld %20d %20d\n",  sizeof(int),        INT_MIN,INT_MAX);
  printf("short int           %3ld %20d %20d\n",  sizeof(short int),  SHRT_MIN,SHRT_MAX);
  printf("long int            %3ld %20ld %20ld\n",sizeof(long int),   LONG_MIN,LONG_MAX);
  printf("float               %3ld %20e %20e\n",  sizeof(float),      FLT_MIN,FLT_MAX);
  printf("double              %3ld %20e %20e\n",  sizeof(double),     DBL_MIN,DBL_MAX);
  printf("long double         %3ld %20Le %20Le\n",sizeof(long double),LDBL_MIN,LDBL_MAX);
  printf("float complex       %3ld\n",            sizeof(float complex));
  printf("long complex        %3ld\n",            sizeof(long complex));
  printf("long double complex %3ld\n",            sizeof(long double complex));

}