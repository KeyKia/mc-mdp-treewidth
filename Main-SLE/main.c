//
// Created by kiarash on 9/26/19.
//

//TODO: input assumptions:
/* Each equation is given as pairs of variable index and coeficient
 * In each equation, the index of the constant is -1
 * When entering an edge, user should specify the equations corresponding to that edge
 * There MUST be an edge between ALL pairs of vertices in each bag (even if they are together in 0 equations)
 * Variable indexes don't have to be sorted
 * First bag MUST contain exactly one vertex
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "vector.h"
#include "tree_decomposition.h"
#include "gram_schmidt.h"
#include "derive_independent_eqs.h"

int eqCnt, varCnt; //number of equations and variables
int eqSize[MAXN];
vector eqInd[MAXN],eqCoef[MAXN];
bool removed[MAXN];
vector e; //contains all possible edges of each bag (even with 0 prob)
bag bags[MAXN]; //the bags
int totV,totE; // vertices, edges
int ansInd[MAXN]; // ansInd[v] is the index of the equation from which x_v shall be finally calculated
int ansVal[MAXN]; // final solutions
vector removed_order;

void input()
{

    FILE *fp = fopen("/home/kiarash/Desktop/rmc/rmc-treewidth-code/Main-SLE/example.txt", "r");
    int i, j;

    /* read the equations */
    fscanf(fp, "%d%d", &eqCnt, &varCnt);
    for (i=0; i<eqCnt; i++)
    {
        vector_init(eqInd+i);
        vector_init(eqCoef+i);
        fscanf(fp,"%d",&eqSize[i]);
        int k=eqSize[i];
        for(j=0;j<k;j++)
        {
            int *ind=malloc(sizeof(int));
            float *f=malloc(sizeof(float));
            fscanf(fp,"%d%f",ind,f);
            vector_add(eqInd+i,ind);
            vector_add(eqCoef+i,f);
        }
    }

    /* read the primal graph */

    fscanf(fp, "%d%d", &totV, &totE);
    vector_init(&e);
    //get edges
    for(i=0;i<totE;i++)
    {
        edge *ej = malloc(sizeof(edge));
        vector_init(&(ej->eqs));
        fscanf(fp, "%d%d", &(ej->v), &(ej->u));
        int k;
        fscanf(fp,"%d",&k);
        for(j=0;j<k;j++)
        {
            int *x=malloc(sizeof(int));
            fscanf(fp,"%d",x);
            vector_add(&(ej->eqs),x);
        }

        vector_add(&e, ej);
        edge_print(ej);
    }

    /* read the tree decomposition of the primal graph */

    // read vertices and edges within bags
    int n;
    fscanf(fp,"%d",&n);
    for(i=0;i<n;i++)
    {
        bag_init(bags+i,i);
        int V,E;
        fscanf(fp,"%d%d",&V,&E);

        // read vertices within bag i
        for(j=0;j<V;j++)
        {
            int *x=malloc(sizeof(int));
            fscanf(fp,"%d",x);
            bag_add_vertex(bags+i,x);
        }

        // read edges within bag i
        for(j=0;j<E;j++)
        {
            int ind;
            fscanf(fp,"%d",&ind);
            edge *ej=vector_get(&e,ind);
            bag_add_edge(bags+i,ej);
        }
    }

    // read the tree edges between bags
    // assumptions: root is bag 0 and contains the target
    // and edges are from parent(v) to child(u)
    for(i=0;i+1<n;i++)
    {
        int *v=malloc(sizeof(int)),*u=malloc(sizeof(int));
        fscanf(fp,"%d%d",v,u);
        bag_add_kid(bags+*v,bags+*u);
    }
    fclose(fp);

}

void update_edges(bag *b, int ind)
{
    int i, j, k;
    for (i=0; i<b->verCnt; i++)
        for (j=0; j<b->verCnt; j++)
        {
            if (i==j)
                continue;
            // Update the edge between the i-th and j-th vertex of this bag
            bool f1=false, f2=false;
            for (k=0; k<vector_total(eqInd+ind); k++)
            {
                int var=*(int *)vector_get(eqInd+ind, k);
                if (var==*get_vertex(b, i))
                    f1=true;
                if (var==*get_vertex(b, j))
                    f2=true;
            }
            if(!f1 || !f2) //either the i-th or j-th vertex is not in the ind-th equation
                continue;
            edge *ej=b->edges[i][j];
            bool f3=false; //true if ind is already in equations associated with this edge
            for(k=0;k<vector_total(&(ej->eqs));k++)
                if(*(int *)vector_get(&(ej->eqs),k)==ind)
                    f3=true;
            if(!f3)
            {
                int *tmp_ind=malloc(sizeof(int));
                *tmp_ind=ind;
                vector_add(&(ej->eqs), tmp_ind);
            }
        }
}

void remove_vertex(bag *b, int v)
{
    //add the vertex to removed_order
    int *v_tmp=malloc(sizeof(int));
    *v_tmp=v;
    vector_add(&removed_order, v_tmp);

    vector conEq; // All equations that contain x_v
    vector_init(&conEq);

    int i,j,k;
    int vIndex=get_vertex_index(b,v); // Index of v in bag b

    /* make conEq which should contain all the indices of eqs
     * having x_v
     */

    for(i=0;i<b->verCnt;i++)
    {
        if (i==vIndex || varRemoved[*get_vertex(b, i)])
            continue;
        edge *ej=b->edges[vIndex][i];
        vector *eqPointer=&(ej->eqs);
        for(j=0;j<vector_total(eqPointer);j++)
        {
            int ind=*(int *) vector_get(eqPointer, j);
            if (!removed[ind])
                vector_add(&conEq, (int *) vector_get(eqPointer, j));
        }
    }

    vector indeps=k_independent(&conEq, b);

    //get indeps[0], make x_v=something, replace x_v with something in indeps[1...], done
    int ind0=*(int *)vector_get(&indeps, 0);
    ansInd[v]=ind0; //The equation with which we can compute x_v in the backward pass
    float *coefV=malloc(sizeof(float));
    // Taking the coeficient of v to divide ind0 by that
    for (i=0; i<vector_total(eqInd+ind0); i++)
    {
        int *var=malloc(sizeof(int));
        *var=*(int *)vector_get(eqInd+ind0, i);
        float *coef=malloc(sizeof(float));
        *coef=*(float *)vector_get(eqCoef+ind0,i);
        if (*var==v)
        {
            *coefV = *coef;
            break;
        }

    }
    // replacing
    for (i=1; i<vector_total(&indeps); i++)
    {
        int indi=*(int *)vector_get(&indeps, i);

        // Finding the coef of x_v in indeps[i]th equation
        float *coefVindi = malloc(sizeof(float));
        for (j=0; j<vector_total(eqCoef+indi); j++)
        {
            int *var=malloc(sizeof(int));
            *var=*(int *)vector_get(eqInd+indi, j);
            float *coef=malloc(sizeof(float));
            *coef=*(float *)vector_get(eqCoef+indi,j);
            if (*var==v)
            {
                *coefVindi = *coef;
                break;
            }
        }

        // replacing
        for (j=0; j<vector_total(eqInd+ind0); j++)
        {
            int *var=malloc(sizeof(int));
            *var=*(int *)vector_get(eqInd+ind0, j);
            float *coef=malloc(sizeof(float));
            *coef=*(float *)vector_get(eqCoef+ind0,j);
            *coef*=-1;
            *coef /= *coefV; // Dividing the whole equation by the coef of x_v in ind0
            *coef *= *coefVindi; // Multiplying by the coef of x_v in indi

            bool found=false;
            for (k=0; k<vector_total(eqInd+indi); k++)
                if (*(int *)vector_get(eqInd+indi, k) == *var)
                {
                    found = true;
                    if (*var==v)
                        *(float *)vector_get(eqCoef+indi, k) = 0;
                    else
                        *(float *) vector_get(eqCoef + indi, k) += *coef;
                }
            if (!found && *coef!=0)
            {
                vector_add(eqInd+indi, var);
                vector_add(eqCoef+indi, coef);
            }
        }
        // no need to take care of possible zeros produced
        // but some edges might be added as some coefs are becoming non-zero
        update_edges(b, indi);
    }
    removed[ind0]=true;
    varRemoved[v]=true;
}

void dfs(int curIndex,int parIndex)
{

    int i;
    bag_print(bags+curIndex);
    for(i=0;i<(bags+curIndex)->kidCnt;i++)
    {
        int ind=get_kid(bags+curIndex,i)-bags;
        if(ind!=parIndex)
            dfs(ind,curIndex);
    }

    if(parIndex!=-1)
        while(get_first_extra_vertex(bags+curIndex,bags+parIndex)!=-1)
        {
            int v=get_first_extra_vertex(bags+curIndex,bags+parIndex);
            remove_vertex(bags+curIndex,v);
        }
}

void solve()
{
    memset(ansInd, -1, sizeof(ansInd));
    memset(ansVal, -1, sizeof(ansVal));
    vector_init(&removed_order);
    dfs(0,-1);
    int i,j,k;
    for (i=0;i<eqCnt;i++)
        if (!removed[i])
        {

            for(j=0;j<vector_total(eqInd+i);j++)
            {
                printf("%d %f\n",*(int *)vector_get(eqInd+i,j),*(float *)vector_get(eqCoef+i,j));
            }
            printf("************\n");
        }
    for (i=vector_total(&removed_order)-1; i>=0; i--)
    {
        int var=*(int *)vector_get(&removed_order, i);
        int ind=ansInd[var];

        printf("\nCalculate x_%d from this equation:\n", var);
        if (ind==-1)
        {
            printf("-1 -1 -1 ridiiiii\n");
            continue;
        }
        for (j=0; j<vector_total(eqInd+ind); j++)
            printf("%fx_%d ", *(float *)vector_get(eqCoef+ind, j), *(int *)vector_get(eqInd+ind, j));

    }
}

void show_results()
{

}

int main()
{
    input();
    solve();
    show_results();
    return 0;
}