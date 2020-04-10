//
// Created by kiarash on 9/27/19.
//

#ifndef RMC_TREEWIDTH_CODE_DERIVE_INDEPENDENT_EQS_H
#define RMC_TREEWIDTH_CODE_DERIVE_INDEPENDENT_EQS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../commons/vector.h"
#include "sle_treedec.h"
#include "gaussian_elimination.h"

extern vector eqInd[MAXN],eqCoef[MAXN];
extern bool removed[MAXN];



bool has_solution(float Ap[MAX_TREEWIDTH+1][MAX_TREEWIDTH+2]){
//    float A[MAX_TREEWIDTH+1][MAX_TREEWIDTH+2];
    float A[MAX_TREEWIDTH+1][MAX_TREEWIDTH+1];
    float B[MAX_TREEWIDTH+1];
    int i, j, k;

    for (i=0; i<MAX_TREEWIDTH+1; i++)
        for (j = 0; j < MAX_TREEWIDTH + 1; j++)
            A[i][j] = Ap[i][j];

    for (i=0; i<MAX_TREEWIDTH+1; i++)
        B[i] = Ap[i][MAX_TREEWIDTH+1];

    return Gaussian_Elimination(&A[0][0], MAX_TREEWIDTH+1, B);
}

// returns k independent equations
vector k_independent(vector *conEq, bag *b)
{
    // conEq eqations are in the same bag and none of them are removed yet
    // i.e. removed[conEqs...]=false
    vector res;
    vector_init(&res);

    float A[MAX_TREEWIDTH + 1][MAX_TREEWIDTH + 2];
    memset(A, 0, sizeof(A));

    int i, j;
    int *first = malloc(sizeof(int));
    *first = *(int *) vector_get(conEq, 0);
    vector_add(&res, first);

    int ACnt=1, varCnt=b->verCnt;
    //storing the first vector in A
    for (i=0; i<vector_total(eqInd+*first); i++)
    {
        int var=*(int *)vector_get(eqInd+*first,i);
        int ind=get_vertex_index(b,var);
        if (var==-1)
            ind=varCnt;
        A[ind][0] = *(float *) vector_get(eqCoef+*first, i);
    }

    for(i=1;i<vector_total(conEq);i++)
    {
        int *ind = malloc(sizeof(int));
        *ind=*(int *)vector_get(conEq,i);

        if(ACnt==varCnt)
        {
            removed[*ind]=true;
            continue;
        }

        for (j=0; j<vector_total(eqInd+*ind); j++)
        {
            int var=*(int *)vector_get(eqInd+*ind,j);
            if (var==-1)
                A[varCnt][MAX_TREEWIDTH+1]=*(float *)vector_get(eqCoef+*ind,j); // We define that x_-1 will be in the last row
            else
            {
                int index=get_vertex_index(b,var);
                A[index][MAX_TREEWIDTH+1]=*(float *)vector_get(eqCoef+*ind,j);
            }
        }

        if(has_solution(A))
        {
            removed[*ind]=true;
            for(j=0;j<=varCnt;j++)
                A[j][MAX_TREEWIDTH+1]=0;
        }
        else
        {
            vector_add(&res,ind);
            for(j=0;j<vector_total(eqInd+*ind);j++)
            {
                int var=*(int *)vector_get(eqInd+*ind,j);
                if (var==-1)
                    A[varCnt][ACnt]=*(float *)vector_get(eqCoef+*ind,j);
                else
                {
                    int index=get_vertex_index(b,var);
                    A[index][ACnt]=*(float *)vector_get(eqCoef+*ind,j);
                }
            }
            for(j=0;j<=varCnt;j++)
                A[j][MAX_TREEWIDTH+1]=0;
            ACnt++;
        }
    }

    return res;
}

#endif //RMC_TREEWIDTH_CODE_DERIVE_INDEPENDENT_EQS_H
