//
// Created by kiarash on 9/26/19.
//

//TODO: input assumptions:
/* Each equation is given as pairs of variable index and coeficient
 * In each equation, the index of the constant is -1
 * When entring an edge, user should specify the equations corresponding to that edge
 * There MUST be an edge between ALL pairs of vertices in each bag (even if they are together in 0 equations)
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

void input()
{

    FILE *fp = fopen("/home/kiarash/Desktop/rmc/rmc-treewidth-code/Main-SLE/example.txt", "r");
    int i, j;

    /* read the equations */
    fscanf(fp, "%d%d", &eqCnt, &varCnt);
    printf("%d %d\n",eqCnt,varCnt);
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
    printf("%d %d\n",totV,totE);
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

void remove_vertex(bag *b,int v)
{
    vector conEq;
    vector_init(&conEq);
    int i,j;
    int vIndex=get_vertex_index(b,v);

    /* make conEq which should contain all the indices of eqs
     * having x_v
     */
    for(i=0;i<b->verCnt;i++)
    {
        edge *ej=b->edges[vIndex][i];
        vector *eqPointer=&(ej->eqs);
        for(j=0;j<vector_total(eqPointer);j++)
        {
            int ind=*(int *) vector_get(eqPointer, j);
            if (!removed[ind])
                vector_add(&conEq, (int *) vector_get(eqPointer, j));
        }
    }

    vector ort=k_independent(&conEq, b);
    //get ort[0], make x_v=something, replace x_v with something in ort[1...], done


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
    dfs(0,-1);
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