#include <stdio.h>

int
main(int argc, char * argv[]) {
  int m[2][4] = { {0,1,2,3}, {4,5,6,7} };
    
  for (int r = 0; r < 2; r++) {
    for (int c = 0; c < 4; c++) {
      printf(" m[%d][%d] = %d ",r,c,m[r][c]);
    }
    printf("\n");
  }
  printf("\n");
  
  int * n[2];
  int n0[4] = {0,1,2,3};
  int n1[4] = {4,5,6,7};
  
  n[0] = n0; n[1] = n1;
  
  for (int r = 0; r < 2; r++) {
    for (int c = 0; c < 4; c++) {
      printf(" n[%d][%d] = %d ",r,c,n[r][c]);
    }
    printf("\n");
  }
  printf("\n");
  
}
