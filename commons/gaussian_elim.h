//
// Created by kiarash on 10/19/19.
//

#ifndef RMC_TREEWIDTH_CODE_GAUSSIAN_ELIM_H
#define RMC_TREEWIDTH_CODE_GAUSSIAN_ELIM_H


#include <math.h>                                     // required for fabs()
#include <assert.h>
#include <time.h>
#include "vector.h"
#include "mc_treedec.h"

int Gaussian_Elimination(double *A, int n, double *B)
{
    int row, i, j, pivot_row;
    double max, dum, *pa, *pA, *A_pivot_row;

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

double gaussian_solve_mc_discsum(vector e, int totV, int totE, double lambda, double res[])
{
    if (totV>=1241)
        return 0;

    double A[totV][totV], B[totV];
    memset(A, 0, sizeof(A));
    memset(B, 0, sizeof(B));

    clock_t begin = clock();

    int i, j;
    for (i=0; i<totV; i++)
        A[i][i] = -1.;

    assert(totE == vector_total(&e));
    for (i=0; i<totE; i++)
    {
        edge *ej = vector_get(&e, i);
        if (ej->delta < EPSILON)
            continue;
        B[ej->v] -= ej->delta * ej->reward;
        A[ej->v][ej->u] += ej->delta * lambda;
    }
    int has_solution = Gaussian_Elimination(&A[0][0], totV, B);
    if (!has_solution)
//        printf("******************* NO SOLUTION! *******************\n");
        ;
    else
        for (i=0; i<totV; i++)
            res[i] = B[i];

    clock_t end = clock();
    return (double) (end - begin) / CLOCKS_PER_SEC;
}

double gaussian_solve_mc_hitprob(vector e, int totV, int totE, int target, double res[])
{

    if (totV>=1241)
        return 0;
    double A[totV][totV], B[totV];
    memset(A, 0, sizeof(A));
    memset(B, 0, sizeof(B));

    clock_t begin = clock();

    int i, j;
    for (i=0; i<totV; i++)
        A[i][i] = -1.;
    A[target][target] = 1.;
    B[target] = 1;

    assert(totE == vector_total(&e));
    for (i=0; i<totE; i++)
    {
        edge *ej = vector_get(&e, i);
        if (ej->delta < EPSILON)
            continue;
        A[ej->v][ej->u] += ej->delta;
    }

    int has_solution = Gaussian_Elimination(&A[0][0], totV, B);
    if (!has_solution)
//        printf("******************* NO SOLUTION! *******************\n");
        ;
    else
        for (i=0; i<totV; i++)
            res[i] = B[i];

    clock_t end = clock();
    return (double) (end - begin) / CLOCKS_PER_SEC;
}

#endif //RMC_TREEWIDTH_CODE_GAUSSIAN_ELIM_H
