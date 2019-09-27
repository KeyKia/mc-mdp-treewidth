//
// Created by kiarash on 9/27/19.
//

#ifndef RMC_TREEWIDTH_CODE_GAUSSIAN_ELIMINATION_H
#define RMC_TREEWIDTH_CODE_GAUSSIAN_ELIMINATION_H


#include "tree_decomposition.h"
#include <stdio.h>
#include <math.h>

// function to reduce matrix to r.e.f.  Returns a value to
// indicate whether matrix is singular or not
int forwardElim(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1]);

// function to calculate the values of the unknowns
void backSub(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1]);

// function to get matrix content
bool gaussianElimination(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1])
{
    /* reduction into r.e.f. */
    int singular_flag = forwardElim(mat);

    /* if matrix is singular */
    if (singular_flag != -1)
    {
//        printf("Singular Matrix.\n");

        /* if the RHS of equation corresponding to
           zero row  is 0, * system has infinitely
           many solutions, else inconsistent*/
        if (mat[singular_flag][MAX_TREEWIDTH])
            return false;
//            printf("Inconsistent System.");
        else
            return true;
//            printf("May have infinitely many "
//                   "solutions.");

    }

    /* get solution to system and print it using
       backward substitution */
//    backSub(mat);
    return true;
}

// function for elementary operation of swapping two rows
void swap_row(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1], int i, int j)
{
    //printf("Swapped rows %d and %d\n", i, j);
    int k;
    for (k=0; k<=MAX_TREEWIDTH; k++)
    {
        float temp = mat[i][k];
        mat[i][k] = mat[j][k];
        mat[j][k] = temp;
    }
}

// function to print matrix content at any stage
void print(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1])
{
    int i,j;
    for (i=0; i<MAX_TREEWIDTH; i++, printf("\n"))
        for (j=0; j<=MAX_TREEWIDTH; j++)
            printf("%lf ", mat[i][j]);

    printf("\n");
}

// function to reduce matrix to r.e.f.
int forwardElim(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1])
{
    int i,j,k;
    for (k=0; k<MAX_TREEWIDTH; k++)
    {
        // Initialize maximum value and index for pivot
        int i_max = k;
        float v_max = mat[i_max][k];

        /* find greater amplitude for pivot if any */
        for (i = k+1; i < MAX_TREEWIDTH; i++)
            if (fabs(mat[i][k]) > v_max)
                v_max = mat[i][k], i_max = i;
        printf("%d %f \n",i_max,v_max);
        /* if a prinicipal diagonal element  is zero,
         * it denotes that matrix is singular, and
         * will lead to a division-by-zero later. */
        if (!mat[k][i_max])
            return k; // Matrix is singular

        /* Swap the greatest value row with current row */
        if (i_max != k)
            swap_row(mat, k, i_max);


        for (i=k+1; i<MAX_TREEWIDTH; i++)
        {
            /* factor f to set current row kth element to 0,
             * and subsequently remaining kth column to 0 */
            float f = mat[i][k]/mat[k][k];

            /* subtract fth multiple of corresponding kth
               row element*/
            for (j=k+1; j<=MAX_TREEWIDTH; j++)
                mat[i][j] -= mat[k][j]*f;

            /* filling lower triangular matrix with zeros*/
            mat[i][k] = 0;
        }

        //print(mat);        //for matrix state
    }
    //print(mat);            //for matrix state
    return -1;
}

// function to calculate the values of the unknowns
void backSub(float mat[MAX_TREEWIDTH][MAX_TREEWIDTH+1])
{
    double x[MAX_TREEWIDTH];  // An array to store solution

    /* Start calculating from last equation up to the
       first */
    int i,j,k;
    for (i = MAX_TREEWIDTH-1; i >= 0; i--)
    {
        /* start with the RHS of the equation */
        x[i] = mat[i][MAX_TREEWIDTH];

        /* Initialize j to i+1 since matrix is upper
           triangular*/
        for (j=i+1; j<MAX_TREEWIDTH; j++)
        {
            /* subtract all the lhs values
             * except the coefficient of the variable
             * whose value is being calculated */
            x[i] -= mat[i][j]*x[j];
        }

        /* divide the RHS by the coefficient of the
           unknown being calculated */
        x[i] = x[i]/mat[i][i];
    }

    printf("\nSolution for the system:\n");
    for (i=0; i<MAX_TREEWIDTH; i++)
        printf("%lf\n", x[i]);
}

#endif //RMC_TREEWIDTH_CODE_GAUSSIAN_ELIMINATION_H
