//
// Created by kiarash on 9/24/19.
//

//TODO: input assumptions:
/* Root is bag 0 and contains exactly two vertices including the target
 * Edges of the tdc are from parent (v) to child (u)
 * All possible edges of each bag are given in edges (even with 0 prob)
 * (and even all the loops - which might have 0 prob)
 * All edges are given in both directions (even with 0 prob)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "vector.h"
#include "tree_decomposition.h"

vector e; //contains all possible edges of each bag (even with 0 prob)
vector e_copy; //backup of e
bag bags[MAXN]; //the bags
bag bags_copy[MAXN]; //backup of the bags
int totV,totE,target; // vertices, edges, and target in the Markov chain

void input()
{

    FILE* fp=fopen("/home/kiarash/Desktop/rmc/rmc-treewidth-code/Main-MC/example.txt","r");
    int i,j;

    //read totV
    fscanf(fp, "%d%d%d", &totV, &totE, &target);

    vector_init(&e);
    vector_init(&e_copy);
    //get edges
    for(i=0;i<totE;i++)
    {
        edge *ej = malloc(sizeof(edge));
        fscanf(fp, "%d%d%f", &(ej->v), &(ej->u), &(ej->prob));
        vector_add(&e, ej);

        edge *ej_copy = malloc(sizeof(edge));
        ej_copy->v = ej->v;
        ej_copy->u = ej->u;
        ej_copy->prob = ej->prob;
        vector_add(&e_copy,ej_copy);
    }

    // read vertices and edges within bags
    int n;
    fscanf(fp,"%d",&n);
    for(i=0;i<n;i++)
    {
        bag_init(bags+i,i);
        bag_init(bags_copy+i,i);
        int V,E;
        fscanf(fp,"%d%d",&V,&E);

        // read vertices within bag i
        for(j=0;j<V;j++)
        {
            int *x=malloc(sizeof(int));
            fscanf(fp,"%d",x);
            bag_add_vertex(bags+i,x);

            int *y=malloc(sizeof(int));
            *y=*x;
            bag_add_vertex(bags_copy+i,y);

        }

        // read edges within bag i
        for(j=0;j<E;j++)
        {
            int ind;
            fscanf(fp,"%d",&ind);
            edge *ej=vector_get(&e,ind);
            bag_add_edge(bags+i,ej);

            edge *ej_copy=vector_get(&e_copy,ind);
            bag_add_edge(bags_copy+i,ej_copy);
        }
    }

    //read the tree edges between bags
    // assumptions: root is bag 0 and contains the target
    // and edges are from parent(v) to child(u)
    for(i=0;i+1<n;i++)
    {
        int *v=malloc(sizeof(int)),*u=malloc(sizeof(int));
        fscanf(fp,"%d%d",v,u);
        bag_add_kid(bags+*v,bags+*u);

        int *vt=malloc(sizeof(int)),*ut=malloc(sizeof(int));
        *vt=*v;
        *ut=*u;
        bag_add_kid(bags_copy+*vt,bags_copy+*ut);
    }
    fclose(fp);

}


/*first removes all its kids without changing hitting
  probabilities of the remaining part
  then removes the current bag
*/
void dfs_up(int curIndex,int parIndex)
{
    int i;
    for(i=0;i<bags[curIndex].kidCnt;i++)
        if(get_kid(bags+curIndex,i)-bags!=parIndex)
            dfs_up(get_kid(bags+curIndex,i)-bags,curIndex);
    if(parIndex!=-1)
    {
        while(get_first_extra_vertex(bags + curIndex, bags + parIndex)!=-1)
        {
            int v=get_first_extra_vertex(bags+curIndex,bags+parIndex);
            remove_vertex(bags+curIndex,v);
        }
    }

}

/*starting from root which has only two vertices (including
 * target), returns all vertices in each bag in the reversed
 * order they were removed in dfs_up. While returning computes
 * the correct hitting probability of the returned vertex
 */
void dfs_down(int curIndex,int parIndex)
{
    int i;
    for(i=0;i<bags_copy[curIndex].kidCnt;i++)
    {

        bag *tmp=get_kid(bags_copy+curIndex,i);
        int kidIndex=tmp->id;
        if(kidIndex==parIndex)
            continue;
        while(get_last_extra_vertex(get_kid(bags_copy+curIndex,i),bags_copy+curIndex)!=-1)
        {
            int v=get_last_extra_vertex(get_kid(bags_copy+curIndex,i),bags_copy+curIndex);
            return_vertex(get_kid(bags_copy+curIndex,i),v);
        }

        dfs_down(kidIndex,curIndex);
    }

}


void solve()
{
    memset(res,0,sizeof(res));
    dfs_up(0,-1);
    printf("\ndfs up finished -- two-vertex root bag now:\n");
    bag_print(bags);
    init_res(bags,target);
    memset(removed,0,sizeof(removed));
    dfs_down(0,-1);
}

void show_results()
{
    int i;
    for(i=0;i<totV;i++)
        printf("Hitting prob of vertex %d: %f\n", i, res[i]);
}

int main()
{
    input();
    solve();
    show_results();
    return 0;
}
