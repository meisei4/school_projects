#include <stdio.h>
double A[MAX][MAX], B[MAX][MAX]; 
for (i=0; i< MAX; i++) {
        for (j=0; j< MAX; j++) {
        A[i][j] = A[i][j] + B[j][i];
    }
}

for (i=0; i< MAX; i+=tile_size) {
    for (j=0; j< MAX; j+=tile_size) {
        for (ii=i; ii<i+tile_size; ii++) {
            for (jj=j; jj<j+tile_size; jj++) {
                A[ii][jj] = A[ii][jj] + B[jj][ii];
            }
        }
    }
}

