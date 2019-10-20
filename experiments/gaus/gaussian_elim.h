//
// Created by kiarash on 10/19/19.
//

#ifndef RMC_TREEWIDTH_CODE_GAUSSIAN_ELIM_H
#define RMC_TREEWIDTH_CODE_GAUSSIAN_ELIM_H


#include <math.h>                                     // required for fabs()

int Gaussian_Elimination(float *A, int n, float *B)
{
    int row, i, j, pivot_row;
    float max, dum, *pa, *pA, *A_pivot_row;

//    for (i=0; i<n; i++){
//        for (j=0; j<n; j++)
//            printf("%f ", A[i][j]);
//        printf("\n");
//    }

    // for each variable find pivot row and perform forward substitution

    pa = A;
    for (row = 0; row < (n - 1); row++, pa += n) {

        //  find the pivot row

        A_pivot_row = pa;
        max = fabs(*(pa + row));
        pA = pa + n;
        pivot_row = row;
        for (i = row + 1; i < n; pA += n, i++)
            if ((dum = fabs(*(pA + row))) > max) {
                max = dum; A_pivot_row = pA; pivot_row = i;
            }
        if (max == 0.0) return 0;                // the matrix A is singular

        // and if it differs from the current row, interchange the two rows.

        if (pivot_row != row) {
            for (i = row; i < n; i++) {
                dum = *(pa + i);
                *(pa + i) = *(A_pivot_row + i);
                *(A_pivot_row + i) = dum;
            }
            dum = B[row];
            B[row] = B[pivot_row];
            B[pivot_row] = dum;
        }

        // Perform forward substitution

        for (i = row + 1; i < n; i++) {
            pA = A + i * n;
            dum = - *(pA + row) / *(pa + row);
            *(pA + row) = 0.0;
            for (j = row + 1; j < n; j++) *(pA + j) += dum * *(pa + j);
            B[i] += dum * B[row];
        }
    }

    // Perform backward substitution

    pa = A + (n - 1) * n;
    for (row = n - 1; row >= 0; pa -= n, row--) {
        if ( *(pa + row) == 0.0 ) return 0;           // matrix is singular
        dum = 1.0 / *(pa + row);
        for ( i = row + 1; i < n; i++) *(pa + i) *= dum;
        B[row] *= dum;
        for ( i = 0, pA = A; i < row; pA += n, i++) {
            dum = *(pA + row);
            for ( j = row + 1; j < n; j++) *(pA + j) -= dum * *(pa + j);
            B[i] -= dum * B[row];
        }
    }
    return 1;
}

#endif //RMC_TREEWIDTH_CODE_GAUSSIAN_ELIM_H
