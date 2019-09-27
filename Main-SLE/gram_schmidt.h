//
// Created by kiarash on 9/26/19.
//

#include "vector.h"

#ifndef RMC_TREEWIDTH_CODE_GRAM_SCHMIDT_H
#define RMC_TREEWIDTH_CODE_GRAM_SCHMIDT_H

extern vector eqInd[MAXN],eqCoef[MAXN];
extern bool removed[MAXN];


float dot_product(vector *aInd,vector *aCoef,vector *bInd,vector *bCoef)
{

}

void subtract(int ind1,int ind2,float scale)
{
    /* Subtract each scaled component of eq[ind2] from eq[ind1] */
    int i,j;
    for(i=0;i<vector_total(eqInd+ind2);i++)
    {
        int var=*(int *)vector_get(eqInd+ind2,i);
        float coef=*(float *)vector_get(eqCoef+ind2,i);

        coef*=scale;

        int ind=-1;
        for(j=0;j<vector_total(eqInd+ind1);j++)
            if(*(int *)vector_get(eqInd+ind1,j)==var)
                ind=j;
        if(ind==-1)
        {
            int *tmpInd = malloc(sizeof(int));
            *tmpInd = var;

            float *tmpCoef = malloc(sizeof(float));
            *tmpCoef = 0;
            vector_add(eqInd + ind1, tmpInd);
            vector_add(eqCoef + ind1, tmpCoef);
            ind=vector_total(eqInd+ind1)-1;
        }
        *(float *)vector_get(eqCoef+ind1,ind)-=coef;
        //TODO: what happens if after -=coef it becomes zero
    }
}

bool is_all_zero(int ind)
{

}

/* Find an orthonormal basis for the set of vectors q
 * using the Gram-Schmidt Orthogonalization process */

vector gram_schmidt(vector *conEq)
{
    vector res;
    vector_init(&res);

    int i,j,k;
    for(i=0;i<vector_total(conEq);i++)
    {
        int ind=*(int *)vector_get(conEq,i);

        for(j=0;j<vector_total(&res);j++)
        {
            int ind2=*(int *)vector_get(&res,j);
            float scaling_factor=dot_product(eqInd+ind,eqCoef+ind,eqInd+ind2,eqCoef+ind2);
            scaling_factor/=dot_product(eqInd+ind2,eqCoef+ind2,eqInd+ind2,eqCoef+ind2);

            /* subtracting res[j]*scaling_factor from ind-th equation*/
            subtract(ind,ind2,scaling_factor);

            if(!is_all_zero(ind))
                vector_add(&res,&ind);
            else
                removed[ind]=true;
        }
    }
    return res;
}

#endif //RMC_TREEWIDTH_CODE_GRAM_SCHMIDT_H
